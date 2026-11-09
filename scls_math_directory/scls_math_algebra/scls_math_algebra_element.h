//******************
//
// scls_math_algebra_element.h
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
// This file contains the class used as the base of all the algebraic par of SCLS.
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

#ifndef SCLS_MATH_ALGEBRA_ELEMENT
#define SCLS_MATH_ALGEBRA_ELEMENT

// Include SCLS Math bigger header
#include "../../scls_math_root.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
    //
    // The Textual_Math_Settings class
    //
    //*********

    class Textual_Math_Settings {
        // Class representing settings to textually represent mathematical objects
    public:

        // Textual_Math_Settings constructor
        Textual_Math_Settings();

        // Getters and setters
        inline int complex_double()const{return a_complex_double;};
        inline bool hide_if_0() const {return a_hide_if_0;};
        inline bool introduction_in_mathml() const {return a_introduction_in_mathml;};
        void set_complex_double(int new_complex_double){a_complex_double = new_complex_double;};
        void set_hide_if_0(bool new_hide_if_0){a_hide_if_0 = new_hide_if_0;};
        void set_spaces_in_color(bool new_spaces_in_color);
        bool spaces_in_color();
    private:
        // Write the complex with doubles or fraction (-1)
        int a_complex_double = -1;

        // Hides the number if equal to "0"
        bool a_hide_if_0 = true;

        // If the introduction should be in mathml or not
        bool a_introduction_in_mathml = true;

        // Spaces (or not) in colors
        bool a_spaces_in_color = true;
    };

    // XML Settings
    Textual_Math_Settings* textual_math_xml_settings();
    void __load_textual_math_xml_settings();
}

//*********
//
// Useful algebrical tools
//
//*********

namespace scls {

    //*********
    //
    // The Algebra_Element class
    //
    //*********

    class Algebra_Element {
        // Class representing an element in a field
    public:

    	class Algebra_Operator {
    	public:
			// Algebra_Operator constructor
    		Algebra_Operator(){};
    		Algebra_Operator(std::string name):a_name(name){};
    		Algebra_Operator(std::string name, int arity):a_arity(arity),a_name(name){};
    		// Algebra_Operator destructor
    		~Algebra_Operator(){};

    		// Getters and setters
    		inline int arity() const {return a_arity;}
    		inline std::string name() const {return a_name;};
    	private:
    		// Arity of the operator
    		int a_arity = -1;

    		// Name of the operator
    		std::string a_name = std::string();
    	};

    	class Algebra_Operators {
    	    // Class containaing all the content needed to parse a part of an algebra element
        public:
            // Algebra_Operators constructor
            Algebra_Operators(){};
    		Algebra_Operators(std::vector<Algebra_Element::Algebra_Operator> needed_operators, std::vector<Algebra_Element::Algebra_Operator> needed_functions);

    		// Getters and setters
            const std::vector<Algebra_Operator>& functions() const {return a_functions;};
            const std::vector<Algebra_Operator>& operators() const {return a_operators;};
        private:
            // Loaded normal functions
            std::vector<Algebra_Element::Algebra_Operator> a_functions;
            // Loaded normal operators
            std::vector<Algebra_Element::Algebra_Operator> a_operators;
    	};

    	// Container of unknowns
    	struct __Algebra_Unknown{std::string name = std::string();};
    	class Unknowns_Container {
		public:
			// Unknowns_Container constructor
			Unknowns_Container(){};
			virtual ~Unknowns_Container() = default;

			// Clears the container
			virtual void clear(){a_unknowns.clear();};

			// Handle unknown
			// Returns if the container contains an unknown
			bool contains_unknown_by_name(std::string name)const{for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns.at(i).get()->name == name){return true;}} return false;};
			// Creates a unknown
			__Algebra_Unknown* create_unknown(std::string name){return create_algebra_unknown_shared_ptr(name).get();};
            virtual std::shared_ptr<__Algebra_Unknown> create_algebra_unknown_shared_ptr(std::string name){std::shared_ptr<__Algebra_Unknown> temp=algebra_unknown_shared_ptr_by_name(name);if(temp.get()!=0){return temp;}std::shared_ptr<__Algebra_Unknown> unknown=std::make_shared<Algebra_Element::__Algebra_Unknown>();a_unknowns.push_back(unknown);unknown.get()->name=name;return unknown;};
			// Returns an unknown by its name
			Algebra_Element::__Algebra_Unknown* algebra_unknown_by_name(std::string name)const{return algebra_unknown_shared_ptr_by_name(name).get();};
			virtual std::shared_ptr<Algebra_Element::__Algebra_Unknown> algebra_unknown_shared_ptr_by_name(std::string name)const{for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns.at(i).get()->name == name){return a_unknowns.at(i);}} return std::shared_ptr<Algebra_Element::__Algebra_Unknown>();};

		private:
			// Unknowns
			std::vector<std::shared_ptr<__Algebra_Unknown>> a_unknowns;
		};

        // Algebra_Element constructor
        Algebra_Element();
        // Algebra_Element destructor
        virtual ~Algebra_Element();

        // Clears the object
        void clear();

        // Return if the element is a final element
        bool is_final_element() const;
        bool is_known() const;
        bool is_unknown() const;

        // Return if the element is a precise algebric element
        virtual bool is_addition_neutral() const {return false;};
        virtual bool is_multiplication_absorbing() const{return false;};
        virtual bool is_multiplication_neutral() const{return false;};

        // Creates a new algebra element of the same type
        void clone_base(Algebra_Element* e) const;
        void clone_base_without_sub_elements(Algebra_Element* e) const;
        virtual void algebra_clone(Algebra_Element* e) const = 0;
        virtual std::shared_ptr<Algebra_Element> algebra_clone() const = 0;
        virtual std::shared_ptr<Algebra_Element> new_algebra_element() const = 0;
        virtual std::shared_ptr<Algebra_Element> new_algebra_element(std::string content) const = 0;

        // Type of the object
        virtual std::string algebra_type() const = 0;

        // Returns if two elements are equal or not
        virtual bool is_equal(Algebra_Element* other) const = 0;
        virtual bool is_equal_with_value(Algebra_Element* other) const;
        virtual bool is_equal_without_value(Algebra_Element* other, std::string used_operator) const;

        // Evaluate a relation of this element with another one
        enum Relation_State {RS_In_Relation, RS_Not_In_Relation, RS_Unknown};
        virtual Relation_State evaluate_relation(Algebra_Element* other, std::string relation) = 0;
        bool in_relation(Algebra_Element* other, std::string relation);

        // Operates this element with another one
        virtual void operate(Algebra_Element* other, std::string operation) = 0;

        // Available operators for this object
        virtual const Algebra_Operators& operators() const;
        const Algebra_Operator* function_by_name(std::string function_name);
        const Algebra_Operator* operator_by_name(std::string operator_name);

        // Returns a list of all the unknowns
        std::vector<std::string> all_unknowns_name() const;

        // Replaces the unknowns
        virtual void replace_unknowns_algebra(Algebra_Element* element, Unknowns_Container* values) const;

        // Returns the first known algebra element of this element
        Algebra_Element* known_algebra_element();

        // Simplify the element
        virtual void simplify();

        // Sub-places the element
        void sub_place();

        // Getters and setters
        inline std::vector<std::shared_ptr<Algebra_Element>>& algebra_elements() {return a_elements;};
        inline const std::vector<std::shared_ptr<Algebra_Element>>& algebra_elements_const() const {return a_elements;};
        inline __Algebra_Unknown* algebra_unknown() const {return a_unknown.get();};
        inline std::string algebra_unknown_name() const {return a_unknown.get()->name;};
        inline Algebra_Operator* algebra_operator() {return &a_operator;};
        inline int algebra_operator_arity() const {return a_operator.arity();};
        inline std::string algebra_operator_name() const {return a_operator.name();};
        inline void set_algebra_operator(std::string new_algebra_operator){a_operator = Algebra_Operator(new_algebra_operator);};
        inline void set_algebra_operator(std::string new_algebra_operator, int new_algebra_arity){a_operator = Algebra_Operator(new_algebra_operator, new_algebra_arity);};

        // Virtual functions

        // Creates the unknown
        virtual __Algebra_Unknown* create_unknown();
        virtual __Algebra_Unknown* new_unknown(std::string unknown_name);

        // Return the value of the object in C++
        virtual std::string to_cpp() const = 0;

        // Returns the element to a simple std::string
        virtual std::string to_mathml(Textual_Math_Settings* settings) const = 0;
        virtual std::string to_std_string(Textual_Math_Settings* settings) const = 0;

        // DIfferent kinds of values
        virtual double value_to_double() const {return 0;};

    protected:
        // Parent object
        std::weak_ptr<Algebra_Element> a_parent;
        // This object
        std::weak_ptr<Algebra_Element> a_this_object;

        // Possible unknown
        std::shared_ptr<__Algebra_Unknown> a_unknown;

    private:
        // Every elements in this element
        Algebra_Operator a_operator = Algebra_Operator();
        std::vector<std::shared_ptr<Algebra_Element>> a_elements;
    };
    typedef Algebra_Element::Unknowns_Container Unknowns_Container;
}

#endif // SCLS_MATH_ALGEBRA_ELEMENT
