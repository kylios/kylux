// include/lib/kernel/kmalloc.h

//    include/lib/kernel/kmalloc.h is part of Kylux.
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
// Date: 2010-01-28 18:23

#ifndef LIB_KERNEL_KMALLOC_H
#define LIB_KERNEL_KMALLOC_H

#include "type.h"
#include "lib/kernel/list.h"
#include "kernel/spinlock.h"

#define POOL_MAGIC  0xADAB817F
#define USED_MAGIC  0xFEA41EAF

struct pool
{
    struct list list;
    struct spinlock lock;
};

struct free_block
{
    struct list_elem elem;      /* Goes in list for a pool */
    size_t size;                /* Size of block including header */
};

struct used_block
{
    size_t size;
    int magic;
    uint8 data;
};

void init_kmalloc ();

void* kmalloc_allocate (size_t size);

void kmalloc_free (void* ptr);


#endif  // LIB_KERNEL_KMALLOC_H
