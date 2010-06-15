// include/arch/x86/pagedir.h

//    include/arch/x86/pagedir.h is part of Kylux.
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

#ifndef ARCH_X86_PAGEDIR_H
#define ARCH_X86_PAGEDIR_H

#include "type.h"

typedef uint32 pte;


/*
   Structure of x86 page tables: 
   (Diagram taken from Pintos: threads/pte.h)

    31                  22 21                  12 11                   0
   +----------------------+----------------------+----------------------+
   | Page Directory Index |   Page Table Index   |     Page Offset      |
   +----------------------+----------------------+----------------------+
*/


/*
    FROM THE INTEL x86 MANUAL
    SECTION 4.3
Bit         Contents
Position(s)
0 (P)       Present; must be 1 to map a 4-KByte page

1 (R/W)     Read/write; if 0, writes may not be allowed to the 4-KByte 
            page referenced by this entry (depends on CPL and CR0.WP; 
            see Section 4.6)

2 (U/S)     User/supervisor; if 0, accesses with CPL=3 are not allowed 
            to the 4-KByte page referenced by this entry 
            (see Section 4.6)

3 (PWT)     Page-level write-through; indirectly determines the memory 
            type used to access the 4-KByte page referenced by this entry
            (see Section 4.9)

4 (PCD)     Page-level cache disable; indirectly determines the memory 
            type used to access the 4-KByte page referenced by this entry
            (see Section 4.9)

5 (A)       Accessed; indicates whether software has accessed the 
            4-KByte page referenced by this entry (see Section 4.8)

6 (D)       Dirty; indicates whether software has written to the 4-KByte
            page referenced by this entry (see Section 4.8)

7 (PAT)     If the PAT is supported, indirectly determines the memory 
            type used to access the 4-KByte page referenced by this entry
            (see Section 4.9); otherwise, reserved (must be 0)1

8 (G)       Global; if CR4.PGE = 1, determines whether the translation 
            is global (see Section 4.10); ignored otherwise

11:9        Ignored

31:12       Physical address of the 4-KByte page referenced by this entry

*/


#define PTE_BITS    (12)
#define PTE_SHIFT   PTE_BITS

#define PTE_FLAGS      (0x00000FFF)
#define PTE_P          (0x01 << 0)
#define PTE_RW         (0x01 << 1)
#define PTE_US         (0x01 << 2)
#define PTE_PWT        (0x01 << 3)
#define PTE_PCD        (0x01 << 4)
#define PTE_A          (0x01 << 5)
#define PTE_D          (0x01 << 6)
#define PTE_PAT        (0x01 << 7)
#define PTE_G          (0x01 << 8)
#define PTE_PADDR      (0xFFFFF000)

#define PTE_TO_PADDR(PTE)   \
    ((uint8*) (PTE & PTE_PADDR))

/* Given a virtual address, returns the index in the page table
   where the page table will be that contains the mapping. */
#define PTE_NO(VADDR) \
    ((pte) ((VADDR) & BITMASK(PTE_SHIFT, PTE_BITS) >> PDE_SHIFT))

/* Structure describing a page table */
struct page_table
{
    pte table[1024];
};

typedef uint32 pde;


/*
   FROM THE INTEL x86 MANUAL
   SECTION 4.3
Bit         Contents
Position(s)
0 (P)       Present; must be 1 to reference a page table

1 (R/W)     Read/write; if 0, writes may not be allowed to the 4-MByte 
            region controlled by this entry (depends on CPL and CR0.WP; 
            see Section 4.6)

2 (U/S)     User/supervisor; if 0, accesses with CPL=3 are not allowed 
            to the 4-MByte region controlled by this entry 
            (see Section 4.6)

3 (PWT)     Page-level write-through; indirectly determines the memory 
            type used to access the page table referenced by this entry 
            (see Section 4.9)

4 (PCD)     Page-level cache disable; indirectly determines the memory 
            type used to access the page table referenced by this entry 
            (see Section 4.9)

5 (A)       Accessed; indicates whether this entry has been used for 
            linear-address translation (see Section 4.8)

6           Ignored

7 (PS)      If CR4.PSE = 1, must be 0 (otherwise, this entry maps a 
            4-MByte page; see Table 4-4); otherwise, ignored

11:8        Ignored

31:12       Physical address of 4-KByte aligned page table referenced 
            by this entry
*/

#define PDE_BITS        10
#define PDE_SHIFT       (PDE_BITS + PTE_BITS)

#define PDE_PADDR       (0xFFFFF000)
#define PDE_PS          (0x01 << 7)
#define PDE_A           (0x01 << 5)
#define PDE_PCD         (0x01 << 4)
#define PDE_PWT         (0x01 << 3)
#define PDE_US          (0x01 << 2)
#define PDE_RW          (0x01 << 1)
#define PDE_P           (0x01 << 0)

/* Return a physical address from a PDE */
#define PDE_TO_PADDR(PDE)   \
    ((struct page_table*) (PDE & PDE_PADDR))

/* Given a virtual address, returns the index in the page directory where
   the page table will be that contains the mapping. */
#define PDE_NO(VADDR) \
    ((pde) ((VADDR) >> PDE_SHIFT))

/* Structure describing a page directory */
struct pagedir
{
    /* These are our page table entries */
    pde dir[1024];
};

/* Sets up the kernel's page directory */
void init_pagedir ();

/* Initilizes a new page directory at the address pointed to by PD.
   PD MUST be page-aligned!! */
struct pagedir* pagedir_init (struct pagedir* pd);

/* Switches page directories.  Passing NULL will switch back to the 
   kernel's page directory */
void pagedir_switch (struct pagedir* pd);

/* Destoys the specified page directory and frees all the pages used by
   the page tables */
void pagedir_destroy (struct pagedir* pd);

/* Finds and returns the page table (PDE) that contains a mapping to
   virtual address VADDR.  Will create the page table if it doesn't 
   exist and if CREATE is true. */
pde pagedir_get_pde (struct pagedir*, void* vaddr, bool create);

/* Finds and returns the physical address mapped to the given virtual
   address.  Returns NULL if the virtual address is not mapped.  The
   given virtual address must fall within the given page table. */
pte pagedir_get_pte (struct page_table*, void* vaddr);

/* Maps a virtual address to a physical address in the specified page 
   table. */
void pagedir_map (struct pagedir* pd, void* vaddr, void* paddr, bool write);

void pagedir_unmap (struct pagedir* pd, void* vaddr);



#endif  // ARCH_X86_PAGEDIR_H
