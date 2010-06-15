// src/kernel/heap.c

//    src/kernel/heap.c is part of Kylux.
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

#include "type.h"
#include "common.h"
#include "debug.h"
#include "kernel/heap.h"

/* 
   This malloc implementation (for now) will serve as the heap algorithm
   for both the kernel heap and for process heaps.  We have the ability
   to specify a start address and a size, and the heap will be created in
   that memory space.  Only the first page of the heap space should be
   pre-allocated -- all other pages will be allocated as necessary.

   The allocator is a combination of Doug Lea's malloc and a first-fit
   linear allocator.  For all sizes <= 512 bytes, we will allocate using
   the first-fit algorithm from a set of dedicated pages (if needed, more
   pages will be obtained).  For greater sized requests, we keep a 
   dedicated page for requests of size 1024, 2048, 4096 and higher.  
   All requests are rounded up to the next highest power of two.

*/

struct h_pool
{
    struct list free_list;
};

struct heap
{
    size_t sz;
    
    uint8* start;
};

struct free_header
{

};

struct used_header
{

};

void
heap_init (uint8* heap_start, size_t sz)
{
    /* We need to create a heap in the given memory area given by 
       HEAP_START.  */
    struct heap* h = (struct heap*) heap_start;

    h->start = heap_start + sizeof (struct heap);
    h->sz = sz - sizeof (struct heap);
};

