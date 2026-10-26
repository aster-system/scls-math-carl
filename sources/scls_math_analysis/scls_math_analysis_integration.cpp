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
#include "../../scls_math_directory/scls_math_analysis/scls_math_analysis_integration.h"


// The namespace "scls" is used to simplify the all.
namespace scls {

    // Approximate integrals
    std::shared_ptr<Formula_Base_Template<Double>> integration_rectangle(std::shared_ptr<Formula_Base_Template<Double>> function, std::shared_ptr<Formula_Base_Template<Double>> interval_start, std::shared_ptr<Formula_Base_Template<Double>> interval_end, int rectangles) {
        std::shared_ptr<Formula_Base_Template<Double>> result = Formula_Base_Template<Double>::new_formula(Double(0));

        std::shared_ptr<Formula_Base_Template<Double>> step = interval_end.get()->clone();
        step.get()->substract(interval_start.get());step.get()->divide(Double(rectangles));

        for(int i = 0;i<rectangles;i++) {
            std::string unknown_name = std::string("x");
            std::shared_ptr<Formula_Base_Template<Double>> new_step = step.get()->clone();new_step.get()->multiply(Double(i));new_step.get()->add(interval_start.get());
            std::shared_ptr<Formula_Base_Template<Double>> value = function.get()->replace_unknowns(unknown_name, new_step);
            value.get()->multiply(step.get());
            result.get()->add(value.get());
        }

        return result;
    }
    std::shared_ptr<Formula_Base_Template<Double>> integration_trapezoid(std::shared_ptr<Formula_Base_Template<Double>> function, std::shared_ptr<Formula_Base_Template<Double>> interval_start, std::shared_ptr<Formula_Base_Template<Double>> interval_end, int rectangles) {
        std::shared_ptr<Formula_Base_Template<Double>> result = Formula_Base_Template<Double>::new_formula(Double(0));

        std::shared_ptr<Formula_Base_Template<Double>> step = interval_end.get()->clone();
        step.get()->substract(interval_start.get());step.get()->divide(Double(rectangles));

        std::vector<std::shared_ptr<Formula_Base_Template<Double>>> needed_steps = std::vector<std::shared_ptr<Formula_Base_Template<Double>>>(rectangles + 1);
        std::vector<std::shared_ptr<Formula_Base_Template<Double>>> needed_value = std::vector<std::shared_ptr<Formula_Base_Template<Double>>>(rectangles + 1);
        for(int i = 0;i<rectangles+1;i++) {
            std::shared_ptr<Formula_Base_Template<Double>> new_step = step.get()->clone();new_step.get()->multiply(Double(i));new_step.get()->add(interval_start.get());
            needed_steps[i] = new_step;
        }
        for(int i = 0;i<rectangles;i++) {
            std::string unknown_name = std::string("x");
            std::shared_ptr<Formula_Base_Template<Double>> value = function.get()->replace_unknowns(unknown_name, needed_steps.at(i));
            value.get()->add(function.get()->replace_unknowns(unknown_name, needed_steps.at(i + 1)).get());
            value.get()->divide(Double(2));
            value.get()->multiply(step.get());
            result.get()->add(value.get());
        }

        return result;
    }
}
