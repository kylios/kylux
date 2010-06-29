// src/kernel/interrupt.c

//    src/kernel/interrupt.c is part of Kylux.
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

#include "type.h"
#include "common.h"
#include "debug.h"
#include "kernel/interrupt.h"

/* Functions defined in arch/x86/interrupt.s */
void enable_interrupts (void);
void disable_interrupts (void);

static enum interrupt_state cur_state = INTERRUPT_OFF;

enum interrupt_state 
interrupt_off ()
{
    disable_interrupts ();
    cur_state = INTERRUPT_OFF;

    return cur_state;
};

void 
interrupt_on ()
{
    cur_state = INTERRUPT_ON;
    enable_interrupts ();
};

void 
interrupt_restore (enum interrupt_state state)
{
    cur_state = state;
    if (cur_state == INTERRUPT_ON)
        enable_interrupts ();
    else
        disable_interrupts ();
};

enum interrupt_state 
interrupt_get_state ()
{
    return cur_state;    
};
