// src/kernel/spinlock.c

//    src/kernel/spinlock.c is part of Kylux.
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

#include "arch/x86/atomic.h"
#include "kernel/spinlock.h"
#include "debug.h"

void 
spinlock_init (struct spinlock* slock)
{
	ASSERT (slock != NULL);

	slock->val = 0;
};

void 
spinlock_acquire (struct spinlock* slock)
{
	ASSERT (slock != NULL);

	while (1 == atomic_test_and_set ((int*) &slock->val));
};

bool 
spinlock_try_acquire (struct spinlock* slock)
{
    ASSERT (slock != NULL);
	int val = atomic_test_and_set (&slock->val);

	return (val == 0);
};

void 
spinlock_release (struct spinlock* slock)
{
	ASSERT (slock != NULL);

	slock->val = 0;
};


