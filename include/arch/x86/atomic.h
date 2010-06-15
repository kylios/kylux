// include/arch/x86/atomic.h

//    include/arch/x86/atomic.h is part of Kylux.
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
// Date: 2010-01-24 20:38

#ifndef ARCH_X86_ATOMIC_H
#define ARCH_X86_ATOMIC_H

int atomic_test_and_set (volatile int* lock);

#endif  // ARCH_X86_ATOMIC_H
