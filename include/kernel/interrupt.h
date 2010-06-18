// include/kernel/interrupt.h

//    include/kernel/interrupt.h is part of Kylux.
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
// Date: 2010-06-17 20:25

#ifndef KERNEL_INTERRUPT_H
#define KERNEL_INTERRUPT_H

#include "type.h"
#include "common.h"

/* The current state of interrupts, either on or off. */
enum intrrupt_state
{
    INTERRUPT_OFF,
    INTERRUPT_ON
};

enum interrupt_state interrupt_off ();
void interrupt_on ();
void interrupt_restore (enum interrupt_state);
enum interrupt_state interrupt_get_state ();


#endif  // KERNEL_INTERRUPT_H
