#ifndef DEBUG_H
#define DEBUG_H

#include "type.h"
#include "common.h"
#include "lib/stdio.h"
#include "lib/stdlib.h"

void debug_backtrace (void);
void hexdump (void* start, uint32 sz);

#define ASSERT(expression)  \
    if (!(expression))  {   \
        printf ("!!!Failed assertion: `%s' in %s: %d in %s \n\n", \
            #expression, __FILE__, __LINE__, __func__);    \
        debug_backtrace (); \
        while (1); \
    }

#define DEBUG_MARK  \
    printf ("DEBUG %s: %d (`%s')\n", __FILE__, __LINE__, __func__);

#define PANIC(MSG)  \
    interrupt_off ();   \
    printf ("!!!KERNEL PANIC: %s \n", MSG); \
    while (1);  

#define NOT_REACHED \
    debug_backtrace (); \
    PANIC("unreachable statement \n");  
    


#endif // DEBUG_H

