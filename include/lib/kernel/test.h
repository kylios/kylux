// include/lib/kernel/test.h

//    include/lib/kernel/test.h is part of Kylux.
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

#ifndef LIB_KERNEL_TEST_H
#define LIB_KERNEL_TEST_H

#include "type.h"
#include "common.h"

/* Declare a bool for our assert */
bool __test_result_bool__;

/* Wrapper for printf */
void test_msg (const char* format, ...);

#define assert(expression)  \
    __test_result_bool__ = expression;  \
    if (__test_result_bool__ == false)  \
    {   \
        test_msg ("Failed assertion: %s\n", #expression);   \
    }   \
    else    \
    {   \
        test_msg ("Passed assertion: %s\n", #expression);   \
    }   \
    __test_result_bool__
    
/* Basic non-fatal assertion */
bool _assert (bool condition);


#endif  // LIB_KERNEL_TEST_H
