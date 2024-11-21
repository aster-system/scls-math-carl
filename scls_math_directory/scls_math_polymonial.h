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

#include "scls_math_numbers.h"

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
        _Base_Unknown(std::string new_name, Complex exponent):a_exponent(exponent),a_name(new_name){};
        _Base_Unknown(std::string new_name):_Base_Unknown(new_name, Complex(1)){};

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
        Monomonial(Complex factor, std::vector<_Base_Unknown> unknowns):Monomonial(factor){a_unknowns=unknowns;};
        Monomonial(Complex factor, std::string unknow):Monomonial(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow))){};
        Monomonial(Complex factor, std::string unknow, Complex exponent):Monomonial(factor,std::vector<_Base_Unknown>(1,_Base_Unknown(unknow, exponent))){};
        // Monomonial copy constructor
        Monomonial(const Monomonial& monomonial_copy):Monomonial(monomonial_copy.a_factor,monomonial_copy.a_unknowns){};

        // Returns the inverse the monomonial
        Monomonial inverse() const {
            Monomonial new_monomonial(*this);
            new_monomonial.set_factor(Complex(1) / new_monomonial.factor());
            // Reverse each unknowns
            for(int i = 0;i<static_cast<int>(new_monomonial.a_unknowns.size());i++) {
                new_monomonial.a_unknowns[i].set_exponent(new_monomonial.a_unknowns[i].exponent() * -1);
            } return new_monomonial;
        };
        // Returns if the limit of the monomonial for an unknown going to + infinity is +/- infinity / +/- 0
        bool limit_pi_is_mi(std::string unknown_name) {
            _Base_Unknown* unknown = contains_unknown(unknown_name);
            if(unknown != 0 && unknown->exponent().real() >= 1 && factor().real() < 0) {
                return true;
            } return false;
        };
        bool limit_pi_is_mz(std::string unknown_name) {
            _Base_Unknown* unknown = contains_unknown(unknown_name);
            if(unknown != 0 && unknown->exponent().real() <= 0 && factor().real() < 0) {
                return true;
            } return false;
        };
        bool limit_pi_is_pi(std::string unknown_name) {
            _Base_Unknown* unknown = contains_unknown(unknown_name);
            if(unknown != 0 && unknown->exponent().real() >= 1 && factor().real() > 0) {
                return true;
            } return false;
        };
        bool limit_pi_is_pz(std::string unknown_name) {
            _Base_Unknown* unknown = contains_unknown(unknown_name);
            if(unknown != 0 && unknown->exponent().real() <= 0 && factor().real() > 0) {
                return true;
            } return false;
        };

        // Add an unknown to the monomonial
        inline void add_unknown(std::string name, Complex exponent) {_Base_Unknown unknown(name); unknown.set_exponent(exponent); a_unknowns.push_back(unknown);};
        // Returns if a monomonial has the same unknows as this one
        inline bool compare_unknown(Monomonial other) {
            if(other.unknowns_number() != unknowns_number()) return false;
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
                if(a_unknowns[i].name() == "") {continue;} bool contains_equal = false;
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
        // Deletes the monomonial unkwnown
        void delete_unknown(std::string unknown_name) {
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
                if(a_unknowns[i].name() == unknown_name) {a_unknowns.erase(a_unknowns.begin() + i); return;}
            }
        };
        inline void delete_unknown(_Base_Unknown* unknown_to_delete) {delete_unknown(unknown_to_delete->name());};
        // Returns if the monomonial is known
        inline bool is_known() const {return a_unknowns.size() <= 0 || (a_unknowns.size() == 1 && (a_unknowns.at(0).name() == "" || a_unknowns.at(0).exponent() == 0));};
        // Returns if the monomonial only contains an unkwnown
        _Base_Unknown* only_contains_unknown(std::string unknown_name, Complex exponent) {
            _Base_Unknown* to_return = 0; unsigned int number = unknowns_number();
            for(int i = 0;i<static_cast<int>(a_unknowns.size());i++) {
                if((a_unknowns[i].name() == unknown_name && a_unknowns[i].exponent() == exponent)) {
                    to_return = &a_unknowns[i];
                } else if(!(number > 0 && a_unknowns[i].name() == "")) {to_return = 0; break;}
            } return to_return;
        };
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
            if(a_factor.real() == 0 || a_factor.imaginary() == 0) return a_factor.to_std_string_simple() + final_unknow;
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
                else if(obj.a_unknowns[i].name() != "") {
                    scls::Complex new_exponent = contained_unknown->exponent() + obj.a_unknowns[i].exponent();
                    if(new_exponent == 0) {
                        delete_unknown(contained_unknown);
                    } else {contained_unknown->set_exponent(new_exponent);}
                }
            }
            return *this;
        }
        // Convert the monomonial to a complex
        operator Complex() const {return a_factor;};

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
        Polymonial(int number){a_monomonials.push_back(Monomonial(Complex(number)));};
        Polymonial(Monomonial monomonial){a_monomonials.push_back(monomonial);};
        // Polymonial copy constructor
        Polymonial(const Polymonial& polymonial_copy):a_monomonials(polymonial_copy.a_monomonials){};

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
        // Returns if the polymonial is known
        inline bool is_known() const {
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                if(!a_monomonials[i].is_known() && a_monomonials[i].factor() != 0) return false;
            } return true;
        };
        // Returns the knows monomonial
        Monomonial known_monomonial() const {
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                if(a_monomonials.at(i).unknowns_number() <= 0) return a_monomonials.at(i);
            }
            return Monomonial(Complex(0));
        };
        // Returns a monomonial by its unknown
        inline Monomonial monomonial(std::string unknown, Complex exponent) {
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                _Base_Unknown* result = a_monomonials.at(i).only_contains_unknown(unknown, exponent);
                if(result != 0) {
                    return a_monomonials.at(i);
                }
            } Monomonial to_return(0); to_return.add_unknown(unknown, exponent); return to_return;
        };
        inline Monomonial monomonial(std::string unknown) {return monomonial(unknown, Complex(1, 0));};
        // Returns the number of monomonials in the polymonial
        inline int monomonials_number() const {return a_monomonials.size();};
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
        // Divide a monomonial to this void
        void __divide(Monomonial value) {
            // Apply each division
            Monomonial used_inverse = value.inverse();
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                a_monomonials[i] *= used_inverse;
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
        void __multiply(Fraction value) {Polymonial temp;temp.add_monomonial(Monomonial(value));__multiply(temp);};

        // Returns the maximum degree in the polymonial
        inline Complex degree(std::string unknown_name) {
            Complex max_degree = 0;
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                if(a_monomonials[i].contains_unknown(unknown_name) && a_monomonials[i].unknowns_number() == 1) {
                    Complex current_factor = a_monomonials[i].unknowns()[0].exponent();
                    if(current_factor.real() > max_degree.real()) max_degree = current_factor;
                }
            } return max_degree;
        };
        // Returns a polymonial from a monomonial where the unknows has been replaced
        static Polymonial polymonial_from_modified_monomonial_unknows(Monomonial used_monomonial, std::string unknown, Polymonial new_value) {
            Polymonial final_polymonial = Polymonial();

            _Base_Unknown* unknown_ptr = used_monomonial.contains_unknown(unknown);
            if(unknown_ptr!= 0) {
                // The monomonial is a multiplication between the unknown and the base value
                Complex exponent = unknown_ptr->exponent();
                used_monomonial.delete_unknown(unknown);
                final_polymonial += used_monomonial;
                // Create the good value
                Polymonial final_to_add = new_value;
                int needed_exponent = static_cast<int>(exponent.real().to_double());
                for(int i = 1;i<needed_exponent;i++) {final_to_add *= new_value;}
                final_polymonial *= final_to_add;
            } else {
                // The unknown is not a multiplication of the base value
                final_polymonial += used_monomonial;
            }

            return final_polymonial;
        };
        // Returns a monomonial where an unkown is replaced by an another unknown
        Polymonial replace_unknown(std::string unknown, Polymonial new_value) const {
            Polymonial final_polymonial = Polymonial();

            // Analyse each monomonial
            for(int i = 0;i<static_cast<int>(a_monomonials.size());i++) {
                Polymonial current_polymonial = polymonial_from_modified_monomonial_unknows(a_monomonials.at(i), unknown, new_value);
                final_polymonial += current_polymonial;
            }

            return final_polymonial;
        };
        // Simplify the polymonial
        Polymonial simplify() const {
            Polymonial poly(*this);
            for(int i = 0;i<static_cast<int>(poly.a_monomonials.size());i++) {
                if(poly.a_monomonials[i].factor() == 0) {
                    poly.a_monomonials.erase(poly.a_monomonials.begin() + i);
                    i--;
                }
            } return poly;
        };

        // Operators
        // With Complex
        bool operator!=(Complex value) const {return !(*this == value);};
        bool operator==(Complex value) const {
            if(value.real() == 0 && value.imaginary() == 0) {
                return (a_monomonials.size() == 0) || (a_monomonials.size() == 1 && a_monomonials.at(0).factor().real() == 0 && a_monomonials.at(0).factor().imaginary() == 0);
            } return (a_monomonials.size() == 1 && a_monomonials.at(0).factor().real() == value);
        };
        // With fractions
        Polymonial& operator*=(Fraction value) {__multiply(value);return*this;};
        // With polymonials
        Polymonial operator-(Polymonial value) const {Polymonial to_return(*this);to_return-=value;return to_return;};
        Polymonial& operator-=(Polymonial value) {value*=Fraction(-1);__add(value);return*this;};
        Polymonial& operator+=(Polymonial value) {__add(value);return*this;};
        Polymonial& operator*=(Polymonial value) {__multiply(value);return*this;};
        // With monomonials
        Polymonial& operator+=(Monomonial value) {add_monomonial(value);return*this;};
        Polymonial& operator/=(Monomonial value) {__divide(value);return*this;};

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

    //*********
	//
	// The "Formula" class
	//
	//*********

	class __Formula_Base {
        // Class representating the base of a mathematical formula
    public:

        // __Formula_Base constructor
        __Formula_Base(){};
        __Formula_Base(int number):a_polymonial_add(number){};
        __Formula_Base(Fraction fraction):__Formula_Base(Monomonial(scls::Complex(fraction))){};
        __Formula_Base(Monomonial monomonial):a_polymonial_add(monomonial){};
        __Formula_Base(Polymonial polymonial):a_polymonial_add(polymonial){};
        // __Formula_Base copy constructor
        __Formula_Base(const __Formula_Base& formula):a_formulas_add(formula.a_formulas_add),a_formulas_factor(formula.a_formulas_factor),a_polymonial_add(formula.a_polymonial_add),a_polymonial_factor(formula.a_polymonial_factor),a_applied_function(formula.a_applied_function){
            if(formula.a_denominator.get() != 0) {
                a_denominator = std::make_shared<__Formula_Base>(*formula.a_denominator.get());
            }
        };

        // Clear the formula
        inline void clear() {a_formulas_add.clear();a_formulas_factor.clear();a_polymonial_add=0;a_polymonial_factor=1;};
        // Returns the polymonial to std::string
        inline std::string to_std_string() const {
            std::string current_str = "";
            std::string to_return = "";
            // Add the polymonial add
            if(a_polymonial_add != 0) {
                current_str = a_polymonial_add.to_std_string();
                if(current_str != "") {to_return += "(" + current_str + ") + ";}
            }
            // Add the polymonial factor
            if(static_cast<int>(a_formulas_factor.size()) > 0 && a_polymonial_factor != 0 && a_polymonial_factor != 1) {
                current_str = a_polymonial_factor.to_std_string();
                if(current_str != "") {to_return += "(" + current_str + ") * ";}
            }

            // Add the factors formulas
            for(int i = 0;i<static_cast<int>(a_formulas_factor.size());i++) {
                current_str = a_formulas_factor.at(i).to_std_string();
                if(current_str != "") {
                    to_return += "(" + current_str + ")";
                    if(i < static_cast<int>(a_formulas_factor.size() - 1)) {to_return += " * ";}
                }
            }
            while(to_return[to_return.size() - 1] == '+' || to_return[to_return.size() - 1] == '*' || to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
            to_return += " + ";

            // Add the add formulas
            for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++) {
                to_return += "(" + a_formulas_add.at(i).to_std_string() + ") + ";
            }
            // Format the text
            while(to_return[0] == '+' || to_return[0] == '*' || to_return[0] == ' ') to_return = to_return.substr(1, to_return.size() - 1);
            while(to_return[to_return.size() - 1] == '+' || to_return[to_return.size() - 1] == '*' || to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
            // Apply the function
            if(applied_function() != "") to_return = applied_function() + "(" + to_return + ")";

            // Add the denominator if needed
            if(a_denominator.get() != 0) {
                to_return = "(" + to_return + ")/(" + a_denominator.get()->to_std_string() + ")";
            }

            return to_return;
        };

        // Returns the internal value of the formula (without functions)
        __Formula_Base internal_value() const {
            __Formula_Base to_return = *this;
            to_return.set_applied_function("");
            return to_return;
        };
        // Returns if the formula is a basic formula or not
        bool is_basic() const {return a_applied_function == "" && a_denominator.get() == 0;};
        // Returns if the formula is a simple monomonial / polymonial or not
        bool is_simple_monomonial() const {return a_formulas_add.size() <= 0 && (a_formulas_factor.size() <= 0 || a_polymonial_factor == 0) && a_polymonial_add.monomonials_number() <= 0;};
        bool is_simple_polymonial() const {return a_formulas_add.size() <= 0 && (a_formulas_factor.size() <= 0 || a_polymonial_factor == 0);};

        // Returns a formula from a monomonial where the unknows has been replaced
        static __Formula_Base formula_from_modified_monomonial_unknows(Monomonial used_monomonial, std::string unknown, __Formula_Base new_value) {
            __Formula_Base final_formula = __Formula_Base();

            _Base_Unknown* unknown_ptr = used_monomonial.contains_unknown(unknown);
            if(unknown_ptr!= 0) {
                // The monomonial is a multiplication between the unknown and the base value
                Complex exponent = unknown_ptr->exponent();
                used_monomonial.delete_unknown(unknown);
                final_formula += used_monomonial;
                // Create the good value
                __Formula_Base final_to_add = new_value;
                int needed_exponent = static_cast<int>(exponent.real().to_double());
                for(int i = 1;i<needed_exponent;i++) {final_to_add *= new_value;}
                final_formula *= final_to_add;
            } else {
                // The unknown is not in the polymonial value
                final_formula = used_monomonial;
            }

            return final_formula;
        };
        // Returns a formula from a polymonial where the unknows has been replaced
        static __Formula_Base formula_from_modified_polymonial_unknows(Polymonial used_polymonial, std::string unknown, __Formula_Base new_value) {
            __Formula_Base final_formula = __Formula_Base();

            // Browse each polymonials
            std::vector<Monomonial>& monomonials = used_polymonial.monomonials();
            for(int i = 0;i<static_cast<int>(monomonials.size());i++) {
                final_formula += formula_from_modified_monomonial_unknows(monomonials[i], unknown, new_value);
            }

            return final_formula;
        };
        // Returns a monomonial where an unkown is replaced by an another unknown
        __Formula_Base replace_unknown(std::string unknown, __Formula_Base new_value) const {
            __Formula_Base current_formula;
            __Formula_Base final_formula = __Formula_Base(1);
            final_formula.set_applied_function(applied_function());

            // Add the factor polymonial
            current_formula = formula_from_modified_polymonial_unknows(a_polymonial_factor, unknown, new_value);
            final_formula *= current_formula;
            // Add the factors formulas
            for(int i = 0;i<static_cast<int>(a_formulas_factor.size());i++) {
                final_formula *= a_formulas_factor[i].replace_unknown(unknown, new_value);
            }
            // Add the added polymonial
            if(final_formula == 1) {std::string used_function = applied_function(); final_formula = 0; final_formula.set_applied_function(used_function);}
            final_formula += formula_from_modified_polymonial_unknows(a_polymonial_add, unknown, new_value);
            // Add the added formulas
            for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++) {
                final_formula += a_formulas_add[i].replace_unknown(unknown, new_value);
            }

            return final_formula;
        };

        // Methods operators
        // Add a formula to this one
        void __add(Monomonial value) {a_polymonial_add += value;};
        void __add(Polymonial value) {a_polymonial_add += value;};
        void __add(__Formula_Base value) {
            if(value == 0) {return;}

            // Check if this formula is basic or not
            if(!is_basic()) {
                __Formula_Base temp(*this);
                clear(); a_applied_function = ""; a_denominator.reset();
                a_formulas_add.push_back(temp);
            }

            if(value.is_basic()) {
                // Added formulas
                __add(value.a_polymonial_add);
                int formula_size = static_cast<int>(value.a_formulas_add.size());
                for(int i = 0;i<formula_size;i++) {
                    a_formulas_add.push_back(value.a_formulas_add[0]);
                    value.a_formulas_add.erase(value.a_formulas_add.begin());
                }
                // Factors
                if(static_cast<int>(value.a_formulas_factor.size()) > 0) {a_formulas_add.push_back(value);}
            } else {a_formulas_add.push_back(value);}
        };
        // Divide a formula to this one
        void __divide(__Formula_Base value) {
            if(!value.is_basic()) {
                a_denominator=std::make_shared<__Formula_Base>(value);
            } else {
                if(value.is_simple_monomonial()) {
                    // Apply a division of a simple monomonial
                    Monomonial used_monomonial = value.a_polymonial_add.monomonials()[0];
                    a_polymonial_add /= used_monomonial;
                    a_polymonial_factor /= used_monomonial;
                    for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++) {
                        a_formulas_add[i] /= used_monomonial;
                    }
                } else {
                    a_denominator=std::make_shared<__Formula_Base>(value);
                }
            }
        };
        // Multiply a polymonial to this one
        void __multiply(Polymonial value) {a_polymonial_add *= value;a_polymonial_factor *= value;for(int i=0;i<static_cast<int>(a_formulas_add.size());i++)a_formulas_add[i]*=value;};
        void __multiply(__Formula_Base value) {
            if(value.applied_function() == "") {
                // The formula can be directly multiplied
                __Formula_Base first_formula = internal_value();
                __Formula_Base second_formula = internal_value();
                std::vector<__Formula_Base> other_formulas;
                first_formula.__multiply(value.a_polymonial_add);
                if(value.a_formulas_factor.size() > 0) {
                    second_formula.__multiply(value.a_polymonial_factor);
                    for(int i = 0;i<static_cast<int>(value.a_formulas_factor.size());i++){second_formula.__multiply(value.a_formulas_factor.at(i));}
                } else {second_formula.clear();}
                for(int i = 0;i<static_cast<int>(value.a_formulas_add.size());i++) {
                    __Formula_Base current_formula = internal_value();
                    current_formula *= value.a_formulas_add.at(i);
                    other_formulas.push_back(current_formula);
                }

                // Add the needed formulas
                clear();
                __add(first_formula);
                __add(second_formula);
                for(int i = 0;i<static_cast<int>(other_formulas.size());i++) {__add(other_formulas[i]);}
            } else {
                // The formula can't be directly multiplied
                // Update the formulas
                for(int i = 0;i<static_cast<int>(a_formulas_add.size());i++){value.a_formulas_add[i].__multiply(value);}
                // Add the factor
                if(a_formulas_factor.size() > 0) a_formulas_factor.push_back(value);
                // Create a new formula from the add polymonial
                if(a_polymonial_add != 0) {
                    if(a_formulas_factor.size() > 0) {
                        __Formula_Base new_formula = value;
                        new_formula *= a_polymonial_add;
                        a_formulas_add.push_back(new_formula);
                        a_polymonial_add = 0;
                    } else {
                        a_polymonial_factor = a_polymonial_add;
                        a_formulas_factor.push_back(value);
                        a_polymonial_add = 0;
                    }
                }
            }
        };
        void __multiply(Fraction value) {Polymonial temp;temp.add_monomonial(Monomonial(value));__multiply(temp);};

        // Operators
        // With int
        bool operator==(int value) {return a_formulas_add.size() <= 0 && (a_formulas_factor.size() <= 0 || a_polymonial_factor == 0) && a_polymonial_add == value;};
        // With fractions
        __Formula_Base& operator*=(Fraction value) {__multiply(value);return*this;};
        bool operator==(Fraction value) {return a_formulas_add.size() <= 0 && (a_formulas_factor.size() <= 0 || a_polymonial_factor == 0) && a_polymonial_add == value;};
        // With formulas
        __Formula_Base operator-(__Formula_Base value) const {__Formula_Base to_return(*this);to_return-=value;return to_return;};
        __Formula_Base& operator-=(__Formula_Base value) {value*=Fraction(-1);__add(value);return*this;};
        __Formula_Base& operator+=(__Formula_Base value) {__add(value);return*this;};
        __Formula_Base& operator*=(__Formula_Base value) {__multiply(value);return*this;};
        __Formula_Base operator/(__Formula_Base value) const {__Formula_Base other(*this);other.__divide(value);return other;};
        __Formula_Base& operator/=(__Formula_Base value) {__divide(value);return*this;};
        // Converts the formula to a polymonial
        inline Polymonial to_polymonial() const {return a_polymonial_add.simplify();};
        operator Polymonial() const {return to_polymonial();};

        // Getters and setters
        inline std::string applied_function() const {return a_applied_function;};
        inline void set_applied_function(std::string new_applied_function) {a_applied_function = new_applied_function;};

    private:
        // Attached add polymonials
        std::vector<__Formula_Base> a_formulas_add;
        // Attached factors polymonials
        std::vector<__Formula_Base> a_formulas_factor;
        // Attached add polymonial
        Polymonial a_polymonial_add;
        // Attached factor polymonial
        Polymonial a_polymonial_factor = 1;

        // Applied function to the formula
        std::string a_applied_function = "";

        // Division of the formula
        std::shared_ptr<__Formula_Base> a_denominator;
        // Exponent of the formula
        std::shared_ptr<__Formula_Base> a_exponent;
    }; typedef __Formula_Base Formula;

    //*********
	//
	// The sets class
	//
	//*********

	class Interval {
	    // Class representating an interval of real numbers
    public:

        // Interval constructor
        Interval(Fraction start, Fraction end):a_end(end),a_start(start){};
        Interval():Interval(Fraction(0), Fraction(0)){};

        // Getters and setters
        inline Fraction end() const {return a_end;};
        inline bool end_infinite() const {return a_end_infinite;};
        inline void set_end(Fraction new_end) {a_end=new_end;};
        inline void set_start(Fraction new_start) {a_start=new_start;};
        inline Fraction start() const {return a_start;};
        inline void set_end_infinite(bool new_end_infinite) {a_end_infinite=new_end_infinite;};
        inline void set_start_infinite(bool new_start_infinite) {a_start_infinite=new_start_infinite;};
        inline bool start_infinite() const {return a_start_infinite;};

    private:
        // End of the interval
        Fraction a_end;
        // If the end is + infinite or not
        bool a_end_infinite = false;
        // Start of the interval
        Fraction a_start;
        // If the start is + infinite or not
        bool a_start_infinite = false;
	};

	// Comparaison function for the interval sorting
	static bool __sort_interval_function(const Interval& i_1, const Interval& i_2) {return i_1.start() > i_2.start();};
	// Comparaison function for the numbers sorting
	static bool __sort_numbers_function(const Complex& i_1, const Complex& i_2) {return i_1.real() < i_2.real();};
	class Set_Number {
        // Class representating a set of numbers
    public:

        // Set_Number constructor
        Set_Number(){};
        // Set_Number constructor convertors
        Set_Number(Interval interval){a_intervals.push_back(interval);};

        // Add an interval to the set
        inline void add_interval(Interval to_add) {
            a_intervals.push_back(to_add);
            __sort_interval();
        };
        // Add a number to the set
        inline void add_number(Complex to_add) {
            a_numbers.push_back(to_add);
            __sort_numbers();
        };
        void add_number(Fraction to_add) {add_number(Complex(to_add));};
        // Returns if the set is empty or not
        inline bool is_empty() const {return a_intervals.size()<=0;};
        // Returns if the set is infinite or not
        inline bool is_infinite() const {return a_intervals.size() > 0 && a_intervals.at(0).start_infinite() && a_intervals.at(0).end_infinite();};
        // Sort the intervals in the set
        inline void __sort_interval() {std::sort(a_intervals.begin(), a_intervals.end(), __sort_interval_function);};
        // Sort the numbers in the set
        inline void __sort_numbers() {std::sort(a_numbers.begin(), a_numbers.end(), __sort_numbers_function);};

        // Getters and setters
        const std::vector<Interval>& intervals() const {return a_intervals;};
        const std::vector<Complex>& numbers() const {return a_numbers;};

    private:
        // Intervals of fractions in this set
        std::vector<Interval> a_intervals;
        // Numbers of fractions in this set
        std::vector<Complex> a_numbers;
	};

    //*********
	//
	// The parsers class
	//
	//*********

	class String_To_Formula_Parse {
        // Class allowing to properly parse a std::string to a polymonial
    public:
        // String_To_Formula_Parse constructor
        String_To_Formula_Parse(unsigned int level):a_level(level){};
        String_To_Formula_Parse():String_To_Formula_Parse(0){};

        // Returns if a std::string is a number or not
        inline bool __string_is_number(char text) const {return (string_is_number(text) || text == '/' || text == 'i' || text == '-');};
        inline bool __string_is_number(std::string text) const {
            for(int i = 0;i<static_cast<int>(text.size());i++) {
                if(!__string_is_number(text[i])) return false;
            } return true;
        };
        // Returns if a std::string is an operator or not
        inline bool __string_is_operator(char text) const {return (text == '+' || text == '-' || text == '*' || text == '/' || text == '>');};
        // Returns a given first base string to a formula
        Formula __string_to_formula_base(std::string base, std::string used_function = "") {
            Formula formula;

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
                Polymonial current_polymonial;
                current_polymonial.add_monomonial(to_add);
                formula = current_polymonial;
            } else {
                // Parenthesis form
                String_To_Formula_Parse new_parser(level() + 1);
                formula = new_parser.string_to_formula(base.substr(1, base.size() - 2));
            }

            // Create the formula
            formula.set_applied_function(used_function);
            return formula;
        };

        // Converts a std::string to a Formula
        Formula __string_to_formula_without_division(std::string source) {
            // Format the text as needed
            std::vector<std::string> cutted;

            // Prepare the needed datas
            Formula to_return;

            // Cut the text operator by > operator
            cutted = cut_string_out_of_2(source, ">", "(", ")");
            if(cutted.size() > 1) {
                // At least one function applied
                for(int i = 1;i<static_cast<int>(cutted.size());i+=2) {
                    Formula current_polymonial = __string_to_formula_base(cutted[i], cutted[i - 1]);
                    to_return += current_polymonial;
                }
            } else {
                // No function applied
                to_return = __string_to_formula_base(cutted[0]);
            }

            // Return the result
            return to_return;
        };
        Formula __string_to_formula_without_multiplication(std::string source) {
            // Format the text as needed
            std::vector<std::string> cutted;

            // Prepare the needed datas
            Formula to_return; bool to_return_modified = false;

            // Cut the text operator by / operator
            cutted = cut_string_out_of_2(source, "/", "(", ")");
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                Formula current_polymonial = __string_to_formula_without_division(cutted[i]);
                if(to_return_modified) {to_return /= current_polymonial;}
                else {to_return += current_polymonial;to_return_modified=true;}
            }

            // Return the result
            return to_return;
        };
        Formula __string_to_formula_without_addition(std::string source) {
            // Format the text as needed
            std::vector<std::string> cutted;

            // Prepare the needed datas
            Formula to_return; bool to_return_modified = false;

            // Cut the text operator by * operator
            cutted = cut_string_out_of_2(source, "*", "(", ")");
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                Formula current_polymonial = __string_to_formula_without_multiplication(cutted[i]);
                std::cout << "T " << level_indentation() << source << " " << cutted[i] << " " << current_polymonial.to_std_string() << std::endl;
                if(to_return_modified) {to_return *= current_polymonial;}
                else {to_return += current_polymonial;to_return_modified=true;}
            }

            // Return the result
            return to_return;
        };
        Formula string_to_formula(std::string source) {
            // Format the text as needed
            source = remove_space(source);
            for(int i = 0;i<static_cast<int>(source.size());i++) {
                // Handle functions
                if(i > 0 && contains_function(source[i])) {
                    if(!__string_is_operator(source[i - 1]) && source[i - 1] != '(') {
                        // The part is a simple variable
                        source.insert(i, "*");
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
                        if(contains_function(source[i - 1])) {
                            // The part is a function
                            source.insert(i, ">");
                            i++;
                        } else {
                            // The part is a simple variable
                            source.insert(i, "*");
                            i++;
                        }
                    }
                }
            }
            // Handle the "-"
            for(int i = 0;i<static_cast<int>(source.size());i++) {
                // Remove the useless "-"
                if(i > 0 && static_cast<int>(source[i]) == static_cast<int>('-')) {
                    if(!__string_is_operator(source[i - 1]) && (i >= static_cast<int>(source.size()) || (static_cast<int>(source[i + 1]) != static_cast<int>('(')))) {
                        source.insert(i, "+");
                        i++;
                    }
                }
            }

            // Prepare the needed datas
            std::vector<std::string> cutted;
            Formula to_return;

            // Cut the text operator by + operator
            cutted = cut_string_out_of_2(source, "+", "(", ")");
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                Formula current_polymonial = __string_to_formula_without_addition(cutted[i]);
                to_return += current_polymonial;
            }

            // Return the result
            return to_return;
        };

        // Add a function to the defined functions
        inline void add_function(std::string function_name) {if(!contains_function(function_name)) a_functions.push_back(function_name); };
        // Returns if a function is defined or not
        inline bool contains_function(std::string function_name) const {for(int i = 0;i<static_cast<int>(a_functions.size());i++) {if(a_functions.at(i) == function_name) return true; } return false;};
        inline bool contains_function(char function_name) const {std::string str;str+=function_name;return contains_function(str);};
        // Returns the indentation for this level
        inline std::string level_indentation() const {std::string to_return = "";for(int i = 0;i<static_cast<int>(level()*4);i++)to_return+=" ";return to_return;};

        // Getters and setters
        inline std::vector<std::string>& functions() {return a_functions;};
        inline unsigned int level() const {return a_level;};
    private:

        // Every defined functions in the parser
        std::vector<std::string> a_functions;
        // Level of the parser
        unsigned int a_level = 0;
    };

    // Use parsers methods outside the class
    static __Formula_Base string_to_formula(std::string source){String_To_Formula_Parse parser;return parser.string_to_formula(source);};
    static __Formula_Base replace_unknown(__Formula_Base used_formula, std::string unknown, std::string new_value) { return used_formula.replace_unknown(unknown, string_to_formula(new_value));};

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
