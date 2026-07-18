//******************
//
// scls_math_analysis_limit.h
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
// This file contains some tools to calculate limits with SCLS.
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

// The namespace "scls" is used to simplify the all.
namespace scls {
    // Struct representing a limit
    struct Limit {
        Limit(scls::Fraction v){value = v;};

        bool exists = true;
        char infinity = 0;
        scls::Fraction value = 0;

        // Limit sign
        char sign() {
            if(infinity != 0){return infinity;}
            return scls::sign(value.to_double());
        }
    };

    static Limit formula_limit(scls::Formula_Base* f, Limit to) {
        Limit to_return = Limit(0);
        if(f->is_known()){to_return = Limit(*f->value<scls::Fraction>());}
        else if(f->is_final_element()) {to_return = to;}
        else {
            bool first_limit = true;

            if(f->algebra_operator_name() == std::string_view("+")) {
                for(std::size_t i = 0;i<f->algebra_elements_const().size();i++) {
                    scls::Formula_Base* current_f = f->formula_element(i);
                    Limit current_limit = formula_limit(current_f, to);
                    if(first_limit){to_return = current_limit;first_limit = false;}
                    else {
                        if(to_return.infinity * current_limit.infinity == -1) {
                            to_return.exists = false;
                            break;
                        }
                        else {
                            if(to_return.infinity == 0){to_return.infinity = current_limit.infinity;}
                            to_return.value += current_limit.value;
                        }
                    }
                }
            }
            else if(f->algebra_operator_name() == std::string_view("*")) {
                for(std::size_t i = 0;i<f->algebra_elements_const().size();i++) {
                    scls::Formula_Base* current_f = f->formula_element(i);
                    Limit current_limit = formula_limit(current_f, to);

                    if(first_limit){to_return = current_limit;first_limit = false;}
                    else {
                        if((std::abs(to_return.infinity) == 1 && (current_limit.infinity == 0 && current_limit.value == 0)) || (std::abs(current_limit.infinity) == 1 && (to_return.infinity == 0 && to_return.value == 0))) {
                            to_return.exists = false;
                            break;
                        }
                        else {
                            if(to_return.infinity == 0){to_return.infinity = current_limit.infinity;}
                            else if(current_limit.infinity != 0){to_return.infinity *= current_limit.infinity;}
                            to_return.value *= current_limit.value;
                        }
                    }
                }
            }
            else if(f->algebra_operator_name() == std::string_view("/")) {
                scls::Formula_Base* denominator = f->formula_element(1);
                scls::Formula_Base* numerator = f->formula_element(0);
                Limit denominator_limit = formula_limit(denominator, to);
                Limit numerator_limit = formula_limit(numerator, to);

                std::cout << "A " << numerator->to_std_string(0) << " " << (int)numerator_limit.infinity << " " << (int)denominator_limit.infinity << " " << to.value.to_double() << " " << (int)to.infinity << " " << to.exists << std::endl;
                std::cout << "B " << numerator_limit.value.to_double() << " " << (int)numerator_limit.infinity << " " << (int)denominator_limit.infinity << std::endl;
                if(denominator_limit.infinity == 0 && numerator_limit.infinity == 0){
                    if(denominator_limit.value == 0 && numerator_limit.value == 0){
                        to_return.exists = false;
                    }
                    else if(denominator_limit.value == 0){
                        to_return.infinity = scls::sign(numerator_limit.value.to_double());
                    }
                    else if(numerator_limit.value == 0) {
                        to_return.value = 0;
                    }
                    else {
                        to_return.value = numerator_limit.value / denominator_limit.value;
                    }
                }
                else if(denominator_limit.infinity != 0 && numerator_limit.infinity != 0) {
                    to_return.exists = false;
                }
                else {
                    to_return.infinity = scls::sign(denominator_limit.sign() * numerator_limit.sign());
                }
            }
        }

        return to_return;
    }
}
