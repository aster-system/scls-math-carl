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

// Root
#include "scls_math_root.h"

// Root of SCLS Math
#include "scls_math_directory/scls_math_complex.h"
#include "scls_math_directory/scls_math_extendable_fraction.h"
#include "scls_math_directory/scls_math_extendable_int.h"
#include "scls_math_directory/scls_math_formula.h"
#include "scls_math_directory/scls_math_numbers.h"
#include "scls_math_directory/scls_math_set.h"
#include "scls_math_directory/scls_math_statistic.h"
#include "scls_math_directory/scls_math_string.h"
// Algebra in SCLS Math
#include "scls_math_directory/scls_math_boolean.h"
#include "scls_math_directory/scls_math_graph.h"
#include "scls_math_directory/scls_math_matrix.h"
// Geometry in SCLS Math
#include "scls_math_directory/scls_math_2d_physic.h"
#include "scls_math_directory/scls_math_geometry_core.h"

// Function handling (indev)
#ifdef __ASTER_DEV
#include "scls_math_directory/scls_math_polynomial.h"

// 3D Geometry handling (indev)
#include "scls_math_directory/scls_math_3d_core.h"
#include "scls_math_directory/scls_math_3d_model.h"
#include "scls_math_directory/scls_math_3d_physic.h"
#endif // __ASTER_DEV

#endif // SCLS_MATH
