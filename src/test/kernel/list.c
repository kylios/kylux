// src/test/kernel/list.c

//    src/test/kernel/list.c is part of Kylux.
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

#include "type.h"
#include "common.h"
#include "debug.h"
#include "lib/kernel/test.h"
#include "lib/kernel/list.h"
#include "test/kernel/list.h"

static void test_list_int_print (struct list_elem* e);
static int test_list_int_cmp (struct list_elem* a, struct list_elem* b,
        void* AUX);

void
test_list ()
{
    struct list l1;
    struct list l2;
    struct list l3;

    test_msg ("Testing list \n");

    list_init (&l1);
    list_init (&l2);
    list_init (&l3);

    test_msg ("Printing lists.  They should be empty \n");
    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("-Printing list l2\n");
    list_print (&l2, &test_list_int_print);

    test_msg ("-Printing list l3\n");
    list_print (&l3, &test_list_int_print);

    test_msg ("Adding '5' to l1\n");
    struct test_list_int i5;
    i5.i = 5;
    list_push_back (&l1, &i5.elem);

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("Adding '8' to l1\n");
    struct test_list_int i8;
    i8.i = 8;
    list_push_back (&l1, &i8.elem);

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("Adding '7' to l1\n");
    struct test_list_int i7;
    i7.i = 7;
    list_push_back (&l1, &i7.elem);

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("Adding '4' to l1\n");
    struct test_list_int i4;
    i4.i = 4;
    list_push_back (&l1, &i4.elem);

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("Removing '7' from l1\n");
    list_remove (list_find (&l1, &i7.elem, &test_list_int_cmp, NULL));

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("Popping front off l1\n");
    list_pop_front (&l1);

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("Popping back off l1\n");
    list_pop_back (&l1);

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("Adding '7' to l1\n");
    i7.i = 7;
    list_push_back (&l1, &i7.elem);

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("Adding '4' to l1\n");
    i4.i = 4;
    list_push_back (&l1, &i4.elem);

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);

    test_msg ("Removing '7' from l1\n");
    list_remove (list_find (&l1, &i7.elem, &test_list_int_cmp, NULL));

    test_msg ("-Printing list l1\n");
    list_print (&l1, &test_list_int_print);
};

static void 
test_list_int_print (struct list_elem* e)
{
    struct test_list_int* i;

    ASSERT (e != NULL);    

    i = LIST_ENTRY (e, struct test_list_int, elem);
    ASSERT (i != NULL);

    test_msg ("%d\n", i->i);
};

static int
test_list_int_cmp (struct list_elem* a, struct list_elem* b, void* aux)
{
    struct test_list_int* ia;
    struct test_list_int* ib;

    ASSERT (a != NULL);
    ASSERT (b != NULL);

    ia = LIST_ENTRY (a, struct test_list_int, elem);
    ib = LIST_ENTRY (b, struct test_list_int, elem);

    return ia->i - ib->i;
};
