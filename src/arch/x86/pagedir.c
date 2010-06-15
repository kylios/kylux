// src/arch/x86/pagedir.c

//    src/arch/x86/pagedir.c is part of Kylux.
//
//    Kylux is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Kylux is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Kylux.  If not, see <http://www.gnu.org/licenses/>.

// Author: Kyle Racette
// Date: 2010-01-25 20:36

#include "arch/x86/pagedir.h"
#include "arch/x86/framebuf.h"
#include "kernel/frame_mgr.h"
#include "lib/stdio.h"
#include "debug.h"
#include "type.h"
#include "common.h"

extern uint32 kernel_frames;
extern uint32 free_start;

static struct pagedir* base_dir;
static struct pagedir* kernel_dir;
static struct pagedir* current_dir;

static pde create_pde (void* paddr, uint32 flags);
static pte create_pte (void* paddr, uint32 flags);

void
init_pagedir ()
{
    uint32 i;
    uint8* addr;

    ASSERT (sizeof (struct pagedir) == PAGE_SIZE);
    ASSERT (sizeof (struct page_table) == PAGE_SIZE);
    
    /* Now set up the kernel directory */
    kernel_dir = (struct pagedir*) frame_mgr_alloc (1, false, true);
    framebuf_printf ("base_dir allocated at %p \n", base_dir);

    ASSERT (kernel_dir != NULL);

    /* Map the first 1 GiB */
    addr = 0;
    for (i = 0; i < 1024; i++)
    {
        if (i < 256)
        {
            struct page_table* pt = 
                (struct page_table*) frame_mgr_alloc (1, false, true);
            uint32 j;
            for (j = 0; j < 1024; j++)
            {
                pt->table[j] = create_pte (addr, PTE_RW | PTE_P | PTE_G);
                addr += 0x1000;
            }
            kernel_dir->dir[i] = create_pde (pt, PDE_P | PDE_RW);
        }
        else
            kernel_dir->dir[i] = 0;
    }

    /* First things first is to set up the base page directory.  The
       base page directory will provide the basic setup for all our
       process' page directories, as it will basically be copied into
       our process's page directories when a process is created.  In this
       way, each process will have access to kernel virtual memory.
       Mappings will be such that 0xc0000000 (virt) == 0x00000000 (phys)
       */
    base_dir = (struct pagedir*) frame_mgr_alloc (1, false, true);
    ASSERT (base_dir != NULL);

    for (i = 0; i < 1024; i++) 
    {
        if (i < 768)
            base_dir->dir[i] = NULL;
        else
            base_dir->dir[i] = kernel_dir->dir[i - 768];
    }
};

struct pagedir*
pagedir_init (struct pagedir* pd)
{
    ASSERT (pd != NULL);

    /* Now we will map each page of the process's page directory
       from address 0xc0000000 till the end of memory to the 
       equivalent page tables in the kernel's page directory. 
       This will be done by copying the page table pointers from the
       kernel page directory over to the current page directory
       being initialized.  */
    *pd = *base_dir;
    return pd;
};

void 
pagedir_switch (struct pagedir* pd)
{
    if (pd == NULL)
    {
        pd = kernel_dir;
    }
    ASSERT (pd != NULL);
    ASSERT ((((uint32) pd) % 4096 == 0));

    /* First update cr3, the page directory pointer */    
    asm volatile ("movl %0, %%cr3" : : "r" (pd));

    /* Our current page directory pointer must also be updated */
    current_dir = pd;

    /* Enable paging by setting the most significant bit in cr0  */
    asm volatile (
        "movl %cr0, %eax \n\t"
        "orl $0x80000000, %eax\n\t"
        "movl %eax, %cr0");
};

void 
pagedir_destroy (struct pagedir* pd)
{
    ASSERT (pd != NULL);

    /* Clear the whole thing */
    unsigned i = 0;
    while (i < 1024)
    {
        frame_mgr_free ((void*) pd->dir[i]);
        pd->dir[i] = 0x0000000;
        i++;
    }
};

pde
pagedir_get_pde (struct pagedir* pd, void* vaddr, bool create)
{
    ASSERT (pd != NULL);    
    ASSERT (vaddr != NULL);

    uint32 pde_idx = PDE_NO((uint32) vaddr);
    pde entry = pd->dir[pde_idx];
    if (entry == NULL && create)
    {
        /* Create a page table here */
        struct page_table* pt = 
            (struct page_table*) frame_mgr_alloc (1, false, true);
        pd->dir[pde_idx] = create_pde (pt, PDE_P | PDE_RW);
        entry = pd->dir[pde_idx];
    }

    return entry;
};

pte
pagedir_get_pte (struct page_table* pt, void* vaddr)
{
    ASSERT (pt != NULL);
    ASSERT (vaddr != NULL);

    uint32 pte_idx = PTE_NO((uint32) vaddr);
    pte entry = pt->table[pte_idx];
    return entry;
};

void
pagedir_map (struct pagedir* pd, void* vaddr, void* paddr, bool writable)
{
    ASSERT (pd != NULL);
    ASSERT (paddr != NULL);
    ASSERT ((((uint32) vaddr) / PAGE_SIZE) * PAGE_SIZE == (uint32) vaddr);
    ASSERT ((((uint32) vaddr) / PAGE_SIZE) * PAGE_SIZE == (uint32) vaddr);

    /* Find the right PDE, create it if it doesn't exist. */
    pde entry = pagedir_get_pde (pd, vaddr, true);
    ASSERT (entry != 0);
    ASSERT (entry & PDE_P);

    struct page_table* pt = (struct page_table*) (entry & PDE_PADDR);
    ASSERT (pt != NULL);

    uint32 pt_idx = PTE_NO((uint32) vaddr);
    pt->table[pt_idx] = 
        create_pte (paddr, PTE_P | PTE_US | (writable && PTE_RW));
};

void
pagedir_unmap (struct pagedir* pd, vaid* vaddr)
{
    ASSERT (pd != NULL);
    ASSERT (vaddr != NULL);

    ASSERT ((((uint32) vaddr) / PAGE_SIZE) * PAGE_SIZE == (uint32) vaddr);

    pde pde_entry = pagedir_get_pde (pd, vaddr, true);
    if (pde_entry == 0)
    {
        return;
    }

    struct page_table* pt = PDE_TO_PADDR(pde_entry);
    pt->table[PTE_IDX(vaddr)] = 0;
};

static pde 
create_pde (void* paddr, uint32 flags)
{
    pde to_ret = (pde) paddr;
    ASSERT (to_ret % 0x1000 == 0);
 
    return (to_ret | flags);
};

static pte 
create_pte (void* paddr, uint32 flags)
{
    pte to_ret = (pte) paddr;
    ASSERT (to_ret % 0x1000 == 0);

    return (to_ret | flags);
};
