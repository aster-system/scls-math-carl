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
	// The "Polynomial" class
	//
	//*********

	class _Base_Unknown {
        // Class representating the base of an unknow in a monomonial
    public:
        // Possible unknows comparaison
        enum Unknown_Comparaison {UC_DIFFERENT, UC_EQUAL, UC_EQUAL_UNKNOWN};

        // Unknow constructor
        _Base_Unknown(std::string new_name, int exponent):a_exponent(exponent),a_name(new_name){};
        _Base_Unknown(std::string new_name):_Base_Unknown(new_name, 1){};

        // Returns the comparaison between two unknows
        bool compare_unknown(_Base_Unknown other) const;

        // Getters and setters
        inline bool conjugate() const {return a_conjugate;};
        inline int exponent() const {return a_exponent;};
        inline std::string name() const {return a_name;};
        inline void set_exponent(int new_exponent) {a_exponent = new_exponent;};
        inline void set_name(std::string new_name) {a_name = new_name;};

        // Operator ==
        inline bool operator==(_Base_Unknown other) const {return compare_unknown(other) == Unknown_Comparaison::UC_EQUAL;};

    private:

        // If the unknown is a conjugate
        bool a_conjugate = false;
        // Exponent of the unknow
        int a_exponent = 1;
        // Name of the unknow
        std::string a_name = "";
    };

    class __Monomonial_Base {
        // Class representating a monomonial in a polynomial form
    public:

        // __Monomonial_Base constructor
        __Monomonial_Base(std::shared_ptr<__Field_Element> factor):a_factor(factor){};
        __Monomonial_Base(std::shared_ptr<__Field_Element> factor, std::vector<_Base_Unknown> unknowns):__Monomonial_Base(factor){a_unknowns=unknowns;};
        __Monomonial_Base(std::shared_ptr<__Field_Element> factor, std::string unknow):__Monomonial_Base(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow))){};
        __Monomonial_Base(std::shared_ptr<__Field_Element> factor, std::string unknow, int exponent):__Monomonial_Base(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow, exponent))){};
        // __Monomonial_Base copy constructor
        __Monomonial_Base(const __Monomonial_Base& monomonial_copy):__Monomonial_Base(monomonial_copy.a_factor,monomonial_copy.a_unknowns){};
        // __Monomonial_Base destructor
        virtual ~__Monomonial_Base(){};
        // Returns the inverse / opposite of the monomonial
        std::shared_ptr<__Monomonial_Base> opposite() const;
        std::shared_ptr<__Monomonial_Base> inverse() const;

        // Add an unknown to the monomonial
        inline void add_unknown(std::string name, int exponent) {_Base_Unknown unknown(name); unknown.set_exponent(exponent); a_unknowns.push_back(unknown);};
        // Returns if the monomonial contains an unkwnown
        inline _Base_Unknown* contains_unknown(std::string unknown_name) {for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {if(a_unknowns.at(i).name() == unknown_name) {return &a_unknowns[i];}} return 0;};
        // Deletes the monomonial unkwnown
        inline void delete_unknown(std::string unknown_name) {for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {if(a_unknowns[i].name() == unknown_name) {a_unknowns.erase(a_unknowns.begin() + i); return;}}};
        inline void delete_unknown(_Base_Unknown* unknown_to_delete) {delete_unknown(unknown_to_delete->name());};
        // Returns if the monomonial is known
        inline bool is_known() const {return a_unknowns.size() <= 0 || (a_unknowns.size() == 1 && (a_unknowns.at(0).name() == "" || a_unknowns.at(0).exponent() == 0));};
        // Returns if the monomonial only contains an unkwnown
        _Base_Unknown* only_contains_unknown(std::string unknown_name, int exponent);
        // Returns this monomonial where an unknown is replaced by an another unknown
        std::shared_ptr<__Monomonial_Base> replace_unknown(std::string unknown, __Monomonial_Base* new_value);
        // Returns if two monomonials has the same unknowns
        bool same_unknowns(const __Monomonial_Base* object) const;
        // Unknows of the monomonial
        inline _Base_Unknown* unknown(){if(unknowns().size()>0){if(unknowns()[0].name()==std::string()){if(unknowns().size()>1){return &unknowns()[1];}}else{return &unknowns()[0];}}return 0;};
        inline std::vector<_Base_Unknown>& unknowns() {return a_unknowns;};
        inline const std::vector<_Base_Unknown>& unknowns_const() const {return a_unknowns;};
        inline unsigned int unknowns_number() const {unsigned int to_return = 0;for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns[i].name() != "")to_return++;}return to_return;};

        // Returns the monomonial to a GLSL calculation
        std::string to_glsl(Textual_Math_Settings* settings) const;
        // Returns the monomonial to mathml
        std::string to_mathml(Textual_Math_Settings* settings) const;
        // Returns the monomonial converted to std::string
        std::string to_std_string(Textual_Math_Settings* settings) const;

        // Operators
        // Equal operator assignment
        bool __is_equal(const __Monomonial_Base* object) const;
        // Operations operator
        void __add(const __Monomonial_Base* object);
        void __multiply(const __Field_Element* object);
        void __multiply(const __Monomonial_Base* object);
        void __substract(const __Monomonial_Base* object);

        // Hierarchy functions
        // Comparaison
        virtual bool is_equal(__Field_Element* operand_1, __Field_Element* operand_2) const = 0;
        // Operations
        virtual std::shared_ptr<__Field_Element> addition(__Field_Element* operand_1, __Field_Element* operand_2) = 0;
        virtual std::shared_ptr<__Field_Element> division(__Field_Element* operand_1, __Field_Element* operand_2) = 0;
        virtual std::shared_ptr<__Field_Element> multiplication(__Field_Element* operand_1, const __Field_Element* operand_2) = 0;
        virtual std::shared_ptr<__Field_Element> substraction(__Field_Element* operand_1, __Field_Element* operand_2) = 0;
        // Property
        virtual std::shared_ptr<__Monomonial_Base> clone()const = 0;
        virtual bool is_null(__Field_Element* operand) const = 0;
        bool is_null()const{return is_null(__factor());};

        // Getters and setters
        inline __Field_Element* __factor()const {return a_factor.get();};
        inline void __set_factor(std::shared_ptr<__Field_Element> factor){a_factor = factor;};

    private:

        // Factor of the monomonial
        std::shared_ptr<__Field_Element> a_factor;

        // Unknows of the monomonial
        std::vector<_Base_Unknown> a_unknowns;
    };

    class Polynomial_Base {
        // Class representating a full polynomial form
    public:

        // Polynomial_Base constructor
        Polynomial_Base(){};
        Polynomial_Base(std::shared_ptr<__Monomonial_Base> monomonial){a_monomonials.push_back(monomonial);};
        // Polynomial_Base copy constructor
        Polynomial_Base(const Polynomial_Base& polynomial_copy);
        // Polynomial_Base destructor
        virtual ~Polynomial_Base(){};

        // Add a new monomonial to the polynomial
        void __add_monomonial(__Monomonial_Base* new_monomonial);
        // Returns all the unknowns in the formula
        std::vector<std::string> all_unknowns();
        // Returns the bigger monomonial
        __Monomonial_Base* __bigger_monomonial() const;
        // Returns if the polynomial contains a monomonial
        __Monomonial_Base* contains_monomonial(__Monomonial_Base* new_monomonial);
        // Returns the maximum degree in the polynomial
        int degree(std::string unknown_name);
        // Returns if the polynomial is known
        bool is_known() const;
        // Returns if te polynomial is null
        bool is_null() const {return a_monomonials.size() <= 0;};
        // Returns if the polynomial is a simple monomonial
        bool is_simple_monomonial() const;
        // Returns the knows monomonial
        __Monomonial_Base* __known_monomonial() const;
        // Returns a monomonial by its unknown
        __Monomonial_Base* __monomonial(std::string unknown, int exponent);
        __Monomonial_Base* __monomonial(std::string unknown);
        __Monomonial_Base* __monomonial();
        // Returns the number of monomonials in the polynomial
        int monomonials_number() const;
        // Returns a list of unknowns monomonials
        std::vector<__Monomonial_Base*> unknown_monomonials() const;

        // Returns the polynomial to a GLSL function
        std::string to_glsl(Textual_Math_Settings* settings) const;
        // Returns the polynomial to mathml
        std::string to_mathml(Textual_Math_Settings* settings) const;
        // Returns the polynomial to std::string
        std::string to_std_string(Textual_Math_Settings* settings) const;

        // Methods operators
        // Add a polynomial to this one
        void __add(__Monomonial_Base* value);
        void __add(Polynomial_Base* value);
        // Divide a monomonial to this void
        void __divide(__Monomonial_Base* value);
        void __divide(Polynomial_Base* value);
        // Returns if two values are equals or not
        bool __is_equal(__Field_Element* value) const;
        bool __is_equal(Polynomial_Base* value) const;
        // Multiply a polynomial to this one
        void __multiply(__Field_Element* value);
        void __multiply(Polynomial_Base* value);
        // Substracts a monomonial / polynomial to this void
        void __substract(__Monomonial_Base* value);
        void __substract(Polynomial_Base* value);

        // Returns a polynomial from a monomonial where the unknows has been replaced
        static std::shared_ptr<Polynomial_Base> polynomial_from_modified_monomonial_unknows(__Monomonial_Base* used_monomonial, std::string unknown, Polynomial_Base* new_value);
        // Returns this polynomial where an unknown is replaced by an another unknown
        std::shared_ptr<Polynomial_Base> replace_unknown(std::string unknown, Polynomial_Base* new_value) const;
        // Simplify the polynomial
        std::shared_ptr<Polynomial_Base> simplify() const;

        // Comparaison
        virtual bool is_equal(__Field_Element* operand_1, __Field_Element* operand_2) const = 0;
        // Operations
        virtual std::shared_ptr<__Field_Element> addition(__Field_Element* operand_1, __Field_Element* operand_2) = 0;
        virtual std::shared_ptr<__Field_Element> division(__Field_Element* operand_1, __Field_Element* operand_2) = 0;
        virtual std::shared_ptr<__Field_Element> multiplication(__Field_Element* operand_1, const __Field_Element* operand_2) = 0;
        virtual std::shared_ptr<__Field_Element> substraction(__Field_Element* operand_1, __Field_Element* operand_2) = 0;
        // Property
        virtual std::shared_ptr<Polynomial_Base> clone() const = 0;
        virtual std::shared_ptr<Polynomial_Base> clone_empty() const = 0;
        virtual bool is_absorbing_multiplication(__Field_Element* operand) const = 0;
        virtual bool is_absorbing_multiplication(std::shared_ptr<__Field_Element> operand) const {return is_absorbing_multiplication(operand.get());};

        // Getters and setters
        inline std::vector<std::shared_ptr<__Monomonial_Base>>& monomonials() {return a_monomonials;};
        inline const std::vector<std::shared_ptr<__Monomonial_Base>>& monomonials_const() const {return a_monomonials;};

    private:

        // Each monomonial in the polynomial
        std::vector<std::shared_ptr<__Monomonial_Base>> a_monomonials = std::vector<std::shared_ptr<__Monomonial_Base>>();
	};

    template <typename T> class __Monomonial_Template : public __Monomonial_Base {
    public:
        // __Monomonial_Template constructor
        __Monomonial_Template(T factor):__Monomonial_Base(std::make_shared<T>(factor)){}
        __Monomonial_Template(T factor, std::vector<_Base_Unknown> unknowns):__Monomonial_Base(std::make_shared<T>(factor), unknowns){};
        __Monomonial_Template(std::shared_ptr<__Field_Element> factor, std::vector<_Base_Unknown> unknowns):__Monomonial_Base(factor, unknowns){};
        __Monomonial_Template(T factor, std::string unknow):__Monomonial_Base(std::make_shared<T>(factor),std::vector<_Base_Unknown>(1,_Base_Unknown(unknow))){};
        __Monomonial_Template(T factor, std::string unknow, int exponent):__Monomonial_Base(std::make_shared<T>(factor),std::vector<_Base_Unknown>(1,_Base_Unknown(unknow, exponent))){};

        // Comparaison
        virtual bool is_equal(__Field_Element* operand_1, __Field_Element* operand_2)const{return (*(reinterpret_cast<T*>(operand_1))) == (*(reinterpret_cast<T*>(operand_2)));};
        // Operations
        virtual std::shared_ptr<__Field_Element> addition(__Field_Element* operand_1, __Field_Element* operand_2){return std::make_shared<T>(*(reinterpret_cast<T*>(operand_1)) + *(reinterpret_cast<T*>(operand_2)));};
        virtual std::shared_ptr<__Field_Element> division(__Field_Element* operand_1, __Field_Element* operand_2){return std::make_shared<T>(*(reinterpret_cast<T*>(operand_1)) / *(reinterpret_cast<T*>(operand_2)));};
        virtual std::shared_ptr<__Field_Element> multiplication(__Field_Element* operand_1, const __Field_Element* operand_2){return std::make_shared<T>(*(reinterpret_cast<T*>(operand_1)) * *(reinterpret_cast<const T*>(operand_2)));};
        virtual std::shared_ptr<__Field_Element> substraction(__Field_Element* operand_1, __Field_Element* operand_2){return std::make_shared<T>(*(reinterpret_cast<T*>(operand_1)) - *(reinterpret_cast<T*>(operand_2)));};
        // Property
        virtual std::shared_ptr<__Monomonial_Base> clone()const{return std::make_shared<__Monomonial_Template<T>>(*factor(), unknowns_const());};
        virtual bool is_null(__Field_Element* operand)const{return (*(reinterpret_cast<T*>(operand))) == 0;};

        // Getters and setters
        inline T* factor()const{return reinterpret_cast<T*>(__factor());};
        inline void set_factor(T factor){__set_factor(std::make_shared<T>(factor));};
    };
    template <typename T> class Polynomial_Template : public Polynomial_Base {
    public:
        // Polynomial_Template constructor
        Polynomial_Template():Polynomial_Base(){};
        Polynomial_Template(T factor):Polynomial_Base(std::make_shared<__Monomonial_Template<T>>(factor)){};
        Polynomial_Template(__Monomonial_Base* factor):Polynomial_Base(factor->clone()){};
        Polynomial_Template(__Monomonial_Template<T> factor):Polynomial_Base(factor.clone()){};
        Polynomial_Template(T factor, std::vector<_Base_Unknown> unknowns):Polynomial_Base(std::make_shared<__Monomonial_Template<T>>(std::make_shared<T>(factor), unknowns)){};
        Polynomial_Template(std::shared_ptr<__Field_Element> factor, std::vector<_Base_Unknown> unknowns):Polynomial_Base(std::make_shared<__Monomonial_Template<T>>(factor, unknowns)){};
        Polynomial_Template(T factor, std::string unknow):Polynomial_Base(std::make_shared<__Monomonial_Template<T>>(std::make_shared<T>(factor),std::vector<_Base_Unknown>(1,_Base_Unknown(unknow)))){};
        Polynomial_Template(T factor, std::string unknow, int exponent):Polynomial_Base(std::make_shared<__Monomonial_Template<T>>(std::make_shared<T>(factor),std::vector<_Base_Unknown>(1,_Base_Unknown(unknow, exponent)))){};
        // Polynomial_Template copy constructor
        Polynomial_Template(const Polynomial_Template& polynomial_copy):Polynomial_Template(){for(int i = 0;i<static_cast<int>(polynomial_copy.monomonials_const().size());i++){__add_monomonial(polynomial_copy.monomonials_const().at(i).get());}};

        // Add a new monomonial to the polynomial
        void add_monomonial(__Monomonial_Template<T> new_monomonial){__add_monomonial(&new_monomonial);};
        // Returns the bigger monomonial
        __Monomonial_Template<T>* bigger_monomonial() const {return reinterpret_cast<__Monomonial_Template<T>*>(__bigger_monomonial());};
        // Returns the knows monomonial
        __Monomonial_Template<T>* known_monomonial() const {return reinterpret_cast<__Monomonial_Template<T>*>(__known_monomonial());};
        T known_monomonial_factor() const {__Monomonial_Template<T>* needed = known_monomonial();if(needed == 0){return T(0);}return *known_monomonial()->factor();};

        // Returns a monomonial by its unknown
        __Monomonial_Template<T>* monomonial(std::string unknown, int exponent){return reinterpret_cast<__Monomonial_Template<T>*>(__monomonial(unknown, exponent));};
        __Monomonial_Template<T>* monomonial(std::string unknown){return reinterpret_cast<__Monomonial_Template<T>*>(__monomonial(unknown));};
        __Monomonial_Template<T>* monomonial(int index){return reinterpret_cast<__Monomonial_Template<T>*>(monomonials().at(index).get());};
        __Monomonial_Template<T>* monomonial(){return reinterpret_cast<__Monomonial_Template<T>*>(__monomonial());};

        // Returns if two values are equals or not
        bool is_equal(T value) const {return __is_equal(&value);};
        // Multiply a polynomial to this one
        void multiply(T value){__multiply(&value);};

        // Comparaison
        virtual bool is_equal(__Field_Element* operand_1, __Field_Element* operand_2)const{return (*(reinterpret_cast<T*>(operand_1))) == (*(reinterpret_cast<T*>(operand_2)));};
        // Operations
        virtual std::shared_ptr<__Field_Element> addition(__Field_Element* operand_1, __Field_Element* operand_2){return std::make_shared<T>(*(reinterpret_cast<T*>(operand_1)) + *(reinterpret_cast<T*>(operand_2)));};
        virtual std::shared_ptr<__Field_Element> division(__Field_Element* operand_1, __Field_Element* operand_2){return std::make_shared<T>(*(reinterpret_cast<T*>(operand_1)) / *(reinterpret_cast<T*>(operand_2)));};
        virtual std::shared_ptr<__Field_Element> multiplication(__Field_Element* operand_1, const __Field_Element* operand_2){return std::make_shared<T>(*(reinterpret_cast<T*>(operand_1)) * *(reinterpret_cast<const T*>(operand_2)));};
        virtual std::shared_ptr<__Field_Element> substraction(__Field_Element* operand_1, __Field_Element* operand_2){return std::make_shared<T>(*(reinterpret_cast<T*>(operand_1)) - *(reinterpret_cast<T*>(operand_2)));};
        // Property
        virtual std::shared_ptr<Polynomial_Base> clone()const{std::shared_ptr<Polynomial_Base> to_return = std::make_shared<Polynomial_Template<T>>();for(int i = 0;i<static_cast<int>(monomonials_const().size());i++){to_return.get()->__add_monomonial(monomonials_const().at(i).get());}return to_return;};
        virtual std::shared_ptr<Polynomial_Base> clone_empty()const{std::shared_ptr<Polynomial_Base> to_return = std::make_shared<Polynomial_Template<T>>();return to_return;};
        virtual bool is_absorbing_multiplication(__Field_Element* operand)const{return (*(reinterpret_cast<T*>(operand))) == 0;};

        // Operators
        Polynomial_Template<T>& operator+=(__Monomonial_Template<T> to_add){__add(&to_add);return *this;};
    };

}

#endif // SCLS_MATH_polynomial
