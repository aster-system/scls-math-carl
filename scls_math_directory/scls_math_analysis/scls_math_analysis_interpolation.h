//******************
//
// scls_math_analysis_interpolation.h
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
// This file contains some tools used to calculate interpolation.
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

#ifndef SCLS_MATH_ANALYSIS_INTERPOLATION
#define SCLS_MATH_ANALYSIS_INTERPOLATION

// Include SCLS Math bigger header
#include "../scls_math_formula.h"
#include "../scls_math_geometry_core.h"
#include "../scls_math_algebra/scls_math_algebra_double.h"


// The namespace "scls" is used to simplify the all.
namespace scls {

    // Calculate a lagrange polynomial
    std::shared_ptr<Formula_Base_Template<Double>> lagrange_polynomial(std::vector<Double> values, std::size_t current_value);

    // Calculate the lagrange polynomial interpolator
    std::shared_ptr<Formula_Base_Template<Double>> lagrange_interpolator(std::vector<Point_2D> points);

}

#endif // SCLS_MATH_ANALYSIS_INTERPOLATION
