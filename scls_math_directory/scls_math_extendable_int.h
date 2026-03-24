//******************
//
// scls_math_extendable_int.h
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
// This file contains some tools to work with very very big / precise numbers in C++.
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

#ifndef SCLS_MATH_EXTANDABLE_INT
#define SCLS_MATH_EXTANDABLE_INT

// Include SCLS Math bigger header
#include "../scls_math.h"

namespace scls {
    // Returns the number of bit in a number (position of the last bit)
    int bit_in_number(unsigned long long t);

    constexpr double ln_2 = 0.69314718055994530941723212145818;
    constexpr double ln_10 = 2.3025850929940456840179914546844;

    class Extendable_Int {
        // Class representing an extendable integer
    public:
        // Extendable_Int constructor
        Extendable_Int(){};
        //Extendable_Int(unsigned int i){a_parts[0] = i;};
        Extendable_Int(unsigned long long i){a_parts[0] = i;i = i >> 32;a_parts.push_back(i);simplify();};
        Extendable_Int(const Extendable_Int& m){a_parts = m.a_parts;};
        // Extendable_Int destructor
        ~Extendable_Int(){};

        // Add an integer to this one
        void add(Extendable_Int* c) {
            std::vector<unsigned int> min_parts;
            std::vector<unsigned int> max_parts;
            if(c->a_parts.size() < a_parts.size()){min_parts = c->a_parts;max_parts = a_parts;}
            else {min_parts = a_parts;max_parts = c->a_parts;}
            a_parts = std::vector<unsigned int>(max_parts.size());

            // Do the addition
            unsigned long long r = 0;
            for(std::size_t i = 0;i<min_parts.size();i++) {
                unsigned long long v = min_parts.at(i) + max_parts.at(i) + r;
                r = (v / 0b100000000000000000000000000000000);
                v = v - (r * 0b100000000000000000000000000000000);
                a_parts[i] = v;
            }
            for(std::size_t i = min_parts.size();i<max_parts.size();i++) {
                unsigned long long v = max_parts.at(i) + r;
                r = (v / 0b100000000000000000000000000000000);
                v = v - (r * 0b100000000000000000000000000000000);
                a_parts[i] = v;
            }
        }

        // Returns if the current Int is bigger than an other or not
        bool is_bigger_than(Extendable_Int *c) {
            // Parts
            if(c->a_parts.size() > a_parts.size()){return false;}
            else if(a_parts.size() > c->a_parts.size()){return true;}

            for(std::size_t i = 0;i<a_parts.size();i++){if(a_parts.at(a_parts.size() - (i + 1)) > c->a_parts.at(a_parts.size() - (i + 1))){return true;}else if(a_parts.at(a_parts.size() - (i + 1)) < c->a_parts.at(a_parts.size() - (i + 1))){return false;}}
            return false;
        }
        bool is_equal(Extendable_Int *c) {
            // Parts
            if(c->a_parts.size() != a_parts.size()){return false;}

            for(std::size_t i = 0;i<a_parts.size();i++){if(a_parts.at(i) != c->a_parts.at(i)){return false;}}
            return true;
        }

        // Returns if the Int has a value
        bool is_null(){return a_parts.size() == 1 && a_parts.at(0) == 0;}

        // Divide an integer to this one
        void divide(unsigned long long c){Extendable_Int i = Extendable_Int(c);divide(&i);}
        void divide(Extendable_Int* c) {
            Extendable_Int t = (*this);
            int t_size = t.size_in_bit();
            Extendable_Int u = (*c);
            int u_size = u.size_in_bit();

            // Do the substraction
            Extendable_Int one = Extendable_Int(1);
            Extendable_Int result = Extendable_Int(0);
            u.shift_left(t_size - u_size);
            while(t_size >= u_size) {
                result.shift_left(1);
                if(t.is_bigger_than(&u) || t.is_equal(&u)) {t.sub(&u);result.add(&one);}
                t_size--;u.shift_right(1);
            }
            (*this) =  result;
        }
        void modulo(Extendable_Int* c) {
            Extendable_Int t = (*this);
            int t_size = t.size_in_bit();
            Extendable_Int u = (*c);
            int u_size = u.size_in_bit();

            // Do the substraction
            u.shift_left(t_size - u_size);
            while(t_size >= u_size) {
                if(t.is_bigger_than(&u) || t.is_equal(&u)) {t.sub(&u);}
                t_size = t.size_in_bit();
                u.shift_right(1);
            }

            (*this) = u;
        }

        // Multiply an integer to this one
        void multiply(unsigned long long c){Extendable_Int i = Extendable_Int(c);multiply(&i);}
        void multiply(Extendable_Int* c) {
            // Size
            if(c->a_parts.size() == 1) {
                // Do the multiplication
                unsigned long long current_m = c->a_parts.at(0);
                unsigned long long r = 0;
                for(std::size_t i = 0;i<a_parts.size();i++) {
                    unsigned long long v = current_m * static_cast<unsigned long long>(a_parts.at(i)) + r;
                    r = v >> 32;
                    v = (v << 32) >> 32;
                    a_parts[i] = v;
                }

                if(r != 0){a_parts.push_back(r);}
            }
            else if(a_parts.size() == 1) {Extendable_Int temp = (*this);(*this) = (*c);multiply(&temp);}
        }

        // Shifts the number
        void shift_left(int n){
            // Preparation
            if(n == 0){return;}
            while(n > 32){n -= 32;a_parts.insert(a_parts.begin(), 0);}

            unsigned int r = 0;
            for(std::size_t i = 0;i<a_parts.size();i++) {
                unsigned int r_temp = a_parts[i] >> (32 - n);
                a_parts[i] = (a_parts[i] << n) | r;
                r = r_temp;
            }

            if(r != 0){a_parts.push_back(r);}
            simplify();
        }
        void shift_right(int n){
            // Preparation
            if(n == 0){return;}
            while(n > 32){n -= 32;a_parts.erase(a_parts.end() - 1);}

            unsigned int r = 0;
            std::size_t start_size = a_parts.size();
            for(std::size_t i = 0;i<a_parts.size();i++) {
                std::size_t current_i = a_parts.size() - (i + 1);
                unsigned int r_temp = a_parts[current_i] << (32 - n);
                a_parts[current_i] = (a_parts[current_i] >> n) | r;
                r = r_temp;
            }

            simplify();
        }

        // Simplify the integer
        void simplify(){while(a_parts.size() > 1 && a_parts.at(a_parts.size() - 1) == 0){a_parts.pop_back();}}

        // Sub an integer to this one
        void sub(Extendable_Int* c) {
            if(c->is_bigger_than(this)) {

            }
            else {
                // Do the substraction
                unsigned long long r = 0;
                for(std::size_t i = 0;i<c->a_parts.size();i++) {
                    long long v = a_parts.at(i) - (c->a_parts.at(i) + r);
                    if(v >= 0){a_parts[i] = v;r=0;}
                    else{
                        a_parts[i] = 0b100000000000000000000000000000000 + v;
                        r = 1 + (static_cast<unsigned long long>(-v) / 0b100000000000000000000000000000000);
                    }
                }

                // Rest
                for(std::size_t i = c->a_parts.size();i<a_parts.size();i++) {
                    unsigned long long v = a_parts.at(i) - r;
                    r = (v / 0b100000000000000000000000000000000);
                    v = v - (r * 0b100000000000000000000000000000000);
                    a_parts[i] = v;
                }

                simplify();
            }
        }

        // Convert directly to std string
        std::string to_std_string() {
            //if(a_parts.size() > 1){return std::to_string(((unsigned long long)a_parts.at(1)) * 0b100000000000000000000000000000000ll + (unsigned long long)a_parts.at(0));}
            //return std::to_string(a_parts.at(0));

            std::string to_return = std::string();
            Extendable_Int f = (*this);
            int current_part = a_parts.size() - 1;
            int size_f = f.size_in_bit();
            unsigned long long n = std::ceil(static_cast<double>(size_f) * (ln_2 / ln_10));
            for(int i = 0;i<n;i++) {
                // Get the power
                Extendable_Int d = 1;
                Extendable_Int temp = Extendable_Int(10);
                for(int j = 1;j<n - i;j++){d.multiply(&temp);}

                Extendable_Int mod = f;
                mod.divide(&d);temp = mod;
                mod.multiply(&d);
                f.sub(&mod);

                std::string to_add = std::to_string(temp.a_parts.at(0));
                if(to_add.empty() && !to_return.empty()){to_add = std::string("0");}
                to_return += to_add;
            }
            return to_return;
        };

        // Returns the size in bit
        inline unsigned long long size_in_bit()const{std::size_t c_i = a_parts.size() - 1;while(c_i > 0 && a_parts.at(c_i) == 0){c_i--;}return bit_in_number(a_parts.at(c_i)) + c_i * 32;};

        // Getters and setters
        inline std::vector<unsigned int>& parts() {return a_parts;};
    private:
        // Different pars of the integer
        std::vector<unsigned int> a_parts = std::vector<unsigned int>(1, 0);
    };
}

#endif // SCLS_MATH_EXTANDABLE_INT
