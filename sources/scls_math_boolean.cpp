//******************
//
// scls_math_boolean.cpp
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
// This file contains the source code of scls_math_boolean.h.
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

// Include the good header file
#include "../scls_math_directory/scls_math_boolean.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
	//
	// The "Boolean" class
	//
	//*********

	// Available operators for this object
	std::vector<Algebra_Element::Algebra_Operator> formula_operators = {scls::Algebra_Element::Algebra_Operator("."), scls::Algebra_Element::Algebra_Operator("+")};
	const std::vector<Algebra_Element::Algebra_Operator>& Boolean::operators(){return formula_operators;}

    // Replaces the unknowns
	void Boolean::replace_unknowns_algebra(Algebra_Element* element, Unknowns_Container* values) const {
		// The element is final
		if(is_final_element()) {
			if(is_unknown()){
				__Boolean_Unknown* current = reinterpret_cast<__Boolean_Unknown*>(values->unknown_by_name(algebra_unknown()->name));
				if(current == 0){algebra_clone(element);}
				else{reinterpret_cast<Boolean*>(element)->a_value = current->value;}
			}
			else {reinterpret_cast<Boolean*>(element)->a_value = a_value;}
		}
		else {Algebra_Element::replace_unknowns_algebra(element, values); }
	}
    std::shared_ptr<Boolean> Boolean::replace_unknowns(Unknowns_Container* values) const {
        // Create the object
        std::shared_ptr<Boolean> s = std::make_shared<Boolean>();
        s.get()->a_this_object=s;replace_unknowns_algebra(s.get(), values);

        return s;
    }

    // Get each unknowns
    std::vector<std::string> Boolean::unknowns() {
        // Is an unknown
        if(is_unknown()){return std::vector<std::string>(1, algebra_unknown()->name);}

        // Get each children
        std::vector<std::string> to_return;
        for(int i = 0;i<static_cast<int>(algebra_elements_const().size());i++) {
            std::vector<std::string> result = reinterpret_cast<Boolean*>(algebra_elements_const().at(i).get())->unknowns();
            for(int j = 0;j<static_cast<int>(result.size());j++) {
                bool good = true;
                for(int k = 0;k<static_cast<int>(to_return.size());k++) {
                    if(to_return.at(k) == result.at(j)){good = false;break;}
                }
                if(good){to_return.push_back(result.at(j));}
            }
        }

        return to_return;
    }

    // Returns the verity table
    std::string Boolean::verity_table() {
        // Get each unknowns
        std::vector<std::string> needed_unknowns = unknowns();
        std::vector<char> unknowns_value = std::vector<char>(needed_unknowns.size());
        std::sort(needed_unknowns.begin(), needed_unknowns.end());

        // Get the start and the diff
        std::string to_return = std::string();
        for(int i = 0;i<static_cast<int>(needed_unknowns.size());i++){to_return += needed_unknowns.at(i) + std::string(" | ");}
        std::string last_part = to_std_string(0);to_return += last_part;
        std::string diff = std::string();
        for(int i = 0;i<static_cast<int>(to_return.size());i++){diff += std::string("-");}
        to_return += std::string("\n") + diff + std::string("\n");

        // Get the values
        int unknowns_number = std::pow(2, needed_unknowns.size());
        for(int i = 0;i<unknowns_number;i++) {
            Unknowns_Container a = Unknowns_Container();
            for(int j = 0;j<static_cast<int>(needed_unknowns.size());j++){bool result = (static_cast<int>(floor(static_cast<double>(i) / pow(2,needed_unknowns.size() - (1 + j)))) % 2) == 1;a.create_unknown<Boolean_Unknown>(needed_unknowns.at(j))->value = result;unknowns_value[j] = result;}
            for(int j = 0;j<static_cast<int>(needed_unknowns.size());j++){to_return += std::to_string(static_cast<int>(unknowns_value.at(j))) + std::string(" | ");}
            for(int j = 0;j<static_cast<int>(last_part.size())/2;j++){to_return += std::string(" ");}
            to_return += replace_unknowns(&a).get()->to_std_string(0) + std::string("\n") + diff + std::string("\n");
        }

        return to_return;
    }
}
