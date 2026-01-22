//******************
//
// scls_math.h
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
// This file contains the includes to all the others SCLS Math files.
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

#ifndef SCLS_MATH
#define SCLS_MATH

// Avoid some errors with libraries path
// SCLS Foundation
#ifndef SCLS_FOUNDATION_PATH
#ifdef __ASTER_DEV
#define SCLS_FOUNDATION_PATH "../scls-foundation-leonhard/scls_foundation.h"
#else
#define SCLS_FOUNDATION_PATH <scls_foundation.h>
#endif // __ASTER_DEV
#endif // SCLS_FOUNDATION_PATH

// Include SCLS Foundation
#include SCLS_FOUNDATION_PATH

// Include basic C++ files
#include <algorithm>

// Ignore all this if the header is already used
#ifndef SCLS_MATH_NUMBERS

    // Define SCLS_INIT
    #ifndef SCLS_MATH_INIT
        // 3D Geometry handling (indev)
        #ifndef SCLS_MATH_3D_INIT
            #ifdef __ASTER_DEV
                #define SCLS_MATH_3D_INIT int scls::model_maker::Point::a_points_number = 0; int scls::model_maker::Face::a_faces_number = 0; int scls::model_maker::Solid::a_solids_number = 0; int __current_point_in_grid_x = 0; int __current_point_in_grid_y = 0;
            #else
                #define SCLS_MATH_3D_INIT
            #endif // __ASTER_DEV
        #endif // SCLS_MATH_3D_INIT

        #define SCLS_MATH_INIT SCLS_MATH_3D_INIT
    #endif // SCLS_MATH_INIT
    #ifdef SCLS_INIT
    #undef SCLS_INIT
    #endif // SCLS_INIT
    #ifndef SCLS_INIT
    #define SCLS_INIT SCLS_MATH_INIT SCLS_FOUNDATION_INIT
    #endif // SCLS_INIT

    // Define some usefull numbers
    #ifndef SCLS_PI
    #define SCLS_PI 3.1415926535
    #endif // SCLS_PI
    #ifndef SCLS_HALP_PI
    #define SCLS_HALF_PI 1.5707963267948966
    #endif // SCLS_HALP_PI
    #ifndef SCLS_RADIANS_TO_ANGLE
    #define SCLS_RADIANS_TO_ANGLE 57.2957795131
    #endif // SCLS_RADIANS_TO_ANGLE

    // Root of SCLS Math
    #include "scls_math_directory/scls_math_complex.h"
	#include "scls_math_directory/scls_math_numbers.h"
	#include "scls_math_directory/scls_math_statistic.h"
	#include "scls_math_directory/scls_math_string.h"
    // Algebra in SCLS Math
    #include "scls_math_directory/scls_math_boolean.h"
    #include "scls_math_directory/scls_math_matrix.h"
    // Geometry in SCLS Math
    #include "scls_math_directory/scls_math_2d_physic.h"
    #include "scls_math_directory/scls_math_geometry_core.h"

    // Function handling (indev)
    #ifdef __ASTER_DEV
    #include "scls_math_directory/scls_math_formula.h"
    #include "scls_math_directory/scls_math_polynomial.h"

    // 3D Geometry handling (indev)
    #include "scls_math_directory/scls_math_3d_core.h"
    #include "scls_math_directory/scls_math_3d_model.h"
    #include "scls_math_directory/scls_math_string.h"
    #endif // __ASTER_DEV

#endif // SCLS_MATH_NUMBERS

#endif // SCLS_MATH
