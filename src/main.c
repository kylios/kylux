#include "lib/stdlib.h"
#include "lib/stdio.h"
#include "lib/string.h"
#include "lib/kernel/framebuf.h"
#include "lib/kernel/kmalloc.h"
#include "kernel/frame_mgr.h"
#include "kernel/paging.h"
#include "kernel/thread.h"
#include "arch/x86/gdt.h"
#include "arch/x86/idt.h"
#include "arch/x86/pagedir.h"
#include "arch/x86/timer.h"
#include "multiboot.h"
#include "type.h"
#include "debug.h"
#include "common.h"
#include "main.h"
#include "test/test.h"

/* Holds our command line parameters for the kernel */
static struct koptions kernel_options = KOPTION_DEFAULTS;

/* The number of available frames in the system */
uint32 num_frames;
/* The number of frames available to the user and the kernel 
   respectively */
uint32 user_frames, kernel_frames;

/* Declared in our linker script as the first available address past the
   BSS that we can use. */
extern uint32 end;
/* The start of our available memory.  This will be
   ROUND_UP (&end, PAGE_SIZE) */
uint32* free_start;

/* Parse the command line and store the options in the koptions struct */
static void parse_command_line (unsigned long cmdline, struct koptions*);

/* Initialize our BSS segment by zero'ing the whole thing out. */ 
static void init_bss ();

extern struct pagedir* kernel_dir;
 
/*
 * Testing thread_create.  REMOVE WHEN DONE 
 */
static int test_thread_create_func (void*);


int 
main (multiboot_info_t *mboot_ptr, int magic)
{
    init_thread ();

    framebuf_init ();
    framebuf_clear ();
    framebuf_printf ("Welcome to Kylux!\n");

    /* Clear the BSS */
    init_bss ();

    /* Maybe one day we won't have to rely on multiboot compliance, but
       for now that's not my primary concern.  If you can't install grub
       on the partition, then I don't know what you're doing using a 
       computer... */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        printf ("Kylux not booted by a multiboot compliant bootloader.\n");
        printf ("  Expected magic value: %x \n", MULTIBOOT_HEADER_MAGIC);
        printf ("  Received magic value: %x \n", magic);
        return 1;
    }
    /* Handle command line */
    parse_command_line (mboot_ptr->cmdline, &kernel_options);
 
    /* Descriptor Tables */
    init_gdt ();
    init_idt ();

    /* Round our available memory up to a page boundary */
    free_start = (uint32*) ROUND_UP (((uint32) &end), PAGE_SIZE);
    printf ("Start of available memory: %p \n", free_start);
    printf ("Upper memory available (KiB): %u \n", 
            (unsigned) mboot_ptr->mem_upper);

    /* Our bootloader should report the available upper memory in KiB */
    /* GRUB gives upper memory (past 1MiB) in KiB while END is reported
       as an address.  Therefore, MEM_UPPER must be divided by 4 and
       END must be divided by 4096 in order to get the number of frames*/
    num_frames = (mboot_ptr->mem_upper / 4) - 
        (((uint32) (free_start) - 0x100000) / 4096);
    printf ("%u available frames of memory \n", num_frames);

    /* It should be safe to run tests at this point */
//#ifdef TEST_KERNEL
//    run_kernel_tests ();
//#endif // TEST_KERNEL

    /* Here we must decide how much memory to give to the kernel and to
       user programs.  This should be controlled via kernel parameters */
    kernel_frames = kernel_options.kernel_frames;
    user_frames = num_frames - kernel_frames;
    if (user_frames > kernel_options.user_frames)
        user_frames = kernel_options.user_frames;


    /* Initialize our memory management routines.  After this, we should
       have a working frame allocator and a kernel page directory.  */
    init_frame_mgr (user_frames, kernel_frames);
    init_kmalloc ();
    init_pagedir ();
    init_paging ();

    /* NOTE TO SELF */
    // PAGE FAULTING WORKS UP TO THIS POINT AND WE HAVE A PAGE DIR
    // THAT MAPS THE FIRST 8MiB (??) 

    /* Initialise the hardware timer */
    init_timer ();

    /* Initilize threading system and start kernel + idle threads */
    // init_threads ();


    /* Enable interrupts */
//    asm volatile ("sti");
    start_threading ();
    

    // TODO: start the kernel here
    tid_t tid = thread_create (PRI_MED, test_thread_create_func, NULL);

    return 0xDEADBABA;
} 


void 
test_isr_1 (void)
{
    framebuf_write ("Testing ISR function 1 \n");   
};

void 
test_isr_2 (void)
{
    framebuf_write ("Testing ISR function 2 \n");   
};

int 
test_thread_create_func (void* aux)
{
    framebuf_printf ("I am speaking to you from a new thread! \n");

    return 55;
};

void test_isr_3 (void)
{
    framebuf_write ("Testing ISR function 3 \n");   
};

static void
parse_command_line (unsigned long cmdline, struct koptions* options)
{

};

static void
init_bss ()
{
    extern uint32 _bss;
    uint8* bss = (uint8*) &_bss;

    memset (bss, 0, (uint32) (((uint8*) (&end)) - bss));
};
