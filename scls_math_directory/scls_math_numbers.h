//******************
//
// scls_math_numbers.h
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
// This file contains the every specific numbers class (one by number set).
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

#ifndef SCLS_MATH_NUMBERS
#define SCLS_MATH_NUMBERS

#ifndef SCLS_MATH_NUMBER_LIMIT_ERROR_IPI
#define SCLS_MATH_NUMBER_LIMIT_ERROR_IPI 120
#endif // SCLS_MATH_NUMBER_LIMIT_ERROR_IPI
#ifndef SCLS_MATH_NUMBER_LIMIT_SPECIAL_MI
#define SCLS_MATH_NUMBER_LIMIT_SPECIAL_MI -1
#endif // SCLS_MATH_NUMBER_LIMIT_SPECIAL_MI
#ifndef SCLS_MATH_NUMBER_LIMIT_SPECIAL_MZ
#define SCLS_MATH_NUMBER_LIMIT_SPECIAL_MZ -2
#endif // SCLS_MATH_NUMBER_LIMIT_SPECIAL_MZ
#ifndef SCLS_MATH_NUMBER_LIMIT_SPECIAL_PI
#define SCLS_MATH_NUMBER_LIMIT_SPECIAL_PI 1
#endif // SCLS_MATH_NUMBER_LIMIT_SPECIAL_PI
#ifndef SCLS_MATH_NUMBER_LIMIT_SPECIAL_PZ
#define SCLS_MATH_NUMBER_LIMIT_SPECIAL_PZ 2
#endif // SCLS_MATH_NUMBER_LIMIT_SPECIAL_PZ
#ifndef SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION
#define SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION 1000000.0
#endif // SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION

#define SCLS_MATH_NUMBER_NORMALIZE_VALUE 8

//*********
//
// Useful datas for textual representation of numbers
//
//*********

namespace scls {

    //*********
    //
    // The Textual_Math_Settings class
    //
    //*********

    class Textual_Math_Settings {
        // Class representating settings to textually represent mathematical objects
    public:

        // Textual_Math_Settings constructor
        Textual_Math_Settings();

        // Getters and setters
        inline bool introduction_in_mathml() const {return a_introduction_in_mathml;};
    private:
        // If the introduction should be in mathml or not
        bool a_introduction_in_mathml = true;
    };
}

//*********
//
// The Rational part
//
//*********

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
    //
    // The Fraction class
    //
    //*********

    class __Fraction_Base {
	    // Class representating the base of a fraction
    public:
        //*********
        //
        // __Fraction_Base simple methods
        //
        //*********

        // Most simple fraction constructor
        __Fraction_Base(double real){if(real == round(real)){a_numerator=real;}else{a_numerator=real*SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION;a_denominator=SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION;normalize();}};
        __Fraction_Base():__Fraction_Base(0){};
        // Simple fraction constructor
        __Fraction_Base(long long numerator, long long denominator) : a_denominator(denominator), a_numerator(numerator) {if(a_denominator!=1&&a_numerator!=1){normalize();}}
        // Fraction copy constructor
        __Fraction_Base(const __Fraction_Base& to_copy) : a_denominator(to_copy.a_denominator), a_normalized(to_copy.a_normalized), a_numerator(to_copy.a_numerator) {normalize();};

        // Returns a fraction from a double
        static __Fraction_Base from_double(double result);
        // Returns a fraction from a std::string
        static __Fraction_Base from_std_string(std::string content);
        // Returns the inverse of the fraction
        inline __Fraction_Base inverse() const {return __Fraction_Base(a_denominator, a_numerator);};
        // Normalize the fraction
        void normalize_force();
        void __normalize();
        inline void normalize(){if(SCLS_MATH_NUMBER_NORMALIZE_VALUE > 0){normalize(SCLS_MATH_NUMBER_NORMALIZE_VALUE);}else{__normalize();}};
        inline void normalize(int limit){if(std::abs(a_denominator) > std::pow(10, limit) && std::abs(a_numerator) > std::pow(10, limit)){int value = std::ceil(std::log10(std::max(std::abs(a_denominator), std::abs(a_numerator)))) - limit;a_denominator/=std::pow(10, value);a_numerator/=std::pow(10, value);}normalize_force();};
        inline __Fraction_Base normalized()const{__Fraction_Base other=*this;other.normalize();return other;};
        inline __Fraction_Base normalized(int limit)const{__Fraction_Base other=*this;other.normalize(limit);return other;};
        // Sets this fraction as a double
        inline void set_from_double(double result) {__Fraction_Base new_value = from_double(result);a_denominator = new_value.a_denominator;a_numerator = new_value.a_numerator;};
        // Returns the fraction in int
        inline long long to_int() const {if(a_denominator == 0) return 0; return a_numerator / a_denominator;};
        // Returns the fraction in double
        inline double to_double() const {if(a_denominator == 0) return 0; return static_cast<double>(a_numerator) / static_cast<double>(a_denominator);};
        inline double to_double_ceil() const {return std::ceil(to_double());};
        inline double to_double_floor() const {return std::floor(to_double());};
        inline double to_double_round() const {return std::round(to_double());};
        // Returns the fraction to MathML
        inline std::string to_mathml(){normalize();if(a_denominator == 1){return std::string("<math><mi>") + std::to_string(numerator()) + std::string("</mi></math>");}return std::string("<math><frac><mrow>") + std::to_string(a_numerator) + "</mrow><mrow>" + std::to_string(a_denominator) + std::string("</mrow></frac></math>");};
        // Returns the fraction to std::string, in the fraction redaction
        inline std::string to_std_string_fraction(Textual_Math_Settings* settings) const {if(denominator() == 1) return std::to_string(numerator()); return std::to_string(numerator()) + "/" + std::to_string(denominator());};
        inline std::string to_std_string(unsigned int max_number_size, Textual_Math_Settings* settings) const {std::string from_fraction = to_std_string_fraction(settings);if(from_fraction.size() <= max_number_size) return from_fraction;return format_number_to_text(to_double());};
        inline std::string to_std_string(Textual_Math_Settings* settings) const {return to_std_string(-1, settings);};

        // Getters and setter
        inline long long denominator() const {return a_denominator;};
        inline long long numerator() const {return a_numerator;};

        //*********
        //
        // Operator methods
        //
        //*********

        // Function to do operations with fractions
        // Adds an another Fraction to this fraction
        void _add(__Fraction_Base obj);
        __Fraction_Base _add_without_modification(__Fraction_Base obj) const;

        // Divides the fraction with an another fraction
        inline void _divide(__Fraction_Base obj) { _multiply(__Fraction_Base(obj.a_denominator, obj.a_numerator)); a_normalized = false;};
        inline __Fraction_Base _divide_without_modification(__Fraction_Base obj) const { return _multiply_without_modification(__Fraction_Base(obj.a_denominator, obj.a_numerator)); };

        // Returns if this fraction is equal to another
        inline bool _equal(__Fraction_Base obj) const {return obj.a_numerator * a_denominator == a_numerator * obj.a_denominator;};
        inline bool _equal(int obj) const {return a_numerator == obj && a_denominator == 1;};

        // Multiplies the fraction with an another Fraction
        void _multiply(__Fraction_Base obj) { a_numerator *= obj.a_numerator; a_denominator *= obj.a_denominator; a_normalized = false; };
        inline __Fraction_Base _multiply_without_modification(__Fraction_Base obj) const { return __Fraction_Base(a_numerator * obj.a_numerator, a_denominator * obj.a_denominator); };
        inline __Fraction_Base _multiply_without_modification(double obj) const { return __Fraction_Base(a_numerator * obj, a_denominator); };
        inline __Fraction_Base _multiply_without_modification(int obj) const { return __Fraction_Base(a_numerator * obj, a_denominator); };
        inline __Fraction_Base _multiply_without_modification(unsigned int obj) const { return __Fraction_Base(a_numerator * obj, a_denominator); };

        // Substracts an another Fraction to this fraction
        void _substract(__Fraction_Base obj);
        __Fraction_Base _substract_without_modification(__Fraction_Base obj) const;
        // Returns the square root of the fraction
        inline __Fraction_Base sqrt() {return __Fraction_Base(std::sqrt(numerator()) * SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION, std::sqrt(denominator()) * SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION);};

        // Operator overloading with fractions
        // Decrement operator
        __Fraction_Base& operator--(int) { _substract(__Fraction_Base(1, 1)); return *this; }
        // Divisor operator
        __Fraction_Base operator/(__Fraction_Base obj) const { return _divide_without_modification(obj); };
        // Divisor operator assignment
        __Fraction_Base& operator/=(__Fraction_Base obj) { _divide(obj); return *this; };
        // Equality operator
        bool operator==(__Fraction_Base obj) const { return _equal(obj); }
        // Greater or equal than than operator
        bool operator>=(__Fraction_Base r) const { return _equal(r) || to_double() > r.to_double(); }
        // Greater than than operator
        bool operator>(__Fraction_Base r) const { return to_double() > r.to_double(); }
        // Increment operator
        __Fraction_Base& operator++(int) { _add(__Fraction_Base(1, 1)); return *this; }
        // Lesser than than operator
        bool operator<(__Fraction_Base r) const { return to_double() < r.to_double(); }
        // Lesser or equal than than operator
        bool operator<=(__Fraction_Base r) const { return _equal(r) || to_double() < r.to_double(); }
        // Arithmetic operator
        __Fraction_Base operator-(__Fraction_Base obj) const { return _substract_without_modification(obj); };
        __Fraction_Base& operator-=(__Fraction_Base obj) { _substract(obj); return *this; }
        __Fraction_Base operator*(__Fraction_Base obj) const { return _multiply_without_modification(obj); };
        __Fraction_Base& operator*=(__Fraction_Base obj) { _multiply(obj); return *this; };
        __Fraction_Base operator+(__Fraction_Base obj) const { return _add_without_modification(obj); };
        __Fraction_Base& operator+=(__Fraction_Base obj) { _add(obj); return *this; }
    private:
        //*********
        //
        // Fraction simple attributes
        //
        //*********

        // Denominator of the fraction
        long long a_denominator = 1;
        // If the fraction has been normalized or not
        bool a_normalized = false;
        // Numerator of the fraction
        long long a_numerator = 0;

	};

    class Fraction : public __Fraction_Base {
	    // Class representating a precise (but slow and unstable) fraction
    public:
        //*********
        //
        // Fraction simple methods
        //
        //*********

        // Most simple fraction constructor
        Fraction(double real):__Fraction_Base(real){};
        Fraction():__Fraction_Base(0){};
        // Simple fraction constructor
        Fraction(long long numerator, long long denominator) : __Fraction_Base(numerator, denominator) {}
        // Fraction copy constructor
        Fraction(const __Fraction_Base& to_copy) : __Fraction_Base(to_copy) {};

        // Arithmetic operator
        Fraction operator-(__Fraction_Base obj) const { return _substract_without_modification(obj); };
        Fraction& operator-=(__Fraction_Base obj) { _substract(obj); return *this; }
        Fraction operator*(__Fraction_Base obj) const { return _multiply_without_modification(obj); };
        Fraction& operator*=(__Fraction_Base obj) { _multiply(obj); return *this; };
        Fraction operator+(__Fraction_Base obj) const { return _add_without_modification(obj); };
        Fraction& operator+=(__Fraction_Base obj) { _add(obj); return *this; }
	};

	// Multiplication operator
    __Fraction_Base operator*(int obj_1, __Fraction_Base obj);
    Fraction operator*(int obj_1, Fraction obj);
    // Minus operator
    __Fraction_Base operator-(int obj_1, __Fraction_Base obj);
    Fraction operator-(int obj_1, Fraction obj);
	// Stream operator overloading (indev)
    std::ostream& operator<<(std::ostream& os, const Fraction& obj);

    // Returns a random fraction
    scls::Fraction random_fraction(scls::Fraction min_value, scls::Fraction max_value, int precision);
    scls::Fraction random_fraction(scls::Fraction min_value, scls::Fraction max_value);
    // Function to sort a std::vector of fraction
    void remove_duplication_sorted_fractions(std::vector<Fraction>& fractions);
    void sort_fractions(std::vector<Fraction>& fractions);
}

//*********
//
// The Complex part
//
//*********

// The namespace "scls" is used to simplify the all.
namespace scls {

    class Complex {
	    // Class representating a complex number
    public:
        //*********
        //
        // Complex simple methods
        //
        //*********

        // Simple fraction constructor
        Complex(Fraction real_part, Fraction imaginary) : a_imaginary(imaginary), a_real(real_part) {};
        Complex(long long real_part, long long imaginary_part) : Complex(Fraction(real_part), Fraction(imaginary_part)) {};
        Complex(long long real_part, Fraction imaginary_part) : Complex(Fraction(real_part), imaginary_part) {};
        Complex(Fraction real_part, long long imaginary_part) : Complex(real_part, Fraction(imaginary_part)) {};
        // Most simple fraction constructor
        Complex(Fraction real_part) : Complex(real_part, Fraction(0)) {};
        Complex(double real_part) : Complex(Fraction(real_part)) {};
        // Complex copy constructor
        Complex(const Complex& to_copy) : Complex(to_copy.real(), to_copy.imaginary()) {};

        // Returns the module of the complex
        inline scls::Fraction module() const{return a_real * a_real + a_imaginary * a_imaginary; };
        // Normalize the number
        inline void normalize(int limit){a_imaginary.normalize(limit);a_real.normalize(limit);};

        // Getters and setter
        inline Fraction imaginary() const {return a_imaginary;};
        inline Fraction real() const {return a_real;};
        inline void set_imaginary(Fraction new_imaginary) {a_imaginary=new_imaginary;};
        inline void set_real(Fraction new_real) {a_real=new_real;};

        // Returns the Complex to a simple std::string
        std::string to_std_string_simple(unsigned int max_number_size, Textual_Math_Settings* settings) const;
        inline std::string to_std_string_simple(Textual_Math_Settings* settings) const {return to_std_string_simple(-1, settings);};

        //*********
        //
        // Operator methods
        //
        //*********

        // Returns the conjugate of this Complex
        inline Complex conjugate() const {return Complex(a_real, a_imaginary * -1);};

        // Function to do operations with Complex
        // Adds an another Complex to this Complex
        void _add(Complex const& obj) {a_imaginary += obj.imaginary();a_real += obj.real();};
        Complex _add_without_modification(Complex const& obj) const {Complex new_complex = Complex(real() + obj.real(), imaginary() + obj.imaginary());return new_complex;};

        // Divides the Complex with a Complex
        void _divide(Complex const& obj);
        Complex _divide_without_modification(Complex const& obj) const;

        // Returns if this Complex is equal to another
        bool _equal(Complex const& obj) const {return obj.real() == real() && obj.imaginary() == imaginary();};

        // Multiplies the Complex with a Complex
        void _multiply(Complex const& obj);
        inline Complex _multiply_without_modification(Complex const& obj) const {return Complex(real() * obj.real() - (imaginary() * obj.imaginary()), real() * obj.imaginary() + imaginary() * obj.real());};

        // Multiplies the Complex with a double
        Complex _multiply_without_modification(Fraction const& obj) const { return Complex(real() * obj, imaginary() * obj); };
        Complex _multiply_without_modification(int const& obj) const { return Complex(real() * obj, imaginary() * obj); };
        Complex _multiply_without_modification(unsigned int const& obj) const { return Complex(real() * obj, imaginary() * obj); };

        // Substracts an another Complex to this Complex
        void _substract(Complex const& obj) {a_imaginary = imaginary() - obj.imaginary();a_real = real() - obj.real();};
        Complex _substract_without_modification(Complex const& obj) const {Complex new_complex = Complex(real() - obj.real(), imaginary() - obj.imaginary());return new_complex;};

        // Operator overloading with int
        // Equality operator
        bool operator==(int obj) { return _equal(obj); }
        // Multiplication operator
        Complex operator*(int obj) const { return _multiply_without_modification(obj); }
        // Multiplication operator
        Complex operator*(unsigned int obj) const { return _multiply_without_modification(obj); }

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

	// Returns a complex from a std::string (indev)
    Complex string_to_complex(std::string source);

	// Multiplciation operator
    Complex operator*(int obj_1, Complex obj);
	// Stream operator overloading (indev)
    std::ostream& operator<<(std::ostream& os, const Complex& obj);
}

//*********
//
// The Point_2D part
//
//*********

namespace scls {

    class Point_2D {
        // Class representating a point in 2D
    public:

        // Point_2D constructor
        Point_2D(scls::Fraction x, scls::Fraction y):a_x(x),a_y(y){};

        // Getters and setters
        inline scls::Fraction x() const {return a_x;}
        inline scls::Fraction y() const {return a_y;};
    private:
        // X / Y position of the point
        scls::Fraction a_x = 0; scls::Fraction a_y = 0;
    };

}

//*********
//
// The specific numbers part
//
//*********

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
    //
    // The Limit part
    //
    //*********

    class Limit {
        // Class representating the limit of a formula
    public:

        // Limit constructor
        Limit(Fraction value):a_value(value){};
        Limit():Limit(Fraction(1)){};

        // Returns the limit in a std::string
        std::string to_std_string(Textual_Math_Settings* settings) const;

        // Getters and setters
        inline bool is_error_ipi() const {return a_special_value == SCLS_MATH_NUMBER_LIMIT_ERROR_IPI;};
        inline bool is_mi() const {return a_special_value == SCLS_MATH_NUMBER_LIMIT_SPECIAL_MI;};
        inline bool is_mz() const {return a_special_value == SCLS_MATH_NUMBER_LIMIT_SPECIAL_MZ;};
        inline bool is_pi() const {return a_special_value == SCLS_MATH_NUMBER_LIMIT_SPECIAL_PI;};
        inline bool is_pz() const {return a_special_value == SCLS_MATH_NUMBER_LIMIT_SPECIAL_PZ;};
        inline bool is_special() const {return a_special_value != 0;};
        inline void set_mi() {a_special_value = SCLS_MATH_NUMBER_LIMIT_SPECIAL_MI;};
        inline void set_mz() {a_special_value = SCLS_MATH_NUMBER_LIMIT_SPECIAL_MZ;};
        inline void set_pi() {a_special_value = SCLS_MATH_NUMBER_LIMIT_SPECIAL_PI;};
        inline void set_pz() {a_special_value = SCLS_MATH_NUMBER_LIMIT_SPECIAL_PZ;};
        inline void set_special_value(char new_special_value) {a_special_value = new_special_value;};
        inline char special_value() const {return a_special_value;};
        inline Fraction value() const {return a_value;};

    private:
        // Special value of the limit
        char a_special_value = 0;
        // Finite value of the limit
        Fraction a_value;
    };
}

#endif // SCLS_MATH_NUMBERS
