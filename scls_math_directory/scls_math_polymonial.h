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

#include "scls_math_fraction.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
	//
	// The "Monomonial" class
	//
	//*********

    class _Base_Unknown {
        // Class representating the base of an unknow in a monomonial
    public:

        // Unknow constructor
        _Base_Unknown(std::string new_name):a_name(new_name){};

        // Returnss the exponent of the unknown
        inline Complex exponent() const {return a_exponent;};
        // Returns the name of the unknown
        inline std::string name() const {return a_name;};
        // Change the value of name
        inline void set_name(std::string new_name) {a_name = new_name;};

    private:

        // Exponent of the unknow
        Complex a_exponent = Complex(1, 0);
        // Name of the unknow
        std::string a_name = "";
    };

    class Monomonial {
        // Class representating a monomonial in a polymonial form
    public:

        // Monomonial constructor
        Monomonial(Complex factor):a_factor(factor){};
        // Monomonial constructor
        Monomonial(Complex factor, std::string unknow):Monomonial(factor){a_unknowns[0].set_name(unknow);};

        // Returns the factor of the monomonial
        inline Complex factor() const {return a_factor;};
        // Unknows of the monomonial
        inline std::vector<_Base_Unknown>& unknowns() {return a_unknowns;};

        // Returns the monomonial converted to std::string
        inline std::string to_std_string() const {
            std::string final_unknow = "";
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
                final_unknow += a_unknowns.at(i).name();
            }
            return "(" + a_factor.to_std_string_simple() + ")" + final_unknow;
        };

    private:

        // Factor of the monomonial
        Complex a_factor = Complex(Fraction(0), Fraction(0));

        // Unknows of the monomonial
        std::vector<_Base_Unknown> a_unknowns = std::vector<_Base_Unknown>(1, _Base_Unknown("x"));
    };

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

        // Add a new monomonial to the polymonial
        void add_monomonial(Monomonial new_monomonial) {
            a_monomonials.push_back(new_monomonial);
        };

        // Returns the polymonial to std::string
        inline std::string to_std_string() const {
            std::string to_return = "";
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                to_return += a_monomonials.at(i).to_std_string();
            }
            return to_return;
        };

    private:

        // Each monomonial in the polymonial
        std::vector<Monomonial> a_monomonials = std::vector<Monomonial>();
	};

    //*********
	//
	// Redaction methods
	//
	//*********

	// Returns the redaction of a 2 degrees polymonial solving
    std::string redaction_polymonial_solving_2_degrees(Fraction first_number, Fraction second_number, Fraction third_number) {
        std::string introduction = "Nous avons un polynôme de degré 2 tel que " + first_number.to_std_string_fraction() + "x^2 + " + second_number.to_std_string_fraction() + "x + " + third_number.to_std_string_fraction() + ".\n\n";

        // Search the "discriminant"
        std::string discriminant_research = "Nous cherchons le discriminant de cette fonction :\n";
        Fraction discriminant = second_number * second_number - first_number * third_number * 4;
        discriminant_research += "D = b^ - 4ac = " + second_number.to_std_string_fraction() + "^2 - 4 * " + first_number.to_std_string_fraction() + " * " + third_number.to_std_string_fraction() + " = " + discriminant.to_std_string_fraction();
        discriminant_research += "Le discriminant est donc égal à " + discriminant.to_std_string_fraction() + "\n\n";

        // Research the roots
        std::string root_research = "";
        if(discriminant > 0) {
            root_research += "Le discriminant est supérieur à 0, il y a donc deux solutions où f(x) = 0 dans l'ensemble des réels. Calculons les :\n";

            Fraction root_1 = (second_number * -1 + discriminant.sqrt()) / (first_number * 2);
            root_research += "x1 = (-b + sqrt(D)) / 2a = (" + (second_number * -1).to_std_string_fraction() + " + sqrt(" + discriminant.to_std_string_fraction() + ") / " + (first_number * 2).to_std_string_fraction() + " = " + root_1.to_std_string_fraction() + "\n";
            Fraction root_2 = (second_number * -1 - discriminant.sqrt()) / (first_number * 2);
            root_research += "x1 = (-b - sqrt(D)) / 2a = (" + (second_number * -1).to_std_string_fraction() + " - sqrt(" + discriminant.to_std_string_fraction() + ") / " + (first_number * 2).to_std_string_fraction() + " = " + root_2.to_std_string_fraction() + "\n";
            root_research += "Les racines de f sont donc x1 = " + root_1.to_std_string_fraction() + " et x2 = " + root_2.to_std_string_fraction() + ".\n\n";
        }

        // Create the final text
        std::string final_text = "";
        final_text += introduction;
        final_text += discriminant_research;
        final_text += root_research;
        return final_text;
    };
}

#endif // SCLS_MATH_POLYMONIAL
