//******************
//
// scls_math_function.h
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
// This file contains the needed class to use functions class.
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

#ifndef SCLS_MATH_FUNCTION
#define SCLS_MATH_FUNCTION

#include "scls_math_polymonial.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The Function class
	//
	//*********

	class Function : public Polymonial {
        // Class representating a function
    public:
        //*********
        //
        // Function simple methods
        //
        //*********

        // "Function" constructor
        Function(): Polymonial() {}
	};

	//*********
	//
	// The Sequence class
	//
	//*********

	class Sequence : public Function {
        // Class representating a sequence
    public:
        //*********
        //
        // Sequence simple methods
        //
        //*********

        // "Sequence" constructor
        Sequence(): Function() {}

        // Returns a sum of numbers
        Polymonial sum(unsigned int max_n) {
            Polymonial total;

            return total;
        };
	};
}

#endif // SCLS_MATH_FUNCTION
