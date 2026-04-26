//******************
//
// scls_math_extendable_fraction.h
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
// This file contains some tools to work with very very big / precise fractions in C++.
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

#ifndef SCLS_MATH_EXTANDABLE_FRACTION
#define SCLS_MATH_EXTANDABLE_FRACTION

// Include SCLS Math bigger header
#include "../scls_math_root.h"
#include "scls_math_extendable_int.h"

namespace scls {
    class Extendable_Fraction : public Algebra_Element {
        // Class representing an extandable fraction
    public:
        // Extendable_Fraction constructor
        Extendable_Fraction();
        Extendable_Fraction(double f);
        Extendable_Fraction(Fraction f);
        Extendable_Fraction(Extendable_Int value);
        Extendable_Fraction(int64_t numerator, int64_t denominator);
        Extendable_Fraction(Extendable_Int numerator, Extendable_Int denominator);
        virtual ~Extendable_Fraction();

        // Creates a new algebra element of the same type
        void clone(Extendable_Fraction* e) const{__clone_base(e);e->a_denominator = a_denominator;e->a_numerator = a_numerator;};
        virtual void algebra_clone(Algebra_Element* e) const{clone(reinterpret_cast<Extendable_Fraction*>(e));};
        virtual std::shared_ptr<Algebra_Element> algebra_clone() const{std::shared_ptr<Extendable_Fraction> to_return = std::make_shared<Extendable_Fraction>();clone(to_return.get());return to_return;};
        virtual std::shared_ptr<Algebra_Element> new_algebra_element() const{std::shared_ptr<Extendable_Fraction> to_return = std::make_shared<Extendable_Fraction>();return to_return;};
        virtual std::shared_ptr<Algebra_Element> new_algebra_element(std::string content) const{std::shared_ptr<Extendable_Fraction> to_return = std::make_shared<Extendable_Fraction>(scls::Fraction::from_std_string(content));return to_return;};

        // Type of the object
        virtual std::string algebra_type() const{return std::string("extendable_fraction");};

        // Return if the element is a precise algebric element
        virtual bool is_addition_neutral() const {return a_numerator.is_null();};
        virtual bool is_multiplication_neutral() const{return a_numerator.is_equal(&a_denominator);};
        virtual bool is_multiplication_absorbing() const {return a_numerator.is_null();};

        // Add two fractions
        void add(Extendable_Fraction* other);

        // Divide two fractions
        void divide(Extendable_Fraction* other);

        // Multiply two fractions
        void multiply(Extendable_Fraction* other);

        // Operates this element with another one
        virtual void operate(Algebra_Element* other, std::string operation);

        // Substract two fractions
        void sub(Extendable_Fraction* other);

        // Integer part
        Extendable_Int integer_part() const;

        // Normalize the fraction
        void normalize(int max_size_in_octets);
        void normalize();

        // Returns the number to a double
        double to_double() const;

        // Returns the element to a simple std::string
        virtual std::string to_mathml(Textual_Math_Settings* settings) const;
        virtual std::string to_std_string(Textual_Math_Settings* settings) const;
        std::string to_std_string_decimal(Textual_Math_Settings* settings) const;

        // Getters and setters
        inline Extendable_Int denominator() const {return a_denominator;};
        inline Extendable_Int numerator() const {return a_numerator;};

    private:
        // Numerator and denominator
        Extendable_Int a_denominator = 1;
        Extendable_Int a_numerator = 0;
    };
}

#endif // SCLS_MATH_EXTANDABLE_FRACTION
