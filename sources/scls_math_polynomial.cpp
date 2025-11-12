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
    std::shared_ptr<__Monomonial_Base> __Monomonial_Base::inverse() const {
        std::shared_ptr<__Monomonial_Base> new_monomonial = clone();
        new_monomonial.get()->__set_factor(__factor()->inverse());
        // Reverse each unknowns
        for(int i = 0;i<static_cast<int>(new_monomonial.get()->a_unknowns.size());i++) {new_monomonial.get()->a_unknowns[i].set_exponent(new_monomonial.get()->a_unknowns[i].exponent() * -1);}
        return new_monomonial;
    };

    // Returns if the monomonial only contains an unkwnown
    _Base_Unknown* __Monomonial_Base::only_contains_unknown(std::string unknown_name, Complex exponent) {
        _Base_Unknown* to_return = 0; unsigned int number = unknowns_number();
        for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
            if((a_unknowns[i].name() == unknown_name && a_unknowns[i].exponent() == exponent)) {
                to_return = &a_unknowns[i];
            } else if(!(number > 0 && a_unknowns[i].name() == "")) {to_return = 0; break;}
        } return to_return;
    };
    // Returns this monomonial where an unknown is replaced by an another unknown
    std::shared_ptr<__Monomonial_Base> __Monomonial_Base::replace_unknown(std::string unknown, __Monomonial_Base* new_value) {
        std::shared_ptr<__Monomonial_Base> final_monomonial = clone();
        _Base_Unknown* unknown_ptr = contains_unknown(unknown);
        if(unknown_ptr!= 0) {
            // The monomonial is a multiplication between the unknown and the base value
            Complex exponent = unknown_ptr->exponent();
            final_monomonial.get()->delete_unknown(unknown);
            // Create the good value
            std::shared_ptr<__Monomonial_Base> final_to_add = new_value->clone();
            int needed_exponent = static_cast<int>(exponent.real().to_double());
            for(int i = 1;i<needed_exponent;i++) {final_to_add.get()->__multiply(new_value);}
            final_monomonial->__multiply(final_to_add.get());
        }

        return final_monomonial;
    };
    // Returns if two monomonials has the same unknowns
    bool __Monomonial_Base::same_unknowns(const __Monomonial_Base* monomonial) const {
        std::vector<_Base_Unknown> unknows_1 = a_unknowns;
        std::vector<_Base_Unknown> unknows_2 = monomonial->a_unknowns;
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
    std::string __Monomonial_Base::to_glsl(Textual_Math_Settings* settings) const {
        if(a_factor == 0){return std::string();}
        std::string to_return = __factor()->to_std_string(settings);
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
    std::string __Monomonial_Base::to_mathml(Textual_Math_Settings* settings) const {
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
        return a_factor.get()->to_mathml(settings) + final_unknow;
    }
    // Returns the monomonial converted to std::string
    std::string __Monomonial_Base::to_std_string(Textual_Math_Settings* settings) const {
        std::string final_unknow = "";
        for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
            final_unknow += a_unknowns.at(i).name();
            int real_exponent = a_unknowns.at(i).exponent().real().to_double();
            if(a_unknowns.at(i).name() != "" && a_unknowns.at(i).exponent() != 1){
                if(real_exponent > 0){for(int j = 1;j<real_exponent;j++){final_unknow += std::string("*") + a_unknowns.at(i).name();}}
                else{final_unknow += std::string("^") + std::to_string(real_exponent);}
            }
        }

        // Get the final text
        return a_factor.get()->to_std_string(settings) + final_unknow;
    };

    // Equal operator assignment
    bool __Monomonial_Base::__is_equal(const __Monomonial_Base* object) const {return is_equal(__factor(), object->__factor()) && same_unknowns(object); }
    // Multiplication operator assignment
    void __Monomonial_Base::__add(const __Monomonial_Base* object){a_factor = addition(a_factor.get(), object->a_factor.get());}
    void __Monomonial_Base::__multiply(const __Field_Element* object){a_factor = multiplication(a_factor.get(), object);}
    void __Monomonial_Base::__multiply(const __Monomonial_Base* object) {
        a_factor = multiplication(a_factor.get(), object->a_factor.get());
        for(int i = 0;i<static_cast<int>(object->a_unknowns.size());i++) {
            _Base_Unknown* contained_unknown = contains_unknown(object->a_unknowns[i].name());
            if(contained_unknown == 0) {a_unknowns.push_back(object->a_unknowns.at(i));}
            else if(object->a_unknowns.at(i).name() != "") {
                scls::Complex new_exponent = contained_unknown->exponent() + object->a_unknowns.at(i).exponent();
                if(new_exponent == 0) {
                    delete_unknown(contained_unknown);
                } else {contained_unknown->set_exponent(new_exponent);}
            }
        }
    }
    void __Monomonial_Base::__substract(const __Monomonial_Base* object){a_factor = substraction(a_factor.get(), object->a_factor.get());}

    // Stream operator overloading
    std::ostream& operator<<(std::ostream& os, const __Monomonial_Base& obj) { os << obj.to_std_string(0) ; return os; }

    //*********
	//
	// The "Polynomial_Base" class
	//
	//*********

	// Polynomial_Base copy constructor
    Polynomial_Base::Polynomial_Base(const Polynomial_Base& polynomial_copy){for(int i = 0;i<static_cast<int>(polynomial_copy.a_monomonials.size());i++){a_monomonials.push_back(polynomial_copy.a_monomonials.at(i).get()->clone());}};

	// Add a new monomonial to the Polynomial_Base
    void Polynomial_Base::__add_monomonial(__Monomonial_Base* new_monomonial) {
        if(new_monomonial == 0 || new_monomonial->is_null()){return;}

        // Add the monomonial
        __Monomonial_Base* same_monomonial = contains_monomonial(new_monomonial);
        if(same_monomonial == 0){a_monomonials.push_back(new_monomonial->clone());}
        else{
            if(is_absorbing_multiplication(addition(same_monomonial->__factor(), new_monomonial->__factor()))) {
                // Remove the empty monomonial
                for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                    if(a_monomonials.at(i).get()->same_unknowns(same_monomonial)) {
                        a_monomonials.erase(a_monomonials.begin() + i, a_monomonials.begin() + i + 1);
                        break;
                    }
                }
            }
            else {same_monomonial->__add(new_monomonial);}
        }
    };

    // Returns all the unknowns in the formula
    std::vector<std::string> Polynomial_Base::all_unknowns(){std::vector<std::string> to_return;for(int i=0;i<static_cast<int>(a_monomonials.size());i++){for(int j=0;j<static_cast<int>(a_monomonials.at(i).get()->unknowns().size());j++){std::string to_add=a_monomonials.at(i).get()->unknowns().at(j).name();if(to_add!=""&&std::count(to_return.begin(),to_return.end(),to_add)<=0){to_return.push_back(to_add);}}}return to_return;};

    // Returns if the polynomial contains a monomonial
    __Monomonial_Base* Polynomial_Base::contains_monomonial(__Monomonial_Base* new_monomonial) {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {if(a_monomonials[i].get()->same_unknowns(new_monomonial)) {return a_monomonials.at(i).get();}} return 0;}

    // Returns the maximum degree in the Polynomial
    int Polynomial_Base::degree(std::string unknown_name) {
        Complex max_degree = 0;
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(a_monomonials.at(i).get()->contains_unknown(unknown_name) && a_monomonials.at(i).get()->unknowns_number() == 1) {
                Complex current_factor = a_monomonials.at(i).get()->unknown()->exponent();
                if(current_factor.real() > max_degree.real()){max_degree = current_factor;}
            }
        } return max_degree.real().to_double();
    };

    // Returns if the polynomial is known
    bool Polynomial_Base::is_known() const {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {if(!a_monomonials.at(i).get()->is_known() && a_monomonials.at(i).get()->is_null()){return false;}}return true;}

    // Returns if the polynomial is a simple monomonial
    bool Polynomial_Base::is_simple_monomonial() const {return monomonials_number() <= 1;};

    // Returns the knows monomonial
    __Monomonial_Base* Polynomial_Base::__known_monomonial() const {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {if(a_monomonials.at(i).get()->is_known()){return a_monomonials.at(i).get();}}return 0;};

    // Returns a monomonial by its unknown
    __Monomonial_Base* Polynomial_Base::__monomonial(std::string unknown) {return __monomonial(unknown, Complex(1, 0));};
    __Monomonial_Base* Polynomial_Base::__monomonial() {if(monomonials().size() > 0){return monomonials().at(0).get();}return 0;};
    __Monomonial_Base* Polynomial_Base::__monomonial(std::string unknown, Complex exponent) {
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            _Base_Unknown* result = a_monomonials.at(i).get()->only_contains_unknown(unknown, exponent);
            if(result != 0) {return a_monomonials.at(i).get();}
        }
        return 0;
    };

    // Returns the number of monomonials in the polynomial
    int Polynomial_Base::monomonials_number() const {return a_monomonials.size();};

    // Returns a list of unknowns monomonials
    std::vector<__Monomonial_Base*> Polynomial_Base::unknown_monomonials() const {
        std::vector<__Monomonial_Base*> to_return;
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(a_monomonials.at(i).get()->unknowns_number() > 0) to_return.push_back(a_monomonials.at(i).get());
        }
        return to_return;
    };

	// Returns the Polynomial_Base to a GLSL function
    std::string Polynomial_Base::to_glsl(Textual_Math_Settings* settings) const {
        std::string to_return = std::string("float poly(float x){float y = ");
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(!a_monomonials.at(i).get()->is_null()) {
                std::string to_add = a_monomonials.at(i).get()->to_glsl(settings);
                to_return += to_add;
                if(to_add != std::string() && i < static_cast<int>(a_monomonials.size()) - 1) {
                    to_return += " + ";
                }
            }
        }
        to_return += std::string(";return y;}");
        return to_return;
    }
     // Returns the Polynomial_Base to mathml
    std::string Polynomial_Base::to_mathml(Textual_Math_Settings* settings) const {
        // Asserts
        if(static_cast<int>(a_monomonials.size()) == 0){return std::string("<mi>0</mi>");}

        // Generates text
        std::string to_return = "";
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(!a_monomonials.at(i).get()->is_null()) {
                to_return += a_monomonials.at(i).get()->to_mathml(settings);
                if(i < static_cast<int>(a_monomonials.size()) - 1) {
                    to_return += std::string("<mo>+</mo>");
                }
            }
        }
        while(to_return[to_return.size() - 1] == '+' || to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
        return to_return;
    }
    // Returns the Polynomial_Base to std::string
    std::string Polynomial_Base::to_std_string(Textual_Math_Settings* settings) const {
        // Asserts
        if(static_cast<int>(a_monomonials.size()) == 0){return std::string("0");}

        std::string to_return = "";
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            if(!a_monomonials.at(i).get()->is_null()) {
                to_return += a_monomonials.at(i).get()->to_std_string(settings);
                if(i < static_cast<int>(a_monomonials.size()) - 1) {
                    to_return += " + ";
                }
            }
        }
        while(to_return[to_return.size() - 1] == '+' || to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
        return to_return;
    };

    // Methods operators
    // Add a Polynomial_Base to this one
    void Polynomial_Base::__add(__Monomonial_Base* value) {
        __Monomonial_Base* contained_monomonial = contains_monomonial(value);
        if(contained_monomonial == 0) {__add_monomonial(value);} else {contained_monomonial->__add(value);}
    }
    void Polynomial_Base::__add(Polynomial_Base* value) {
        // Asserts / speed
        if(value->monomonials().size() <= 0 || (value->monomonials().size() == 1 && value->monomonials().at(0).get()->is_null())){return;}
        if(value->monomonials().size() == 1 && monomonials().size() == 1 && value->monomonials().at(0).get()->is_known() && monomonials().at(0).get()->is_known()){monomonials().at(0).get()->__add(value->monomonials().at(0).get());return;}

        // Add the monomonial
        for(int i = 0;i<static_cast<int>(value->a_monomonials.size());i++) {
            __Monomonial_Base* current_monomonial = value->a_monomonials.at(i).get();
            __Monomonial_Base* contained_monomonial = contains_monomonial(current_monomonial);
            if(contained_monomonial == 0) {__add_monomonial(current_monomonial);} else {contained_monomonial->__add(current_monomonial);}
        }
    };

    // Divide a monomonial to this void
    void Polynomial_Base::__divide(__Monomonial_Base* value) {if(value == 0){return;}std::shared_ptr<__Monomonial_Base> used_inverse = value->inverse();for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {a_monomonials.at(i).get()->__multiply(used_inverse.get());}};
    void Polynomial_Base::__divide(Polynomial_Base* value) {
        // The value is a simple monomonial
        if(value->is_simple_monomonial()) {__divide(value->__monomonial());}
    }

    // Multiply a Polynomial_Base to this one
    void Polynomial_Base::__multiply(__Field_Element* value) {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {a_monomonials.at(i).get()->__multiply(value);}}
    void Polynomial_Base::__multiply(Polynomial_Base* value) {
        std::vector<std::shared_ptr<Polynomial_Base>> created_polynomial;

        // Apply each addition
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            std::shared_ptr<Polynomial_Base> current_polynomial = clone_empty();
            current_polynomial.get()->__add_monomonial(a_monomonials.at(i).get());

            // Apply each multiplication
            for(int j = 0;j<static_cast<int>(value->a_monomonials.size());j++) {
                __Monomonial_Base* current_monomonial = value->a_monomonials.at(j).get();
                std::shared_ptr<Polynomial_Base> needed_polynomial = current_polynomial.get()->clone();
                if(needed_polynomial.get()->a_monomonials.size() > 0) {
                    needed_polynomial.get()->a_monomonials.at(0).get()->__multiply(current_monomonial);
                    created_polynomial.push_back(needed_polynomial);
                }
            }
        }
        // Apply the multiplication
        a_monomonials.clear();
        for(int i = 0;i<static_cast<int>(created_polynomial.size());i++) {__add(created_polynomial.at(i).get());}
    };

    // Returns a Polynomial_Base from a monomonial where the unknows has been replaced
    std::shared_ptr<Polynomial_Base> Polynomial_Base::polynomial_from_modified_monomonial_unknows(__Monomonial_Base* used_monomonial, std::string unknown, Polynomial_Base* new_value) {
        std::shared_ptr<Polynomial_Base> final_polynomial = new_value->clone_empty();

        _Base_Unknown* unknown_ptr = used_monomonial->contains_unknown(unknown);
        if(unknown_ptr!= 0) {
            // The monomonial is a multiplication between the unknown and the base value
            Complex exponent = unknown_ptr->exponent();
            used_monomonial->delete_unknown(unknown);
            final_polynomial.get()->__add(used_monomonial);
            // Create the good value
            std::shared_ptr<Polynomial_Base> final_to_add = new_value->clone();
            int needed_exponent = static_cast<int>(exponent.real().to_double());
            if(needed_exponent > 0) {for(int i = 1;i<needed_exponent;i++) {final_to_add.get()->__multiply(new_value);}}
            //else{final_to_add=1;for(int i = 0;i<-needed_exponent;i++) {final_to_add.__divide(&new_value);}}
            final_polynomial.get()->__multiply(final_to_add.get());
        } else {
            // The unknown is not a multiplication of the base value
            final_polynomial.get()->__add(used_monomonial);
        }

        return final_polynomial;
    };

    // Returns this Polynomial where an unknown is replaced by an another unknown
    std::shared_ptr<Polynomial_Base> Polynomial_Base::replace_unknown(std::string unknown, Polynomial_Base* new_value) const {
        std::shared_ptr<Polynomial_Base> final_polynomial = new_value->clone_empty();

        // Analyse each monomonial
        for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
            std::shared_ptr<Polynomial_Base> current_polynomial = polynomial_from_modified_monomonial_unknows(a_monomonials.at(i).get(), unknown, new_value);
            final_polynomial.get()->__add(current_polynomial.get());
        }

        return final_polynomial;
    };

    // Simplify the Polynomial
    std::shared_ptr<Polynomial_Base> Polynomial_Base::simplify() const {
        std::shared_ptr<Polynomial_Base> poly = clone();
        for(int i = 0;i<static_cast<int>(poly.get()->a_monomonials.size());i++) {
            if(poly.get()->a_monomonials.at(i).get()->is_null()) {
                poly.get()->a_monomonials.erase(poly.get()->a_monomonials.begin() + i);
                i--;
            }
        }
        return poly;
    };

    // Operators
    bool Polynomial_Base::__is_equal(__Field_Element* value) const {return a_monomonials.size() == 1 && a_monomonials.at(0).get()->is_known() && a_monomonials.at(0).get()->is_equal(a_monomonials.at(0).get()->__factor(), value);};
    bool Polynomial_Base::__is_equal(Polynomial_Base* value) const {
        std::vector<std::shared_ptr<__Monomonial_Base>> monomonials_1 = value->a_monomonials;
        std::vector<std::shared_ptr<__Monomonial_Base>> monomonials_2 = a_monomonials;
        for(int i = 0;i<static_cast<int>(monomonials_1.size());i++){if(monomonials_1.at(i).get()->is_null()){monomonials_1.erase(monomonials_1.begin() + i);i--;}}
        for(int i = 0;i<static_cast<int>(monomonials_2.size());i++){if(monomonials_2.at(i).get()->is_null()){monomonials_2.erase(monomonials_2.begin() + i);i--;}}
        if(monomonials_1.size() != monomonials_2.size()){return false;}int i = 0;
        while(monomonials_1.size() > 0 && monomonials_2.size() > 0 && i < static_cast<int>(monomonials_1.size())) {
            if(monomonials_1.at(i).get()->__is_equal(monomonials_2.at(0).get())) {
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
        to_return.add_monomonial(__Monomonial(obj.division(obj.factor(), other.factor()), obj.unknowns()));
        return to_return;
    }
    //*/

    // Converts a polynomial to a complex
    std::shared_ptr<Polynomial_Complex> Polynomial_Complex::from_polynomial(Polynomial* polynomial) {
        // Create the polynomial complex
        std::shared_ptr<Polynomial_Complex> needed_polynomial = std::make_shared<Polynomial_Complex>();

        // Handle each monomonial
        Polynomial* imaginary_polynomial = needed_polynomial.get()->imaginary_polynomial();
        Polynomial* real_polynomial = needed_polynomial.get()->real_polynomial();
        for(int i = 0;i<static_cast<int>(polynomial->monomonials().size());i++) {
            // Real and imaginary factor
            scls::__Monomonial* needed_monomonial = polynomial->monomonial(i);
            scls::Fraction imaginary_factor = needed_monomonial->factor()->imaginary();
            scls::Fraction real_factor = needed_monomonial->factor()->real();

            // Add the needed factors
            scls::__Monomonial imaginary_monomonial = *needed_monomonial;
            imaginary_monomonial.set_factor(imaginary_factor);
            scls::__Monomonial real_monomonial = *needed_monomonial;
            real_monomonial.set_factor(real_factor);
            imaginary_polynomial->__add(&imaginary_monomonial);
            real_polynomial->__add(&real_monomonial);
        }

        // Return the needed polynomial
        return needed_polynomial;
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
