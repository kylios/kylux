// src/kernel/page_mgr.c

//    src/kernel/page_mgr.c is part of Kylux.
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
// Date: 2010-02-05 11:21

#include "type.h"
#include "common.h"
#include "debug.h"
#include "kernel/page_mgr.h"
#include "kernel/frame_mgr.h"
#include "lib/kernel/kmalloc.h"
#include "kernel/interrupt.h"

/* Brings this page into memory such that a page fault should no longer 
   occur on this page */
static void page_mgr_make_avail (uint8* vaddr);

/* Evict a page to free one in physical memory */
static bool evict_page ();

void 
init_page_mgr ()
{
};

void* 
page_mgr_alloc (struct pagemap* pm, void* vaddr, bool us, bool rw)
{
    void* paddr = NULL;

    ASSERT (pm != NULL);
    ASSERT (vaddr != NULL);

    while (paddr == NULL)
    {
        paddr = frame_mgr_alloc (1, us, true);
        if (paddr == NULL)
        {
            if (us == false)
            {
                /* Kernel pages cannot be evicted */
                PANIC ("Out of kernel memory");
            }

            /* We need to evict a page */
            if (!evict_page ())
            {
                // TODO panic??
                PANIC ("could not evict a page");
            }
        }
    }

    /* Set up the entry in the page map */
    struct pagemap_map* map = 
         kmalloc_allocate (sizeof (struct pagemap_map));
//    struct pagemap_map* map;
    if (map == NULL)
    {
        frame_mgr_free (paddr);
        return NULL;
    }
    /* Map the page from VADDR to memory address PADDR */
    map->vaddr = vaddr;
    map->loc = PAGELOC_MEMORY;
    map->info.phys_addr = paddr;

    pagemap_map (pm, map);

    return paddr;
};

void 
page_mgr_free (struct pagemap* pm, void* vaddr)
{
    /* Create a dummy mapping so we can look up VADDR */
    /*
    struct pagemap_map m = {
        HASH_ELEM_INITIALIZER,
        vaddr,
        PAGELOC_MEMORY,
        {0}
    };
    */

    ASSERT (pm != NULL);
    ASSERT (vaddr != NULL);

    struct pagemap_map* mp = pagemap_unmap (pm, vaddr);
    // free (mp);
};

static void 
page_mgr_make_avail (uint8* vaddr)
{
    
};

static bool
evict_page ()
{

    return false;
};
