#ifndef ARCH_X86_IO_BUS_H
#define ARCH_X86_IO_BUS_H

#include "type.h"

enum ioport
{
    fbcontrol = 0x3D4,
    fbdata = 0x3D5,
    slavepic = 0xA0,
    masterpic = 0x20
};

void outb (enum ioport port, uint8 value);
uint8 inb (enum ioport port);
uint16 inw (enum ioport port);

#endif // ARCH_X86_IO_BUS_H
