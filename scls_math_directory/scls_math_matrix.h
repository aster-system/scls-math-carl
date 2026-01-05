//******************
//
// scls_math_matrix.h
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
// This file contains some tools used to work with matrix.
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

#ifndef SCLS_MATH_MATRIX
#define SCLS_MATH_MATRIX

// Include SCLS Math bigger header
#include "../scls_math.h"


// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
    //
    // The Matrix part
    //
    //*********

    class Matrix {
    public:
        // Matrix constructor
        Matrix(int width, int height);

        // Do a matricial addition
        void add(Matrix* m);

        // Do a matrix multiplication
        void multiply(scls::__Formula f);
        void multiply(scls::__Formula* f);

        // Do a matrix product (this * m)
        Matrix product(Matrix* m);

        // Access to an element
        scls::__Formula* element_at(int x, int y);

        // Returns the matrix to an MathML
        std::string to_mathml(scls::Textual_Math_Settings* settings);
        // Returns the matrix to an std::string
        std::string to_std_string(scls::Textual_Math_Settings* settings);

    private:
        // Create the elements
        void create_elements();

        // Dimension of the matrix
        int a_height = 1;int a_width = 1;

        // Elements
        std::vector<std::shared_ptr<scls::__Formula>> a_elements;
    };
}

#endif // SCLS_MATH_MATRIX
