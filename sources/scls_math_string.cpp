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

    // Returns if a std::string is an operator or not
    bool String_To_Formula_Parse::__string_is_operator(char text) const {return (text == '+' || text == '-' || text == '*' || text == '/' || text == '>' || text == '^');};

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
    std::shared_ptr<Formula_Base> Math_Environment::value_formula(std::string base)const{std::shared_ptr<Formula_Base> new_object = std::make_shared<Formula_Base>();string_to_algebra_element(0, new_object.get(), base, new_object.get()->operators().operators());return new_object.get()->replace_unknowns(a_unknowns.get());}
    // Returns a number value
    double Math_Environment::value_double(std::string base)const{return value_number(base).to_double();}
    Fraction Math_Environment::value_number(std::string base)const{return *value_formula(base).get()->replace_unknowns(a_unknowns.get()).get()->value<Fraction>();}
    // Returns a Point_2D value
    Point_2D_Formula Math_Environment::value_point_2d(std::string base)const{
        // Format the text
        while(base.size() > 0 && base.at(0) == '('){base = base.substr(1, base.size() - 1);}
        while(base.size() > 0 && base.at(base.size() - 1) == ')'){base = base.substr(0, base.size() - 1);}

        // Get the point
        base = replace(base, std::string(";"), std::string(","));
        std::vector<std::string> cutted = cut_string(base, std::string(","));
        if(cutted.size() != 2) {print(std::string("PLEOS Text Environment"), std::string("Can't get a point 2D from \"") + base + std::string("\"."));return Point_2D(0, 0);}
        return Point_2D_Formula(value_number(cutted.at(0)), value_number(cutted.at(1)));
    };

    // Handle unknowns
    // Creates a unknown
    Formula_Base::Formula_Unknown* Math_Environment::create_unknown(std::string name){return a_unknowns.get()->create_unknown(name);};
    std::shared_ptr<Formula_Base::Formula_Unknown> Math_Environment::create_unknown_shared_ptr(std::string name){return a_unknowns.get()->create_unknown_shared_ptr(name);};
    // Sets the value of an unknown by its name
    void Math_Environment::set_unknown_value_by_name(std::string name, Fraction new_value){set_unknown_value_by_name(name, Formula_Base::new_formula(new_value));}
    void Math_Environment::set_unknown_value_by_name(std::string name, std::shared_ptr<Formula_Base> new_value){reinterpret_cast<Formula_Base::Formula_Unknown*>(create_unknown(name))->value = new_value;}
    // Returns a value by its name
    Fraction Math_Environment::value_by_name(std::string name)const{Formula_Base::Formula_Unknown*unknown=unknown_by_name(name);if(unknown==0||unknown->value.get()==0){return 0;}return (*unknown->value.get()->value<scls::Fraction>());};
    // Returns a unknown by its name
    Formula_Base::Formula_Unknown* Math_Environment::unknown_by_name(std::string name)const{return a_unknowns.get()->unknown_by_name(name);};
    std::shared_ptr<Formula_Base::Formula_Unknown> Math_Environment::unknown_shared_ptr_by_name(std::string name)const{return a_unknowns.get()->unknown_shared_ptr_by_name(name);};

    // Use parsers methods outside the class
    bool string_is_operator(const std::vector<Algebra_Element::Algebra_Operator>& operator_order, std::string to_test) {
		if(to_test == std::string_view(">")){return true;}
    	for(std::size_t i = 0;i<operator_order.size();i++) {
			if(operator_order.at(i).name() == to_test){return true;}
		}
		return false;
	}
    bool string_is_operator(const std::vector<Algebra_Element::Algebra_Operator>& operator_order, char to_test) {
    	std::string t = std::string();t += to_test;
    	return string_is_operator(operator_order, t);
    }
    bool string_is_special(const std::vector<Algebra_Element::Algebra_Operator>& operator_order, std::string to_test){
    	if(to_test == std::string_view("-") || to_test == std::string_view("(")  || to_test == std::string_view(")")){return true;}
    	else if(string_is_operator(operator_order, to_test)){return true;}
		return false;
	}
    bool string_is_special(const std::vector<Algebra_Element::Algebra_Operator>& operator_order, char to_test){
    	std::string t = std::string();t += to_test;
    	return string_is_special(operator_order, t);
    }
    void __string_to_algebra_element_operator(Algebra_Element* element, std::string source, const std::vector<Algebra_Element::Algebra_Operator>& operator_order, int position) {
        // Cut the text operator by * operator
    	std::vector<std::string> cutted = cut_string_out_of_2(source, operator_order.at(position).name(), "(", ")");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::shared_ptr<Algebra_Element> current_element;
            if(position <= 0) {
                std::vector<std::string> parts = cut_string_out_of_2(cutted.at(i), std::string(">"), "(", ")");
                if(parts.at(parts.size() - 1).size() < 2 || (parts.at(parts.size() - 1).at(0) != '(')) {current_element = element->new_algebra_element(parts.at(parts.size() - 1));}
                else {current_element = element->new_algebra_element();__string_to_algebra_element_operator(current_element.get(), parts.at(parts.size() - 1).substr(1, parts.at(parts.size() - 1).size() - 2), operator_order, operator_order.size() - 1);}

                if(parts.size() == 2) {
                    std::string needed_function = parts.at(0);
                    if(needed_function == std::string_view("random")) {current_element = element->new_algebra_element(random_fraction(0, 1).to_std_string(0));}
                    else if(needed_function == std::string_view("repetition")) {

                    }
                    else {current_element.get()->operate(0, parts.at(0));}
                }
            }
            else{current_element = element->new_algebra_element();__string_to_algebra_element_operator(current_element.get(), cutted.at(i), operator_order, position - 1);}
            element->operate(current_element.get(), operator_order.at(position).name());
        }
    }
    void Math_Environment::string_to_algebra_element(Math_Environment* env, Algebra_Element* element, std::string source, const std::vector<Algebra_Element::Algebra_Operator>& operator_order) {
        // Operator order
    	std::vector<std::string> functions = {"ln", "exp", "sqrt", "cos", "sin", "tan", "arcsin", "arccos", "arctan", "random", "repetition"};
        source = remove_space(source);

        // First / last elements
        if(source.size() > 0 && source.at(0) == '-'){source.insert(1, std::string("1*"));}

        // Handle the parenthesis
		for(int i = 0;i<static_cast<int>(source.size());i++) {
			// Remove the useless ")("
			if(source[i] == '(') {
				char last_char = 0;
				if(i > 0){last_char = source[i - 1];}

				if(last_char == ')') {source.insert(i, "*");i++;}
				else if(last_char == '-') {source.insert(i, "1*");i++;}
				else if(i > 0 && !string_is_special(operator_order, last_char)) {
					std::string total_function = std::string();
					int current_pos = i - 1;
					while(current_pos >= 0 && (!string_is_operator(operator_order, source[current_pos]) && source[current_pos]!='(' && source[current_pos]!=')')){total_function=source[current_pos]+total_function;current_pos--;}
					std::size_t index = 0;char l = -1;
					for(;index<functions.size();index++) {if(functions.at(index) == total_function) {l=0;break;}}

					if(l != -1) {
						// The part is a function
						source.insert(i, ">");
						if(i > static_cast<int>(total_function.size()) + 1 && source.at(i - (total_function.size() + 1)) == '-'){source.insert(i - total_function.size(), std::string("1*"));}
						i++;
					}
					else {source.insert(i, "*");i++;}
				}
			}
		}

		// Add the necessary "*"
		std::string operator_to_add = std::string("*");
		for(int i = 1;i<static_cast<int>(source.size());i++) {
			// Remove the implicit operator
			char last_char = source[i - 1];
			if((string_is_number(last_char) || last_char == ')') && (!string_is_number(source[i])) && !string_is_special(operator_order, source[i])) {
				source.insert(i, "*");i++;
			}
		}

		// Handle the "-"
		for(int i = 0;i<static_cast<int>(source.size()) - 1;i++) {
			// Remove the useless "-"
			if(static_cast<int>(source[i]) == static_cast<int>('-') && i > 0) {
				if(!string_is_operator(operator_order, source[i - 1]) && (i >= static_cast<int>(source.size()) || (static_cast<int>(source[i + 1]) != static_cast<int>('(')))) {
                    source.insert(i, "+");
                    i++;
                }

                // Add some other explicit "-"
                if(!string_is_number(source[i + 1])){source.insert(i + 1, "1*");i++;}
			}
		}

        // Cut the text operator by * operator
        __string_to_algebra_element_operator(element, source, operator_order, operator_order.size() - 1);
    }
}
