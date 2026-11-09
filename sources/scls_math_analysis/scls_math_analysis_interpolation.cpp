//******************
//
// scls_math_analysis_integration.cpp
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
// This file contains the source code of "scls_math_analysis_integration.h".
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
#include "../../scls_math_directory/scls_math_analysis/scls_math_analysis_interpolation.h"


// The namespace "scls" is used to simplify the all.
namespace scls {

    // Calculate a lagrange polynomial
    std::shared_ptr<Formula_Base_Template<Double>> lagrange_polynomial(std::vector<Double> values, std::size_t current_value) {
        // Check values
        std::vector<Double> values_check;
        for(std::size_t i = 0;i<values.size();i++){
            std::size_t j = 0;
            for(;j<i;j++) {
                if(values_check.at(j) > values.at(i)) {break;}
                else if(values_check.at(j) == values.at(i)){scls::print("Warning", "SCLS Math Carl", "A Lagrange interpolation try to be used with two points with the same abscissa.");return std::shared_ptr<Formula_Base_Template<Double>>();}
            }
            values_check.insert(values_check.begin() + j, values.at(i));
        }

        std::shared_ptr<Formula_Base_Template<Double>> to_return;
        std::string unknown_name = std::string("t");
        for(std::size_t i = 0;i<values.size();i++) {
            if(i != current_value) {
                if(to_return.get() == 0) {
                    to_return = Formula_Base_Template<Double>::new_formula(unknown_name);
                    to_return.get()->substract(values.at(i));
                }
                else {
                    std::shared_ptr<Formula_Base_Template<Double>> m = Formula_Base_Template<Double>::new_formula(unknown_name);
                    m.get()->substract(values.at(i));
                    to_return.get()->multiply(m.get());
                }

                // Division
                std::shared_ptr<Formula_Base_Template<Double>> to_divide = Formula_Base_Template<Double>::new_formula(values.at(current_value));
                to_divide.get()->substract(values.at(i));
                to_return.get()->divide(to_divide.get());
            }
        }

        // Return the result
        return to_return;
    }

    // Calculate the lagrange polynomial interpolator
    std::shared_ptr<Formula_Base_Template<Double>> lagrange_interpolator(std::vector<Point_2D> points) {
        // Get the values
        std::vector<Double> values = std::vector<Double>(points.size());
        for(std::size_t i = 0;i<values.size();i++){values[i]  = points.at(i).x();}

        std::shared_ptr<Formula_Base_Template<Double>> to_return;
        for(std::size_t i = 0;i<values.size();i++) {
            std::shared_ptr<Formula_Base_Template<Double>> to_add = Formula_Base_Template<Double>::new_formula(points.at(i).y());
            to_add.get()->multiply(lagrange_polynomial(values, i).get());

            if(to_return.get() == 0) {to_return = to_add;}
            else {to_return.get()->add(to_add.get());}
        }

        // Return the result
        to_return.get()->simplify();
        return to_return;
    }
}
