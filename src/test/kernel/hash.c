// src/test/kernel/hash.c

//    src/test/kernel/hash.c is part of Kylux.
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
#include "test/kernel/hash.h"
#include "lib/kernel/hash.h"

static uint32 test_hash_int_hash (struct hash_elem* e);
static int test_hash_int_compare (struct hash_elem* e1, struct hash_elem*
        e2, void* aux);
static void test_hash_int_print (struct list_elem* e);

void
test_hash ()
{
    struct hash h1;

    hash_init (&h1, &test_hash_int_hash, &test_hash_int_compare);

    struct test_hash_int i5;
    i5.i = 5;
    hash_insert (&h1, &i5.elem);

    test_msg ("Printing hash h1\n");
    hash_print (&h1, &test_hash_int_print);

    struct test_hash_int i7;
    i7.i = 7;
    hash_insert (&h1, &i7.elem);

    test_msg ("Printing hash h1\n");
    hash_print (&h1, &test_hash_int_print);

    struct test_hash_int i2;
    i2.i = 2;
    hash_insert (&h1, &i2.elem);

    test_msg ("Printing hash h1\n");
    hash_print (&h1, &test_hash_int_print);
    
    struct test_hash_int copy;
    copy.i = 2 + HASH_BUCKETS;
    hash_insert (&h1, &copy.elem);

    test_msg ("Printing hash h1\n");
    hash_print (&h1, &test_hash_int_print);

    struct test_hash_int find;
    find.i = 2;
    test_msg ("Finding '2' in hash h1\n");
    struct hash_elem* efound;
    struct test_hash_int* found;
    efound = hash_find (&h1, &find.elem, NULL);
    ASSERT (efound != NULL);
    found = HASH_ENTRY (efound, struct test_hash_int, elem);
    test_msg ("Found %d\n", found->i);

    hash_remove (efound);
    hash_print (&h1, &test_hash_int_print);
};

static uint32 
test_hash_int_hash (struct hash_elem* e)
{
    struct test_hash_int* i;
    int num;
    ASSERT (e != NULL);

    i = HASH_ENTRY (e, struct test_hash_int, elem);

    num = i->i;

    return num;
};

static int 
test_hash_int_compare (struct hash_elem* e1, struct hash_elem*
        e2, void* aux)
{
    struct test_hash_int* i1;
    struct test_hash_int* i2;

    ASSERT (e1 != NULL);
    ASSERT (e2 != NULL);

    i1 = HASH_ENTRY (e1, struct test_hash_int, elem);
    i2 = HASH_ENTRY (e2, struct test_hash_int, elem);

    return i1->i - i2->i;
};

void 
test_hash_int_print (struct list_elem* e)
{
    struct test_hash_int* i;

    ASSERT (e != NULL);    

//    i = LIST_ENTRY (e, struct test_hash_int, elem);
    test_msg ("%d", i->i);
};
