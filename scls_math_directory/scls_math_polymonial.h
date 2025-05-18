//******************
//
// scls_math_polymonial.h
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
// This file contains some functions ot handle polymonial expression.
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

#ifndef SCLS_MATH_POLYMONIAL
#define SCLS_MATH_POLYMONIAL

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
        // Class representating a monomonial in a polymonial form
    public:

        // __Monomonial constructor
        __Monomonial(Complex factor):a_factor(factor){a_factor.normalize(4);};
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
        // Returns the factor of the monomonial
        inline Complex factor() const {return a_factor;};
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

        // Getters and setters
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
	// The "Polymonial" class
	//
	//*********

	class Polymonial {
        // Class representating a full polymonial form
    public:

        // Polymonial to complex form
        class Polymonial_Complex {
        public:
            // Polymonial_Complex constructor
            Polymonial_Complex(){a_polymonial.push_back(__Polymonial_Complex_Member());a_polymonial.push_back(__Polymonial_Complex_Member());a_polymonial[0].unknown = std::string("");a_polymonial[1].unknown = std::string("i");};

            // Conjugates the polymonial
            inline std::shared_ptr<Polymonial_Complex> conjugate(){std::shared_ptr<Polymonial_Complex> to_return = std::make_shared<Polymonial_Complex>(*this);(*to_return.get()->imaginary_polymonial()) *= scls::Fraction(-1);return to_return;};
            // Converts a polymonial to a complex
            static std::shared_ptr<Polymonial_Complex> from_polymonial(Polymonial* polymonial);
            // Returns the imaginary polymonial
            inline Polymonial* imaginary_polymonial() const {for(int i = 0;i<static_cast<int>(a_polymonial.size());i++){if(a_polymonial[i].unknown == std::string("i")){return a_polymonial[i].polymonial.get();}}return 0;};
            // Returns the real polymonial
            inline Polymonial* real_polymonial() const {for(int i = 0;i<static_cast<int>(a_polymonial.size());i++){if(a_polymonial[i].unknown == std::string("")){return a_polymonial[i].polymonial.get();}}return 0;};
            // Returns the polymonial to an entire polymonial
            inline std::shared_ptr<Polymonial> to_polymonial() const {std::shared_ptr<Polymonial> to_return = std::make_shared<Polymonial>(*real_polymonial());to_return.get()->__add((*imaginary_polymonial()) * scls::Complex(0, 1));return to_return;};

        private:
            // Struct representating each members of the number
            struct __Polymonial_Complex_Member{std::shared_ptr<Polymonial>polymonial=std::make_shared<Polymonial>();std::string unknown = std::string();};

            // Each parts of the polymonial
            std::vector<__Polymonial_Complex_Member> a_polymonial;
        };

        // Polymonial constructor
        Polymonial(){};
        Polymonial(int number){a_monomonials.push_back(__Monomonial(Complex(number)));};
        Polymonial(Fraction number){a_monomonials.push_back(__Monomonial(Complex(number)));};
        Polymonial(__Monomonial monomonial){a_monomonials.push_back(monomonial);};
        Polymonial(Complex factor, std::string unknow, Complex exponent):Polymonial(__Monomonial(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow, exponent)))){};
        // Polymonial copy constructor
        Polymonial(const Polymonial& polymonial_copy){for(int i = 0;i<static_cast<int>(polymonial_copy.a_monomonials.size());i++){a_monomonials.push_back(polymonial_copy.a_monomonials.at(i));}};

        // Returns the limit of a (only monomonial) polymonial for an unknown
        scls::Limit limit(Limit needed_limit, std::string unknown_name);

        // Add a new monomonial to the polymonial
        void add_monomonial(__Monomonial new_monomonial);
         // Returns all the unknowns in the formula
        inline std::vector<std::string> all_unknowns(){std::vector<std::string> to_return;for(int i=0;i<static_cast<int>(a_monomonials.size());i++){for(int j=0;j<static_cast<int>(a_monomonials[i].unknowns().size());j++){std::string to_add=a_monomonials[i].unknowns()[j].name();if(to_add!=""&&std::count(to_return.begin(),to_return.end(),to_add)<=0){to_return.push_back(to_add);}}}return to_return;};
        // Returns if the polymonial contains a monomonial
        inline __Monomonial* contains_monomonial(__Monomonial new_monomonial) {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {if(a_monomonials[i].compare_unknown(new_monomonial)) {return &a_monomonials[i];}} return 0;};
        // Returns if the polymonial is known
        inline bool is_known() const {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {if(!a_monomonials[i].is_known() && a_monomonials[i].factor() != 0) return false;} return true;};
        // Returns if the polymonial is a simple monomonial
        inline bool is_simple_monomonial() const {return monomonials_number() <= 1;};
        // Returns the knows monomonial
        inline __Monomonial known_monomonial() const {for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {if(a_monomonials.at(i).unknowns_number() <= 0) return a_monomonials.at(i);}return __Monomonial(Complex(0));};
        // Returns a monomonial by its unknown
        __Monomonial monomonial(std::string unknown, Complex exponent);
        inline __Monomonial monomonial(std::string unknown) {return monomonial(unknown, Complex(1, 0));};
        inline __Monomonial monomonial() {if(monomonials().size() > 0){return monomonials()[0];}return __Monomonial(0);};
        // Returns the number of monomonials in the polymonial
        inline int monomonials_number() const {return a_monomonials.size();};
        // Returns a list of unknowns monomonials
        std::vector<__Monomonial> unknown_monomonials() const;

        // Returns the polymonial to a GLSL function
        std::string to_glsl(Textual_Math_Settings* settings) const;
        // Returns the polymonial to std::string
        std::string to_std_string(Textual_Math_Settings* settings) const;

        // Methods operators
        // Add a polymonial to this one
        void __add(Polymonial value);
        // Divide a monomonial to this void
        inline void __divide(__Monomonial value) {__Monomonial used_inverse = value.inverse();for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {a_monomonials[i] *= used_inverse;}};
        inline void __divide(Complex value) {__divide(__Monomonial(value));};
        inline void __divide(Fraction value) {__divide(__Monomonial(value));};
        void __divide(Polymonial* value);
        // Returns if two values are equals or not
        inline bool __is_equal(int value) const{return __is_equal(Complex(value));};
        inline bool __is_equal(Fraction value) const{return __is_equal(Complex(value));};
        bool __is_equal(Complex value) const;
        bool __is_equal(Polymonial value) const;
        // Multiply a polymonial to this one
        void __multiply(Polymonial value);
        inline void __multiply(Complex value){Polymonial temp; temp.add_monomonial(value);__multiply(temp);};
        inline void __multiply(Fraction value) {Polymonial temp;temp.add_monomonial(__Monomonial(value));__multiply(temp);};

        // Returns the maximum degree in the polymonial
        Complex degree(std::string unknown_name);
        // Returns a polymonial from a monomonial where the unknows has been replaced
        static Polymonial polymonial_from_modified_monomonial_unknows(__Monomonial used_monomonial, std::string unknown, Polymonial new_value);
        // Returns this polymonial where an unknown is replaced by an another unknown
        Polymonial replace_unknown(std::string unknown, Polymonial new_value) const;
        // Simplify the polymonial
        Polymonial simplify() const;

        // Operators
        // With Complex
        bool operator!=(Complex value) const {return !(*this == value);};
        bool operator==(Complex value)const{return __is_equal(value);};
        Polymonial operator*(Complex value){Polymonial to_return(*this);to_return.__multiply(value);return to_return;}
        Polymonial& operator/=(Complex value) {__divide(value);return*this;};
        // With fractions
        Polymonial& operator*=(Fraction value) {__multiply(value);return*this;};
        Polymonial& operator/=(Fraction value) {__divide(value);return*this;};
        // With polymonials
        Polymonial operator-(Polymonial value) const {Polymonial to_return(*this);to_return-=value;return to_return;};
        Polymonial& operator-=(Polymonial value) {value*=Fraction(-1);__add(value);return*this;};
        Polymonial& operator+=(Polymonial value) {__add(value);return*this;};
        Polymonial& operator*=(Polymonial value) {__multiply(value);return*this;};
        // With monomonials
        Polymonial& operator+=(__Monomonial value) {add_monomonial(value);return*this;};
        Polymonial& operator/=(__Monomonial value) {__divide(value);return*this;};

        // Getters and setters
        inline std::vector<__Monomonial>& monomonials() {return a_monomonials;};

    private:

        // Each monomonial in the polymonial
        std::vector<__Monomonial> a_monomonials = std::vector<__Monomonial>();
	};

	// Stream operator overloading
    std::ostream& operator<<(std::ostream& os, Polymonial& obj);
	// Divide operator
    Polymonial operator/(__Monomonial& obj, __Monomonial const& other);

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

        // Divide a polymonial to this one
        virtual void __divide(Field value) {__divide(value.a_element_add);};
        virtual void __divide(E value) {};
        virtual void __divide(Fraction value) {a_element_add /= value;};

        // Multiply a polymonial to this one
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

    //*********
	//
	// The "Formula" class
	//
	//*********

	class __Formula_Base {
        // Class representating the base of a mathematical formula
    public:

        // Container of unknowns
        struct Unknown{std:: string name = std::string();std::shared_ptr<__Formula_Base> value = std::make_shared<__Formula_Base>();void set_value(__Formula_Base formula){value = formula.formula_copy();};};;
        class Unknowns_Container {
        public:
            // Unknowns_Container constructor
            Unknowns_Container(__Formula_Base value):a_default_value(std::make_shared<__Formula_Base>(value)){};
            Unknowns_Container(){};

            // Handle unknown
            // Creates a unknown
            Unknown* create_unknown(std::string name){return create_unknown_shared_ptr(name).get();};
            std::shared_ptr<Unknown> create_unknown_shared_ptr(std::string name){std::shared_ptr<Unknown> unknown=unknown_shared_ptr_by_name(name);if(unknown.get()!=0){return unknown;}unknown=std::make_shared<Unknown>();a_unknowns.push_back(unknown);unknown.get()->name=name;return unknown;};
            // Returns an unknown by its name
            Unknown* unknown_by_name(std::string name)const{return unknown_shared_ptr_by_name(name).get();};
            std::shared_ptr<Unknown> unknown_shared_ptr_by_name(std::string name)const{for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns.at(i).get()->name == name){return a_unknowns.at(i);}} return std::shared_ptr<Unknown>();};

            // Returns a value by its name
            __Formula_Base* value_by_name(std::string name){Unknown*temp=unknown_by_name(name);if(temp!=0){return temp->value.get();}return a_default_value.get();};
        private:
            // Default value for an unknown
            std::shared_ptr<__Formula_Base> a_default_value = std::make_shared<__Formula_Base>();
            // Unknowns
            std::vector<std::shared_ptr<Unknown>> a_unknowns;
        };

        // Function possible for a formula
        class __Formula_Base_Function {
            public:
                // __Formula_Base_Function constructor
                __Formula_Base_Function(std::string new_name):a_name(new_name) {};
                __Formula_Base_Function(const __Formula_Base_Function& function_copy):a_name(function_copy.a_name){};

                // Definition set of the function
                virtual Set_Number definition_set() = 0;
                // Derivate value
                virtual std::shared_ptr<__Formula_Base> derivate_value(__Formula_Base formula) = 0;
                // Real value
                virtual double real_value(__Formula_Base* formula) = 0;
                // Simplify a value with the function
                virtual std::shared_ptr<__Formula_Base> simplify(__Formula_Base* value) = 0;

                // Copies and returns this function
                virtual std::shared_ptr<__Formula_Base_Function> function_copy() = 0;

                // Getters and setters
                std::string name() const {return a_name;};
            protected:
                // Name of the function
                const std::string a_name;
        };

        //*********
        // Sub-classes of formulas
        //*********

        // Factor of formulas
        struct Formula_Factor {
            // Constructors
            Formula_Factor(){};
            Formula_Factor(std::shared_ptr<__Formula_Base> base){a_factors.push_back(base);};
            Formula_Factor(__Formula_Base base):Formula_Factor(std::make_shared<__Formula_Base>(base)){};
            Formula_Factor(int base):Formula_Factor(std::make_shared<__Formula_Base>(base)){};
            Formula_Factor(const Formula_Factor& formula_factor_copy):a_factors(formula_factor_copy.a_factors){};

            // Returns the formula factor to a MathML / std::string
            std::string to_mathml(Textual_Math_Settings* settings) const;
            std::string to_std_string(Textual_Math_Settings* settings) const;

            // Returns the basic formula
            inline bool basic_formula_exists()const{for(int i = 0;i<static_cast<int>(a_factors.size());i++){if(a_factors.at(i).get()->is_basic()){return true;}}return false;};
            inline std::shared_ptr<__Formula_Base> basic_formula_shared_ptr(){for(int i = 0;i<static_cast<int>(a_factors.size());i++){if(a_factors.at(i).get()->is_basic()){return a_factors.at(i);}}std::shared_ptr<__Formula_Base>to_add=std::make_shared<__Formula_Base>();a_factors.insert(a_factors.begin(), to_add);return a_factors.at(0);};
            inline __Formula_Base* basic_formula(){return basic_formula_shared_ptr().get();};

            // Returns a copy of the factor
            inline std::shared_ptr<Formula_Factor> factor_copy(){std::shared_ptr<Formula_Factor> to_return = std::make_shared<Formula_Factor>();for(int i = 0;i<static_cast<int>(a_factors.size());i++){to_return.get()->a_factors.push_back(a_factors.at(i).get()->formula_copy());}return to_return;};
            // Form of the formula
            inline bool is_simple_monomonial() const {return a_factors.size() == 0 || (a_factors.size() == 1 && a_factors[0].get()->is_simple_monomonial());};
            inline bool is_simple_polymonial() const {return a_factors.size() == 0 || (a_factors.size() == 1 && a_factors[0].get()->is_simple_polymonial());};
            // Converts the formula to a polymonial
            inline Polymonial to_polymonial() const {for(int i = 0;i<static_cast<int>(a_factors.size());i++){if(a_factors.at(i).get()->is_simple_polymonial()){return a_factors.at(i).get()->to_polymonial();}}return Polymonial();};
            operator Polymonial() const {return to_polymonial();};

            // Divides a formula to this one
            void __divide(__Formula_Base value);
            inline void __divide(Polymonial value){__divide(__Formula_Base(value));};
            inline void __divide(__Monomonial value){__divide(__Formula_Base(value));};
            // Multiplies the factor by a polymonial
            inline void __multiply(Polymonial value) {if(!basic_formula_exists()){basic_formula()->set_polymonial(value);}else{basic_formula()->__multiply(value);}};
            void __multiply(__Formula_Base value);

            // Returns if two numbers/formulas are equals
            bool __is_equal(int value)const{return __is_equal(scls::Fraction(value));};
            bool __is_equal(Fraction value)const{return a_factors.size() == 1 && a_factors[0].get()->__is_equal(value);};
            bool __is_equal(Polymonial value)const{return a_factors.size() == 1 && a_factors[0].get()->__is_equal(value);};
            bool __is_equal(__Formula_Base value)const{return a_factors.size() == 1 && a_factors.at(0).get()->__is_equal(value);};

            // Replaces unknowns in the formula
            __Formula_Base replace_unknown(std::string unknown, __Formula_Base new_value) const;

            // Operators
            Formula_Factor& operator*=(__Formula_Base value) {__multiply(value);return*this;};

            // Returns the final value of the formula
            scls::Complex value(__Formula_Base::Unknowns_Container* values);

            // Factors
            std::vector<std::shared_ptr<__Formula_Base>> a_factors;
            inline std::vector<std::shared_ptr<__Formula_Base>>& factors(){return a_factors;};
        };

        // Sum of formula factor
        class Formula_Sum {
        public:
            // Constructors
            Formula_Sum(){};
            Formula_Sum(std::shared_ptr<Formula_Factor> base){a_formulas_add.push_back(base);};
            Formula_Sum(Formula_Factor base):Formula_Sum(std::make_shared<Formula_Factor>(base)){};
            Formula_Sum(__Formula_Base base):Formula_Sum(std::make_shared<Formula_Factor>(base)){};
            Formula_Sum(std::shared_ptr<__Formula_Base> base):Formula_Sum(std::make_shared<Formula_Factor>(base)){};
            Formula_Sum(int base):Formula_Sum(std::make_shared<Formula_Factor>(base)){};
            Formula_Sum(const Formula_Sum& formula_factor_copy){for(int i = 0;i<static_cast<int>(formula_factor_copy.a_formulas_add.size());i++){a_formulas_add.push_back(formula_factor_copy.a_formulas_add.at(i).get()->factor_copy());}};

            // Returns the polymonial to mathml / std::string
            inline std::string to_mathml(Textual_Math_Settings* settings) const{std::string to_return = std::string();for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){to_return+=a_formulas_add.at(i).get()->to_mathml(settings);if(i<static_cast<int>(a_formulas_add.size())-1){to_return += std::string("<mo>+</mo>");}}return to_return;};
            inline std::string to_std_string(Textual_Math_Settings* settings) const{std::string to_return = std::string();for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){to_return+=a_formulas_add.at(i).get()->to_std_string(settings);if(i<static_cast<int>(a_formulas_add.size())-1){to_return += std::string("+");}}return to_return;};

            // Form of the formula
            inline bool is_simple_monomonial() const {return a_formulas_add.size() == 1 && a_formulas_add[0].get()->is_simple_monomonial();};
            inline bool is_simple_polymonial() const {return a_formulas_add.size() == 1 && a_formulas_add[0].get()->is_simple_polymonial();};
            // Returns a copy of the sum
            inline std::shared_ptr<Formula_Sum> sum_copy(){std::shared_ptr<Formula_Sum> to_return = std::make_shared<Formula_Sum>();for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){to_return.get()->a_formulas_add.push_back(a_formulas_add.at(i).get()->factor_copy());}return to_return;};
            // Converts the formula to a polymonial / monomonial
            inline Polymonial to_polymonial() const {for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){if(a_formulas_add.at(i).get()->is_simple_polymonial()){return a_formulas_add.at(i).get()->to_polymonial();}}return Polymonial();};
            operator Polymonial() const {return to_polymonial();};

            // Methods operators
            void __add(__Formula_Base value){a_formulas_add.push_back(std::make_shared<Formula_Factor>(value));};
            // Divide a formula to this one
            void __divide(__Monomonial value){for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){a_formulas_add.at(i).get()->__divide(value);}};
            // Returns if two numbers/formulas are equals
            bool __is_equal(int value)const{return __is_equal(scls::Fraction(value));};
            bool __is_equal(Fraction value)const{return __is_equal(Polymonial(value));};
            bool __is_equal(Polymonial value)const{return a_formulas_add.size() == 1 && a_formulas_add.at(0).get()->__is_equal(value);};
            bool __is_equal(__Formula_Base value)const{return a_formulas_add.size() == 1 && a_formulas_add.at(0).get()->__is_equal(value);};

            // Multiply a polymonial to this one
            virtual void __multiply(Polymonial value) {for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){a_formulas_add.at(i).get()->__multiply(value);}};
            virtual void __multiply(__Formula_Base value) {std::cout << "J " << value.to_std_string(0) << std::endl; for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){a_formulas_add.at(i).get()->__multiply(value);}};

            // Returns the final value of the formula
            scls::Complex value(__Formula_Base::Unknowns_Container* values);

            // Getters and setters
            inline std::vector<std::shared_ptr<Formula_Factor>>& formulas_add(){return a_formulas_add;};
        private:
            // Attached add polymonials (factorized)
            std::vector<std::shared_ptr<Formula_Factor>> a_formulas_add;
        };

        // Fraction of formulas
        class Formula_Fraction {
        public:
            // Constructors
            Formula_Fraction(){};
            Formula_Fraction(std::shared_ptr<Formula_Sum> base):a_numerator(base){};
            Formula_Fraction(std::shared_ptr<Formula_Sum> base_1, std::shared_ptr<Formula_Sum> base_2):a_denominator(base_2),a_numerator(base_1){};
            Formula_Fraction(Formula_Sum base):Formula_Fraction(std::make_shared<Formula_Sum>(base)){};
            Formula_Fraction(__Formula_Base base):Formula_Fraction(std::make_shared<Formula_Sum>(base)){};
            Formula_Fraction(std::shared_ptr<__Formula_Base> base):Formula_Fraction(std::make_shared<Formula_Sum>(base)){};
            Formula_Fraction(int base):Formula_Fraction(std::make_shared<Formula_Sum>(base)){};
            Formula_Fraction(const Formula_Fraction& formula_factor_copy):a_denominator(formula_factor_copy.denominator_copy()),a_numerator(formula_factor_copy.numerator_copy()){};

            // Cleart the denominator in the fraction
            inline void clear_denominator(){a_denominator.reset();};
            // Returns the polymonial to mathml / std::string
            inline std::string to_mathml(Textual_Math_Settings* settings) const{std::string to_return = a_numerator.get()->to_mathml(settings);if(a_denominator.get() != 0){to_return = std::string("<mfrac><mi>") + to_return + std::string("</mi><mi>") + a_denominator.get()->to_mathml(settings) + std::string("</mi></mfrac>");}return to_return;};
            inline std::string to_std_string(Textual_Math_Settings* settings) const{std::string to_return = a_numerator.get()->to_std_string(settings);if(a_denominator.get() != 0){to_return = std::string("(") + to_return + std::string(")/(") + a_denominator.get()->to_std_string(settings) + std::string(")");}return to_return;};

            // Returns a copy of this formula
            inline std::shared_ptr<Formula_Sum> denominator_copy()const{if(a_denominator.get()==0){return std::shared_ptr<Formula_Sum>();}return a_denominator.get()->sum_copy();};
            inline std::shared_ptr<Formula_Fraction> fraction_copy() {return std::make_shared<Formula_Fraction>(numerator_copy(), denominator_copy());};
            inline std::shared_ptr<Formula_Sum> numerator_copy()const{if(a_numerator.get()==0){return std::shared_ptr<Formula_Sum>();} return a_numerator.get()->sum_copy();};
            // Form of the formula
            inline bool is_simple_monomonial() const {return a_denominator.get() == 0 && a_numerator.get()->is_simple_monomonial();};
            inline bool is_simple_polymonial() const {return a_denominator.get() == 0 && a_numerator.get()->is_simple_polymonial();};
            // Converts the formula to a polymonial
            inline Polymonial to_polymonial() const {return a_numerator.get()->to_polymonial();};
            operator Polymonial() const {return to_polymonial();};

            // Methods operators
            void __add(__Formula_Base value){if(a_denominator.get() != 0){value.__multiply(*a_denominator.get());}a_numerator.get()->__add(value);};
            // Divide a formula to this one
            void __divide(__Formula_Base value);
            // Returns if two numbers/formulas are equals
            bool __is_equal(int value)const{return __is_equal(scls::Fraction(value));};
            bool __is_equal(Fraction value)const{return a_numerator.get() == 0 && a_numerator.get()->__is_equal(value);};
            bool __is_equal(Polymonial value)const{return a_numerator.get() == 0 && a_numerator.get()->__is_equal(value);};
            // Multiply a polymonial to this one
            virtual void __multiply(Polymonial value) {if(a_denominator.get() != 0 && a_denominator.get()->__is_equal(value)){a_denominator.reset();}else{a_numerator.get()->__multiply(value);}};
            virtual void __multiply(__Formula_Base value) {std::cout << "I " << value.to_std_string(0) << " " << a_numerator.get() << std::endl;if(a_denominator.get() != 0 && a_denominator.get()->__is_equal(value)){std::cout << "P1" << std::endl;a_denominator.reset();}else{std::cout << "P2" << std::endl;a_numerator.get()->__multiply(value);}};

            // Returns the final value of the formula
            scls::Complex value(__Formula_Base::Unknowns_Container* values);

            // Getters and setters
            Formula_Sum* denominator(){return a_denominator.get();};
            Formula_Sum* numerator(){return a_numerator.get();};
        private:
            // Denominator of the fraction
            std::shared_ptr<Formula_Sum> a_denominator;
            // Numerator of the fraction
            std::shared_ptr<Formula_Sum> a_numerator = std::make_shared<Formula_Sum>();
        };

        // __Formula_Base constructor
        __Formula_Base(){};
        __Formula_Base(int number):a_polymonial(std::make_shared<Polymonial>(number)){};
        __Formula_Base(Fraction number):a_polymonial(std::make_shared<Polymonial>(number)){};
        __Formula_Base(Complex number):a_polymonial(std::make_shared<Polymonial>(number)){};
        __Formula_Base(__Monomonial monomonial):a_polymonial(std::make_shared<Polymonial>(monomonial)){};
        __Formula_Base(Polymonial polymonial):a_polymonial(std::make_shared<Polymonial>(polymonial)){};
        __Formula_Base(Formula_Fraction frac){set_fraction(frac);};
        __Formula_Base(Formula_Sum sum){if(sum.is_simple_polymonial()){set_polymonial(sum);}else{set_fraction(sum);}};
        __Formula_Base(const __Formula_Base& formula):a_polymonial(formula.polymonial_copy()),a_fraction(formula.fraction_copy()),a_applied_function(formula.applied_function_copy()){};

        // Checks if the formula is well formatted
        void check_formula();
        // Clear the formula
        inline void clear() {a_applied_function.reset();a_fraction.reset();a_polymonial.reset();};
        // Pastes a formula to this one
        virtual void paste(__Formula_Base* value){a_applied_function=value->applied_function_copy();a_fraction = value->fraction_copy();a_polymonial = value->polymonial_copy();};
        // Returns a copy of this formula
        std::shared_ptr<__Formula_Base> formula_copy() {std::shared_ptr<__Formula_Base>to_return=std::make_shared<__Formula_Base>(*this);return to_return;};
        inline std::shared_ptr<Formula_Fraction> fraction_copy()const{if(a_fraction.get()==0){return std::shared_ptr<Formula_Fraction>();} return a_fraction.get()->fraction_copy();};
        inline std::shared_ptr<Polymonial> polymonial_copy()const{if(a_polymonial.get()==0){return std::shared_ptr<Polymonial>();}return std::make_shared<Polymonial>(*a_polymonial.get());};
        // Sub-place the current formula to a "formula add"
        inline void sub_place(){std::shared_ptr<__Formula_Base> needed_copy = formula_copy();clear();a_fraction = std::make_shared<Formula_Fraction>(needed_copy);};

        // Returns the polymonial to mathml
        std::string to_mathml(Textual_Math_Settings* settings) const;
        // Returns the polymonial to std::string
        std::string to_std_string(Textual_Math_Settings* settings) const;

        // Returns the denominator / numerator of the formula
        inline std::shared_ptr<__Formula_Base> denominator()const{if(!has_denominator()){return std::shared_ptr<__Formula_Base>();}return std::make_shared<__Formula_Base>(*a_fraction.get()->denominator());};
        inline bool has_denominator()const{return a_fraction.get() != 0 && a_fraction.get()->denominator() != 0;};
        inline std::shared_ptr<__Formula_Base> numerator()const{if(a_fraction.get() == 0){return std::shared_ptr<__Formula_Base>();}return std::make_shared<__Formula_Base>(*a_fraction.get()->numerator());};
        // Returns the internal value of the formula (without functions)
        inline __Formula_Base internal_value() const {__Formula_Base to_return = *this;to_return.clear_applied_function();return to_return;};
        // Returns if the formula is a basic formula or not
        inline bool is_basic() const {return a_applied_function.get() == 0 && a_exponent.get() == 0;};
        // Returns if the formula is a simple monomonial / polymonial or not
        inline bool is_simple_fraction() const {return is_basic() && a_fraction.get() != 0;};
        inline bool is_simple_monomonial() const {return is_simple_polymonial() && (a_polymonial.get() == 0 || a_polymonial.get()->is_simple_monomonial());};
        inline bool is_simple_polymonial() const {return is_basic() && (a_polymonial.get() != 0 || a_fraction.get() == 0);};
        // Returns the numerator value
        inline __Formula_Base numerator_value() const {if(a_polymonial.get() != 0){return (*a_polymonial.get());} __Formula_Base to_return = *this;to_return.clear_applied_function();to_return.clear_denominator();return to_return;};
        // Sets the value of the formula with a fraction / polymonial
        inline void set_fraction(std::shared_ptr<Formula_Fraction> frac){a_fraction=frac;a_polymonial.reset();};
        inline void set_fraction(Formula_Fraction frac){set_fraction(frac.fraction_copy());};
        inline void set_fraction(std::shared_ptr<Formula_Sum> sum){set_fraction(std::make_shared<Formula_Fraction>(sum));};
        inline void set_fraction(Formula_Sum sum){set_fraction(std::make_shared<Formula_Sum>(sum));};
        inline void set_polymonial(std::shared_ptr<Polymonial> sum){a_polymonial=sum;a_fraction.reset();};
        inline void set_polymonial(Formula_Sum sum){set_polymonial(std::make_shared<Polymonial>(sum.to_polymonial()));};
        inline void set_polymonial(Polymonial polymonial){set_polymonial(std::make_shared<Polymonial>(polymonial));};
        // Converts the formula to a polymonial / monomonial
        inline __Monomonial to_monomonial() const {return to_polymonial().monomonial();};
        inline Polymonial to_polymonial() const {if(a_polymonial.get() != 0){return *a_polymonial.get();}else if(a_fraction.get()==0){return 0;} return a_fraction.get()->to_polymonial();};
        operator Polymonial() const {return to_polymonial();};

        /*// Converts the formula to field of another object
        template <typename E>
        E to_field(std::vector<E> defined_names) {
            int operation_number = 0; E to_return;
            // Add the added element
            scls::Polymonial needed_element = added_element();
            std::cout << "T" << needed_element.monomonials().size() << std::endl;
            for(int i = 0;i<static_cast<int>(needed_element.monomonials().size());i++) {
                scls::__Monomonial &needed_monomonial = needed_element.monomonials()[i];
                std::cout << "G " << needed_monomonial.to_std_string() << std::endl;
                // Browse unknows
                E current_element; int current_operation = 0; scls::Fraction exponent = 1; bool found = false;
                for(int j = 0;j<static_cast<int>(needed_monomonial.unknowns().size());j++) {
                    // Check each unknowns
                    for(int k = 0;k<static_cast<int>(defined_names.size());k++) {
                        std::cout << "U " << k << " " << defined_names[k].name() << " " << defined_names[k].to_std_string() << std::endl;
                        if(defined_names[k].name() == needed_monomonial.unknowns()[j].name()) {
                            // The good element has been found
                            if(current_operation == 0) {current_element = defined_names[k];}
                            else {current_element *= defined_names[k];}
                            exponent = needed_monomonial.unknowns()[j].exponent().real();
                            current_operation++;
                            found = true; break;
                        }
                    }
                }
                // If a symbol has been found
                if(found) {
                    std::cout << "L " << current_element.to_std_string() << " ^ " << exponent << std::endl;
                    for(int l = 1;l<static_cast<int>(exponent.to_double());l++) {
                        current_element *= current_element;
                    }
                    current_element *= needed_monomonial.factor().real();
                    if(operation_number <= 0) {to_return = current_element;}
                    else {to_return += current_element;}
                    operation_number++;
                }
            }
            return to_return;
        };
        //*/

        // Methods operators
        virtual void __add(__Formula_Base* value);
        inline void __add(scls::Fraction value){__Formula_Base temp=value;__add(&temp);};
        inline void __substract(__Formula_Base* value){__Formula_Base temp(*value);temp.__multiply(-1);__add(&temp);};

        // Divide a formula to this one
        void __divide(Polymonial value){__divide(__Formula_Base(value));};
        void __divide(Complex value){__divide(__Formula_Base(value));};
        virtual void __divide(__Formula_Base value);

        // Returns if two numbers/formulas are equals
        bool __is_equal(int value)const{if(a_polymonial.get() != 0){return a_polymonial.get()->__is_equal(value);} return a_fraction.get()->__is_equal(value);};
        bool __is_equal(Fraction value)const{if(a_polymonial.get() != 0){return a_polymonial.get()->__is_equal(value);}return a_fraction.get()->__is_equal(value);};
        bool __is_equal(Polymonial value)const{if(a_polymonial.get() != 0){return a_polymonial.get()->__is_equal(value);}return a_fraction.get()->__is_equal(value);};
        bool __is_equal(__Formula_Base value)const{return a_applied_function.get()->name() == value.a_applied_function.get()->name() && a_fraction.get()->__is_equal(value.internal_value());};

        // Multiply a polymonial to this one
        virtual void __multiply(Polymonial value) {__multiply(__Formula_Base(value));};
        inline void __multiply(int value){__multiply(__Formula_Base(value));};
        inline void __multiply(__Monomonial value){__multiply(__Formula_Base(value));};
        virtual void __multiply(Fraction value) {Polymonial temp;temp.add_monomonial(__Monomonial(value));__multiply(temp);};
        inline void __multiply(Complex value){__multiply(__Formula_Base(value));};
        // Multiply an another formula to this one
        void __multiply(__Formula_Base value);
        void __multiply(Formula_Sum value){__multiply(__Formula_Base(value));};

        // Operators
        // With int
        bool operator==(int value) {return __is_equal(value);};
        __Formula_Base operator*(int value) {__Formula_Base to_return(*this);to_return.__multiply(value);return to_return;};
        __Formula_Base& operator*=(int value) {__multiply(value);return*this;};
        // With fractions
        bool operator==(Fraction value) {return __is_equal(value);};
        __Formula_Base& operator*=(Fraction value) {__multiply(value);return*this;};
        __Formula_Base& operator+=(Fraction value) {__add(value);return*this;};
        // With complex
        __Formula_Base operator*(Complex value) {__Formula_Base to_return(*this);to_return.__multiply(value);return to_return;};
        __Formula_Base& operator*=(Complex value) {__multiply(value);return *this;};
        __Formula_Base& operator/=(Complex value) {__divide(value);return *this;};
        // With monomonial
        __Formula_Base& operator+=(__Monomonial value) {__Formula_Base temp=value;__add(&temp);return*this;};
        __Formula_Base& operator*=(__Monomonial value) {__multiply(value);return*this;};
        // With formulas
        __Formula_Base operator-(__Formula_Base value) const {__Formula_Base to_return(*this);value*=Fraction(-1);to_return.__add(&value);return to_return;};
        __Formula_Base& operator-=(__Formula_Base value) {value*=Fraction(-1);__add(&value);return*this;};
        __Formula_Base operator+(__Formula_Base value) {__Formula_Base to_return(*this);to_return.__add(&value);return to_return;};
        __Formula_Base& operator+=(__Formula_Base value) {__add(&value);return*this;};
        __Formula_Base operator*(__Formula_Base value) {__Formula_Base to_return(*this);to_return.__multiply(value);return to_return;};
        __Formula_Base& operator*=(__Formula_Base value) {__multiply(value);return*this;};
        __Formula_Base operator/(__Formula_Base value) const {__Formula_Base other(*this);other.__divide(value);return other;};
        __Formula_Base& operator/=(__Formula_Base value) {__divide(value);return*this;};

        // Getters and setters
        template<typename T> inline void add_applied_function(std::shared_ptr<T> new_applied_function) {if(applied_function() != 0){sub_place();}set_applied_function(new_applied_function);};
        template<typename T> inline void add_applied_function() {add_applied_function(std::make_shared<T>());};
        template<typename T = __Formula_Base_Function> inline T* applied_function() const {return a_applied_function.get();};
        inline std::shared_ptr<__Formula_Base_Function> applied_function_copy() const {if(a_applied_function.get()==0){return std::shared_ptr<__Formula_Base_Function>();}return a_applied_function.get()->function_copy();};
        template<typename T = __Formula_Base_Function> inline std::shared_ptr<T> applied_function_shared_ptr() const {return a_applied_function;};
        inline void clear_applied_function(){a_applied_function.reset();};
        inline void clear_denominator(){a_fraction.get()->clear_denominator();};
        inline Formula_Fraction* fraction()const{return a_fraction.get();};
        //inline __Formula_Base* denominator() const {return a_fraction.get()->denominator();};
        template<typename T> inline void set_applied_function(std::shared_ptr<T> new_applied_function) {a_applied_function = new_applied_function;if(a_applied_function.get()!=0){std::shared_ptr<__Formula_Base>f=a_applied_function.get()->simplify(this);if(f.get()!=0){paste(f.get());}}};
        template<typename T> inline void set_applied_function() {set_applied_function(std::make_shared<T>());};

        //*********
        // Unknown handling
        //*********

        // "Shared" __Formula_Base
        class Formula {
        public:
            // Formula constructor
            Formula(Complex number):Formula(std::make_shared<__Formula_Base>(number)){};
            Formula(std::shared_ptr<__Formula_Base> needed_formula):a_formula(needed_formula){};
            Formula(int number):Formula(std::make_shared<__Formula_Base>(number)){};
            Formula(__Monomonial monomonial):Formula(std::make_shared<__Formula_Base>(monomonial)){};
            Formula(Polymonial polymonial):Formula(std::make_shared<__Formula_Base>(polymonial)){};
            Formula(__Formula_Base needed_formula):Formula(std::make_shared<__Formula_Base>(needed_formula)){};
            Formula():Formula(std::make_shared<__Formula_Base>()){};

            // Operations from __Formula_Base
            inline void clear()const{a_formula.get()->clear();};
            inline __Formula_Base* formula_base() const {return a_formula.get();};
            inline void paste(__Formula_Base* to_paste)const{a_formula.get()->paste(to_paste);};
            inline void paste(Formula to_paste)const{a_formula.get()->paste(to_paste.a_formula.get());};
            inline Polymonial to_polymonial() const {return a_formula.get()->to_polymonial();};
            inline std::string to_std_string(Textual_Math_Settings* settings) const {return a_formula.get()->to_std_string(settings);};

            // Getters and setters
            inline __Formula_Base_Function* applied_function()const{return a_formula.get()->applied_function();};
            template<typename T> inline void set_applied_function(std::shared_ptr<T> new_applied_function) {a_formula.get()->set_applied_function<T>(new_applied_function);};
            template<typename T> inline void set_applied_function() {a_formula.get()->set_applied_function(std::make_shared<T>());};

            // Operators

            // With int
            bool operator==(int value) {return a_formula.get()->__is_equal(value);};
            Formula& operator*=(int value) {__Formula_Base temp=value;a_formula.get()->__multiply(temp);return*this;};
            // With complex
            Formula& operator*=(Complex value) {__Formula_Base temp=value;a_formula.get()->__multiply(temp);return*this;};
            Formula& operator+=(Complex value) {__Formula_Base temp=value;a_formula.get()->__add(&temp);return*this;};
            Formula& operator/=(Complex value) {a_formula.get()->__divide(value);return*this;};
            // With monomonial
            Formula& operator+=(__Monomonial value) {__Formula_Base temp=value;a_formula.get()->__add(&temp);return*this;};
            Formula& operator*=(__Monomonial value) {a_formula.get()->__multiply(value);return*this;};
            // With __Formula_Base
            Formula operator-(__Formula_Base value) const {Formula to_return(*a_formula.get());value*=Fraction(-1);to_return -= value;return to_return;};
            Formula& operator-=(__Formula_Base value) {value*=Fraction(-1);a_formula.get()->__add(&value);return*this;};
            Formula operator+(__Formula_Base value) {Formula to_return(*a_formula.get());to_return += value;return to_return;};
            Formula& operator+=(__Formula_Base value) {a_formula.get()->__add(&value);return*this;};
            Formula operator*(__Formula_Base value) {Formula to_return(*a_formula.get());to_return *= value;return to_return;};
            Formula& operator*=(__Formula_Base value) {a_formula.get()->__multiply(value);return*this;};
            Formula operator/(__Formula_Base value) const {Formula other(*a_formula.get());other /= value;return other;};
            Formula& operator/=(__Formula_Base value) {a_formula.get()->__divide(value);return*this;};
            // With Formula
            Formula& operator+=(Formula value) {a_formula.get()->__add(value.a_formula.get());return*this;};
            Formula& operator*=(Formula value) {a_formula.get()->__multiply(*value.a_formula.get());return*this;};
            Formula& operator/=(Formula value) {a_formula.get()->__divide(*value.a_formula.get());return*this;};

        private:
            // Shared pointer to the needed formula base
            std::shared_ptr<__Formula_Base> a_formula;
        };

        // Returns all the unknowns in the formula
        std::vector<std::string> all_unknowns();
        // Returns a formula from a monomonial where the unknows has been replaced
        static Formula formula_from_modified_monomonial_unknows(__Monomonial used_monomonial, std::string unknown, __Formula_Base new_value);
        // Returns a formula from a polymonial where the unknows has been replaced
        static Formula formula_from_modified_polymonial_unknows(Polymonial used_polymonial, std::string unknown, __Formula_Base new_value);
        // Returns a monomonial where an unkown is replaced by an another unknown
        Formula replace_unknown(std::string unknown, __Formula_Base new_value) const;
        Formula replace_unknown(std::string unknown, Formula new_value) const;
        // Returns the final value of the formula
        scls::Complex value(Unknowns_Container* values);
        scls::Complex value(scls::Fraction current_value){Unknowns_Container temp = Unknowns_Container(current_value);return value(&temp);};
        inline scls::Fraction value_to_fraction(scls::Fraction current_value){return value(current_value).real();};
        inline scls::Fraction value_to_fraction(){return value(1).real();};
        inline double value_to_double(scls::Fraction current_value){return value(current_value).real().to_double();};
        inline double value_to_double(){return value(1).real().to_double();};

    private:
        // Each parts of the formula are in order

        // Polymonial of the formula (fraction is ignored if a polymonial is used)
        std::shared_ptr<scls::Polymonial> a_polymonial = std::make_shared<scls::Polymonial>();

        // Attached fraction of the formula
        std::shared_ptr<Formula_Fraction> a_fraction;
        // Exponent of the formula
        std::shared_ptr<__Formula_Base> a_exponent;
        // Applied function to the ENTIRE formula
        std::shared_ptr<__Formula_Base_Function> a_applied_function;
    }; typedef __Formula_Base Formula;

    // Cosinus function possible for a formula
    class __Cos_Function : public __Formula_Base::__Formula_Base_Function {
        public:
            // __Formula_Base_Function constructor
            __Cos_Function():__Formula_Base_Function("cos"){};

            // Definition set of the function
            virtual Set_Number definition_set() {Set_Number real = Set_Number::real();return real;};
            // Derivate value
            virtual std::shared_ptr<__Formula_Base> derivate_value(__Formula_Base formula);
            // Real value
            virtual double real_value(__Formula_Base* formula){double value = formula->to_polymonial().known_monomonial().factor().real().to_double();return std::cos(value);};
            // Simplify a value with the function
            virtual std::shared_ptr<__Formula_Base> simplify(__Formula_Base* value) {return std::shared_ptr<__Formula_Base>();};

            // Copies and returns this function
            virtual std::shared_ptr<__Formula_Base_Function> function_copy(){return std::make_shared<__Cos_Function>();};
    };

    // Square root function possible for a formula
    class __Exp_Function : public __Formula_Base::__Formula_Base_Function {
        public:
            // __Exp_Function constructor
            __Exp_Function():__Formula_Base_Function("exp"){};

            // Creates a formula with this function
            static __Formula_Base create_formula(__Formula_Base base){base.add_applied_function<__Exp_Function>();return base;};
            // Definition set of the function
            virtual Set_Number definition_set() {Set_Number real = Set_Number();real.set_real();return real;};
            // Derivate value
            virtual std::shared_ptr<__Formula_Base> derivate_value(__Formula_Base formula){std::shared_ptr<__Formula_Base> to_return = std::make_shared<__Formula_Base>(formula);to_return.get()->set_applied_function<__Exp_Function>();return to_return;};
            // Real value
            virtual double real_value(__Formula_Base* formula){double value = formula->value_to_double();return std::exp(value);};
            // Simplify a value with the function
            virtual std::shared_ptr<__Formula_Base> simplify(__Formula_Base* value) {return std::shared_ptr<__Formula_Base>();};

            // Copies and returns this function
            virtual std::shared_ptr<__Formula_Base_Function> function_copy(){return std::make_shared<__Exp_Function>();};
    };

    // Logarithm function function possible for a formula
    class __Log_Function : public __Formula_Base::__Formula_Base_Function {
        public:
            // __Log_Function constructor
            __Log_Function():__Formula_Base_Function("log"){};

            // Definition set of the function
            virtual Set_Number definition_set() {Set_Number real = Set_Number();real.set_real();return real;};
            // Derivate value
            virtual std::shared_ptr<__Formula_Base> derivate_value(__Formula_Base formula){formula.clear_applied_function();std::shared_ptr<__Formula_Base> to_return=std::make_shared<__Formula_Base>(1);to_return.get()->__divide(formula);return to_return;};
            // Real value
            virtual double real_value(__Formula_Base* formula){double value = formula->value_to_double();return std::log(value);};
            // Simplify a value with the function
            virtual std::shared_ptr<__Formula_Base> simplify(__Formula_Base* value) {return std::shared_ptr<__Formula_Base>();};

            // Copies and returns this function
            virtual std::shared_ptr<__Formula_Base_Function> function_copy(){return std::make_shared<__Log_Function>();};
    };

    // Sinus function possible for a formula
    class __Sin_Function : public __Formula_Base::__Formula_Base_Function {
        public:
            // __Sin_Function constructor
            __Sin_Function():__Formula_Base_Function("sin"){};

            // Definition set of the function
            virtual Set_Number definition_set() {Set_Number real = Set_Number::real();return real;};
            // Derivate value
            virtual std::shared_ptr<__Formula_Base> derivate_value(__Formula_Base formula);
            // Real value
            virtual double real_value(__Formula_Base* formula){double value = formula->to_polymonial().known_monomonial().factor().real().to_double();return std::sin(value);};
            // Simplify a value with the function
            virtual std::shared_ptr<__Formula_Base> simplify(__Formula_Base* value) {return std::shared_ptr<__Formula_Base>();};

            // Copies and returns this function
            virtual std::shared_ptr<__Formula_Base_Function> function_copy(){return std::make_shared<__Sin_Function>();};
    };

    // Square root function possible for a formula
    class __Sqrt_Function : public __Formula_Base::__Formula_Base_Function {
        public:
            // __Formula_Base_Function constructor
            __Sqrt_Function():__Formula_Base_Function("sqrt"){};

            // Creates a formula with this function
            static __Formula_Base create_formula(__Formula_Base base){base.add_applied_function<__Sqrt_Function>();return base;};
            // Definition set of the function
            virtual Set_Number definition_set() {Set_Number real = Set_Number();Interval it; it.set_start(0); it.set_end_infinite(true);real.add_interval(it);return real;};
            // Derivate value
            virtual std::shared_ptr<__Formula_Base> derivate_value(__Formula_Base formula);
            // Real value
            virtual double real_value(__Formula_Base* formula){double value = formula->to_polymonial().known_monomonial().factor().real().to_double();return std::sqrt(value);};
            // Simplify a value with the function
            virtual std::shared_ptr<__Formula_Base> simplify(__Formula_Base* value) {
                if(value->applied_function() == 0) {
                    __Formula_Base inner = value->internal_value();
                    if(inner.is_simple_monomonial()) {
                        Polymonial to_poly = inner.to_polymonial();
                        if(to_poly.is_simple_monomonial()){
                            // Simplify a simple monomonial
                            __Monomonial needed_monomonial = to_poly.monomonial();
                            std::vector<double> needed_exponent; bool good = true;
                            for(int i = 0;i<static_cast<int>(needed_monomonial.unknowns().size());i++){if(needed_monomonial.unknowns()[i].name()!=std::string()){needed_exponent.push_back(std::log2(needed_monomonial.unknowns()[i].exponent().real().to_double()));}}
                            for(int i = 0;i<static_cast<int>(needed_exponent.size());i++){if(needed_exponent[i]!=round(needed_exponent[i])||needed_exponent[i]==0){good=false;break;}}
                            if(good) {
                                // Create the needed monomonial
                                scls::Fraction value = std::sqrt(needed_monomonial.factor().real().to_double());
                                scls::__Monomonial final_monomonial = needed_monomonial;
                                final_monomonial.set_factor(value);
                                for(int i = 0;i<static_cast<int>(final_monomonial.unknowns().size());i++){if(final_monomonial.unknowns()[i].name()!=std::string()){final_monomonial.unknowns()[i].set_exponent(final_monomonial.unknowns()[i].exponent()/2);}}
                                return std::make_shared<__Formula_Base>(final_monomonial);
                            }
                        }
                    }
                }
                return std::shared_ptr<__Formula_Base>();
            };

            // Copies and returns this function
            virtual std::shared_ptr<__Formula_Base_Function> function_copy(){return std::make_shared<__Sqrt_Function>();};
    };

    //*********
	//
	// The parsers class
	//
	//*********

	class String_To_Formula_Parse {
        // Class allowing to properly parse a std::string to a polymonial
    public:
        // String_To_Formula_Parse constructor
        String_To_Formula_Parse(unsigned int level);
        String_To_Formula_Parse():String_To_Formula_Parse(0){};

        // Returns if a std::string is a number or not
        inline bool __string_is_number(char text) const {return (string_is_number(text) || text == '/' || text == 'i' || text == '-');};
        inline bool __string_is_number(std::string text) const {for(int i = 0;i<static_cast<int>(text.size());i++) {if(!__string_is_number(text[i])) return false;} return true;};
        // Returns if a std::string is an operator or not
        inline bool __string_is_operator(char text) const {return (text == '+' || text == '-' || text == '*' || text == '/' || text == '>');};
        // Returns a given first base string to a formula
        Formula __string_to_formula_base(std::string base, std::string used_function = "");

        // Converts a std::string to a Formula
        Formula __string_to_formula_without_division(std::string source);
        Formula __string_to_formula_without_multiplication(std::string source);
        Formula __string_to_formula_without_addition(std::string source);
        Formula string_to_formula(std::string source);

        // Add a function to the defined functions
        inline void add_function(std::string function_name) {if(!contains_function(function_name)) a_functions.push_back(function_name); };
        // Returns if a function is defined or not
        inline bool contains_function(std::string function_name) const {for(int i = 0;i<static_cast<int>(a_functions.size());i++) {if(a_functions.at(i) == function_name) return true; } return false;};
        inline bool contains_function(char function_name) const {std::string str;str+=function_name;return contains_function(str);};
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

    // Use parsers methods outside the class
    __Formula_Base string_to_formula(std::string source);
    __Formula_Base::Formula replace_unknown(__Formula_Base used_formula, std::string unknown, std::string new_value);
}

#endif // SCLS_MATH_POLYMONIAL
