#include "arch/x86/isr.h"
#include "arch/x86/io_bus.h"
#include "type.h"
#include "lib/string.h"
#include "lib/stdio.h"

static isr_func* isr_list[256];

void
isr_init ()
{
    memset (isr_list, 0, 256 * sizeof (void*));
};

void 
isr_handler (struct registers regs)
{
	framebuf_printf ("recieved interrupt: %x\n", regs.int_no);

	/* Call the correct interrupt service routine here. */
	if (isr_list[regs.int_no])
        isr_list[regs.int_no] (&regs);	
}

void irq_handler(struct registers regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        outb(slavepic, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(masterpic, 0x20);

	/* Call the correct interrupt service routine here. */
	if (isr_list[regs.int_no])
		isr_list[regs.int_no] (&regs);
};

void 
isr_reg_func (enum isr_num num, isr_func* func)
{
	isr_list[num] = func;
};

