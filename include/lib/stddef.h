// include/lib/stddef.h

//    include/lib/stddef.h is part of Kylux.
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
// Date: 2010-02-10 18:06

#ifndef LIB_STDDEF_H
#define LIB_STDDEF_H

#include "type.h"
#include "common.h"

#define offsetof(TYPE, MEMBER)  \
    __builtin_offsetof(TYPE, MEMBER)


#endif  // LIB_STDDEF_H
