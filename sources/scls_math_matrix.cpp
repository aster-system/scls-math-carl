//******************
//
// scls_math_matrix.cpp
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
// This file contains the source code of "scls_math_matrix.h".
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

// Include SCLS Math header
#include "../scls_math_directory/scls_math_matrix.h"


// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
    //
    // The Matrix part
    //
    //*********

    // Matrix constructor
	Matrix::Matrix(int width):Matrix(1, width){}
    Matrix::Matrix(int width, int height):a_height(height),a_width(width){create_elements();};
    Matrix::Matrix(int width, int height, std::vector<std::shared_ptr<Formula_Base>> elements):a_height(height),a_width(width),a_elements(elements){
    	// Handle the elements vector
    	if(a_elements.size() > static_cast<unsigned int>(width * height)){a_elements.resize(width * height);scls::print("Warning", "SCLS Math Matrix", "Too much elements in a matrix.");}
    	else if(a_elements.size() < static_cast<unsigned int>(width * height)){a_elements.resize(width * height);scls::print("Warning", "SCLS Math Matrix", "Too many elements in a matrix.");}
    	for(std::size_t i = 0;i<a_elements.size();i++){
    		if(a_elements.at(i).get() == 0){
    			a_elements[i] = std::make_shared<scls::Formula_Base>(0);
    		}
    	}
    }

    // Matrix colum
    Matrix Matrix::matrix_column(std::vector<std::shared_ptr<Formula_Base>> elements){return Matrix(1, elements.size(), elements);}

    // Matrix random
    Matrix Matrix::matrix_random_integer_included_between(int width, int height, int min, int max) {
    	std::vector<std::shared_ptr<Formula_Base>> values = std::vector<std::shared_ptr<Formula_Base>>(width * height);
    	for(std::size_t i = 0;i<values.size();i++){values[i] = std::make_shared<scls::Formula_Base>(scls::Fraction(scls::random_int_between_included(min, max)));}
    	return Matrix(width, height, values);
    }

    // Do a matricial addition
    void Matrix::add(Matrix* m) {
        // Verification
        if(a_width != m->a_width || a_height != m->a_height){return;}

        // Do the addition
        for(int i = 0;i<a_width;i++) {
            for(int j = 0;j<a_height;j++) {
                (a_elements[i * a_height + j].get())->add(m->a_elements[i * a_height + j].get());
            }
        }
    }

    // Clone the matrice
    void Matrix::clone(Matrix* m) {
    	for(int i = 0;i<a_width;i++) {
    		for(int j = 0;j<a_height;j++) {
    			m->set_element_at(i, j, element_at(i, j)->clone());
    		}
    	}
    }
	std::shared_ptr<Matrix> Matrix::clone_shared_ptr() {
		std::shared_ptr<Matrix> cloned = std::make_shared<Matrix>(a_width, a_height);
		for(std::size_t i = 0;i<a_elements.size();i++) {
			cloned.get()->a_elements[i] = a_elements.at(i).get()->clone();
		}

		return cloned;
	}

    // Create the elements
    void Matrix::create_elements(){
        a_elements = std::vector<std::shared_ptr<scls::Formula_Base>>(a_width * a_height);
        for(int i = 0;i<a_width;i++) {
            for(int j = 0;j<a_height;j++) {
                a_elements[i * a_height + j] = std::make_shared<scls::Formula_Base>(0);
            }
        }
    }

    // Do a matrix multiplication
    void Matrix::multiply(Formula_Base* f) {
        // Do the addition
        for(int i = 0;i<a_width;i++) {
            for(int j = 0;j<a_height;j++) {
                (a_elements[i * a_height + j].get())->multiply(f);
            }
        }
    }
    void Matrix::multiply_line(int line, Formula_Base* f) {
    	for(int i = 0;i<a_width;i++) {
			element_at(i, line)->multiply(f);
		}
    }

    // Do a matrix product (this * m)
    Matrix Matrix::product(Matrix* m) {
        // Verification
        if(a_width != m->a_height){return Matrix(0, 0);}

        Matrix to_return(m->a_width, a_height);

        // Do the product
        for(int i = 0;i<m->a_width;i++) {
            for(int j = 0;j<a_height;j++) {
                scls::Formula_Base* current = to_return.element_at(i, j);
                for(int k = 0;k<a_width;k++) {
                    std::shared_ptr<Formula_Base> f = element_at(k, j)->clone();
                    f.get()->multiply(m->element_at(i, k));
                    current->add(f.get());
                }
            }
        }

        return to_return;
    };

    // Access to an element
    Formula_Base* Matrix::element_at(int x){return element_at(0, x);};
    Formula_Base* Matrix::element_at(int x, int y){return a_elements.at(x * a_height + y).get();};
    std::shared_ptr<Formula_Base> Matrix::element_at_shared_ptr(int x, int y){return a_elements.at(x * a_height + y);};
    void Matrix::set_element_at(int x, std::shared_ptr<Formula_Base> value){return set_element_at(0, x, value);};
    void Matrix::set_element_at(int x, int y, std::shared_ptr<Formula_Base> value){a_elements[x * a_height + y] = value;};

    // Get a sub-matrix
    Matrix Matrix::sub_matrix_copy(int x, int y, int width, int height) {
        Matrix s = Matrix(width, height);
        for(int i = 0;i<width;i++){
        	for(int j = 0;j<height;j++){
				s.set_element_at(i, j, element_at(x + i, y + j)->clone());
			}
        }
        return s;
    }

    // Subtract a line with an another line (with a multiplication)
    void Matrix::subtract_line(int line_1, int line_2, Formula_Base* multiple) {
    	for(int i = 0;i<a_width;i++) {
    		std::shared_ptr<Formula_Base> f = element_at(i, line_2)->clone();f.get()->multiply(multiple);
			element_at(i, line_1)->substract(f.get());element_at(i, line_1)->simplify();
		}
    }

    // Swap lines / columns
    void Matrix::swap_lines(int line_1, int line_2) {
    	for(int i = 0;i<width();i++) {
			std::shared_ptr<scls::Formula_Base> temp = element_at_shared_ptr(i, line_2);
			set_element_at(i, line_2, element_at_shared_ptr(i, line_1));
			set_element_at(i, line_1, temp);
		}
    }

    // Returns the matrix to an MathML
    std::string Matrix::to_mathml(scls::Textual_Math_Settings* settings) {
    	std::string to_return = std::string("<mtable>");
		for(int i = 0;i<a_height;i++) {
			std::string current_line = std::string();
			for(int j = 0;j<a_width;j++) {
				std::string current = element_at(j, i)->to_mathml(settings);
				current_line += std::string("<mtd>") + current + std::string("</mtd>");
			}

			// Breakline
			to_return += std::string("<mtr>") + current_line + std::string("</mtr>");
		}
		to_return += std::string("</mtable>");
		return to_return;
    }

    // Returns the matrix to an std::string
    std::string Matrix::to_std_string(scls::Textual_Math_Settings* settings) {
        // Get the column size
        std::vector<int> column_size = std::vector<int>(a_width, 0);
        for(int i = 0;i<a_width;i++) {
            for(int j = 0;j<a_height;j++) {
                std::string current = element_at(i, j)->to_std_string(settings);
                if(column_size.at(i) < static_cast<int>(current.size())){
                    column_size[i] = current.size();
                }
            }
        }

        std::string to_return = std::string();
        for(int i = 0;i<a_height;i++) {
            for(int j = 0;j<a_width;j++) {
                std::string current = element_at(j, i)->to_std_string(settings);
                while(static_cast<int>(current.size()) < column_size.at(j)){current = std::string(" ") + current;}
                to_return += current;
                if(j < a_width - 1){
                    to_return += std::string(" ");
                }
            }

            // Breakline
            if(i < a_height - 1){to_return += std::string("\n");}
        }
        return to_return;
    }

    // Gaussian elemination
    std::shared_ptr<Matrix> gaussian_elimination_shared_ptr(Matrix* to_reduce) {
        std::shared_ptr<Matrix> m = to_reduce->clone_shared_ptr();
        int number = m.get()->height();

        // Do the algorithm
        for(int i = 0;i<number;i++) {
            // Get the pivot
            int pivot = 0;
            for(int j = i + 1;j<number;j++) {
                if(m.get()->element_at(i, j)->value_to_double() > pivot) {
                    pivot = j;
                }
            }

            // Permutation
            for(int j = pivot;j>i;j--) {
                m.get()->swap_lines(j, j - 1);
            }

            // Divide
            m.get()->multiply_line(i, m.get()->element_at(i, i)->multiplication_inverse().get());

            // Subtraction
            for(int j = 0;j<i;j++) {
                std::shared_ptr<Formula_Base> p_value = m.get()->element_at(i, j)->clone();
                m.get()->subtract_line(j, i, p_value.get());
            }
            for(int j = i + 1;j<number;j++) {
                std::shared_ptr<Formula_Base> p_value = m.get()->element_at(i, j)->clone();
                m.get()->subtract_line(j, i, p_value.get());
            }
        }

        return m;
    }
    std::shared_ptr<Matrix> gaussian_elimination_inverse_shared_ptr(Matrix* to_reduce) {
        std::shared_ptr<Matrix> m = std::make_shared<Matrix>(to_reduce->width() * 2, to_reduce->height());
        to_reduce->clone(m.get());
        for(int i = 0;i<m.get()->height();i++) {
            for(int j = 0;j<m.get()->height();j++) {
                if(i == j){m.get()->set_element_at(m.get()->height() + i, i, std::make_shared<Formula_Base>(Fraction(1)));}
                else{m.get()->set_element_at(m.get()->height() + i, j, std::make_shared<Formula_Base>(Fraction(0)));}
            }
        }

        m = gaussian_elimination_shared_ptr(m.get());

        return std::make_shared<Matrix>(m.get()->sub_matrix_copy(m.get()->height(), 0, m.get()->height(), m.get()->height()));
    }
}
