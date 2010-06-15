// include/kernel/process.h

//    include/kernel/process.h is part of Kylux.
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

#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include "type.h"
#include "common.h"
#include "kernel/pagemap.h"
#include "kernel/heap.h"
#include "lib/kernel/list.h"
#include "arch/x86/isr.h"

#define PROCESS_MAGIC   0xDEADBABE

struct process
{
    /* Stack */
    int magic;  /* Should always equal PROCESS_MAGIC */
    struct heap heap;
    struct list_elem sched_elem;
    struct registers regs;
    //struct pagemap pm;
    /* TODO: Use a page directory for now until swapping is implemented */
    struct pagedir* pd;
};

/* Turns the current execution into a process */
void init_process ();


#endif  // KERNEL_PROCESS_H
