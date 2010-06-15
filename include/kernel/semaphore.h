// include/kernel/semaphore.h

//    include/kernel/semaphore.h is part of Kylux.
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

#ifndef KERNEL_SEMAPHORE_H
#define KERNEL_SEMAPHORE_H

#include "type.h"
#include "common.h"
#include "lib/kernel/list.h"
#include "kernel/spinlock.h"

/* Blocking semaphore data structure */
struct semaphore
{
    int val;
    struct list waiters;
    struct spinlock sync;
};

void sema_init (struct semaphore* sema, int val);
void sema_down (struct semaphore* sema);
bool sema_try_down (struct semaphore* sema);
void sema_up (struct semaphore* sema);



#endif  // KERNEL_SEMAPHORE_H
