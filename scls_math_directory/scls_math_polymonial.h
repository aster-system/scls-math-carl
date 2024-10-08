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
        // Possible unknows comparaison
        enum Unknown_Comparaison {UC_DIFFERENT, UC_EQUAL, UC_EQUAL_UNKNOWN};

        // Unknow constructor
        _Base_Unknown(std::string new_name):a_name(new_name){};

        // Returns the comparaison between two unknows
        inline bool compare_unknown(_Base_Unknown other) const {
            if(a_conjugate == other.a_conjugate && a_exponent == other.a_exponent && a_name == other.a_name) {
                return Unknown_Comparaison::UC_EQUAL;
            } return Unknown_Comparaison::UC_DIFFERENT;
        };
        // Returns if the unknow is a conjugate
        inline bool conjugate() const {return a_conjugate;};
        // Returns the exponent of the unknown
        inline Complex exponent() const {return a_exponent;};
        // Returns the name of the unknown
        inline std::string name() const {return a_name;};
        // Change the value of name
        inline void set_name(std::string new_name) {a_name = new_name;};

        // Operator ==
        inline bool operator==(_Base_Unknown other) const {
            return compare_unknown(other) == Unknown_Comparaison::UC_EQUAL;
        };

    private:

        // If the unknown is a conjugate
        bool a_conjugate = false;
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
        // Monomonial complete constructor
        Monomonial(Complex factor, std::vector<_Base_Unknown> unknowns):Monomonial(factor){a_unknowns=unknowns;};
        // Monomonial constructor
        Monomonial(Complex factor, std::string unknow):Monomonial(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow))){};

        // Returns if a monomonial has the same unknows as this one
        inline bool compare_unknown(Monomonial other) {
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
                bool contains_equal = false;
                for(int j = 0;j<static_cast<int>(other.a_unknowns.size());j++) {
                    if(a_unknowns[i] == other.a_unknowns[i]) {contains_equal = true;break;}
                }
                if(!contains_equal) return false;
            } return true;
        };
        // Returns the factor of the monomonial
        inline Complex factor() const {return a_factor;};
        // Unknows of the monomonial
        inline std::vector<_Base_Unknown>& unknowns() {return a_unknowns;};
        inline unsigned int unknowns_number() const {return a_unknowns.size();};

        // Returns the monomonial converted to std::string
        inline std::string to_std_string() const {
            std::string final_unknow = "";
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
                final_unknow += a_unknowns.at(i).name();
            }
            return "(" + a_factor.to_std_string_simple() + ")" + final_unknow;
        };

        // Minus operator assignment
        Monomonial& operator-=(Monomonial const& obj) { a_factor -= obj.a_factor; return *this; }
        // Plus operator assignment
        Monomonial& operator+=(Monomonial const& obj) { a_factor += obj.a_factor; return *this; }

        // Getters and setters
        inline void set_factor(Complex new_factor) {a_factor = new_factor;};

    private:

        // Factor of the monomonial
        Complex a_factor = Complex(Fraction(0), Fraction(0));

        // Unknows of the monomonial
        std::vector<_Base_Unknown> a_unknowns;
    };

    // Stream operator overloading
    static std::ostream& operator<<(std::ostream& os, const Monomonial& obj) { os << obj.to_std_string() ; return os; }

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
            Monomonial* same_monomonial = contains_monomonial(new_monomonial);
            if(same_monomonial == 0) a_monomonials.push_back(new_monomonial);
            else same_monomonial->set_factor(same_monomonial->factor() + new_monomonial.factor());
        };
        // Returns if the polymonial contains a monomonial
        Monomonial* contains_monomonial(Monomonial new_monomonial) {
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                if(a_monomonials[i].compare_unknown(new_monomonial)) {
                    return &a_monomonials[i];
                }
            } return 0;
        };
        // Returns the knows monomonial
        Monomonial known_monomonial() const {
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                if(a_monomonials.at(i).unknowns_number() <= 0) return a_monomonials.at(i);
            }
            return Monomonial(Complex(0));
        };
        // Returns a list of unknowns monomonials
        std::vector<Monomonial> unknown_monomonials() const {
            std::vector<Monomonial> to_return;
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                if(a_monomonials.at(i).unknowns_number() > 0) to_return.push_back(a_monomonials.at(i));
            }
            return to_return;
        };

        // Returns the polymonial to std::string
        inline std::string to_std_string() const {
            std::string to_return = "";
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                to_return += a_monomonials.at(i).to_std_string() + " + ";
            }
            while(to_return[to_return.size() - 1] == '+' || to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
            return to_return;
        };

    private:

        // Each monomonial in the polymonial
        std::vector<Monomonial> a_monomonials = std::vector<Monomonial>();
	};

	// Divide operator
    Polymonial operator/(Monomonial& obj, Monomonial const& other) {
        Polymonial to_return;
        to_return.add_monomonial(Monomonial(other.factor() / obj.factor(), obj.unknowns()));
        return to_return;
    }

    //*********
	//
	// Redaction (or without redaction) methods
	//
	//*********

	// Returns the redaction of a 2 degrees polymonial solving
    static std::string redaction_polymonial_solving_2_degrees(Fraction first_number, Fraction second_number, Fraction third_number) {
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
            root_research += "Les racines de f sont donc x1 = " + root_1.to_std_string_fraction() + " = " + std::to_string(root_1.to_double()) + " et x2 = " + root_2.to_std_string_fraction() + " = " + std::to_string(root_2.to_double()) + ".\n\n";
        }

        // Create the final text
        std::string final_text = "";
        final_text += introduction;
        final_text += discriminant_research;
        final_text += root_research;
        return final_text;
    };

    // Solve an equation between 2 polymonial with 1 unknown
    static Complex solve(Polymonial first_polymonial, Polymonial second_polymonial) {
        // Get the unknown in a single monomonial
        Monomonial unknown_monomonial(Complex(0));
        // Add the first unknown monomonial
        std::vector<Monomonial> unknowns_polymonials = first_polymonial.unknown_monomonials();
        if(unknowns_polymonials.size() > 0) {
            if(unknowns_polymonials.size() == 1) {
                Monomonial current_unknown_monomonial = unknowns_polymonials[0];
                unknown_monomonial -= current_unknown_monomonial;
            }
        }
        // Add the second unknown monomonial
        unknowns_polymonials = second_polymonial.unknown_monomonials();
        if(unknowns_polymonials.size() > 0) {
            if(unknowns_polymonials.size() == 1) {
                Monomonial current_unknown_monomonial = unknowns_polymonials[0];
                unknown_monomonial += current_unknown_monomonial;
            }
        }

        // Get the known part in a single monomonial
        Monomonial known_monomonial(Complex(0));
        known_monomonial += first_polymonial.known_monomonial();
        known_monomonial -= second_polymonial.known_monomonial();

        // Return the final datas
        std::cout << "U " << known_monomonial << " . " << first_polymonial.known_monomonial() << " " << second_polymonial.known_monomonial() << " - " << unknown_monomonial << std::endl;
        return (known_monomonial / unknown_monomonial).unknown_monomonials()[0].factor();
    };
}

#endif // SCLS_MATH_POLYMONIAL
