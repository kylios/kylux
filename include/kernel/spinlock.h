// include/kernel/spinlock.h

//    include/kernel/spinlock.h is part of Kylux.
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
// Date: 2010-01-24 20:24

#ifndef KERNEL_SPINLOCK_H
#define KERNEL_SPINLOCK_H

#include "type.h"

struct spinlock
{
	volatile int val;
};

/* Initializes a new spinlock */
void spinlock_init (struct spinlock* slock);
/* Aquires a spinlock.  Spins (busywaits) if the lock is currently held*/
void spinlock_acquire (struct spinlock* slock);
/* If the lock is not held, acquire it and return TRUE, otherwise return
   FALSE and does not busywait. */
bool spinlock_try_acquire (struct spinlock* slock);
/* Releases a spinlock */
void spinlock_release (struct spinlock* slock);

#endif  // KERNEL_SPINLOCK_H
