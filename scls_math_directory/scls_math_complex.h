//******************
//
// scls_math_complex.h
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
// This file contains the "Complex" class.
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

#ifndef SCLS_MATH_COMPLEX
#define SCLS_MATH_COMPLEX

#include "scls_math_fraction.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The Complex class
	//
	//*********

	class Complex {
	    // Class representating a fraction
    public:
        //*********
        //
        // Complex simple methods
        //
        //*********

        // Dimple fraction constructor
        Complex(Fraction real_part, Fraction imaginary) : a_real(real_part), a_imaginary(imaginary) {};
        Complex(long long real_part, long long imaginary_part) : Complex(Fraction(real_part), Fraction(imaginary_part)) {};
        Complex(long long real_part, Fraction imaginary_part) : Complex(Fraction(real_part), imaginary_part) {};
        Complex(Fraction real_part, long long imaginary_part) : Complex(real_part, Fraction(imaginary_part)) {};
        // Most simple fraction constructor
        Complex(Fraction real_part) : Complex(real_part, Fraction(0)) {};
        Complex(long long real_part) : Complex(Fraction(real_part)) {};
        // Complex copy constructor
        Complex(const Complex& to_copy) : Complex(to_copy.real(), to_copy.imaginary()) {};

        // Getters and setter
        inline Fraction imaginary() const {return a_imaginary;};
        inline Fraction real() const {return a_real;};
        inline void set_imaginary(Fraction new_imaginary) {a_imaginary=new_imaginary;};
        inline void set_real(Fraction new_real) {a_real=new_real;};

        // Returns the Complex to a simple std::string
        inline std::string to_std_string_simple() const {
            std::string to_return = "";
            if(real() != 0) {
                to_return += real().to_std_string_fraction() + " ";
            }
            if(imaginary() != 0) {
                if(imaginary() > 0) {
                    to_return += "+ " + imaginary().to_std_string_fraction() + "i";
                }
                else {
                    to_return += "- " + (imaginary() * -1).to_std_string_fraction() + "i";
                }
            }
            while(to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
            if(to_return == "") to_return = "0";
            return to_return;
        };

        //*********
        //
        // Operator methods
        //
        //*********

        // Returns the conjugate of this Complex
        Complex conjugate() const {return Complex(a_real, a_imaginary * -1);};

        // Function to do operations with Complex
        // Adds an another Complex to this Complex
        void _add(Complex const& obj) {
            a_imaginary += obj.imaginary();
            a_real += obj.real();
        };
        // Returns the adding of this Complex and another Complex
        Complex _add_without_modification(Complex const& obj) const {
            Complex new_complex = Complex(real() + obj.real(), imaginary() + obj.imaginary());
            return new_complex;
        };
        // Divides the Complex with a Complex
        void _divide(Complex const& obj) {
            _multiply(obj.conjugate());
            Complex real_denominateur = obj * obj.conjugate();
            a_imaginary = a_imaginary / real_denominateur.imaginary();
            a_real = a_real / real_denominateur.real();
        };
        // Divides the Complex with a Complex
        Complex _divide_without_modification(Complex const& obj) const {
            Complex new_complex = Complex(a_real, a_imaginary);
            new_complex._multiply(obj.conjugate());
            Complex real_denominateur = obj * obj.conjugate();
            new_complex.a_imaginary = new_complex.a_imaginary / real_denominateur.real();
            new_complex.a_real = new_complex.a_real / real_denominateur.real();
            return new_complex;
        };
        // Returns if this Complex is equal to another
        bool _equal(Complex const& obj) const {return obj.real() == real() && obj.imaginary() == imaginary();};
        // Multiplies the Complex with a Complex
        void _multiply(Complex const& obj) {
            Fraction new_imaginary = real() * obj.imaginary() + imaginary() * obj.real();
            Fraction new_real = real() * obj.real() - (imaginary() * obj.imaginary());
            a_imaginary = new_imaginary;
            a_real = new_real;
        };
        // Multiplies the Complex with a Complex
        Complex _multiply_without_modification(Complex const& obj) const {
            return Complex(real() * obj.real() - (imaginary() * obj.imaginary()), real() * obj.imaginary() + imaginary() * obj.real());
        };

        // Multiplies the Complex with a double
        Complex _multiply_without_modification(Fraction const& obj) const { return Complex(real() * obj, imaginary() * obj); };
        // Multiplies the Complex with an unsigned int
        Complex _multiply_without_modification(int const& obj) const { return Complex(real() * obj, imaginary() * obj); };
        // Multiplies the Complex with an unsigned int
        Complex _multiply_without_modification(unsigned int const& obj) const { return Complex(real() * obj, imaginary() * obj); };
        // Substracts an another Complex to this Complex
        void _substract(Complex const& obj) {
            a_imaginary = imaginary() - obj.imaginary();
            a_real = real() - obj.real();
        };
        // Returns the substracting of this Complex and another Complex
        Complex _substract_without_modification(Complex const& obj) const {
            Complex new_complex = Complex(real() - obj.real(), imaginary() - obj.imaginary());
            return new_complex;
        };

        // Operator overloading with int
        // Equality operator
        bool operator==(const int& obj) { return _equal(obj); }
        // Multiplication operator
        Complex operator*(int const& obj) const { return _multiply_without_modification(obj); }
        // Multiplication operator
        Complex operator*(unsigned int const& obj) const { return _multiply_without_modification(obj); }

        // Operator overloading with Fraction
        // Minus operator
        Complex operator-(Fraction const& obj) { return _substract_without_modification(Complex(obj)); }
        // Minus operator assignment
        Complex& operator-=(Fraction const& obj) { _substract(Complex(obj)); return *this; }
        // Multiplication operator
        Complex operator*(Fraction const& obj) const { return _multiply_without_modification(obj); }
        // Plus operator
        Complex operator+(Fraction const& obj) { return _add_without_modification(Complex(obj)); };
        // Plus operator assignment
        Complex& operator+=(Fraction const& obj) { _add(Complex(obj)); return *this; }

        // Operator overloading with fractions
        // Decrement operator
        Complex& operator--(int) { _substract(Fraction(1)); return *this; }
        // Divide operator
        Complex operator/(Complex const& obj) const { return _divide_without_modification(obj); };
        // Divide operator assignment
        Complex& operator/=(Complex const& obj) { _divide(obj);return *this; };
        // Equality operator
        bool operator==(const Complex& obj) const { return _equal(obj); }
        // Increment operator
        Complex& operator++(int) { _add(Fraction(1)); return *this; }
        // Minus operator
        Complex operator-(Complex const& obj) const { return _substract_without_modification(obj); };
        // Minus operator assignment
        Complex& operator-=(const Complex& obj) { _substract(obj); return *this; }
        // Multiply operator assignment
        Complex operator*(Complex const& obj) const { return _multiply_without_modification(obj); };
        // Multiply operator
        Complex& operator*=(Complex const& obj) { _multiply(obj);return *this; };
        // Plus operator
        Complex operator+(Complex const& obj) const { return _add_without_modification(obj); };
        // Plus operator assignment
        Complex& operator+=(const Complex& obj) { _add(obj); return *this; }
    private:
        //*********
        //
        // Complex simple attributes
        //
        //*********

        // Imaginary part of the complex
        Fraction a_imaginary = Fraction(0);
        // Real part of the complex
        Fraction a_real = Fraction(0);

	};

	// Returns a complex from a std::string
	static Complex string_to_complex(std::string source) {
	    Complex to_return(Fraction(0));

	    // Cut the number by + operator
	    std::vector<std::string> cutted = cut_string(source, "+");
	    for(int i = 0;i<static_cast<int>(cutted.size());i++){if(cutted[i] == ""){cutted.erase(cutted.begin()+i);i--;}}
	    if(cutted.size() > 0) {
            // First part of a complex number
            bool is_imaginary = false;
            std::string& current_string = cutted[0];
            // Special cases
            if(current_string == "i") to_return.set_imaginary(Fraction(1));
            else {
                // Analyse the part
                if(current_string[0] == 'i') {is_imaginary = true;current_string=current_string.substr(1,current_string.size()-1);}
                else if(current_string[current_string.size()-1] == 'i') {is_imaginary = true;current_string=current_string.substr(0,current_string.size()-1);}
                // Get the number
                Fraction current_number = Fraction::from_std_string(current_string);
                if(is_imaginary) {
                    to_return.set_imaginary(current_number);
                } else {
                    to_return.set_real(current_number);
                }
            }
	    }

	    return to_return;
	};

	// Stream operator overloading
    static std::ostream& operator<<(std::ostream& os, const Complex& obj) { os << "Complex : " << obj.real().to_double() << " + " << obj.imaginary().to_double() << "i" ; return os; }
}

#endif // SCLS_MATH_COMPLEX
