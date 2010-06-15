#ifndef STDLIB_H
#define STDLIB_H

/* A function to 'output' a character.  We will need this to implement
   printf and sprinf.  printf will specify a function that will print 
   to the screen while sprintf will supply a function that prints to the
   user-supplied pointer. */
typedef void output (char c, void* buf);


/* Convert VAL to a string representation in number base BASE.
   FUNC - the supplied output function where the string will be sent
   BUF - the required parameter to FUNC
   VAL - the number to convert
   BASE - the number base to use
   SIGN - treat VAL as signed or unsigned
   WIDTH - the width of the string.  Adds spaces as padding on the left
        side of the number.
   LOWER - use lower case letters for representing numbers of base >10
   Returns the number of characters printed.  Does not print a
   '\0' at the end of the string.
   */
int __itoa (output* func, void* buf, int val, 
        int base, int sign, int width, int lower);

/* Converts VALUE to a string representation in number base BASE.  The
   converted string gets written to STR.  SIGN indicates whether to treat
   VALUE as a signed or unsigned integer. */
int _itoa (int value, char* str, int base, int sign);

/* Converts VALUE to a string representation in number base BASE.  The 
   converted string gets written to STR. */
int itoa (int value, char* str, int base);

#endif // STDLIB_H

