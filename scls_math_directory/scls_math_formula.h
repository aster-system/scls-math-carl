//******************
//
// scls_math_formula.h
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
// This file contains some functions to handle complex formula expression.
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

#ifndef SCLS_MATH_FORMULA
#define SCLS_MATH_FORMULA

#include "../scls_math_root.h"
#include "scls_math_extendable_fraction.h"
#include "scls_math_polynomial.h"
#include "scls_math_set.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

	//*********
	//
	// The "Proposition" class
	//
	//*********

	class Proposition {
		// Mathematical proposition
	public:
		// Proposition constructor
		Proposition(){};
		// Proposition destructor
		virtual ~Proposition() = default;

		// Evaluate the proposition
		virtual bool is_true() = 0;

	private:

	};

	class Relation : public Proposition {
		// Class representing a relation module
	public:
		// Type of relation
		enum class Relation_Type{Egality, Module, Order};

		// Relation constructor
		Relation():Proposition(){};
		// Relation destructor
		virtual ~Relation() = default;

		// Returns if a value is in relation with another
		virtual bool is_in_relation(std::vector<std::shared_ptr<Algebra_Element>> objects) = 0;

		// Evaluate the proposition
		virtual bool is_true(){return false;}

		// Relation type
		virtual Relation_Type relation_type() const = 0;
	};

	//*********
	//
	// The "Formula_Base_Field" class
	//
	//*********

    class Formula_Base_Field : public Algebra_Element {
        // Class representing a formula in a field
    public:
        // Formula_Base constructor
        Formula_Base_Field():Algebra_Element(){};
        Formula_Base_Field(std::shared_ptr<Algebra_Element> e):Formula_Base_Field(){a_value=e;};

        // Creates a new algebra element from this one
        virtual std::shared_ptr<Algebra_Element> algebra_clone() const = 0;
        virtual void algebra_clone(Algebra_Element* b) const {clone(reinterpret_cast<Formula_Base_Field*>(b));};
        virtual void clone(Formula_Base_Field* b) const {__clone_base(b);if(a_value.get() != 0){b->a_value = a_value.get()->algebra_clone();}else{b->a_value.reset();};};

        // Returns if the formula is empty
        bool empty() const;
        void fix_emptiness();

        // Adds an element to this one
        void add(Formula_Base_Field* formula);
        void divide(Formula_Base_Field* formula);
        void multiply(Formula_Base_Field* formula);
        virtual void operate(Algebra_Element* other, std::string operation);

        // Simplify the element
        virtual void simplify(){while(simplify_step() != scls::Formula_Base_Field::NO_SIMPLIFICATION){}};
        // Simplify the expression
        constexpr static char NO_SIMPLIFICATION = 0;
        constexpr static char SIMPLIFICATION_TERMINATED = 1;
        constexpr static char SIMPLIFICATION_UNTERMINATED = 2;
        virtual char simplify_step();

        // Obtains the precise elements
        virtual std::shared_ptr<Algebra_Element> multiplication_absorbing_field_element() const = 0;

        // Returns the element to a simple std::string
        virtual std::string to_mathml(Textual_Math_Settings* settings) const;
        virtual std::string to_std_string(Textual_Math_Settings* settings) const;

        // Getters and setters
        inline Algebra_Element* algebra_value() const {return a_value.get();};

    protected:
        // If the element is modified or not
        bool a_modified = true;
        // Value of the formula
        std::shared_ptr<Algebra_Element> a_value;

    };

    //*********
	//
	// The "Formula_Base" class
	//
	//*********

    class Formula_Base : public Formula_Base_Field {
    public:
        // Container of unknowns
        struct Formula_Unknown : public Algebra_Element::__Algebra_Unknown {std::shared_ptr<Formula_Base> value;void set_value(std::shared_ptr<Formula_Base> e){value=e;}void set_value(std::shared_ptr<Fraction> e){value=std::make_shared<Formula_Base>(e);}};;
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

        // Formula_Base constructor
        Formula_Base():Formula_Base_Field(){};
        Formula_Base(std::shared_ptr<Algebra_Element> e):Formula_Base_Field(e){};
        Formula_Base(Fraction e):Formula_Base_Field(std::make_shared<Fraction>(e)){};
        Formula_Base(std::string unknown_name):Formula_Base(){if(string_is_number(unknown_name)){a_value = std::make_shared<Fraction>(Fraction::from_std_string(unknown_name));}else{new_unknown(unknown_name);}};

        // Creates a new formula
        static std::shared_ptr<Formula_Base> new_formula(Fraction content) {std::shared_ptr<Formula_Base> s = std::make_shared<Formula_Base>(content);s.get()->a_this_object=s;return s;};

        // Creates a new algebra element of the same type
        void algebra_clone(Formula_Base* b) const {Formula_Base_Field::algebra_clone(b);};
        void algebra_clone(Algebra_Element* b) const {Formula_Base_Field::algebra_clone(b);};
        void clone(Formula_Base* b) const {Formula_Base_Field::clone(b);};
        virtual std::shared_ptr<Algebra_Element> algebra_clone() const {return clone();};
        virtual std::shared_ptr<Formula_Base> clone() const {std::shared_ptr<Formula_Base> b = std::make_shared<Formula_Base>();clone(b.get());b.get()->a_modified = a_modified;return b;};
        virtual std::shared_ptr<Algebra_Element> new_algebra_element() const {std::shared_ptr<Formula_Base> s = std::make_shared<Formula_Base>();s.get()->a_parent=a_this_object;s.get()->a_this_object=s;s.get()->a_modified = false;return s;};
        virtual std::shared_ptr<Algebra_Element> new_algebra_element(std::string content) const {std::shared_ptr<Formula_Base> s = std::make_shared<Formula_Base>(content);s.get()->a_parent=a_this_object;s.get()->a_this_object=s;return s;};
        virtual std::shared_ptr<Formula_Base> new_formula(std::string content) const {std::shared_ptr<Formula_Base> s = std::make_shared<Formula_Base>(content);s.get()->a_parent=a_this_object;s.get()->a_this_object=s;return s;};

        // Type of the object
        virtual std::string algebra_type() const;

        // Obtains the precise elements
        virtual std::shared_ptr<Algebra_Element> multiplication_absorbing_field_element() const{return std::make_shared<Fraction>(0);};

        // Returns a part of the formula
        Formula_Base* formula_element(int index);

        // Adds an element to this one
        void add(Formula_Base* formula);
        void add(Fraction other);
        void divide(Formula_Base* formula);
        void divide(Fraction other);
        void multiply(Formula_Base* formula);
        void multiply(Fraction other);
        void operate(Formula_Base* formula, std::string operation);
        void operate(Fraction other, std::string operation);
        void substract(Formula_Base* formula);
        void substract(Fraction other);

        // Equality operators
        bool equals(Fraction f);

        // Creates the unknown
        virtual Algebra_Element::__Algebra_Unknown* create_unknown();

        // Available operators for this object
        virtual const Algebra_Operators& operators() const;

        // Replaces the unknowns
        virtual void replace_unknowns_algebra(Algebra_Element* element, Algebra_Element::Unknowns_Container* values) const;
        std::shared_ptr<Formula_Base> replace_unknowns(std::string unknown, scls::Fraction f) const;
        std::shared_ptr<Formula_Base> replace_unknowns(Unknowns_Container* values) const;

        // Simplify the formula
        virtual char simplify_step();

        // Returns the definition domain
        Set_Number definition_domain();

        // Returns a set of number which respects a precise relatio,
        Set_Number check_relation(Relation* r, Formula_Base* f);

        // Returns if a precise number is defined or not
        bool is_defined(Fraction f);
        bool is_fully_defined(Fraction f_1, Fraction f_2);

        // DIfferent kinds of values
        virtual double value_to_double(Unknowns_Container* values) const;
        virtual double value_to_double() const;

        // Getters and setters
        template <typename T> T* value() const {return reinterpret_cast<T*>(algebra_value());};
    };

    class Extendable_Formula_Base : public Formula_Base_Field {
    public:
        // Container of unknowns
        struct Extended_Formula_Unknown : public Algebra_Element::__Algebra_Unknown {std::shared_ptr<Extendable_Formula_Base> value;void set_value(std::shared_ptr<Extendable_Formula_Base> e){value=e;}void set_value(std::shared_ptr<Extendable_Fraction> e){value=std::make_shared<Extendable_Formula_Base>(e);}};;
        class Extendable_Unknowns_Container : public Algebra_Element::Unknowns_Container {
        public:
            // Extendable_Unknowns_Container constructor
            Extendable_Unknowns_Container(){};
            // Extendable_Unknowns_Container destructor
            virtual ~Extendable_Unknowns_Container(){};

            // Clears the container
            virtual void clear(){a_unknowns.clear();};

            // Handle unknown
            // Creates a unknown
            Extended_Formula_Unknown* create_unknown(std::string name){return create_unknown_shared_ptr(name).get();};
            std::shared_ptr<Extended_Formula_Unknown> create_unknown_shared_ptr(std::string name){std::shared_ptr<Extended_Formula_Unknown> temp=unknown_shared_ptr_by_name(name);if(temp.get()!=0){return temp;}std::shared_ptr<Extended_Formula_Unknown> unknown=std::make_shared<Extended_Formula_Unknown>();a_unknowns.push_back(unknown);unknown.get()->name=name;return unknown;};
            virtual std::shared_ptr<__Algebra_Unknown> create_algebra_unknown_shared_ptr(std::string name){return create_unknown_shared_ptr(name);};
            // Returns an unknown by its name
            virtual std::shared_ptr<Algebra_Element::__Algebra_Unknown> algebra_unknown_shared_ptr_by_name(std::string name)const{return unknown_shared_ptr_by_name(name);};
            Extended_Formula_Unknown* unknown_by_name(std::string name)const{return unknown_shared_ptr_by_name(name).get();};
            std::shared_ptr<Extended_Formula_Unknown> unknown_shared_ptr_by_name(std::string name)const{for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns.at(i).get()->name == name){return a_unknowns.at(i);}} return std::shared_ptr<Extended_Formula_Unknown>();};

        private:
            // Unknowns
            std::vector<std::shared_ptr<Extended_Formula_Unknown>> a_unknowns;
        };

        // Extendable_Formula_Base constructor
        Extendable_Formula_Base():Formula_Base_Field(){};
        Extendable_Formula_Base(std::shared_ptr<Algebra_Element> e):Formula_Base_Field(e){};
        Extendable_Formula_Base(Extendable_Fraction e):Formula_Base_Field(std::make_shared<Extendable_Fraction>(e)){};
        Extendable_Formula_Base(std::string unknown_name):Extendable_Formula_Base(){if(string_is_number(unknown_name)){a_value = std::make_shared<Extendable_Fraction>(Fraction::from_std_string(unknown_name));}else{new_unknown(unknown_name);}};

        // Creates a new formula
        static std::shared_ptr<Extendable_Formula_Base> new_formula(Extendable_Fraction content) {std::shared_ptr<Extendable_Formula_Base> s = std::make_shared<Extendable_Formula_Base>(content);s.get()->a_this_object=s;return s;};

        // Creates a new algebra element of the same type
        void algebra_clone(Extendable_Formula_Base* b) const {Formula_Base_Field::algebra_clone(b);};
        void algebra_clone(Algebra_Element* b) const {Formula_Base_Field::algebra_clone(b);};
        void clone(Extendable_Formula_Base* b) const {Formula_Base_Field::clone(b);};
        virtual std::shared_ptr<Algebra_Element> algebra_clone() const {return clone();};
        virtual std::shared_ptr<Extendable_Formula_Base> clone() const {std::shared_ptr<Extendable_Formula_Base> b = std::make_shared<Extendable_Formula_Base>();clone(b.get());b.get()->a_modified = a_modified;return b;};
        virtual std::shared_ptr<Algebra_Element> new_algebra_element() const {std::shared_ptr<Extendable_Formula_Base> s = std::make_shared<Extendable_Formula_Base>();s.get()->a_parent=a_this_object;s.get()->a_this_object=s;s.get()->a_modified = false;return s;};
        virtual std::shared_ptr<Algebra_Element> new_algebra_element(std::string content) const {std::shared_ptr<Extendable_Formula_Base> s = std::make_shared<Extendable_Formula_Base>(content);s.get()->a_parent=a_this_object;s.get()->a_this_object=s;return s;};
        virtual std::shared_ptr<Extendable_Formula_Base> new_formula(std::string content) const {std::shared_ptr<Extendable_Formula_Base> s = std::make_shared<Extendable_Formula_Base>(content);s.get()->a_parent=a_this_object;s.get()->a_this_object=s;return s;};

        // Type of the object
        virtual std::string algebra_type() const;

        // Obtains the precise elements
        virtual std::shared_ptr<Algebra_Element> multiplication_absorbing_field_element() const{return std::make_shared<Extendable_Fraction>(0);};

        // Returns a part of the formula
        Extendable_Formula_Base* formula_element(int index);

        // Adds an element to this one
        void add(Extendable_Formula_Base* formula);
        void add(Extendable_Fraction other);
        void divide(Extendable_Formula_Base* formula);
        void divide(Extendable_Fraction other);
        void multiply(Extendable_Formula_Base* formula);
        void multiply(Extendable_Fraction other);
        void operate(Extendable_Formula_Base* formula, std::string operation);
        void operate(Extendable_Fraction other, std::string operation);
        void substract(Extendable_Formula_Base* formula);
        void substract(Extendable_Fraction other);

        // Creates the unknown
        virtual Algebra_Element::__Algebra_Unknown* create_unknown();

        // Available operators for this object
        virtual const Algebra_Operators& operators() const;

        // Replaces the unknowns
        virtual void replace_unknowns_algebra(Algebra_Element* element, Algebra_Element::Unknowns_Container* values) const;
        std::shared_ptr<Extendable_Formula_Base> replace_unknowns(std::string unknown, Extendable_Fraction f) const;
        std::shared_ptr<Extendable_Formula_Base> replace_unknowns(Unknowns_Container* values) const;

        // Simplify the formula
        virtual char simplify_step();

        // DIfferent kinds of values
        virtual double value_to_double(Extendable_Unknowns_Container* values) const;
        virtual double value_to_double() const;

        // Getters and setters
        inline Algebra_Element* algebra_value() const {return a_value.get();};
        template <typename T> T* value() const {return reinterpret_cast<T*>(algebra_value());};
    };

    class Relation_Module : public Relation {
		// Class representing a relation module
	public:
		// Relation_Module constructor
		Relation_Module(scls::Fraction modulo):a_modulo(modulo){};
		Relation_Module(){};

		// Returns if a value is in relation with another
		scls::Fraction congruence_class(scls::Fraction f_1) {
			scls::Fraction to_return = f_1 - ((f_1 / a_modulo).to_double_floor()) * a_modulo;
			if(to_return < 0){to_return += a_modulo;}
			return to_return;
		};

		// Returns if a value is in relation with another
		virtual bool is_in_relation(std::vector<std::shared_ptr<Algebra_Element>> objects){if(objects.size() != 2){return false;}return is_in_relation(*reinterpret_cast<Formula_Base*>(objects.at(0).get())->value<Fraction>(), *reinterpret_cast<Formula_Base*>(objects.at(1).get())->value<Fraction>());};
		bool is_in_relation(scls::Fraction f_1, scls::Fraction f_2) {return congruence_class(f_1) == congruence_class(f_2);};
		bool is_in_relation_interval(scls::Fraction f_1, scls::Fraction f_2) {
			scls::Fraction a = congruence_class(f_1);
			scls::Fraction b = congruence_class(f_2);
			return (a - b) != (f_1 - f_2);
		};

		// Evaluate the proposition
		virtual bool is_true(){return false;}

		// Relation type
		virtual Relation_Type relation_type() const{return Relation_Type::Module;};

	private:
		// Type of relation
		int a_type = 0;

		// Class of the relation
		scls::Fraction a_modulo = 1;
	};

    class Relation_Egality : public Relation {
		// Class representing an egality relation
	public:

		// Relation_Order constructor
    	Relation_Egality(){};

		// Returns if a value is in relation with another
		virtual bool is_in_relation(std::vector<std::shared_ptr<Algebra_Element>> objects){if(objects.size() != 2){return false;}return is_in_relation(*reinterpret_cast<Formula_Base*>(objects.at(0).get())->value<Fraction>(), *reinterpret_cast<Formula_Base*>(objects.at(1).get())->value<Fraction>());};
		bool is_in_relation(scls::Fraction f_1, scls::Fraction f_2) {return f_1 == f_2;};

		// Evaluate the proposition
		virtual bool is_true(){return false;}

		// Relation type
		virtual Relation_Type relation_type() const{return Relation_Type::Egality;};
	};

	class Relation_Order : public Relation {
		// Class representing a relation module
	public:
		// Type of relation
		static const int greater_equal = 0;
		static const int greater_strict = 1;
		static const int lesser_equal = 2;
		static const int lesser_strict = 3;

		// Relation_Order constructor
		Relation_Order(int relation):a_type(relation){};
		Relation_Order(){};

		// Returns if a value is in relation with another
		virtual bool is_in_relation(std::vector<std::shared_ptr<Algebra_Element>> objects){if(objects.size() != 2){return false;}return is_in_relation(*reinterpret_cast<Formula_Base*>(objects.at(0).get())->value<Fraction>(), *reinterpret_cast<Formula_Base*>(objects.at(1).get())->value<Fraction>());};
		bool is_in_relation(scls::Fraction f_1, scls::Fraction f_2) {
			if(a_type == greater_equal){return f_1 >= f_2;}
			else if(a_type == greater_strict){return f_1 > f_2;}
			else if(a_type == lesser_equal){return f_1 <= f_2;}
			else if(a_type == lesser_strict){return f_1 < f_2;}
			return false;
		};

		// Evaluate the proposition
		virtual bool is_true(){return false;}

		// Relation type
		virtual Relation_Type relation_type() const{return Relation_Type::Order;};

	private:
		// Type of relation
		int a_type = 0;
	};

	//*********
	//
	// Basic analysis tools
	//
	//*********

	// Returns the continuity domain
	Set_Number continuity_domain();

	// Returns the derivability domain
	Set_Number deravibility_domain();

    // Derivate a formula
    std::shared_ptr<scls::Extendable_Formula_Base> derivate(scls::Extendable_Formula_Base* f, std::string unknown);
    std::shared_ptr<scls::Formula_Base> derivate(scls::Formula_Base* f, std::string unknown);

    // Apply a McLaurin serie
    std::shared_ptr<scls::Extendable_Formula_Base> mclaurin(scls::Extendable_Formula_Base* f, std::string unknown, int step);
    std::shared_ptr<scls::Formula_Base> mclaurin(scls::Formula_Base* f, std::string unknown, int step);
}

#endif // SCLS_MATH_FORMULA
