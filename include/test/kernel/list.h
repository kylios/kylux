// include/test/kernel/list.h

//    include/test/kernel/list.h is part of Kylux.
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
// Date: 2010-02-14 12:37

#ifndef TEST_KERNEL_LIST_H
#define TEST_KERNEL_LIST_H

#include "type.h"
#include "common.h"
#include "lib/kernel/list.h"

struct test_list_int
{
    struct list_elem elem;
    int i;
};

/* Test the fnuctionality of our linked list. */
void test_list ();


#endif  // TEST_KERNEL_LIST_H
