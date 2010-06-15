#include "type.h"
#include "arch/x86/timer.h"
#include "arch/x86/isr.h"
#include "arch/x86/io_bus.h"
#include "lib/kernel/framebuf.h"
#include "lib/stdio.h"

/* 50 hZ */
static const uint32 timer_freq = 50;

static uint32 ticks = 0;

/* Our timer tick callback function */
static void timer_tick (struct registers* regs);

void
init_timer ()
{
    asm volatile ("sti");

    /* Register the timer callback. */
    isr_reg_func (IRQ0, &timer_tick);

    /* We will send this value to the PIC based on the timer frequency we
       desire. */
    uint32 divisor = 1193180 / timer_freq;

    /* Send the command byte. */
    outb (0x43, 0x36);

    /* Send the frequency divisor. */
    outb (0x40, (uint8) divisor & 0xFF);
    outb (0x40, (uint8) (divisor >> 8) & 0xFF);

    asm volatile ("cli");

    printf ("Timer initialized \n");
};

void
timer_tick (struct registers* regs)
{
    ticks++;
//    framebuf_printf ("tick: %u \n", ticks);
};
