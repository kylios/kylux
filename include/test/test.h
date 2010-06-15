// include/test/test.h

//    include/test/test.h is part of Kylux.
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
// Date: 2010-02-14 12:23

/*  This file includes all our testing routines and executes certain ones
    depending on which compiler flags and boot parameters we receive. 
    The following compiler flags will include tests into Kylux:
        
        - --test-kernel: include kernel tests such as data structures
        -

    When the flags are compiled into the kernel, the tests will always 
    be run.  However, to run basic tests before or while the kernel is
    booting, the option --run-tests can be passed to Kylux at boot time.
*/

#ifndef TEST_TEST_H
#define TEST_TEST_H

#include "type.h"
#include "common.h"
#include "test/kernel/list.h"
#include "test/kernel/hash.h"

void run_kernel_tests ();




/* Runs tests for the overall system before boot occurs.  This should be
   used when most of the basic kernel functionality has been implemented
*/
void run_system_tests ();

#endif  // TEST_TEST_H
