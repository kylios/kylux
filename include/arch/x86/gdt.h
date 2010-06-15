#ifndef GDT_H
#define GDT_H

#include "type.h"

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
struct gdt_entry
{
   uint16 limit_low;       // The lower 16 bits of the limit.
   uint16 base_low;      // The lower 16 bits of the base.
   uint8 base_middle;      // The next 8 bits of the base.
   uint8 access;           // Access flags, determine what ring this segment can be used in.
   uint8  granularity;
   uint8  base_high;       // The last 8 bits of the base.
} __attribute__((packed));

struct gdt_ptr
{
   uint16 limit;           // The upper 16 bits of all selector limits.
   uint32 base;            // The address of the first gdt_entry_t struct
}
__attribute__((packed));

/* Initializes the GDT. */
void init_gdt (); 

/* Lets us access our ASM functions from our C code. */
extern void gdt_flush (uint32);



#endif // GDT_H
