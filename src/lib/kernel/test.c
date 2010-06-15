// src/lib/kernel/test.c

//    src/lib/kernel/test.c is part of Kylux.
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
// Date: 2010-02-14 13:06

#include "type.h"
#include "common.h"
#include "debug.h"
#include "lib/kernel/test.h"
#include "lib/kernel/framebuf.h"
#include "lib/stdio.h"
#include "lib/stdarg.h"

void 
test_msg (const char* format, ...)
{
    va_list arg;

    ASSERT (format != NULL);    

    va_start (arg, format);
    vprintf (format, arg);
    va_end (arg);
};


