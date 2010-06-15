// src/lib/kernel/framebuf.c

//    src/lib/kernel/framebuf.c is part of Kylux.
//
//    Kylux is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Kylux is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Kylux.  If not, see <http://www.gnu.org/licenses/>.

// Author: Kyle Racette
// Date: 2010-01-24 16:46

#include "lib/kernel/framebuf.h"
#include "arch/x86/framebuf.h"
#include "kernel/spinlock.h"

static int cur_x = 0, cur_y = 0;
static struct spinlock fb_lock;

static enum fbcolor FG = FBWHITE;
static enum fbcolor BG = FBBLACK;
static uint8 TAB_WIDTH = DEFAULT_TAB_WIDTH;

static void scroll ();

void
framebuf_init ()
{
    spinlock_init (&fb_lock);
};

void 
framebuf_clear ()
{
    uint16 cleared = FB_WORD (' ', BG, FG);
    unsigned i;

    spinlock_acquire (&fb_lock);
    for (i = 0; i < FB_HEIGHT * FB_WIDTH; i++)
        fb_write_word (cleared, i);

    cur_x = 0;
    cur_y = 0;

    fb_move_cursor (cur_x, cur_y);
    spinlock_release (&fb_lock);
};

void 
framebuf_put_char (char c)
{
    spinlock_acquire (&fb_lock);

    /* Backspace */
    if (c == 0x08 && cur_x)
        cur_x--;
    /* Tab */
    else if (c == 0x09)
        cur_x = (cur_x + TAB_WIDTH) & ~(TAB_WIDTH - 1);
    else if (c == '\r')
        cur_x = 0;
    else if (c == '\n')
    {
        cur_x = 0;
        cur_y++;
    }
    else if (c >= ' ')
    {
        uint16 toprint = FB_WORD (c, BG, FG);
        fb_write_word (toprint, FB_OFFSET (cur_x, cur_y));
        cur_x++;
    }

    if (cur_x >= FB_WIDTH)
    {
        cur_x = 0;
        cur_y++;
    }

    scroll ();
    fb_move_cursor (cur_x, cur_y);

    spinlock_release (&fb_lock);
};

void 
framebuf_write (char* str)
{
    char c;
    while ((c = *str++) != '\0')
        framebuf_put_char (c);
};

void 
framebuf_set_bg (enum fbcolor bg)
{
    BG = bg;    
};

void 
framebuf_set_fg (enum fbcolor fg)
{
    FG = fg;
};

enum fbcolor 
framebuf_get_bg ()
{
    return BG;
};

enum fbcolor 
framebuf_get_fg ()
{
    return FG;
};


static void
scroll ()
{
    if (cur_y >= FB_HEIGHT)
    {
        unsigned i;
        for (i = 0; i < FB_WIDTH * FB_HEIGHT - FB_WIDTH; i++)
            fb_write_word (fb_read_word(i + FB_WIDTH), i);
        /* Fill in the remaining empty line */
        uint16 clear = FB_WORD (' ', BG, FG);
        for (i = FB_WIDTH * FB_HEIGHT - FB_WIDTH; 
                i < FB_WIDTH * FB_HEIGHT; i++)
            fb_write_word (clear, i);
        cur_y = FB_HEIGHT - 1;
    }
};
