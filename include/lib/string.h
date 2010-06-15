#ifndef STRING_H
#define STRING_H

#include "type.h"

void* memcpy (void* destination, const void* source, size_t num);
void* memmove (void* destination, const void* source, size_t num);
char* strcpy (char* destination, const char* source);
char* strncpy (char* destination, const char* source, size_t num);

char* strcat (char* destination, const char* source);
char* strncat (char* destination, const char* source, size_t num);

int memcmp (const void* ptr1, const void* ptr2, size_t num);
int strcmp (const char* str1, const char* str2);
int strncmp (const char* str1, const char* str2, size_t num);

void* memset (void* addr, int c, size_t n);

#endif // STRING_H

