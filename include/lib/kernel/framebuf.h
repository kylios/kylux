// include/lib/kernel/framebuf.h

//    include/lib/kernel/framebuf.h is part of Kylux.
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

#ifndef LIB_KERNEL_FRAMEBUF_H
#define LIB_KERNEL_FRAMEBUF_H

#include "arch/x86/framebuf.h"

#define DEFAULT_TAB_WIDTH 8

/* Initialize the framebuffer */
void framebuf_init ();

/* Clear the screen */
void framebuf_clear ();

/* Write C to the screen */
void framebuf_put_char (char c);
/* Write STR to the screen */
void framebuf_write (char* str);

/* Printf to the framebuffer */
int framebuf_printf (const char* format, ...);

/* Set the colors of the framebuffer */
void framebuf_set_bg (enum fbcolor bg);
void framebuf_set_fg (enum fbcolor fg);
/* Get the colors of the framebuffer */
enum fbcolor framebuf_get_bg ();
enum fbcolor framebuf_get_fg ();

#endif  // LIB_KERNEL_FRAMEBUF_H
