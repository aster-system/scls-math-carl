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
        Extendable_Int();
        Extendable_Int(unsigned long long i);
        Extendable_Int(const Extendable_Int& m);
        // Extendable_Int destructor
        ~Extendable_Int();

        // Add an integer to this one
        void add(Extendable_Int* c);

        // Returns if the current Int is bigger than an other or not
        bool is_bigger_than(Extendable_Int *c);
        bool is_equal(Extendable_Int *c);

        // Returns if the Int has a value
        bool is_null(){return a_parts.size() == 1 && a_parts.at(0) == 0;}

        // Divide an integer to this one
        void divide(unsigned long long c);
        void divide(Extendable_Int* c);
        void divide_better(unsigned long long c);
        void divide_better(Extendable_Int* c);
        void modulo(Extendable_Int* c);

        // Multiply an integer to this one
        void multiply(unsigned long long c);
        void multiply(Extendable_Int* c);

        // Shifts the number
        void shift_left(int n);
        void shift_right(int n);

        // Simplify the integer
        void simplify();

        // Sub an integer to this one
        void sub(Extendable_Int* c);

        // Convert directly to std string
        std::string to_std_string();

        // Returns the size in bit
        unsigned long long size_in_base(unsigned long long base) const;
        unsigned long long size_in_bit() const;

        // Getters and setters
        inline std::vector<unsigned int>& parts() {return a_parts;};
    private:
        // Different pars of the integer
        std::vector<unsigned int> a_parts = std::vector<unsigned int>(1, 0);
    };

    // Returns a factorial calculated with an "Extendable_Int"
    Extendable_Int factorial_extendable(long long f);
}

#endif // SCLS_MATH_EXTANDABLE_INT
