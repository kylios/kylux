// include/kernel/frame_mgr.h

//    include/kernel/frame_mgr.h is part of Kylux.
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
// Date: 2010-01-25 21:41

#ifndef KERNEL_FRAME_MGR_H
#define KERNEL_FRAME_MGR_H

#include "type.h"
#include "debug.h"
#include "kernel/frame_mgr.h"

/* Initialize the page manager, specifying the number of user
   and kernel pages to use */
void init_frame_mgr (size_t num_user, size_t num_kernel);

/* Allocates NUM contiguous pages using the provided flags.  Returns
   the physical address of the first page. */
void* frame_mgr_alloc (size_t num, bool user, bool zero);

/* Frees the page specified by the physical address PADDR. */
void frame_mgr_free (void* paddr);

/* For debugging */
void frame_mgr_dump_bitmap (bool user);
    

#endif  // KERNEL_FRAME_MGR_H
