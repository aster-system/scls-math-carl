//******************
//
// scls_math_graph.h
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
// This file contains some functions to handle graph with mathematics.
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

#ifndef SCLS_MATH_GRAPH
#define SCLS_MATH_GRAPH

#include "../scls_math.h"
#include <list>

// The namespace "scls" is used to simplify the all.
namespace scls {
    // Use the djikstra algorithm in a 2D plane
    constexpr char djikstra_2d_bottom = 0;
    constexpr char djikstra_2d_left = 1;
    constexpr char djikstra_2d_right = 2;
    constexpr char djikstra_2d_top = 3;
    std::vector<char> djikstra_2d(std::vector<std::vector<int>> needed_map, Point_2D position_start, Point_2D position_end);
}

#endif // SCLS_MATH_GRAPH
