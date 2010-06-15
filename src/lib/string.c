#include "lib/string.h"
#include "type.h"

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
