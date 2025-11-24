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

	// __Formula_Base constructor
    __Formula_Base::__Formula_Base(){}
    __Formula_Base::__Formula_Base(Polynomial_Base* polynomial):a_polynomial(polynomial->clone()){}
    __Formula_Base::__Formula_Base(Formula_Fraction frac){set_fraction(frac);}
    __Formula_Base::__Formula_Base(Formula_Sum sum):__Formula_Base(&sum){}
    __Formula_Base::__Formula_Base(Formula_Sum* sum){if(sum->is_simple_polynomial()){set_polynomial(sum);}else{set_fraction(sum);}}
    __Formula_Base::__Formula_Base(const __Formula_Base& formula):a_polynomial(formula.polynomial_copy()),a_fraction(formula.fraction_copy()),a_applied_function(formula.applied_function_copy()){};

	// Clears the container
    void __Formula_Base::Unknowns_Container::clear(){a_default_value.reset();a_unknowns.clear();};

	// Checks if the formula is well formatted
    void __Formula_Base::check_formula() {
        // Check if the formula is sub-placed too heavily
        if(is_simple_fraction() && !has_denominator() && a_fraction.get()->numerator()->formulas_add().size() == 1 && a_fraction.get()->numerator()->formulas_add().at(0).get()->factors().size() == 1) {
            if(a_fraction.get()->numerator()->formulas_add().at(0).get()->factors().at(0).get() == 0){clear();}
            else{std::shared_ptr<__Formula_Base>temp=a_fraction.get()->numerator()->formulas_add().at(0).get()->factors().at(0);paste(temp.get());}
        }
    }
    // Clears the formula
    void __Formula_Base::clear() {a_applied_function.reset();a_fraction.reset();a_polynomial.reset();};
    // Pastes a formula to this one
    void __Formula_Base::paste(__Formula_Base* value){a_applied_function=value->applied_function_copy();a_fraction = value->fraction_copy();a_polynomial = value->polynomial_copy();};
    // Soft_resets the formula
    void __Formula_Base::soft_reset(){a_to_double.clear();a_to_double_containers.clear();}
    // Sub-place the current formula to a "formula add"
    void __Formula_Base::sub_place(){std::shared_ptr<__Formula_Base> needed_copy = clone();clear();a_fraction = std::make_shared<__Formula_Base::Formula_Fraction>(needed_copy);};

    // Returns a copy of the formula
    std::shared_ptr<__Formula_Base::Formula_Fraction> __Formula_Base::fraction_copy()const{if(a_fraction.get()==0){return std::shared_ptr<__Formula_Base::Formula_Fraction>();} return a_fraction.get()->fraction_copy();};
    std::shared_ptr<Polynomial_Base> __Formula_Base::polynomial_copy()const{if(a_polynomial.get()==0){return std::shared_ptr<Polynomial>();}return a_polynomial.get()->clone();};
    // Returns a part of the formula
    __Monomonial_Base* __Formula_Base::__monomonial() const {if(__polynomial() == 0 || __polynomial()->__monomonial() == 0){return 0;}return __polynomial()->__monomonial();};
    Polynomial_Base* __Formula_Base::Formula_Sum::polynomial() const {for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){if(a_formulas_add.at(i).get()->is_simple_polynomial()){return a_formulas_add.at(i).get()->polynomial();}}return 0;}
    Polynomial_Base* __Formula_Base::Formula_Factor::polynomial() const {for(int i = 0;i<static_cast<int>(a_factors.size());i++){if(a_factors.at(i).get()->is_simple_polynomial()){return a_factors.at(i).get()->__polynomial();}}return 0;};
    Polynomial_Base* __Formula_Base::Formula_Fraction::polynomial() const {return a_numerator.get()->polynomial();}
    Polynomial_Base* __Formula_Base::__polynomial() const {if(a_polynomial.get() != 0){return a_polynomial.get();}else if(a_fraction.get()==0){return 0;} return a_fraction.get()->polynomial();};

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
            Polynomial_Base* needed_polynomial = a_polynomial.get();
            if(!needed_polynomial->is_null()) {
                current_str = needed_polynomial->to_mathml(settings);
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
            Polynomial_Base* needed_polynomial = a_polynomial.get();
            if(!needed_polynomial->is_null() || (settings != 0 && !settings->hide_if_0())) {
                current_str = needed_polynomial->to_std_string(settings);
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
    void __Formula_Base::formula_from_modified_monomonial_unknows(__Formula_Base* final_formula, __Monomonial_Base* used_monomonial, std::string unknown, __Formula_Base* new_value) {
        _Base_Unknown* unknown_ptr = used_monomonial->contains_unknown(unknown);
        if(unknown_ptr!= 0) {
            // The monomonial is a multiplication between the unknown and the base value
            Complex exponent = unknown_ptr->exponent();
            used_monomonial->delete_unknown(unknown);
            // Create the good value
            int needed_exponent = static_cast<int>(exponent.real().to_double());
            final_formula->paste(new_value);
            if(needed_exponent > 0){for(int i = 1;i<needed_exponent;i++) {final_formula->__multiply(new_value);}}
            else{for(int i = 0;i<-needed_exponent;i++) {final_formula->__divide(new_value);}}
            final_formula->__multiply(used_monomonial);
        }
        else {final_formula->set_polynomial(used_monomonial);}
    };
    // Returns a formula from a Polynomial where the unknows has been replaced
    void __Formula_Base::formula_from_modified_polynomial_unknows(__Formula_Base* final_formula, Polynomial_Base* used_polynomial, std::string unknown, __Formula_Base* new_value) {
        // Browse each Polynomials
        std::vector<std::shared_ptr<__Monomonial_Base>>& monomonials = used_polynomial->monomonials();
        std::shared_ptr<__Formula_Base> temp = final_formula->new_formula();std::shared_ptr<__Formula_Base> total = final_formula->new_formula();
        for(int i = 0;i<static_cast<int>(monomonials.size());i++) {
            formula_from_modified_monomonial_unknows(temp.get(), used_polynomial->monomonials().at(i).get(), unknown, new_value);
            total.get()->__add(temp.get());
        }
        final_formula->paste(total.get());
    };
    // Returns a formula where an unkown is replaced by an another unknown
    // Returns a Formula_Factor where an unkown is replaced by an another unknown
    void __Formula_Base::Formula_Factor::replace_unknown(__Formula_Base* to_return, std::string unknown, __Formula_Base* new_value) const {
        // Replace each factors
        for(int i = 0;i<static_cast<int>(a_factors.size());i++) {std::shared_ptr<__Formula_Base> current=std::shared_ptr<__Formula_Base>(a_factors.at(i).get()->replace_unknown_new(unknown, new_value));to_return->__multiply(current.get());}
    }
    __Formula_Base* __Formula_Base::replace_unknown_new(std::string unknown, __Formula_Base* new_value) const {
        __Formula_Base* final_formula = clone_new();
        final_formula->set_applied_function(applied_function_shared_ptr());
        if(final_formula->is_multiplication_neutral()) {std::shared_ptr<__Formula_Base_Function> used_function = applied_function_shared_ptr(); final_formula->clear();final_formula->set_applied_function(used_function);}

        // Add the added Polynomial
        if(a_polynomial.get() != 0) {formula_from_modified_polynomial_unknows(final_formula, a_polynomial.get(), unknown, new_value);}

        // Returns the result
        final_formula->set_applied_function(applied_function_shared_ptr());
        return final_formula;
    };
    __Formula_Base* __Formula_Base::replace_unknowns_new(Unknowns_Container* values) const{
        // Get the needed datas
        __Formula_Base* current_formula = clone_new();
        std::vector<std::string> unknowns = all_unknowns();

        // Get the needed value
        if(static_cast<int>(unknowns.size()) > 0 && values == 0){print("Warning", std::string("SCLS Carl Math"), std::string("Can't use unknowns with the formula \"") + to_std_string(0) + std::string("\" : now unknown container used."));}
        for(int i = 0;i<static_cast<int>(unknowns.size());i++) {__Formula_Base* needed_value=values->value_by_name(unknowns[i]);if(needed_value!=0){current_formula = current_formula->replace_unknown_new(unknowns[i], needed_value);}}

        // Return the result
        if(a_applied_function.get() != 0){current_formula->set_applied_function(a_applied_function.get()->function_copy());}
        return current_formula;
    };

    // Methods operators
    // Add a formula to this one
    void __Formula_Base::Formula_Fraction::__add(__Formula_Base* value){std::shared_ptr<__Formula_Base>temp=value->clone();if(a_denominator.get() != 0){temp.get()->__multiply(a_denominator.get());}a_numerator.get()->__add(temp.get());};
    void __Formula_Base::__add(__Formula_Base* value) {
        // Asserts
        if(value == 0 || value->is_null()) {return;}
        else if(is_null()) {paste(value);return;}

        // Does the redaction
        if(a_redaction != 0) {(*a_redaction) += std::string(std::string("Nous cherchon � rajouter la formule \"") + value->to_std_string(0) + std::string("\" � \"") + to_std_string(0) + std::string("."));}

        // Check if values are both Polynomial
        if(is_simple_polynomial() && value->is_simple_polynomial()){
            Polynomial_Base* temp = value->__polynomial();
            a_polynomial.get()->__add(temp);
            if(a_redaction != 0) {(*a_redaction) += std::string(std::string("Or, il s'agit d'une simple addition de polyn�me."));}
        }
        else {
            if(!is_basic() || is_simple_polynomial()){sub_place();}
            a_fraction.get()->__add(value);
            if(a_redaction != 0) {
                (*a_redaction) += std::string(std::string("Or, il s'agit d'une addition de termes non-additionnables."));
            }
        }

        // Finish the result
        check_formula();
        soft_reset();
    };
    // Divide a formula to this one
    void __Formula_Base::Formula_Factor::__divide(__Formula_Base* value) {basic_formula()->__divide(value);}
    void __Formula_Base::Formula_Fraction::__divide(__Formula_Base* value){
        // Handle the division
        if(!value->is_basic() || !is_simple_polynomial() || true) {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
        // TEMPORARY DISABLED
        /*else {
            if(value->is_simple_monomonial()) {
                // Apply a division of a simple monomonial
                __Monomonial_Base used_monomonial* = value->__polynomial()->__monomonial();
                // Handle the complex part
                if(used_monomonial.factor()->imaginary() != 0) {
                    // Get the needed divisor
                    scls::__Formula_Base needed_divisor = used_monomonial;
                    needed_divisor *= used_monomonial.factor()->conjugate();
                    __divide(needed_divisor);
                    // Multiply the numerator
                    __multiply(scls::Polynomial(used_monomonial.factor()->conjugate(), "", 1));
                }
                else if(!used_monomonial.is_known()) {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
                else {a_numerator.get()->__divide(used_monomonial);}
            }
            else if(value.is_simple_polynomial()) {
                // Apply a division of a simple Polynomial
                Polynomial used_Polynomial = value.to_polynomial();
                std::shared_ptr<Polynomial_Complex> needed_complex = Polynomial_Complex::from_polynomial(&used_Polynomial);
                if(needed_complex.get()->imaginary_polynomial()->is_null()) {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
                else {
                    // Handle the complex part
                    std::shared_ptr<Polynomial_Complex> needed_conjugate = needed_complex.get()->conjugate();
                    Polynomial needed_polynomial = (*needed_conjugate.get()->to_polynomial());
                    used_Polynomial.__multiply(&needed_polynomial);
                    if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(used_Polynomial);}else{a_denominator.get()->__multiply(used_Polynomial);}
                    // Multiply the numerator
                    __multiply(needed_polynomial);
                }
            }
            else {if(a_denominator.get() == 0){a_denominator=std::make_shared<Formula_Sum>(value);}else{a_denominator.get()->__multiply(value);}}
        }//*/
    }
    void __Formula_Base::__divide(__Formula_Base* value) {
        // Redaction
        if(a_redaction != 0) {
            (*a_redaction) += std::string(std::string("Nous cherchons � diviser la formule \"") + value->to_std_string(0) + std::string("\" � \"") + to_std_string(0) + std::string("."));
        }

        // Check if values are both Polynomial
        if(is_basic() && is_simple_polynomial() && value->is_simple_monomonial()) {
            __Monomonial_Base* temp = value->__monomonial();
            a_polynomial.get()->__divide(temp);
            if(a_redaction != 0) {
                (*a_redaction) += std::string(std::string("Or, il s'agit d'une simple division polyn�me / mon�me."));
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
    bool __Formula_Base::Formula_Sum::__is_equal(Polynomial_Base* value)const{return a_formulas_add.size() == 1 && a_formulas_add.at(0).get()->__is_equal(value);}
    bool __Formula_Base::Formula_Sum::__is_equal(__Formula_Base* value)const{return a_formulas_add.size() == 1 && a_formulas_add.at(0).get()->__is_equal(value);}
    bool __Formula_Base::Formula_Factor::__is_equal(Polynomial_Base* value)const{return a_factors.size() == 1 && a_factors.at(0).get()->__is_equal(value);}
    bool __Formula_Base::Formula_Factor::__is_equal(__Formula_Base* value)const{return a_factors.size() == 1 && a_factors.at(0).get()->__is_equal(value);}
    bool __Formula_Base::__is_equal(Polynomial_Base* value)const{std::shared_ptr<__Formula_Base>temp=new_formula();temp.get()->set_polynomial(value);return __is_equal(temp.get());}
    bool __Formula_Base::__is_equal(__Formula_Base* value)const{
        if(!((a_applied_function.get() == 0 && value->a_applied_function.get() == 0) || (a_applied_function.get() != 0 && value->a_applied_function.get() != 0 && a_applied_function.get()->name() == value->a_applied_function.get()->name()))){return false;}

        // Polynomial
        if(a_fraction.get() == 0 && value->fraction() == 0) {return a_polynomial.get()->__is_equal(value->__polynomial());}

        // Fraction
        else if((a_fraction.get() != 0 && value->fraction() == 0) || (a_fraction.get() == 0 && value->fraction() != 0)){return false;}
        std::shared_ptr<__Formula_Base>temp=std::shared_ptr<__Formula_Base>(value->__internal_value_new());return fraction()->__is_equal(temp.get());
    };
    // Multiply a Polynomial to this one
    void __Formula_Base::Formula_Sum::__multiply(Polynomial_Base* value) {for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){a_formulas_add.at(i).get()->__multiply(value);}}
    void __Formula_Base::Formula_Sum::__multiply(__Formula_Base* value) {for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){a_formulas_add.at(i).get()->__multiply(value);}}
    void __Formula_Base::Formula_Factor::__multiply(Polynomial_Base* value) {if(!basic_formula_exists()){basic_formula()->set_polynomial(value);}else{basic_formula()->__multiply(value);}}
    void __Formula_Base::Formula_Factor::__multiply(__Formula_Base* value){
        if(value->is_simple_polynomial()){__multiply(value->__polynomial());}
        else{a_factors.push_back(value->clone());}
    }
    void __Formula_Base::Formula_Fraction::__multiply(Polynomial_Base* value) {if(a_denominator.get() != 0 && a_denominator.get()->__is_equal(value)){a_denominator.reset();}else{a_numerator.get()->__multiply(value);}}
    void __Formula_Base::Formula_Fraction::__multiply(__Formula_Base* value) {if(a_denominator.get() != 0 && a_denominator.get()->__is_equal(value)){a_denominator.reset();}else{a_numerator.get()->__multiply(value);}}
    void __Formula_Base::__multiply(__Monomonial_Base* value){std::shared_ptr<__Formula_Base>temp=new_formula();temp.get()->set_polynomial(value);__multiply(temp.get());}
    void __Formula_Base::__multiply(Polynomial_Base* value){std::shared_ptr<__Formula_Base>temp=new_formula();temp.get()->set_polynomial(value);__multiply(temp.get());}
    void __Formula_Base::__multiply(__Formula_Base::Formula_Sum* value){std::shared_ptr<__Formula_Base>temp=new_formula();temp.get()->set_polynomial(value);__multiply(temp.get());}
    void __Formula_Base::__multiply(__Formula_Base* value) {
        // Redaction
        if(a_redaction != 0) {
            (*a_redaction) += std::string(std::string("Nous cherchon � multiplier la formule \"") + value->to_std_string(0) + std::string("\" � \"") + to_std_string(0) + std::string("."));
        }

        // Check Polynomial
        if(is_simple_polynomial() && value->is_simple_polynomial()) {
            Polynomial_Base* temp = value->__polynomial();
            a_polynomial.get()->__multiply(temp);
            if(a_redaction != 0) {
                (*a_redaction) += std::string(std::string("Or, il s'agit d'une simple multiplication de polyn�mes."));
            }
        }
        else {
            // Handle functions
            bool control = false;
            if(applied_function() != 0){
                std::shared_ptr<__Formula_Base> new_value = applied_function()->multiply(this, value);
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
    // Substract a value to this one
    void __Formula_Base::__substract(__Formula_Base* value){std::shared_ptr<__Formula_Base> temp = value->opposite();__add(temp.get());}

    // Multiply a value with the function
    std::shared_ptr<__Formula_Base> __Sqrt_Function::multiply(__Formula_Base* value_1, __Formula_Base* value_2){
        if(value_1->applied_function() != 0 && value_2->applied_function() != 0 && value_1->applied_function()->name() == value_2->applied_function()->name()) {
            std::shared_ptr<__Formula_Base> copied_1 = value_1->clone();
            std::shared_ptr<__Formula_Base> copied_2 = value_2->clone();
            copied_1.get()->clear_applied_function();copied_2.get()->clear_applied_function();

            // Same value
            if(copied_1.get()->__is_equal(copied_2.get())){return copied_1;}

            // Normal multiplication
            copied_1.get()->__multiply(copied_2.get());
            copied_1.get()->set_applied_function<__Sqrt_Function>();
            return copied_1;
        }

        return std::shared_ptr<__Formula_Base>();
    };

}
