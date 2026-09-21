//******************
//
// scls_math_algebra_double.cpp
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
// This file contains the source code of "scls_math_algebra_double.h".
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
#include "../../scls_math_directory/scls_math_algebra/scls_math_algebra_double.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

	// Most simple double constructor
	Double::Double(const Double& to_copy) : a_value(to_copy.a_value) {};
	Double::Double(double real):a_value(real){};
	Double::Double():Double(0){};

	// Clone
	void Double::algebra_clone(Algebra_Element* e) const{reinterpret_cast<Double*>(e)->a_value = a_value;};
	std::shared_ptr<Algebra_Element> Double::algebra_clone() const{std::shared_ptr<Algebra_Element>f=new_algebra_element();algebra_clone(f.get());return f;};
	std::shared_ptr<Algebra_Element> Double::new_algebra_element() const{return std::make_shared<Double>();};
	std::shared_ptr<Algebra_Element> Double::new_algebra_element(std::string content) const{return std::make_shared<Double>(Double::from_std_string(content));};

	// Type of the object
	std::string Double::algebra_type() const{return std::string("double");};

	// Operates this element with another one
	void Double::operate(Algebra_Element* other, std::string operation){
		if(operation == "+"){add(*reinterpret_cast<Double*>(other));}
		else if(operation == "*"){multiply(*reinterpret_cast<Double*>(other));}
		else if(operation == "/"){divide(*reinterpret_cast<Double*>(other));}
		else if(operation == "abs"){if(a_value < 0){a_value = -a_value;}}
		else if(operation == "cos"){a_value = std::cos(a_value);;}
		else if(operation == "exp"){a_value = std::exp(a_value);;}
		else if(operation == "ln"){a_value = std::log(a_value);}
		else if(operation == "sin"){a_value = std::sin(a_value);}
		else if(operation == "sqrt"){a_value = std::sqrt(a_value);}
		else if(operation == "tan"){a_value = std::tan(a_value);}
	}

	// Returns the absolute value of the double
	Double Double::abs() const{if(a_value >= 0){return Double(a_value);}return Double(-a_value);};
	// Returns the inverse of the double
	Double Double::opposite() const{return Double(-a_value);};
	Double Double::inverse() const {return Double(1.0 / a_value);};

	// Returns a double from a std::string
	Double Double::from_std_string(std::string content) {return Double(std::stod(content));};

	// Returns the double in integer
	long long Double::to_integer() const {return a_value;};
	// Returns the double in double
	double Double::to_double() const{return a_value;}
	double Double::to_double_ceil() const {return std::ceil(a_value);};
	double Double::to_double_floor() const {return std::floor(a_value);};
	double Double::to_double_round() const {return std::round(a_value);};

	// Returns the double to MathML
	std::string Double::to_mathml(Textual_Math_Settings* settings)const{return scls::Fraction::from_double(a_value).to_mathml(settings);};
	// Returns the double to std::string, in the double redaction
	std::string Double::to_std_string_fraction(Textual_Math_Settings* settings) const {return scls::Fraction::from_double(a_value).to_std_string_fraction(settings);};
	std::string Double::to_std_string(unsigned int max_number_size, Textual_Math_Settings* settings) const {std::string from_fraction = to_std_string_fraction(settings);if(from_fraction.size() <= max_number_size) return from_fraction;return format_number_to_text(a_value);};
	std::string Double::to_std_string(Textual_Math_Settings* settings) const {return to_std_string(-1, settings);};

    //*********
    // Operator methods
    //*********

	// Function to do operations with doubles
	// Adds an another Double to this double
	void Double::add(Double obj) {a_value += obj.a_value;};
	Double Double::add_without_modification(Double obj) const {return Double(a_value + obj.a_value);};

    // Divides the double with an another double
    void Double::divide(Double obj) {a_value /= obj.a_value;};
    Double Double::divide_without_modification(Double obj) const { return Double(a_value / obj.a_value); };

    // Returns if this double is equal to another
    bool Double::equal(Double obj) const {return obj.a_value == a_value;};

    // Multiplies the double with an another Fraction
    void Double::multiply(Double obj) { a_value *= obj.a_value;};
    Double Double::multiply_without_modification(Double obj) const { return Double(a_value * obj.a_value); };

    // Returns the square root of the double
    Double Double::sqrt() {return Double(std::sqrt(a_value));};

    // Substracts an another Fraction to this fraction
	void Double::substract(Double obj) {a_value -= obj.a_value;};
	Double Double::substract_without_modification(Double obj) const {return Double(a_value - obj.a_value);};

	// Operator overloading with doubles
	// Decrement operator
	Double& Double::operator--(int) {a_value--; return *this; }
	// Divisor operator
	Double Double::operator/(Double obj) const { return divide_without_modification(obj); };
	// Divisor operator assignment
	Double& Double::operator/=(Double obj) { divide(obj); return *this; };
	// Equality operator
	bool Double::operator==(Double obj) const { return equal(obj); }
	// Greater or equal than than operator
	bool Double::operator>=(Double r) const { return equal(r) || a_value > r.a_value; }
	// Greater than than operator
	bool Double::operator>(Double r) const { return a_value > r.a_value; }
	// Increment operator
	Double& Double::operator++(int) { a_value++; return *this; }
	// Lesser than than operator
	bool Double::operator<(Double r) const { return a_value < r.a_value; }
	// Lesser or equal than than operator
	bool Double::operator<=(Double r) const { return equal(r) || a_value < r.a_value; }
	// Arithmetic operator
	Double Double::operator-(Double obj) const { return substract_without_modification(obj); };
	Double& Double::operator-=(Double obj) { substract(obj); return *this; }
	Double Double::operator*(Double obj) const { return multiply_without_modification(obj); };
	Double& Double::operator*=(Double obj) { multiply(obj); return *this; };
	Double Double::operator+(Double obj) const { return add_without_modification(obj); };
	Double& Double::operator+=(Double obj) { add(obj); return *this; }
}
