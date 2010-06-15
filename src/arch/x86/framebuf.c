#include "arch/x86/framebuf.h"
#include "arch/x86/io_bus.h"
#include "type.h"

/* The BIOS says we need to write here to output to the framebuffer */
static uint16* video_memory = (uint16*) 0xB8000;


void 
fb_write_word (uint16 word, uint16 off)
{
    video_memory[off] = word;
};

uint16
fb_read_word (uint16 off)
{
    return video_memory[off];
};

void
fb_move_cursor (uint16 x, uint16 y)
{
    uint16 loc = y * FB_WIDTH + x;

    /* Send high byte */
    outb (fbcontrol, 14);
    outb (fbdata, loc >> 8);
    /* Send low byte */
    outb (fbcontrol, 15);
    outb (fbdata, loc);
};
