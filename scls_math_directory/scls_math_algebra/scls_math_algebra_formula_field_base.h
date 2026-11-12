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

#ifndef SCLS_MATH_ALGEBRA_FORMULA_FIELD_BASE
#define SCLS_MATH_ALGEBRA_FORMULA_FIELD_BASE

#include "../../scls_math_root.h"
#include "../scls_math_extendable_fraction.h"
#include "../scls_math_polynomial.h"
#include "../scls_math_set.h"

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
        virtual void clone(Formula_Base_Field* b) const {clone_base(b);if(a_value.get() != 0){b->a_value = a_value.get()->algebra_clone();}else{b->a_value.reset();};};

        // Returns if the formula is empty
        bool empty() const;
        void fix_emptiness();

        // Datas about the formula
        virtual bool is_multiplication_absorbing() const;
        virtual bool is_multiplication_neutral() const;

        // Returns if two elements are equal or not
        virtual bool is_equal(Algebra_Element* other) const;

        // Evaluate a relation of this element with another one
        virtual Relation_State evaluate_relation(Algebra_Element* other, std::string relation);

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

        // Replaces the unknowns
        virtual std::shared_ptr<Formula_Base_Field> replace_unknowns_formula(std::string unknown, Algebra_Element* element) const = 0;

        // Returns the formula as a C++ function
        virtual std::string cpp_type_with_library() const = 0;
        virtual std::string to_cpp() const;
        virtual std::string to_cpp_function(const std::string& function_name);

        // Returns the element to a simple std::string
        virtual std::string to_mathml(Textual_Math_Settings* settings) const;
        virtual std::string to_std_string(Textual_Math_Settings* settings) const;

        // Different kinds of values
        virtual double value_to_double() const = 0;

        // Getters and setters
        inline Algebra_Element* algebra_value() const {return a_value.get();};

    protected:
        // If the element is modified or not
        bool a_modified = true;
        // Value of the formula
        std::shared_ptr<Algebra_Element> a_value;

    };

}

#endif // SCLS_MATH_ALGEBRA_FORMULA_FIELD_BASE
