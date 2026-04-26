//******************
//
// scls_math_extendable_fraction.cpp
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
// This file contains the source code of "scls_math_extendable_fraction.h".
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

// Include SCLS Math bigger header
#include "../scls_math.h"
#include "../scls_math_directory/scls_math_extendable_fraction.h"

namespace scls {
    // Extendable_Fraction constructor
    Extendable_Fraction::Extendable_Fraction(){};
    Extendable_Fraction::Extendable_Fraction(double f):Extendable_Fraction(Fraction::from_double(f)){}
    Extendable_Fraction::Extendable_Fraction(Fraction f):Extendable_Fraction(f.numerator(), f.denominator()){}
    Extendable_Fraction::Extendable_Fraction(Extendable_Int value):a_numerator(value){};
    Extendable_Fraction::Extendable_Fraction(int64_t numerator, int64_t denominator):a_denominator(denominator),a_numerator(numerator){};
    Extendable_Fraction::Extendable_Fraction(Extendable_Int numerator, Extendable_Int denominator):a_denominator(denominator),a_numerator(numerator){};
    Extendable_Fraction::~Extendable_Fraction(){};

    // Add two fractions
    void Extendable_Fraction::add(Extendable_Fraction* other) {
        Extendable_Fraction modified = (*other);
        Extendable_Int temp = a_numerator;
        temp.multiply(&other->a_denominator);
        modified.a_denominator.multiply(&a_denominator);
        modified.a_numerator.multiply(&a_denominator);

        //std::cout << "D " << a_numerator.to_std_string() << " " << a_denominator.to_std_string() << " " << temp.to_std_string() << " "<< to_double() << std::endl;
        //std::cout << "E " << other->a_numerator.to_std_string() << " " << other->a_denominator.to_std_string() << " " << other->to_double() << std::endl;
        //std::cout << "F " << modified.a_numerator.to_std_string() << " " << modified.a_denominator.to_std_string() << " " << modified.to_double() << std::endl;

        modified.a_numerator.add(&temp);

        //std::cout << "G " << modified.a_numerator.to_std_string() << " " << modified.a_denominator.to_std_string() << " " << modified.to_double() << std::endl;

        modified.normalize();

        //std::cout << "H " << modified.a_numerator.to_std_string() << " " << modified.a_denominator.to_std_string() << " " << modified.to_double() << std::endl;

        (*this) = modified;
    }

    // Divide two fractions
    void Extendable_Fraction::divide(Extendable_Fraction* other) {
        a_denominator.multiply(&other->a_numerator);
        a_numerator.multiply(&other->a_denominator);
        //std::cout << "A " << a_numerator.to_std_string() << " " << a_denominator.to_std_string() << " " << to_double() << std::endl;
        //std::cout << "B " << other->a_numerator.to_std_string() << " " << other->a_denominator.to_std_string() << " " << other->to_double() << std::endl;
        normalize();
        //std::cout << "C " << a_numerator.to_std_string() << " " << a_denominator.to_std_string() << " " << a_numerator.to_uint64_end() << " " << a_denominator.to_uint64_end() << " " << to_double() << std::endl;
    }

    // Multiply two fractions
    void Extendable_Fraction::multiply(Extendable_Fraction* other) {
        a_denominator.multiply(&other->a_denominator);
        a_numerator.multiply(&other->a_numerator);
        normalize();
    }

    // Operates this element with another one
    void Extendable_Fraction::operate(Algebra_Element* other, std::string operation) {
        if(operation == "+"){add(reinterpret_cast<Extendable_Fraction*>(other));}
        else if(operation == "*"){multiply(reinterpret_cast<Extendable_Fraction*>(other));}
        else if(operation == "/"){divide(reinterpret_cast<Extendable_Fraction*>(other));}
        else if(operation == "exp"){double temp = to_double();temp = std::exp(temp);(*this) = Fraction::from_double(temp);}
    }

    // Substract two fractions
    void Extendable_Fraction::sub(Extendable_Fraction* other) {
        Extendable_Fraction modified = (*other);
        modified.a_numerator.multiply(-1);
        add(&modified);
    }

    // Integer part
    Extendable_Int Extendable_Fraction::integer_part() const {Extendable_Int temp = a_numerator;temp.divide(&a_denominator);return temp;};

    // Normalize the fraction
    void Extendable_Fraction::normalize(int max_size_in_octets) {
        normalize();

        int s_1 = a_numerator.size_in_bit() / 8;
        int s_2 = a_numerator.size_in_bit() / 8;
        int max_s = std::max(s_1, s_2);
        if(max_s > max_size_in_octets) {
            a_denominator.shift_right((max_s - max_size_in_octets) * 8);
            a_numerator.shift_right((max_s - max_size_in_octets) * 8);
        }
    }
    void Extendable_Fraction::normalize() {
        const Extendable_Int base_denominator = a_denominator; const Extendable_Int base_numerator = a_numerator;
        a_denominator.set_negative(false);a_numerator.set_negative(false);
        const Extendable_Int base_denominator_abs = a_denominator; const Extendable_Int base_numerator_abs = a_numerator;

        // Apply the Euclid algorithm
        if(a_numerator != 1 && a_denominator != 1) {
            if(a_numerator != 0) {
                int i = 0;
                while(a_denominator != 0 && i < 10) {
                    Extendable_Int t = a_denominator;
                    a_denominator = a_numerator;Extendable_Int temp = a_denominator;temp.divide(&t);temp.multiply(&t);
                    a_denominator.modulo(&t);//std::cout << "E " << a_numerator.to_std_string() << " " << t.to_std_string() << " " << temp.to_std_string() << " " << a_denominator.to_std_string() << std::endl;
                    a_numerator = t;i++;
                }
                Extendable_Int temp = a_numerator;
                a_denominator = base_denominator_abs;
                a_denominator.divide(&temp);
                a_numerator = base_numerator_abs;
                a_numerator.divide(&temp);
                //std::cout << "L " << temp.to_std_string() << std::endl;
            }
            else {a_denominator = 1;}
        }

        // Apply the sign
        if((base_denominator < 0 && base_numerator > 0) || (base_denominator > 0 && base_numerator < 0)){a_numerator.set_negative(true);}
    }

    // Returns the number to a double
    double Extendable_Fraction::to_double() const {
        int p_s_1 = a_numerator.parts_size();
        int p_s_2 = a_denominator.parts_size();
        if(p_s_1 == 1){p_s_1++;}
        if(p_s_2 == 1){p_s_2++;}
        uint64_t p_1 = a_numerator.to_uint64_end();
        uint64_t p_2 = a_denominator.to_uint64_end();

        // Refactoring
        if(p_s_1 > p_s_2) {p_2 = (p_2 >> (p_s_1 - p_s_2) * 32);}
        else if(p_s_2 > p_s_1) {p_1 = (p_1 >> (p_s_2 - p_s_1) * 32);}

        double s = -(a_numerator < 0);if(s == 0){s = 1;}
        return (static_cast<double>(p_1) / static_cast<double>(p_2)) * s;
    }

    // Convert directly to std string
    std::string Extendable_Fraction::to_mathml(Textual_Math_Settings* settings) const {return to_std_string(settings);}
    std::string Extendable_Fraction::to_std_string(Textual_Math_Settings* settings) const {return a_numerator.to_std_string() + std::string("/") + a_denominator.to_std_string();}
    std::string Extendable_Fraction::to_std_string_decimal(Textual_Math_Settings* settings) const {
        Extendable_Int integer = integer_part();
        std::string to_return = integer.to_std_string() + std::string(".");

        Extendable_Fraction f = (*this);
        Extendable_Fraction temp = Extendable_Fraction(10);
        int64_t size = 6;
        for(int i = 0;i<size;i++) {
            // Integer part
            Extendable_Fraction integer_fraction = Extendable_Fraction(f.integer_part());
            f.sub(&integer_fraction);

            // Get the power
            f.multiply(&temp);

            std::string to_add = f.integer_part().to_std_string();
            if(to_add.empty() && !to_return.empty()){to_add = std::string("0");}
            to_return += to_add;
        }//*/

        while(to_return.at(to_return.size() - 1) == '0'){to_return = to_return.substr(0, to_return.size() - 1);}
        while(to_return.at(to_return.size() - 1) == '.'){to_return = to_return.substr(0, to_return.size() - 1);}
        return to_return;
    }
}
