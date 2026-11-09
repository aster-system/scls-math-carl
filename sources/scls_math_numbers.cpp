//******************
//
// scls_math_numbers.cpp
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
// This file contains the source code of scls_math_numbers.h.
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
#include "../scls_math.h"

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

    // Most simple fraction constructor
    __Fraction_Base::__Fraction_Base(const __Fraction_Base& to_copy) : a_denominator(to_copy.a_denominator), a_normalized(to_copy.a_normalized), a_numerator(to_copy.a_numerator) {normalize();};
    __Fraction_Base::__Fraction_Base(double real):__Fraction_Base(from_double(real)){};
    __Fraction_Base::__Fraction_Base():__Fraction_Base(0){};
    __Fraction_Base::__Fraction_Base(long long numerator, long long denominator) : a_denominator(denominator), a_numerator(numerator) {if(a_denominator!=1&&a_numerator!=1){normalize();}}

    // Clone
    void __Fraction_Base::algebra_clone(Algebra_Element* e) const{reinterpret_cast<__Fraction_Base*>(e)->a_denominator = a_denominator;reinterpret_cast<__Fraction_Base*>(e)->a_numerator = a_numerator;};
    std::shared_ptr<Algebra_Element> __Fraction_Base::algebra_clone() const{std::shared_ptr<Algebra_Element>f=new_algebra_element();algebra_clone(f.get());return f;};
    std::shared_ptr<Algebra_Element> __Fraction_Base::new_algebra_element() const{return std::make_shared<__Fraction_Base>();};
    std::shared_ptr<Algebra_Element> __Fraction_Base::new_algebra_element(std::string content) const{return std::make_shared<__Fraction_Base>(__Fraction_Base::from_std_string(content));};

    // Type of the object
    std::string __Fraction_Base::algebra_type() const{return std::string("fraction_base");};

    // Evaluate a relation of this element with another one
    Algebra_Element::Relation_State __Fraction_Base::evaluate_relation(Algebra_Element* other, std::string relation) {
        return Algebra_Element::Relation_State::RS_Not_In_Relation;
    }

    // Returns if two elements are equal or not
    bool __Fraction_Base::is_equal(Algebra_Element* other) const {
    	return a_numerator * reinterpret_cast<__Fraction_Base*>(other)->a_denominator == a_denominator * reinterpret_cast<__Fraction_Base*>(other)->a_numerator;
    }

    // Operates this element with another one
    void __Fraction_Base::operate(Algebra_Element* other, std::string operation){
        if(operation == "+"){_add(*reinterpret_cast<__Fraction_Base*>(other));}
        else if(operation == "*"){_multiply(*reinterpret_cast<__Fraction_Base*>(other));}
        else if(operation == "/"){_divide(*reinterpret_cast<__Fraction_Base*>(other));}
        else if(operation == "^"){double d = to_double();d = std::pow(d, other->value_to_double());set_from_double(d);std::cout << "UNPRECISE USE OF EXPONANT WITH FRACTION BASE " << std::endl; }
        else if(operation == "abs"){if(a_numerator < 0){a_numerator = -a_numerator;}}
        else if(operation == "cos"){double d = to_double();d = std::cos(d);set_from_double(d);}
        else if(operation == "exp"){double d = to_double();d = std::exp(d);set_from_double(d);}
        else if(operation == "ln"){double d = to_double();d = std::log(d);set_from_double(d);}
        else if(operation == "sin"){double d = to_double();d = std::sin(d);set_from_double(d);}
        else if(operation == "sqrt"){double d = to_double();d = std::sqrt(d);set_from_double(d);}
        else if(operation == "tan"){double d = to_double();d = std::tan(d);set_from_double(d);}
    }

    // Returns the absolute value of the fraction
    __Fraction_Base __Fraction_Base::abs() const{if(to_double() >= 0){return *this;}return (*this) * -1;};
    // Returns the inverse of the fraction
    __Fraction_Base __Fraction_Base::opposite() const{return __Fraction_Base(-a_numerator, a_denominator);};
    __Fraction_Base __Fraction_Base::inverse() const {return __Fraction_Base(a_denominator, a_numerator);};

    // Returns a fraction from a double
    constexpr double from_double_precision = 1000;
    __Fraction_Base __Fraction_Base::from_double(double result, double precision) {
        long long result_in_long = static_cast<long long>(result);
        double after_decimal_point = static_cast<double>(result - static_cast<double>(result_in_long));
        if(after_decimal_point == 0){return Fraction(result_in_long, 1);}
        long long after_decimal_point_in_long = static_cast<long long>(after_decimal_point * precision);
        return Fraction(result_in_long, 1) + Fraction(after_decimal_point_in_long, precision);
    }
    __Fraction_Base __Fraction_Base::from_double(double result) {return from_double(result, from_double_precision);};
    // Returns a fraction from a std::string
    __Fraction_Base __Fraction_Base::from_std_string(std::string content) {
        std::vector<std::string> cutted = cut_string(content, "/");
        if(cutted.size() <= 0) return Fraction(0);
        else if(cutted.size() == 1) {
            if(cutted[0]=="-")return Fraction(-1);
            try{return Fraction(std::stoll(cutted[0]));}catch(std::exception& e){print("SCLS Math Carl", std::string("Can't convert the string \"") + cutted[0] + std::string("\" to a number."));}
        }
        return Fraction(std::stoll(cutted[0]), std::stoll(cutted[1]));
    };

    // Crop the fraction
    void __Fraction_Base::crop(int limit){int a_den = std::ceil(std::log10(std::abs(a_denominator)));int a_num = std::ceil(std::log10(std::abs(a_numerator)));if(a_den > limit || a_num > limit){int to_divide = std::max(a_den, a_num);a_denominator /= std::pow(10, to_divide - limit);if(a_denominator<=0){a_denominator=1;}a_numerator /= std::pow(10, to_divide - limit);}}

    // Normalize the fraction
    int __normalize_value = 0;int __normalize_limit = 0;
    void __Fraction_Base::normalize(){if(__normalize_value > 0){normalize(__normalize_value);}else{__normalize();}};
    void __Fraction_Base::normalize(int limit){if(std::abs(a_denominator) > std::pow(10, limit) && std::abs(a_numerator) > std::pow(10, limit)){int value = std::ceil(std::log10(std::max(std::abs(a_denominator), std::abs(a_numerator)))) - limit;a_denominator/=std::pow(10, value);a_numerator/=std::pow(10, value);}normalize_force();};
    __Fraction_Base __Fraction_Base::normalized()const{__Fraction_Base other=*this;other.normalize();return other;};
    __Fraction_Base __Fraction_Base::normalized(int limit)const{__Fraction_Base other=*this;other.normalize(limit);return other;};
    void __Fraction_Base::__normalize() {if(a_normalized || (std::abs(a_denominator) < __normalize_limit && std::abs(a_numerator) < __normalize_limit)){return;}normalize_force();};
    void __Fraction_Base::normalize_force() {
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
        else {a_denominator = 1;}
        if(a_denominator < 0) a_denominator = -a_denominator;
        if(a_numerator < 0) a_numerator = -a_numerator;

        // Apply the sign
        if((base_denominator < 0 && base_numerator > 0) || (base_denominator > 0 && base_numerator < 0)) a_numerator = -a_numerator;
        a_normalized = true;
    };

    // Sets this fraction as a double
    void __Fraction_Base::set_from_double(double result) {__Fraction_Base new_value = from_double(result);a_denominator = new_value.a_denominator;a_numerator = new_value.a_numerator;};
    // Simplify the element
    void __Fraction_Base::simplify(){normalize_force();};
    // Returns the fraction in int
    long long __Fraction_Base::to_int() const {if(a_denominator == 0) return 0; return a_numerator / a_denominator;};
    // Returns the fraction in double
    double __Fraction_Base::to_double() const {if(a_denominator == 0) return 0; return static_cast<double>(a_numerator) / static_cast<double>(a_denominator);};
    double __Fraction_Base::to_double_ceil() const {return std::ceil(to_double());};
    double __Fraction_Base::to_double_floor() const {return std::floor(to_double());};
    double __Fraction_Base::to_double_round() const {return std::round(to_double());};

    // Return the value of the object in C++
    std::string __Fraction_Base::to_cpp() const {return std::string("scls::__Fraction_Base(") + std::to_string(a_numerator) + std::string(", ") + std::to_string(a_denominator) + std::string(")");}

    // Returns the fraction to MathML
    std::string __Fraction_Base::to_mathml(Textual_Math_Settings* settings)const{if(a_denominator == 1){return std::string("<math><mi>") + std::to_string(numerator()) + std::string("</mi></math>");}return std::string("<math><frac><mrow>") + std::to_string(a_numerator) + "</mrow><mrow>" + std::to_string(a_denominator) + std::string("</mrow></frac></math>");};
    // Returns the fraction to std::string, in the fraction redaction
    std::string __Fraction_Base::to_std_string_fraction(Textual_Math_Settings* settings) const {if(denominator() == 1) return std::to_string(numerator()); return std::to_string(numerator()) + "/" + std::to_string(denominator());};
    std::string __Fraction_Base::to_std_string(unsigned int max_number_size, Textual_Math_Settings* settings) const {std::string from_fraction = to_std_string_fraction(settings);if(from_fraction.size() <= max_number_size) return from_fraction;return format_number_to_text(to_double());};
    std::string __Fraction_Base::to_std_string(Textual_Math_Settings* settings) const {return to_std_string(-1, settings);};

    // Getters and setter
    long long __Fraction_Base::denominator() const {return a_denominator;};
    long long __Fraction_Base::numerator() const {return a_numerator;};

    //*********
    // Operator methods
    //*********

    // Divides the fraction with an another fraction
    void __Fraction_Base::_divide(__Fraction_Base obj) { _multiply(__Fraction_Base(obj.a_denominator, obj.a_numerator)); a_normalized = false;};
    __Fraction_Base __Fraction_Base::_divide_without_modification(__Fraction_Base obj) const { return _multiply_without_modification(__Fraction_Base(obj.a_denominator, obj.a_numerator)); };

    // Returns if this fraction is equal to another
    bool __Fraction_Base::_equal(__Fraction_Base obj) const {return obj.a_numerator * a_denominator == a_numerator * obj.a_denominator;};
    bool __Fraction_Base::_equal(int obj) const {return a_numerator == obj && a_denominator == 1;};

    // Multiplies the fraction with an another Fraction
    void __Fraction_Base::_multiply(__Fraction_Base obj) { a_numerator *= obj.a_numerator; a_denominator *= obj.a_denominator; a_normalized = false; };
    __Fraction_Base __Fraction_Base::_multiply_without_modification(__Fraction_Base obj) const { return __Fraction_Base(a_numerator * obj.a_numerator, a_denominator * obj.a_denominator); };
    __Fraction_Base __Fraction_Base::_multiply_without_modification(double obj) const { return __Fraction_Base(a_numerator * obj, a_denominator); };
    __Fraction_Base __Fraction_Base::_multiply_without_modification(int obj) const { return __Fraction_Base(a_numerator * obj, a_denominator); };
    __Fraction_Base __Fraction_Base::_multiply_without_modification(unsigned int obj) const { return __Fraction_Base(a_numerator * obj, a_denominator); };

    // Returns the square root of the fraction
    __Fraction_Base __Fraction_Base::sqrt() {return from_double(std::sqrt(to_double()));};

    // Operator overloading with fractions
    // Decrement operator
    __Fraction_Base& __Fraction_Base::operator--(int) { _substract(__Fraction_Base(1, 1)); return *this; }
    // Divisor operator
    __Fraction_Base __Fraction_Base::operator/(__Fraction_Base obj) const { return _divide_without_modification(obj); };
    // Divisor operator assignment
    __Fraction_Base& __Fraction_Base::operator/=(__Fraction_Base obj) { _divide(obj); return *this; };
    // Equality operator
    bool __Fraction_Base::operator==(__Fraction_Base obj) const { return _equal(obj); }
    // Greater or equal than than operator
    bool __Fraction_Base::operator>=(__Fraction_Base r) const { return _equal(r) || to_double() > r.to_double(); }
    // Greater than than operator
    bool __Fraction_Base::operator>(__Fraction_Base r) const { return to_double() > r.to_double(); }
    bool __Fraction_Base::operator>(double r) const { return to_double() > r;}
    // Increment operator
    __Fraction_Base& __Fraction_Base::operator++(int) { _add(__Fraction_Base(1, 1)); return *this; }
    // Lesser than than operator
    bool __Fraction_Base::operator<(__Fraction_Base r) const { return to_double() < r.to_double(); }
    bool __Fraction_Base::operator<(double r) const { return to_double() < r; }
    // Lesser or equal than than operator
    bool __Fraction_Base::operator<=(__Fraction_Base r) const { return _equal(r) || to_double() < r.to_double(); }
    // Arithmetic operator
    __Fraction_Base __Fraction_Base::operator-(__Fraction_Base obj) const { return _substract_without_modification(obj); };
    __Fraction_Base& __Fraction_Base::operator-=(__Fraction_Base obj) { _substract(obj); return *this; }
    __Fraction_Base __Fraction_Base::operator*(__Fraction_Base obj) const { return _multiply_without_modification(obj); };
    __Fraction_Base& __Fraction_Base::operator*=(__Fraction_Base obj) { _multiply(obj); return *this; };
    __Fraction_Base __Fraction_Base::operator+(__Fraction_Base obj) const { return _add_without_modification(obj); };
    __Fraction_Base& __Fraction_Base::operator+=(__Fraction_Base obj) { _add(obj); return *this; }

    // Function to do operations with fractions
    // Adds an another Fraction to this fraction
    void __Fraction_Base::_add(__Fraction_Base obj) {
        long long first_numerator = obj.a_numerator * a_denominator;
        long long second_numerator = a_numerator * obj.a_denominator;
        a_denominator = obj.a_denominator * a_denominator;
        a_numerator = first_numerator + second_numerator;
        a_normalized = false;
        normalize();
    };
    // Returns the adding of this fraction and another function
    __Fraction_Base __Fraction_Base::_add_without_modification(__Fraction_Base obj) const {
        long long first_numerator = obj.a_numerator * a_denominator;
        long long second_numerator = a_numerator * obj.a_denominator;
        long long denominateur = obj.a_denominator * a_denominator;

        Fraction new_fraction = Fraction(first_numerator + second_numerator, denominateur);
        return new_fraction;
    };
    // Substracts an another Fraction to this fraction
    void __Fraction_Base::_substract(__Fraction_Base obj) {
        long long first_numerator = obj.a_numerator * a_denominator;
        long long second_numerator = a_numerator * obj.a_denominator;
        a_denominator = obj.a_denominator * a_denominator;
        a_numerator = second_numerator - first_numerator;
        a_normalized = false;
        normalize();
    };
    // Returns the substracting of this fraction and another function
    __Fraction_Base __Fraction_Base::_substract_without_modification(__Fraction_Base obj) const {
        long long first_numerator = obj.a_numerator * a_denominator;
        long long second_numerator = a_numerator * obj.a_denominator;
        long long denominateur = obj.a_denominator * a_denominator;

        __Fraction_Base new_fraction = Fraction(second_numerator - first_numerator, denominateur);
        return new_fraction;
    };

    // Comparaison operator
    bool operator>(double obj_1, __Fraction_Base obj){return obj_1 > obj.to_double();};
    bool operator<(double obj_1, __Fraction_Base obj){return obj_1 < obj.to_double();};
	// Multiplication operator
    __Fraction_Base operator*(int obj_1, __Fraction_Base obj){return obj._multiply_without_modification(obj_1);}
    Fraction operator*(int obj_1, Fraction obj){return obj._multiply_without_modification(obj_1);}
    // Minus operator
    __Fraction_Base operator-(int obj_1, __Fraction_Base obj){return __Fraction_Base(obj_1)._substract_without_modification(obj);}
    Fraction operator-(int obj_1, Fraction obj){return Fraction(obj_1)._substract_without_modification(obj);}
	// Stream operator overloading (indev)
    std::ostream& operator<<(std::ostream& os, const __Fraction_Base& obj){ os << "Fraction : " << obj.numerator() << " / " << obj.denominator() << " = " << obj.to_double(); return os; }
    std::ostream& operator<<(std::ostream& os, const Fraction& obj){ os << "Fraction : " << obj.numerator() << " / " << obj.denominator() << " = " << obj.to_double(); return os; }

    // Returns a random fraction
    Fraction random_fraction(Fraction min_value, Fraction max_value, int precision){return (min_value + (max_value - min_value) * Fraction(random_int_between_included(0, precision), precision)).normalized();}
    Fraction random_fraction(Fraction min_value, Fraction max_value){return random_fraction(min_value, max_value, 1000);}
    // Function to sort a std::vector of fraction
    void remove_duplication_sorted_fractions(std::vector<Fraction>& fractions){for(int i = 1;i<static_cast<int>(fractions.size());i++){if(fractions.at(i)==fractions.at(i-1)){fractions.erase(fractions.begin()+i);i--;}}}
    bool __sort_fractions(const Fraction& obj_1, const Fraction& obj_2){return obj_1 < obj_2;};
    void sort_fractions(std::vector<Fraction>& fractions){std::sort(fractions.begin(), fractions.end(), __sort_fractions);}

    // Returns a fraction from a double
    Fraction Fraction::from_double(double result, double precision){__Fraction_Base b = __Fraction_Base::from_double(result, precision);return Fraction(b);}
    Fraction Fraction::from_double(double result) {__Fraction_Base b = __Fraction_Base::from_double(result);return Fraction(b);}
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

    std::string Limit::to_std_string(Textual_Math_Settings* settings) const {
        if(is_pi()) {return std::string("+infini");}
        else if(is_mi()) {return std::string("-infini");}
        else if(is_pz()) {return std::string("0+");}
        else if(is_mz()) {return std::string("0-");}
        return value().to_std_string(settings);
    };
}
