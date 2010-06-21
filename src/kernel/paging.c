// src/kernel/paging.c

//    src/kernel/paging.c is part of Kylux.
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
// Date: 2010-02-03 17:11

#include "kernel/paging.h"
#include "arch/x86/isr.h"
#include "arch/x86/framebuf.h"
#include "arch/x86/pagedir.h"
#include "type.h"
#include "common.h"
#include "debug.h"

void
init_paging ()
{
    /* Register our page fault handler */
    isr_reg_func (ISR_PAGE_FAULT_EXCEPTION, &page_fault);
    /* Activate the kernel page directory */
    pagedir_switch (NULL);

    framebuf_printf ("kernel pagedir created \n");
};

void
page_fault (struct registers* regs)
{
    debug_backtrace ();
    PANIC ("Page fault!");
};
