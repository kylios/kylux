// include/kernel/lock.h

//    include/kernel/lock.h is part of Kylux.
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

#ifndef KERNEL_LOCK_H
#define KERNEL_LOCK_H

#include "type.h"
#include "common.h"
#include "kernel/semaphore.h"

/* Blocking lock mechanism */
struct lock
{
    struct semaphore sema;
//    struct thread* holder;
};

void lock_init (struct lock* lock);
void lock_acquire (struct lock* lock);
bool lock_try_acquire (struct lock* lock);
void lock_release (struct lock* lock);
//struct thread* lock_holder (struct lock* lock);


#endif  // KERNEL_LOCK_H
