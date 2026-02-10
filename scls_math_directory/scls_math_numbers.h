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

// Include SCLS Math bigger header
#include "../scls_math.h"

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
        // Class representing settings to textually represent mathematical objects
    public:

        // Textual_Math_Settings constructor
        Textual_Math_Settings();

        // Getters and setters
        inline int complex_double()const{return a_complex_double;};
        inline bool hide_if_0() const {return a_hide_if_0;};
        inline bool introduction_in_mathml() const {return a_introduction_in_mathml;};
        void set_complex_double(int new_complex_double){a_complex_double = new_complex_double;};
        void set_hide_if_0(bool new_hide_if_0){a_hide_if_0 = new_hide_if_0;};
        void set_spaces_in_color(bool new_spaces_in_color);
        bool spaces_in_color();
    private:
        // Write the complex with doubles or fraction (-1)
        int a_complex_double = -1;

        // Hides the number if equal to "0"
        bool a_hide_if_0 = true;

        // If the introduction should be in mathml or not
        bool a_introduction_in_mathml = true;

        // Spaces (or not) in colors
        bool a_spaces_in_color = true;
    };

    // XML Settings
    Textual_Math_Settings* textual_math_xml_settings();
    void __load_textual_math_xml_settings();
}

//*********
//
// Useful algebrical tools
//
//*********

namespace scls {

    //*********
    //
    // The Algebra_Element class
    //
    //*********

    class Algebra_Element {
        // Class representing an element in a field
    public:

    	class Algebra_Operator {
    	public:
			// Algebra_Operator constructor
    		Algebra_Operator(){};
    		Algebra_Operator(std::string name):a_name(name){};
    		Algebra_Operator(std::string name, int arity):a_arity(arity),a_name(name){};
    		// Algebra_Operator destructor
    		~Algebra_Operator(){};

    		// Getters and setters
    		inline int arity() const {return a_arity;}
    		inline std::string name() const {return a_name;};
    	private:
    		// Arity of the operator
    		int a_arity = -1;

    		// Name of the operator
    		std::string a_name = std::string();
    	};

    	// Container of unknowns
    	struct __Algebra_Unknown{std::string name = std::string();};
    	class Unknowns_Container {
		public:
			// Unknowns_Container constructor
			Unknowns_Container(){};

			// Clears the container
			void clear();

			// Handle unknown
			// Creates a unknown
			template <typename T> T* create_unknown(std::string name){return create_unknown_shared_ptr<T>(name).get();};
			template <typename T> std::shared_ptr<T> create_unknown_shared_ptr(std::string name){std::shared_ptr<Algebra_Element::__Algebra_Unknown> temp=unknown_shared_ptr_by_name(name);if(temp.get()!=0){return std::shared_ptr<T>();}std::shared_ptr<T> unknown=std::make_shared<T>();a_unknowns.push_back(unknown);unknown.get()->name=name;return unknown;};
			// Returns an unknown by its name
			Algebra_Element::__Algebra_Unknown* unknown_by_name(std::string name)const{return unknown_shared_ptr_by_name(name).get();};
			std::shared_ptr<Algebra_Element::__Algebra_Unknown> unknown_shared_ptr_by_name(std::string name)const{for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns.at(i).get()->name == name){return a_unknowns.at(i);}} return std::shared_ptr<Algebra_Element::__Algebra_Unknown>();};

		private:
			// Unknowns
			std::vector<std::shared_ptr<__Algebra_Unknown>> a_unknowns;
		};

        // Algebra_Element constructor
        Algebra_Element();
        // Algebra_Element destructor
        virtual ~Algebra_Element();

        // Clears the object
        void clear();

        // Return if the element is a final element
        bool is_final_element() const;
        bool is_known() const;
        bool is_unknown() const;

        // Creates a new algebra element of the same type
        void __clone_base(Algebra_Element* e) const;
        virtual void algebra_clone(Algebra_Element* e) const = 0;
        virtual std::shared_ptr<Algebra_Element> algebra_clone() const = 0;
        virtual std::shared_ptr<Algebra_Element> new_algebra_element() const = 0;
        virtual std::shared_ptr<Algebra_Element> new_algebra_element(std::string content) const = 0;

        // Returns if two elements are equal or not
        virtual bool is_equal_without_value(Algebra_Element* other) const;

        // Operates this element with another one
        virtual void operate(Algebra_Element* other, std::string operation) = 0;

        // Available operators for this object
        virtual const std::vector<Algebra_Operator>& operators();

        // Replaces the unknowns
        virtual void replace_unknowns_algebra(Algebra_Element* element, Unknowns_Container* values) const;

        // Returns the first known algebra element of this element
        Algebra_Element* known_algebra_element();

        // Sub-places the element
        void sub_place();

        // Getters and setters
        inline std::vector<std::shared_ptr<Algebra_Element>>& algebra_elements() {return a_elements;};
        inline const std::vector<std::shared_ptr<Algebra_Element>>& algebra_elements_const() const {return a_elements;};
        inline __Algebra_Unknown* algebra_unknown() const {return a_unknown.get();};
        inline Algebra_Operator* algebra_operator() {return &a_operator;};
        inline std::string algebra_operator_name() const {return a_operator.name();};
        inline void set_algebra_operator(std::string new_algebra_operator){a_operator = Algebra_Operator(new_algebra_operator);};

        // Virtual functions

        // Creates the unknown
        virtual __Algebra_Unknown* create_unknown();
        virtual __Algebra_Unknown* new_unknown(std::string unknown_name);

        // Returns the element to a simple std::string
        virtual std::string to_mathml(Textual_Math_Settings* settings) const = 0;
        virtual std::string to_std_string(Textual_Math_Settings* settings) const = 0;

    protected:
        // Parent object
        std::weak_ptr<Algebra_Element> a_parent;
        // This object
        std::weak_ptr<Algebra_Element> a_this_object;

        // Possible unknown
        std::shared_ptr<__Algebra_Unknown> a_unknown;

    private:
        // Every elements in this element
        Algebra_Operator a_operator = Algebra_Operator();
        std::vector<std::shared_ptr<Algebra_Element>> a_elements;
    };
    typedef Algebra_Element::Unknowns_Container Unknowns_Container;

    //*********
    //
    // The __Field_Element class
    //
    //*********

    class __Field_Element {
        // Class representing an element in a field
    public:

        // __Field_Element constructor
        __Field_Element(){};
        // __Field_Element destructor
        virtual ~__Field_Element(){};

        // Returns the element to a simple std::string
        virtual std::string to_mathml(Textual_Math_Settings* settings) const = 0;
        virtual std::string to_std_string(Textual_Math_Settings* settings) const = 0;

        // Returns the inverse / opposite of this element
        virtual std::shared_ptr<__Field_Element> inverse() = 0;
        virtual std::shared_ptr<__Field_Element> opposite() = 0;
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

    extern int __normalize_value;
    class __Fraction_Base : public __Field_Element {
	    // Class representing the base of a fraction
    public:
        //*********
        //
        // __Fraction_Base simple methods
        //
        //*********

        // Most simple fraction constructor
        __Fraction_Base(const __Fraction_Base& to_copy);
        __Fraction_Base(double real);
        __Fraction_Base();
        __Fraction_Base(long long numerator, long long denominator);

        // Returns the absolute value of the fraction
        __Fraction_Base abs() const;
        // Returns a fraction from a double
        static __Fraction_Base from_double(double result);
        // Returns a fraction from a std::string
        static __Fraction_Base from_std_string(std::string content);
        // Returns the inverse of the fraction
        __Fraction_Base opposite() const;
        __Fraction_Base inverse() const;
        // Normalize the fraction
        void normalize_force();
        void __normalize();
        void normalize();
        void normalize(int limit);
        __Fraction_Base normalized() const;
        __Fraction_Base normalized(int limit) const;
        // Sets this fraction as a double
        void set_from_double(double result);
        // Returns the fraction in int
        long long to_int() const;
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

        // Getters and setter
        long long denominator() const;
        long long numerator() const;

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
        void _divide(__Fraction_Base obj);
        __Fraction_Base _divide_without_modification(__Fraction_Base obj) const;

        // Returns if this fraction is equal to another
        bool _equal(__Fraction_Base obj) const;
        bool _equal(int obj) const;

        // Multiplies the fraction with an another Fraction
        void _multiply(__Fraction_Base obj);
        __Fraction_Base _multiply_without_modification(__Fraction_Base obj) const;
        __Fraction_Base _multiply_without_modification(double obj) const;
        __Fraction_Base _multiply_without_modification(int obj) const;
        __Fraction_Base _multiply_without_modification(unsigned int obj) const;

        // Returns the inverse of this element
        virtual std::shared_ptr<__Field_Element> opposite(){return std::make_shared<__Fraction_Base>(-numerator(), denominator());};
        virtual std::shared_ptr<__Field_Element> inverse(){return std::make_shared<__Fraction_Base>(denominator(), numerator());};

        // Substracts an another Fraction to this fraction
        void _substract(__Fraction_Base obj);
        __Fraction_Base _substract_without_modification(__Fraction_Base obj) const;
        // Returns the square root of the fraction
        __Fraction_Base sqrt();

        // Operator overloading with fractions
        // Decrement operator
        __Fraction_Base& operator--(int);
        // Equality operator
        bool operator==(__Fraction_Base obj) const;
        // Greater or equal than than operator
        bool operator>=(__Fraction_Base r) const;
        // Greater than than operator
        bool operator>(__Fraction_Base r) const;
        bool operator>(double r) const;
        // Increment operator
        __Fraction_Base& operator++(int);
        // Lesser than than operator
        bool operator<(__Fraction_Base r) const;
        bool operator<(double r) const;
        // Lesser or equal than than operator
        bool operator<=(__Fraction_Base r) const;
        // Arithmetic operator
        __Fraction_Base operator-(__Fraction_Base obj) const;
        __Fraction_Base& operator-=(__Fraction_Base obj);
        __Fraction_Base operator*(__Fraction_Base obj) const;
        __Fraction_Base& operator*=(__Fraction_Base obj);
        __Fraction_Base operator+(__Fraction_Base obj) const;
        __Fraction_Base& operator+=(__Fraction_Base obj);
        __Fraction_Base operator/(__Fraction_Base obj) const;
        __Fraction_Base& operator/=(__Fraction_Base obj);
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
	    // Class representing a precise (but slow and unstable) fraction
    public:
        //*********
        //
        // Fraction simple methods
        //
        //*********

    	// Returns a fraction from a double
    	static Fraction from_double(double result);

        // Most simple fraction constructor
        Fraction(double real):__Fraction_Base(real){};
        Fraction():__Fraction_Base(0){};
        // Simple fraction constructor
        Fraction(long long numerator, long long denominator) : __Fraction_Base(numerator, denominator) {}
        // Fraction copy constructor
        Fraction(const __Fraction_Base& to_copy) : __Fraction_Base(to_copy) {};

        // Returns the real value
        inline Fraction real() const {return *this;};

        // Arithmetic operator
        Fraction operator-(__Fraction_Base obj) const { return _substract_without_modification(obj); };
        Fraction& operator-=(__Fraction_Base obj) { _substract(obj); return *this; }
        Fraction operator*(__Fraction_Base obj) const { return _multiply_without_modification(obj); };
        Fraction& operator*=(__Fraction_Base obj) { _multiply(obj); return *this; };
        Fraction operator+(__Fraction_Base obj) const { return _add_without_modification(obj); };
        Fraction& operator+=(__Fraction_Base obj) { _add(obj); return *this; }
        // Comparaison operator
        bool operator==(__Fraction_Base obj) const { return __Fraction_Base::operator==(obj); };
        bool operator!=(__Fraction_Base obj) const { return !__Fraction_Base::operator==(obj); };
	};

	// Comparaison operator
    bool operator>(double obj_1, __Fraction_Base obj);
    bool operator<(double obj_1, __Fraction_Base obj);
	// Multiplication operator
    __Fraction_Base operator*(int obj_1, __Fraction_Base obj);
    Fraction operator*(int obj_1, Fraction obj);
    // Minus operator
    __Fraction_Base operator-(int obj_1, __Fraction_Base obj);
    Fraction operator-(int obj_1, Fraction obj);
	// Stream operator overloading (indev)
    std::ostream& operator<<(std::ostream& os, const __Fraction_Base& obj);
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
        // Class representing the limit of a formula
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
