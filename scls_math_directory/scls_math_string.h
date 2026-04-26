//******************
//
// scls_math_string.h
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
// This file contains some functions to handle string with mathematics.
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

#ifndef SCLS_MATH_STRING
#define SCLS_MATH_STRING

#include "../scls_math_root.h"
#include "scls_math_formula.h"
#include "scls_math_geometry_core.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The environment class
	//
	//*********

	class Math_Environment {
        // Class representating an environment for maths. in SCLS
    public:

        class Namespace {
            // Class representing a variable container for maths. in SCLS
        public:
            // Namespace constructor
            Namespace(std::string name);

            // Handle unknowns
            // Creates a unknown
            Formula_Base::Formula_Unknown* create_unknown(std::string name);
            std::shared_ptr<Formula_Base::Formula_Unknown> create_unknown_shared_ptr(std::string name);
            // Sets the value of an unknown by its name
            void set_unknown_value_by_name(std::string name, Fraction new_value);
            void set_unknown_value_by_name(std::string name, std::shared_ptr<Formula_Base> new_value);
            // Returns a value by its name
            scls::Fraction value_by_name(std::string name)const;
            // Returns a unknown by its name
            Formula_Base::Formula_Unknown* unknown_by_name(std::string name)const;
            std::shared_ptr<Formula_Base::Formula_Unknown> unknown_shared_ptr_by_name(std::string name)const;

        private:
            // Name of the namespace
            std::string a_name = std::string();

            // Variables
            std::shared_ptr<Formula_Base::Unknowns_Container> a_unknowns = std::make_shared<Formula_Base::Unknowns_Container>();
        };

        // Math_Environment constructor
        Math_Environment();
        // Math_Environment destructor
        virtual ~Math_Environment(){};

        // Converts a string to an algebra element
        static void string_to_algebra_element(const Math_Environment* env, Algebra_Element* element, std::string source, const std::vector<Algebra_Element::Algebra_Operator>& o);

        // Clears the environment
        void clear();

        // Handle repetitions
        // Adds a repetition
        void add_repetition();
        // Returns the last repetition
        int last_repetition();
        // Removes the last repetition
        void remove_repetition();
        // Repetes the last repetition
        void repeat();
        // Returns a repetition
        int repetition(int index) const;
        // Sets the last repetition
        void set_repetition(int value);

        // Conditions
        // Evaluate relation
        bool evaluate_relation();

        // Handle unknowns
        // Creates a unknown
        Formula_Base::Formula_Unknown* create_unknown(std::string name);
        std::shared_ptr<Formula_Base::Formula_Unknown> create_unknown_shared_ptr(std::string name);
        // Sets the value of an unknown by its name
        void set_unknown_value_by_name(std::string name, Fraction new_value);
        void set_unknown_value_by_name(std::string name, std::shared_ptr<Formula_Base> new_value);
        // Returns a value by its name
        scls::Fraction value_by_name(std::string name)const;
        // Returns a unknown by its name
        Formula_Base::Formula_Unknown* unknown_by_name(std::string name)const;
        std::shared_ptr<Formula_Base::Formula_Unknown> unknown_shared_ptr_by_name(std::string name)const;

        // Returns a formula value
        virtual std::shared_ptr<Formula_Base> value_formula(std::string base)const;

        // Returns a number value
        double value_double(std::string base)const;
        scls::Fraction value_number(std::string base)const;
        // Returns a point 2D value
        scls::Point_2D_Formula value_point_2d(std::string base) const;

        // Namespaces
        // Add a namespace in the stack
        void add_namespace_stack(std::shared_ptr<Namespace>needed_namespace);
        void pop_namespace_stack();
        // Create a namespace
        std::shared_ptr<Namespace> create_namespace(std::string name);
        // Returns the back namespace
        Namespace* back_namespace();

        // Getters and setters
        inline int namespaces_size() const {return a_namespaces_size;};
        inline std::vector<int>& repetitions() {return a_repetitions;};
        inline Formula_Base::Unknowns_Container* unknowns(){return a_unknowns.get();};
        inline std::shared_ptr<Formula_Base::Unknowns_Container> unknowns_shared_ptr(){return a_unknowns;};

    private:

        // Namespaces
        std::list<std::shared_ptr<Namespace>> a_namespaces;
        int a_namespaces_size = 0;
        // Repetitions
        std::vector<int> a_repetitions;
        // Variables
        std::shared_ptr<Formula_Base::Unknowns_Container> a_unknowns = std::make_shared<Formula_Base::Unknowns_Container>();
    };

    // Use parsers methods outside the class
    template <typename T> std::shared_ptr<T> string_to_algebra_element(std::string source, const Math_Environment* env){std::shared_ptr<T> new_object = std::make_shared<T>();Math_Environment::string_to_algebra_element(env, new_object.get(), source, new_object.get()->operators().operators());return new_object;};
    template <typename T> std::shared_ptr<T> string_to_algebra_element(std::string source){return string_to_algebra_element<T>(source, 0);};
}

#endif // SCLS_MATH_STRING
