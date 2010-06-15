#ifndef ARCH_X86_FRAMEBUF_H
#define ARCH_X86_FRAMEBUF_H

#include "type.h"

#define FB_WIDTH    80
#define FB_HEIGHT   25

enum fbcolor
{
    FBBLACK = 0,
    FBBLUE = 1,
    FBGREEN = 2,
    FBCYAN = 3,
    FBRED = 4,
    FBMAGENTA = 5,
    FBBROWN = 6,
    FBLGREY = 7,
    FBDGREY = 8,
    FBLBLUE = 9,
    FBLGREEN = 10,
    FBLCYAN = 11,
    FBLRED = 12,
    FBLMAGENTA = 13,
    FBLBROWN = 14,
    FBWHITE = 15
};

#define FB_BYTE(BG, FG)  \
    ((uint8) BG << 4) | ((uint8) FG & 0x0F)

#define FB_OFFSET(X, Y) \
    ((int) ((Y * FB_WIDTH) + X))

#define FB_WORD(C, BG, FG) \
    ((uint16) (((uint16) C) | ((uint16) (FB_BYTE(BG, FG) << 8))))

/* Writes WORD to offset OFF in video_memory */
void fb_write_word (uint16 word, uint16 off);
/* Reads word from offset OFF in video_memory */
uint16 fb_read_word (uint16 off);
/* Moves the cursor to position (x, y) */
void fb_move_cursor (uint16 x, uint16 y);


#endif // ARCH_X86_FRAMEBUF_H
