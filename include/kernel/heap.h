// include/kernel/heap.h

//    include/kernel/heap.h is part of Kylux.
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
// Date: 2010-02-04 20:35

#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include "type.h"
#include "common.h"

struct heap
{

};

/* Create a new heap in the specified region of memory */
void heap_init (uint8* heap_start, size_t sz);

void* heap_alloc (int bytes);
void heap_free (void* addr);
void heap_realloc (int bytes);

#endif  // KERNEL_HEAP_H
