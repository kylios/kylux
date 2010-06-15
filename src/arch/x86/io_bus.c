#include "arch/x86/io_bus.h"
#include "type.h"

void 
outb (enum ioport port, uint8 value)
{
    asm volatile ("outb %1, %0" : : "dN" ((uint16) port), "a" (value));
};

uint8 
inb (enum ioport port)
{
    uint8 ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" ((uint16) port));
    return ret; 
};

uint16 
inw (enum ioport port)
{
    uint16 ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" ((uint16) port));
    return ret;
};

