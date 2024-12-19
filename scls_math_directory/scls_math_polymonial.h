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
        // Unknows of the monomonial
        inline std::vector<_Base_Unknown>& unknowns() {return a_unknowns;};
        inline unsigned int unknowns_number() const {unsigned int to_return = 0;for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns[i].name() != "")to_return++;}return to_return;};

        // Returns the monomonial converted to std::string
        std::string to_std_string() const;

        // Operators
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

        // Polymonial constructor
        Polymonial(){};
        Polymonial(int number){a_monomonials.push_back(__Monomonial(Complex(number)));};
        Polymonial(Fraction number){a_monomonials.push_back(__Monomonial(Complex(number)));};
        Polymonial(__Monomonial monomonial){a_monomonials.push_back(monomonial);};
        Polymonial(Complex factor, std::string unknow, Complex exponent):Polymonial(__Monomonial(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow, exponent)))){};
        // Polymonial copy constructor
        Polymonial(const Polymonial& polymonial_copy):a_monomonials(polymonial_copy.a_monomonials){};

        // Returns the limit of a (only monomonial) polymonial for an unknown
        scls::Limit limit(Limit needed_limit, std::string unknown_name);

        // Add a new monomonial to the polymonial
        void add_monomonial(__Monomonial new_monomonial);
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
        // Returns the number of monomonials in the polymonial
        inline int monomonials_number() const {return a_monomonials.size();};
        // Returns a list of unknowns monomonials
        std::vector<__Monomonial> unknown_monomonials() const;

        // Returns the polymonial to std::string
        std::string to_std_string() const;

        // Methods operators
        // Add a polymonial to this one
        void __add(Polymonial value);
        // Divide a monomonial to this void
        inline void __divide(Fraction value) {Fraction used_inverse = value.inverse();for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {a_monomonials[i] *= used_inverse;}};
        inline void __divide(__Monomonial value) {__Monomonial used_inverse = value.inverse();for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {a_monomonials[i] *= used_inverse;}};
        // Multiply a polymonial to this one
        void __multiply(Polymonial value);
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
        bool operator==(Complex value) const;
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

	template <typename E>
	class Field {
        // Class representating a mathematical element field
    public:
        // Field constructor
        Field(){};
        Field(E base_value):a_element_add(base_value){};

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
	// The "Formula" class
	//
	//*********

	class __Formula_Base : public Field<scls::Polymonial> {
        // Class representating the base of a mathematical formula
    public:

        // Function possible for a formula
        class __Formula_Base_Function {
            public:
                // __Formula_Base_Function constructor
                __Formula_Base_Function(){};
                __Formula_Base_Function(std::string new_name):a_name(new_name) {};

                // Getters and setters
                std::string name() const {return a_name;};
            private:
                // Name of the function
                std::string a_name = "";
        };

        // __Formula_Base constructor
        __Formula_Base():Field<scls::Polymonial>(){};
        __Formula_Base(int number):Field<scls::Polymonial>(number){};
        __Formula_Base(Fraction fraction):__Formula_Base(__Monomonial(scls::Complex(fraction))){};
        __Formula_Base(__Monomonial monomonial):Field<scls::Polymonial>(monomonial){};
        __Formula_Base(Polymonial polymonial):Field<scls::Polymonial>(polymonial){};
        // __Formula_Base copy constructor
        __Formula_Base(const __Formula_Base& formula):Field<scls::Polymonial>(formula.added_element()),a_formulas_add(formula.a_formulas_add),a_formulas_factor(formula.a_formulas_factor),a_polymonial_factor(formula.a_polymonial_factor),a_applied_function(formula.a_applied_function){if(formula.a_denominator.get() != 0) {a_denominator = std::make_shared<__Formula_Base>(*formula.a_denominator.get());}};

        // Clear the formula
        inline void clear() {a_formulas_add.clear();a_formulas_factor.clear();set_added_element(0);a_polymonial_factor=1;};
        // Returns the polymonial to std::string
        std::string to_std_string() const;

        // Returns the internal value of the formula (without functions)
        inline __Formula_Base internal_value() const {__Formula_Base to_return = *this;to_return.set_applied_function("");return to_return;};
        // Returns if the formula is a basic formula or not
        inline bool is_basic() const {return a_applied_function.get() == 0 && a_denominator.get() == 0;};
        // Returns if the formula is a simple monomonial / polymonial or not
        inline bool is_simple_monomonial() const {return is_basic() && a_formulas_add.size() <= 0 && (a_formulas_factor.size() <= 0 || a_polymonial_factor == 0) && added_element().monomonials_number() <= 1;};
        inline bool is_simple_polymonial() const {return is_basic() && a_formulas_add.size() <= 0 && (a_formulas_factor.size() <= 0 || a_polymonial_factor == 0);};

        // Returns a formula from a monomonial where the unknows has been replaced
        static __Formula_Base formula_from_modified_monomonial_unknows(__Monomonial used_monomonial, std::string unknown, __Formula_Base new_value);
        // Returns a formula from a polymonial where the unknows has been replaced
        static __Formula_Base formula_from_modified_polymonial_unknows(Polymonial used_polymonial, std::string unknown, __Formula_Base new_value);
        // Returns a monomonial where an unkown is replaced by an another unknown
        __Formula_Base replace_unknown(std::string unknown, __Formula_Base new_value) const;

        // Converts the formula to field of another object
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
                E current_element; int current_operation = 0; bool found = false;
                for(int j = 0;j<static_cast<int>(needed_monomonial.unknowns().size());j++) {
                    // Check each unknowns
                    for(int k = 0;k<static_cast<int>(defined_names.size());k++) {
                        std::cout << "U " << k << " " << defined_names[k].name() << " " << defined_names[k].to_std_string() << std::endl;
                        if(defined_names[k].name() == needed_monomonial.unknowns()[j].name()) {
                            // The good element has been found
                            if(current_operation == 0) {current_element = defined_names[k];}
                            else {current_element *= defined_names[k];}
                            current_operation++;
                            found = true; break;
                        }
                    }
                }
                // If a symbol has been found
                if(found) {
                    std::cout << "L " << current_element.to_std_string() << std::endl;
                    current_element *= needed_monomonial.factor().real();
                    if(operation_number <= 0) {to_return = current_element;}
                    else {to_return += current_element;}
                    operation_number++;
                }
            }
            return to_return;
        };

        // Methods operators
        // Add a formula to this one
        inline void __add(__Monomonial value) {__add(value);};
        virtual void __add(__Formula_Base value);

        // Divide a formula to this one
        virtual void __divide(__Formula_Base value);
        void __divide(Polymonial value){__divide(__Formula_Base(value));};

        // Returns if two numbers/formulas are equals
        bool __is_equal(int value)const{return a_formulas_add.size() <= 0 && (a_formulas_factor.size() <= 0 || a_polymonial_factor == 0) && a_element_add == value;};
        bool __is_equal(Fraction value)const{return a_formulas_add.size() <= 0 && (a_formulas_factor.size() <= 0 || a_polymonial_factor == 0) && a_element_add == value;};

        // Multiply a polymonial to this one
        virtual void __multiply(Polymonial value) {Field::__multiply(value);a_polymonial_factor *= value;for(int i=0;i<static_cast<int>(a_formulas_add.size());i++)a_formulas_add[i]*=value;};
        void __multiply(__Formula_Base value);
        virtual void __multiply(Fraction value) {Polymonial temp;temp.add_monomonial(__Monomonial(value));__multiply(temp);};

        // Operators
        // With int
        bool operator==(int value) {return __is_equal(value);};
        // With fractions
        bool operator==(Fraction value) {return __is_equal(value);};
        __Formula_Base& operator*=(Fraction value) {__multiply(value);return*this;};
        // With formulas
        __Formula_Base operator-(__Formula_Base value) const {__Formula_Base to_return(*this);value*=Fraction(-1);to_return.__add(value);return to_return;};
        __Formula_Base& operator-=(__Formula_Base value) {value*=Fraction(-1);__add(value);return*this;};
        __Formula_Base operator+(__Formula_Base value) {__Formula_Base to_return(*this);to_return.__add(value);return to_return;};
        __Formula_Base& operator+=(__Formula_Base value) {__add(value);return*this;};
        __Formula_Base operator*(__Formula_Base value) {__Formula_Base to_return(*this);to_return.__multiply(value);return to_return;};
        __Formula_Base& operator*=(__Formula_Base value) {__multiply(value);return*this;};
        __Formula_Base operator/(__Formula_Base value) const {__Formula_Base other(*this);other.__divide(value);return other;};
        __Formula_Base& operator/=(__Formula_Base value) {__divide(value);return*this;};
        // Converts the formula to a polymonial
        inline Polymonial to_polymonial() const {return added_element();};
        operator Polymonial() const {return to_polymonial();};

        // Getters and setters
        inline __Formula_Base_Function* applied_function() const {return a_applied_function.get();};
        inline std::shared_ptr<__Formula_Base_Function> applied_function_shared_ptr() const {return a_applied_function;};
        inline __Formula_Base* denominator() const {return a_denominator.get();};
        inline void set_applied_function(std::shared_ptr<__Formula_Base_Function> new_applied_function) {a_applied_function = new_applied_function;};
        inline void set_applied_function(std::string new_applied_function) {if(new_applied_function==""){a_applied_function.reset();}else{set_applied_function(std::make_shared<__Formula_Base_Function>(new_applied_function));}};

    private:
        // Attached add polymonials
        std::vector<__Formula_Base> a_formulas_add;

        // Attached factors formulas
        std::vector<__Formula_Base> a_formulas_factor;
        // Attached factor of the formulas polymonial
        Polymonial a_polymonial_factor = 1;

        // Applied function to the ENTIRE formula EXCEPTED DENOMINATOR
        std::shared_ptr<__Formula_Base_Function> a_applied_function;

        // Division of the formula
        std::shared_ptr<__Formula_Base> a_denominator;
        // Exponent of the formula
        std::shared_ptr<__Formula_Base> a_exponent;
    }; typedef __Formula_Base Formula;

    //*********
	//
	// The sets class
	//
	//*********

	class Interval {
	    // Class representating an interval of real numbers
    public:

        // Interval constructor
        Interval(Fraction start, Fraction end):a_end(end),a_start(start){};
        Interval():Interval(Fraction(0), Fraction(0)){};

        // Getters and setters
        inline Fraction end() const {return a_end;};
        inline bool end_infinite() const {return a_end_infinite;};
        inline void set_end(Fraction new_end) {a_end=new_end;};
        inline void set_start(Fraction new_start) {a_start=new_start;};
        inline Fraction start() const {return a_start;};
        inline void set_end_infinite(bool new_end_infinite) {a_end_infinite=new_end_infinite;};
        inline void set_start_infinite(bool new_start_infinite) {a_start_infinite=new_start_infinite;};
        inline bool start_infinite() const {return a_start_infinite;};

    private:
        // End of the interval
        Fraction a_end;
        // If the end is + infinite or not
        bool a_end_infinite = false;
        // Start of the interval
        Fraction a_start;
        // If the start is + infinite or not
        bool a_start_infinite = false;
	};

	class Set_Number {
        // Class representating a set of numbers
    public:

        // Set_Number constructor
        Set_Number(){};
        // Set_Number constructor convertors
        Set_Number(Interval interval){a_intervals.push_back(interval);};

        // Add an interval to the set
        inline void add_interval(Interval to_add) {a_intervals.push_back(to_add);__sort_interval();};
        // Add a number to the set
        inline void add_number(Complex to_add) {a_numbers.push_back(to_add);__sort_numbers();};
        inline void add_number(Fraction to_add) {add_number(Complex(to_add));};

        // Returns if the set is empty or not
        inline bool is_empty() const {return a_intervals.size()<=0;};
        // Returns if the set is infinite or not
        inline bool is_infinite() const {return a_intervals.size() > 0 && a_intervals.at(0).start_infinite() && a_intervals.at(0).end_infinite();};

        // Returns the set in a std::string
        std::string to_std_string();

        // Sort the intervals / numbers in the set
        void __sort_interval();
        void __sort_numbers();

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
	// The parsers class
	//
	//*********

	class String_To_Formula_Parse {
        // Class allowing to properly parse a std::string to a polymonial
    public:
        // String_To_Formula_Parse constructor
        String_To_Formula_Parse(unsigned int level):a_level(level){};
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
    __Formula_Base replace_unknown(__Formula_Base used_formula, std::string unknown, std::string new_value);
}

#endif // SCLS_MATH_POLYMONIAL
