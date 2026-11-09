//******************
//
// scls_math_algebra_element.cpp
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
// This file contains the source code of scls_math_algebra_element.h.
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
#include "../../scls_math_directory/scls_math_algebra/scls_math_algebra_element.h"

//*********
//
// Useful datas for textual representation of numbers
//
//*********

namespace scls {

    //*********
    //
    // The Textual_Math_Settings class
    //
    //*********

    // Textual_Math_Settings constructor
    Textual_Math_Settings::Textual_Math_Settings(){}

    // Getters and setters
    void Textual_Math_Settings::set_spaces_in_color(bool new_spaces_in_color){a_spaces_in_color = new_spaces_in_color;}
    bool Textual_Math_Settings::spaces_in_color(){return a_spaces_in_color;}

    // XML Settings
    Textual_Math_Settings __textual_math_xml_settings;
    bool __textual_math_xml_settings_loaded = false;
    Textual_Math_Settings* textual_math_xml_settings(){if(!__textual_math_xml_settings_loaded){__load_textual_math_xml_settings();}return &__textual_math_xml_settings;}
    void __load_textual_math_xml_settings(){__textual_math_xml_settings.set_spaces_in_color(false);}
}

//*********
//
// Useful datas for algebra handling
//
//*********

namespace scls {

	//*********
    //
    // The Algebra_Element class
    //
    //*********

    // Algebra_Operators constructor
    Algebra_Element::Algebra_Operators::Algebra_Operators(std::vector<Algebra_Element::Algebra_Operator> needed_operators, std::vector<Algebra_Element::Algebra_Operator> needed_functions):a_functions(needed_functions),a_operators(needed_operators){};

    // Algebra_Element constructor
	Algebra_Element::Algebra_Element(){}
	// Algebra_Element destructor
	Algebra_Element::~Algebra_Element(){}

	// Clears the object
	void Algebra_Element::clear(){a_elements.clear();a_operator = std::string();a_unknown.reset();}

	// Return if the element is a final element
	bool Algebra_Element::is_final_element()const{return static_cast<int>(a_elements.size()) == 0;};
	bool Algebra_Element::is_known() const{return is_final_element() && a_unknown.get() == 0;};
	bool Algebra_Element::is_unknown()const{return is_final_element() && a_unknown.get() != 0;};

	// Sub-places the element
	void Algebra_Element::sub_place() {
		std::shared_ptr<Algebra_Element> e = algebra_clone();
		clear();
		a_elements.push_back(e);
	}

	// Virtual functions

	// Creates a new algebra element of the same type
	void Algebra_Element::clone_base(Algebra_Element* e)const{
		// Base
		clone_base_without_sub_elements(e);

		// Copy the element
		for(std::size_t i = 0;i<a_elements.size();i++) {
			e->a_elements[i] = a_elements.at(i).get()->algebra_clone();
		}
	}
	void Algebra_Element::clone_base_without_sub_elements(Algebra_Element* e) const {
	    e->a_operator = a_operator;
		e->a_unknown = a_unknown;
		e->a_elements = std::vector<std::shared_ptr<Algebra_Element>>(a_elements.size());
	}

	// Creates the unknown
	Algebra_Element::__Algebra_Unknown* Algebra_Element::create_unknown(){clear();a_unknown = std::make_shared<Algebra_Element::__Algebra_Unknown>();return a_unknown.get();};
	Algebra_Element::__Algebra_Unknown* Algebra_Element::new_unknown(std::string unknown_name){create_unknown();a_unknown.get()->name = unknown_name;return a_unknown.get();};

	// Returns the first known algebra element of this element
	Algebra_Element* Algebra_Element::known_algebra_element() {
		for(std::size_t i = 0;i<a_elements.size();i++) {
			if(a_elements.at(i).get()->is_final_element() && a_elements.at(i).get()->a_unknown.get() == 0) {
				return a_elements.at(i).get();
			}
		}
		return 0;
	}

	// Evaluate a relation of this element with another one
	bool Algebra_Element::in_relation(Algebra_Element* other, std::string relation) {return evaluate_relation(other, relation) == Algebra_Element::Relation_State::RS_In_Relation;}

	// Returns if two elements are equal or not
	bool Algebra_Element::is_equal_with_value(Algebra_Element* other) const {
        if(is_final_element() && other->is_final_element()) {return is_equal(other);}
		else if(a_elements.size() == other->a_elements.size()) {
			if(a_operator.name() == other->a_operator.name()) {
				std::vector<std::shared_ptr<Algebra_Element>> sub_elements = a_elements;
                std::vector<std::shared_ptr<Algebra_Element>> sub_elements_other = other->a_elements;
                for(int i = 0;i<static_cast<int>(sub_elements.size());i++) {
                    for(int j = 0;j<static_cast<int>(sub_elements_other.size());j++) {
                        if(sub_elements.at(i).get()->is_equal_with_value(sub_elements_other.at(j).get())) {
                            sub_elements.erase(sub_elements.begin() + i);
                            sub_elements_other.erase(sub_elements_other.begin() + j);
                            i = -1;break;
                        }
                    }
                }

                return sub_elements.size() == 0 && sub_elements_other.size() == 0;
			}
		}
		return false;
    }
	bool Algebra_Element::is_equal_without_value(Algebra_Element* other, std::string used_operator) const {
	    // Operator
	    if(!is_final_element() && algebra_operator_name() != used_operator){return false;}

		if(is_final_element() && other->is_final_element()) {
			if(a_unknown.get() == 0 && other->a_unknown.get() == 0){return true;}
			else if(a_unknown.get() != 0 && other->a_unknown.get() != 0) {
				return a_unknown.get()->name == other->a_unknown.get()->name;
			}
		}
		else if(!is_final_element() && !other->is_final_element()) {
            std::vector<std::shared_ptr<Algebra_Element>> sub_elements = a_elements;
            std::vector<std::shared_ptr<Algebra_Element>> sub_elements_other = other->a_elements;
            for(int i = 0;i<static_cast<int>(sub_elements.size());i++) {
                for(int j = 0;j<static_cast<int>(sub_elements_other.size());j++) {
                    if(sub_elements.at(i).get()->is_equal_with_value(sub_elements_other.at(j).get())) {
                        sub_elements.erase(sub_elements.begin() + i);
                        sub_elements_other.erase(sub_elements_other.begin() + j);
                        i = -1;break;
                    }
                }
            }

            // Possible remaining element
            if(sub_elements.size() == 1 && sub_elements.at(0).get()->is_known()){sub_elements.clear();}
            if(sub_elements_other.size() == 1 && sub_elements_other.at(0).get()->is_known()){sub_elements_other.clear();}

            return sub_elements.size() == 0 && sub_elements_other.size() == 0;
		}
		else {
			if(a_operator.name() == other->a_operator.name() && a_elements.size() == other->a_elements.size()) {
				for(std::size_t i = 0;i<a_elements.size();i++) {
					bool good = false;
					for(std::size_t j = 0;j<other->a_elements.size();j++) {
						if(a_elements.at(i).get()->is_equal_without_value(other->a_elements.at((i + j) % other->a_elements.size()).get(), used_operator)){good = true;break;}
					}
					if(!good){return false;}
				}
				return true;
			}
			else if(other->is_final_element() && other->is_unknown() && a_elements.size() == 2) {
                if(a_elements.at(0).get()->is_known()){return other->is_equal_without_value(a_elements.at(1).get(), used_operator);}
				else if(a_elements.at(1).get()->is_known()){return other->is_equal_without_value(a_elements.at(0).get(), used_operator);}
			}
			else if(is_final_element() && is_unknown() && other->a_elements.size() == 2) {
				if(other->a_elements.at(0).get()->is_known()){return is_equal_without_value(other->a_elements.at(1).get(), used_operator);}
				else if(other->a_elements.at(1).get()->is_known()){return is_equal_without_value(other->a_elements.at(0).get(), used_operator);}
			}
		}
		return false;
	}

	// Available operators for this object
	Algebra_Element::Algebra_Operators base_operators;
	const Algebra_Element::Algebra_Operators& Algebra_Element::operators() const {return base_operators;}
	const Algebra_Element::Algebra_Operator* Algebra_Element::function_by_name(std::string operator_name){
        const std::vector<Algebra_Element::Algebra_Operator>& needed_functions = operators().functions();
        for(std::size_t i = 0;i<needed_functions.size();i++) {
            if(needed_functions.at(i).name() == operator_name){return &needed_functions.at(i);}
        }
        return 0;
    };
    const Algebra_Element::Algebra_Operator* Algebra_Element::operator_by_name(std::string operator_name){
        const std::vector<Algebra_Element::Algebra_Operator>& needed_operators = operators().operators();
        for(std::size_t i = 0;i<needed_operators.size();i++) {
            if(needed_operators.at(i).name() == operator_name){return &needed_operators.at(i);}
        }
        return 0;
    };

    // Returns a list of all the unknowns
    std::vector<std::string> Algebra_Element::all_unknowns_name() const {
        std::vector<std::string> to_return = std::vector<std::string>();
        if(is_unknown()){to_return.push_back(algebra_unknown_name());}
        else if(!is_known()){
            for(std::size_t i = 0;i<a_elements.size();i++) {
                std::vector<std::string> current = a_elements.at(i).get()->all_unknowns_name();
                for(std::size_t j = 0;j<current.size();j++) {
                    bool good = true;
                    for(std::size_t k = 0;k<to_return.size();k++) {
                        if(current.at(j) == to_return.at(k)){
                            good = false;
                            break;
                        }
                    }
                    if(good){to_return.push_back(current.at(j));}
                }
            }
        }

        return to_return;
    }

	// Replaces the unknowns
	void Algebra_Element::replace_unknowns_algebra(Algebra_Element* element, Unknowns_Container* values) const {
		// The element is final
		if(!is_final_element()) {
			std::shared_ptr<Algebra_Element> c = algebra_elements_const().at(0).get()->algebra_clone();
			c.get()->replace_unknowns_algebra(element, values);

			if(algebra_operator_arity() == 1) {element->operate(0, algebra_operator_name());}
			else {
                for(int i = 1;i<static_cast<int>(algebra_elements_const().size());i++) {
                    std::shared_ptr<Algebra_Element> b_replaced = algebra_elements_const().at(i).get()->algebra_clone();
                    c = new_algebra_element();b_replaced.get()->replace_unknowns_algebra(c.get(), values);
                    element->operate(c.get(), algebra_operator_name());
                    element->simplify();
                }
			}
		}
	}

	// Simplify the element
    void Algebra_Element::simplify(){};

}
