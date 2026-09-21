//******************
//
// scls_math_algebra_double.h
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
// This file contains some tools to use double with SCLS
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

#ifndef SCLS_MATH_ALGEBRA_DOUBLE
#define SCLS_MATH_ALGEBRA_DOUBLE

// Headers
#include "../../scls_math_root.h"
#include "../scls_math_numbers.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

	// Double
	class Double : public __Field_Element {
	    // Class representing the base of a fraction
    public:
        //*********
        //
        // Double simple methods
        //
        //*********

        // Most simple double constructor
		Double(const Double& to_copy);
		Double(double real);
		Double();

		// Clone
        virtual void algebra_clone(Algebra_Element* e) const;
        virtual std::shared_ptr<Algebra_Element> algebra_clone() const;
        virtual std::shared_ptr<Algebra_Element> new_algebra_element() const;
        virtual std::shared_ptr<Algebra_Element> new_algebra_element(std::string content) const;

        // Type of the object
        virtual std::string algebra_type() const;

        // Return if the element is a precise algebric element
        virtual bool is_addition_neutral() const {return a_value == 0;};
        virtual bool is_multiplication_neutral() const{return a_value == 1;};
        virtual bool is_multiplication_absorbing() const {return a_value == 0;};

        // Operates this element with another one
        virtual void operate(Algebra_Element* other, std::string operation);

        // Returns the absolute value of the fraction
        Double abs() const;
        // Returns a fraction from a std::string
        static Double from_std_string(std::string content);
        // Returns the inverse of the fraction
        Double opposite() const;
        Double inverse() const;
        // Returns the fraction in integer
        long long to_integer() const;
        // Returns the fraction in double
        double to_double() const;
        double to_double_ceil() const;
        double to_double_floor() const;
        double to_double_round() const;
        // Returns the fraction to MathML
        virtual std::string to_mathml(Textual_Math_Settings* settings) const;
        // Returns the fraction to std::string, in the fraction redaction
        std::string to_std_string_fraction(Textual_Math_Settings* settings) const;
        std::string to_std_string(unsigned int max_number_size, Textual_Math_Settings* settings) const;
        virtual std::string to_std_string(Textual_Math_Settings* settings) const;

        //*********
        //
        // Operator methods
        //
        //*********

        // Function to do operations with fractions
        // Adds an another Fraction to this fraction
        void add(Double obj);
        Double add_without_modification(Double obj) const;

        // Divides the fraction with an another fraction
        void divide(Double obj);
        Double divide_without_modification(Double obj) const;

        // Returns if this fraction is equal to another
        bool equal(Double obj) const;

        // Multiplies the fraction with an another Fraction
        void multiply(Double obj);
        Double multiply_without_modification(Double obj) const;

        // Returns the inverse of this element
        virtual std::shared_ptr<__Field_Element> opposite(){return std::make_shared<Double>(-a_value);};
        virtual std::shared_ptr<__Field_Element> inverse(){return std::make_shared<Double>(1.0 / a_value);};

        // Substracts an another Double to this fraction
        void substract(Double obj);
        Double substract_without_modification(Double obj) const;

        // Returns the square root of the fraction
        Double sqrt();

        // Operator overloading with fractions
        // Decrement operator
        Double& operator--(int);
        // Equality operator
        bool operator==(Double obj) const;
        // Greater or equal than than operator
        bool operator>=(Double r) const;
        // Greater than than operator
        bool operator>(Double r) const;
        // Increment operator
        Double& operator++(int);
        // Lesser than than operator
        bool operator<(Double r) const;
        // Lesser or equal than than operator
        bool operator<=(Double r) const;
        // Arithmetic operator
        Double operator-(Double obj) const;
        Double& operator-=(Double obj);
        Double operator*(Double obj) const;
        Double& operator*=(Double obj);
        Double operator+(Double obj) const;
        Double& operator+=(Double obj);
        Double operator/(Double obj) const;
        Double& operator/=(Double obj);
    private:
        //*********
        //
        // Fraction simple attributes
        //
        //*********

        // Value
        double a_value = 0;

	};
}

#endif //SCLS_MATH_ALGEBRA_DOUBLE
