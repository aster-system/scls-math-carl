//******************
//
// scls_math_boolean.h
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
// This file contains some functions to handle boolean algebra in C++.
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

#ifndef SCLS_MATH_BOOLEAN
#define SCLS_MATH_BOOLEAN

#include "scls_math_polynomial.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

	//*********
	//
	// The "Boolean" class
	//
	//*********

    class Boolean: public __Algebra_Element {
    public:
    	// Container of unknowns
    	struct __Boolean_Unknown : public __Algebra_Element::__Algebra_Unknown {bool value = false;};

    	// Boolean constructor
    	Boolean(){};
    	Boolean(std::string unknown_name):Boolean(){new_unknown(unknown_name);};

    	// Adds an operator
    	void add_and(char other){if(other == 0){clear();a_value = false;}};
    	void add_and(Boolean* other){
    	    // Has a value
    	    if(other->is_final_element() && !other->is_unknown()){add_and(other->a_value);return;}

    	    // Not a value
			if(is_final_element()){if(is_unknown()){sub_place();}}
            else if(algebra_operator() != std::string(".")){sub_place();}
            set_algebra_operator(".");algebra_elements().push_back(other->clone());
		};
    	void add_and(__Algebra_Element* other){add_and(reinterpret_cast<Boolean*>(other));};
    	void add_and(Boolean other){add_and(&other);};
    	void add_and(std::string unknown_name){Boolean b = Boolean(unknown_name);add_and(b);};
    	void add_or(char other){if(other == 1){clear();a_value = true;}};
    	void add_or(__Algebra_Element* other){add_or(reinterpret_cast<Boolean*>(other));};
    	void add_or(Boolean* other){
    	    // Has a value
    	    if(other->is_final_element() && !other->is_unknown()){add_or(other->a_value);return;}

    	    // Not a value
			if(is_final_element()){if(is_unknown()){sub_place();}}
            else if(algebra_operator() != std::string("+")){sub_place();}
			set_algebra_operator("+");algebra_elements().push_back(other->clone());
		};
    	void add_or(Boolean other){add_or(&other);};
		void add_or(std::string unknown_name){Boolean b = Boolean(unknown_name);add_or(b);};

    	// Creates a new algebra element of the same type
		virtual std::shared_ptr<__Algebra_Element> algebra_clone() const {return clone();};
		virtual std::shared_ptr<Boolean> clone() const {std::shared_ptr<Boolean> b = std::make_shared<Boolean>();__clone_base(b.get());b.get()->a_value = a_value;return b;};
    	virtual std::shared_ptr<__Algebra_Element> new_algebra_element() const {std::shared_ptr<Boolean> s = std::make_shared<Boolean>();s.get()->a_parent=a_this_object;s.get()->a_this_object=s;return s;};
    	virtual std::shared_ptr<__Algebra_Element> new_algebra_element(std::string content) const {std::shared_ptr<Boolean> s = std::make_shared<Boolean>(content);s.get()->a_parent=a_this_object;s.get()->a_this_object=s;return s;};

    	// Creates the unknown
    	virtual __Algebra_Element::__Algebra_Unknown* create_unknown(){clear();a_unknown = std::make_shared<__Boolean_Unknown>();return a_unknown.get();};

    	// Operates this element with another one
        void operate(__Algebra_Element* other, std::string operation){if(operation == std::string("+")){add_or(other);}else if(operation == std::string(".")){add_and(other);}};

    	// Replaces the unknowns
    	std::shared_ptr<Boolean> replace_unknowns(Unknowns_Container* values) const;

    	// Get each unknowns
    	std::vector<std::string> unknowns();

    	// Returns the element to a simple std::string
    	virtual std::string to_mathml(Textual_Math_Settings* settings) const {if(is_unknown()){return algebra_unknown()->name;}return std::string();};
    	virtual std::string to_std_string(Textual_Math_Settings* settings) const {
    		// If the element is the final element
    		if(is_final_element()) {
    			if(is_unknown()){return algebra_unknown()->name;}
    			else{if(a_value){return std::string("1");}return std::string("0");}
    		}

    		// A lot of objects
    		std::string to_return = std::string();
    		for(int i = 0;i<static_cast<int>(algebra_elements_const().size());i++) {
    			std::string current = algebra_elements_const().at(i).get()->to_std_string(settings);
    			if(!algebra_elements_const().at(i).get()->is_final_element()){current = std::string("(") + current + std::string(")");}
    			to_return += current;
    			if(i != static_cast<int>(algebra_elements_const().size()) - 1){to_return += std::string(" ") + algebra_operator() + std::string(" ");}
    		}

    		return to_return;
    	};

        // Returns the verity table
        std::string verity_table();

        // Getters and setters
        inline bool value() const {return a_value;}
    private:
    	// Value of the element
    	bool a_value = false;
    };
    typedef Boolean::__Boolean_Unknown Boolean_Unknown;
}

#endif // SCLS_MATH_BOOLEAN
