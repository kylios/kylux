// src/lib/kernel/kmalloc.c

//    src/lib/kernel/kmalloc.c is part of Kylux.
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

#include "lib/kernel/kmalloc.h"
#include "kernel/frame_mgr.h"
#include "lib/stdio.h"
#include "kernel/spinlock.h"
#include "type.h"
#include "debug.h"
#include "common.h"
#include "kernel/interrupt.h"

static struct pool pools[11];

static struct pool* pool_from_size (size_t size);
static int 
free_block_compare_func (struct list_elem*, struct list_elem*, void*);

void init_kmalloc ()
{
    unsigned i;

    for (i = 0; i < 11; i++)
    {
        list_init (&pools[i].list);
        spinlock_init (&pools[i].lock);
    }
};

void*
kmalloc_allocate (size_t size)
{
    size_t rounded_size;
    struct pool* pool;
    struct list_elem* e;
    struct used_block* ub;
    struct free_block* fb;

    if (size == 0)  return NULL;

    size += sizeof (struct used_block);
    rounded_size = (size_t) round_up_power_2 ((int) size);

    /* If the requested size is greater than 1 KiB, then we do something
       special. */
    if (rounded_size > 2048)
    {

        return NULL;
    }

    pool = pool_from_size (rounded_size);
    ub = NULL;

    spinlock_acquire (&pool->lock);

    /* Now search the list for free blocks.  If none exist, then we will
       allocate a new page and add it to the list. */
    while (list_empty (&pool->list))
    {
        spinlock_release (&pool->lock);

        /* Get a new frame of memory and zero it out to be safe */
        fb = (struct free_block*) frame_mgr_alloc (1, false, true);
        if (fb == NULL)
        {
            PANIC ("Kernel out of memory for kmalloc!");
        }

        /* The page needs to be broken up into chunks of SIZE bytes, 
           where SIZE is the size that this pool services */
        uint8* ptr = (uint8*) fb;
        struct free_block* end_fb = (struct free_block*) 
            (ptr + PAGE_SIZE);

        spinlock_acquire (&pool->lock);
        while (fb < end_fb)
        {
            fb = (struct free_block*) ptr;
            fb->size = rounded_size;
            list_push_back (&pool->list, &fb->elem);

            ptr += rounded_size;
        }
        ASSERT (fb == end_fb);
    }

    /* Get the first available free block */
    e = list_pop_front (&pool->list);
    fb = LIST_ENTRY (e, struct free_block, elem);

    /* Just double check that our logic is right */
    ASSERT (fb->size == rounded_size);

    list_remove (e);
    spinlock_release (&pool->lock);

    ub = (struct used_block*) fb;
    ub->size = fb->size;
    ub->magic = USED_MAGIC;

    return (void*) &ub->data;
};

void 
kmalloc_free (void* ptr)
{
    struct used_block* ub;
    struct free_block* fb;
    struct pool* pool;

    ASSERT (ptr != NULL);
    /* Get a used_block struct from the pointer */
    ub = (struct used_block*) (((uint8*) ptr) - 
            offsetof (struct used_block, data));
    ASSERT (ub->magic == USED_MAGIC);
    /* Turn it into a free block struct */
    fb = (struct free_block*) ub;
    fb->size = ub->size;
    ub = NULL;

    /* Figure out the pool */
    pool = pool_from_size (fb->size);

    spinlock_acquire (&pool->lock);
    list_insert_ordered (&pool->list, &fb->elem, 
            &free_block_compare_func, NULL);
    spinlock_release (&pool->lock);
};

static struct pool* 
pool_from_size (size_t size)
{
    ASSERT (size != 0);
    /* Size should be a power of 2 */

    /* Determine the log2(rounded_size), which will be the index
       for the array POOLS */
    register unsigned int r = (size & 0xAAAAAAAA) != 0;
    r |= ((size & 0xFFFF0000) != 0) << 4;
    r |= ((size & 0xFF00FF00) != 0) << 3;
    r |= ((size & 0xF0F0F0F0) != 0) << 2;
    r |= ((size & 0xCCCCCCCC) != 0) << 1;

    return &pools[r];
};

static int 
free_block_compare_func (struct list_elem* a, struct list_elem* b, void*
        aux)
{
    ASSERT (a != NULL);
    ASSERT (b != NULL);

    return ((uint32) a) - ((uint32) b);
};

