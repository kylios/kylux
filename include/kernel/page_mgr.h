// include/kernel/page_mgr.h

//    include/kernel/page_mgr.h is part of Kylux.
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

#ifndef KERNEL_PAGE_MGR_H
#define KERNEL_PAGE_MGR_H

#include "type.h"
#include "common.h"
#include "kernel/pagemap.h"

/*
   The page manager is different from the frame manager in that it
   allocates virtual pages rather than physical frames.  The page 
   manager can be thought of as the next layer of abstraction on top 
   of the frame manager.  If a page request is made, the page manager
   will attempt to allocate a frame and map it in the given page 
   directory, but if no frames are available, then the page manager 
   will attempt to swap out a physical frame.  

   */

void init_page_mgr ();

/* Allocates a new page, returning the virtual address of the page, or
   NULL if the allocation failed.  The user must provide a page directory
   and flags indicating a user page (us=TRUE) and read/write (rw=TRUE).*/
void* page_mgr_alloc (struct pagemap* pm, void* vaddr, bool us, bool rw);

/* Frees the specified page. */
void page_mgr_free (struct pagemap* pm, void* vaddr);

#endif  // KERNEL_PAGE_MGR_H
