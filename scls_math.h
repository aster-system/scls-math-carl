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

// Include scls_foundation
#ifdef __ASTER_DEV
#include "../scls-foundation-leonhard/scls_foundation.h"
#else
#include <scls_foundation.h>
#endif // __ASTER_DEV
#include <algorithm>

#ifndef SCLS_MATH_INIT
#define SCLS_MATH_INIT int scls::model_maker::Point::a_points_number = 0; int scls::model_maker::Face::a_faces_number = 0; int scls::model_maker::Solid::a_solids_number = 0; int __current_point_in_grid_x = 0; int __current_point_in_grid_y = 0;
#endif // SCLS_MATH_INIT

#ifndef SCLS_PI
#define SCLS_PI 3.1415926535
#endif // SCLS_PI
#ifndef SCLS_HALP_PI
#define SCLS_HALF_PI 1.5707963267948966
#endif // SCLS_HALP_PI

#include "scls_math_directory/scls_math_fraction.h"
#include "scls_math_directory/scls_math_3d_core.h"
#include "scls_math_directory/scls_math_3d_model.h"

#endif // SCLS_MATH
