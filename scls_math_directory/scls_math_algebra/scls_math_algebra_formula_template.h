//******************
//
// scls_math_formula_template.h
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
// This file contains some functions to handle the base of the formula system.
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

#ifndef SCLS_MATH_ALGEBRA_FORMULA_TEMPLATE
#define SCLS_MATH_ALGEBRA_FORMULA_TEMPLATE

#include "./scls_math_algebra_formula_field_base.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The "Formula_Base_Template" class
	//
	//*********

	extern Algebra_Element::Algebra_Operators formula_operators;

	template <typename T>
	class Formula_Base_Template : public Formula_Base_Field {
    public:
        // Container of unknowns
        struct Formula_Unknown : public Algebra_Element::__Algebra_Unknown {
            std::shared_ptr<Formula_Base_Template<T>> value = std::make_shared<Formula_Base_Template<T>>(std::make_shared<T>(0));

            void set_value(std::shared_ptr<Formula_Base_Template<T>> e);
            void set_value(std::shared_ptr<T> e);
            void set_value(T e);
        };
        class Unknowns_Container : public Algebra_Element::Unknowns_Container {
		public:
			// Unknowns_Container constructor
			Unknowns_Container(){};
			// Unknowns_Container destructor
			virtual ~Unknowns_Container(){};

			// Clears the container
			virtual void clear(){a_unknowns.clear();};

			// Handle unknown
			// Creates a unknown
			Formula_Unknown* create_unknown(std::string name){return create_unknown_shared_ptr(name).get();};
            std::shared_ptr<Formula_Unknown> create_unknown_shared_ptr(std::string name){std::shared_ptr<Formula_Unknown> temp=unknown_shared_ptr_by_name(name);if(temp.get()!=0){return temp;}std::shared_ptr<Formula_Unknown> unknown=std::make_shared<Formula_Unknown>();a_unknowns.push_back(unknown);unknown.get()->name=name;return unknown;};
			virtual std::shared_ptr<__Algebra_Unknown> create_algebra_unknown_shared_ptr(std::string name){return create_unknown_shared_ptr(name);};
			// Returns an unknown by its name
			virtual std::shared_ptr<Algebra_Element::__Algebra_Unknown> algebra_unknown_shared_ptr_by_name(std::string name)const{return unknown_shared_ptr_by_name(name);};
            Formula_Unknown* unknown_by_name(std::string name)const{return unknown_shared_ptr_by_name(name).get();};
            std::shared_ptr<Formula_Unknown> unknown_shared_ptr_by_name(std::string name)const{for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns.at(i).get()->name == name){return a_unknowns.at(i);}} return std::shared_ptr<Formula_Unknown>();};

		private:
			// Unknowns
			std::vector<std::shared_ptr<Formula_Unknown>> a_unknowns;
		};

        // Formula_Base_Template constructor
        Formula_Base_Template():Formula_Base_Field(){};
        Formula_Base_Template(std::shared_ptr<Algebra_Element> e):Formula_Base_Field(e){};
        Formula_Base_Template(T e):Formula_Base_Field(std::make_shared<T>(e)){};
        Formula_Base_Template(std::string unknown_name):Formula_Base_Template(){if(string_is_number(unknown_name, {'i'})){a_value = std::make_shared<T>(T::from_std_string(unknown_name));}else{new_unknown(unknown_name);}};

        // Clone this algebra element
        void algebra_clone(Formula_Base_Template<T>* b) const {Formula_Base_Field::algebra_clone(b);};
        void algebra_clone(Algebra_Element* b) const {Formula_Base_Field::algebra_clone(b);};
        virtual void clone(Formula_Base_Field* b) const {Formula_Base_Field::clone(b);};
        virtual std::shared_ptr<Algebra_Element> algebra_clone() const {return clone();};
        virtual std::shared_ptr<Formula_Base_Template<T>> clone() const {std::shared_ptr<Formula_Base_Template<T>> b = std::make_shared<Formula_Base_Template<T>>();clone(b.get());b.get()->a_modified = a_modified;return b;};
        virtual std::shared_ptr<Formula_Base_Field> formula_clone() const {return clone();};

        // Creates a new algebra element of the same type
        virtual std::shared_ptr<Algebra_Element> new_algebra_element() const {std::shared_ptr<Formula_Base_Template<T>> s = std::make_shared<Formula_Base_Template<T>>();s.get()->a_parent=a_this_object;s.get()->a_this_object=s;s.get()->a_modified = false;return s;};
        virtual std::shared_ptr<Algebra_Element> new_algebra_element(std::string content) const {std::shared_ptr<Formula_Base_Template<T>> s = std::make_shared<Formula_Base_Template<T>>(content);s.get()->a_parent=a_this_object;s.get()->a_this_object=s;return s;};
        static std::shared_ptr<Formula_Base_Template<T>> new_formula() {std::shared_ptr<Formula_Base_Template<T>> s = std::make_shared<Formula_Base_Template<T>>();s.get()->a_this_object=s;return s;};
        static std::shared_ptr<Formula_Base_Template<T>> new_formula(T content) {std::shared_ptr<Formula_Base_Template<T>> s = std::make_shared<Formula_Base_Template<T>>(content);s.get()->a_this_object=s;return s;};
        static std::shared_ptr<Formula_Base_Template<T>> new_formula(std::shared_ptr<Algebra_Element> content) {std::shared_ptr<Formula_Base_Template<T>> s = std::make_shared<Formula_Base_Template<T>>(content);s.get()->a_this_object=s;return s;};
        static std::shared_ptr<Formula_Base_Template<T>> new_formula(std::string content) {std::shared_ptr<Formula_Base_Template<T>> s = std::make_shared<Formula_Base_Template<T>>(content);s.get()->a_this_object=s;return s;};
        virtual std::shared_ptr<Formula_Base_Field> new_formula_field(std::string content) const {return new_formula(content);};

        // Type of the object
        virtual std::string algebra_type() const{return std::string("formula_base_template");};

        // Returns the formula as a C++ function
        virtual std::string cpp_type_with_library() const {return "scls::Double";};

        // Obtains the precise elements
        virtual std::shared_ptr<Algebra_Element> multiplication_absorbing_field_element() const{return std::make_shared<T>(0);};

        // Returns a part of the formula
        Formula_Base_Template<T>* formula_element(int index);

        // Adds an element to this one
        // Algebra element
        void add(Formula_Base_Template<T>* formula){Formula_Base_Field::add(formula);};;
        void divide(Formula_Base_Template<T>* formula){Formula_Base_Field::divide(formula);};;
        void multiply(Formula_Base_Template<T>* formula){Formula_Base_Field::multiply(formula);};;
        void substract(Formula_Base_Template<T>* formula){std::shared_ptr<Formula_Base_Template<T>>f=formula->clone();f.get()->multiply(-1);add(f.get());};
        virtual void operate(Algebra_Element* formula, std::string operation){Formula_Base_Field::operate(formula, operation);};;
        // T
        void add(T other){std::shared_ptr<Formula_Base_Template<T>>f=new_formula(other);add(f.get());};
        void divide(T other){std::shared_ptr<Formula_Base_Template<T>>f=new_formula(other);divide(f.get());};
        void multiply(T other){std::shared_ptr<Formula_Base_Template<T>>f=new_formula(other);multiply(f.get());};
        void operate(T other, std::string operation){std::shared_ptr<Formula_Base_Template<T>>f=new_formula(other);operate(f.get(), operation);};
        void substract(T other){std::shared_ptr<Formula_Base_Template<T>>f=new_formula(other);substract(f.get());};

        // Returns the neutral of known operations
        std::shared_ptr<Algebra_Element> operation_neutral(std::string operation) const {
            std::shared_ptr<Algebra_Element> n = algebra_value_for_type()->operation_neutral(operation);
            if(n.get() == 0){return std::shared_ptr<Formula_Base_Template<T>>();}
            return new_formula(n);
        }

        // Creates the unknown
        virtual Algebra_Element::__Algebra_Unknown* create_unknown(){clear();a_unknown = std::make_shared<Formula_Unknown>();return a_unknown.get();};;

        // Available operators for this object
        virtual const Algebra_Operators& operators() const {return formula_operators;};

        // Replaces the unknowns
        virtual void replace_unknowns_algebra(Algebra_Element* element, Algebra_Element::Unknowns_Container* values_raw) const{
            Formula_Base_Template<T>::Unknowns_Container* values = reinterpret_cast<Formula_Base_Template<T>::Unknowns_Container*>(values_raw);

            // The element is final
            if(is_final_element()) {
                if(is_unknown()){
                    Formula_Unknown* current = reinterpret_cast<Formula_Unknown*>(values->algebra_unknown_by_name(algebra_unknown()->name));
                    if(current == 0){algebra_clone(element);}
                    else{current->value.get()->clone(reinterpret_cast<Formula_Base_Template<T>*>(element));}
                }
                else if(a_value.get() != 0) {reinterpret_cast<Formula_Base_Template<T>*>(element)->a_value = a_value.get()->algebra_clone();}
                else{reinterpret_cast<Formula_Base_Template<T>*>(element)->a_value = std::make_shared<T>(0);}
            }
            else {Algebra_Element::replace_unknowns_algebra(element, values);}

            // Modified
            reinterpret_cast<Formula_Base_Template<T>*>(element)->a_modified = true;

            // Simplification
            element->simplify();
        };
        virtual std::shared_ptr<Formula_Base_Field> replace_unknowns_formula(std::string unknown, Algebra_Element* element) const{return replace_unknowns(unknown, *reinterpret_cast<T*>(element));};
        std::shared_ptr<Formula_Base_Template<T>> replace_unknowns(std::string unknown, std::shared_ptr<Formula_Base_Template<T>> f) const{Unknowns_Container c;c.create_unknown(unknown)->value = f;return replace_unknowns(&c);};
        std::shared_ptr<Formula_Base_Template<T>> replace_unknowns(std::string unknown, T f) const{Unknowns_Container c;c.create_unknown(unknown)->value = new_formula(f);return replace_unknowns(&c);};
        std::shared_ptr<Formula_Base_Template<T>> replace_unknowns(Unknowns_Container* values) const{std::shared_ptr<Formula_Base_Template<T>> s = std::make_shared<Formula_Base_Template<T>>();s.get()->a_this_object=s;replace_unknowns_algebra(s.get(), values);s.get()->simplify();return s;};

        // Simplify the formula
        virtual char simplify_step(){return Formula_Base_Field::simplify_step();};

        // Different kinds of values
        virtual double value_to_double(Unknowns_Container* values) const{std::shared_ptr<Formula_Base_Template<T>> u = replace_unknowns(values);return u.get()->value_to_double();};
        virtual double value_to_double(std::string unknown, T f) const{std::shared_ptr<Formula_Base_Template<T>> u = replace_unknowns(unknown, f);return u.get()->value_to_double();};
        virtual double value_to_double() const{if(a_value.get() == 0){return 0;}return value()->to_double();};;

        // Get an non-empty pointer containing an "T" object (useful if algebra_value() == 0 but the formula as children)
        virtual Algebra_Element* algebra_value_for_type() const{return a_value_for_type.get();};

        // Getters and setters
        T* value() const {return reinterpret_cast<T*>(algebra_value());};

    private:

        // Object containing the data for the type of the formula
        std::shared_ptr<T> a_value_for_type = std::make_shared<T>();
    };
}

#endif // SCLS_MATH_ALGEBRA_FORMULA_TEMPLATE
