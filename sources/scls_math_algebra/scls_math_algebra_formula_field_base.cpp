//******************
//
// scls_math_formula_template.cpp
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
// This file contains the source code of scls_math_formula_template.h.
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
#include "../../scls_math_directory/scls_math_algebra/scls_math_algebra_formula_field_base.h"

namespace scls {
    //*********
	//
	// The "Proposition" class
	//
	//*********

	//*********
	//
	// The "Formula_Base_Field" class
	//
	//*********

    // Adds an element to this one
    void Formula_Base_Field::add(Formula_Base_Field* formula){
        if(formula->is_final_element() && formula->is_known() && (formula->a_value.get() == 0 || formula->a_value.get()->is_addition_neutral())){fix_emptiness();return;}
        else if(is_final_element() && !is_unknown() && (a_value.get() == 0 || a_value.get()->is_addition_neutral())){formula->clone(this);}
        else if(is_final_element() && formula->is_final_element() && !is_unknown() && !formula->is_unknown()){
            a_value->operate(formula->a_value.get(), "+");
        }
        else {
            if(algebra_operator_name() != std::string("+")){sub_place();set_algebra_operator(std::string("+"));}
            algebra_elements().push_back(formula->algebra_clone());
        }
    }

    // Divides  an element to this one
    void Formula_Base_Field::divide(Formula_Base_Field* formula) {
    	if(is_final_element() && formula->is_final_element() && !is_unknown() && !formula->is_unknown()){a_value->operate(formula->a_value.get(), "/");}
    	else {
    		sub_place();
			set_algebra_operator(std::string("/"));
			algebra_elements().push_back(formula->algebra_clone());
		}
    }

    // Returns if the formula is empty
    bool Formula_Base_Field::empty() const{return a_value.get() == 0 && algebra_elements_const().size() == 0;}
    void Formula_Base_Field::fix_emptiness(){if(empty()){a_value = multiplication_absorbing_field_element();}}

    // Evaluate a relation of this element with another one
    Algebra_Element::Relation_State Formula_Base_Field::evaluate_relation(Algebra_Element* other, std::string relation) {
        if(relation == std::string_view("=")){return is_equal(other) ? Relation_State::RS_In_Relation : Relation_State::RS_Not_In_Relation;}

    	return Relation_State::RS_Not_In_Relation;
    }

    // Returns if two elements are equal or not
    bool Formula_Base_Field::is_equal(Algebra_Element* other) const {
    	if(is_final_element() && other->is_final_element()) {
			if(algebra_unknown() == 0 && other->algebra_unknown() == 0){return algebra_value()->is_equal(reinterpret_cast<Formula_Base_Field*>(other)->algebra_value());}
			else if(algebra_unknown() != 0 && other->algebra_unknown() != 0) {return algebra_unknown()->name == other->algebra_unknown()->name;}

			return false;
		}

    	return is_equal_with_value(other);
    }

    // Datas about the formula
    bool Formula_Base_Field::is_multiplication_absorbing() const{if(is_known()){if(a_value.get() == 0){return true;}else{return a_value.get()->is_multiplication_absorbing();}}return false;};
    bool Formula_Base_Field::is_multiplication_neutral() const{if(is_known() && a_value.get() != 0){return a_value.get()->is_multiplication_neutral();}return false;};

    // Multiplies an element to this one
    void Formula_Base_Field::multiply(Formula_Base_Field* formula){
        if(is_multiplication_absorbing()){return;}
    	else if(formula->is_multiplication_absorbing()){std::shared_ptr<Algebra_Element>v=multiplication_absorbing_field_element();clear();a_value=v;return;}
    	else if(formula->is_multiplication_neutral()){return;}else if(is_multiplication_neutral()){formula->clone(this);return;}
        else if(is_final_element() && formula->is_final_element() && is_known() && formula->is_known()){
            a_value->operate(formula->a_value.get(), "*");a_value.get()->simplify();
        }
        else {
            if(algebra_operator_name() != std::string_view("*")){
                sub_place();
                set_algebra_operator(std::string("*"));
                algebra_elements().push_back(formula->algebra_clone());
            }
            else {
                if(formula->is_known()) {
                    bool good = false;
                    for(std::size_t i = 0;i<algebra_elements().size();i++) {
                        if(algebra_elements().at(i).get()->is_known()) {
                            algebra_elements().at(i).get()->operate(formula, std::string("*"));
                            good = true;break;
                        }
                    }
                    if(!good){algebra_elements().push_back(formula->algebra_clone());}
                }
                else {
                    algebra_elements().push_back(formula->algebra_clone());
                }
            }
        }
    }

    // Operates this element with another one
    void Formula_Base_Field::operate(Algebra_Element* other, std::string operation) {
        // Datas
        const Algebra_Element::Algebra_Operator* current_operator = function_by_name(operation);
        if(current_operator != 0 && current_operator->arity() == 1) {
            if(is_final_element() && !is_unknown()){
                a_value->operate(0, operation);
            }
            else {sub_place();set_algebra_operator(operation, 1);}
        }
        else {
            // Normal operator
            if(!a_modified){a_modified = true;reinterpret_cast<Formula_Base_Field*>(other)->algebra_clone(this);}
            else if(operation == std::string("+")){add(reinterpret_cast<Formula_Base_Field*>(other));}
            else if(operation == std::string("*")){multiply(reinterpret_cast<Formula_Base_Field*>(other));}
            else if(operation == std::string("/")){divide(reinterpret_cast<Formula_Base_Field*>(other));}
            else if(is_final_element() && other->is_final_element() && !is_unknown() && !other->is_unknown()){
                a_value->operate(reinterpret_cast<Formula_Base_Field*>(other)->a_value.get(), operation);
            }
            else {
                sub_place();set_algebra_operator(operation);
                algebra_elements().push_back(other->algebra_clone());
            }
        }

        // Simplify after an operation
        simplify();
    }

    // Set the value of the formula
    void Formula_Base_Field::set_value_as_formula(std::shared_ptr<Algebra_Element> new_value){clear();a_value = new_value;}

    // Simplify the expression
    char Formula_Base_Field::simplify_step() {
        // Basic
        if(algebra_operator_arity() != 1 && algebra_elements().size() == 1){std::shared_ptr<Algebra_Element>temp=algebra_elements().at(0);reinterpret_cast<Formula_Base_Field*>(temp.get())->clone(this);return SIMPLIFICATION_UNTERMINATED;}

        // Children simplification
        for(std::size_t i = 0;i<algebra_elements().size();i++) {
            Formula_Base_Field* current_formula = reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(i).get());
            if(current_formula->is_final_element() && current_formula->is_known() && current_formula->a_value.get() == 0) {algebra_elements().erase(algebra_elements().begin() + i);i--;}
        	else if(current_formula->simplify_step() != NO_SIMPLIFICATION){return SIMPLIFICATION_UNTERMINATED;}
		}

        // Datas
        bool commutative_operator = (algebra_operator_name() == std::string_view("+") || algebra_operator_name() == std::string_view("*"));
    	bool simplified = false;

        if(algebra_operator_name() == std::string_view("+")) {
            // Possible merging
        	for(std::size_t i = 0;i<algebra_elements().size();i++) {
			    if(algebra_elements().at(i).get()->algebra_operator_name() == std::string_view("+")) {
                    std::shared_ptr<Algebra_Element> temp_element = algebra_elements().at(i);
                    algebra_elements().erase(algebra_elements().begin() + i);
			    	for(std::size_t j = 0;j<temp_element.get()->algebra_elements().size();j++) {
						add(reinterpret_cast<Formula_Base_Field*>(temp_element.get()->algebra_elements().at(j).get()));
					}
			    	return SIMPLIFICATION_UNTERMINATED;
				}
			}

        	for(std::size_t i = 0;i<algebra_elements().size();i++) {
                // Possible combinations
                for(std::size_t j = 0;j<i;j++) {
                    if(algebra_elements().at(j).get()->is_equal_without_value(algebra_elements().at(i).get(), "*")) {
                        std::cout << "R " << algebra_elements().at(i).get()->to_std_string(0) << std::endl << "PPPPPPPP" << std::endl << algebra_elements().at(j).get()->to_std_string(0) << std::endl;
                        if(algebra_elements().at(i).get()->is_known() && algebra_elements().at(j).get()->is_known()) {
                        	// Known case
                            reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get())->a_value.get()->operate(reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(i).get())->a_value.get(), "+");
                            algebra_elements().erase(algebra_elements().begin() + i);i--;
                            simplified = true;break;
                        }
                        else {
                        	// Operation
                            Formula_Base_Field* first_object = reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get()->known_algebra_element());
                            Formula_Base_Field* second_object = reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(i).get()->known_algebra_element());

                            if(true) {
                                bool should_delete = false;
                                if(first_object != 0 && second_object != 0) {first_object->a_value.get()->operate(second_object->a_value.get(), "+");if(first_object->a_value.get()->is_multiplication_absorbing()){should_delete = true;}}
								else if(second_object != 0){std::shared_ptr<Algebra_Element> one = new_algebra_element("1");second_object->operate(one.get(), "+");algebra_elements()[j] = algebra_elements()[i].get()->algebra_clone();}
								else if(first_object != 0) {std::shared_ptr<Algebra_Element> one = new_algebra_element("1");first_object->operate(one.get(), "+");}
								else{std::shared_ptr<Algebra_Element>f=new_algebra_element("2");reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get())->multiply(reinterpret_cast<Formula_Base_Field*>(f.get()));}
								algebra_elements().erase(algebra_elements().begin() + i);--i;simplified = true;
								if(should_delete){algebra_elements().erase(algebra_elements().begin() + j);--j;}break;
                            }
                        }
                    }
                }
            }
        }
        else if(algebra_operator_name() == std::string_view("*")) {
			// Possible merging
        	for(std::size_t i = 0;i<algebra_elements().size();i++) {
                if(algebra_elements().at(i).get()->is_final_element() && algebra_elements().at(i).get()->is_known() && algebra_elements().at(i).get()->is_multiplication_neutral()){
                    algebra_elements().erase(algebra_elements().begin() + i);
                    return SIMPLIFICATION_UNTERMINATED;
                }
				else if(algebra_elements().at(i).get()->algebra_operator_name() == std::string_view("*")) {
					std::shared_ptr<Algebra_Element> temp_element = algebra_elements().at(i);
                    algebra_elements().erase(algebra_elements().begin() + i);
					for(std::size_t j = 0;j<temp_element.get()->algebra_elements().size();j++) {
						multiply(reinterpret_cast<Formula_Base_Field*>(temp_element.get()->algebra_elements().at(j).get()));
					}
					return SIMPLIFICATION_UNTERMINATED;
				}
			}

			// Distributivity simple (only one element)
			for(int i = 0;i<static_cast<int>(algebra_elements().size());i++) {
				// Check if the distributivity is possible
				if(!algebra_elements().at(i).get()->is_final_element()){continue;}

				// Possible combinations
				for(int j = 0;j<static_cast<int>(algebra_elements().size());j++) {
					if(i != j && reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get())->algebra_operator_name() == std::string_view("+")) {
						for(std::size_t k = 0;k<reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get())->algebra_elements().size();k++) {
							reinterpret_cast<Formula_Base_Field*>(reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get())->algebra_elements().at(k).get())->multiply(reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(i).get()));
						}
						algebra_elements().erase(algebra_elements().begin() + i);
						i = -1;simplified = true;
						break;
					}
				}
			}//*/

			// Distributivity complex (a lot of elements)
			for(int i = 0;i<static_cast<int>(algebra_elements().size());i++) {
				// Check if the distributivity is possible
				if(algebra_elements().at(i).get()->algebra_operator_name() != std::string_view("+")){continue;}

				// Possible combinations
				for(int j = 0;j<static_cast<int>(algebra_elements().size());j++) {
					if(i != j && reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get())->algebra_operator_name() == std::string_view("+")) {
						std::shared_ptr<Algebra_Element> t = reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(i).get())->algebra_clone();
						reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(i).get())->multiply(reinterpret_cast<Formula_Base_Field*>(reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get())->algebra_elements().at(0).get()));
                        while(reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(i).get())->simplify_step() != NO_SIMPLIFICATION){}
						for(std::size_t k = 1;k<reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get())->algebra_elements().size();k++) {
							std::shared_ptr<Algebra_Element> temp = t.get()->algebra_clone();
							reinterpret_cast<Formula_Base_Field*>(temp.get())->multiply(reinterpret_cast<Formula_Base_Field*>(reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(j).get())->algebra_elements().at(k).get()));
							while(reinterpret_cast<Formula_Base_Field*>(temp.get())->simplify_step() != NO_SIMPLIFICATION){}
							reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(i).get())->add(reinterpret_cast<Formula_Base_Field*>(temp.get()));
						}
						algebra_elements().erase(algebra_elements().begin() + j);
						i = -1;simplified = true;
						break;
					}
				}
			}

			// Late modification
			if(algebra_elements().size() == 1){std::shared_ptr<Algebra_Element> temp = algebra_elements().at(0);clear();reinterpret_cast<Formula_Base_Field*>(temp.get())->clone(this);return SIMPLIFICATION_UNTERMINATED;}
        }

        // Sort known / unknown
        if(commutative_operator) {
            for(std::size_t i = 1;i<algebra_elements().size();i++) {
                Formula_Base_Field* current_formula = reinterpret_cast<Formula_Base_Field*>(algebra_elements().at(i).get());
                if(current_formula->is_known()){std::shared_ptr<Algebra_Element> e = algebra_elements().at(0);algebra_elements()[0] = algebra_elements().at(i);algebra_elements()[i] = e;}
            }
        }

        if(simplified){return SIMPLIFICATION_TERMINATED;}
        return NO_SIMPLIFICATION;
    }

    // Returns the formula as a C++ function
    std::string Formula_Base_Field::to_cpp() const {
    	std::string to_return = std::string();

    	// Add what is needed to add
    	if(is_final_element()) {
    		if(is_unknown()){to_return = algebra_unknown()->name;}
    		else{to_return = algebra_value()->to_cpp();}
    	}
    	else {
    		if(algebra_operator_arity() == 1) {
    			to_return = algebra_operator_name() + std::string("(") + algebra_elements_const().at(0).get()->to_cpp() + std::string(")");
    		}
    		else {
                to_return += std::string("(");
    			for(std::size_t i = 0;i<algebra_elements_const().size() - 1;i++) {
    				to_return += algebra_elements_const().at(i).get()->to_cpp();
    				to_return += std::string(" ") + algebra_operator_name() + std::string(" ");
    			}
    			to_return += algebra_elements_const().at(algebra_elements_const().size() - 1).get()->to_cpp() + std::string(")");
    		}
    	}

    	return to_return;
    }
    std::string Formula_Base_Field::to_cpp_function(const std::string& function_name) {
    	std::string to_return = cpp_type_with_library() + std::string(" ") + function_name + std::string("(");
    	std::vector<std::string> unknowns = all_unknowns_name();
    	for(std::size_t i = 0;i<unknowns.size();i++){
    		to_return += cpp_type_with_library() + std::string(" ") + unknowns.at(i);
    		if(i != unknowns.size() - 1){to_return += std::string(", ");}
    	}
    	to_return += std::string("){");

    	to_return += std::string("return ") + to_cpp() + std::string(";");

    	// Finish the result
    	to_return += std::string("}");
		return to_return;
    }

    // Returns the formula as a GLSL function
    std::string Formula_Base_Field::to_glsl() const {
    	std::string to_return = std::string();

		// Add what is needed to add
		if(is_final_element()) {
			if(is_unknown()){to_return = algebra_unknown()->name;}
			else{to_return = algebra_value()->to_glsl();}
		}
		else {
			if(algebra_operator_arity() == 1) {
				to_return = algebra_operator_name() + std::string("(") + algebra_elements_const().at(0).get()->to_glsl() + std::string(")");
			}
			else {
				to_return += std::string("(");
				for(std::size_t i = 0;i<algebra_elements_const().size() - 1;i++) {
					to_return += algebra_elements_const().at(i).get()->to_glsl();
					to_return += std::string(" ") + algebra_operator_name() + std::string(" ");
				}
				to_return += algebra_elements_const().at(algebra_elements_const().size() - 1).get()->to_glsl() + std::string(")");
			}
		}

		return to_return;
    }

    // Returns the element to a simple std::string
	std::string Formula_Base_Field::to_mathml(scls::Textual_Math_Settings* settings) const{return to_std_string(settings);};
	std::string Formula_Base_Field::to_std_string(scls::Textual_Math_Settings* settings) const{
		std::string to_return = std::string();
		if(is_final_element()){
			if(!is_unknown()){if(a_value.get() == 0){return std::string();}return a_value.get()->to_std_string(settings);}
			else{to_return = algebra_unknown()->name;}
		}
		else {
			if(algebra_operator_arity() == 1) {
				to_return += algebra_operator_name() + std::string("(") + algebra_elements_const().at(0).get()->to_std_string(settings) + std::string(")");
			}
			else {
				for(int i = 0;i<static_cast<int>(algebra_elements_const().size());i++) {
					to_return += std::string("(");
					to_return += algebra_elements_const().at(i).get()->to_std_string(settings);
					to_return += std::string(")");
					if(i < static_cast<int>(algebra_elements_const().size()) - 1){to_return += std::string(" ") + algebra_operator_name() + std::string(" ");}
				}
			}
		}
		return to_return;
	};
}
