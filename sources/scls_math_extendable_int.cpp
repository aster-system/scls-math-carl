//******************
//
// scls_math_extendable_int.cpp
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The Math "Carl" part represents the mathematical part of SCLS.
// It is named after one one of the greatest mathematician of all times, Carl Freiderich Gauss.
//
// This file contains the source code of "scls_math_extendable_int.h".
//
//******************
//
// License (LGPL V3.0) :
//
// Copyright (C) 2024 by Aster System, Inc. <https://aster-system.github.io/aster-system/>
// This file is part of SCLS.
// SCLS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// SCLS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with SCLS. If not, see <https://www.gnu.org/licenses/>.
//

// Include SCLS Math bigger header
#include "../scls_math.h"

namespace scls {
    // Returns the number of bit in a number (position of the last bit)
    int bit_in_number(unsigned long long t) {
        if(t == 0ll){return 0;}

        unsigned long long max = 0b1111111111111111111111111111111111111111111111111111111111111111;
        unsigned long long current = 0b1000000000000000000000000000000000000000000000000000000000000000;
        for(int i = 0;i<64;i++){
            if(((~current) | t) == max){return 64 - i;}
            current = current >> 1;
        }
        return 0;
    }
}
