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

        // Container of unknowns
        struct Unknown{std:: string name = std::string();std::shared_ptr<__Formula_Base> value = std::make_shared<__Formula_Base>();void set_value(__Formula_Base formula){value = formula.formula_copy();};};;
        class Unknowns_Container {
        public:
            // Unknowns_Container constructor
            Unknowns_Container(__Formula_Base value):a_default_value(std::make_shared<__Formula_Base>(value)){};
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
                __Formula_Base_Function(std::string new_name):a_name(new_name) {};
                __Formula_Base_Function(const __Formula_Base_Function& function_copy):a_name(function_copy.a_name){};

                // Definition set of the function
                virtual Set_Number definition_set() = 0;
                // Derivate value
                virtual std::shared_ptr<__Formula_Base> derivate_value(__Formula_Base formula) = 0;
                // Multiply a value with the function
                virtual std::shared_ptr<__Formula_Base> multiply(__Formula_Base* value_1, __Formula_Base* value_2){return std::shared_ptr<__Formula_Base>();};
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
            inline bool is_simple_polynomial() const {return a_factors.size() == 0 || (a_factors.size() == 1 && a_factors[0].get()->is_simple_polynomial());};
            // Converts the formula to a polynomial
            inline Polynomial to_polynomial() const {for(int i = 0;i<static_cast<int>(a_factors.size());i++){if(a_factors.at(i).get()->is_simple_polynomial()){return a_factors.at(i).get()->to_polynomial();}}return Polynomial();};
            operator Polynomial() const {return to_polynomial();};

            // Divides a formula to this one
            void __divide(__Formula_Base value);
            inline void __divide(Polynomial value){__divide(__Formula_Base(value));};
            inline void __divide(__Monomonial value){__divide(__Formula_Base(value));};
            // Multiplies the factor by a polynomial
            inline void __multiply(Polynomial value) {if(!basic_formula_exists()){basic_formula()->set_polynomial(value);}else{basic_formula()->__multiply(value);}};
            void __multiply(__Formula_Base value);

            // Returns if two numbers/formulas are equals
            bool __is_equal(int value)const{return __is_equal(scls::Fraction(value));};
            bool __is_equal(Fraction value)const{return a_factors.size() == 1 && a_factors[0].get()->__is_equal(value);};
            bool __is_equal(Polynomial value)const{return a_factors.size() == 1 && a_factors[0].get()->__is_equal(value);};
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

            // Returns the polynomial to mathml / std::string
            inline std::string to_mathml(Textual_Math_Settings* settings) const{std::string to_return = std::string();for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){to_return+=a_formulas_add.at(i).get()->to_mathml(settings);if(i<static_cast<int>(a_formulas_add.size())-1){to_return += std::string("<mo>+</mo>");}}return to_return;};
            inline std::string to_std_string(Textual_Math_Settings* settings) const{std::string to_return = std::string();for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){to_return+=a_formulas_add.at(i).get()->to_std_string(settings);if(i<static_cast<int>(a_formulas_add.size())-1){to_return += std::string("+");}}return to_return;};

            // Form of the formula
            inline bool is_simple_monomonial() const {return a_formulas_add.size() == 1 && a_formulas_add[0].get()->is_simple_monomonial();};
            inline bool is_simple_polynomial() const {return a_formulas_add.size() == 1 && a_formulas_add[0].get()->is_simple_polynomial();};
            // Returns a copy of the sum
            inline std::shared_ptr<Formula_Sum> sum_copy(){std::shared_ptr<Formula_Sum> to_return = std::make_shared<Formula_Sum>();for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){to_return.get()->a_formulas_add.push_back(a_formulas_add.at(i).get()->factor_copy());}return to_return;};
            // Converts the formula to a polynomial / monomonial
            inline Polynomial to_polynomial() const {for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){if(a_formulas_add.at(i).get()->is_simple_polynomial()){return a_formulas_add.at(i).get()->to_polynomial();}}return Polynomial();};
            operator Polynomial() const {return to_polynomial();};

            // Methods operators
            void __add(__Formula_Base value){a_formulas_add.push_back(std::make_shared<Formula_Factor>(value));};
            // Divide a formula to this one
            void __divide(__Monomonial value){for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){a_formulas_add.at(i).get()->__divide(value);}};
            // Returns if two numbers/formulas are equals
            bool __is_equal(int value)const{return __is_equal(scls::Fraction(value));};
            bool __is_equal(Fraction value)const{return __is_equal(Polynomial(value));};
            bool __is_equal(Polynomial value)const{return a_formulas_add.size() == 1 && a_formulas_add.at(0).get()->__is_equal(value);};
            bool __is_equal(__Formula_Base value)const{return a_formulas_add.size() == 1 && a_formulas_add.at(0).get()->__is_equal(value);};

            // Multiply a polynomial to this one
            virtual void __multiply(Polynomial value) {for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){a_formulas_add.at(i).get()->__multiply(value);}};
            virtual void __multiply(__Formula_Base value) {for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){a_formulas_add.at(i).get()->__multiply(value);}};

            // Returns the final value of the formula
            scls::Complex value(__Formula_Base::Unknowns_Container* values);

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
            Formula_Fraction(__Formula_Base base):Formula_Fraction(std::make_shared<Formula_Sum>(base)){};
            Formula_Fraction(std::shared_ptr<__Formula_Base> base):Formula_Fraction(std::make_shared<Formula_Sum>(base)){};
            Formula_Fraction(int base):Formula_Fraction(std::make_shared<Formula_Sum>(base)){};
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
            inline Polynomial to_polynomial() const {return a_numerator.get()->to_polynomial();};
            operator Polynomial() const {return to_polynomial();};

            // Methods operators
            void __add(__Formula_Base value){if(a_denominator.get() != 0){value.__multiply(*a_denominator.get());}a_numerator.get()->__add(value);};
            // Divide a formula to this one
            void __divide(__Formula_Base value);
            // Returns if two numbers/formulas are equals
            bool __is_equal(int value)const{return __is_equal(scls::Fraction(value));};
            bool __is_equal(Fraction value)const{return a_denominator.get() == 0 && a_numerator.get() != 0 && a_numerator.get()->__is_equal(value);};
            bool __is_equal(Polynomial value)const{return a_denominator.get() == 0 && a_numerator.get() != 0 && a_numerator.get()->__is_equal(value);};
            // Multiply a polynomial to this one
            virtual void __multiply(Polynomial value) {if(a_denominator.get() != 0 && a_denominator.get()->__is_equal(value)){a_denominator.reset();}else{a_numerator.get()->__multiply(value);}};
            virtual void __multiply(__Formula_Base value) {if(a_denominator.get() != 0 && a_denominator.get()->__is_equal(value)){a_denominator.reset();}else{a_numerator.get()->__multiply(value);}};

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
        __Formula_Base(double number):a_polynomial(std::make_shared<Polynomial>(number)){};
        __Formula_Base(__Fraction_Base number):a_polynomial(std::make_shared<Polynomial>(number)){};
        __Formula_Base(Fraction number):a_polynomial(std::make_shared<Polynomial>(number)){};
        __Formula_Base(Complex number):a_polynomial(std::make_shared<Polynomial>(number)){};
        __Formula_Base(__Monomonial monomonial):a_polynomial(std::make_shared<Polynomial>(monomonial)){};
        __Formula_Base(Polynomial polynomial):a_polynomial(std::make_shared<Polynomial>(polynomial)){};
        __Formula_Base(Formula_Fraction frac){set_fraction(frac);};
        __Formula_Base(Formula_Sum sum){if(sum.is_simple_polynomial()){set_polynomial(sum);}else{set_fraction(sum);}};
        __Formula_Base(const __Formula_Base& formula):a_polynomial(formula.polynomial_copy()),a_fraction(formula.fraction_copy()),a_applied_function(formula.applied_function_copy()){};

        // Checks if the formula is well formatted
        void check_formula();
        // Clear the formula
        inline void clear() {a_applied_function.reset();a_fraction.reset();a_polynomial.reset();};
        // Pastes a formula to this one
        virtual void paste(__Formula_Base* value){a_applied_function=value->applied_function_copy();a_fraction = value->fraction_copy();a_polynomial = value->polynomial_copy();};
        // Returns a copy of this formula
        std::shared_ptr<__Formula_Base> formula_copy() {std::shared_ptr<__Formula_Base>to_return=std::make_shared<__Formula_Base>(*this);return to_return;};
        inline std::shared_ptr<Formula_Fraction> fraction_copy()const{if(a_fraction.get()==0){return std::shared_ptr<Formula_Fraction>();} return a_fraction.get()->fraction_copy();};
        inline std::shared_ptr<Polynomial> polynomial_copy()const{if(a_polynomial.get()==0){return std::shared_ptr<Polynomial>();}return std::make_shared<Polynomial>(*a_polynomial.get());};
        // Soft_resets the formula
        void soft_reset();
        // Sub-place the current formula to a "formula add"
        inline void sub_place(){std::shared_ptr<__Formula_Base> needed_copy = formula_copy();clear();a_fraction = std::make_shared<Formula_Fraction>(needed_copy);};

        // Returns the polynomial to mathml
        std::string to_mathml(Textual_Math_Settings* settings) const;
        // Returns the polynomial to std::string
        std::string to_std_string(Textual_Math_Settings* settings) const;

        // Returns the denominator / numerator of the formula
        inline std::shared_ptr<__Formula_Base> denominator()const{if(!has_denominator()){return std::shared_ptr<__Formula_Base>();}return std::make_shared<__Formula_Base>(*a_fraction.get()->denominator());};
        inline bool has_denominator()const{return a_fraction.get() != 0 && a_fraction.get()->denominator() != 0;};
        inline std::shared_ptr<__Formula_Base> numerator()const{if(a_fraction.get() == 0){return std::shared_ptr<__Formula_Base>();}return std::make_shared<__Formula_Base>(*a_fraction.get()->numerator());};
        // Returns the internal value of the formula (without functions)
        inline __Formula_Base internal_value() const {__Formula_Base to_return = *this;to_return.clear_applied_function();return to_return;};
        // Returns if the formula is a basic formula or not
        inline bool is_basic() const {return a_applied_function.get() == 0 && a_exponent.get() == 0;};
        // Returns if the formula is a simple monomonial / polynomial or not
        inline bool is_simple_fraction() const {return is_basic() && a_fraction.get() != 0;};
        inline bool is_simple_monomonial() const {return is_simple_polynomial() && (a_polynomial.get() == 0 || a_polynomial.get()->is_simple_monomonial());};
        inline bool is_simple_polynomial() const {return is_basic() && (a_polynomial.get() != 0 || a_fraction.get() == 0);};
        // Returns the numerator value
        inline __Formula_Base numerator_value() const {if(a_polynomial.get() != 0){return (*a_polynomial.get());} __Formula_Base to_return = *this;to_return.clear_applied_function();to_return.clear_denominator();return to_return;};
        // Sets the value of the formula with a fraction / polynomial
        inline void set_fraction(std::shared_ptr<Formula_Fraction> frac){a_fraction=frac;a_polynomial.reset();};
        inline void set_fraction(Formula_Fraction frac){set_fraction(frac.fraction_copy());};
        inline void set_fraction(std::shared_ptr<Formula_Sum> sum){set_fraction(std::make_shared<Formula_Fraction>(sum));};
        inline void set_fraction(Formula_Sum sum){set_fraction(std::make_shared<Formula_Sum>(sum));};
        inline void set_polynomial(std::shared_ptr<Polynomial> sum){a_polynomial=sum;a_fraction.reset();};
        inline void set_polynomial(Formula_Sum sum){set_polynomial(std::make_shared<Polynomial>(sum.to_polynomial()));};
        inline void set_polynomial(Polynomial polynomial){set_polynomial(std::make_shared<Polynomial>(polynomial));};
        // Converts the formula to a polynomial / monomonial
        inline __Monomonial to_monomonial() const {if(to_polynomial().monomonial() == 0){return __Monomonial(Complex(0));} return *to_polynomial().monomonial();};
        inline Polynomial to_polynomial() const {if(a_polynomial.get() != 0){return *a_polynomial.get();}else if(a_fraction.get()==0){return Polynomial(0);} return a_fraction.get()->to_polynomial();};
        operator Polynomial() const {return to_polynomial();};

        // Methods operators
        virtual void __add(__Formula_Base* value);
        inline void __add(scls::__Fraction_Base value){__Formula_Base temp=value;__add(&temp);};
        inline void __add(scls::Fraction value){__Formula_Base temp=value;__add(&temp);};
        inline void __substract(__Formula_Base* value){__Formula_Base temp(*value);temp.__multiply(-1);__add(&temp);};

        // Divide a formula to this one
        void __divide(Fraction value){__divide(__Formula_Base(value));};
        void __divide(Polynomial value){__divide(__Formula_Base(value));};
        void __divide(Complex value){__divide(__Formula_Base(value));};
        virtual void __divide(__Formula_Base value);

        // Returns if two numbers/formulas are equals
        bool __is_equal(int value) const;
        bool __is_equal(Fraction value) const;
        bool __is_equal(Polynomial value) const;
        bool __is_equal(__Formula_Base value) const;

        // Multiply a polynomial to this one
        virtual void __multiply(Polynomial value) {__multiply(__Formula_Base(value));};
        inline void __multiply(int value){__multiply(__Formula_Base(value));};
        inline void __multiply(__Monomonial value){__multiply(__Formula_Base(value));};
        inline void __multiply(__Fraction_Base value) {Polynomial temp;temp.add_monomonial(__Monomonial(value));__multiply(temp);};
        virtual void __multiply(Fraction value) {Polynomial temp;temp.add_monomonial(__Monomonial(value));__multiply(temp);};
        inline void __multiply(Complex value){__multiply(__Formula_Base(value));};
        // Multiply an another formula to this one
        void __multiply(__Formula_Base value);
        void __multiply(Formula_Sum value){__multiply(__Formula_Base(value));};

        // Operators
        // With int
        bool operator==(int value) {return __is_equal(value);};
        __Formula_Base operator*(int value) {__Formula_Base to_return = (*formula_copy().get());to_return.__multiply(value);return to_return;};
        __Formula_Base& operator*=(int value) {__multiply(value);return*this;};
        // With fractions
        bool operator==(Fraction value) {return __is_equal(value);};
        __Formula_Base operator*(__Fraction_Base value) {__Formula_Base to_return(*this);to_return.__multiply(value);return to_return;};
        __Formula_Base operator*(Fraction value) {__Formula_Base to_return(*this);to_return.__multiply(value);return to_return;};
        __Formula_Base& operator*=(__Fraction_Base value) {__multiply(value);return*this;};
        __Formula_Base& operator*=(Fraction value) {__multiply(value);return*this;};
        __Formula_Base& operator/=(Fraction value) {__divide(value);return*this;};
        __Formula_Base& operator+=(__Fraction_Base value) {__add(value);return*this;};
        __Formula_Base& operator+=(Fraction value) {__add(value);return*this;};
        // With complex
        __Formula_Base operator*(Complex value) {__Formula_Base to_return(*this);to_return.__multiply(value);return to_return;};
        __Formula_Base& operator*=(Complex value) {__multiply(value);return *this;};
        __Formula_Base& operator/=(Complex value) {__divide(value);return *this;};
        // With monomonial
        __Formula_Base& operator+=(__Monomonial value) {__Formula_Base temp=value;__add(&temp);return*this;};
        __Formula_Base& operator*=(__Monomonial value) {__multiply(value);return*this;};
        // With polynomial
        __Formula_Base& operator/=(Polynomial value) {__divide(value);return *this;};
        // With formulas
        bool operator==(__Formula_Base value) {return __is_equal(value);};
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
        inline std::string* redaction_ptr() {return a_redaction;};
        template<typename T> void set_applied_function(std::shared_ptr<T> new_applied_function) {a_applied_function = new_applied_function;if(a_applied_function.get()!=0){std::shared_ptr<__Formula_Base>f=a_applied_function.get()->simplify(this);if(f.get()!=0){paste(f.get());}}soft_reset();};
        template<typename T> void set_applied_function() {set_applied_function(std::make_shared<T>());};
        inline void set_redaction(std::string* new_redaction){a_redaction = new_redaction;};

        //*********
        // Unknown handling
        //*********

        // "Shared" __Formula_Base
        class Formula {
        public:
            // Formula constructor
            Formula(__Fraction_Base number):Formula(std::make_shared<__Formula_Base>(number)){};
            Formula(Fraction number):Formula(std::make_shared<__Formula_Base>(number)){};
            Formula(Complex number):Formula(std::make_shared<__Formula_Base>(number)){};
            Formula(std::shared_ptr<__Formula_Base> needed_formula):a_formula(needed_formula){};
            Formula(double number):Formula(std::make_shared<__Formula_Base>(number)){};
            Formula(__Monomonial monomonial):Formula(std::make_shared<__Formula_Base>(monomonial)){};
            Formula(Polynomial polynomial):Formula(std::make_shared<__Formula_Base>(polynomial)){};
            Formula(__Formula_Base needed_formula):Formula(std::make_shared<__Formula_Base>(needed_formula)){};
            Formula():Formula(std::make_shared<__Formula_Base>()){};

            // Operations from __Formula_Base
            inline void clear()const{a_formula.get()->clear();};
            inline __Formula_Base* formula_base() const {return a_formula.get();};
            inline std::shared_ptr<__Formula_Base> formula_base_shared_ptr() const {return a_formula;};
            inline Formula formula_copy()const{return Formula(a_formula.get()->formula_copy());};
            inline void paste(__Formula_Base* to_paste)const{a_formula.get()->paste(to_paste);};
            inline void paste(Formula to_paste)const{a_formula.get()->paste(to_paste.a_formula.get());};
            inline std::string to_mathml(Textual_Math_Settings* settings) const {return a_formula.get()->to_mathml(settings);};
            inline Polynomial to_polynomial() const {return a_formula.get()->to_polynomial();};
            inline std::string to_std_string(Textual_Math_Settings* settings) const {return a_formula.get()->to_std_string(settings);};

            // Returns if the formula is a simple monomonial / polynomial or not
            inline bool is_simple_fraction() const {return a_formula.get()->is_simple_fraction();};
            inline bool is_simple_monomonial() const {return a_formula.get()->is_simple_monomonial();};
            inline bool is_simple_polynomial() const {return a_formula.get()->is_simple_polynomial();};

            // Returns the final value of the formula
            Complex value(Unknowns_Container* values) const;
            Complex value(Fraction current_value) const;
            Fraction value_to_fraction(Unknowns_Container* values) const;
            Fraction value_to_fraction(Fraction current_value) const;
            Fraction value_to_fraction() const;
            double value_to_double(Unknowns_Container* values) const;
            double value_to_double(Fraction current_value) const;
            double value_to_double() const;

            // Getters and setters
            inline __Formula_Base_Function* applied_function()const{return a_formula.get()->applied_function();};
            template<typename T> inline void set_applied_function(std::shared_ptr<T> new_applied_function) {a_formula.get()->set_applied_function<T>(new_applied_function);};
            template<typename T> inline void set_applied_function() {a_formula.get()->set_applied_function(std::make_shared<T>());};

            // Operators

            // Comparaisons
            bool operator<(int value) {return value_to_double(0) < value;};

            // Operations
            // With int
            bool operator==(int value) {return a_formula.get()->__is_equal(value);};
            bool operator!=(int value) {return !a_formula.get()->__is_equal(value);};
            Formula& operator*=(int value) {__Formula_Base temp=value;a_formula.get()->__multiply(temp);return*this;};
            Formula operator*(int value) {return ((*a_formula.get()) * value);};
            // With fractions
            Formula operator+(__Fraction_Base value) {return ((*a_formula.get()) + value);};
            Formula& operator+=(__Fraction_Base value) {__Formula_Base temp=value;a_formula.get()->__add(&temp);return*this;};
            Formula operator+(Fraction value) {return ((*a_formula.get()) + value);};
            Formula& operator+=(Fraction value) {__Formula_Base temp=value;a_formula.get()->__add(&temp);return*this;};
            Formula operator*(__Fraction_Base value) {return ((*a_formula.get()) * value);};
            Formula& operator*=(__Fraction_Base value) {__Formula_Base temp=value;a_formula.get()->__multiply(temp);return*this;};
            Formula operator*(Fraction value) {return ((*a_formula.get()) * value);};
            Formula& operator*=(Fraction value) {__Formula_Base temp=value;a_formula.get()->__multiply(temp);return*this;};
            // With complex
            Formula& operator*=(Complex value) {__Formula_Base temp=value;a_formula.get()->__multiply(temp);return*this;};
            Formula& operator+=(Complex value) {__Formula_Base temp=value;a_formula.get()->__add(&temp);return*this;};
            Formula& operator/=(Complex value) {a_formula.get()->__divide(value);return*this;};
            // With monomonial
            Formula& operator+=(__Monomonial value) {__Formula_Base temp=value;a_formula.get()->__add(&temp);return*this;};
            Formula& operator*=(__Monomonial value) {a_formula.get()->__multiply(value);return*this;};
            // With polynomial
            Formula& operator*=(Polynomial value) {a_formula.get()->__divide(value);return*this;};
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
            Formula operator+(Formula value) {return (*value.a_formula.get() + *a_formula.get());};
            Formula& operator+=(Formula value) {a_formula.get()->__add(value.a_formula.get());return*this;};
            Formula operator-(Formula value) {return (*a_formula.get() - *value.a_formula.get());};
            Formula& operator-=(Formula value) {a_formula.get()->__substract(value.a_formula.get());return*this;};
            Formula operator*(Formula value) {return (*value.a_formula.get() * *a_formula.get());};
            Formula& operator*=(Formula value) {a_formula.get()->__multiply(*value.a_formula.get());return*this;};
            Formula operator/(Formula value) {return (*a_formula.get() / *value.a_formula.get());};
            Formula& operator/=(Formula value) {a_formula.get()->__divide(*value.a_formula.get());return*this;};

        private:
            // Shared pointer to the needed formula base
            std::shared_ptr<__Formula_Base> a_formula;
        };

        // Returns all the unknowns in the formula
        std::vector<std::string> all_unknowns() const;
        // Returns a formula from a monomonial where the unknows has been replaced
        static Formula formula_from_modified_monomonial_unknows(__Monomonial used_monomonial, std::string unknown, __Formula_Base new_value);
        // Returns a formula from a polynomial where the unknows has been replaced
        static Formula formula_from_modified_polynomial_unknows(Polynomial used_polynomial, std::string unknown, __Formula_Base new_value);
        // Returns a monomonial where an unkown is replaced by an another unknown
        Formula replace_unknown(std::string unknown, __Formula_Base new_value) const;
        Formula replace_unknown(std::string unknown, Formula new_value) const;
        Formula replace_unknowns(Unknowns_Container* values) const;
        // Returns the final value of the formula
        Complex value(Unknowns_Container* values);
        Complex value(scls::Fraction current_value);
        Fraction value_to_fraction(Unknowns_Container* values);
        Fraction value_to_fraction(scls::Fraction current_value);
        Fraction value_to_fraction();
        double value_to_double(Unknowns_Container* values);
        double value_to_double(scls::Fraction current_value);
        double value_to_double();

    private:
        // Redaction text
        std::string* a_redaction = 0;

        // Each parts of the formula are in order

        // Polynomial of the formula (fraction is ignored if a polynomial is used)
        std::shared_ptr<Polynomial> a_polynomial = std::make_shared<Polynomial>();

        // Attached fraction of the formula
        std::shared_ptr<Formula_Fraction> a_fraction;
        // Exponent of the formula
        std::shared_ptr<__Formula_Base> a_exponent;
        // Applied function to the ENTIRE formula
        std::shared_ptr<__Formula_Base_Function> a_applied_function;

        // Already calculated parts
        std::vector<double> a_to_double = std::vector<double>();
        std::vector<Unknowns_Container*> a_to_double_containers = std::vector<Unknowns_Container*>();
        double* calculated_to_double(Unknowns_Container* container);
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
            virtual double real_value(__Formula_Base* formula){__Monomonial* needed_monomonial = formula->to_polynomial().known_monomonial();if(needed_monomonial == 0){return 1;}double value = needed_monomonial->factor()->real().to_double();return std::cos(value);};
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
            virtual double real_value(__Formula_Base* formula){__Monomonial* needed_monomonial = formula->to_polynomial().known_monomonial();if(needed_monomonial == 0){return 0;}double value = needed_monomonial->factor()->real().to_double();return std::sin(value);};
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
            // Multiply a value with the function
            virtual std::shared_ptr<__Formula_Base> multiply(__Formula_Base* value_1, __Formula_Base* value_2);
            // Real value
            virtual double real_value(__Formula_Base* formula){double value = formula->value_to_double();return std::sqrt(value);};
            // Simplify a value with the function
            virtual std::shared_ptr<__Formula_Base> simplify(__Formula_Base* value) {
                if(value->applied_function() == 0) {
                    __Formula_Base inner = value->internal_value();
                    if(inner.is_simple_monomonial()) {
                        Polynomial to_poly = inner.to_polynomial();
                        if(to_poly.is_simple_monomonial()){
                            // Simplify a simple monomonial
                            __Monomonial needed_monomonial = *to_poly.monomonial();
                            std::vector<double> needed_exponent; bool good = true;
                            for(int i = 0;i<static_cast<int>(needed_monomonial.unknowns().size());i++){if(needed_monomonial.unknowns()[i].name()!=std::string()){needed_exponent.push_back(std::log2(needed_monomonial.unknowns()[i].exponent().real().to_double()));}}
                            for(int i = 0;i<static_cast<int>(needed_exponent.size());i++){if(needed_exponent[i]!=round(needed_exponent[i])||needed_exponent[i]==0){good=false;break;}}
                            if(good) {
                                // Create the needed monomonial
                                scls::Fraction value = std::sqrt(needed_monomonial.factor()->real().to_double());
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
}

#endif // SCLS_MATH_FORMULA
