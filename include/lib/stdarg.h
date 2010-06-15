#ifndef LIB_STDARG_H
#define LIB_STDARG_H

/* These were my attemts at implementing these functions before I learned
   that gcc already implements them for you.  Safer to use gcc's version,
   plus mine didn't work anyway... */
/*
//typedef struct struct_va_list
//{
//    void* cur_arg;
//}   va_list;
//
//#define va_start(LIST, FIRST)   \
//    LIST.cur_arg = &FIRST - sizeof(FIRST)
//
//#define va_arg(LIST, TYPE)  \
//    ((TYPE) *((TYPE*)((LIST.cur_arg) - sizeof(TYPE)))) \
//    (LIST.cur_arg - sizeof(TYPE)
//
//#define va_end(LIST)    \
//    LIST.cur_arg = NULL
*/

typedef __builtin_va_list va_list;

#define va_start(LIST, ARG)	__builtin_va_start (LIST, ARG)
#define va_end(LIST)        __builtin_va_end (LIST)
#define va_arg(LIST, TYPE)	__builtin_va_arg (LIST, TYPE)
#define va_copy(DST, SRC)	__builtin_va_copy (DST, SRC)



#endif  // LIB_STDARG_H

