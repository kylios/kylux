#include "lib/string.h"
#include "type.h"
#include "debug.h"

void*
memset (void* _ptr, int _val, size_t _len)
{
    uint8* ptr = (uint8*) _ptr;
    int val = _val;
    size_t len = _len;

    if (ptr)
    {
        while (len--)
            *ptr++ = (uint8) val;
    }

    return ptr;
};

char* 
strcpy (char* destination, const char* source)
{
    char* dest = destination;
    char* src = source;
    char* d = dest;
    char* s = src;

    ASSERT (dest != NULL);
    ASSERT (src != NULL);

    while (*s != '\0')
    {
        *d++ = *s++;
    }
    *d = '\0';

    return dest;
};

char* 
strncpy (char* destination, const char* source, size_t num)
{
    char* dest = destination;
    char* src = source;
    char* d = dest;
    char* s = src;
    int count = 0;

    ASSERT (dest != NULL);
    ASSERT (src != NULL);

    while (*s != '\0' && count++ < num)
    {
        *d++ = *s++;
    }
    *d = '\0';

    return dest;
};

int 
strlen (const char* str)
{
    char* c = str;
    int count = 0;

    ASSERT (str != NULL);

    while (*c++ != '\0')
    {
        count++;
    }

    return count;
};

int 
strnlen (const char* str, size_t num)
{
    char* c = str;
    int count = 0;

    ASSERT (str != NULL);

    while (*c++ != '\0' && count < num)
    {
        count++;
    }

    return count;
};
