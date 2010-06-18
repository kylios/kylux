// src/kernel/semaphore.c

//    src/kernel/semaphore.c is part of Kylux.
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
// Date: 2010-02-05 11:55

#include "type.h"
#include "common.h"
#include "debug.h"
#include "kernel/semaphore.h"
#include "kernel/thread.h"
#include "kernel/interrupt.h"

void 
sema_init (struct semaphore* sema, int val)
{
    ASSERT (sema != NULL);

    sema->val = val;
    list_init (&sema->waiters);
    spinlock_init (&sema->sync);
};

void 
sema_down (struct semaphore* sema)
{
    ASSERT (sema != NULL);

    enum interrupt_state state = interrupt_off ();
    /* All accesses to this semaphore must be synchronized using the
       semaphore's internal spinlock. */
    spinlock_acquire (&sema->sync);

    /* If the semaphore's value is at 0, then we must block the thread */
    while (sema->val == 0)
    {
        list_push_back (&sema->waiters, &thread_current ()->elem);
        spinlock_release (&sema->sync);

        thread_block ();

        spinlock_acquire (&sema->sync);
    }

    /* Decrement the semaphore's value */
    sema->val--;
    spinlock_release (&sema->sync);
    interrupt_restore (state);
};

bool
sema_try_down (struct semaphore* sema)
{
    bool result = true;

    ASSERT (sema != NULL);

    enum interrupt_state state = interrupt_off ();
    spinlock_acquire (&sema->sync);
    result = (sema->val > 0);
    if (result)
    {
        sema->val--;
    }
    spinlock_release (&sema->sync);
    interrupt_restore (state);

    return result;
};

void 
sema_up (struct semaphore* sema)
{
    ASSERT (sema != NULL);

    enum interrupt_state state = interrupt_off ();
    spinlock_acquire (&sema->sync);
    sema->val++;
    struct thread* t = LIST_ENTRY (
        list_pop_back (&sema->waiters), struct thread, elem);
    spinlock_release (&sema->sync);

    thread_unblock (t);
    interrupt_restore (state);
};

