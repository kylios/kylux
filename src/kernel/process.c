// src/kernel/process.c

//    src/kernel/process.c is part of Kylux.
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
// Date: 2010-02-16 14:16

#include "type.h"
#include "common.h"
#include "debug.h"
#include "kernel/process.h"
#include "kernel/frame_mgr.h"
#include "kernel/page_mgr.h"
#include "arch/x86/pagedir.h"

void
init_process ()
{
    struct process* p;
    struct pagedir* pd;

    /* The goal of this function is to create a kernel process and 
       continue our initialization as an instance of that process.  
       This involves setting up the page directory, setting the stack
       pointer to 0xc0000000, and adding this process to the process
       list. */

    /* Get a page in which to set up the struct */
    p = (struct process*) frame_mgr_alloc (1, false, true);
    if (p == NULL)
        PANIC ("Could not allocate enough kernel memory for initialization!");
    pd = frame_mgr_alloc (1, false, false);
    if (pd == NULL)
        PANIC ("Could not allocate enough kernel memory for initialization!");

    /* Page directory setup */
    p->magic = PROCESS_MAGIC;
    pagedir_init (pd);
//    heap_init (&p->heap);
//    pagemap_init (&p->pm);
};

struct process*
process_init (struct process* p)
{

    return NULL;
};
