#ifndef ISR_H
#define ISR_H

#include "type.h"

#define IRQ0	32
#define IRQ1	33

enum isr_num
{
    ISR_DIVIDE_BY_ZERO = 0,
    ISR_DEBUG_EXCEPTION = 1,
    ISR_NMI_INTERRUPT = 2,
    ISR_BREAKPOINT_EXCEPTION = 3,
    ISR_OVERFLOW_EXCEPTION = 4,
    ISR_BOUND_RANGE_ECXEEDED_EXCEPTION = 5,
    ISR_INVALID_OPCODE_EXCEPTION = 6,
    ISR_DEVICE_NOT_AVAILABLE_ECXEPTION = 7,
    ISR_DOUBLE_FAULT_EXCEPTION = 8,
    ISR_COPROCESSOR_SEGMENT_OVERRUN = 9,
    ISR_INVALID_TSS_EXCEPTION = 10,
    ISR_SEGMENT_NOT_PRESENT_EXCEPTION = 11,
    ISR_STACK_FAULT_EXCEPTION = 12,
    ISR_GENERAL_PROTECTION_EXCEPTION = 13,
    ISR_PAGE_FAULT_EXCEPTION = 14,
    ISR_X87_FLOATING_POINT_ERROR = 16,
    ISR_ALIGNMENT_CHECK_EXCEPTION = 17,
    ISR_MACHINE_CHECK_EXCEPTION = 18,
    ISR_SIMD_FLOATING_POINT_EXCEPTION = 19
};

struct registers
{
   uint32 ds;                  // Data segment selector
   uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32 int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
}; 

typedef void isr_func (struct registers* regs);

void isr_init ();
void isr_reg_func (enum isr_num num, isr_func* func);

#endif // ISR_H

