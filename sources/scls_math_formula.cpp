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
	// The "Formula_Base" class
	//
	//*********

	void Formula_Base::Formula_Unknown::set_value(std::shared_ptr<Formula_Base> e){value=e;}
	void Formula_Base::Formula_Unknown::set_value(std::shared_ptr<Fraction> e){value=std::make_shared<Formula_Base>(e);}
	void Formula_Base::Formula_Unknown::set_value(Fraction e){set_value(std::make_shared<Fraction>(e));}

	// Formula operator
    Algebra_Element::Algebra_Operators formula_operators = Algebra_Element::Algebra_Operators({Algebra_Element::Algebra_Operator("^"), Algebra_Element::Algebra_Operator("/"), Algebra_Element::Algebra_Operator("*"), Algebra_Element::Algebra_Operator("+")}, {Algebra_Element::Algebra_Operator("abs", 1), Algebra_Element::Algebra_Operator("ln", 1), Algebra_Element::Algebra_Operator("exp", 1), Algebra_Element::Algebra_Operator("sqrt", 1), Algebra_Element::Algebra_Operator("cos", 1), Algebra_Element::Algebra_Operator("factorial", 1), Algebra_Element::Algebra_Operator("sin", 1), Algebra_Element::Algebra_Operator("tan", 1)});

    // Type of the object
    std::string Formula_Base::algebra_type() const{return std::string("formula_base");};
    std::string Extendable_Formula_Base::algebra_type() const{return std::string("extandable_formula_base");};

    // Returns a part of the formula
    Extendable_Formula_Base* Extendable_Formula_Base::formula_element(int index){return reinterpret_cast<Extendable_Formula_Base*>(algebra_elements_const().at(index).get());}
    Formula_Base* Formula_Base::formula_element(int index){return reinterpret_cast<Formula_Base*>(algebra_elements_const().at(index).get());}

    // Extendable_Formula_Base operators
    // Extendable_Formula_Base operations
    void Extendable_Formula_Base::add(Extendable_Formula_Base* formula){Formula_Base_Field::add(formula);};
    void Extendable_Formula_Base::divide(Extendable_Formula_Base* formula){Formula_Base_Field::divide(formula);};
    void Extendable_Formula_Base::multiply(Extendable_Formula_Base* formula){Formula_Base_Field::multiply(formula);};
    void Extendable_Formula_Base::operate(Algebra_Element* formula, std::string operation){Formula_Base_Field::operate(formula, operation);};
    void Extendable_Formula_Base::substract(Extendable_Formula_Base* formula){std::shared_ptr<Extendable_Formula_Base>f=formula->clone();f.get()->multiply(-1);add(f.get());}
    // Fraction operations
    void Extendable_Formula_Base::add(Extendable_Fraction other){std::shared_ptr<Extendable_Formula_Base>f=std::make_shared<Extendable_Formula_Base>(other);add(f.get());}
    void Extendable_Formula_Base::divide(Extendable_Fraction other){std::shared_ptr<Extendable_Formula_Base>f=std::make_shared<Extendable_Formula_Base>(other);divide(f.get());}
    void Extendable_Formula_Base::multiply(Extendable_Fraction other){std::shared_ptr<Extendable_Formula_Base>f=std::make_shared<Extendable_Formula_Base>(other);multiply(f.get());}
    void Extendable_Formula_Base::operate(Extendable_Fraction other, std::string operation){std::shared_ptr<Extendable_Formula_Base> temp = std::make_shared<Extendable_Formula_Base>(other);operate(temp.get(), operation);}
    void Extendable_Formula_Base::substract(Extendable_Fraction other){Extendable_Fraction temp = Extendable_Fraction(-1);other.multiply(&temp);std::shared_ptr<Extendable_Formula_Base>f=std::make_shared<Extendable_Formula_Base>(other);add(f.get());}

    // Formula_Base operators
    // Formula_Base operations
    void Formula_Base::add(Formula_Base* formula){Formula_Base_Field::add(formula);};
    void Formula_Base::divide(Formula_Base* formula){Formula_Base_Field::divide(formula);};
    void Formula_Base::multiply(Formula_Base* formula){Formula_Base_Field::multiply(formula);};
    void Formula_Base::operate(Algebra_Element* formula, std::string operation){Formula_Base_Field::operate(formula, operation);};
    void Formula_Base::substract(Formula_Base* formula){std::shared_ptr<Formula_Base>f=formula->clone();f.get()->multiply(-1);add(f.get());}
    // Fraction operations
    void Formula_Base::add(Fraction other){std::shared_ptr<Formula_Base>f=std::make_shared<Formula_Base>(other);add(f.get());}
    void Formula_Base::divide(Fraction other){std::shared_ptr<Formula_Base>f=std::make_shared<Formula_Base>(other);divide(f.get());}
    void Formula_Base::multiply(Fraction other){std::shared_ptr<Formula_Base>f=std::make_shared<Formula_Base>(other);multiply(f.get());}
    void Formula_Base::operate(Fraction other, std::string operation){std::shared_ptr<Formula_Base> temp = std::make_shared<Formula_Base>(other);operate(temp.get(), operation);}
    void Formula_Base::substract(Fraction other){std::shared_ptr<Formula_Base>f=std::make_shared<Formula_Base>(other * -1);add(f.get());}
    // Do the operation without modification
    std::shared_ptr<Formula_Base> Formula_Base::multiplied(Fraction other){std::shared_ptr<Formula_Base> to_return = clone();to_return.get()->multiply(other);return to_return;}

    // Returns the neutral of known operations
    std::shared_ptr<Algebra_Element> Formula_Base::operation_neutral(std::string operation) const {return new_formula(algebra_value_for_type()->operation_neutral(operation));}
    std::shared_ptr<Algebra_Element> Extendable_Formula_Base::operation_neutral(std::string operation) const {return new_formula(algebra_value_for_type()->operation_neutral(operation));}

    // Returns a set of number which respects a precise relatio,
    Set_Number Formula_Base::check_relation(Relation* r, Formula_Base* f) {
    	Set_Number to_return = Set_Number::real();

    	// Check the relation
    	if(r->relation_type() == Relation::Relation_Type::Egality) {
    		//Relation_Egality* relation = static_cast<Relation_Egality*>(r);
    		if(f->equals(Fraction(0))) {
    			std::cout << "E " << to_std_string(0) << " " << f->to_std_string(0) << std::endl;
    		}
    		else{
    			// Do the comparison with zero
    			std::shared_ptr<Formula_Base> g = clone();
    			g.get()->substract(f);
    			std::shared_ptr<Formula_Base> zero = std::make_shared<Formula_Base>(Fraction(0));
    			g.get()->simplify();
    			return g.get()->check_relation(r, zero.get());
    		}
    	}

    	return to_return;
    }

    // Creates the unknown
    Algebra_Element::__Algebra_Unknown* Extendable_Formula_Base::create_unknown(){clear();a_unknown = std::make_shared<Extended_Formula_Unknown>();return a_unknown.get();};
    Algebra_Element::__Algebra_Unknown* Formula_Base::create_unknown(){clear();a_unknown = std::make_shared<Formula_Unknown>();return a_unknown.get();};

    // Returns the definition domain
    Set_Number Formula_Base::definition_domain() {
        Set_Number to_return = Set_Number::real();

        // Check operators
        if(algebra_operator_name() == std::string("ln")){to_return = Set_Number::real_positive();to_return.exclude(Fraction(0));}
        else if(algebra_operator_name() == std::string("sqrt")){to_return = Set_Number::real_positive();}

        // Do the needed_intersection
        for(std::size_t i = 0;i<algebra_elements().size();i++){to_return = to_return.intersection(formula_element(i)->definition_domain());}

        return to_return;
    }

    // Returns if a precise number is defined or not
    bool Formula_Base::is_defined(Fraction f) {
        if(!is_final_element()) {

        }

        return true;
    }
    bool Formula_Base::is_fully_defined(Fraction f_1, Fraction f_2) {


        // Do the needed_intersection
        for(std::size_t i = 0;i<algebra_elements().size();i++){if(!formula_element(i)->is_fully_defined(f_1, f_2)){return false;}}

        return true;
    }

    // Elements in a Formula_Base
    const Algebra_Element::Algebra_Operators& Extendable_Formula_Base::operators() const {return formula_operators;}
    const Algebra_Element::Algebra_Operators& Formula_Base::operators() const {return formula_operators;}

    // Replaces the unknowns
    // Extendable
    void Extendable_Formula_Base::replace_unknowns_algebra(Algebra_Element* element, Algebra_Element::Unknowns_Container* values_raw) const {
        Extendable_Formula_Base::Unknowns_Container* values = reinterpret_cast<Extendable_Formula_Base::Extendable_Unknowns_Container*>(values_raw);

        // The element is final
        if(is_final_element()) {
            if(is_unknown()){
                Extended_Formula_Unknown* current = reinterpret_cast<Extended_Formula_Unknown*>(values->algebra_unknown_by_name(algebra_unknown()->name));
                if(current == 0){algebra_clone(element);}
                else{current->value.get()->clone(reinterpret_cast<Extendable_Formula_Base*>(element));}
            }
            else if(a_value.get() != 0) {reinterpret_cast<Extendable_Formula_Base*>(element)->a_value = a_value.get()->algebra_clone();}
            else{reinterpret_cast<Extendable_Formula_Base*>(element)->a_value = std::make_shared<Extendable_Fraction>(Extendable_Int(0));}
        }
        else {Algebra_Element::replace_unknowns_algebra(element, values);}

        // Modified
        reinterpret_cast<Extendable_Formula_Base*>(element)->a_modified = true;

        // Simplification
        element->simplify();
    }
    std::shared_ptr<Formula_Base_Field> Extendable_Formula_Base::replace_unknowns_formula(std::string unknown, Algebra_Element* element) const{return replace_unknowns(unknown, *reinterpret_cast<Extendable_Fraction*>(element));};
    std::shared_ptr<Extendable_Formula_Base> Extendable_Formula_Base::replace_unknowns(std::string unknown, Extendable_Fraction f) const {
        Extendable_Unknowns_Container c;c.create_unknown(unknown)->value = new_formula(f);
        return replace_unknowns(&c);
    }
    std::shared_ptr<Extendable_Formula_Base> Extendable_Formula_Base::replace_unknowns(Unknowns_Container* values) const {
        // Create the object
        std::shared_ptr<Extendable_Formula_Base> s = std::make_shared<Extendable_Formula_Base>();
        s.get()->a_this_object=s;replace_unknowns_algebra(s.get(), values);

        // Simplify
        s.get()->simplify();
        return s;
    }
    // Normal
    void Formula_Base::replace_unknowns_algebra(Algebra_Element* element, Algebra_Element::Unknowns_Container* values_raw) const {
        Formula_Base::Unknowns_Container* values = reinterpret_cast<Formula_Base::Unknowns_Container*>(values_raw);

        // The element is final
        if(is_final_element()) {
            if(is_unknown()){
                Formula_Unknown* current = reinterpret_cast<Formula_Unknown*>(values->algebra_unknown_by_name(algebra_unknown()->name));
                if(current == 0){algebra_clone(element);}
                else{current->value.get()->clone(reinterpret_cast<Formula_Base*>(element));}
            }
            else if(a_value.get() != 0) {reinterpret_cast<Formula_Base*>(element)->a_value = a_value.get()->algebra_clone();}
            else{reinterpret_cast<Formula_Base*>(element)->a_value = std::make_shared<Fraction>(0);}
        }
        else {Algebra_Element::replace_unknowns_algebra(element, values);}

        // Modified
        reinterpret_cast<Formula_Base*>(element)->a_modified = true;

        // Simplification
        element->simplify();
    }
    std::shared_ptr<Formula_Base_Field> Formula_Base::replace_unknowns_formula(std::string unknown, Algebra_Element* element) const{return replace_unknowns(unknown, *reinterpret_cast<Fraction*>(element));};
    std::shared_ptr<Formula_Base> Formula_Base::replace_unknowns(std::string unknown, Fraction f) const {
        Unknowns_Container c;c.create_unknown(unknown)->value = new_formula(f.to_std_string(0));
        return replace_unknowns(&c);
    }
    std::shared_ptr<Formula_Base> Formula_Base::replace_unknowns(Unknowns_Container* values) const {
        // Create the object
        std::shared_ptr<Formula_Base> s = std::make_shared<Formula_Base>();
        s.get()->a_this_object=s;replace_unknowns_algebra(s.get(), values);

        // Simplify
        s.get()->simplify();
        return s;
    }

    // Equality operators
    bool Formula_Base::equals(Fraction f) {return is_known() && (*value<Fraction>()) == f;}

    // Simplify the formula
    char Extendable_Formula_Base::simplify_step() {
    	//if(is_known()){value<Extendable_Fraction>()->normalize(2);}
    	return Formula_Base_Field::simplify_step();
    }
    char Formula_Base::simplify_step() {
    	//if(is_known()){value<Fraction>()->normalize(6);}
    	return Formula_Base_Field::simplify_step();
    }

    // Different kinds of values
    double Extendable_Formula_Base::value_to_double(Extendable_Unknowns_Container* values) const {std::shared_ptr<Extendable_Formula_Base> u = replace_unknowns(values);return u.get()->value_to_double();};
    double Extendable_Formula_Base::value_to_double() const {if(a_value.get() == 0){return 0;}return value<Extendable_Fraction>()->to_double();};
    double Formula_Base::value_to_double(Unknowns_Container* values) const {std::shared_ptr<Formula_Base> u = replace_unknowns(values);return u.get()->value_to_double();};
    double Formula_Base::value_to_double() const {if(a_value.get() == 0){return 0;}return value<Fraction>()->to_double();};

    // Get an non-empty pointer containing an "T" object (useful if algebra_value() == 0 but the formula as children)
    Algebra_Element* Extendable_Formula_Base::algebra_value_for_type() const{return a_value_for_type.get();};
    Algebra_Element* Formula_Base::algebra_value_for_type() const{return a_value_for_type.get();};

    // Returns the continuity domain
	Set_Number continuity_domain() {return Set_Number::real();}

	// Returns the derivability domain
	Set_Number deravibility_domain() {return Set_Number::real();}

    // Derivate a formula
    long long factorial(long long f){if(f == 0){return 1;}return f * factorial(f - 1);}
    std::shared_ptr<scls::Extendable_Formula_Base> derivate(scls::Extendable_Formula_Base* formula, std::string unknown) {
        std::shared_ptr<scls::Extendable_Formula_Base> to_return = std::make_shared<scls::Extendable_Formula_Base>();

        if(formula->is_final_element()) {
            if(formula->is_known() || formula->algebra_unknown_name() != unknown){to_return = std::make_shared<scls::Extendable_Formula_Base>(scls::Fraction(0));}
            else {to_return = std::make_shared<scls::Extendable_Formula_Base>(scls::Fraction(1));}
        }
        else if(formula->algebra_elements_const().size() == 1){
            std::string function_name = formula->algebra_operator_name();
            std::shared_ptr<scls::Extendable_Formula_Base> content = formula->formula_element(0)->clone();
            std::shared_ptr<scls::Extendable_Formula_Base> content_derivated = derivate(content.get(), unknown);

            // Get the good function
            to_return = content;
            std::string new_function = std::string();
            if(function_name == std::string_view("cos")){new_function = std::string("sin");}
            else if(function_name == std::string_view("sin")){new_function = std::string("cos");}
            else if(function_name == std::string_view("exp")){new_function = std::string("exp");}
            else if(function_name == std::string_view("tan")){new_function = std::string("tan");}
            if(new_function != std::string_view()){
                to_return.get()->sub_place();
                to_return.get()->set_algebra_operator(new_function, 1);
            }

            // Post-function operation
            if(function_name == std::string_view("cos")){to_return.get()->operate(-1, "*");}
            else if(function_name == std::string_view("ln")){std::shared_ptr<scls::Extendable_Formula_Base> temp = to_return.get()->clone();to_return = std::make_shared<scls::Extendable_Formula_Base>(scls::Fraction(1));to_return.get()->operate(temp.get(), std::string("/"));}
            else if(function_name == std::string_view("tan")){std::shared_ptr<scls::Extendable_Formula_Base> temp = std::make_shared<scls::Extendable_Formula_Base>(scls::Fraction(1));to_return.get()->operate(to_return.get(), std::string("*"));to_return.get()->operate(temp.get(), std::string("+"));}

            // Post composition operation
            to_return.get()->operate(content_derivated.get(), "*");
        }
        else {
            std::string needed_operator = formula->algebra_operator_name();
            if(needed_operator == std::string_view("+")) {
                to_return = derivate(formula->formula_element(0), unknown);

                for(std::size_t i = 1;i<formula->algebra_elements_const().size();i++) {
                    std::shared_ptr<scls::Extendable_Formula_Base> current_derivate = derivate(formula->formula_element(i), unknown);

                    // Chain rule
                    to_return.get()->operate(current_derivate.get(), "+");
                }
            }
            else if(needed_operator == std::string_view("*")) {
                to_return = formula->formula_element(0)->clone();
                std::shared_ptr<scls::Extendable_Formula_Base> next_derivate = derivate(to_return.get(), unknown);

                for(std::size_t i = 1;i<formula->algebra_elements_const().size();i++) {
                    std::shared_ptr<scls::Extendable_Formula_Base> current = formula->formula_element(i)->clone();
                    std::shared_ptr<scls::Extendable_Formula_Base> current_derivate = derivate(formula->formula_element(i), unknown);
                    std::shared_ptr<scls::Extendable_Formula_Base> to_return_current = to_return.get()->clone();
                    std::shared_ptr<scls::Extendable_Formula_Base> to_return_derivate = next_derivate.get()->clone();

                    // Chain rule
                    to_return.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(current_derivate.get(), "*");
                    to_return_derivate.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(to_return_derivate.get(), "+");
                    next_derivate = to_return_current;
                }

                to_return = next_derivate;
            }
            else if(needed_operator == std::string_view("/")) {
                to_return = formula->formula_element(0)->clone();
                std::shared_ptr<scls::Extendable_Formula_Base> next_derivate = derivate(to_return.get(), unknown);

                for(std::size_t i = 1;i<formula->algebra_elements_const().size();i++) {
                    std::shared_ptr<scls::Extendable_Formula_Base> current = formula->formula_element(i)->clone();
                    std::shared_ptr<scls::Extendable_Formula_Base> current_derivate = derivate(formula->formula_element(i), unknown);
                    std::shared_ptr<scls::Extendable_Formula_Base> to_return_current = to_return.get()->clone();
                    std::shared_ptr<scls::Extendable_Formula_Base> to_return_derivate = next_derivate.get()->clone();

                    // Chain rule
                    to_return.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(current_derivate.get(), "*");
                    to_return_derivate.get()->operate(current.get(), "*");
                    std::shared_ptr<scls::Extendable_Formula_Base>temp=std::make_shared<Extendable_Formula_Base>(Fraction(-1));
                    to_return_current.get()->operate(temp.get(), "*");
                    to_return_current.get()->operate(to_return_derivate.get(), "+");
                    current.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(current.get(), "/");
                    next_derivate = to_return_current;
                }

                to_return = next_derivate;
            }
        }

        to_return.get()->simplify();
        return to_return;
    }
    std::shared_ptr<scls::Formula_Base> derivate(scls::Formula_Base* formula, std::string unknown) {
        std::shared_ptr<scls::Formula_Base> to_return = std::make_shared<scls::Formula_Base>();

        if(formula->is_final_element()) {
            if(formula->is_known() || formula->algebra_unknown_name() != unknown){to_return = std::make_shared<scls::Formula_Base>(scls::Fraction(0));}
            else {to_return = std::make_shared<scls::Formula_Base>(scls::Fraction(1));}
        }
        else if(formula->algebra_elements_const().size() == 1){
            std::string function_name = formula->algebra_operator_name();
            std::shared_ptr<scls::Formula_Base> content = formula->formula_element(0)->clone();
            std::shared_ptr<scls::Formula_Base> content_derivated = derivate(content.get(), unknown);

            // Get the good function
            to_return = content;
            std::string new_function = std::string();
            if(function_name == std::string_view("cos")){new_function = std::string("sin");}
            else if(function_name == std::string_view("sin")){new_function = std::string("cos");}
            else if(function_name == std::string_view("exp")){new_function = std::string("exp");}
            else if(function_name == std::string_view("tan")){new_function = std::string("tan");}
            if(new_function != std::string_view()){
                to_return.get()->sub_place();
                to_return.get()->set_algebra_operator(new_function, 1);
            }

            // Post-function operation
            if(function_name == std::string_view("cos")){to_return.get()->operate(-1, "*");}
            else if(function_name == std::string_view("ln")){std::shared_ptr<scls::Formula_Base> temp = to_return.get()->clone();to_return = std::make_shared<scls::Formula_Base>(scls::Fraction(1));to_return.get()->operate(temp.get(), std::string("/"));}
            else if(function_name == std::string_view("tan")){std::shared_ptr<scls::Formula_Base> temp = std::make_shared<scls::Formula_Base>(scls::Fraction(1));to_return.get()->operate(to_return.get(), std::string("*"));to_return.get()->operate(temp.get(), std::string("+"));}

            // Post composition operation
            to_return.get()->operate(content_derivated.get(), "*");
        }
        else {
            std::string needed_operator = formula->algebra_operator_name();
            if(needed_operator == std::string_view("+")) {
                to_return = derivate(formula->formula_element(0), unknown);

                for(std::size_t i = 1;i<formula->algebra_elements_const().size();i++) {
                    std::shared_ptr<scls::Formula_Base> current_derivate = derivate(formula->formula_element(i), unknown);

                    // Chain rule
                    to_return.get()->operate(current_derivate.get(), "+");
                }
            }
            else if(needed_operator == std::string_view("*")) {
                to_return = formula->formula_element(0)->clone();
                std::shared_ptr<scls::Formula_Base> next_derivate = derivate(to_return.get(), unknown);

                for(std::size_t i = 1;i<formula->algebra_elements_const().size();i++) {
                    std::shared_ptr<scls::Formula_Base> current = formula->formula_element(i)->clone();
                    std::shared_ptr<scls::Formula_Base> current_derivate = derivate(formula->formula_element(i), unknown);
                    std::shared_ptr<scls::Formula_Base> to_return_current = to_return.get()->clone();
                    std::shared_ptr<scls::Formula_Base> to_return_derivate = next_derivate.get()->clone();

                    // Chain rule
                    to_return.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(current_derivate.get(), "*");
                    to_return_derivate.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(to_return_derivate.get(), "+");
                    next_derivate = to_return_current;
                }

                to_return = next_derivate;
            }
            else if(needed_operator == std::string_view("/")) {
                to_return = formula->formula_element(0)->clone();
                std::shared_ptr<scls::Formula_Base> next_derivate = derivate(to_return.get(), unknown);

                for(std::size_t i = 1;i<formula->algebra_elements_const().size();i++) {
                    std::shared_ptr<scls::Formula_Base> current = formula->formula_element(i)->clone();
                    std::shared_ptr<scls::Formula_Base> current_derivate = derivate(formula->formula_element(i), unknown);
                    std::shared_ptr<scls::Formula_Base> to_return_current = to_return.get()->clone();
                    std::shared_ptr<scls::Formula_Base> to_return_derivate = next_derivate.get()->clone();

                    // Chain rule
                    to_return.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(current_derivate.get(), "*");
                    to_return_derivate.get()->operate(current.get(), "*");
                    std::shared_ptr<scls::Formula_Base>temp=std::make_shared<Formula_Base>(Fraction(-1));
                    to_return_current.get()->operate(temp.get(), "*");
                    to_return_current.get()->operate(to_return_derivate.get(), "+");
                    current.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(current.get(), "/");
                    next_derivate = to_return_current;
                }

                to_return = next_derivate;
            }
        }

        to_return.get()->simplify();
        return to_return;
    }

    void derivate(Formula_Base_Field* formula_to_return, Formula_Base_Field* formula_to_derivate, std::string unknown) {
        if(formula_to_derivate->is_final_element()) {
            if(formula_to_derivate->is_known() || formula_to_derivate->algebra_unknown_name() != unknown){formula_to_return->set_value_as_formula(formula_to_return->algebra_value()->operation_neutral("+"));}
            else {formula_to_return->set_value_as_formula(formula_to_return->algebra_value_for_type()->operation_neutral("*"));}
        }
        else if(formula_to_derivate->algebra_elements_const().size() == 1){
            std::string function_name = formula_to_derivate->algebra_operator_name();
            std::shared_ptr<Formula_Base_Field> content = reinterpret_cast<Formula_Base_Field*>(formula_to_derivate->algebra_elements_const().at(0).get())->formula_clone();
            std::shared_ptr<Formula_Base_Field> content_derivated = content.get()->formula_clone();
            derivate(content_derivated.get(), content.get(), unknown);

            // Get the good function
            content.get()->clone(formula_to_return);
            std::string new_function = std::string();
            if(function_name == std::string_view("cos")){new_function = std::string("sin");}
            else if(function_name == std::string_view("sin")){new_function = std::string("cos");}
            else if(function_name == std::string_view("exp")){new_function = std::string("exp");}
            else if(function_name == std::string_view("tan")){new_function = std::string("tan");}
            if(new_function != std::string_view()){
                formula_to_return->sub_place();
                formula_to_return->set_algebra_operator(new_function, 1);
            }

            // Post-function operation
            if(function_name == std::string_view("cos")){formula_to_return->operate(formula_to_return->new_formula_field("-1").get(), "*");}
            else if(function_name == std::string_view("ln")){std::shared_ptr<Formula_Base_Field> temp = formula_to_return->formula_clone();formula_to_return->set_value_as_formula(formula_to_return->operation_neutral("*"));formula_to_return->operate(temp.get(), std::string("/"));}
            else if(function_name == std::string_view("tan")){formula_to_return->operate(formula_to_return, std::string("*"));formula_to_return->operate(formula_to_return->operation_neutral("*").get(), std::string("+"));}

            // Post composition operation
            formula_to_return->operate(content_derivated.get(), "*");
        }
        /*
        else {
            std::string needed_operator = formula->algebra_operator_name();
            if(needed_operator == std::string_view("+")) {
                to_return = derivate(formula->formula_element(0), unknown);

                for(std::size_t i = 1;i<formula->algebra_elements_const().size();i++) {
                    std::shared_ptr<scls::Formula_Base> current_derivate = derivate(formula->formula_element(i), unknown);

                    // Chain rule
                    to_return.get()->operate(current_derivate.get(), "+");
                }
            }
            else if(needed_operator == std::string_view("*")) {
                to_return = formula->formula_element(0)->clone();
                std::shared_ptr<scls::Formula_Base> next_derivate = derivate(to_return.get(), unknown);

                for(std::size_t i = 1;i<formula->algebra_elements_const().size();i++) {
                    std::shared_ptr<scls::Formula_Base> current = formula->formula_element(i)->clone();
                    std::shared_ptr<scls::Formula_Base> current_derivate = derivate(formula->formula_element(i), unknown);
                    std::shared_ptr<scls::Formula_Base> to_return_current = to_return.get()->clone();
                    std::shared_ptr<scls::Formula_Base> to_return_derivate = next_derivate.get()->clone();

                    // Chain rule
                    to_return.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(current_derivate.get(), "*");
                    to_return_derivate.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(to_return_derivate.get(), "+");
                    next_derivate = to_return_current;
                }

                to_return = next_derivate;
            }
            else if(needed_operator == std::string_view("/")) {
                to_return = formula->formula_element(0)->clone();
                std::shared_ptr<scls::Formula_Base> next_derivate = derivate(to_return.get(), unknown);

                for(std::size_t i = 1;i<formula->algebra_elements_const().size();i++) {
                    std::shared_ptr<scls::Formula_Base> current = formula->formula_element(i)->clone();
                    std::shared_ptr<scls::Formula_Base> current_derivate = derivate(formula->formula_element(i), unknown);
                    std::shared_ptr<scls::Formula_Base> to_return_current = to_return.get()->clone();
                    std::shared_ptr<scls::Formula_Base> to_return_derivate = next_derivate.get()->clone();

                    // Chain rule
                    to_return.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(current_derivate.get(), "*");
                    to_return_derivate.get()->operate(current.get(), "*");
                    std::shared_ptr<scls::Formula_Base>temp=std::make_shared<Formula_Base>(Fraction(-1));
                    to_return_current.get()->operate(temp.get(), "*");
                    to_return_current.get()->operate(to_return_derivate.get(), "+");
                    current.get()->operate(current.get(), "*");
                    to_return_current.get()->operate(current.get(), "/");
                    next_derivate = to_return_current;
                }

                to_return = next_derivate;
            }
        }
        //*/

        formula_to_return->simplify();
    }
    //*/

    // Apply a McLaurin serie
    std::shared_ptr<scls::Extendable_Formula_Base> mclaurin(scls::Extendable_Formula_Base* formula, std::shared_ptr<scls::Extendable_Formula_Base>& f_derivated, std::string unknown, int step) {
        if(step == 0){return formula->replace_unknowns(unknown, 0);}

        // Sub-part
        std::shared_ptr<scls::Extendable_Formula_Base> o = mclaurin(formula, f_derivated, unknown, step - 1);

        f_derivated = derivate(f_derivated.get(), unknown);
        while(f_derivated.get()->simplify_step() != scls::Extendable_Formula_Base::NO_SIMPLIFICATION){}
        std::shared_ptr<scls::Extendable_Formula_Base> to_return = f_derivated.get()->replace_unknowns(unknown, 0);
        to_return.get()->operate(scls::Extendable_Fraction(factorial(step)), "/");

        // Add the unknowns
        std::shared_ptr<scls::Extendable_Formula_Base> unknown_formula = std::make_shared<scls::Extendable_Formula_Base>(unknown);
        std::shared_ptr<scls::Extendable_Formula_Base> unknown_formula_const = unknown_formula.get()->clone();
        for(int i = 0;i<step-1;i++){unknown_formula.get()->operate(unknown_formula_const.get(), "*");}
        to_return.get()->operate(unknown_formula.get(), "*");

        to_return.get()->operate(o.get(), "+");

        while(to_return.get()->simplify_step() != scls::Formula_Base::NO_SIMPLIFICATION){}
        return to_return;
    }
    std::shared_ptr<scls::Extendable_Formula_Base> mclaurin(scls::Extendable_Formula_Base* f, std::string unknown, int step){std::shared_ptr<scls::Extendable_Formula_Base>temp=f->clone();return mclaurin(f, temp, unknown, step);}
    std::shared_ptr<scls::Formula_Base> mclaurin(scls::Formula_Base* f, std::shared_ptr<scls::Formula_Base>& f_derivated, std::string unknown, int step) {
        if(step == 0){return f->replace_unknowns(unknown, 0);}

        // Sub-part
        std::shared_ptr<scls::Formula_Base> o = mclaurin(f, f_derivated, unknown, step - 1);

        f_derivated = derivate(f_derivated.get(), unknown);
        while(f_derivated.get()->simplify_step() != scls::Formula_Base::NO_SIMPLIFICATION){}
        std::shared_ptr<scls::Formula_Base> to_return = f_derivated.get()->replace_unknowns(unknown, 0);
        to_return.get()->operate(scls::Fraction(factorial(step)), "/");

        // Add the unknowns
        std::shared_ptr<scls::Formula_Base> unknown_formula = std::make_shared<scls::Formula_Base>(unknown);
        std::shared_ptr<scls::Formula_Base> unknown_formula_const = unknown_formula.get()->clone();
        for(int i = 0;i<step-1;i++){unknown_formula.get()->operate(unknown_formula_const.get(), "*");}
        to_return.get()->operate(unknown_formula.get(), "*");

        to_return.get()->operate(o.get(), "+");

        while(to_return.get()->simplify_step() != scls::Formula_Base::NO_SIMPLIFICATION){}
        return to_return;
    }
    std::shared_ptr<scls::Formula_Base> mclaurin(scls::Formula_Base* f, std::string unknown, int step){std::shared_ptr<scls::Formula_Base>temp=f->clone();return mclaurin(f, temp, unknown, step);}
}
