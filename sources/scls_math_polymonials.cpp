//******************
//
// scls_math_polymonial.cpp
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
// This file contains the source code of scls_math_polymonial.h.
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

        if(unknows_1.size() != unknows_2.size()){return false;}int i = 0;
        while(i < static_cast<int>(unknows_1.size())) {
            if(unknows_1.at(i) == unknows_2.at(0)) {
                unknows_1.erase(unknows_1.begin() + i);
                unknows_2.erase(unknows_2.begin());
                i++;
            }
            else{i = 0;}
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
        if(a_factor.real() == 0 || a_factor.imaginary() == 0) return a_factor.to_std_string_simple(settings) + final_unknow;
        return "(" + a_factor.to_std_string_simple(settings) + ")" + final_unknow;
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
	// The "Polymonial" class
	//
	//*********

	// Converts a polymonialto a complex
    std::shared_ptr<Polymonial::Polymonial_Complex> Polymonial::Polymonial_Complex::from_polymonial(Polymonial* polymonial) {
        // Create the polymonial complex
        std::shared_ptr<Polymonial::Polymonial_Complex> needed_polymonial = std::make_shared<Polymonial::Polymonial_Complex>();

        // Handle each monomonial
        Polymonial* imaginary_polymonial = needed_polymonial.get()->imaginary_polymonial();
        Polymonial* real_polymonial = needed_polymonial.get()->real_polymonial();
        for(int i = 0;i<static_cast<int>(polymonial->monomonials().size());i++) {
            // Real and imaginary factor
            scls::__Monomonial& needed_monomonial = polymonial->monomonials()[i];
            scls::Fraction imaginary_factor = needed_monomonial.factor().imaginary();
            scls::Fraction real_factor = needed_monomonial.factor().real();

            // Add the needed factors
            scls::__Monomonial imaginary_monomonial = needed_monomonial;
            imaginary_monomonial.set_factor(imaginary_factor);
            scls::__Monomonial real_monomonial = needed_monomonial;
            real_monomonial.set_factor(real_factor);
            (*imaginary_polymonial) += imaginary_monomonial;
            (*real_polymonial) += real_monomonial;
        }

        // Return the needed polymonial
        return needed_polymonial;
    }

	// Returns the limit of a (only monomonial) polymonial for an unknown
    scls::Limit Polymonial::limit(Limit needed_limit, std::string unknown_name) {
        scls::Limit to_return;
        if(is_simple_monomonial()) {
            __Monomonial& needed_monomonial = a_monomonials[0];
            _Base_Unknown* unknown = needed_monomonial.contains_unknown(unknown_name);
            if(unknown != 0) {
                if(needed_limit.is_special()) {to_return = needed_monomonial.limit_special(needed_limit, unknown_name);}
                else {
                    // Other value limit
                    Polymonial modified = replace_unknown(unknown_name, needed_limit.value());
                    if(modified.a_monomonials.size() > 0) to_return = modified.a_monomonials.at(0).factor().real();
                }
            }
            else {to_return = needed_monomonial.factor().real();}
        }
        return to_return;
    };

    // Add a new monomonial to the polymonial
    void Polymonial::add_monomonial(__Monomonial new_monomonial) {
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
    __Monomonial Polymonial::monomonial(std::string unknown, Complex exponent) {
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            _Base_Unknown* result = a_monomonials.at(i).only_contains_unknown(unknown, exponent);
            if(result != 0) {return a_monomonials.at(i);}
        }
        __Monomonial to_return(0); to_return.add_unknown(unknown, exponent); return to_return;
    };
    // Returns a list of unknowns monomonials
    std::vector<__Monomonial> Polymonial::unknown_monomonials() const {
        std::vector<__Monomonial> to_return;
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(a_monomonials.at(i).unknowns_number() > 0) to_return.push_back(a_monomonials.at(i));
        }
        return to_return;
    };

    // Returns the polymonial to a GLSL function
    std::string Polymonial::to_glsl(Textual_Math_Settings* settings) const {
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
    // Returns the polymonial to std::string
    std::string Polymonial::to_std_string(Textual_Math_Settings* settings) const {
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
    // Add a polymonial to this one
    void Polymonial::__add(Polymonial value) {
        if(value.monomonials().size() <= 0 || (value.monomonials().size() == 1 && value.monomonials().at(0).factor() == 0)){return;}

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

    // Divide the polymonial by an another polymonial
    void Polymonial::__divide(Polymonial* value) {
        // The value is a simple monomonial
        if(value->is_simple_monomonial()) {__divide(value->monomonial());}
    }

    // Multiply a polymonial to this one
    void Polymonial::__multiply(Polymonial value) {
        std::vector<Polymonial> created_polymonial;
        // Apply each addition
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            Polymonial current_polymonial;
            current_polymonial.add_monomonial(a_monomonials[i]);
            // Apply each multiplication
            for(int j = 0;j<static_cast<int>(value.a_monomonials.size());j++) {
                __Monomonial& current_monomonial = value.a_monomonials[j];
                Polymonial needed_polymonial = current_polymonial;
                if(needed_polymonial.a_monomonials.size() > 0) {
                    needed_polymonial.a_monomonials[0] *= current_monomonial;
                    created_polymonial.push_back(needed_polymonial);
                }
            }
        }
        // Apply the multiplication
        a_monomonials.clear();
        for(int i = 0;i<static_cast<int>(created_polymonial.size());i++) {
            __add(created_polymonial[i]);
        }
    };

    // Returns the maximum degree in the polymonial
    Complex Polymonial::degree(std::string unknown_name) {
        Complex max_degree = 0;
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(a_monomonials[i].contains_unknown(unknown_name) && a_monomonials[i].unknowns_number() == 1) {
                Complex current_factor = a_monomonials[i].unknown()->exponent();
                if(current_factor.real() > max_degree.real()) max_degree = current_factor;
            }
        } return max_degree;
    };

    // Returns a polymonial from a monomonial where the unknows has been replaced
    Polymonial Polymonial::polymonial_from_modified_monomonial_unknows(__Monomonial used_monomonial, std::string unknown, Polymonial new_value) {
        Polymonial final_polymonial = Polymonial();

        _Base_Unknown* unknown_ptr = used_monomonial.contains_unknown(unknown);
        if(unknown_ptr!= 0) {
            // The monomonial is a multiplication between the unknown and the base value
            Complex exponent = unknown_ptr->exponent();
            used_monomonial.delete_unknown(unknown);
            final_polymonial += used_monomonial;
            // Create the good value
            Polymonial final_to_add = new_value;
            int needed_exponent = static_cast<int>(exponent.real().to_double());
            if(needed_exponent > 0) {for(int i = 1;i<needed_exponent;i++) {final_to_add *= new_value;}}
            else{final_to_add=1;for(int i = 0;i<-needed_exponent;i++) {final_to_add.__divide(&new_value);}}
            final_polymonial *= final_to_add;
        } else {
            // The unknown is not a multiplication of the base value
            final_polymonial += used_monomonial;
        }

        return final_polymonial;
    };

    // Returns this polymonial where an unknown is replaced by an another unknown
    Polymonial Polymonial::replace_unknown(std::string unknown, Polymonial new_value) const {
        Polymonial final_polymonial = Polymonial();

        // Analyse each monomonial
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            Polymonial current_polymonial = polymonial_from_modified_monomonial_unknows(a_monomonials.at(i), unknown, new_value);
            final_polymonial += current_polymonial;
        }

        return final_polymonial;
    };

    // Simplify the polymonial
    Polymonial Polymonial::simplify() const {
        Polymonial poly(*this);
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
    bool Polymonial::__is_equal(Complex value) const {
        if(value.real() == 0 && value.imaginary() == 0) {
            return (a_monomonials.size() == 0) || (a_monomonials.size() == 1 && a_monomonials.at(0).factor().real() == 0 && a_monomonials.at(0).factor().imaginary() == 0);
        }
        return (a_monomonials.size() == 1 && a_monomonials.at(0).factor().real() == value);
    };
    // With polymonial
    bool Polymonial::__is_equal(Polymonial value) const {
        std::vector<__Monomonial> monomonials_1 = value.a_monomonials;
        std::vector<__Monomonial> monomonials_2 = a_monomonials;
        for(int i = 0;i<static_cast<int>(monomonials_1.size());i++){if(monomonials_1.at(i).factor() == 0){monomonials_1.erase(monomonials_1.begin() + i);}}
        for(int i = 0;i<static_cast<int>(monomonials_2.size());i++){if(monomonials_2.at(i).factor() == 0){monomonials_2.erase(monomonials_2.begin() + i);}}
        if(monomonials_1.size() != monomonials_2.size()){return false;}int i = 0;
        while(i < static_cast<int>(monomonials_1.size())) {
            if(monomonials_1.at(i) == monomonials_2.at(0)) {
                monomonials_1.erase(monomonials_1.begin() + i);
                monomonials_2.erase(monomonials_2.begin());
                i++;
            }
            else{i = 0;}
        }
        return monomonials_1.size() == 0 && monomonials_2.size() == 0;
    };

    // Stream operator overloading
    std::ostream& operator<<(std::ostream& os, Polymonial& obj) {os << obj.to_std_string(0); return os; }
	// Divide operator
    Polymonial operator/(__Monomonial& obj, __Monomonial const& other) {
        Polymonial to_return;
        to_return.add_monomonial(__Monomonial(other.factor() / obj.factor(), obj.unknowns()));
        return to_return;
    }

    //*********
	//
	// The "Formula" class
	//
	//*********

	// Clears the container
    void __Formula_Base::Unknowns_Container::clear(){a_default_value.reset();a_unknowns.clear();};

	// Checks if the formula is well formatted
    void __Formula_Base::check_formula() {
        // Check if the formula is sub-placed too heavily
        if(is_simple_fraction() && !has_denominator() && a_fraction.get()->numerator()->formulas_add().size() == 1 && a_fraction.get()->numerator()->formulas_add().at(0).get()->factors().size() == 1) {
            if(a_fraction.get()->numerator()->formulas_add().at(0).get()->factors().at(0).get() == 0){*this = 0;}
            else{std::shared_ptr<__Formula_Base>temp=a_fraction.get()->numerator()->formulas_add().at(0).get()->factors().at(0);paste(temp.get());}
        }
    }

	// Returns the formula factor to a MathML
    std::string __Formula_Base::Formula_Factor::to_mathml(Textual_Math_Settings* settings) const {
        std::string to_return = "";

        // Add the factors
        for(int i = 0;i<static_cast<int>(a_factors.size());i++){
            if(to_return != std::string()){to_return += std::string("<mo>*</mo>");}
            to_return += a_factors.at(i).get()->to_mathml(settings);
        }

        // Return the result
        return to_return;
    }
	std::string __Formula_Base::to_mathml(Textual_Math_Settings* settings) const {
        std::string to_return = "";

        // Add the polymonial add
        if(a_polymonial.get() != 0) {
            std::string current_str = std::string();
            Polymonial needed_polymonial = (*a_polymonial.get());
            if(needed_polymonial != 0) {
                current_str = needed_polymonial.to_std_string(settings);
                if(current_str != "") {to_return += current_str;}
            }
            if(to_return != ""){to_return = std::string("<mi>") + to_return + std::string("</mi>");}
        }
        else {to_return = a_fraction.get()->to_mathml(settings);}

        // Add the applied function if needed
        if(a_applied_function.get() != 0) {
            to_return = std::string("<mi>") + a_applied_function.get()->name() + std::string("</mi><mo>(</mo>") + to_return + std::string("<mo>)</mo>");
        }

        // Return the result
        return to_return;
    }
    // Returns the formula factor to a std::string
    std::string __Formula_Base::Formula_Factor::to_std_string(Textual_Math_Settings* settings) const {
        std::string to_return = "";

        // Add the factors
        for(int i = 0;i<static_cast<int>(a_factors.size());i++){if(to_return != std::string()){to_return += std::string("*");}to_return += a_factors.at(i).get()->to_std_string(settings);}

        // Return the result
        return to_return;
    }
	std::string __Formula_Base::to_std_string(Textual_Math_Settings* settings) const {
        std::string current_str = "";
        std::string to_return = "";

        // Add the polymonial add
        if(a_polymonial.get() != 0) {
            std::string current_str = std::string();
            Polymonial needed_polymonial = (*a_polymonial.get());
            if(needed_polymonial != 0) {
                current_str = needed_polymonial.to_std_string(settings);
                if(current_str != "") {to_return += current_str;}
            }
        }
        else {to_return = a_fraction.get()->to_std_string(settings);}

        // Apply the function
        if(applied_function() != 0) {to_return = applied_function()->name() + "(" + to_return + ")";}

        return to_return;
    };

    // Returns all the unknowns in the formula
    std::vector<std::string> __Formula_Base::all_unknowns() const {
        std::vector<std::string> to_return;

        if(a_polymonial.get() != 0) {
            // Check the polymonial
            std::vector<std::string> current = a_polymonial.get()->all_unknowns();
            for(int j=0;j<static_cast<int>(current.size());j++){std::string to_add=current.at(j);if(to_add!=""&&std::count(to_return.begin(),to_return.end(),to_add)<=0){to_return.push_back(to_add);}}
        }

        return to_return;
    }
    // Returns a formula from a monomonial where the unknows has been replaced
    __Formula_Base::Formula __Formula_Base::formula_from_modified_monomonial_unknows(__Monomonial used_monomonial, std::string unknown, __Formula_Base new_value) {
        __Formula_Base::Formula final_formula;

        _Base_Unknown* unknown_ptr = used_monomonial.contains_unknown(unknown);
        if(unknown_ptr!= 0) {
            // The monomonial is a multiplication between the unknown and the base value
            Complex exponent = unknown_ptr->exponent();
            used_monomonial.delete_unknown(unknown);
            // Create the good value
            __Formula_Base final_to_add = new_value;
            int needed_exponent = static_cast<int>(exponent.real().to_double());
            final_formula = new_value;
            if(needed_exponent > 0){for(int i = 1;i<needed_exponent;i++) {final_formula *= new_value;}}
            else{final_formula = 1;for(int i = 0;i<-needed_exponent;i++) {final_formula /= new_value;}}
            final_formula *= used_monomonial;
        }
        else {final_formula = used_monomonial;}

        return final_formula;
    };
    // Returns a formula from a polymonial where the unknows has been replaced
    __Formula_Base::Formula __Formula_Base::formula_from_modified_polymonial_unknows(Polymonial used_polymonial, std::string unknown, __Formula_Base new_value) {
        __Formula_Base::Formula final_formula;

        // Browse each polymonials
        std::vector<__Monomonial>& monomonials = used_polymonial.monomonials();
        for(int i = 0;i<static_cast<int>(monomonials.size());i++) {final_formula += formula_from_modified_monomonial_unknows(monomonials[i], unknown, new_value);}

        return final_formula;
    };
    // Returns a formula where an unkown is replaced by an another unknown
    // Returns a Formula_Factor where an unkown is replaced by an another unknown
    __Formula_Base __Formula_Base::Formula_Factor::replace_unknown(std::string unknown, __Formula_Base new_value) const {
        __Formula_Base to_return;

        // Replace each factors
        for(int i = 0;i<static_cast<int>(a_factors.size());i++) {__Formula_Base current=(*a_factors.at(i).get()->replace_unknown(unknown, new_value).formula_base());to_return*=current;}

        return to_return;
    }
    __Formula_Base::Formula __Formula_Base::replace_unknown(std::string unknown, __Formula_Base new_value) const {
        __Formula_Base::Formula current_formula;
        __Formula_Base::Formula final_formula = Formula(1);
        final_formula.set_applied_function(applied_function_shared_ptr());
        if(final_formula == 1) {std::shared_ptr<__Formula_Base_Function> used_function = applied_function_shared_ptr(); final_formula = 0; final_formula.set_applied_function(used_function);}

        // Add the added polymonial
        if(a_polymonial.get() != 0) {final_formula += formula_from_modified_polymonial_unknows(*a_polymonial.get(), unknown, new_value);}

        // Returns the result
        final_formula.set_applied_function(applied_function_shared_ptr());
        return final_formula;
    };
    __Formula_Base::Formula __Formula_Base::replace_unknown(std::string unknown, __Formula_Base::Formula new_value) const{return replace_unknown(unknown, *new_value.formula_base());}
    __Formula_Base::Formula __Formula_Base::replace_unknowns(Unknowns_Container* values) const{
        // Get the needed datas
        __Formula_Base::Formula current_formula_complete_base = internal_value();
        __Formula_Base current_formula = (*current_formula_complete_base.formula_base());
        std::vector<std::string> unknowns = all_unknowns();

        // Get the needed value
        for(int i = 0;i<static_cast<int>(unknowns.size());i++) {__Formula_Base* needed_value=values->value_by_name(unknowns[i]);if(needed_value!=0){current_formula = (*current_formula.replace_unknown(unknowns[i], *needed_value).formula_base());}}

        // Return the result
        return current_formula;
    };
    // Returns the final value of the formula
    scls::Complex __Formula_Base::Formula_Factor::value(__Formula_Base::Unknowns_Container* values){scls::Complex to_return = scls::Complex(1);for(int i=0;i<static_cast<int>(a_factors.size());i++){to_return*=a_factors.at(i).get()->value(values);}return to_return;}
    scls::Complex __Formula_Base::Formula_Sum::value(__Formula_Base::Unknowns_Container* values){scls::Complex to_return = scls::Complex(0);for(int i=0;i<static_cast<int>(a_formulas_add.size());i++){to_return+=a_formulas_add.at(i).get()->value(values);}return to_return;}
    scls::Complex __Formula_Base::Formula_Fraction::value(__Formula_Base::Unknowns_Container* values){scls::Complex to_return = numerator()->value(values);if(denominator() != 0){to_return._divide(denominator()->value(values));}return to_return;}
    scls::Complex __Formula_Base::value(scls::Fraction current_value){Unknowns_Container temp = Unknowns_Container(current_value);return value(&temp);};
    scls::Complex __Formula_Base::value(__Formula_Base::Unknowns_Container* values) {
        // Simpler models
        if(is_simple_monomonial()){__Monomonial needed_monomonial = a_polymonial.get()->monomonial();if(needed_monomonial.is_known()){return needed_monomonial.factor();}}

        // Get the needed datas
        __Formula_Base::Formula current_formula_complete_base = internal_value();
        __Formula_Base current_formula = (*current_formula_complete_base.formula_base());
        std::vector<std::string> unknowns = all_unknowns();

        // Get the needed value
        current_formula = *current_formula.replace_unknowns(values).formula_base();

        // Get the final formula
        __Formula_Base::Formula final_formula;
        if(current_formula.a_polymonial.get() != 0){final_formula = (*current_formula.a_polymonial.get());}
        else if(current_formula.a_fraction.get() != 0){final_formula = current_formula_complete_base.formula_base()->a_fraction.get()->value(values);}

        // Apply the function
        scls::Complex to_return = scls::Complex(1);
        if(applied_function() != 0){final_formula = final_formula.formula_base()->value(values);to_return = scls::Fraction::from_double(applied_function()->real_value(final_formula.formula_base()));}
        else{to_return = final_formula.to_polymonial().known_monomonial().factor();}

        // Returns the value
        return to_return;
    }

    // Methods operators
    // Add a formula to this one
    void __Formula_Base::__add(__Formula_Base* value) {
        // Asserts
        if(value == 0 || *value == 0) {return;}
        else if(*this == 0) {*this = *value;return;}

        // Check if values are both polymonial
        if(a_polymonial.get() != 0) {
            if(!is_basic()){sub_place();a_fraction.get()->__add(*value);}
            else if(!value->is_simple_polymonial()){sub_place();a_fraction.get()->__add(*value);}
            else{a_polymonial.get()->__add(value->to_polymonial());}
        }
        else {if(!is_basic()){sub_place();}a_fraction.get()->__add(*value);}

        // Finish the result
        check_formula();
    };
    // Divide a formula to this one
    void __Formula_Base::Formula_Factor::__divide(__Formula_Base value) {basic_formula()->__divide(value);}
    // Divide a formula to this one
    void __Formula_Base::Formula_Fraction::__divide(__Formula_Base value){
        // Handle the division
        if(!value.is_basic() || !is_simple_polymonial()) {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
        else {
            if(value.is_simple_monomonial()) {
                // Apply a division of a simple monomonial
                __Monomonial used_monomonial = value.to_polymonial().monomonial();
                // Handle the complex part
                if(used_monomonial.factor().imaginary() != 0) {
                    // Get the needed divisor
                    scls::__Formula_Base needed_divisor = used_monomonial;
                    needed_divisor *= used_monomonial.factor().conjugate();
                    __divide(needed_divisor);
                    // Multiply the numerator
                    __multiply(scls::Polymonial(used_monomonial.factor().conjugate(), "", 1));
                }
                else if(!used_monomonial.is_known()) {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
                else {a_numerator.get()->__divide(used_monomonial);}
            }
            else if(value.is_simple_polymonial()) {
                // Apply a division of a simple polymonial
                Polymonial used_polymonial = value.to_polymonial();
                std::shared_ptr<Polymonial::Polymonial_Complex> needed_complex = Polymonial::Polymonial_Complex::from_polymonial(&used_polymonial);
                if((*needed_complex.get()->imaginary_polymonial()) == 0) {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
                else {
                    // Handle the complex part
                    std::shared_ptr<Polymonial::Polymonial_Complex> needed_conjugate = needed_complex.get()->conjugate();
                    Polymonial needed_polymonial = (*needed_conjugate.get()->to_polymonial());
                    used_polymonial *= needed_polymonial;
                    if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(used_polymonial);}else{a_denominator.get()->__multiply(used_polymonial);}
                    // Multiply the numerator
                    __multiply(needed_polymonial);
                }
            }
            else {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
        }
    }
    void __Formula_Base::__divide(__Formula_Base value) {
        // Check if values are both polymonial
        if(is_basic() && is_simple_polymonial() && value.is_simple_monomonial()) {a_polymonial.get()->__divide(value.to_monomonial());}
        else {
            if(!is_simple_fraction()){sub_place();}
            a_fraction.get()->__divide(value);
        }
        check_formula();
    };
    // Multiply a polymonial to this one
    void __Formula_Base::Formula_Factor::__multiply(__Formula_Base value){
        if(value.is_simple_polymonial()){__multiply(value.to_polymonial());}
        else{a_factors.push_back(std::make_shared<__Formula_Base>(value));}
    }
    void __Formula_Base::__multiply(__Formula_Base value) {
        // Check polymonial
        if(is_simple_polymonial()) {
            if(value.is_simple_polymonial()) {a_polymonial.get()->__multiply(value.to_polymonial());}
            else{sub_place();a_fraction.get()->__multiply(value);}
        }
        else {
            if(is_basic()) {a_fraction.get()->__multiply(value);}
            else{sub_place();a_fraction.get()->__multiply(value);}
        }

        // Finish the result
        check_formula();
    };

    // Calculate the derivate value of the SQRT function
    std::shared_ptr<__Formula_Base> __Sqrt_Function::derivate_value(__Formula_Base formula){
        if(formula.applied_function() == 0){formula.set_applied_function<__Sqrt_Function>();}
        std::shared_ptr<__Formula_Base> to_return=std::make_shared<__Formula_Base>(1);
        to_return.get()->__divide(formula * 2);
        return to_return;
    };

    // Calculate the derivate value of the COS and SIN function
    std::shared_ptr<__Formula_Base> __Cos_Function::derivate_value(__Formula_Base formula){formula.set_applied_function<__Sin_Function>();formula*=-1;return std::make_shared<__Formula_Base>(formula);}
    std::shared_ptr<__Formula_Base> __Sin_Function::derivate_value(__Formula_Base formula){formula.set_applied_function<__Cos_Function>();return std::make_shared<__Formula_Base>(formula);}

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

    //*********
	//
	// The parsers class
	//
	//*********

	// String_To_Formula_Parse constructor
	String_To_Formula_Parse::String_To_Formula_Parse(unsigned int level):a_level(level){
        add_function(std::string("cos"));
        add_function(std::string("exp"));
        add_function(std::string("ln"));
        add_function(std::string("normal"));
        add_function(std::string("sin"));
        add_function(std::string("sqrt"));
        add_function(std::string("tan"));
    };

	// Returns a given first base string to a formula
	void __string_to_formula_function(Formula& formula, std::string used_function){
        if(used_function == "cos"){formula.add_applied_function<__Cos_Function>();}
        else if(used_function == "exp"){formula.add_applied_function<__Exp_Function>();}
        else if(used_function == "ln"){formula.add_applied_function<__Log_Function>();}
        else if(used_function == "normal"){
            formula = 1;
            formula /= __Sqrt_Function::create_formula(2);
            formula *= __Exp_Function::create_formula(__Monomonial(-1, "x", 2));
        }
        else if(used_function == "sin"){formula.add_applied_function<__Sin_Function>();}
        else if(used_function == "sqrt"){formula.add_applied_function<__Sqrt_Function>();}
	}
    Formula String_To_Formula_Parse::__string_to_formula_base(std::string base, std::string used_function) {
        Formula formula;

        if(base.size() < 2 || (base[0] != '(')) {
            // Simple form
            // Get the needed datas
            unsigned int number_i = 0; std::string number_part_1 = ""; std::string number_part_2 = ""; std::string unknow_part = "";
            while(number_i < base.size() && __string_is_number(base[number_i])){number_part_1+=base[number_i];number_i++;}
            while(number_i < base.size() && !__string_is_number(base[number_i])){unknow_part+=base[number_i];number_i++;}
            while(number_i < base.size() && __string_is_number(base[number_i])){number_part_2+=base[number_i];number_i++;}
            // Add the monomonial
            Complex number = string_to_complex(number_part_1);
            if(unknow_part != "" && number_part_1 == "") number = 1;
            if(number_part_2 != "") number *= string_to_complex(number_part_2);
            __Monomonial to_add(number, unknow_part);
            Polymonial current_polymonial;
            current_polymonial.add_monomonial(to_add);
            formula = current_polymonial;
        }
        else {
            // Parenthesis form
            String_To_Formula_Parse new_parser(level() + 1);
            formula = new_parser.string_to_formula(base.substr(1, base.size() - 2));
        }

        // Create the formula
        __string_to_formula_function(formula, used_function);
        return formula;
    };

    // Converts a std::string to a Formula
    Formula String_To_Formula_Parse::__string_to_formula_without_division(std::string source) {
        // Format the text as needed
        std::vector<std::string> cutted;

        // Prepare the needed datas
        Formula to_return;

        // Cut the text operator by > operator
        cutted = cut_string_out_of_2(source, ">", "(", ")");
        if(cutted.size() > 1) {
            // At least one function applied
            for(int i = 1;i<static_cast<int>(cutted.size());i+=2) {
                Formula current_polymonial = __string_to_formula_base(cutted[i], cutted[i - 1]);
                to_return += current_polymonial;
            }
        } else {to_return = __string_to_formula_base(cutted[0]);}

        // Return the result
        return to_return;
    };
    Formula String_To_Formula_Parse::__string_to_formula_without_multiplication(std::string source) {
        // Format the text as needed
        std::vector<std::string> cutted;

        // Prepare the needed datas
        Formula to_return; bool to_return_modified = false;

        // Cut the text operator by / operator
        cutted = cut_string_out_of_2(source, "/", "(", ")");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            Formula current_polymonial = __string_to_formula_without_division(cutted[i]);
            if(to_return_modified) {
                to_return /= current_polymonial;
            }
            else {to_return += current_polymonial;to_return_modified=true;}
        }

        // Return the result
        return to_return;
    };
    Formula String_To_Formula_Parse::__string_to_formula_without_addition(std::string source) {
        // Format the text as needed
        std::vector<std::string> cutted;

        // Prepare the needed datas
        Formula to_return; bool to_return_modified = false;

        // Cut the text operator by * operator
        cutted = cut_string_out_of_2(source, "*", "(", ")");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            Formula current_polymonial = __string_to_formula_without_multiplication(cutted[i]);
            if(to_return_modified) {to_return *= current_polymonial;}
            else {to_return += current_polymonial;to_return_modified=true;}
        }

        // Return the result
        return to_return;
    };
    Formula String_To_Formula_Parse::string_to_formula(std::string source) {
        // Format the text as needed
        source = remove_space(source);
        std::string last_text_2 = std::string();std::string last_text_3 = std::string();std::string last_text_4 = std::string();
        for(int i = 0;i<static_cast<int>(source.size());i++) {
            /*// Handle functions
            if(i > 0) {
                if(contains_function(source[i])) {
                    if(!__string_is_operator(source[i - 1]) && source[i - 1] != '(') {
                        // The part is a simple variable
                        source.insert(i, "*");
                        i++;
                    }
                }
                else if(last_text_4.size() >= 4 && contains_function(last_text_4)) {
                    if(!__string_is_operator(source[i - 1]) && source[i - 1] != '(') {
                        // The part is a simple variable
                        source.insert(i, "*");
                        i++;
                    }
                }
            }//*/
            // Remove the useless ")("
            if(i > 0 && source[i] == '(') {
                if(source[i - 1] == ')') {source.insert(i, "*");i++;}
                else if(source[i - 1] == '-') {source.insert(i, "1*");i++;}
                else if(!__string_is_operator(source[i - 1])) {
                    std::string total_function = std::string();
                    int current_pos = i - 1;
                    while(current_pos >= 0 && (!__string_is_operator(source[current_pos]) && source[current_pos]!='(' && source[current_pos]!=')')){total_function=source[current_pos]+total_function;current_pos--;}
                    if(contains_function(total_function)) {
                        // The part is a function
                        source.insert(i, ">");
                        i++;
                    } else {
                        // The part is a simple variable
                        source.insert(i, "*");
                        i++;
                    }
                }
            }

            // Handle last text
            last_text_2 += source[i];last_text_3 += source[i];last_text_4 += source[i];
            if(last_text_2.size() > 2){last_text_2 = last_text_2.substr(1, last_text_2.size() - 1);}
            if(last_text_3.size() > 3){last_text_3 = last_text_3.substr(1, last_text_3.size() - 1);}
            if(last_text_4.size() > 4){last_text_4 = last_text_4.substr(1, last_text_4.size() - 1);}
        }
        // Handle the "-"
        for(int i = 0;i<static_cast<int>(source.size());i++) {
            // Remove the useless "-"
            if(i > 0 && static_cast<int>(source[i]) == static_cast<int>('-')) {
                if(!__string_is_operator(source[i - 1]) && (i >= static_cast<int>(source.size()) || (static_cast<int>(source[i + 1]) != static_cast<int>('(')))) {
                    source.insert(i, "+");
                    i++;
                }
            }
        }

        // Prepare the needed datas
        std::vector<std::string> cutted;
        Formula to_return;

        // Cut the text operator by + operator
        cutted = cut_string_out_of_2(source, "+", "(", ")");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            Formula current_polymonial = __string_to_formula_without_addition(cutted[i]);
            to_return += current_polymonial;
        }

        // Return the result
        to_return.check_formula();
        return to_return;
    };

    // Use parsers methods outside the class
    __Formula_Base string_to_formula(std::string source){String_To_Formula_Parse parser;return parser.string_to_formula(source);};
    __Formula_Base::Formula replace_unknown(__Formula_Base used_formula, std::string unknown, std::string new_value) { return used_formula.replace_unknown(unknown, string_to_formula(new_value));};
}
