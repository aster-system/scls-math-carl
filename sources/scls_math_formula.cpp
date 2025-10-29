//******************
//
// scls_math_formula.cpp
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
// This file contains the source code of scls_math_formula.h.
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
    // Soft_resets the formula
    void __Formula_Base::soft_reset(){a_to_double.clear();a_to_double_containers.clear();}

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

        // Add the Polynomial add
        if(a_polynomial.get() != 0) {
            std::string current_str = std::string();
            Polynomial needed_polynomial = (*a_polynomial.get());
            if(needed_polynomial != 0) {
                current_str = needed_polynomial.to_mathml(settings);
                if(current_str != "") {to_return += current_str;}
            }
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
        for(int i = 0;i<static_cast<int>(a_factors.size());i++){
            if(to_return != std::string()){to_return += std::string("*");}
            if(static_cast<int>(a_factors.size()) > 1){to_return += std::string("(") + a_factors.at(i).get()->to_std_string(settings) + std::string(")");}
            else{to_return += a_factors.at(i).get()->to_std_string(settings);}
        }

        // Return the result
        return to_return;
    }
	std::string __Formula_Base::to_std_string(Textual_Math_Settings* settings) const {
        std::string current_str = "";
        std::string to_return = "";

        // Add the Polynomial add
        if(a_polynomial.get() != 0) {
            std::string current_str = std::string();
            Polynomial needed_polynomial = (*a_polynomial.get());
            if(needed_polynomial != 0 || (settings != 0 && !settings->hide_if_0())) {
                current_str = needed_polynomial.to_std_string(settings);
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

        if(a_polynomial.get() != 0) {
            // Check the Polynomial
            std::vector<std::string> current = a_polynomial.get()->all_unknowns();
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
    // Returns a formula from a Polynomial where the unknows has been replaced
    __Formula_Base::Formula __Formula_Base::formula_from_modified_polynomial_unknows(Polynomial used_Polynomial, std::string unknown, __Formula_Base new_value) {
        __Formula_Base::Formula final_formula;

        // Browse each Polynomials
        std::vector<__Monomonial>& monomonials = used_Polynomial.monomonials();
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

        // Add the added Polynomial
        if(a_polynomial.get() != 0) {final_formula += formula_from_modified_polynomial_unknows(*a_polynomial.get(), unknown, new_value);}

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
        if(static_cast<int>(unknowns.size()) > 0 && values == 0){print("Warning", std::string("SCLS Carl Math"), std::string("Can't use unknowns with the formula \"") + to_std_string(0) + std::string("\" : now unknown container used."));}
        for(int i = 0;i<static_cast<int>(unknowns.size());i++) {__Formula_Base* needed_value=values->value_by_name(unknowns[i]);if(needed_value!=0){current_formula = (*current_formula.replace_unknown(unknowns[i], *needed_value).formula_base());}}

        // Return the result
        if(a_applied_function.get() != 0){current_formula.set_applied_function(a_applied_function.get()->function_copy());}
        return current_formula;
    };
    // Returns the final value of the formula
    scls::Complex __Formula_Base::Formula_Factor::value(__Formula_Base::Unknowns_Container* values){scls::Complex to_return = scls::Complex(1);for(int i=0;i<static_cast<int>(a_factors.size());i++){to_return*=a_factors.at(i).get()->value(values);}return to_return;}
    scls::Complex __Formula_Base::Formula_Sum::value(__Formula_Base::Unknowns_Container* values){scls::Complex to_return = scls::Complex(0);for(int i=0;i<static_cast<int>(a_formulas_add.size());i++){to_return+=a_formulas_add.at(i).get()->value(values);}return to_return;}
    scls::Complex __Formula_Base::Formula_Fraction::value(__Formula_Base::Unknowns_Container* values){scls::Complex to_return = 0;if(numerator()!=0){to_return = numerator()->value(values);}if(denominator() != 0){to_return._divide(denominator()->value(values));}return to_return;}
    scls::Complex __Formula_Base::value(scls::Fraction current_value){Unknowns_Container temp = Unknowns_Container(current_value);return value(&temp);};
    scls::Complex __Formula_Base::value(__Formula_Base::Unknowns_Container* values) {
        // Simpler models
        if(is_simple_monomonial() && is_basic()){__Monomonial needed_monomonial = a_polynomial.get()->monomonial();if(needed_monomonial.is_known()){return needed_monomonial.factor();}}

        // Get the needed datas
        __Formula_Base::Formula current_formula_complete_base = internal_value();
        __Formula_Base current_formula = (*current_formula_complete_base.formula_base());
        std::vector<std::string> unknowns = all_unknowns();

        // Get the needed value
        current_formula = *current_formula.replace_unknowns(values).formula_base();

        // Get the final formula
        __Formula_Base::Formula final_formula;
        if(current_formula.a_polynomial.get() != 0){final_formula = (*current_formula.a_polynomial.get());}
        else if(current_formula.a_fraction.get() != 0){final_formula = current_formula.a_fraction.get()->value(values);}

        // Apply the function
        scls::Complex to_return = scls::Complex(1);
        if(applied_function() != 0){final_formula = final_formula.formula_base()->value(values);to_return = scls::Fraction::from_double(applied_function()->real_value(final_formula.formula_base()));}
        else{to_return = final_formula.to_polynomial().known_monomonial().factor();}

        // Returns the value
        return to_return;
    }
    // Returns the value to a fraction
    scls::Fraction __Formula_Base::value_to_fraction(Unknowns_Container* values){return value(values).real();};
    scls::Fraction __Formula_Base::value_to_fraction(scls::Fraction current_value){return value(current_value).real();};
    scls::Fraction __Formula_Base::value_to_fraction(){return value(1).real();};
    // Returns the value to a double
    double* __Formula_Base::calculated_to_double(Unknowns_Container* container){
        for(int i = 0;i<static_cast<int>(a_to_double_containers.size());i++){
            if(a_to_double_containers.at(i)==container){return &a_to_double[i];}
        }
        return 0;
    }
    double __Formula_Base::value_to_double(Unknowns_Container* values){
        // TEMP
        return value(values).real().to_double();

        // TO EDIT
        double* calculated = calculated_to_double(values);
        if(calculated == 0){
            double new_to_double = value(values).real().to_double();
            a_to_double.push_back(new_to_double);
            a_to_double_containers.push_back(values);
            calculated = &a_to_double[a_to_double.size() - 1];
        }

        return *calculated;
    };
    double __Formula_Base::value_to_double(scls::Fraction current_value){
        if(current_value == 1) {
            double* calculated = calculated_to_double(0);
            if(calculated == 0){
                double new_to_double=value(1).real().to_double();
                a_to_double.push_back(new_to_double);
                a_to_double_containers.push_back(0);
                calculated = &a_to_double[a_to_double.size() - 1];
            }
            return *calculated;
        }

        return value(current_value).real().to_double();
    }
    double __Formula_Base::value_to_double(){return value_to_double(1);}

    // Returns the final value of the formula
    scls::Complex __Formula_Base::Formula::value(Unknowns_Container* values) const {return a_formula.get()->value(values);};
    scls::Complex __Formula_Base::Formula::value(scls::Fraction current_value) const {return a_formula.get()->value(current_value);};
    scls::Fraction __Formula_Base::Formula::value_to_fraction(Unknowns_Container* values) const {return a_formula.get()->value_to_fraction(values);};
    scls::Fraction __Formula_Base::Formula::value_to_fraction(scls::Fraction current_value) const {return a_formula.get()->value_to_fraction(current_value);};
    scls::Fraction __Formula_Base::Formula::value_to_fraction() const {return a_formula.get()->value_to_fraction();};
    double __Formula_Base::Formula::value_to_double(Unknowns_Container* values) const {return a_formula.get()->value_to_double(values);};
    double __Formula_Base::Formula::value_to_double(scls::Fraction current_value) const {return a_formula.get()->value_to_double(current_value);};
    double __Formula_Base::Formula::value_to_double() const {return a_formula.get()->value_to_double();};

    // Methods operators
    // Add a formula to this one
    void __Formula_Base::__add(__Formula_Base* value) {
        // Asserts
        if(value == 0 || *value == 0) {return;}
        else if(*this == 0) {*this = *value;return;}

        // Does the redaction
        if(a_redaction != 0) {(*a_redaction) += std::string(std::string("Nous cherchon à rajouter la formule \"") + value->to_std_string(0) + std::string("\" à \"") + to_std_string(0) + std::string("."));}

        // Check if values are both Polynomial
        if(is_simple_polynomial() && value->is_simple_polynomial()){
            a_polynomial.get()->__add(value->to_polynomial());
            if(a_redaction != 0) {
                (*a_redaction) += std::string(std::string("Or, il s'agit d'une simple addition de polynôme."));
            }
        }
        else {
            if(!is_basic() || is_simple_polynomial()){sub_place();}
            a_fraction.get()->__add(*value);
            if(a_redaction != 0) {
                (*a_redaction) += std::string(std::string("Or, il s'agit d'une addition de termes non-additionnables."));
            }
        }

        // Finish the result
        check_formula();
        soft_reset();
    };
    // Divide a formula to this one
    void __Formula_Base::Formula_Factor::__divide(__Formula_Base value) {basic_formula()->__divide(value);}
    // Divide a formula to this one
    void __Formula_Base::Formula_Fraction::__divide(__Formula_Base value){
        // Handle the division
        if(!value.is_basic() || !is_simple_polynomial()) {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
        else {
            if(value.is_simple_monomonial()) {
                // Apply a division of a simple monomonial
                __Monomonial used_monomonial = value.to_polynomial().monomonial();
                // Handle the complex part
                if(used_monomonial.factor().imaginary() != 0) {
                    // Get the needed divisor
                    scls::__Formula_Base needed_divisor = used_monomonial;
                    needed_divisor *= used_monomonial.factor().conjugate();
                    __divide(needed_divisor);
                    // Multiply the numerator
                    __multiply(scls::Polynomial(used_monomonial.factor().conjugate(), "", 1));
                }
                else if(!used_monomonial.is_known()) {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
                else {a_numerator.get()->__divide(used_monomonial);}
            }
            else if(value.is_simple_polynomial()) {
                // Apply a division of a simple Polynomial
                Polynomial used_Polynomial = value.to_polynomial();
                std::shared_ptr<Polynomial::Polynomial_Complex> needed_complex = Polynomial::Polynomial_Complex::from_polynomial(&used_Polynomial);
                if((*needed_complex.get()->imaginary_polynomial()) == 0) {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
                else {
                    // Handle the complex part
                    std::shared_ptr<Polynomial::Polynomial_Complex> needed_conjugate = needed_complex.get()->conjugate();
                    Polynomial needed_polynomial = (*needed_conjugate.get()->to_polynomial());
                    used_Polynomial *= needed_polynomial;
                    if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(used_Polynomial);}else{a_denominator.get()->__multiply(used_Polynomial);}
                    // Multiply the numerator
                    __multiply(needed_polynomial);
                }
            }
            else {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
        }
    }
    void __Formula_Base::__divide(__Formula_Base value) {
        // Redaction
        if(a_redaction != 0) {
            (*a_redaction) += std::string(std::string("Nous cherchons à diviser la formule \"") + value.to_std_string(0) + std::string("\" à \"") + to_std_string(0) + std::string("."));
        }

        // Check if values are both Polynomial
        if(is_basic() && is_simple_polynomial() && value.is_simple_monomonial()) {
            a_polynomial.get()->__divide(value.to_monomonial());
            if(a_redaction != 0) {
                (*a_redaction) += std::string(std::string("Or, il s'agit d'une simple division polynôme / monôme."));
            }
        }
        else {
            if(!is_simple_fraction()){sub_place();}
            a_fraction.get()->__divide(value);
            if(a_redaction != 0) {
                (*a_redaction) += std::string(std::string("Or, il s'agit de deux termes non-divisables."));
            }
        }

        // Finish the result
        check_formula();
        soft_reset();
    };
    // Returns if two numbers/formulas are equals
    bool __Formula_Base::__is_equal(int value)const{if(a_polynomial.get() != 0){return a_polynomial.get()->__is_equal(value);} return a_fraction.get()->__is_equal(value);};
    bool __Formula_Base::__is_equal(Fraction value)const{if(a_polynomial.get() != 0){return a_polynomial.get()->__is_equal(value);}return a_fraction.get()->__is_equal(value);};
    bool __Formula_Base::__is_equal(Polynomial value)const{if(a_polynomial.get() != 0){return a_polynomial.get()->__is_equal(value);}return a_fraction.get()->__is_equal(value);};
    bool __Formula_Base::__is_equal(__Formula_Base value)const{
        if(!((a_applied_function.get() == 0 && value.a_applied_function.get() == 0) || (a_applied_function.get() != 0 && value.a_applied_function.get() != 0 && a_applied_function.get()->name() == value.a_applied_function.get()->name()))){return false;}

        // Polynomial
        if(a_fraction.get() == 0 && value.a_fraction.get() == 0) {return a_polynomial.get()->__is_equal(value.a_polynomial.get());}

        // Fraction
        else if((a_fraction.get() != 0 && value.a_fraction.get() == 0) || (a_fraction.get() == 0 && value.a_fraction.get() != 0)){return false;}
        return a_fraction.get()->__is_equal(value.internal_value());
    };
    // Multiply a Polynomial to this one
    void __Formula_Base::Formula_Factor::__multiply(__Formula_Base value){
        if(value.is_simple_polynomial()){__multiply(value.to_polynomial());}
        else{a_factors.push_back(std::make_shared<__Formula_Base>(value));}
    }
    void __Formula_Base::__multiply(__Formula_Base value) {
        // Redaction
        if(a_redaction != 0) {
            (*a_redaction) += std::string(std::string("Nous cherchon à multiplier la formule \"") + value.to_std_string(0) + std::string("\" à \"") + to_std_string(0) + std::string("."));
        }

        // Check Polynomial
        if(is_simple_polynomial() && value.is_simple_polynomial()) {
            a_polynomial.get()->__multiply(value.to_polynomial());
            if(a_redaction != 0) {
                (*a_redaction) += std::string(std::string("Or, il s'agit d'une simple multiplication de polynômes."));
            }
        }
        else {
            // Handle functions
            bool control = false;
            if(applied_function() != 0){
                std::shared_ptr<__Formula_Base> new_value = applied_function()->multiply(this, &value);
                if(new_value.get() != 0) {
                    paste(new_value.get());
                    control = true;
                }
            }

            // No possible ways
            if(!control) {
                if(is_simple_fraction()) {a_fraction.get()->__multiply(value);}
                else{sub_place();a_fraction.get()->__multiply(value);}
                if(a_redaction != 0) {
                    (*a_redaction) += std::string(std::string("Or, il s'agit de termes non-multipliables."));
                }
            }
        }

        // Finish the result
        check_formula();
        soft_reset();
    };

    // Calculate the derivate value of the SQRT function
    std::shared_ptr<__Formula_Base> __Sqrt_Function::derivate_value(__Formula_Base formula){
        if(formula.applied_function() == 0){formula.set_applied_function<__Sqrt_Function>();}
        std::shared_ptr<__Formula_Base> to_return=std::make_shared<__Formula_Base>(1);
        to_return.get()->__divide(formula * 2);
        return to_return;
    };

    // Multiply a value with the function
    std::shared_ptr<__Formula_Base> __Sqrt_Function::multiply(__Formula_Base* value_1, __Formula_Base* value_2){
        if(value_1->applied_function() != 0 && value_2->applied_function() != 0 && value_1->applied_function()->name() == value_2->applied_function()->name()) {
            std::shared_ptr<__Formula_Base> copied_1 = value_1->formula_copy();
            std::shared_ptr<__Formula_Base> copied_2 = value_2->formula_copy();
            copied_1.get()->clear_applied_function();copied_2.get()->clear_applied_function();

            // Same value
            if(*copied_1.get() == *copied_2.get()){return copied_1;}

            // Normal multiplication
            copied_1.get()->__multiply(*copied_2.get());
            copied_1.get()->set_applied_function<__Sqrt_Function>();
            return copied_1;
        }

        return std::shared_ptr<__Formula_Base>();
    };

    // Calculate the derivate value of the COS and SIN function
    std::shared_ptr<__Formula_Base> __Cos_Function::derivate_value(__Formula_Base formula){formula.set_applied_function<__Sin_Function>();formula*=-1;return std::make_shared<__Formula_Base>(formula);}
    std::shared_ptr<__Formula_Base> __Sin_Function::derivate_value(__Formula_Base formula){formula.set_applied_function<__Cos_Function>();return std::make_shared<__Formula_Base>(formula);}
}
