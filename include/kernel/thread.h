// include/kernel/thread.h

//    include/kernel/thread.h is part of Kylux.
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
// Date: 2010-02-16 14:16

#ifndef KERNEL_THREAD_H
#define KERNEL_THREAD_H

#include "type.h"
#include "common.h"
#include "lib/kernel/list.h"

#define THREAD_MAGIC    0x0DE703A7

enum thread_state
{
    THREAD_READY,
    THREAD_RUNNING, 
    THREAD_BLOCKED,
    THREAD_DYING
};

typedef int tid_t;

struct thread
{
    tid_t tid;
    int priority;
    enum thread_status status;

    uint32* stack;

    struct list_elem all_elem;
    struct list_elem elem;

    /* Magic needs to be the last field to detect stack overflow. */
    uint32 magic;
};

/* Defines the function type that our kernel threads will require in
   order to execute. */
typedef int thread_func (void*);

/* Initializes the threading system.  */
void init_thread ();

struct thread* thread_create (int priority, thread_func* func, void* aux);



#endif  // KERNEL_THREAD_H
