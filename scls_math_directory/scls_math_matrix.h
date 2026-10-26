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
    	Matrix(int width);
    	Matrix(int width, int height);
    	Matrix(int width, int height, std::vector<std::shared_ptr<Formula_Base>> elements);

    	// Matrix colum
    	static Matrix matrix_from_column(std::vector<Matrix> c);
    	static Matrix matrix_column(int height);
    	static Matrix matrix_column(std::vector<std::shared_ptr<Formula_Base>> elements);
    	// Matrix random
    	static Matrix matrix_diagonalizable_random_integer_included_between(int width, int min, int max);
    	static Matrix matrix_random_integer_included_between(int width, int height, int min, int max);
    	static Matrix matrix_with_rank_random_integer_included_between(int width, int rank, int min, int max);

        // Do a matricial addition
        void add(Matrix* m);

        // Clone the matrice
        Matrix clone();
        void clone(Matrix* m);
        std::shared_ptr<Matrix> clone_shared_ptr();

        // Do a matrix multiplication
        std::shared_ptr<Matrix> multiplied_shared_ptr(Fraction f);
        void multiply(Formula_Base* f);
        void multiply(Fraction f);

        // Do a matrix product (this * m)
        Matrix product(Matrix* m);

        // Access to an element
        Formula_Base* element_at(int x) const;
        Formula_Base* element_at(int x, int y) const;
        std::shared_ptr<Formula_Base> element_at_shared_ptr(int x, int y) const;
        void set_element_at(int x, std::shared_ptr<Formula_Base> value);
        void set_element_at(int x, int y, std::shared_ptr<Formula_Base> value);

        // Get a sub-matrix
        std::shared_ptr<Matrix> sub_line_shared_ptr(int line, int start, int width) const;
        Matrix sub_matrix_copy(int x, int y, int width, int height);

        // Multiply a value to a line
        void multiply_line(int line, Formula_Base* f);

        // Set the column of the matrix with a column matrix
        void set_column(int column, Matrix* column_matrix);

        // Set the line of the matrix with a line matrix
        void set_line(int line, Matrix* line_matrix);

        // Subtract a line with an another line (with a multiplication)
        void subtract_line(int line_1, int line_2, Formula_Base* multiple);

        // Swap lines / columns
        void swap_lines(int line_1, int line_2);

        // Returns the matrix to an MathML
        std::string to_mathml(scls::Textual_Math_Settings* settings);
        // Returns the matrix to an std::string
        std::string to_std_string(scls::Textual_Math_Settings* settings) const;

        // Getters and setters
        inline int height() const {return a_height;};
        inline int width() const {return a_width;};

    private:
        // Create the elements
        void create_elements();

        // Dimension of the matrix
        int a_height = 1;int a_width = 1;

        // Elements
        std::vector<std::shared_ptr<Formula_Base>> a_elements;
    };

    // Characteristic polynomial
    std::shared_ptr<Formula_Base> characteristic_polynomial(Matrix* m);

    // Determinant of a matrix
    std::shared_ptr<Formula_Base> determinant(const scls::Matrix* matrix);

    // Gaussian elemination
    std::shared_ptr<Matrix> gaussian_elimination_shared_ptr(Matrix* to_reduce);
    std::shared_ptr<Matrix> gaussian_elimination_inverse_shared_ptr(Matrix* to_reduce);
}

#endif // SCLS_MATH_MATRIX
