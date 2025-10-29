//******************
//
// scls_math_polynomial.h
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
// This file contains some functions ot handle polynomial expression.
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

#ifndef SCLS_MATH_polynomial
#define SCLS_MATH_polynomial

#include "scls_math_numbers.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
	//
	// The "__Monomonial" class
	//
	//*********

    class _Base_Unknown {
        // Class representating the base of an unknow in a monomonial
    public:
        // Possible unknows comparaison
        enum Unknown_Comparaison {UC_DIFFERENT, UC_EQUAL, UC_EQUAL_UNKNOWN};

        // Unknow constructor
        _Base_Unknown(std::string new_name, Complex exponent):a_exponent(exponent),a_name(new_name){};
        _Base_Unknown(std::string new_name):_Base_Unknown(new_name, Complex(1)){};

        // Returns the comparaison between two unknows
        bool compare_unknown(_Base_Unknown other) const;

        // Getters and setters
        inline bool conjugate() const {return a_conjugate;};
        inline Complex exponent() const {return a_exponent;};
        inline std::string name() const {return a_name;};
        inline void set_exponent(Complex new_exponent) {a_exponent = new_exponent;};
        inline void set_name(std::string new_name) {a_name = new_name;};

        // Operator ==
        inline bool operator==(_Base_Unknown other) const {return compare_unknown(other) == Unknown_Comparaison::UC_EQUAL;};

    private:

        // If the unknown is a conjugate
        bool a_conjugate = false;
        // Exponent of the unknow
        Complex a_exponent = Complex(1, 0);
        // Name of the unknow
        std::string a_name = "";
    };

    class __Monomonial {
        // Class representating a monomonial in a polynomial form
    public:

        // __Monomonial constructor
        __Monomonial(Complex factor):a_factor(factor){};
        __Monomonial(Complex factor, std::vector<_Base_Unknown> unknowns):__Monomonial(factor){a_unknowns=unknowns;};
        __Monomonial(Complex factor, std::string unknow):__Monomonial(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow))){};
        __Monomonial(Complex factor, std::string unknow, Complex exponent):__Monomonial(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow, exponent))){};
        // __Monomonial copy constructor
        __Monomonial(const __Monomonial& monomonial_copy):__Monomonial(monomonial_copy.a_factor,monomonial_copy.a_unknowns){};

        // Returns the inverse the monomonial
        __Monomonial inverse() const;
        // Returns the special limit of the monomonial for an unknown
        scls::Limit limit_special(Limit needed_limit, std::string unknown_name);

        // Add an unknown to the monomonial
        inline void add_unknown(std::string name, Complex exponent) {_Base_Unknown unknown(name); unknown.set_exponent(exponent); a_unknowns.push_back(unknown);};
        // Returns if a monomonial has the same unknows as this one
        bool compare_unknown(__Monomonial other);
        // Returns if the monomonial contains an unkwnown
        inline _Base_Unknown* contains_unknown(std::string unknown_name) {for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {if(a_unknowns.at(i).name() == unknown_name) {return &a_unknowns[i];}} return 0;};
        // Deletes the monomonial unkwnown
        inline void delete_unknown(std::string unknown_name) {for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {if(a_unknowns[i].name() == unknown_name) {a_unknowns.erase(a_unknowns.begin() + i); return;}}};
        inline void delete_unknown(_Base_Unknown* unknown_to_delete) {delete_unknown(unknown_to_delete->name());};
        // Returns if the monomonial is known
        inline bool is_known() const {return a_unknowns.size() <= 0 || (a_unknowns.size() == 1 && (a_unknowns.at(0).name() == "" || a_unknowns.at(0).exponent() == 0));};
        // Returns if the monomonial only contains an unkwnown
        _Base_Unknown* only_contains_unknown(std::string unknown_name, Complex exponent);
        // Returns this monomonial where an unknown is replaced by an another unknown
        __Monomonial replace_unknown(std::string unknown, __Monomonial new_value);
        // Returns if two monomonials has the same unknowns
        bool same_unknowns(__Monomonial monomonial) const;
        // Unknows of the monomonial
        inline _Base_Unknown* unknown(){if(unknowns().size()>0){if(unknowns()[0].name()==std::string()){if(unknowns().size()>1){return &unknowns()[1];}}else{return &unknowns()[0];}}return 0;};
        inline std::vector<_Base_Unknown>& unknowns() {return a_unknowns;};
        inline unsigned int unknowns_number() const {unsigned int to_return = 0;for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns[i].name() != "")to_return++;}return to_return;};

        // Returns the monomonial to a GLSL calculation
        std::string to_glsl(Textual_Math_Settings* settings) const;
        // Returns the monomonial to mathml
        std::string to_mathml(Textual_Math_Settings* settings) const;
        // Returns the monomonial converted to std::string
        std::string to_std_string(Textual_Math_Settings* settings) const;

        // Operators
        // Equal operator assignment
        bool operator==(__Monomonial const& obj) const {return a_factor == obj.a_factor && same_unknowns(obj); }
        // Minus operator assignment
        __Monomonial& operator-=(__Monomonial const& obj) { a_factor -= obj.a_factor; return *this; }
        // Plus operator assignment
        __Monomonial& operator+=(__Monomonial const& obj) { a_factor += obj.a_factor; return *this; }
        // Multiplication operator assignment
        __Monomonial& operator*=(__Monomonial const& obj);
        // With fractions
        inline __Monomonial& operator*=(Fraction obj){__Monomonial temp(obj);return operator*=(temp);};
        // Convert the monomonial to a complex
        operator Complex() const {return a_factor;};

        // Adds a value to the factor
        void add_to_factor(Complex value){a_factor += value;};

        // Getters and setters
        inline Complex factor() const {return a_factor;};
        inline void set_factor(Complex new_factor) {a_factor = new_factor;};

    private:

        // Factor of the monomonial
        Complex a_factor = Complex(Fraction(0), Fraction(0));

        // Unknows of the monomonial
        std::vector<_Base_Unknown> a_unknowns;
    };

    // Stream operator overloading
    std::ostream& operator<<(std::ostream& os, const __Monomonial& obj);

    //*********
	//
	// The "Polynomial" class
	//
	//*********

	class Polynomial {
        // Class representating a full polynomial form
    public:

        // Polynomial to complex form
        class Polynomial_Complex {
        public:
            // Polynomial_Complex constructor
            Polynomial_Complex(){a_polynomial.push_back(__Polynomial_Complex_Member());a_polynomial.push_back(__Polynomial_Complex_Member());a_polynomial[0].unknown = std::string("");a_polynomial[1].unknown = std::string("i");};

            // Conjugates the polynomial
            inline std::shared_ptr<Polynomial_Complex> conjugate(){std::shared_ptr<Polynomial_Complex> to_return = std::make_shared<Polynomial_Complex>(*this);(*to_return.get()->imaginary_polynomial()) *= scls::Fraction(-1);return to_return;};
            // Converts a polynomial to a complex
            static std::shared_ptr<Polynomial_Complex> from_polynomial(Polynomial* polynomial);
            // Returns the imaginary polynomial
            inline Polynomial* imaginary_polynomial() const {for(int i = 0;i<static_cast<int>(a_polynomial.size());i++){if(a_polynomial[i].unknown == std::string("i")){return a_polynomial[i].polynomial.get();}}return 0;};
            // Returns the real polynomial
            inline Polynomial* real_polynomial() const {for(int i = 0;i<static_cast<int>(a_polynomial.size());i++){if(a_polynomial[i].unknown == std::string("")){return a_polynomial[i].polynomial.get();}}return 0;};
            // Returns the polynomial to an entire polynomial
            inline std::shared_ptr<Polynomial> to_polynomial() const {std::shared_ptr<Polynomial> to_return = std::make_shared<Polynomial>(*real_polynomial());to_return.get()->__add((*imaginary_polynomial()) * scls::Complex(0, 1));return to_return;};

        private:
            // Struct representating each members of the number
            struct __Polynomial_Complex_Member{std::shared_ptr<Polynomial>polynomial=std::make_shared<Polynomial>();std::string unknown = std::string();};

            // Each parts of the polynomial
            std::vector<__Polynomial_Complex_Member> a_polynomial;
        };

        // Polynomial constructor
        Polynomial(){};
        Polynomial(double number){a_monomonials.push_back(__Monomonial(Complex(number)));};
        Polynomial(__Fraction_Base number){a_monomonials.push_back(__Monomonial(Complex(number)));};
        Polynomial(Fraction number){a_monomonials.push_back(__Monomonial(Complex(number)));};
        Polynomial(__Monomonial monomonial){a_monomonials.push_back(monomonial);};
        Polynomial(Complex factor, std::string unknow, Complex exponent):Polynomial(__Monomonial(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow, exponent)))){};
        // Polynomial copy constructor
        Polynomial(const Polynomial& polynomial_copy){for(int i = 0;i<static_cast<int>(polynomial_copy.a_monomonials.size());i++){a_monomonials.push_back(polynomial_copy.a_monomonials.at(i));}};

        // Returns the limit of a (only monomonial) polynomial for an unknown
        scls::Limit limit(Limit needed_limit, std::string unknown_name);

        // Add a new monomonial to the polynomial
        void add_monomonial(__Monomonial new_monomonial);
         // Returns all the unknowns in the formula
        inline std::vector<std::string> all_unknowns(){std::vector<std::string> to_return;for(int i=0;i<static_cast<int>(a_monomonials.size());i++){for(int j=0;j<static_cast<int>(a_monomonials[i].unknowns().size());j++){std::string to_add=a_monomonials[i].unknowns()[j].name();if(to_add!=""&&std::count(to_return.begin(),to_return.end(),to_add)<=0){to_return.push_back(to_add);}}}return to_return;};
        // Returns if the polynomial contains a monomonial
        inline __Monomonial* contains_monomonial(__Monomonial new_monomonial) {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {if(a_monomonials[i].compare_unknown(new_monomonial)) {return &a_monomonials[i];}} return 0;};
        // Returns if the polynomial is known
        inline bool is_known() const {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {if(!a_monomonials[i].is_known() && a_monomonials[i].factor() != 0) return false;} return true;};
        // Returns if the polynomial is a simple monomonial
        inline bool is_simple_monomonial() const {return monomonials_number() <= 1;};
        // Returns the knows monomonial
        inline __Monomonial known_monomonial() const {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {if(a_monomonials.at(i).unknowns_number() <= 0) return a_monomonials.at(i);}return __Monomonial(Complex(0));};
        // Returns a monomonial by its unknown
        __Monomonial monomonial(std::string unknown, Complex exponent);
        inline __Monomonial monomonial(std::string unknown) {return monomonial(unknown, Complex(1, 0));};
        inline __Monomonial monomonial() {if(monomonials().size() > 0){return monomonials()[0];}return __Monomonial(0);};
        // Returns the number of monomonials in the polynomial
        inline int monomonials_number() const {return a_monomonials.size();};
        // Returns a list of unknowns monomonials
        std::vector<__Monomonial> unknown_monomonials() const;

        // Returns the polynomial to a GLSL function
        std::string to_glsl(Textual_Math_Settings* settings) const;
        // Returns the polynomial to mathml
        std::string to_mathml(Textual_Math_Settings* settings) const;
        // Returns the polynomial to std::string
        std::string to_std_string(Textual_Math_Settings* settings) const;

        // Methods operators
        // Add a polynomial to this one
        void __add(Polynomial value);
        // Divide a monomonial to this void
        void __divide(__Monomonial value);
        void __divide(Complex value);
        void __divide(Fraction value);
        void __divide(Polynomial* value);
        // Returns if two values are equals or not
        inline bool __is_equal(int value) const{return __is_equal(Complex(value));};
        inline bool __is_equal(Fraction value) const{return __is_equal(Complex(value));};
        bool __is_equal(Complex value) const;
        bool __is_equal(Polynomial value) const;
        bool __is_equal(Polynomial* value) const;
        // Multiply a polynomial to this one
        void __multiply(Polynomial value);
        inline void __multiply(Complex value){Polynomial temp; temp.add_monomonial(value);__multiply(temp);};
        inline void __multiply(Fraction value) {Polynomial temp;temp.add_monomonial(__Monomonial(value));__multiply(temp);};

        // Returns the maximum degree in the polynomial
        Complex degree(std::string unknown_name);
        // Returns a polynomial from a monomonial where the unknows has been replaced
        static Polynomial polynomial_from_modified_monomonial_unknows(__Monomonial used_monomonial, std::string unknown, Polynomial new_value);
        // Returns this polynomial where an unknown is replaced by an another unknown
        Polynomial replace_unknown(std::string unknown, Polynomial new_value) const;
        // Simplify the polynomial
        Polynomial simplify() const;

        // Operators
        // With Complex
        bool operator!=(Complex value) const {return !(*this == value);};
        bool operator==(Complex value)const{return __is_equal(value);};
        Polynomial operator*(Complex value){Polynomial to_return(*this);to_return.__multiply(value);return to_return;}
        Polynomial& operator/=(Complex value) {__divide(value);return*this;};
        // With fractions
        Polynomial& operator*=(Fraction value) {__multiply(value);return*this;};
        Polynomial& operator/=(Fraction value) {__divide(value);return*this;};
        // With polynomials
        Polynomial operator-(Polynomial value) const {Polynomial to_return(*this);to_return-=value;return to_return;};
        Polynomial& operator-=(Polynomial value) {value*=Fraction(-1);__add(value);return*this;};
        Polynomial& operator+=(Polynomial value) {__add(value);return*this;};
        Polynomial& operator*=(Polynomial value) {__multiply(value);return*this;};
        // With monomonials
        Polynomial& operator+=(__Monomonial value) {add_monomonial(value);return*this;};
        Polynomial& operator/=(__Monomonial value) {__divide(value);return*this;};

        // Getters and setters
        inline std::vector<__Monomonial>& monomonials() {return a_monomonials;};

    private:

        // Each monomonial in the polynomial
        std::vector<__Monomonial> a_monomonials = std::vector<__Monomonial>();
	};

	// Stream operator overloading
    std::ostream& operator<<(std::ostream& os, Polynomial& obj);
	// Divide operator
    Polynomial operator/(__Monomonial& obj, __Monomonial const& other);

    //*********
	//
	// The "Field" class
	//
	//*********

	template <typename E> class Field {
        // Class representating a mathematical element field
    public:
        // Field constructor
        Field(){};
        Field(E base_value):a_element_add(base_value){};

        // Pastes a formula to this one
        virtual void paste(Field* value){a_element_add=value->a_element_add;};

        // Methods operators
        // Add a formula to this one
        virtual void __add(Field value) {a_element_add += value.a_element_add;};
        virtual void __add(E value) {a_element_add += value;};

        // Divide a polynomial to this one
        virtual void __divide(Field value) {__divide(value.a_element_add);};
        virtual void __divide(E value) {};
        virtual void __divide(Fraction value) {a_element_add /= value;};

        // Multiply a polynomial to this one
        virtual void __multiply(Field value) {a_element_add *= value.a_element_add;};
        virtual void __multiply(E value) {a_element_add *= value;};
        virtual void __multiply(Fraction value) {a_element_add *= value;};

        // Operators
        Field& operator*=(Fraction value) {__multiply(value);return*this;};
        // With E
        Field operator-(E value) const {Field to_return(*this);to_return-=value;return to_return;};
        Field& operator-=(E value) {value*=Fraction(-1);__add(value);return*this;};
        Field operator+(E value) {Field to_return(*this);to_return.__add(value);return to_return;};
        Field& operator+=(E value) {__add(value);return*this;};
        Field operator*(E value) {Field to_return(*this);to_return.__multiply(value);return to_return;};
        Field& operator*=(E value) {__multiply(value);return*this;};
        Field operator/(E value) const {Field other(*this);other.__divide(value);return other;};
        Field& operator/=(E value) {__divide(value);return*this;};
        // With formulas
        Field operator-(Field value) const {Field to_return(*this);to_return-=value;return to_return;};
        Field& operator-=(Field value) {value*=Fraction(-1);__add(value);return*this;};
        Field operator+(Field value) {Field to_return(*this);to_return.__add(value);return to_return;};
        Field& operator+=(Field value) {__add(value);return*this;};
        Field operator*(Field value) {Field to_return(*this);to_return.__multiply(value);return to_return;};
        Field& operator*=(Field value) {__multiply(value);return*this;};
        Field operator/(Field value) const {Field other(*this);other.__divide(value);return other;};
        Field& operator/=(Field value) {__divide(value);return*this;};

        // Getters and setters
        inline E added_element() const {return a_element_add;};
        inline E& added_element_reference() {return a_element_add;};
        inline void set_added_element(E new_added_element){a_element_add=new_added_element;};
    protected:
        // Attached added element
        E a_element_add;
	};

	//*********
	//
	// The sets class
	//
	//*********

	class Interval {
	    // Class representating an interval of real numbers
    public:

        // Interval constructor
        Interval(Fraction start, bool start_included, Fraction end, bool end_included):a_end(end),a_end_included(end_included),a_start(start),a_start_included(start_included){};
        Interval(Fraction start, Fraction end):Interval(start, true, end, true){};
        Interval(Fraction value):Interval(value, true, value, true){};
        Interval():Interval(Fraction(0), Fraction(0)){};
        Interval(const Interval& other):a_end(other.a_end),a_end_included(other.a_end_included),a_end_infinite(other.a_end_infinite),a_start(other.a_start),a_start_included(other.a_start_included),a_start_infinite(other.a_start_infinite){}

        // Clears the interval
        inline void clear(){a_end=0;a_end_included=false;a_end_infinite=false;a_start=0;a_start_included=false;a_start_infinite=false; }
        // Compares this definition set with another
        inline bool compare(Interval value){return ((a_end_infinite == value.a_end_infinite) && (a_end_infinite || a_end == value.a_end)) && ((a_start_infinite == value.a_start_infinite) && (a_start_infinite || a_start == value.a_start)); };
        // Returns if the interval is empty or not
        inline bool is_empty() const {return a_start == a_end && (!a_start_included || !a_end_included) && !a_start_infinite && !a_end_infinite;};
        // Returns if a value is in an interval
        inline bool is_in(Fraction value){return ((a_end_infinite || value < a_end || (value == a_end && a_end_included)) && (a_start_infinite || value > a_start || (value == a_start && a_start_included)));};
        // Returns if an interval is in an interval
        inline bool is_in(Interval* value){return (((a_end_infinite) || (!value->a_end_infinite && value->a_end <= a_end)) && ((a_start_infinite) || (!value->a_start_infinite && value->a_start >= a_start)));};
        // Returns the intersection between this interval and an another interval
        Interval intersection(Interval other);
        inline Interval intersection(Fraction needed_start, bool needed_start_included, Fraction needed_end, bool needed_end_included){return intersection(Interval(needed_start, needed_start_included, needed_end, needed_end_included));};

        // Returns the interval to an std::string
        std::string end_to_std_string(Textual_Math_Settings* settings) const;
        std::string start_to_std_string(Textual_Math_Settings* settings) const;
        std::string to_std_string(Textual_Math_Settings* settings);

        // Getters and setters
        inline Fraction end() const {return a_end;};
        inline bool end_included() const {return a_end_included;};
        inline bool end_infinite() const {return a_end_infinite;};
        inline void set_end(Fraction new_end) {a_end=new_end;};
        inline void set_end_included(bool new_end_included) {a_end_included=new_end_included;};
        inline void set_start(Fraction new_start) {a_start=new_start;};
        inline void set_start_included(bool new_start_included) {a_start_included=new_start_included;};
        inline Fraction start() const {return a_start;};
        inline bool start_included() const {return a_start_included;};
        inline void set_end_infinite(bool new_end_infinite) {a_end_infinite=new_end_infinite;};
        inline void set_start_infinite(bool new_start_infinite) {a_start_infinite=new_start_infinite;};
        inline bool start_infinite() const {return a_start_infinite;};

        // Operators
        // With Interval
        bool operator==(Interval value) {return compare(value);};
        bool operator!=(Interval value) {return !compare(value);};

    private:
        // End of the interval
        Fraction a_end = 0;
        // If the end is included or not
        bool a_end_included = false;
        // If the end is + infinite or not
        bool a_end_infinite = false;
        // Start of the interval
        Fraction a_start = 0;
        // If the start is included or not
        bool a_start_included = false;
        // If the start is + infinite or not
        bool a_start_infinite = false;
	};

	class Set_Number {
        // Class representating a set of numbers
    public:

        // Set_Number constructor
        Set_Number(){};
        Set_Number(Interval interval){a_intervals.push_back(interval);};
        Set_Number(const Set_Number& other):a_intervals(other.a_intervals),a_numbers(other.a_numbers){}

        // Add an interval to the set
        inline void add_interval(Interval to_add) {if(!to_add.is_empty()){a_intervals.push_back(to_add);__sort_interval();check_intervals();};};
        // Add a number to the set
        inline void add_number(Fraction to_add) {if(!is_in(to_add)){add_interval(Interval(to_add));}};
        inline void add_number(Complex to_add) {add_number(to_add.real());};

        // Clears the interval
        inline void clear(){a_intervals.clear();a_numbers.clear();};
        // Returns the cardinal of the set (only works for denombrable sets)
        inline int cardinal()const{return a_intervals.size();};
        // Compares this definition set with another
        bool compare(Set_Number value);

        // Set algebra
        // Excludes a value
        void exclude(scls::Fraction number);
        void exclude(Interval interval);
        void exclude(Set_Number* set_number);
        inline Set_Number exclusion(scls::Fraction number){Set_Number temp = *this;temp.exclude(number);return temp;};
        inline Set_Number exclusion(Interval interval){Set_Number temp = *this;temp.exclude(interval);return temp;};
        inline Set_Number exclusion(Set_Number set_number){Set_Number temp = *this;temp.exclude(&set_number);return temp;};
        // Returns the intersection between this set and an interval
        Set_Number intersection(Interval other);
        Set_Number intersection(Set_Number other);
        // Returns if the set is empty or not
        inline bool is_empty() const {return a_intervals.size()<=0;};
        // Returns if a value is in the set numbers
        inline bool is_in_intervals(scls::Fraction value){for(int i = 0;i<static_cast<int>(a_intervals.size());i++){if(a_intervals[i].is_in(value)){return true;}}return false;};
        inline bool is_in_numbers(scls::Fraction value){for(int i = 0;i<static_cast<int>(a_numbers.size());i++){if(a_numbers[i]==value){return true;}}return false;};
        inline bool is_in(scls::Fraction value){return is_in_intervals(value) || is_in_numbers(value);};
        // Returns if an interval is in the set
        inline bool is_in_intervals(Interval value){for(int i = 0;i<static_cast<int>(a_intervals.size());i++){if(a_intervals[i].is_in(&value)){return true;}}return false;};
        inline bool is_in(Interval value){return is_in_intervals(value);};
        // Returns if the set is infinite or not
        inline bool is_infinite() const {return a_intervals.size() > 0 && a_intervals.at(0).start_infinite() && a_intervals.at(0).end_infinite();};
        // Unites with an another set
        inline Set_Number reunion(Interval number){Set_Number temp = *this;temp.unite(number);return temp;};
        inline Set_Number reunion(Set_Number* set_number){Set_Number temp = *this;temp.unite(set_number);return temp;};
        inline Set_Number reunion(Set_Number set_number){return reunion(&set_number);};
        void unite(Interval number);
        void unite(Set_Number* set_number);
        inline void unite(Set_Number set_number){unite(&set_number);};

        // Sub-parties algebra
        // Returns the complement between this sets and the real set (and another one if needed)
        Set_Number complement_real(Set_Number* set_number);
        // Returns the complement between two sets
        Set_Number complement_relative(Set_Number* set_number);
        Set_Number complement_relative_symetrical(Set_Number* set_number);

        // Returns the set in a std::string
        std::string to_std_string(Textual_Math_Settings* settings);

        // Checks the intervals / numbers
        void check_intervals();
        // Sort the intervals / numbers in the set
        void __sort_interval();
        void __sort_numbers();

        // Predefined sets
        // "After" set
        static Interval after_strictly(scls::Fraction value){Interval it;it.set_start(value);it.set_start_included(false);it.set_end_infinite(true);return it;};
        // "Before" set
        static Interval before_strictly(scls::Fraction value){Interval it;it.set_end(value);it.set_end_included(false);it.set_start_infinite(true);return it;};
        // "Between" set
        static Interval between_strictly(scls::Fraction after, scls::Fraction before){if(after==before){return Interval();} Interval it;it.set_end(before);it.set_end_included(false);it.set_start(after);it.set_start_included(false);it.set_end_infinite(true);return it;};
        // Real set
        static Set_Number real(){return set_real();};
        static Set_Number set_real() {Set_Number to_return;Interval i;i.set_end_infinite(true);i.set_start_infinite(true);to_return.add_interval(i);return to_return;};

        // Operators
        // With Set_Number
        bool operator==(Set_Number value) {return compare(value);};
        bool operator!=(Set_Number value) {return !compare(value);};

        // Getters and setters
        const std::vector<Interval>& intervals() const {return a_intervals;};
        const std::vector<Complex>& numbers() const {return a_numbers;};

    private:
        // Intervals of fractions in this set
        std::vector<Interval> a_intervals;
        // Numbers of fractions in this set
        std::vector<Complex> a_numbers;
	};

}

#endif // SCLS_MATH_polynomial
