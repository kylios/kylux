#include "type.h"
#include "debug.h"
#include "common.h"
#include "kernel/frame_mgr.h"
#include "lib/kernel/bitmap.h"
#include "lib/string.h"
#include "lib/stdio.h"
#include "lib/stdlib.h"

struct memory_pool
{
    bool user;          /* User pages = TRUE */
    uint32 start;       /* starting memory address */
    uint32 num_pages;   /* The number of pages in this pool */
    struct bitmap bmap; /* Bitmap to keep track of used frames */
};

extern uint32 num_frames;
extern uint32* free_start;

static struct memory_pool user_mem, kernel_mem;

void 
init_frame_mgr (size_t num_user, size_t num_kernel)
{
    unsigned i;
	/* We start dealing out frames at addresses past .data, .text, and 
	   .bss.  This is given by the symbol END in our linker script. */
    uint32 mem_start = (uint32) free_start;    
    uint32 num = num_frames;
    uint32 mem_end = mem_start + num * PAGE_SIZE;

    if (num_kernel > num)
        num_kernel = num;
    num_user = num - num_kernel;

    /* Set up our memory pools. */
    printf ("Initializing kernel pool starting at %x with %u pages \n",
            mem_start, num_kernel);
    kernel_mem.user = false;
    kernel_mem.start = mem_start;
    kernel_mem.num_pages = num_kernel;
    bitmap_init (&kernel_mem.bmap, (void*) kernel_mem.start, 
            kernel_mem.num_pages);
    for (   i = 0; 
            i < ROUND_UP (num_kernel, PAGE_SIZE * 8) / (PAGE_SIZE * 8);
            i++)
    {
        bitmap_set (&kernel_mem.bmap, i);
    }

    printf ("Initializing user pool starting at %x with %u pages \n",
            mem_start + kernel_mem.num_pages * PAGE_SIZE
			, num_user);
    user_mem.user = true;
    user_mem.start = mem_start + kernel_mem.num_pages * PAGE_SIZE;
    user_mem.num_pages = num_user;
    bitmap_init (&user_mem.bmap, (void*) user_mem.start, 
            user_mem.num_pages);
    for (   i = 0; 
            i < ROUND_UP (num_user, PAGE_SIZE * 8) / (PAGE_SIZE * 8);
            i++)
    {
        bitmap_set (&user_mem.bmap, i);
    }
};


void* 
frame_mgr_alloc (size_t num, bool user, bool zero)
{
    if (num == 0)   return NULL;

    struct memory_pool* pool = (user ? &user_mem : &kernel_mem);
    uint32 idx = bitmap_scan_and_set (&pool->bmap, num);

    if (idx == BITMAP_ERROR)    return NULL;
	void* to_return = (void*) (pool->start + idx * PAGE_SIZE);
	/* Ensure it's page aligned */
	ASSERT (((uint32) to_return) % 4096 == 0);

    /* Zero it out if requested */
    if (zero)
        memset (to_return, 0, PAGE_SIZE);

    return to_return;
};

void 
frame_mgr_free (void* paddr)
{
    bool user = true;
    uint32 addr = (uint32) paddr;
    if (addr < kernel_mem.start)    return;
    if (addr >= kernel_mem.start && 
        addr < (kernel_mem.start + kernel_mem.num_pages * PAGE_SIZE))
        user = false;

    struct memory_pool* pool = (user ? &user_mem : &kernel_mem);
    uint32 idx = (addr - pool->start) / PAGE_SIZE;
    if (idx > pool->num_pages - 1) return;

    bitmap_clear (&pool->bmap, idx);
};

void 
frame_mgr_dump_bitmap (bool user)
{
	if (user)
	{
		framebuf_printf ("User pool: \n");
		bitmap_dump (&user_mem.bmap);
	}
	else
	{
		framebuf_printf ("Kernel pool: \n");
		bitmap_dump (&kernel_mem.bmap);
	}
};
