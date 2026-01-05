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
    Matrix::Matrix(int width, int height):a_height(height),a_width(width){create_elements();};

    // Do a matricial addition
    void Matrix::add(Matrix* m) {
        // Verification
        if(a_width != m->a_width || a_height != m->a_height){return;}

        // Do the addition
        for(int i = 0;i<a_width;i++) {
            for(int j = 0;j<a_height;j++) {
                (a_elements[i * a_height + j].get())->__add(m->a_elements[i * a_height + j].get());
            }
        }
    }

    // Do a matrix multiplication
    void Matrix::multiply(__Formula f){multiply(&f);}
    void Matrix::multiply(__Formula* f) {
        // Do the addition
        for(int i = 0;i<a_width;i++) {
            for(int j = 0;j<a_height;j++) {
                (a_elements[i * a_height + j].get())->__multiply(f);
            }
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
                scls::__Formula* current = to_return.element_at(i, j);
                for(int k = 0;k<a_width;k++) {
                    std::shared_ptr<scls::__Formula_Base> f = element_at(k, j)->clone();
                    f.get()->__multiply(m->element_at(i, k));
                    current->__add(f.get());
                }
            }
        }

        return to_return;
    };

    // Access to an element
    scls::__Formula* Matrix::element_at(int x, int y){return a_elements.at(x * a_height + y).get();};

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

    // Create the elements
    void Matrix::create_elements(){
        a_elements = std::vector<std::shared_ptr<scls::__Formula>>(a_width * a_height);
        for(int i = 0;i<a_width;i++) {
            for(int j = 0;j<a_height;j++) {
                a_elements[i * a_height + j] = std::make_shared<scls::__Formula>(0);
            }
        }
    }
}
