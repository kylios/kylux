#ifndef ARCH_X86_IDT_H
#define ARCH_X86_IDT_H   

#include "type.h"

/* An interrupt gate */
struct idt_entry
{
   uint16 base_lo;             /* The lower 16 bits of the address 
								  to jump to when this interrupt fires */
   uint16 sel;                 /* Kernel segment selector */
   uint8  always0;             /* This must always be zero */
   uint8  flags;               /* More flags. See documentation */
   uint16 base_hi;             /* The upper 16 bits of the address 
								  to jump to */
} __attribute__((packed));

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr
{
   uint16 limit;
   struct idt_entry* base;
} __attribute__((packed));

void init_idt ();
void idt_finalize (uint32 addr);

void isr0 ();
void isr1 ();
void isr2 ();
void isr3 ();
void isr4 ();
void isr5 ();
void isr6 ();
void isr7 ();
void isr8 ();
void isr9 ();
void isr10 ();
void isr11 ();
void isr12 ();
void isr13 ();
void isr14 ();
void isr15 ();
void isr16 ();
void isr17 ();
void isr18 ();
void isr19 ();
void isr20 ();
void isr21 ();
void isr22 ();
void isr23 ();
void isr24 ();
void isr25 ();
void isr26 ();
void isr27 ();
void isr28 ();
void isr29 ();
void isr30 ();
void isr31 ();

void irq0 ();
void irq1 ();
void irq2 ();
void irq3 ();
void irq4 ();
void irq5 ();
void irq6 ();
void irq7 ();
void irq8 ();
void irq9 ();
void irq10 ();
void irq11 ();
void irq12 ();
void irq13 ();
void irq14 ();
void irq15 ();

#endif // IDT_H

