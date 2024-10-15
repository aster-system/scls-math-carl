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

        // Getters and setters
        inline bool conjugate() const {return a_conjugate;};
        inline Complex exponent() const {return a_exponent;};
        inline std::string name() const {return a_name;};
        inline void set_exponent(Complex new_exponent) {a_exponent = new_exponent;};
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
        // Monomonial copy constructor
        Monomonial(const Monomonial& monomonial_copy):Monomonial(monomonial_copy.a_factor,monomonial_copy.a_unknowns){};

        // Returns if a monomonial has the same unknows as this one
        inline bool compare_unknown(Monomonial other) {
            if(other.unknowns_number() != unknowns_number()) return false;
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
                if(a_unknowns[i].name() == "") continue; bool contains_equal = false;
                for(int j = 0;j<static_cast<int>(other.a_unknowns.size());j++) {
                    if(a_unknowns[i] == other.a_unknowns[j]) {contains_equal = true;break;}
                } if(!contains_equal) return false;
            } return true;
        };
        // Returns if the monomonial contains an unkwnown
        _Base_Unknown* contains_unknown(std::string unknown_name) {
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
                if(a_unknowns[i].name() == unknown_name) {
                    return &a_unknowns[i];
                }
            } return 0;
        };
        // Returns the factor of the monomonial
        inline Complex factor() const {return a_factor;};
        // Unknows of the monomonial
        inline std::vector<_Base_Unknown>& unknowns() {return a_unknowns;};
        inline unsigned int unknowns_number() const {
            unsigned int to_return = 0;
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++){if(a_unknowns[i].name() != "")to_return++;}
            return to_return;
        };

        // Returns the monomonial converted to std::string
        inline std::string to_std_string() const {
            std::string final_unknow = "";
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
                final_unknow += a_unknowns.at(i).name();
                if(a_unknowns.at(i).name() != "" && a_unknowns.at(i).exponent() != 1) final_unknow += "^" + a_unknowns.at(i).exponent().to_std_string_simple();
            }
            return "(" + a_factor.to_std_string_simple() + ")" + final_unknow;
        };

        // Minus operator assignment
        Monomonial& operator-=(Monomonial const& obj) { a_factor -= obj.a_factor; return *this; }
        // Plus operator assignment
        Monomonial& operator+=(Monomonial const& obj) { a_factor += obj.a_factor; return *this; }
        // Multiplication operator assignment
        Monomonial& operator*=(Monomonial const& obj) {
            a_factor *= obj.a_factor;
            for(int i = 0;i<static_cast<int>(obj.a_unknowns.size());i++) {
                _Base_Unknown* contained_unknown = contains_unknown(obj.a_unknowns[i].name());
                if(contained_unknown == 0) {
                    a_unknowns.push_back(obj.a_unknowns[i]);
                }
                else if(obj.a_unknowns[i].name() != "") {contained_unknown->set_exponent(contained_unknown->exponent() + obj.a_unknowns[i].exponent());}
            }
            return *this;
        }

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
                if(a_monomonials.at(i).factor() != 0) {
                    to_return += a_monomonials.at(i).to_std_string();
                    if(i < static_cast<int>(a_monomonials.size()) - 1) {
                        to_return += " + ";
                    }
                }
            }
            while(to_return[to_return.size() - 1] == '+' || to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
            return to_return;
        };

        // Methods operators
        // Add a polymonial to this one
        void __add(Polymonial value) {
            for(int i = 0;i<static_cast<int>(value.a_monomonials.size());i++) {
                Monomonial& current_monomonial = value.a_monomonials[i];
                Monomonial* contained_monomonial = contains_monomonial(current_monomonial);
                if(contained_monomonial == 0) {
                    add_monomonial(current_monomonial);
                } else {
                    (*contained_monomonial) += current_monomonial;
                }
            }
        };
        // Multiply a polymonial to this one
        void __multiply(Polymonial value) {
            std::vector<Polymonial> created_polymonial;
            // Apply each addition
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                Polymonial current_polymonial;
                current_polymonial.add_monomonial(a_monomonials[i]);
                // Apply each multiplication
                for(int j = 0;j<static_cast<int>(value.a_monomonials.size());j++) {
                    Monomonial& current_monomonial = value.a_monomonials[j];
                    Polymonial needed_polymonial = current_polymonial;
                    needed_polymonial.a_monomonials[0] *= current_monomonial;
                    created_polymonial.push_back(needed_polymonial);
                }
            }
            // Apply the multiplication
            a_monomonials.clear();
            for(int i = 0;i<static_cast<int>(created_polymonial.size());i++) {
                __add(created_polymonial[i]);
            }
        };

        // Operators
        Polymonial& operator+=(Polymonial value) {__add(value);return*this;};
        Polymonial& operator*=(Polymonial value) {__multiply(value);return*this;};

        // Getters and setters
        inline std::vector<Monomonial>& monomonials() {return a_monomonials;};

    private:

        // Each monomonial in the polymonial
        std::vector<Monomonial> a_monomonials = std::vector<Monomonial>();
	};

	// Stream operator overloading
    static std::ostream& operator<<(std::ostream& os, Polymonial& obj) {os << obj.to_std_string(); return os; }

	// Divide operator
    static Polymonial operator/(Monomonial& obj, Monomonial const& other) {
        Polymonial to_return;
        to_return.add_monomonial(Monomonial(other.factor() / obj.factor(), obj.unknowns()));
        return to_return;
    }

    class __String_To_Polymonial_Parse {
        // Class allowing to properly parse a std::string to a polymonial
    public:
        // __String_To_Polymonial_Parse constructor
        __String_To_Polymonial_Parse(){};

        // Returns if a std::string is a number or not
        inline bool __string_is_number(char text) const {return (string_is_number(text) || text == '/' || text == 'i' || text == '-');};
        inline bool __string_is_number(std::string text) const {
            for(int i = 0;i<static_cast<int>(text.size());i++) {
                if(!__string_is_number(text[i])) return false;
            } return true;
        };
        // Returns if a std::string is an operator or not
        inline bool __string_is_operator(char text) const {return (text == '+' || text == '-' || text == '*' || text == '/');};
        // Returns a given first base string to a polymonial
        Polymonial __string_to_polymonial_base(std::string base) {
            Polymonial current_polymonial;

            if(base.size() < 2 || (base[0] != '(')) {
                // Simple form
                // Get the needed datas
                unsigned int number_i = 0; std::string number_part_1 = ""; std::string number_part_2 = ""; std::string unknow_part = "";
                while(number_i < base.size() && __string_is_number(base[number_i])){number_part_1+=base[number_i];number_i++;}
                while(number_i < base.size() && !__string_is_number(base[number_i])){unknow_part+=base[number_i];number_i++;}
                while(number_i < base.size() && __string_is_number(base[number_i])){number_part_2+=base[number_i];number_i++;}
                // Add the monomonial
                Complex number = string_to_complex(number_part_1);
                if(unknow_part != "" && number_part_1 == "") number = 1;
                if(number_part_2 != "") number *= string_to_complex(number_part_2);
                Monomonial to_add(number, unknow_part);
                current_polymonial.add_monomonial(to_add);
            } else {
                // Parenthesis form
                __String_To_Polymonial_Parse new_parser;
                current_polymonial = new_parser.string_to_polymonial(base.substr(1, base.size() - 2));
            }
            return current_polymonial;
        };

        // Converts a std::string to a Polymonial
        Polymonial __string_to_polymonial_without_addition(std::string source) {
            // Format the text as needed
            std::vector<std::string> cutted;

            // Prepare the needed datas
            Polymonial to_return; bool to_return_modified = false;

            // Cut the text operator by * operator
            cutted = cut_string_out_of_2(source, "*", "(", ")");
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                Polymonial current_polymonial = __string_to_polymonial_base(cutted[i]);
                if(to_return_modified) {to_return *= current_polymonial;}
                else{to_return = current_polymonial;to_return_modified = true;}
            }

            // Return the result
            return to_return;
        };
        Polymonial __string_to_polymonial_without_parenthesis(std::string source) {
            // Format the text as needed
            std::vector<std::string> cutted;

            // Prepare the needed datas
            Polymonial to_return; bool to_return_modified = false;

            // Cut the text operator by + operator
            cutted = cut_string_out_of_2(source, "+", "(", ")");
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                Polymonial current_polymonial = __string_to_polymonial_without_addition(cutted[i]);
                if(to_return_modified) {to_return += current_polymonial;}
                else{to_return = current_polymonial;to_return_modified = true;}
            }

            // Return the result
            return to_return;
        };
        Polymonial string_to_polymonial(std::string source) {
            // Format the text as needed
            source = remove_space(source);
            for(int i = 0;i<static_cast<int>(source.size());i++) {
                // Remove the useless "-"
                if(i > 0 && source[i] == '-') {
                    if(!__string_is_operator(source[i - 1]) && (i >= source.size() || (source[i + 1] != '('))) {
                        source.insert(i, "+");
                        i++;
                    }
                }
                // Remove the useless ")("
                if(i > 0 && source[i] == '(') {
                    if(source[i - 1] == ')') {
                        source.insert(i, "*");
                        i++;
                    } else if(source[i - 1] == '-') {
                        source.insert(i, "1*");
                        i++;
                    } else if(!__string_is_operator(source[i - 1])) {
                        source.insert(i, "*");
                        i++;
                    }
                }
            }
            std::vector<std::string> cutted;

            // Prepare the needed datas
            Polymonial to_return; bool to_return_modified = false;

            // Cut the text operator by + operator
            cutted = cut_string_out_of_2(source, "+", "(", ")");
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                Polymonial current_polymonial = __string_to_polymonial_without_addition(cutted[i]);
                if(to_return_modified) {to_return += current_polymonial;}
                else{to_return = current_polymonial;to_return_modified = true;}
            }

            // Return the result
            return to_return;
        };
    };

    static Polymonial string_to_polymonial(std::string source){
        __String_To_Polymonial_Parse parser;
        return parser.string_to_polymonial(source);
    };

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
