//******************
//
// scls_math_formula.h
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
// This file contains some functions to handle complex formula expression.
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

#ifndef SCLS_MATH_FORMULA
#define SCLS_MATH_FORMULA

#include "scls_math_polynomial.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

	//*********
	//
	// The "Formula" class
	//
	//*********

	class __Formula_Base {
	    // Class representating the base of a mathematical formula
    public:

        // Returns a new formula
        virtual std::shared_ptr<__Formula_Base> new_formula() const = 0;

        // Container of unknowns
        struct Unknown{std:: string name = std::string();std::shared_ptr<__Formula_Base> value;void set_value(std::shared_ptr<__Formula_Base> formula){value = formula;};void set_value(__Formula_Base* formula){value = formula->clone();};};
        class Unknowns_Container {
        public:
            // Unknowns_Container constructor
            Unknowns_Container(__Formula_Base* value):a_default_value(value->clone()){};
            Unknowns_Container(){};

            // Clears the container
            void clear();

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
            std::shared_ptr<__Formula_Base> a_default_value;
            // Unknowns
            std::vector<std::shared_ptr<Unknown>> a_unknowns;
        };

        // Function possible for a formula
        class __Formula_Base_Function {
            public:
                // __Formula_Base_Function constructor
                __Formula_Base_Function(std::string new_name, unsigned int arity):a_arity(arity),a_name(new_name) {};
                __Formula_Base_Function(const __Formula_Base_Function& function_copy):a_arity(function_copy.a_arity),a_name(function_copy.a_name){};
                // __Formula_Base_Function destructor
                virtual ~__Formula_Base_Function(){};

                // Definition set of the function
                virtual Set_Number definition_set(){return Set_Number::real();};
                // Derivate value
                virtual std::shared_ptr<__Formula_Base> derivate_value(){return std::shared_ptr<__Formula_Base>();};
                // Multiply a value with the function
                virtual std::shared_ptr<__Formula_Base> multiply(__Formula_Base* value_1, __Formula_Base* value_2){return std::shared_ptr<__Formula_Base>();};
                // Real value
                double real_value(__Formula_Base* formula){return real_value(std::vector<__Formula_Base*>(1, formula));}
                virtual double real_value(std::vector<__Formula_Base*> formula) = 0;
                // Simplify a value with the function
                virtual std::shared_ptr<__Formula_Base> simplify(__Formula_Base* value) = 0;

                // Copies and returns this function
                virtual std::shared_ptr<__Formula_Base_Function> function_copy() = 0;

                // Getters and setters
                inline unsigned int arity() const {return a_arity;};
                inline std::string name() const {return a_name;};
            protected:
                // Arity of the function
                const unsigned int a_arity;
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
            Formula_Factor(__Formula_Base* base):Formula_Factor(base->clone()){};
            Formula_Factor(const Formula_Factor& formula_factor_copy):a_factors(formula_factor_copy.a_factors){};

            // Returns the formula factor to a MathML / std::string
            std::string to_mathml(Textual_Math_Settings* settings) const;
            std::string to_std_string(Textual_Math_Settings* settings) const;

            // Returns the basic formula
            inline bool basic_formula_exists()const{for(int i = 0;i<static_cast<int>(a_factors.size());i++){if(a_factors.at(i).get()->is_basic()){return true;}}return false;};
            inline std::shared_ptr<__Formula_Base> basic_formula_shared_ptr(){for(int i = 0;i<static_cast<int>(a_factors.size());i++){if(a_factors.at(i).get()->is_basic()){return a_factors.at(i);}}std::shared_ptr<__Formula_Base>n=a_factors.at(0).get()->new_formula();a_factors.insert(a_factors.begin(), n);return n;};
            inline __Formula_Base* basic_formula(){return basic_formula_shared_ptr().get();};

            // Returns a copy of the factor
            inline std::shared_ptr<Formula_Factor> factor_copy(){std::shared_ptr<Formula_Factor> to_return = std::make_shared<Formula_Factor>();for(int i = 0;i<static_cast<int>(a_factors.size());i++){to_return.get()->a_factors.push_back(a_factors.at(i).get()->clone());}return to_return;};
            // Form of the formula
            inline bool is_simple_monomonial() const {return a_factors.size() == 0 || (a_factors.size() == 1 && a_factors[0].get()->is_simple_monomonial());};
            inline bool is_simple_polynomial() const {return a_factors.size() == 0 || (a_factors.size() == 1 && a_factors[0].get()->is_simple_polynomial());};
            // Converts the formula to a polynomial
            Polynomial_Base* polynomial() const;

            // Divides a formula to this one
            void __divide(__Formula_Base* value);
            void __divide(Polynomial_Base* value);
            void __divide(__Monomonial_Base* value);
            // Multiplies the factor by a polynomial
            void __multiply(Polynomial_Base* value);
            void __multiply(__Formula_Base* value);

            // Returns if two numbers/formulas are equals
            bool __is_equal(Polynomial_Base* value)const;
            bool __is_equal(__Formula_Base* value)const;

            // Replaces unknowns in the formula
            void replace_unknown(__Formula_Base* to_return, std::string unknown, __Formula_Base* new_value) const;

            // Returns all the unknowns in the formula
            std::vector<std::string> all_unknowns() const;

            // Operators
            Formula_Factor& operator*=(__Formula_Base* value) {__multiply(value);return*this;};

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
            Formula_Sum(__Formula_Base* base):Formula_Sum(base->clone()){};
            Formula_Sum(std::shared_ptr<__Formula_Base> base):Formula_Sum(std::make_shared<Formula_Factor>(base)){};
            Formula_Sum(const Formula_Sum& formula_factor_copy){for(int i = 0;i<static_cast<int>(formula_factor_copy.a_formulas_add.size());i++){a_formulas_add.push_back(formula_factor_copy.a_formulas_add.at(i).get()->factor_copy());}};

            // Returns the polynomial to mathml / std::string
            inline std::string to_mathml(Textual_Math_Settings* settings) const{std::string to_return = std::string();for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){to_return+=a_formulas_add.at(i).get()->to_mathml(settings);if(i<static_cast<int>(a_formulas_add.size())-1){to_return += std::string("<mo>+</mo>");}}return to_return;};
            inline std::string to_std_string(Textual_Math_Settings* settings) const{std::string to_return = std::string();for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){to_return+=a_formulas_add.at(i).get()->to_std_string(settings);if(i<static_cast<int>(a_formulas_add.size())-1){to_return += std::string("+");}}return to_return;};

            // Form of the formula
            inline bool is_simple_monomonial() const {return a_formulas_add.size() == 1 && a_formulas_add[0].get()->is_simple_monomonial();};
            inline bool is_simple_polynomial() const {return a_formulas_add.size() == 1 && a_formulas_add[0].get()->is_simple_polynomial();};
            // Returns a copy of the sum
            inline std::shared_ptr<Formula_Sum> sum_copy(){std::shared_ptr<Formula_Sum> to_return = std::make_shared<Formula_Sum>();for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){to_return.get()->a_formulas_add.push_back(a_formulas_add.at(i).get()->factor_copy());}return to_return;};
            // Converts the formula to a polynomial / monomonial
            Polynomial_Base* polynomial() const;

            // Methods operators
            void __add(__Formula_Base* value){a_formulas_add.push_back(std::make_shared<Formula_Factor>(value->clone()));};
            // Divide a formula to this one
            void __divide(__Monomonial_Base* value){for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){a_formulas_add.at(i).get()->__divide(value);}};
            // Returns if two numbers/formulas are equals
            bool __is_equal(Polynomial_Base* value)const;
            bool __is_equal(__Formula_Base* value)const;

            // Multiply a polynomial to this one
            virtual void __multiply(Polynomial_Base* value);
            virtual void __multiply(__Formula_Base* value);

            // Returns all the unknowns in the formula
            std::vector<std::string> all_unknowns() const;

            // Getters and setters
            inline std::vector<std::shared_ptr<Formula_Factor>>& formulas_add(){return a_formulas_add;};
        private:
            // Attached add polynomials (factorized)
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
            Formula_Fraction(__Formula_Base* base):Formula_Fraction(std::make_shared<Formula_Sum>(base->clone())){};
            Formula_Fraction(std::shared_ptr<__Formula_Base> base):Formula_Fraction(std::make_shared<Formula_Sum>(base)){};
            Formula_Fraction(const Formula_Fraction& formula_factor_copy):a_denominator(formula_factor_copy.denominator_copy()),a_numerator(formula_factor_copy.numerator_copy()){};

            // Cleart the denominator in the fraction
            inline void clear_denominator(){a_denominator.reset();};
            // Returns the polynomial to mathml / std::string
            inline std::string to_mathml(Textual_Math_Settings* settings) const{std::string to_return = a_numerator.get()->to_mathml(settings);if(a_denominator.get() != 0){to_return = std::string("<mfrac><mi>") + to_return + std::string("</mi><mi>") + a_denominator.get()->to_mathml(settings) + std::string("</mi></mfrac>");}return to_return;};
            inline std::string to_std_string(Textual_Math_Settings* settings) const{std::string to_return = a_numerator.get()->to_std_string(settings);if(a_denominator.get() != 0){to_return = std::string("(") + to_return + std::string(")/(") + a_denominator.get()->to_std_string(settings) + std::string(")");}return to_return;};

            // Returns a copy of this formula
            inline std::shared_ptr<Formula_Sum> denominator_copy()const{if(a_denominator.get()==0){return std::shared_ptr<Formula_Sum>();}return a_denominator.get()->sum_copy();};
            inline std::shared_ptr<Formula_Fraction> fraction_copy() {return std::make_shared<Formula_Fraction>(numerator_copy(), denominator_copy());};
            inline std::shared_ptr<Formula_Sum> numerator_copy()const{if(a_numerator.get()==0){return std::shared_ptr<Formula_Sum>();} return a_numerator.get()->sum_copy();};
            // Form of the formula
            inline bool is_simple_monomonial() const {return a_denominator.get() == 0 && a_numerator.get()->is_simple_monomonial();};
            inline bool is_simple_polynomial() const {return a_denominator.get() == 0 && a_numerator.get()->is_simple_polynomial();};
            // Converts the formula to a polynomial
            Polynomial_Base* polynomial() const;

            // Methods operators
            void __add(__Formula_Base* value);
            // Divide a formula to this one
            void __divide(__Formula_Base* value);
            // Returns if two numbers/formulas are equals
            bool __is_equal(Polynomial_Base* value)const{return a_denominator.get() == 0 && a_numerator.get() != 0 && a_numerator.get()->__is_equal(value);};
            bool __is_equal(__Formula_Base* value)const{return a_denominator.get() == 0 && a_numerator.get() != 0 && a_numerator.get()->__is_equal(value);};
            // Multiply a polynomial to this one
            virtual void __multiply(Polynomial_Base* value);
            virtual void __multiply(__Formula_Base* value);

            // Getters and setters
            Formula_Sum* denominator(){return a_denominator.get();};
            Formula_Sum* numerator(){return a_numerator.get();};
            void set_denominator(Formula_Sum* new_denominator){if(new_denominator == 0){a_denominator.reset();}else{a_denominator = new_denominator->sum_copy();}};

            // Returns all the unknowns in the formula
            std::vector<std::string> all_unknowns() const;
        private:
            // Denominator of the fraction
            std::shared_ptr<Formula_Sum> a_denominator;
            // Numerator of the fraction
            std::shared_ptr<Formula_Sum> a_numerator = std::make_shared<Formula_Sum>();
        };

        // __Formula_Base constructor
        __Formula_Base();
        __Formula_Base(Polynomial_Base* polynomial);
        __Formula_Base(Formula_Fraction frac);
        __Formula_Base(Formula_Sum sum);
        __Formula_Base(Formula_Sum* sum);
        __Formula_Base(const __Formula_Base& formula);
        // __Formula_Base destructor
        virtual ~__Formula_Base();

        // Checks if the formula is well formatted
        void check_formula();
        // Clears the formula
        void clear();
        // Clones and returns the formula
        std::shared_ptr<__Formula_Base> clone() const {return std::shared_ptr<__Formula_Base>(clone_new());};
        virtual __Formula_Base* clone_new() const = 0;
        // Creates the polymonial
        virtual std::shared_ptr<Polynomial_Base> create_polynomial() = 0;
        virtual std::shared_ptr<Polynomial_Base> create_polynomial(__Monomonial_Base* monomonial) = 0;
        // Pastes a formula to this one
        virtual void paste(__Formula_Base* value);
        // Returns a copy of this formula
        std::shared_ptr<Formula_Fraction> fraction_copy()const;
        std::shared_ptr<Polynomial_Base> polynomial_copy()const;
        // Soft_resets the formula
        void soft_reset();
        // Sub-place the current formula to a "formula add"
        void sub_place();

        // Returns the polynomial to mathml
        std::string to_mathml(Textual_Math_Settings* settings) const;
        // Returns the polynomial to std::string
        std::string to_std_string(Textual_Math_Settings* settings) const;

        // Returns the denominator / numerator of the formula
        inline std::shared_ptr<__Formula_Base> denominator()const{if(!has_denominator()){return std::shared_ptr<__Formula_Base>();}std::shared_ptr<__Formula_Base>t=new_formula();t.get()->set_polynomial(a_fraction.get()->denominator());return t;};
        inline bool has_denominator()const{return a_fraction.get() != 0 && a_fraction.get()->denominator() != 0;};
        inline std::shared_ptr<__Formula_Base> numerator()const{if(a_fraction.get() == 0){return std::shared_ptr<__Formula_Base>();}std::shared_ptr<__Formula_Base>t=new_formula();t.get()->set_polynomial(a_fraction.get()->numerator());return t;};
        // Returns the internal value of the formula (without functions)
        inline __Formula_Base* __internal_value_new() const {__Formula_Base* to_return = clone_new();to_return->clear_applied_function();return to_return;};
        // Returns if the formula is a basic formula or not
        inline bool is_basic() const {return a_applied_function.get() == 0 && a_exponent.get() == 0;};
        // Returns if the formula is a simple monomonial / polynomial or not
        inline bool is_simple_fraction() const {return is_basic() && a_fraction.get() != 0;};
        inline bool is_simple_monomonial() const {return is_simple_polynomial() && (a_polynomial.get() == 0 || a_polynomial.get()->is_simple_monomonial());};
        inline bool is_simple_polynomial() const {return is_basic() && (a_polynomial.get() != 0 || a_fraction.get() == 0);};
        // Sets the value of the formula with a fraction / polynomial
        inline void set_denominator(std::shared_ptr<Formula_Sum> den){if(a_fraction.get()!=0){a_fraction.get()->set_denominator(den.get());}};
        inline void set_fraction(std::shared_ptr<Formula_Fraction> frac){a_fraction=frac;a_polynomial.reset();};
        inline void set_fraction(Formula_Fraction frac){set_fraction(frac.fraction_copy());};
        inline void set_fraction(std::shared_ptr<Formula_Sum> sum){set_fraction(std::make_shared<Formula_Fraction>(sum));};
        inline void set_fraction(Formula_Sum sum){set_fraction(sum.sum_copy());};
        inline void set_fraction(Formula_Sum* sum){set_fraction(sum->sum_copy());};
        inline void set_polynomial(std::shared_ptr<Polynomial_Base> sum){a_polynomial=sum;a_fraction.reset();};
        inline void set_polynomial(Formula_Sum sum){set_polynomial(sum.polynomial()->clone());};
        inline void set_polynomial(Formula_Sum* sum){if(sum->polynomial() != 0){set_polynomial(sum->polynomial()->clone());}};
        inline void set_polynomial(__Monomonial_Base* polynomial){a_polynomial=create_polynomial();a_polynomial.get()->__add(polynomial);a_fraction.reset();};
        inline void set_polynomial(Polynomial_Base* polynomial){set_polynomial(polynomial->clone());};
        // Converts the formula to a polynomial / monomonial
        __Monomonial_Base* __monomonial(std::string unknown_name) const;
        __Monomonial_Base* __monomonial() const;
        Polynomial_Base* __polynomial() const;
        // Returns a list of formula pointer
        std::vector<__Formula_Base*> formulas_ptr();

        // Apply an operator to this formula
        void __apply(std::string needed_operator, __Formula_Base* value);

        // Methods operators
        void __add(__Formula_Base* value);
        void __substract(__Formula_Base* value);

        // Divides a formula to this one
        void __divide(Polynomial_Base* value);
        void __divide(__Formula_Base* value);

        // Returns if two numbers/formulas are equals
        bool __is_equal(Polynomial_Base* value) const;
        bool __is_equal(__Formula_Base* value) const;

        // Returns the opposite / neutral of the formula
        virtual std::shared_ptr<__Formula_Base> multiplication_neutral() const = 0;
        virtual std::shared_ptr<__Formula_Base> opposite() const = 0;
        virtual void set_multiplication_neutral() = 0;
        // Returns if the formula is a precise object
        virtual bool is_multiplication_neutral() const = 0;
        // Returns if the formula is null or not
        virtual bool is_null() const = 0;

        // Multiply a polynomial to this one
        void __multiply(Polynomial_Base* value);
        void __multiply(__Monomonial_Base* value);
        void __multiply(__Formula_Base* value);
        void __multiply(Formula_Sum* value);

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
        inline std::string* redaction_ptr() {return a_redaction;};
        template<typename T> void set_applied_function(std::shared_ptr<T> new_applied_function) {a_applied_function = new_applied_function;if(a_applied_function.get()!=0){std::shared_ptr<__Formula_Base>f=a_applied_function.get()->simplify(this);if(f.get()!=0){paste(f.get());}}soft_reset();};
        template<typename T> void set_applied_function(int value) {set_applied_function(std::make_shared<T>(value));};
        template<typename T> void set_applied_function() {set_applied_function(std::make_shared<T>());};
        inline void set_redaction(std::string* new_redaction){a_redaction = new_redaction;};

        // Returns all the unknowns in the formula
        std::vector<std::string> all_unknowns() const;
        // Returns a formula from a monomonial where the unknows has been replaced
        static void formula_from_modified_monomonial_unknows(__Formula_Base* final_formula, __Monomonial_Base* used_monomonial, std::string unknown, __Formula_Base* new_value);
        // Returns a formula from a polynomial where the unknows has been replaced
        static void formula_from_modified_polynomial_unknows(__Formula_Base* final_formula, Polynomial_Base* used_polynomial, std::string unknown, __Formula_Base* new_value);
        // Returns a monomonial where an unkown is replaced by an another unknown
        __Formula_Base* replace_unknown_new(std::string unknown, __Formula_Base* new_value) const;
        __Formula_Base* replace_unknowns_new(Unknowns_Container* values) const;

    protected:
        // Already calculated parts
        std::vector<double> a_to_double = std::vector<double>();
        std::vector<Unknowns_Container*> a_to_double_containers = std::vector<Unknowns_Container*>();

    private:
        // Redaction text
        std::string* a_redaction = 0;

        // Each parts of the formula are in order
        // Polynomial of the formula (fraction is ignored if a polynomial is used)
        std::shared_ptr<Polynomial_Base> a_polynomial;

        // Each sub-formulas of this formula
        std::vector<std::shared_ptr<__Formula_Base>> a_formulas;

        // Attached fraction of the formula
        std::shared_ptr<Formula_Fraction> a_fraction;
        // Exponent of the formula
        std::shared_ptr<__Formula_Base> a_exponent;
        // Applied function to the ENTIRE formula
        std::shared_ptr<__Formula_Base_Function> a_applied_function;
	};
    template <typename T> class __Formula_Base_Template : public __Formula_Base {
    public:

        // Returns a new formula
        virtual std::shared_ptr<__Formula_Base> new_formula() const {return std::make_shared<__Formula_Base_Template<T>>();};

        // __Formula_Base_Template constructor
        __Formula_Base_Template(){set_polynomial(create_polynomial());};
        __Formula_Base_Template(T number):__Formula_Base_Template(__Monomonial_Template<T>(number)){};
        __Formula_Base_Template(T* number):__Formula_Base_Template(__Monomonial_Template<T>(*number)){};
        __Formula_Base_Template(int number):__Formula_Base_Template(T(number)){};
        __Formula_Base_Template(__Monomonial_Template<T> monomonial):__Formula_Base_Template(Polynomial_Template<T>(&monomonial)){};
        __Formula_Base_Template(__Monomonial_Template<T>* monomonial):__Formula_Base_Template(Polynomial_Template<T>(monomonial)){};
        __Formula_Base_Template(Polynomial_Template<T> polynomial):__Formula_Base(&polynomial){};
        __Formula_Base_Template(Polynomial_Template<T>* polynomial):__Formula_Base(polynomial){};
        __Formula_Base_Template(Formula_Fraction frac):__Formula_Base(frac){set_polynomial(create_polynomial());};
        __Formula_Base_Template(Formula_Sum sum):__Formula_Base(sum){set_polynomial(create_polynomial());};
        __Formula_Base_Template(const __Formula_Base& formula):__Formula_Base(formula){};

        // Clones this formula
        std::shared_ptr<__Formula_Base_Template<T>> clone() const {return std::shared_ptr<__Formula_Base_Template<T>>(clone_new());};
        virtual __Formula_Base_Template<T>* clone_new() const {return new __Formula_Base_Template<T>(*this);};
        // Creates the polymonial
        virtual std::shared_ptr<Polynomial_Base> create_polynomial(){return std::make_shared<Polynomial_Template<T>>();};
        virtual std::shared_ptr<Polynomial_Base> create_polynomial(__Monomonial_Base* monomonial){return std::make_shared<Polynomial_Template<T>>(monomonial);};

        // Returns the internal value of the formula (without functions)
        std::shared_ptr<__Formula_Base_Template<T>> internal_value() const {return std::shared_ptr<__Formula_Base_Template<T>>(reinterpret_cast<__Formula_Base_Template<T>*>(__internal_value_new()));};

        // Returns the opposite of the formula
        virtual std::shared_ptr<__Formula_Base> multiplication_neutral() const{return std::make_shared<__Formula_Base_Template>(1);};
        virtual std::shared_ptr<__Formula_Base> opposite() const {std::shared_ptr<__Formula_Base_Template<T>>t=clone();t.get()->multiply(T(-1));return t;};
        virtual void set_multiplication_neutral() {*this = __Formula_Base_Template(1);};
        // Returns if the formula is a precise object
        virtual bool is_multiplication_neutral() const{return is_simple_monomonial() && monomonial() != 0 && monomonial()->is_known() && monomonial()->factor() != 0 && (*monomonial()->factor()) == 1;};
        // Returns if the formula is null or not
        virtual bool is_null() const{return (fraction() == 0 && polynomial() == 0) || (polynomial() != 0 && polynomial()->is_null());};

        // Converts the formula to a polynomial / monomonial
        __Monomonial_Template<T>* monomonial(std::string unknown_name) const{return reinterpret_cast<__Monomonial_Template<T>*>(__monomonial(unknown_name));};
        __Monomonial_Template<T>* monomonial() const{return reinterpret_cast<__Monomonial_Template<T>*>(__monomonial());};
        Polynomial_Template<T>* polynomial() const{return reinterpret_cast<Polynomial_Template<T>*>(__polynomial());};
        inline void set_polynomial(T polynomial){__Formula_Base::set_polynomial(std::make_shared<Polynomial_Template<T>>(polynomial));};
        inline void set_polynomial(__Monomonial_Template<T>* polynomial){__Formula_Base::set_polynomial(polynomial->clone());};
        inline void set_polynomial(Polynomial_Template<T>* polynomial){__Formula_Base::set_polynomial(polynomial->clone());};
        inline void set_polynomial(std::shared_ptr<Polynomial_Base> polynomial){__Formula_Base::set_polynomial(polynomial);};

        // Methods operators
        virtual void add(T value){__Formula_Base_Template<T> temp(value);add(&temp);};
        virtual void add(T* value){__Formula_Base_Template<T> temp(value);add(&temp);};
        virtual void add(__Formula_Base_Template<T>* value){__Formula_Base::__add(value);};
        virtual void divide(T value){__Formula_Base_Template<T> temp(value);divide(&temp);};
        virtual void divide(T* value){__Formula_Base_Template<T> temp(value);divide(&temp);};
        virtual void divide(__Formula_Base_Template<T>* value){__Formula_Base::__divide(value);};
        virtual void multiply(T value){__Formula_Base_Template<T> temp(value);multiply(&temp);};
        virtual void multiply(T* value){__Formula_Base_Template<T> temp(value);multiply(&temp);};
        virtual void multiply(__Formula_Base_Template<T>* value){__Formula_Base::__multiply(value);};
        virtual void substract(T value){__Formula_Base_Template<T> temp(value);substract(&temp);};
        virtual void substract(__Formula_Base_Template<T>* value){__Formula_Base::__substract(value);};

        // Returns a monomonial where an unkown is replaced by an another unknown
        std::shared_ptr<__Formula_Base_Template<T>> replace_unknown(std::string unknown, __Formula_Base* new_value) const{return std::shared_ptr<__Formula_Base_Template<T>>(reinterpret_cast<__Formula_Base_Template<T>*>(replace_unknown_new(unknown, new_value)));};
        std::shared_ptr<__Formula_Base_Template<T>> replace_unknowns(Unknowns_Container* values) const{return std::shared_ptr<__Formula_Base_Template<T>>(reinterpret_cast<__Formula_Base_Template<T>*>(replace_unknowns_new(values)));};

        // Returns the final value of the formula
        T value(__Formula_Base::Formula_Factor* fac, __Formula_Base::Unknowns_Container* values){T to_return = T(1);for(int i=0;i<static_cast<int>(fac->factors().size());i++){to_return*=reinterpret_cast<__Formula_Base_Template<T>*>(fac->factors().at(i).get())->value(values);}return to_return;}
        T value(__Formula_Base::Formula_Sum* sum, __Formula_Base::Unknowns_Container* values){T to_return = T(0);for(int i=0;i<static_cast<int>(sum->formulas_add().size());i++){to_return+=value(sum->formulas_add().at(i).get(), values);}return to_return;}
        T value(__Formula_Base::Formula_Fraction* f, __Formula_Base::Unknowns_Container* values){T to_return = T(0);if(f->numerator()!=0){to_return = value(f->numerator(), values);}if(f->denominator() != 0){to_return._divide(value(f->denominator(), values));}return to_return;}
        T value(Fraction current_value){__Formula_Base_Template<T>t=T(current_value);Unknowns_Container temp = Unknowns_Container(&t);return value(&temp);};
        T value(__Formula_Base::Unknowns_Container* values) {
            // Simpler models
            if(is_simple_monomonial() && is_basic()){__Monomonial_Template<T>* needed_monomonial = monomonial();if(needed_monomonial != 0 && needed_monomonial->is_known()){return *needed_monomonial->factor();}}

            // Get the needed datas
            std::shared_ptr<__Formula_Base_Template<T>> current_formula = internal_value();
            std::vector<std::string> unknowns = all_unknowns();

            // Get the needed value
            current_formula = current_formula.get()->replace_unknowns(values);

            // Get the final formula
            __Formula_Base_Template<T> final_formula;
            if(current_formula.get()->fraction() != 0){final_formula = value(current_formula.get()->fraction(), values);}
            else if(current_formula.get()->polynomial() != 0){final_formula = current_formula.get()->polynomial();}

            // Apply the function
            T to_return = T(1);
            if(applied_function() != 0){to_return = scls::Fraction::from_double(applied_function()->real_value(&final_formula));}
            else{to_return = final_formula.polynomial()->known_monomonial_factor();}

            // Returns the value
            return to_return;
        }

        // Returns the final value of the formula
        double* calculated_to_double(Unknowns_Container* container) {
            for(int i = 0;i<static_cast<int>(a_to_double_containers.size());i++){
                if(a_to_double_containers.at(i)==container){return &a_to_double[i];}
            }
            return 0;
        }
        Fraction value_to_fraction(Unknowns_Container* values){return value(values).real();};
        Fraction value_to_fraction(Fraction current_value){return value(current_value).real();};
        Fraction value_to_fraction(){return value(1).real();};
        double value_to_double(Unknowns_Container* values){
            // TEMP
            return value(values).to_double();

            // TO EDIT
            double* calculated = calculated_to_double(values);
            if(calculated == 0){
                double new_to_double = value(values).to_double();
                a_to_double.push_back(new_to_double);
                a_to_double_containers.push_back(values);
                calculated = &a_to_double[a_to_double.size() - 1];
            }

            return *calculated;
        }
        double value_to_double(Fraction current_value){
            if(current_value == 1) {
                double* calculated = calculated_to_double(0);
                if(calculated == 0){
                    double new_to_double=value(1).real().to_double();
                    a_to_double.push_back(new_to_double);
                    a_to_double_containers.push_back(0);
                    calculated = &a_to_double[a_to_double.size() - 1];
                }
                return *calculated;
            }

            return value(current_value).real().to_double();
        }
        double value_to_double(){return value_to_fraction().to_double();};

        // Operators
        // With int
        bool operator==(int value) {return operator==(T(value));};
        // With T
        bool operator==(T value) {return __is_equal(value);};
        __Formula_Base_Template<T> operator*(T value) {__Formula_Base_Template<T> to_return(*this);to_return.__multiply(&value);return to_return;};
        __Formula_Base_Template<T>& operator*=(T value) {multiply(&value);return*this;};
        __Formula_Base_Template<T>& operator/=(T value) {divide(&value);return*this;};
        __Formula_Base_Template<T>& operator+=(T value) {add(&value);return*this;};
        // With formulas
        bool operator==(__Formula_Base_Template<T> value) {return __is_equal(value);};
        __Formula_Base_Template<T> operator-(__Formula_Base_Template<T> value) const {__Formula_Base_Template<T> to_return(*this);value*=Fraction(-1);to_return.__add(&value);return to_return;};
        __Formula_Base_Template<T>& operator-=(__Formula_Base_Template<T> value) {value*=Fraction(-1);__add(&value);return*this;};
        __Formula_Base_Template<T> operator+(__Formula_Base_Template<T> value) {__Formula_Base_Template<T> to_return(*this);to_return.__add(&value);return to_return;};
        __Formula_Base_Template<T>& operator+=(__Formula_Base_Template<T> value) {__add(&value);return*this;};
        __Formula_Base_Template<T> operator*(__Formula_Base_Template<T> value) {__Formula_Base_Template<T> to_return(*this);to_return.__multiply(&value);return to_return;};
        __Formula_Base_Template<T>& operator*=(__Formula_Base_Template<T> value) {__multiply(&value);return*this;};
        __Formula_Base_Template<T> operator/(__Formula_Base_Template<T> value) const {__Formula_Base_Template<T> other(*this);other.__divide(&value);return other;};
        __Formula_Base_Template<T>& operator/=(__Formula_Base_Template<T> value) {__divide(&value);return*this;};
    };

}

#endif // SCLS_MATH_FORMULA
