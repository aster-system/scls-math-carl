//******************
//
// scls_math_polynomial.cpp
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
// This file contains the source code of scls_math_polynomial.h.
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

// Include the good header file
#include "../scls_math.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
	//
	// The "__Monomonial" class
	//
	//*********

	// Returns the comparaison between two unknows
    bool _Base_Unknown::compare_unknown(_Base_Unknown other) const {if(a_conjugate == other.a_conjugate && a_exponent == other.a_exponent && a_name == other.a_name) {return Unknown_Comparaison::UC_EQUAL;}return Unknown_Comparaison::UC_DIFFERENT;};

    // Returns the inverse the monomonial
    __Monomonial __Monomonial::inverse() const {
        __Monomonial new_monomonial(*this);
        new_monomonial.set_factor(Complex(1) / new_monomonial.factor());
        // Reverse each unknowns
        for(int i = 0;i<static_cast<int>(new_monomonial.a_unknowns.size());i++) {new_monomonial.a_unknowns[i].set_exponent(new_monomonial.a_unknowns[i].exponent() * -1);}
        return new_monomonial;
    };
    // Returns the special limit of the monomonial for an unknown
    scls::Limit __Monomonial::limit_special(Limit needed_limit, std::string unknown_name) {
        scls::Limit to_return;
        _Base_Unknown* unknown = contains_unknown(unknown_name);
        if(unknown != 0) {
            if(needed_limit.is_pi() ||(needed_limit.is_mi() && (unknown->exponent().real().to_double() == round(unknown->exponent().real().to_double()) && static_cast<int>(unknown->exponent().real().to_double()) % 2 == 0))) {
                // PI limits
                if(factor().real() < 0) {to_return.set_mi();}
                else {to_return.set_pi();}
                // Handle zeros
                if(unknown->exponent().real() < 0){
                    if(to_return.is_mi()) {
                        to_return.set_mz();
                    } else {
                        to_return.set_pz();
                    }
                }
            } else if(needed_limit.is_mi()) {
                // MI limits
                if(factor().real() < 0) {to_return.set_pi();}
                else {to_return.set_mi();}
                // Handle zeros
                if(unknown->exponent().real() < 0){
                    if(to_return.is_mi()) {
                        to_return.set_mz();
                    } else {
                        to_return.set_pz();
                    }
                }
            } else {to_return = replace_unknown(unknown_name, __Monomonial(Complex(needed_limit.value()))).factor().real();}
        } else {to_return = factor().real();}
        return to_return;
    };

    // Returns if a monomonial has the same unknows as this one
    bool __Monomonial::compare_unknown(__Monomonial other) {
        if(other.unknowns_number() != unknowns_number()) return false;
        for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
            if(a_unknowns[i].name() == "") {continue;} bool contains_equal = false;
            for(int j = 0;j<static_cast<int>(other.a_unknowns.size());j++) {
                if(a_unknowns[i] == other.a_unknowns[j]) {contains_equal = true;break;}
            } if(!contains_equal) return false;
        } return true;
    };
    // Returns if the monomonial only contains an unkwnown
    _Base_Unknown* __Monomonial::only_contains_unknown(std::string unknown_name, Complex exponent) {
        _Base_Unknown* to_return = 0; unsigned int number = unknowns_number();
        for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
            if((a_unknowns[i].name() == unknown_name && a_unknowns[i].exponent() == exponent)) {
                to_return = &a_unknowns[i];
            } else if(!(number > 0 && a_unknowns[i].name() == "")) {to_return = 0; break;}
        } return to_return;
    };
    // Returns this monomonial where an unknown is replaced by an another unknown
    __Monomonial __Monomonial::replace_unknown(std::string unknown, __Monomonial new_value) {
        __Monomonial final_monomonial = *this;
        _Base_Unknown* unknown_ptr = contains_unknown(unknown);
        if(unknown_ptr!= 0) {
            // The monomonial is a multiplication between the unknown and the base value
            Complex exponent = unknown_ptr->exponent();
            final_monomonial.delete_unknown(unknown);
            // Create the good value
            __Monomonial final_to_add = new_value;
            int needed_exponent = static_cast<int>(exponent.real().to_double());
            for(int i = 1;i<needed_exponent;i++) {final_to_add *= new_value;}
            final_monomonial *= final_to_add;
        }

        return final_monomonial;
    };
    // Returns if two monomonials has the same unknowns
    bool __Monomonial::same_unknowns(__Monomonial monomonial) const {
        std::vector<_Base_Unknown> unknows_1 = a_unknowns;
        std::vector<_Base_Unknown> unknows_2 = monomonial.a_unknowns;
        for(int i = 0;i<static_cast<int>(unknows_1.size());i++){if(unknows_1.at(i).name() == std::string()){unknows_1.erase(unknows_1.begin() + i);i--;}}
        for(int i = 0;i<static_cast<int>(unknows_2.size());i++){if(unknows_2.at(i).name() == std::string()){unknows_2.erase(unknows_2.begin() + i);i--;}}

        if(unknows_1.size() != unknows_2.size()){return false;}int i = 0;
        while(unknows_1.size() > 0 && unknows_2.size() > 0 && i < static_cast<int>(unknows_1.size())) {
            if(unknows_1.at(i) == unknows_2.at(0)) {
                unknows_1.erase(unknows_1.begin() + i);
                unknows_2.erase(unknows_2.begin());
                i = 0;
            }
            else{i++;}
        }
        return unknows_1.size() == 0 && unknows_2.size() == 0;
    }

    // Returns the monomonial to a GLSL calculation
    std::string __Monomonial::to_glsl(Textual_Math_Settings* settings) const {
        if(a_factor == 0){return std::string();}
        std::string to_return = a_factor.to_std_string_simple(settings);
        // Add the unknown
        for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
            std::string current_unknow = a_unknowns.at(i).name();
            if(current_unknow == std::string()){continue;}
            int current_exponent = a_unknowns.at(i).exponent().real().to_double();
            for(;current_exponent>0;current_exponent--) {to_return += std::string("*") + current_unknow;}
        }
        // Returns the result
        return to_return;
    }
    // Returns the monomonial to mathml
    std::string __Monomonial::to_mathml(Textual_Math_Settings* settings) const {
        // Unknowns
        std::string final_unknow = "";
        for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
            final_unknow += std::string("<mi>") + a_unknowns.at(i).name() + std::string("</mi>");
            int real_exponent = a_unknowns.at(i).exponent().real().to_double();
            if(a_unknowns.at(i).name() != "" && a_unknowns.at(i).exponent() != 1){
                final_unknow += std::string("<msup>") + std::to_string(real_exponent) + std::string("</msup>");
            }
        }

        // Factor
        if(a_factor.imaginary() == 0){
            if(a_factor.real().denominator() == 1 || final_unknow == std::string()){return a_factor.to_std_string_simple(settings) + final_unknow;}
            else if(a_factor.real().numerator() == 1) {return final_unknow + std::string("/") + std::to_string(a_factor.real().denominator());}
            else {return std::string("(") + std::to_string(a_factor.real().numerator()) + final_unknow + std::string(")/") + std::to_string(a_factor.real().denominator());}
        }
        else if(a_factor.real() == 0){return a_factor.to_std_string_simple(settings) + final_unknow;}
        return std::string("(") + a_factor.to_std_string_simple(settings) + std::string(")") + final_unknow;
    }
    // Returns the monomonial converted to std::string
    std::string __Monomonial::to_std_string(Textual_Math_Settings* settings) const {
        std::string final_unknow = "";
        for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
            final_unknow += a_unknowns.at(i).name();
            int real_exponent = a_unknowns.at(i).exponent().real().to_double();
            if(a_unknowns.at(i).name() != "" && a_unknowns.at(i).exponent() != 1){
                if(real_exponent > 0){for(int j = 1;j<real_exponent;j++){final_unknow += std::string("*") + a_unknowns.at(i).name();}}
                else{final_unknow += std::string("^") + std::to_string(real_exponent);}
            }
        }
        if(a_factor.imaginary() == 0){
            if(a_factor.real().denominator() == 1 || final_unknow == std::string()){return a_factor.to_std_string_simple(settings) + final_unknow;}
            else if(a_factor.real().numerator() == 1) {return final_unknow + std::string("/") + std::to_string(a_factor.real().denominator());}
            else {return std::string("(") + std::to_string(a_factor.real().numerator()) + final_unknow + std::string(")/") + std::to_string(a_factor.real().denominator());}
        }
        else if(a_factor.real() == 0){return a_factor.to_std_string_simple(settings) + final_unknow;}
        return std::string("(") + a_factor.to_std_string_simple(settings) + std::string(")") + final_unknow;
    };

    // Multiplication operator assignment
    __Monomonial& __Monomonial::operator*=(__Monomonial const& obj) {
        a_factor *= obj.a_factor;
        for(int i = 0;i<static_cast<int>(obj.a_unknowns.size());i++) {
            _Base_Unknown* contained_unknown = contains_unknown(obj.a_unknowns[i].name());
            if(contained_unknown == 0) {
                a_unknowns.push_back(obj.a_unknowns[i]);
            }
            else if(obj.a_unknowns[i].name() != "") {
                scls::Complex new_exponent = contained_unknown->exponent() + obj.a_unknowns[i].exponent();
                if(new_exponent == 0) {
                    delete_unknown(contained_unknown);
                } else {contained_unknown->set_exponent(new_exponent);}
            }
        } return *this;
    }

    // Stream operator overloading
    std::ostream& operator<<(std::ostream& os, const __Monomonial& obj) { os << obj.to_std_string(0) ; return os; }

    //*********
	//
	// The "Polynomial" class
	//
	//*********

	// Converts a polynomial to a complex
    std::shared_ptr<Polynomial::Polynomial_Complex> Polynomial::Polynomial_Complex::from_polynomial(Polynomial* polynomial) {
        // Create the polynomial complex
        std::shared_ptr<Polynomial::Polynomial_Complex> needed_polynomial = std::make_shared<Polynomial::Polynomial_Complex>();

        // Handle each monomonial
        Polynomial* imaginary_polynomial = needed_polynomial.get()->imaginary_polynomial();
        Polynomial* real_polynomial = needed_polynomial.get()->real_polynomial();
        for(int i = 0;i<static_cast<int>(polynomial->monomonials().size());i++) {
            // Real and imaginary factor
            scls::__Monomonial& needed_monomonial = polynomial->monomonials()[i];
            scls::Fraction imaginary_factor = needed_monomonial.factor().imaginary();
            scls::Fraction real_factor = needed_monomonial.factor().real();

            // Add the needed factors
            scls::__Monomonial imaginary_monomonial = needed_monomonial;
            imaginary_monomonial.set_factor(imaginary_factor);
            scls::__Monomonial real_monomonial = needed_monomonial;
            real_monomonial.set_factor(real_factor);
            (*imaginary_polynomial) += imaginary_monomonial;
            (*real_polynomial) += real_monomonial;
        }

        // Return the needed polynomial
        return needed_polynomial;
    }

	// Returns the limit of a (only monomonial) Polynomial for an unknown
    scls::Limit Polynomial::limit(Limit needed_limit, std::string unknown_name) {
        scls::Limit to_return;
        if(is_simple_monomonial()) {
            __Monomonial& needed_monomonial = a_monomonials[0];
            _Base_Unknown* unknown = needed_monomonial.contains_unknown(unknown_name);
            if(unknown != 0) {
                if(needed_limit.is_special()) {to_return = needed_monomonial.limit_special(needed_limit, unknown_name);}
                else {
                    // Other value limit
                    Polynomial modified = replace_unknown(unknown_name, needed_limit.value());
                    if(modified.a_monomonials.size() > 0) to_return = modified.a_monomonials.at(0).factor().real();
                }
            }
            else {to_return = needed_monomonial.factor().real();}
        }
        return to_return;
    };

    // Add a new monomonial to the Polynomial
    void Polynomial::add_monomonial(__Monomonial new_monomonial) {
        if(new_monomonial.factor() == 0){return;}

        // Add the monomonial
        __Monomonial* same_monomonial = contains_monomonial(new_monomonial);
        if(same_monomonial == 0){a_monomonials.push_back(new_monomonial);}
        else{
            if(same_monomonial->factor() + new_monomonial.factor() == 0) {
                // Remove the empty monomonial
                for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                    if(a_monomonials[i].compare_unknown(*same_monomonial)) {
                        a_monomonials.erase(a_monomonials.begin() + i, a_monomonials.begin() + i + 1);
                    }
                }
            }
            else {
                // Add the monomonial
                same_monomonial->set_factor(same_monomonial->factor() + new_monomonial.factor());
            }
        }
    };

    // Returns a monomonial by its unknown
    __Monomonial Polynomial::monomonial(std::string unknown, Complex exponent) {
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            _Base_Unknown* result = a_monomonials.at(i).only_contains_unknown(unknown, exponent);
            if(result != 0) {return a_monomonials.at(i);}
        }
        __Monomonial to_return(0); to_return.add_unknown(unknown, exponent); return to_return;
    };
    // Returns a list of unknowns monomonials
    std::vector<__Monomonial> Polynomial::unknown_monomonials() const {
        std::vector<__Monomonial> to_return;
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(a_monomonials.at(i).unknowns_number() > 0) to_return.push_back(a_monomonials.at(i));
        }
        return to_return;
    };

    // Returns the Polynomial to a GLSL function
    std::string Polynomial::to_glsl(Textual_Math_Settings* settings) const {
        std::string to_return = std::string("float poly(float x){float y = ");
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(a_monomonials.at(i).factor() != 0) {
                std::string to_add = a_monomonials.at(i).to_glsl(settings);
                to_return += to_add;
                if(to_add != std::string() && i < static_cast<int>(a_monomonials.size()) - 1) {
                    to_return += " + ";
                }
            }
        }
        to_return += std::string(";return y;}");
        return to_return;
    }
     // Returns the Polynomial to mathml
    std::string Polynomial::to_mathml(Textual_Math_Settings* settings) const {
        // Asserts
        if(static_cast<int>(a_monomonials.size()) == 0){return std::string("<mi>0</mi>");}

        // Generates text
        std::string to_return = "";
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(a_monomonials.at(i).factor() != 0) {
                to_return += a_monomonials.at(i).to_mathml(settings);
                if(i < static_cast<int>(a_monomonials.size()) - 1) {
                    to_return += std::string("<mo>+</mo>");
                }
            }
        }
        while(to_return[to_return.size() - 1] == '+' || to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
        return to_return;
    }
    // Returns the Polynomial to std::string
    std::string Polynomial::to_std_string(Textual_Math_Settings* settings) const {
        // Asserts
        if(static_cast<int>(a_monomonials.size()) == 0){return std::string("0");}

        std::string to_return = "";
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(a_monomonials.at(i).factor() != 0) {
                to_return += a_monomonials.at(i).to_std_string(settings);
                if(i < static_cast<int>(a_monomonials.size()) - 1) {
                    to_return += " + ";
                }
            }
        }
        while(to_return[to_return.size() - 1] == '+' || to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
        return to_return;
    };

    // Methods operators
    // Add a Polynomial to this one
    void Polynomial::__add(Polynomial value) {
        // Asserts / speed
        if(value.monomonials().size() <= 0 || (value.monomonials().size() == 1 && value.monomonials().at(0).factor() == 0)){return;}
        if(value.monomonials().size() == 1 && monomonials().size() == 1 && value.monomonials().at(0).is_known() && monomonials().at(0).is_known()){monomonials()[0].add_to_factor(value.monomonials().at(0).factor());return;}

        // Add the monomonial
        for(int i = 0;i<static_cast<int>(value.a_monomonials.size());i++) {
            __Monomonial& current_monomonial = value.a_monomonials[i];
            __Monomonial* contained_monomonial = contains_monomonial(current_monomonial);
            if(contained_monomonial == 0) {
                add_monomonial(current_monomonial);
            } else {
                (*contained_monomonial) += current_monomonial;
            }
        }
    };

    // Divide a monomonial to this void
    void Polynomial::__divide(__Monomonial value) {__Monomonial used_inverse = value.inverse();for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {a_monomonials[i] *= used_inverse;}};
    void Polynomial::__divide(Complex value) {__divide(__Monomonial(value));};
    void Polynomial::__divide(Fraction value) {__divide(__Monomonial(value));};

    // Divide the Polynomial by an another Polynomial
    void Polynomial::__divide(Polynomial* value) {
        // The value is a simple monomonial
        if(value->is_simple_monomonial()) {__divide(value->monomonial());}
    }

    // Multiply a Polynomial to this one
    void Polynomial::__multiply(Polynomial value) {
        std::vector<Polynomial> created_Polynomial;
        // Apply each addition
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            Polynomial current_Polynomial;
            current_Polynomial.add_monomonial(a_monomonials[i]);
            // Apply each multiplication
            for(int j = 0;j<static_cast<int>(value.a_monomonials.size());j++) {
                __Monomonial& current_monomonial = value.a_monomonials[j];
                Polynomial needed_polynomial = current_Polynomial;
                if(needed_polynomial.a_monomonials.size() > 0) {
                    needed_polynomial.a_monomonials[0] *= current_monomonial;
                    created_Polynomial.push_back(needed_polynomial);
                }
            }
        }
        // Apply the multiplication
        a_monomonials.clear();
        for(int i = 0;i<static_cast<int>(created_Polynomial.size());i++) {
            __add(created_Polynomial[i]);
        }
    };

    // Returns the maximum degree in the Polynomial
    Complex Polynomial::degree(std::string unknown_name) {
        Complex max_degree = 0;
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(a_monomonials[i].contains_unknown(unknown_name) && a_monomonials[i].unknowns_number() == 1) {
                Complex current_factor = a_monomonials[i].unknown()->exponent();
                if(current_factor.real() > max_degree.real()) max_degree = current_factor;
            }
        } return max_degree;
    };

    // Returns a Polynomial from a monomonial where the unknows has been replaced
    Polynomial Polynomial::polynomial_from_modified_monomonial_unknows(__Monomonial used_monomonial, std::string unknown, Polynomial new_value) {
        Polynomial final_Polynomial = Polynomial();

        _Base_Unknown* unknown_ptr = used_monomonial.contains_unknown(unknown);
        if(unknown_ptr!= 0) {
            // The monomonial is a multiplication between the unknown and the base value
            Complex exponent = unknown_ptr->exponent();
            used_monomonial.delete_unknown(unknown);
            final_Polynomial += used_monomonial;
            // Create the good value
            Polynomial final_to_add = new_value;
            int needed_exponent = static_cast<int>(exponent.real().to_double());
            if(needed_exponent > 0) {for(int i = 1;i<needed_exponent;i++) {final_to_add *= new_value;}}
            else{final_to_add=1;for(int i = 0;i<-needed_exponent;i++) {final_to_add.__divide(&new_value);}}
            final_Polynomial *= final_to_add;
        } else {
            // The unknown is not a multiplication of the base value
            final_Polynomial += used_monomonial;
        }

        return final_Polynomial;
    };

    // Returns this Polynomial where an unknown is replaced by an another unknown
    Polynomial Polynomial::replace_unknown(std::string unknown, Polynomial new_value) const {
        Polynomial final_Polynomial = Polynomial();

        // Analyse each monomonial
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            Polynomial current_polynomial = polynomial_from_modified_monomonial_unknows(a_monomonials.at(i), unknown, new_value);
            final_Polynomial += current_polynomial;
        }

        return final_Polynomial;
    };

    // Simplify the Polynomial
    Polynomial Polynomial::simplify() const {
        Polynomial poly(*this);
        for(int i = 0;i<static_cast<int>(poly.a_monomonials.size());i++) {
            if(poly.a_monomonials[i].factor() == 0) {
                poly.a_monomonials.erase(poly.a_monomonials.begin() + i);
                i--;
            }
        }
        return poly;
    };

    // Operators
    // With Complex
    bool Polynomial::__is_equal(Complex value) const {
        if(value.real() == 0 && value.imaginary() == 0) {
            return (a_monomonials.size() == 0) || (a_monomonials.size() == 1 && a_monomonials.at(0).factor().real() == 0 && a_monomonials.at(0).factor().imaginary() == 0);
        }
        return (a_monomonials.size() == 1 && a_monomonials.at(0).factor() == value);
    };
    // With Polynomial
    bool Polynomial::__is_equal(Polynomial value) const {return __is_equal(&value);}
    bool Polynomial::__is_equal(Polynomial* value) const {
        std::vector<__Monomonial> monomonials_1 = value->a_monomonials;
        std::vector<__Monomonial> monomonials_2 = a_monomonials;
        for(int i = 0;i<static_cast<int>(monomonials_1.size());i++){if(monomonials_1.at(i).factor() == 0){monomonials_1.erase(monomonials_1.begin() + i);i--;}}
        for(int i = 0;i<static_cast<int>(monomonials_2.size());i++){if(monomonials_2.at(i).factor() == 0){monomonials_2.erase(monomonials_2.begin() + i);i--;}}
        if(monomonials_1.size() != monomonials_2.size()){return false;}int i = 0;
        while(monomonials_1.size() > 0 && monomonials_2.size() > 0 && i < static_cast<int>(monomonials_1.size())) {
            if(monomonials_1.at(i) == monomonials_2.at(0)) {
                monomonials_1.erase(monomonials_1.begin() + i);
                monomonials_2.erase(monomonials_2.begin());
                i = 0;
            }
            else{i++;}
        }
        return monomonials_1.size() == 0 && monomonials_2.size() == 0;
    };

    // Stream operator overloading
    std::ostream& operator<<(std::ostream& os, Polynomial& obj) {os << obj.to_std_string(0); return os; }
	// Divide operator
    Polynomial operator/(__Monomonial& obj, __Monomonial const& other) {
        Polynomial to_return;
        to_return.add_monomonial(__Monomonial(other.factor() / obj.factor(), obj.unknowns()));
        return to_return;
    }

    //*********
	//
	// The sets class
	//
	//*********

	// Returns the intersection between this interval and an another interval
    Interval Interval::intersection(Interval other) {
        // Empty interval
        if((other.a_start <= a_start && other.a_end < a_start && !a_start_infinite && !other.a_end_infinite) || (a_start <= other.a_start && a_end < other.a_start && !a_end_infinite && !other.a_start_infinite)){return Interval(0, false, 0, false);}
        else if(other.a_end == a_start && !a_start_infinite && !other.a_end_infinite) {
            if(other.a_end_included & a_start_included){return Interval(a_start);}
            else{return Interval(0, false, 0, false);}
        }
        else if(other.a_start == a_end && !other.a_start_infinite && !a_end_infinite) {
            if(other.a_start_included & a_end_included){return Interval(a_end);}
            else{return Interval(0, false, 0, false);}
        }

        // There is an intersection
        // Get the end
        Fraction needed_end; bool needed_end_included = false; bool needed_end_infinite = false;
        if(other.a_end_infinite && a_end_infinite){needed_end_infinite=true;}
        else if(other.a_end > a_end || other.a_end_infinite){needed_end=a_end;needed_end_included=a_end_included;}
        else if(a_end > other.a_end ||a_end_infinite){needed_end=other.a_end;needed_end_included=other.a_end_included;}
        else {needed_end=a_end;needed_end_included=a_end_included&&other.a_end_included;}
        // Get the start
        Fraction needed_start; bool needed_start_included = false; bool needed_start_infinite = false;
        if(other.a_start_infinite && a_start_infinite){needed_start_infinite=true;}
        else if(other.a_start < a_start || other.a_start_infinite){needed_start=a_start;needed_start_included=a_start_included;}
        else if(a_start < other.a_start || a_start_infinite){needed_start=other.a_start;needed_start_included=other.a_start_included;}
        else {needed_start=a_start;needed_start_included=a_start_included&&other.a_start_included;}

        // Returns the result
        Interval to_return = Interval(needed_start, needed_start_included, needed_end, needed_end_included);
        to_return.set_end_infinite(needed_end_infinite);to_return.set_start_infinite(needed_start_infinite);
        return to_return;
    }

    // Returns the Interval to an std::string
    std::string Interval::end_to_std_string(Textual_Math_Settings* settings)const{if(a_end_infinite){return std::string("+inf");}return a_end.to_std_string(settings);}
    std::string Interval::start_to_std_string(Textual_Math_Settings* settings)const{if(a_start_infinite){return std::string("-inf");}return a_start.to_std_string(settings);}
    std::string Interval::to_std_string(Textual_Math_Settings* settings) {
        std::string to_return = std::string();

        if(a_start_infinite||!a_start_included){to_return+=std::string("]");}
        else{to_return+=std::string("[");}

        to_return+=start_to_std_string(settings);
        to_return+=std::string(";");
        to_return+=end_to_std_string(settings);

        if(a_end_infinite||!a_end_included){to_return+=std::string("[");}
        else{to_return+=std::string("]");}

        return to_return;
    };

	// Compares this definition set with another
    bool Set_Number::compare(Set_Number value) {
        // Compare size
        if(a_numbers.size() != value.a_numbers.size() || a_intervals.size() != value.a_intervals.size()){return false;}

        // Compare intervals and values
        for(int i = 0;i<static_cast<int>(a_intervals.size())&&i<static_cast<int>(value.a_intervals.size());i++){if(a_intervals[i] != value.a_intervals[i]){return false;}}
        for(int i = 0;i<static_cast<int>(a_numbers.size())&&i<static_cast<int>(value.a_numbers.size());i++){if(a_intervals[i] != value.a_intervals[i]){return false;}}
        return true;
    }

    // Checks the intervals / numbers
    void Set_Number::check_intervals() {
        // Check the end (infinite)
        for(int i = 1;i<static_cast<int>(a_intervals.size());i++) {
            int current_i = (a_intervals.size() - (i + 1));
            if(a_intervals.at(current_i).end_infinite() && a_intervals.at(current_i + 1).end_infinite()) {
                if(a_intervals.at(current_i).start_infinite() || a_intervals.at(current_i + 1).start_infinite()){a_intervals.at(current_i).set_start_infinite(true);}
                else if(a_intervals.at(current_i).start() > a_intervals.at(current_i + 1).start()){
                    a_intervals.at(current_i).set_start(a_intervals.at(current_i + 1).start());
                    a_intervals.at(current_i).set_start_included(a_intervals.at(current_i + 1).start_included());
                }
                else if(a_intervals.at(current_i).start() == a_intervals.at(current_i + 1).start()){a_intervals.at(current_i).set_start_included(a_intervals.at(current_i).start_included() || a_intervals.at(current_i + 1).start_included());}
                a_intervals.pop_back();i--;
            }
        }

        // Check the start (infinite)
        for(int i = 1;i<static_cast<int>(a_intervals.size());i++) {
            if(a_intervals.at(i - 1).start_infinite() && a_intervals.at(i).start_infinite()) {
                if(a_intervals.at(i - 1).end_infinite() || a_intervals.at(i).end_infinite()){a_intervals.at(i - 1).set_end_infinite(true);}
                else if(a_intervals.at(i - 1).end() < a_intervals.at(i).end()){
                    a_intervals.at(i - 1).set_end(a_intervals.at(i).end());
                    a_intervals.at(i - 1).set_end_included(a_intervals.at(i).end_included());
                }
                else if(a_intervals.at(i - 1).end() == a_intervals.at(i).end()){a_intervals.at(i - 1).set_end_included(a_intervals.at(i - 1).end_included() || a_intervals.at(i).end_included());}
                a_intervals.erase(a_intervals.begin() + i);i--;
            }
        }

        for(int i = 0;i<static_cast<int>(a_intervals.size() - 1);i++) {
            if(a_intervals[i].is_empty() || (a_intervals[i].start() > a_intervals[i].end() && !a_intervals[i].start_infinite() && !a_intervals[i].end_infinite())){a_intervals.erase(a_intervals.begin() + i);i--;}
            else if(a_intervals[i].end() > a_intervals[i + 1].start() || (a_intervals[i].end() == a_intervals[i + 1].start() && (a_intervals[i].end_included() || a_intervals[i + 1].start_included()))){
                scls::Interval new_interval = scls::Interval(a_intervals[i].start(), a_intervals[i].start_included(), a_intervals[i + 1].end(), a_intervals[i + 1].end_included());
                // Set the start
                if(a_intervals[i].start_infinite() || a_intervals[i + 1].start_infinite()){new_interval.set_start_infinite(true);}
                else if(a_intervals[i + 1].start() < a_intervals[i].start()){new_interval.set_start(a_intervals[i + 1].start());new_interval.set_start_included(a_intervals[i + 1].start_included());}
                else if(a_intervals[i + 1].start() == a_intervals[i].start()){new_interval.set_start_included(new_interval.start_included() && a_intervals[i + 1].start_included());}
                // Set the end
                if(a_intervals[i].end_infinite() || a_intervals[i + 1].end_infinite()){new_interval.set_end_infinite(true);}
                else if(a_intervals[i].end() > a_intervals[i + 1].end()){new_interval.set_end(a_intervals[i].end());new_interval.set_end_included(a_intervals[i].end_included());}
                else if(a_intervals[i + 1].end() == a_intervals[i].end()){new_interval.set_end_included(new_interval.start_included() && a_intervals[i].start_included());}
                // Finalize the creation
                a_intervals[i] = new_interval;
                a_intervals.erase(a_intervals.begin() + i + 1);
                i--;
            }
        }
    }
    // Excludes a value
    void Set_Number::exclude(scls::Fraction number) {
        for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
            if(a_intervals.at(i).end() >= number || a_intervals.at(i).end_infinite()) {
                if(a_intervals.at(i).start() <= number || a_intervals.at(i).start_infinite()) {
                    if(a_intervals.at(i).start() != a_intervals.at(i).end() || a_intervals.at(i).end_infinite() || a_intervals.at(i).start_infinite()) {
                        // Create a new interval
                        a_intervals.insert(a_intervals.begin() + i + 1, a_intervals.at(i));

                        // Edit the first interval
                        a_intervals.at(i).set_end(number);
                        a_intervals.at(i).set_end_included(false);
                        a_intervals.at(i).set_end_infinite(false);

                        // Edit the second interval
                        a_intervals.at(i + 1).set_start(number);
                        a_intervals.at(i + 1).set_start_included(false);
                        a_intervals.at(i + 1).set_start_infinite(false);
                    }
                    else {a_intervals.erase(a_intervals.begin() + i);}

                    // Checks the interval
                    check_intervals();
                    return;
                }
                else{return;}
            }
        }
    }
    void Set_Number::exclude(Interval interval) {
        if(interval.start_infinite() && interval.end_infinite()){clear();}
        else if(interval.start_infinite()) {
            // Start infinite
            for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
                if(a_intervals.at(i).start() > interval.end() && !a_intervals.at(i).start_infinite()){break;}
                else if(a_intervals.at(i).start() == interval.end() && !a_intervals.at(i).start_infinite()){break;}
                else if((a_intervals.at(i).start() < interval.end() || a_intervals.at(i).start_infinite()) && (interval.end() < a_intervals.at(i).end() || a_intervals.at(i).end_infinite())){
                    a_intervals[i].set_start(interval.end());
                    a_intervals[i].set_start_included(!interval.end_included());
                    a_intervals[i].set_start_infinite(false);
                    break;
                }
                else{a_intervals.erase(a_intervals.begin());i--;}
            }
        }
        else if(interval.end_infinite()) {
            // End infinite
            for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
                int current_i = static_cast<int>(a_intervals.size()) - (i + 1);
                if(a_intervals.at(current_i).end() < interval.start() && !a_intervals.at(current_i).end_infinite()){break;}
                else if(a_intervals.at(current_i).end() == interval.start() && !a_intervals.at(current_i).end_infinite()){break;}
                else if((a_intervals.at(current_i).end() > interval.start() || a_intervals.at(current_i).end_infinite()) && (a_intervals.at(current_i).start() < interval.start() || a_intervals.at(current_i).start_infinite())){
                    a_intervals[current_i].set_end(interval.start());
                    a_intervals[current_i].set_end_included(!interval.start_included());
                    a_intervals[current_i].set_end_infinite(false);
                    break;
                }
                else{a_intervals.pop_back();}
            }
        }
        else {
            // Go to the next interval
            int i = 0;
            while(i < static_cast<int>(a_intervals.size()) && a_intervals.at(i).end() < interval.start()){i++;}
            while(i < static_cast<int>(a_intervals.size()) && a_intervals.at(i).start() < interval.end()){
                if(a_intervals.at(i).start() < interval.start()){
                    a_intervals[i].set_end(interval.start());a_intervals[i].set_end_included(interval.start_included());
                    i++;
                }
                else{a_intervals.erase(a_intervals.begin() + i);}
            }
        }
    }
    void Set_Number::exclude(scls::Set_Number* set_number) {for(int i = 0;i<static_cast<int>(set_number->a_intervals.size());i++){exclude(set_number->a_intervals.at(i));}}
    // Returns the intersection between this set and an interval
    Set_Number Set_Number::intersection(Interval other) {
        Set_Number to_return;

        // Calculate all the intersections
        for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
            Interval current_intersection = a_intervals[i].intersection(other);
            if(!current_intersection.is_empty()){to_return.add_interval(current_intersection);}
        }

        return to_return;
    }
    Set_Number Set_Number::intersection(Set_Number other) {
        Set_Number to_return;

        // Calculate all the intersections
        for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
            Set_Number current_intersection = other.intersection(a_intervals[i]);
            if(!current_intersection.is_empty()){for(int j = 0;j<static_cast<int>(current_intersection.a_intervals.size());j++){to_return.add_interval(current_intersection.a_intervals.at(j));}}
        }

        // Calculate all the isolated values
        for(int i = 0;i<static_cast<int>(to_return.a_intervals.size());i++) {
            for(int j = 0;j<static_cast<int>(a_numbers.size());j++) {
                Set_Number current_intersection = other.intersection(a_intervals[i]);
                if(!current_intersection.is_empty()){for(int j = 0;j<static_cast<int>(current_intersection.a_intervals.size());j++){to_return.add_interval(current_intersection.a_intervals.at(j));}}
            }
        }

        return to_return;
    }
	// Comparaison function for the interval sorting
    bool __sort_interval_function(const Interval& i_1, const Interval& i_2) {if(!i_1.start_infinite() && i_2.start_infinite()){return false;} return i_1.start() < i_2.start() || (i_1.start() == i_2.start() && (i_2.start_included() || !i_1.start_included()));};
	// Comparaison function for the numbers sorting
    bool __sort_numbers_function(const Complex& i_1, const Complex& i_2) {return i_1.real() < i_2.real();};
    // Sort the intervals / numbers in the set
    void Set_Number::__sort_interval() {std::sort(a_intervals.begin(), a_intervals.end(), __sort_interval_function);};
    void Set_Number::__sort_numbers() {std::sort(a_numbers.begin(), a_numbers.end(), __sort_numbers_function);};
    // Returns the set in a std::string
    std::string Set_Number::to_std_string(Textual_Math_Settings* settings) {
        std::string to_return = "";

        // Add the isolated elements
        for(int i = 0;i<static_cast<int>(numbers().size());i++) {
            to_return += numbers().at(i).to_std_string_simple(settings);
            if(i < static_cast<int>(numbers().size()) - 1){to_return += std::string(";");}
        }

        // Add the intervals
        if(to_return != std::string()){to_return = std::string("{") + to_return + std::string("}");}
        for(int i = 0;i<static_cast<int>(intervals().size());i++) {
            if(to_return != std::string()){to_return += std::string(" U ");}
            to_return += a_intervals.at(i).to_std_string(settings);
        }

        return to_return;
    }
    // Unites with an another set
    void Set_Number::unite(Interval number) {add_interval(number);}
    void Set_Number::unite(Set_Number* set_number) {for(int i = 0;i<static_cast<int>(set_number->a_intervals.size());i++){if(!set_number->a_intervals.at(i).is_empty()){a_intervals.push_back(set_number->a_intervals.at(i));}}__sort_interval();check_intervals();}

    // Returns the complement between this sets and the real set (and another one if needed)
    Set_Number Set_Number::complement_real(Set_Number* set_number) {return scls::Set_Number::real().exclusion(reunion(set_number));}
    // Returns the complement between two sets
    Set_Number Set_Number::complement_relative(Set_Number* set_number) {Set_Number to_return = *this;return to_return;}
    Set_Number Set_Number::complement_relative_symetrical(Set_Number* set_number){return intersection(*set_number).reunion(complement_real(set_number));};
}
