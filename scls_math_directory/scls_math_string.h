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

#include "scls_math_formula.h"
#include "scls_math_geometry_core.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The parsers class
	//
	//*********

	class String_To_Formula_Parse {
        // Class allowing to properly parse a std::string to a polymonial
    public:

        class Math_Environment {
            // Class representating an environment for maths. in SCLS
        public:

            // Math_Environment constructor
            Math_Environment();
            // Math_Environment destructor
            virtual ~Math_Environment(){};

            // Converts a string to an algebra element
            static void string_to_algebra_element(Math_Environment* env, Algebra_Element* element, std::string source, const std::vector<Algebra_Element::Algebra_Operator>& o);

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

            // Getters and setters
            inline String_To_Formula_Parse* parser()const{return a_parser.get();};
            inline std::vector<int>& repetitions() {return a_repetitions;};
            inline Formula_Base::Unknowns_Container* unknowns(){return a_unknowns.get();};
            inline std::shared_ptr<Formula_Base::Unknowns_Container> unknowns_shared_ptr(){return a_unknowns;};

        private:

            // Parser
            std::shared_ptr<String_To_Formula_Parse> a_parser = std::make_shared<String_To_Formula_Parse>();
            // Repetitions
            std::vector<int> a_repetitions;
            // Variables
            std::shared_ptr<Formula_Base::Unknowns_Container> a_unknowns = std::make_shared<Formula_Base::Unknowns_Container>();
        };

        // String_To_Formula_Parse constructor
        String_To_Formula_Parse(unsigned int level);
        String_To_Formula_Parse():String_To_Formula_Parse(0){};

        // Returns if a std::string is a number or not
        inline bool __string_is_number(char text) const {return (string_is_number(text) || text == '/' || text == 'i' || text == '-');};
        inline bool __string_is_number(std::string text) const {for(int i = 0;i<static_cast<int>(text.size());i++) {if(!__string_is_number(text[i])) return false;} return true;};
        // Returns if a std::string is an operator or not
        bool __string_is_operator(char text) const;

        // Add a function to the defined functions
        inline void add_function(std::string function_name) {if(!contains_function(function_name)) a_functions.push_back(function_name); };
        // Returns if a function is defined or not
        bool contains_function(std::string function_name) const;
        bool contains_function(char function_name) const;
        // Returns the indentation for this level
        inline std::string level_indentation() const {std::string to_return = "";for(int i = 0;i<static_cast<int>(level()*4);i++)to_return+=" ";return to_return;};

        // Getters and setters
        inline std::vector<std::string>& functions() {return a_functions;};
        inline unsigned int level() const {return a_level;};
    private:

        // Every defined functions in the parser
        std::vector<std::string> a_functions;
        // Level of the parser
        unsigned int a_level = 0;
    };
    typedef String_To_Formula_Parse::Math_Environment Math_Environment;

    // Use parsers methods outside the class
    template <typename T> std::shared_ptr<T> string_to_algebra_element(std::string source){std::shared_ptr<T> new_object = std::make_shared<T>();Math_Environment::string_to_algebra_element(0, new_object.get(), source, new_object.get()->operators().operators());return new_object;};
}

#endif // SCLS_MATH_STRING
