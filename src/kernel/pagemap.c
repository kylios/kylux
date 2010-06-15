// src/kernel/pagemap.c

//    src/kernel/pagemap.c is part of Kylux.
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

#include "type.h"
#include "common.h"
#include "debug.h"
#include "kernel/pagemap.h"
#include "arch/x86/pagedir.h"

static uint32 page_hash (struct hash_elem* e);
static int page_compare (struct hash_elem* a, struct hash_elem* b, 
        void* aux);

void 
pagemap_init (struct pagemap* pm, struct pagedir* pd)
{
    ASSERT (pm != NULL);
    ASSERT (pd != NULL);

    pd = pagedir_init (pd);
    ASSERT (pd != NULL);

    hash_init (&pm->pm, &page_hash, &page_compare);
    lock_init (&pm->lock);
};

uint8* 
pagemap_map (struct pagemap* pm, struct pagemap_map* m)
{
    ASSERT (pm != NULL);
    ASSERT (m != NULL);
    
    lock_acquire (&pm->lock);
    hash_insert (&pm->pm, &m->elem);
    lock_release (&pm->lock);

    return m->vaddr;
};

struct pagemap_map*
pagemap_unmap (struct pagemap* pm, void* vaddr)
{
    struct hash_elem* e;
    struct pagemap_map* mp;
    struct pagemap_map m = 
    {
        HASH_ELEM_INITIALIZER,
        vaddr,
        PAGELOC_MEMORY,
        {0}
    };

    ASSERT (pm != NULL);

    /* Find the record with this virtual address, then delete it if it
       exists.  We return the actual record, since it can be used later
       for eviction (plus we don't have authority to 'free' it since
       we did not allocate it). */
    lock_acquire (&pm->lock);
    e = hash_find (&pm->pm, &m.elem, NULL);
    if (e == NULL)
    {
        lock_release (&pm->lock);
        return NULL;
    }
    hash_remove (&m.elem);
    lock_release (&pm->lock);

    mp = HASH_ENTRY (e, struct pagemap_map, elem);

    return mp;
};


static uint32
page_hash (struct hash_elem* e)
{
    struct pagemap_map* m;

    ASSERT (e != NULL);    

    m = HASH_ENTRY (e, struct pagemap_map, elem);
    return (uint32) (m->vaddr);
};

static int 
page_compare (struct hash_elem* a, struct hash_elem* b, 
        void* aux)
{
    struct pagemap_map* ma;
    struct pagemap_map* mb;

    ASSERT (a != NULL);
    ASSERT (b != NULL);

    ma = HASH_ENTRY (a, struct pagemap_map, elem);
    mb = HASH_ENTRY (b, struct pagemap_map, elem);
    return ma->vaddr - mb->vaddr; 
};


