//******************
//
// scls_math_fraction.h
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
// This file contains the "Fraction" class.
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

#ifndef SCLS_MATH_FRACTION
#define SCLS_MATH_FRACTION

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
        Fraction(long long numerator) : a_denominator(1), a_numerator(numerator) {};
        // Simple fraction constructor
        Fraction(long long numerator, long long denominator) : a_denominator(denominator), a_numerator(numerator) {normalize();};
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
        // Returns the fraction in double
        inline double to_double() const {if(a_denominator == 0) return 0; return static_cast<double>(a_numerator) / static_cast<double>(a_denominator);};
        // Returns the fraction to std::string, in the fraction redaction
        inline std::string to_std_string_fraction() const {if(denominator() == 1) return std::to_string(numerator()); return std::to_string(numerator()) + "/" + std::to_string(denominator());};

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
        Fraction _multiply_without_modification(double const& obj) const { return Fraction(a_numerator * obj, a_denominator); };
        // Multiplies the fraction with an unsigned int
        Fraction _multiply_without_modification(int const& obj) const { return Fraction(a_numerator * obj, a_denominator); };
        // Multiplies the fraction with an unsigned int
        Fraction _multiply_without_modification(unsigned int const& obj) const { return Fraction(a_numerator * obj, a_denominator); };
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
        Fraction sqrt() {return Fraction(std::sqrt(numerator()) * 100000, std::sqrt(denominator()) * 100000);};

        // Operator overloading with int
        // Greater operator
        bool operator>(const int& obj) { return to_double() > obj; }
        // Less operator
        bool operator<(const int& obj) { return to_double() < obj; }
        // Less or equal operator
        bool operator<=(const int& obj) { return to_double() < obj || _equal(obj); }
        // Equality operator
        bool operator==(const int& obj) { return _equal(obj); }
        // Multiplication operator
        Fraction operator*(int const& obj) const { return _multiply_without_modification(obj); }
        // Multiplication operator
        Fraction operator*(unsigned int const& obj) const { return _multiply_without_modification(obj); }

        // Operator overloading with double
        // Assignment operator assignment
        Fraction& operator=(double const& obj) { set_from_double(obj); return *this; }
        // Divisor operator
        Fraction operator/(double const& obj) { return _divide_without_modification(from_double(obj)); };
        // Greater than than operator
        bool operator>(const double& r) { return to_double() > r; }
        // Less than operator
        bool operator<(const double& r) { return to_double() < r; }
        // Minus operator
        Fraction operator-(double const& obj) { return _substract_without_modification(from_double(obj)); }
        // Minus operator assignment
        Fraction& operator-=(double const& obj) { _substract(from_double(obj)); return *this; }
        // Multiplication operator
        Fraction operator*(double const& obj) const { return _multiply_without_modification(obj); }
        // Plus operator
        Fraction operator+(double const& obj) { return _add_without_modification(from_double(obj)); };
        // Plus operator assignment
        Fraction& operator+=(double const& obj) { _add(from_double(obj)); return *this; }

        // Operator overloading with fractions
        // Decrement operator
        Fraction& operator--(int) { _substract(Fraction(1, 1)); return *this; }
        // Divisor operator
        Fraction operator/(Fraction const& obj) const { return _divide_without_modification(obj); };
        // Divisor operator assignment
        Fraction& operator/=(Fraction const& obj) { _divide(obj); return *this; };
        // Equality operator
        bool operator==(const Fraction& obj) const { return _equal(obj); }
        // Greater or equal than than operator
        bool operator>=(const Fraction& r) const { return _equal(r) || to_double() > r.to_double(); }
        // Greater than than operator
        bool operator>(const Fraction& r) const { return to_double() > r.to_double(); }
        // Increment operator
        Fraction& operator++(int) { _add(Fraction(1, 1)); return *this; }
        // Lesser than than operator
        bool operator<(const Fraction& r) const { return to_double() < r.to_double(); }
        // Minus operator
        Fraction operator-(Fraction const& obj) const { return _substract_without_modification(obj); };
        // Minus operator assignment
        Fraction& operator-=(const Fraction& obj) { _substract(obj); return *this; }
        // Multiplciation operator
        Fraction operator*(Fraction const& obj) const { return _multiply_without_modification(obj); };
        // Multiplication operator assignment
        Fraction& operator*=(Fraction const& obj) { _multiply(obj); return *this; };
        // Plus operator
        Fraction operator+(Fraction const& obj) const { return _add_without_modification(obj); };
        // Plus operator assignment
        Fraction& operator+=(const Fraction& obj) { _add(obj); return *this; }
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

	// Stream operator overloading
    static std::ostream& operator<<(std::ostream& os, const Fraction& obj) { os << "Fraction : " << obj.numerator() << " / " << obj.denominator() << " = " << obj.to_double(); return os; }
}

#endif // SCLS_MATH_FRACTION
