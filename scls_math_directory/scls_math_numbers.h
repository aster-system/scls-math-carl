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

#ifndef SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION
#define SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION 10000000000
#endif // SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION

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

    class Fraction {
	    // Class representating a fraction
    public:
        //*********
        //
        // Fraction simple methods
        //
        //*********

        // Most simple fraction constructor
        Fraction(double real){if(real == static_cast<long long>(real)){a_numerator=real;}else{a_numerator=real*SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION;a_denominator=SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION;} normalize();};
        // Simple fraction constructor
        Fraction(long long numerator, long long denominator) : a_denominator(denominator), a_numerator(numerator) {normalize();}
        // Fraction copy constructor
        Fraction(const Fraction& to_copy) : a_denominator(to_copy.a_denominator), a_numerator(to_copy.a_numerator) {a_normalized = true;};

        // Returns a fraction from a double
        static Fraction from_double(double result) {
            long long result_in_long = static_cast<long long>(result);
            double after_decimal_point = static_cast<double>(result - static_cast<double>(result_in_long));
            if(after_decimal_point == 0) return Fraction(result_in_long, 1);
            long long after_decimal_point_in_long = static_cast<long long>(after_decimal_point * 100000);
            return Fraction(result_in_long, 1) + Fraction(after_decimal_point_in_long, 100000);
        };
        // Returns a fraction from a std::string
        static Fraction from_std_string(std::string content) {
            std::vector<std::string> cutted = cut_string(content, "/");
            if(cutted.size() <= 0) return Fraction(0);
            else if(cutted.size() == 1) return Fraction(std::stoi(cutted[0]));
            else return Fraction(std::stoi(cutted[0]), std::stoi(cutted[1]));
        };
        // Normalize the fraction
        void normalize() {
            if(a_normalized) return;

            const long long base_denominator = a_denominator; const long long base_numerator = a_numerator;
            if(a_denominator < 0) a_denominator = -a_denominator;
            if(a_numerator < 0) a_numerator = -a_numerator;

            // Apply the Euclid algorithm
            if(a_numerator != 0) {
                while(a_denominator != 0) {
                    long long t = a_denominator;
                    a_denominator = a_numerator % a_denominator;
                    a_numerator = t;
                }
                a_denominator = base_denominator / a_numerator;
                a_numerator = base_numerator / a_numerator;
            }
            else {
                a_denominator = 1;
            }
            if(a_denominator < 0) a_denominator = -a_denominator;
            if(a_numerator < 0) a_numerator = -a_numerator;

            // Apply the sign
            if((base_denominator < 0 && base_numerator > 0) || (base_denominator > 0 && base_numerator < 0)) a_numerator = -a_numerator;
            a_normalized = true;
        };
        // Sets this fraction as a double
        void set_from_double(double result) {
            Fraction new_value = from_double(result);
            a_denominator = new_value.a_denominator;
            a_numerator = new_value.a_numerator;
        };
        // Returns the fraction in int
        inline long long to_int() const {if(a_denominator == 0) return 0; return a_numerator / a_denominator;};
        // Returns the fraction in double
        inline double to_double() const {if(a_denominator == 0) return 0; return static_cast<double>(a_numerator) / static_cast<double>(a_denominator);};
        // Returns the fraction to std::string, in the fraction redaction
        inline std::string to_std_string_fraction() const {if(denominator() == 1) return std::to_string(numerator()); return std::to_string(numerator()) + "/" + std::to_string(denominator());};
        inline std::string to_std_string(unsigned int max_number_size) const {
            std::string from_fraction = to_std_string_fraction();
            if(from_fraction.size() <= max_number_size) return from_fraction;
            return format_number_to_text(to_double());
        };
        inline std::string to_std_string() const {return to_std_string(-1);};

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
        void _add(Fraction const& obj) {
            long long first_numerator = obj.a_numerator * a_denominator;
            long long second_numerator = a_numerator * obj.a_denominator;
            a_denominator = obj.a_denominator * a_denominator;
            a_numerator = first_numerator + second_numerator;
            a_normalized = false;
            normalize();
        };
        // Returns the adding of this fraction and another function
        Fraction _add_without_modification(Fraction const& obj) const {
            long long first_numerator = obj.a_numerator * a_denominator;
            long long second_numerator = a_numerator * obj.a_denominator;
            long long denominateur = obj.a_denominator * a_denominator;

            Fraction new_fraction = Fraction(first_numerator + second_numerator, denominateur);
            return new_fraction;
        };
        // Divides the fraction with an another fraction
        void _divide(Fraction const& obj) { _multiply(Fraction(obj.a_denominator, obj.a_numerator)); a_normalized = false; };
        // Divides the fraction with an another fraction
        Fraction _divide_without_modification(Fraction const& obj) const { return _multiply_without_modification(Fraction(obj.a_denominator, obj.a_numerator)); };
        // Returns if this fraction is equal to another
        bool _equal(Fraction const& obj) const {return obj.a_numerator * a_denominator == a_numerator * obj.a_denominator;};
         // Returns if this fraction is equal to an int
        bool _equal(int const& obj) const {return a_numerator == obj && a_denominator == 1;};
        // Multiplies the fraction with an another Fraction
        void _multiply(Fraction const& obj) { a_numerator *= obj.a_numerator; a_denominator *= obj.a_denominator; a_normalized = false; };
        // Multiplies the fraction with an another Fraction
        Fraction _multiply_without_modification(Fraction const& obj) const { return Fraction(a_numerator * obj.a_numerator, a_denominator * obj.a_denominator); };
        // Multiplies the fraction with a double
        Fraction _multiply_without_modification(double obj) const { return Fraction(a_numerator * obj, a_denominator); };
        // Multiplies the fraction with an unsigned int
        Fraction _multiply_without_modification(int obj) const { return Fraction(a_numerator * obj, a_denominator); };
        // Multiplies the fraction with an unsigned int
        Fraction _multiply_without_modification(unsigned int obj) const { return Fraction(a_numerator * obj, a_denominator); };
        // Substracts an another Fraction to this fraction
        void _substract(Fraction const& obj) {
            long long first_numerator = obj.a_numerator * a_denominator;
            long long second_numerator = a_numerator * obj.a_denominator;
            a_denominator = obj.a_denominator * a_denominator;
            a_numerator = second_numerator - first_numerator;
            a_normalized = false;
            normalize();
        };
        // Returns the substracting of this fraction and another function
        Fraction _substract_without_modification(Fraction const& obj) const {
            long long first_numerator = obj.a_numerator * a_denominator;
            long long second_numerator = a_numerator * obj.a_denominator;
            long long denominateur = obj.a_denominator * a_denominator;

            Fraction new_fraction = Fraction(second_numerator - first_numerator, denominateur);
            return new_fraction;
        };
        // Returns the square root of the fraction
        Fraction sqrt() {return Fraction(std::sqrt(numerator()) * SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION, std::sqrt(denominator()) * SCLS_MATH_NUMBER_DOUBLE_TO_FRACTION);};

        // Operator overloading with fractions
        // Decrement operator
        Fraction& operator--(int) { _substract(Fraction(1, 1)); return *this; }
        // Divisor operator
        Fraction operator/(Fraction obj) const { return _divide_without_modification(obj); };
        // Divisor operator assignment
        Fraction& operator/=(Fraction obj) { _divide(obj); return *this; };
        // Equality operator
        bool operator==(Fraction obj) const { return _equal(obj); }
        // Greater or equal than than operator
        bool operator>=(Fraction r) const { return _equal(r) || to_double() > r.to_double(); }
        // Greater than than operator
        bool operator>(Fraction r) const { return to_double() > r.to_double(); }
        // Increment operator
        Fraction& operator++(int) { _add(Fraction(1, 1)); return *this; }
        // Lesser than than operator
        bool operator<(Fraction r) const { return to_double() < r.to_double(); }
        // Lesser or equal than than operator
        bool operator<=(Fraction r) const { return _equal(r) || to_double() < r.to_double(); }
        // Minus operator
        Fraction operator-(Fraction obj) const { return _substract_without_modification(obj); };
        // Minus operator assignment
        Fraction& operator-=(Fraction obj) { _substract(obj); return *this; }
        // Multiplciation operator
        Fraction operator*(Fraction obj) const { return _multiply_without_modification(obj); };
        // Multiplication operator assignment
        Fraction& operator*=(Fraction obj) { _multiply(obj); return *this; };
        // Plus operator
        Fraction operator+(Fraction obj) const { return _add_without_modification(obj); };
        // Plus operator assignment
        Fraction& operator+=(Fraction obj) { _add(obj); return *this; }
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

	// Multiplciation operator
    extern Fraction operator*(int obj_1, Fraction obj);
    // Minus operator
    extern Fraction operator-(int obj_1, Fraction obj);
	// Stream operator overloading (indev)
	extern std::ostream& operator<<(std::ostream& os, const Fraction& obj);
}

//*********
//
// The Complex part
//
//*********

// The namespace "scls" is used to simplify the all.
namespace scls {

    class Complex {
	    // Class representating a fraction
    public:
        //*********
        //
        // Complex simple methods
        //
        //*********

        // Dimple fraction constructor
        Complex(Fraction real_part, Fraction imaginary) : a_imaginary(imaginary), a_real(real_part) {};
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
        inline std::string to_std_string_simple(unsigned int max_number_size) const {
            std::string to_return = "";
            if(real() != 0) {
                to_return += real().to_std_string(max_number_size) + " ";
            }
            if(imaginary() != 0) {
                if(imaginary() > 0) {
                    to_return += "+ " + imaginary().to_std_string(max_number_size) + "i";
                }
                else {
                    to_return += "- " + (imaginary() * -1).to_std_string(max_number_size) + "i";
                }
            }
            while(to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
            if(to_return == "") to_return = "0";
            return to_return;
        };
        inline std::string to_std_string_simple() const {return to_std_string_simple(-1);};

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
	inline Complex string_to_complex(std::string source) {
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

	// Multiplciation operator
    extern Complex operator*(int obj_1, Complex obj);
	// Stream operator overloading (indev)
    extern std::ostream& operator<<(std::ostream& os, const Complex& obj);
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

        // Getters and setters
        inline Fraction value() const {return a_value;};

    private:
        // Finite value of the limite
        Fraction a_value;
    };
}

#endif // SCLS_MATH_NUMBERS
