// src/kernel/lock.c

//    src/kernel/lock.c is part of Kylux.
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
// Date: 2010-02-05 11:54

#include "type.h"
#include "common.h"
#include "debug.h"
#include "kernel/lock.h"
#include "kernel/thread.h"
#include "kernel/semaphore.h"

void
lock_init (struct lock* lock)
{
    ASSERT (lock != NULL);

    sema_init (&lock->sema, 1);
};

void 
lock_acquire (struct lock* lock)
{
    ASSERT (lock != NULL);

    sema_down (&lock->sema);
    lock->holder = thread_current ();
};

bool 
lock_try_acquire (struct lock* lock)
{
    bool result;

    ASSERT (lock != NULL);

    result = sema_try_down (&lock->sema);           
    if (result) lock->holder = thread_current ();
    return result;
};

void 
lock_release (struct lock* lock)
{
    ASSERT (lock != NULL);
    ASSERT (lock_holder (lock) == thread_current ());

    lock->holder = NULL;
    sema_up (&lock->sema);
};

struct thread*
lock_holder (struct lock* lock)
{
    ASSERT (lock != NULL);

    return lock->holder;
};
