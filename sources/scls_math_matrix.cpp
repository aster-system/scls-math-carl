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
    Matrix Matrix::matrix_from_column(std::vector<Matrix> c) {
        // Assert
        int needed_height = c.at(0).height();
        for(std::size_t i = 0;i<c.size();i++) {
            if(c.at(i).width() != 1){print("Warning", "SCLS Math Matrix", std::string("The vector of column you want to build a matrix from contains a columns which is not a column matrix (the column " + std::to_string(i) + " is a (" + std::to_string(c.at(i).height()) + ", " + std::to_string(c.at(i).width()) + ") matrix)."));}
            else if(c.at(i).height() != needed_height){print("Warning", "SCLS Math Matrix", std::string("The vector of column you want to build a matrix from contains two columns with different height (the column 0 contains " + std::to_string(needed_height) + " elements and the column " + std::to_string(i) + " contains " + std::to_string(c.at(i).height()) + " elements)."));}
        }

        // Build the matrix
        Matrix m = Matrix(c.size(), needed_height);
        for(std::size_t i = 0;i<c.size();i++){m.set_column(i, &c.at(i));}
        return m;
    }
    Matrix Matrix::matrix_column(int height){return Matrix(1, height);}
    Matrix Matrix::matrix_column(std::vector<std::shared_ptr<Formula_Base>> elements){return Matrix(1, elements.size(), elements);}

    // Matrix random
    Matrix Matrix::matrix_random_integer_included_between(int width, int height, int min, int max) {
    	std::vector<std::shared_ptr<Formula_Base>> values = std::vector<std::shared_ptr<Formula_Base>>(width * height);
    	for(std::size_t i = 0;i<values.size();i++){values[i] = std::make_shared<scls::Formula_Base>(scls::Fraction(scls::random_int_between_included(min, max)));}
    	return Matrix(width, height, values);
    }
    Matrix Matrix::matrix_with_rank_random_integer_included_between(int width, int rank, int min, int max) {
		// Every positions
    	std::vector<int> positions = std::vector<int>(width);
		for(int i = 0;i<rank;i++){positions[i] = i;}

		// Generate the base
		std::vector<Matrix> needed_base = std::vector<Matrix>(width, Matrix(0, 0));
		for(int i = 0;i<rank;i++){needed_base[i] = Matrix::matrix_column(width);needed_base[i].set_element_at(0, i, std::make_shared<Formula_Base>(1));}

		// Generate the matrix
		for(int j = 0;j<rank;j++) {
            for(int i = 0;i<rank;i++) {
                // Multiplication
                if(j == 0) {
                    Fraction random_fraction = 0;
                    while(random_fraction == 0){random_fraction = random_int_between_included(min, max);}
                    needed_base[i].multiply(std::make_shared<Formula_Base>(random_fraction).get());
                }

                // Column operations
                int random_column = random_int_between_included(0, needed_base.size() - 2);
                if(random_column >= i){random_column++;}
                Fraction random_fraction = random_int_between_included(min, max);
                needed_base[i].add(needed_base[random_column].multiplied_shared_ptr(random_fraction).get());
            }
		}

		// Generate the matrix
		Matrix to_return = Matrix(width, width);
		for(int i = 0;i<rank;i++) {
			int random_p = random_int_between_included(0, needed_base.size() - 1);
			to_return.set_column(i, &needed_base.at(random_p));
			needed_base.erase(needed_base.begin() + random_p);
		}

		// Return the result
    	return to_return;
	}
    Matrix Matrix::matrix_diagonalizable_random_integer_included_between(int width, int min, int max) {
		Matrix d = Matrix(width, width);
		for(int i = 0;i<width;i++){d.set_element_at(i, i, std::make_shared<scls::Formula_Base>(scls::Fraction(scls::random_int_between_included(min, max))));}

		Matrix b = matrix_with_rank_random_integer_included_between(width, width, min, max);
		std::shared_ptr<Matrix> i = gaussian_elimination_inverse_shared_ptr(&b);
		Matrix to_return = i.get()->clone();to_return = to_return.product(&d);to_return = to_return.product(&b);

		std::cout << "U " << std::endl << b.to_std_string(0) << std::endl << "-" << std::endl << d.to_std_string(0) << std::endl << "-" << std::endl << i.get()->to_std_string(0) << std::endl << "-" << std::endl << b.product(i.get()).to_std_string(0) << std::endl << "---------" << std::endl;

		return to_return;
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
    Matrix Matrix::clone() {
        Matrix to_return = Matrix(a_width, a_height);
        clone(&to_return);
        return to_return;
    }
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
    std::shared_ptr<Matrix> Matrix::multiplied_shared_ptr(Fraction f) {
    	std::shared_ptr<Matrix> to_return = clone_shared_ptr();
    	to_return.get()->multiply(f);
    	return to_return;
    }
    void Matrix::multiply(Fraction f){return multiply(std::make_shared<Formula_Base>(f).get());}
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
    Formula_Base* Matrix::element_at(int x)const{return element_at(0, x);};
    Formula_Base* Matrix::element_at(int x, int y)const{return a_elements.at(x * a_height + y).get();};
    std::shared_ptr<Formula_Base> Matrix::element_at_shared_ptr(int x, int y)const{return a_elements.at(x * a_height + y);};
    void Matrix::set_element_at(int x, std::shared_ptr<Formula_Base> value){return set_element_at(0, x, value);};
    void Matrix::set_element_at(int x, int y, std::shared_ptr<Formula_Base> value){a_elements[x * a_height + y] = value;};

    // Set the column of the matrix with a column matrix
	void Matrix::set_column(int column, Matrix* column_matrix){
		// Assert
		if(column_matrix->width() != 1){scls::print("Warning", "SCLS Math Carl", "The column you want to set in a matrix in position \"" + std::to_string(column) + "\" is not a column matrix, but a \"(" + std::to_string(column_matrix->height()) + ", " + std::to_string(column_matrix->width()) + ")\" matrix.");}
		else if(column_matrix->height() != height()){scls::print("Warning", "SCLS Math Carl", "The column you want to set in a matrix in position \"" + std::to_string(column) + "\" has not the same height as the full matrix (the column has a height of \"" + std::to_string(column_matrix->width()) + "\" and the matrix a height of \"" + std::to_string(height()) + "\").");}

		// Place the elements
		for(int i = 0;i<height();i++) {
			set_element_at(column, i, column_matrix->element_at_shared_ptr(0, i));
		}
	}

    // Set the line of the matrix with a line matrix
    void Matrix::set_line(int line, Matrix* line_matrix){
        // Assert
        if(line_matrix->height() != 1){scls::print("Warning", "SCLS Math Carl", "The line you want to set in a matrix in position \"" + std::to_string(line) + "\" is not a line matrix, but a \"(" + std::to_string(line_matrix->height()) + ", " + std::to_string(line_matrix->width()) + ")\" matrix.");}
        else if(line_matrix->width() != width()){scls::print("Warning", "SCLS Math Carl", "The line you want to set in a matrix in position \"" + std::to_string(line) + "\" has not the same width as the full matrix (the line has a wait of \"" + std::to_string(line_matrix->width()) + "\" and the matrix a width of \"" + std::to_string(width()) + "\").");}

        // Place the elements
        for(int i = 0;i<width();i++) {
            set_element_at(i, line, line_matrix->element_at_shared_ptr(i, 0));
        }
    }

    // Get a sub-matrix
    std::shared_ptr<Matrix> Matrix::sub_line_shared_ptr(int line, int start, int sub_line_width) const {
        // Get the line
        std::shared_ptr<Matrix> s = std::make_shared<Matrix>(sub_line_width, 1);
        for(int i = 0;i<sub_line_width;i++){
            s.get()->set_element_at(i, 0, element_at_shared_ptr(start + i, line));
        }
        return s;
    }
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
    std::string Matrix::to_std_string(scls::Textual_Math_Settings* settings) const {
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

    // Characteristic polynomial
    std::shared_ptr<Formula_Base> characteristic_polynomial(Matrix* m) {
        scls::Matrix id = scls::Matrix(m->height(), m->width());
        for(int i = 0;i<id.width();i++){id.set_element_at(i, i, scls::string_to_algebra_element<scls::Formula_Base>("a"));}

        scls::Matrix p = m->clone();p.multiply(std::make_shared<Formula_Base>(scls::Fraction(-1)).get());p.add(&id);
        return determinant(&p);
    }

    // Determinant of a matrix
    std::shared_ptr<Formula_Base> determinant(const scls::Matrix* matrix) {
        // Different sizes
        if(matrix->width() == 1){return matrix->element_at(0, 0)->clone();}
        else if(matrix->width() == 2) {
            std::shared_ptr<Formula_Base> temp_formula_1 = matrix->element_at(0, 0)->clone();temp_formula_1.get()->multiply(matrix->element_at(1, 1));
            std::shared_ptr<Formula_Base> temp_formula_2 = matrix->element_at(0, 1)->clone();temp_formula_2.get()->multiply(matrix->element_at(1, 0));
            temp_formula_1.get()->substract(temp_formula_2.get());
            return temp_formula_1;
        }

        // Laplace formula
        std::shared_ptr<Formula_Base> to_return = std::make_shared<Formula_Base>(Fraction(0));
        for(int i =0;i<matrix->height();i++) {
            scls::Matrix temp = scls::Matrix(matrix->width() - 1, matrix->height() - 1);
            int offset = 0;for(int j = 0;j<temp.width();j++){if(i == j){offset++;}temp.set_line(j, matrix->sub_line_shared_ptr(j + offset, 1, temp.width()).get());}
            std::shared_ptr<Formula_Base> temp_formula = matrix->element_at(0, i)->clone();temp_formula.get()->multiply(determinant(&temp).get());
            if(i % 2 == 1){temp_formula.get()->multiply(Fraction(-1));}
            to_return.get()->add(temp_formula.get());
        }

        to_return.get()->simplify();
        return to_return;
    }

    // Gaussian elemination
    std::shared_ptr<Matrix> gaussian_elimination_shared_ptr(Matrix* to_reduce) {
        std::shared_ptr<Matrix> m = to_reduce->clone_shared_ptr();
        int number = m.get()->height();

        // Do the algorithm
        for(int i = 0;i<number;i++) {
            // Get the pivot
            int pivot = 0;double pivot_value = 0;
            for(int j = i + 1;j<number;j++) {
                double current_value = std::abs(m.get()->element_at(i, j)->value_to_double());
                if(current_value > pivot_value) {
                    pivot = j;pivot_value = current_value;
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
