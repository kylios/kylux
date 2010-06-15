#include "lib/stdio.h"
#include "lib/stdlib.h"
#include "lib/stdarg.h"
#include "lib/kernel/framebuf.h"
#include "debug.h"
#include "type.h"

/* Helper function that will do most of the work.  All characters are
   'printed' using the output function FUNC, with BUF passed as the 
   second parameter. */
static int __vprintf (output* func, void* buf, const char* format, 
        va_list arg);

/* Writes C to BUF. */
static void output_buf (char c, void* buf);
/* Writes C to the framebuffer.  BUF is not used. */
static void output_fb (char c, void* buf);



static void to_upper (char* buf);
static void to_lower (char* buf);
static int is_numeric (char c);


int
framebuf_printf (const char* _format, ...)
{
    const char* format = _format;

    va_list arg;
    va_start (arg, _format);

    char buf;
    int num = __vprintf (&output_fb, &buf, format, arg);

    va_end (arg);

    return num;
};

int
printf (const char* _format, ...)
{
    const char* format = _format;

    va_list arg;
    va_start (arg, _format);

    char buf;
    int num = __vprintf (&output_fb, &buf, format, arg);

    va_end (arg);

    return num;
};

int
vprintf (const char* _format, va_list arg)
{
    const char* format = _format;
    char buf;
    return __vprintf (&output_fb, &buf, format, arg);
};

/*  Prints FORMAT out to BUF, using the additional arguments to write 
    specific data. */
int 
sprintf (char* _buf, const char* _format, ...)
{
    char* buf = _buf;
    const char* format = _format;

    va_list arg;
    va_start (arg, _format);

    int num = vsprintf (buf, format, arg);

    va_end (arg);

    return num;
};

int 
vsprintf (char* _buf, const char* _format, va_list arg)
{
    char* buf = _buf;
    const char* format = _format;
    return __vprintf (&output_buf, buf, format, arg);
};


/**** STATIC FUNCTIONS ****/
//static char sym[11] = "0123456789";

/*  This function performs most of the operations for printf and its
    variants.  It acts as a generic formatting function by allowing
    its caller to pass a reference to a function which is used to handle
    the output.  Thus, printf will supply a function to print a single
    character to the screen, whereas sprintf will provide a function to
    write a single character to a provided buffer.  */
static int 
__vprintf (output* func, void* buf, const char* _format, va_list arg)
{
    char* bptr = buf;
    const char* format = _format;
    const char* fptr = format;

    int num = 0;

    bool flag = false;
    bool width = false;
    bool precision = false;
    bool length = false;
    bool specifier = false;

    char flag_ch = 0;
    bool width_var = false;
    bool width_val = 0;
    bool precision_var = false;
    int precision_val = 0;
    char length_ch = 0;

    char c;
    while ((c = *fptr) != '\0')
    {
        if (specifier)
        {
            fptr++;
            char ch = 0;
            int to_print_i = 0;
            char* strptr = NULL;
            unsigned int n = 0;
            int* nargs = NULL;
            int val = 0;
            switch (c)
            {
            case 'c':   /* Write a single character. */
                func ((char) va_arg (arg, int), bptr++);
                num++;
                break;
            case 'd':   /* Write a signed decimal number. */
            case 'i':
                /* Get the number. */
                to_print_i = va_arg (arg, int);
                val = __itoa (func, bptr, to_print_i, 10, 1, 0, 1);
                num += val;
                bptr += val;
                break;
            case 'e':   /* Scientific notation. */
                break;
            case 'E':   /* Scientific notation. */
                break;
            case 'f':   /* Decimal floating point. */
                break;
            case 'g':
                break;
            case 'G':
                break;
            case 'o':   /* Signed octal. */
                to_print_i = va_arg (arg, int);
                val = __itoa (func, bptr, to_print_i, 8, 1, 0, 1);
                num += val;
                bptr += val;
                break;
            case 's':   /* String. */
                strptr = va_arg (arg, char*);
                while ((ch = *strptr++) != '\0')
                {
                    func (ch, bptr++);
                    num++;
                }
                break;
            case 'u':   /* Unsigned decimal integer. */
                n = va_arg (arg, unsigned int);
                val = __itoa (func, bptr, n, 10, 0, 0, 1);
                num += val;
                bptr += val;
                break;
            case 'p':   /* Pointer address. */
                func ('0', bptr++);
                func ('x', bptr++);
                num += 2;
                /* Fall through since we're just printing the hex. */
            case 'x':   /* Unsigned hexadecimal integer. */
                n = va_arg (arg, unsigned int);
                val = __itoa (func, bptr, n, 16, 0, 0, 1);
                num += val;
                bptr += val;
                break;
            case 'X':   /* Unsigned hexadecimal integer (caps). */
                n = va_arg (arg, unsigned int);
                val = __itoa (func, bptr, n, 16, 0, 0, 0);
                num += val;
                bptr += val;
                break;
            case 'n':   /* Report the number of characters printed.*/
                /* nothing printed. */
                nargs = va_arg (arg, int*);
                *nargs = num;
                break;
            case '%':   /* Just print a '%'. */
                func ('%', bptr++);
                num++;
                break;
            default:
                break;
            }
            specifier = false;
        }
        else if (length)
        {
            specifier = true;
            if (c == 'h' || c == 'l' || c == 'L')
            {
                length_ch = c;
                fptr++;
            }
            length = false;
        }
        else if (precision)
        {
            length = true;
            length_ch = 0;
            if (c == '.')
            {
                c = *++fptr;
                if (c == '*')
                {
                    precision_var = true;
                    fptr++;
                }
                else
                {
                    while (is_numeric (c))
                    {
                        precision_val += (c - 48);
                        c = *++fptr;
                    }
                }
            }
            precision = false;
        }
        else if (width)
        {
            precision = true;
            precision_var = false;
            precision_val = 0;
            if (c == '*')
            {
                width_var = true;
                fptr++;
            }
            else
            {
                while (is_numeric (c))
                {
                    width_val += (c - 48);
                    c = *++fptr;
                }
            }
            width = false;
        }
        else if (flag)
        {
            width = true;
            if (c == '+' || c == '-' || c == ' ' || c == '#' || c == '0')
            {
                flag_ch = c;
                fptr++;
            }
            width_val = 0;
            width_var = false;
            flag = false;
        }
        else if (c == '%')
        {
            flag = true;
            flag_ch = 0;
            fptr++;
        }
        else
        {
            func (*fptr++, bptr++);
            num++;
        }
    }
    func ('\0', bptr);

    return num;
   
};

static void
output_buf (char c, void* buf)
{
    *((char*) buf) = c;
};

static void
output_fb (char c, void* buf)
{
    framebuf_put_char (c);
};


static void 
to_upper (char* buf)
{
    char c;
    while ((c = *buf) != '\0')
    {
        if (c >= 'a' && c <= 'z')
            *buf -= 32;
        buf++;
    }
};

static void
to_lower (char* buf)
{
    char c;
    while ((c = *buf) != '\0')
    {
        if (c >= 'A' && c <= 'Z')
            *buf += 32;
        buf++;
    }
};

static int 
is_numeric (char c)
{
    return (c >= 48 && c <= 57);
};
