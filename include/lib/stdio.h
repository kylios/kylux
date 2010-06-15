#ifndef LIB_STDIO_H
#define LIB_STDIO_H

#include "lib/stdarg.h"


int sprintf (char* buf, const char* format, ...);
int vsprintf (char* buf, const char* format, va_list arg);

int printf (const char* format, ...);
int vprintf (const char* format, va_list arg);

int framebuf_printf (const char* format, ...);

#endif  // LIB_STDIO_H

