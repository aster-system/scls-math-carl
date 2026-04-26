//******************
//
// scls_math_extendable_int.cpp
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
// This file contains the source code of "scls_math_extendable_int.h".
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

namespace scls {
    // Returns the number of bit in a number (position of the last bit)
    int bit_in_number(unsigned long long t) {
        if(t == 0ll){return 0;}

        unsigned long long max = 0b1111111111111111111111111111111111111111111111111111111111111111;
        unsigned long long current = 0b1000000000000000000000000000000000000000000000000000000000000000;
        for(int i = 0;i<64;i++){
            if(((~current) | t) == max){return 64 - i;}
            current = current >> 1;
        }
        return 0;
    }

    // Extendable_Int constructor
    Extendable_Int::Extendable_Int(){};
    //Extendable_Int(unsigned int i){a_parts[0] = i;};
    Extendable_Int::Extendable_Int(int64_t i_start){uint64_t i = i_start;if(i_start < 0){i = -i_start;a_negative = true;}a_parts[0] = i;i = i >> 32;a_parts.push_back(i);simplify();};
    Extendable_Int::Extendable_Int(const Extendable_Int& m){a_negative = m.a_negative;a_parts = m.a_parts;};
    // Extendable_Int destructor
    Extendable_Int::~Extendable_Int(){};

    // Add an integer to this one
    void Extendable_Int::add(Extendable_Int* c) {
    	if(c->a_negative != a_negative){
    		Extendable_Int temp = Extendable_Int(*c);
    		temp.a_negative = !temp.a_negative;
    		sub(&temp);
    	}
    	else {
    		std::vector<unsigned int> min_parts;
			std::vector<unsigned int> max_parts;
			if(c->a_parts.size() < a_parts.size()){min_parts = c->a_parts;max_parts = a_parts;}
			else {min_parts = a_parts;max_parts = c->a_parts;}
			a_parts = std::vector<unsigned int>(max_parts.size());

			// Do the addition
			uint64_t r = 0;
			for(std::size_t i = 0;i<min_parts.size();i++) {
				uint64_t v = static_cast<uint64_t>(min_parts.at(i)) + static_cast<uint64_t>(max_parts.at(i)) + r;
				r = (v >> 32);
				v = ((v << 32) >> 32);
				a_parts[i] = v;
			}
			for(std::size_t i = min_parts.size();i<max_parts.size();i++) {
				uint64_t v = static_cast<uint64_t>(max_parts.at(i)) + r;
				r = (v >> 32);
				v = ((v << 32) >> 32);
				a_parts[i] = v;
			}

			// Add the rest
			if(r != 0){a_parts.push_back(r);}
    	}
    }

    // Returns if the current Int is bigger than an other or not
    bool Extendable_Int::is_bigger_than_without_sign(Extendable_Int *c) const {
        // Parts
        if(c->a_parts.size() > a_parts.size()){return false;}
        else if(a_parts.size() > c->a_parts.size()){return true;}

        for(std::size_t i = 0;i<a_parts.size();i++){if(a_parts.at(a_parts.size() - (i + 1)) > c->a_parts.at(a_parts.size() - (i + 1))){return true;}else if(a_parts.at(a_parts.size() - (i + 1)) < c->a_parts.at(a_parts.size() - (i + 1))){return false;}}
        return false;
    }
    bool Extendable_Int::is_bigger_than(Extendable_Int *c) const {
        // Sign
        bool good = true;
        if(c->a_negative && !a_negative){return true;}
        else if(a_negative && !c->a_negative){return false;}
        else if(a_negative && c->a_negative) {good = false;}

        // Parts
        return is_bigger_than_without_sign(c) == good;
    }
    bool Extendable_Int::is_equal(const Extendable_Int *c) const {
        // Sign
        if(is_null() && c->is_null()){return true;}
        if(c->a_negative != a_negative){return false;}

        // Parts
        if(c->a_parts.size() != a_parts.size()){return false;}

        for(std::size_t i = 0;i<a_parts.size();i++){if(a_parts.at(i) != c->a_parts.at(i)){return false;}}
        return true;
    }

    // Divide an integer to this one
    void Extendable_Int::divide(int64_t c){Extendable_Int i = Extendable_Int(c);divide(&i);}
    void Extendable_Int::divide(const Extendable_Int* c) {
        Extendable_Int t = (*this);
        t.set_negative(false);
        int t_size = t.size_in_bit();
        Extendable_Int u = (*c);
        u.set_negative(false);
        int u_size = u.size_in_bit();

        // Do the division
        Extendable_Int one = Extendable_Int(1);
        Extendable_Int result = Extendable_Int(0);
        u.shift_left(t_size - u_size);
        while(t_size >= u_size) {
            result.shift_left(1);
            if(t.is_bigger_than(&u) || t.is_equal(&u)) {t.sub(&u);result.add(&one);}
            t_size--;u.shift_right(1);
        }
        (*this) = result;

        // Sign
        a_negative = (c->a_negative != a_negative);
        simplify();
    }
    void Extendable_Int::modulo(Extendable_Int* c) {
        Extendable_Int temp = (*this);
    	temp.divide(c);
    	temp.multiply(c);
    	//std::cout << "K " << temp.to_std_string() << " " << to_std_string() << " " << temp.is_null() << std::endl;
		sub(&temp);
		//std::cout << "L " << temp.to_std_string() << " " << to_std_string() << std::endl;
    }

    // Multiply an integer to this one
    void Extendable_Int::multiply(int64_t c){Extendable_Int i = Extendable_Int(c);multiply(&i);}
    void Extendable_Int::multiply(Extendable_Int* c) {
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

            // Sign
            a_negative = (c->a_negative != a_negative);
        }
        else if(a_parts.size() == 1 || c->a_parts.size() > a_parts.size()) {Extendable_Int temp = (*this);(*this) = (*c);multiply(&temp);}
        else {
            Extendable_Int result = 0;
            for(int i = 0;i<a_parts.size();i++) {
                Extendable_Int current = 1;if(i < c->a_parts.size()){current = c->a_parts.at(i);}
                Extendable_Int temp = (*this);//for(int j = 0;j<i;j++){temp.a_parts.erase(temp.a_parts.end() - 1);}
                temp.multiply(&current);temp.shift_left(32 * i);result.add(&temp);//std::cout << "H " << i << " " << temp.to_std_string() << std::endl;
            }
            (*this) = result;
            //std::cout << "JJJJJJJJJJJJJ" << std::endl;
        }

        // Simplify
        simplify();
    }

    // Shifts the number
    void Extendable_Int::shift_left(int n){
        // Preparation
        while(n >= 32){n -= 32;a_parts.insert(a_parts.begin(), 0);}
        if(n == 0){return;}

        unsigned int r = 0;
        for(std::size_t i = 0;i<a_parts.size();i++) {
            unsigned int r_temp = a_parts[i] >> (32 - n);
            a_parts[i] = (a_parts[i] << n) | r;
            r = r_temp;
        }

        if(r != 0){a_parts.push_back(r);}
        simplify();
    }
    void Extendable_Int::shift_right(int n){
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
    void Extendable_Int::simplify(){while(a_parts.size() > 1 && a_parts.at(a_parts.size() - 1) == 0){a_parts.pop_back();}}

    // Returns the size in bit
    unsigned long long Extendable_Int::size_in_base(unsigned long long base) const {return std::ceil(static_cast<double>(size_in_bit()) * (ln_2 / std::log(base)));}
    unsigned long long Extendable_Int::size_in_bit()const{std::size_t c_i = a_parts.size() - 1;while(c_i > 0 && a_parts.at(c_i) == 0){c_i--;}return bit_in_number(a_parts.at(c_i)) + c_i * 32;};

    // Sub an integer to this one
    void Extendable_Int::sub(Extendable_Int* c) {
        // Assert
        if(c->is_null()){return;}

        if(c->a_negative != a_negative){
        	Extendable_Int temp = Extendable_Int(*c);
        	temp.a_negative = !temp.a_negative;
        	add(&temp);
        }
        else {
        	if(c->is_bigger_than_without_sign(this)) {
				Extendable_Int temp = Extendable_Int(*c);
				temp.sub(this);
				a_negative = !temp.a_negative;
				a_parts = temp.a_parts;
			}
			else {
				// Do the substraction
				unsigned long long r = 0;
				for(std::size_t i = 0;i<c->a_parts.size();i++) {
					long long v = static_cast<int64_t>(a_parts.at(i)) - (static_cast<int64_t>(c->a_parts.at(i)) + r);
					if(v >= 0){a_parts[i] = v;r=0;}
					else{
						a_parts[i] = 0b100000000000000000000000000000000 + v;
						r = 1 + (static_cast<unsigned long long>(-v) / 0b100000000000000000000000000000000);
					}
				}

				// Rest
				for(std::size_t i = c->a_parts.size();i<a_parts.size();i++) {
					unsigned long long v = static_cast<uint64_t>(a_parts.at(i)) - r;
					r = (v / 0b100000000000000000000000000000000);
					v = v - (r * 0b100000000000000000000000000000000);
					a_parts[i] = v;
				}

				simplify();
			}
        }
    }

    // Returns the number to an int32
    int32_t Extendable_Int::to_int32() const {if(a_negative){return static_cast<int32_t>(a_parts.at(0)) * -1;}return a_parts.at(0);}
    int64_t Extendable_Int::to_int64() const {
        // 0 or 1 parts
        if(a_parts.size() <= 1){
            if(a_parts.at(0) < 0b10000000000000000000000000000000){return static_cast<int64_t>(to_int32());}
            else{
                int64_t to_return = static_cast<uint64_t>(a_parts.at(0));
                if(a_negative){return to_return * -1;}
                return to_return;
            }
        }

        // More than one part
        uint64_t to_return_u = static_cast<uint64_t>(a_parts.at(0)) + (static_cast<uint64_t>(a_parts.at(1)) << 32);

        // Sign
        int64_t to_return = to_return_u;
        if(a_negative){return to_return * -1;}
        return to_return;
    }
    uint64_t Extendable_Int::to_uint64_end() const {
        if(a_parts.size() <= 0){return 0;}
        else if(a_parts.size() <= 1){return static_cast<uint64_t>(a_parts.at(0));}

        // More than one part
        uint64_t to_return_u = static_cast<uint64_t>(a_parts.at(a_parts.size() - 2)) + (static_cast<uint64_t>(a_parts.at(a_parts.size() - 1)) << 32);
        return to_return_u;
    }
    // Convert directly to std string
    std::string Extendable_Int::to_std_string() const {
        std::string to_return = std::string();
        Extendable_Int f = (*this);
        f.a_negative = false;
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
        while(to_return.size() > 0 && to_return.at(0) == '0'){to_return = to_return.substr(1, to_return.size() - 1);}
        if(to_return == std::string_view()){to_return = std::string("0");}
        if(a_negative){to_return = std::string("-") + to_return;}
        return to_return;
    };

    // Operator
    bool Extendable_Int::operator==(int64_t i) const {
    	Extendable_Int temp = Extendable_Int(i);
    	return is_equal(&temp);
    }
    bool Extendable_Int::operator!=(int64_t i) const {
		Extendable_Int temp = Extendable_Int(i);
		return !is_equal(&temp);
	}
	bool Extendable_Int::operator<(int64_t i) const {
		Extendable_Int temp = Extendable_Int(i);
		return !is_bigger_than(&temp);
	}
	bool Extendable_Int::operator>(int64_t i) const {
		Extendable_Int temp = Extendable_Int(i);
		return is_bigger_than(&temp);
	}

    // Returns a factorial calculated with an "Extendable_Int"
    Extendable_Int factorial_extendable(long long f) {
        Extendable_Int m = Extendable_Int(1);
        for(long long i = 0;i<f;i++) {scls::Extendable_Int n = scls::Extendable_Int(i + 1);m.multiply(&n);}
        return m;
    }
}
