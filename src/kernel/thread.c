// src/kernel/thread.c

//    src/kernel/thread.c is part of Kylux.
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

#include "type.h"
#include "common.h"
#include "debug.h"
#include "kernel/thread.h"
#include "lib/kernel/list.h"
#include "kernel/frame_mgr.h"

static struct list all_list;
static struct list ready_list;

void
init_thread ()
{
    list_init (&all_list);
    list_init (&ready_list);


};

int
thread_create (int priority, thread_func* func, void* aux)
{
   struct thread* t = frame_mgr_alloc (PAGE_SIZE, false, true);
};
