// include/kernel/pagemap.h

//    include/kernel/pagemap.h is part of Kylux.
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
// Date: 2010-02-05 11:49

#ifndef KERNEL_PAGEMAP_H
#define KERNEL_PAGEMAP_H

#include "type.h"
#include "common.h"
#include "kernel/lock.h"
#include "lib/kernel/hash.h"
#include "arch/x86/pagedir.h"

/* Specifies the location of a particular page on the system. */
enum pageloc
{
    PAGELOC_MEMORY = 1,
    PAGELOC_SWAP = 2,
    PAGELOC_FILE = 4,
    PAGELOC_EVICTING = 8
};

/* Aside from the architecture-specific aspects of our page directory, 
   we must also be able to keep track of the location of different pages
   other than just main memory.  Pages may be swapped to disk or other
   locations, or they may be read-only pages that can be physically
   destroyed, or they can be memory-mapped files.  All this information
   must be stored in a hash data structure so that we can look it up
   during a page fault. */
struct pagemap
{
    /* The hardware page directory */
    struct pagedir* pd;

    /* Supplementary information about this process's address space */
    struct hash pm;

    /* Keep accesses synchronized */
    struct lock lock;
};

/* Detailed info about where a given page is on the system, and keeps 
   track of its information */
struct pagemap_map
{
    struct hash_elem elem;

    uint8* vaddr;
    /* Tells us whether the page is in memory, a file, or swap space */
    enum pageloc loc;
    /* Details where exactly the page is */
    union pagemap_info
    {
        uint8* phys_addr;
        uint32 swap_number;
        uint32 file_offset;
        int val;
    } info;
};

/* Initializes the pagemap */
void pagemap_init (struct pagemap* pm, struct pagedir* pd);
/* Maps a virtual address to a page on the system */
uint8* pagemap_map (struct pagemap* pm, struct pagemap_map* m);
/* Unmaps a virtual address */
struct pagemap_map* pagemap_unmap (struct pagemap* pm, void* vaddr);



#endif  // KERNEL_PAGEMAP_H
