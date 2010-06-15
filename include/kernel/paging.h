// include/kernel/paging.h

//    include/kernel/paging.h is part of Kylux.
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

#ifndef KERNEL_PAGING_H
#define KERNEL_PAGING_H

#include "arch/x86/isr.h"

void init_paging ();

void page_fault (struct registers* regs);

#endif  // KERNEL_PAGING_H
