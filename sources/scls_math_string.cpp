//******************
//
// scls_math_string.cpp
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
// This file contains the source code of scls_math_string.cpp.
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

#include "../scls_math_directory/scls_math_string.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The parsers class
	//
	//*********

	// String_To_Formula_Parse constructor
	String_To_Formula_Parse::String_To_Formula_Parse(unsigned int level):a_level(level){
		add_function(std::string("arccos"));
		add_function(std::string("cos"));
        add_function(std::string("exp"));
        add_function(std::string("ln"));
        add_function(std::string("normal"));
        add_function(std::string("random"));
        add_function(std::string("sin"));
        add_function(std::string("sqrt"));
        add_function(std::string("tan"));
    };

    // Returns if a function is defined or not
    bool String_To_Formula_Parse::contains_function(std::string function_name) const {for(int i = 0;i<static_cast<int>(a_functions.size());i++) {if(a_functions.at(i) == function_name) return true; } return false;};
    bool String_To_Formula_Parse::contains_function(char function_name) const {std::string str;str+=function_name;return contains_function(str);};

	// Returns a given first base string to a formula
	void __string_to_formula_function(__Formula* formula, std::string used_function, const Math_Environment* environment){
        if(used_function == "cos"){formula->add_applied_function<__Cos_Function>();}
        else if(used_function == "exp"){formula->add_applied_function<__Exp_Function>();}
        else if(used_function == "ln"){formula->add_applied_function<__Log_Function>();}
        else if(used_function == "normal"){
            /*formula = 1;
            formula /= __Sqrt_Function::create_formula(2);
            formula *= __Exp_Function::create_formula(__Monomonial(-1, "x", 2));//*/
        }
        else if(used_function == "sin"){formula->add_applied_function<__Sin_Function>();}
        else if(used_function == "sqrt"){formula->add_applied_function<__Sqrt_Function>();}

        // Environment functions
        else if(used_function == "random"){formula->set_polynomial(random_fraction(0, 1));}
        else if(used_function == "repetition"){formula->set_polynomial(environment->repetition(formula->value_to_double()));}
	}
    std::shared_ptr<__Formula> String_To_Formula_Parse::__string_to_formula_base(std::string base, std::string used_function, const Math_Environment* environment) {
        std::shared_ptr<__Formula> formula = std::make_shared<__Formula>();

        if(base.size() < 2 || (base[0] != '(')) {
            // Simple form
            // Get the needed datas
            unsigned int number_i = 0; std::string number_part_1 = ""; std::string number_part_2 = ""; std::string unknow_part = "";
            while(number_i < base.size() && __string_is_number(base[number_i])){number_part_1+=base[number_i];number_i++;}
            while(number_i < base.size() && !__string_is_number(base[number_i])){unknow_part+=base[number_i];number_i++;}
            while(number_i < base.size() && __string_is_number(base[number_i])){number_part_2+=base[number_i];number_i++;}
            // Add the monomonial
            //Complex number = string_to_complex(number_part_1);
            Fraction number = scls::Fraction::from_std_string(number_part_1);
            if(unknow_part != "" && number_part_1 == "") number = 1;
            if(number_part_2 != "") number *= scls::Fraction::from_std_string(number_part_2);
            __Monomonial to_add(number, unknow_part);
            Polynomial current_polymonial;
            current_polymonial.add_monomonial(to_add);
            formula.get()->set_polynomial(&current_polymonial);
        }
        else {
            // Parenthesis form
            String_To_Formula_Parse new_parser(level() + 1);new_parser.a_functions = a_functions;
            formula = new_parser.string_to_formula(base.substr(1, base.size() - 2), environment);
        }

        // Create the formula
        __string_to_formula_function(formula.get(), used_function, environment);
        return formula;
    };

    // Converts a std::string to a Formula
    std::shared_ptr<__Formula> String_To_Formula_Parse::string_to_formula(std::string source){return string_to_formula(source, 0);}
    std::shared_ptr<__Formula> String_To_Formula_Parse::__string_to_formula_without_exponent(std::string source, const Math_Environment* environment) {
        // Format the text as needed
        std::vector<std::string> cutted;

        // Prepare the needed datas
        std::shared_ptr<__Formula> formula = std::make_shared<__Formula>();

        // Cut the text operator by > operator
        cutted = cut_string_out_of_2(source, ">", "(", ")");
        if(cutted.size() > 1) {
            // At least one function applied
            for(int i = 1;i<static_cast<int>(cutted.size());i+=2) {
                std::shared_ptr<__Formula> current_polymonial = __string_to_formula_base(cutted[i], cutted[i - 1], environment);
                formula.get()->add(current_polymonial.get());
            }
        }
        else {formula = __string_to_formula_base(cutted[0], std::string(), environment);}

        // Return the result
        return formula;
    };
    std::shared_ptr<__Formula> String_To_Formula_Parse::__string_to_formula_without_division(std::string source, const Math_Environment* environment) {
        // Format the text as needed
        std::vector<std::string> cutted;

        // Prepare the needed datas
        std::shared_ptr<__Formula> formula = std::make_shared<__Formula>(); bool to_return_modified = false;

        // Cut the text operator by ^ operator
        cutted = cut_string_out_of_2(source, "^", "(", ")");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::shared_ptr<__Formula> current_polymonial = __string_to_formula_without_exponent(cutted[i], environment);
            if(to_return_modified){
                if(current_polymonial.get()->is_simple_polynomial()){
                    Polynomial_Base* p = current_polymonial.get()->polynomial();
                    if(p->is_known()){
                        double r = current_polymonial.get()->value_to_double(0);std::shared_ptr<__Formula> f = formula.get()->clone();
                        for(int j=1;j<r;j++){f.get()->multiply(formula.get());}formula->paste(f.get());
                    }
                }
            }
            else {formula = current_polymonial;to_return_modified = true;}
        }

        // Return the result
        return formula;
    };
    std::shared_ptr<__Formula> String_To_Formula_Parse::__string_to_formula_without_multiplication(std::string source, const Math_Environment* environment) {
        // Format the text as needed
        std::vector<std::string> cutted;

        // Prepare the needed datas
        std::shared_ptr<__Formula> formula = std::make_shared<__Formula>(); bool to_return_modified = false;

        // Cut the text operator by / operator
        cutted = cut_string_out_of_2(source, "/", "(", ")");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::shared_ptr<__Formula> current_polymonial = __string_to_formula_without_division(cutted[i], environment);
            if(to_return_modified) {formula.get()->divide(current_polymonial.get());}
            else {formula.get()->__add(current_polymonial.get());to_return_modified=true;}
        }

        // Return the result
        return formula;
    };
    std::shared_ptr<__Formula> String_To_Formula_Parse::__string_to_formula_without_addition(std::string source, const Math_Environment* environment) {
        // Format the text as needed
        std::vector<std::string> cutted;

        // Prepare the needed datas
        std::shared_ptr<__Formula> formula = std::make_shared<__Formula>(); bool to_return_modified = false;

        // Cut the text operator by * operator
        cutted = cut_string_out_of_2(source, "*", "(", ")");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::shared_ptr<__Formula> current_polymonial = __string_to_formula_without_multiplication(cutted[i], environment);
            if(to_return_modified) {formula.get()->__multiply(current_polymonial.get());}
            else {formula.get()->__add(current_polymonial.get());to_return_modified=true;}
        }

        // Return the result
        return formula;
    };
    std::shared_ptr<__Formula> String_To_Formula_Parse::string_to_formula(std::string source, const Math_Environment* environment) {
        // Format the text as needed
        source = remove_space(source);
        std::string last_text_2 = std::string();std::string last_text_3 = std::string();std::string last_text_4 = std::string();
        for(int i = 0;i<static_cast<int>(source.size());i++) {
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
                    }
                    else {source.insert(i, "*");i++;}
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
        std::shared_ptr<__Formula> formula = std::make_shared<__Formula>();

        // Cut the text operator by + operator
        cutted = cut_string_out_of_2(source, "+", "(", ")");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::shared_ptr<__Formula> current_polymonial = __string_to_formula_without_addition(cutted[i], environment);
            formula.get()->__add(current_polymonial.get());
        }

        // Return the result
        formula.get()->check_formula();
        return formula;
    };

    // Use parsers methods outside the class
    std::shared_ptr<__Formula> string_to_formula(std::string source){String_To_Formula_Parse parser;return parser.string_to_formula(source);};
    std::shared_ptr<__Formula> replace_unknown(__Formula* used_formula, std::string unknown, std::string new_value) {std::shared_ptr<__Formula>f=string_to_formula(new_value);return used_formula->replace_unknown(unknown, f.get());};

    // Math_Environment constructor
    Math_Environment::Math_Environment(){clear();};

    // Clears the environment
    void Math_Environment::clear(){
        // Handle unknowns
        if(a_unknowns.get() != 0){a_unknowns.get()->clear();}

        if(parser() != 0){
            // Handle functions
            parser()->add_function("random");
            parser()->add_function("repetition");}
    };

    // Handle repetitions
    // Adds a repetition
    void Math_Environment::add_repetition(){a_repetitions.push_back(0);};

    // Returns the last repetition
    int Math_Environment::last_repetition(){if(a_repetitions.size() <= 0){return 0;}return a_repetitions.at(a_repetitions.size() - 1);}

    // Removes the last repetition
    void Math_Environment::remove_repetition(){if(a_repetitions.size() > 0){a_repetitions.pop_back();}};

    // Repetes the last repetition
    void Math_Environment::repeat(){a_repetitions[a_repetitions.size() - 1]++;};

    // Returns a repetition
    int Math_Environment::repetition(int index) const {if(static_cast<int>(a_repetitions.size()) <= index){return 0;} return a_repetitions.at(index);};

    // Sets the last repetition
    void Math_Environment::set_repetition(int value){if(static_cast<int>(a_repetitions.size()) > 0){a_repetitions[a_repetitions.size() - 1] = value;}};

    // Returns a formula value
    std::shared_ptr<__Formula> Math_Environment::value_formula(std::string base)const{std::shared_ptr<__Formula> formula = parser()->string_to_formula(base, this);return formula.get()->replace_unknowns(a_unknowns.get());}
    // Returns a number value
    double Math_Environment::value_double(std::string base)const{return value_number(base).to_double();}
    scls::Fraction Math_Environment::value_number(std::string base)const{return value_formula(base).get()->value(a_unknowns.get()).real();}
    // Returns a Point_2D value
    scls::Point_2D_Formula Math_Environment::value_point_2d(std::string base)const{
        // Format the text
        while(base.size() > 0 && base.at(0) == '('){base = base.substr(1, base.size() - 1);}
        while(base.size() > 0 && base.at(base.size() - 1) == ')'){base = base.substr(0, base.size() - 1);}

        // Get the point
        base = scls::replace(base, std::string(";"), std::string(","));
        std::vector<std::string> cutted = scls::cut_string(base, std::string(","));
        if(cutted.size() != 2) {scls::print(std::string("PLEOS Text Environment"), std::string("Can't get a point 2D from \"") + base + std::string("\"."));return scls::Point_2D(0, 0);}
        return scls::Point_2D_Formula(value_number(cutted.at(0)), value_number(cutted.at(1)));
    };

    // Handle unknowns
    // Creates a unknown
    scls::__Formula_Base::Unknown* Math_Environment::create_unknown(std::string name){return a_unknowns.get()->create_unknown(name);};
    std::shared_ptr<scls::__Formula_Base::Unknown> Math_Environment::create_unknown_shared_ptr(std::string name){return a_unknowns.get()->create_unknown_shared_ptr(name);};
    // Returns a value by its name
    scls::Fraction Math_Environment::value_by_name(std::string name)const{scls::__Formula_Base::Unknown*unknow=unknown_by_name(name);if(unknow==0||unknow->value.get()==0){return 0;}return (reinterpret_cast<__Formula_Base_Template<Complex>*>(unknow->value.get())->value(0).real());};
    // Returns a unknown by its name
    scls::__Formula_Base::Unknown* Math_Environment::unknown_by_name(std::string name)const{return a_unknowns.get()->unknown_by_name(name);};
    std::shared_ptr<scls::__Formula_Base::Unknown> Math_Environment::unknown_shared_ptr_by_name(std::string name)const{return a_unknowns.get()->unknown_shared_ptr_by_name(name);};

    // Use parsers methods outside the class
    void __string_to_algebra_element_operator(__Algebra_Element* element, std::string source, std::vector<std::string>& operator_order, int position) {
        // Cut the text operator by * operator
        std::vector<std::string> cutted = cut_string_out_of_2(source, operator_order.at(position), "(", ")");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::shared_ptr<__Algebra_Element> current_element;
            if(position <= 0) {
                if(cutted.at(i).size() < 2 || (cutted.at(i).at(0) != '(')) {current_element = element->new_algebra_element(cutted.at(i));}
                else {current_element = element->new_algebra_element();__string_to_algebra_element_operator(current_element.get(), cutted.at(i).substr(1, cutted.at(i).size() - 2), operator_order, operator_order.size() - 1);}
            }
            else{current_element = element->new_algebra_element();__string_to_algebra_element_operator(current_element.get(), cutted.at(i), operator_order, position - 1);}
            element->operate(current_element.get(), operator_order.at(position));
        }
    }
    void __string_to_algebra_element(__Algebra_Element* element, std::string source) {
        // Operator order
        std::vector<std::string> operator_order = {std::string("^"), std::string("*"), std::string("+")};
        source = remove_space(source);

        // Cut the text operator by * operator
        __string_to_algebra_element_operator(element, source, operator_order, operator_order.size() - 1);
    }
}
