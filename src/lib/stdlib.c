#include "lib/stdlib.h"
#include "lib/string.h"

static char ch_lower[17] = "0123456789abcdef";
static char ch_upper[17] = "0123456789ABCDEF";


static void write_to_buf (char c, void* output);

/*  ITOA is a non-standard function, but the common implementation is to
    convert VAL to a string using BASE.  If BASE is 10, then VAL is 
    treated as a signed integer.  Otherwise, it is treated as an unsigned
    integer.  STR must be large enough to store the converted string plus
    an additional NULL terminator.  */
int
itoa (int val, char* _str, int base)
{
    char* str = _str;
    int sign = 0;
    if (base == 10) sign = 1;
    return _itoa (val, str, base, sign);
};

int
_itoa (int val, char* _str, int base, int sign)
{
    return __itoa (&write_to_buf, _str, val, base, sign, 8, 1); 
};


int 
__itoa (output* func, void* _buf, int val, 
        int base, int sign, int width, int lower)
{
    char* buf = _buf;

    /* This buffer is used to reverse the string when we're done with it. */
    char revbuf[32];
    memset (revbuf, 0, 32);
    char* bptr = revbuf;

    /* The number of characters we've printed thus far. */
    int num = 0;
    unsigned int value;
    if (sign && val < 0)
    {
        /* Compute the absolute value.
           This works by right-shifting the bits so that we have a mask
           of all 1's or all 0's depending on the sign bit.  We XOR the 
           original value by that, then subtract the value of the 
           original sign bit.  The reason this works is because if the
           original value was positive, the sign bit will be 0, thus 
           XOR'ing by 32 0's then subtracting by 0 will do nothing.  
           However, if the original value was negative, then we flip all
           the bits by XOR'ing by 32 1's and adding 1 (since it is
           two's compliment).   */
        register int mask = val >> (sizeof (int) * 8 - 1);
        value = (val ^ mask) - mask;
    }
    else
        value = (unsigned int) val;

    while (value >= base)
    {
        int rem = value % base;
        value /= base;
        *bptr++ = (lower ? ch_lower[rem] : ch_upper[rem]);
        num++;
    }
    *bptr++ = (lower ? ch_lower[value] : ch_upper[value]);
    num++;
    if (base == 10 && val < 0)
    {
        *bptr++ = '-';
        num++;
    }
    bptr--;
    
    /* reverse the string */
    int t = num;
    while (t--)
    {
        func (*bptr--, buf++);
    }
    return num;
};

static void 
write_to_buf (char c, void* output)
{
    *((char*) output) = c;    
};
