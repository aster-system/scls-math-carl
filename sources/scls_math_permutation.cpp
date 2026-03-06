//******************
//
// scls_math_permutation.cpp
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
// This file contains the source code of "scls_math_permutation.h".
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

#include "../scls_math.h"
#include "../scls_math_directory/scls_math_permutation.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The "Permutation" class
	//
	//*********

	// Permutation constructor
    Permutation::Permutation(){};
    Permutation::Permutation(std::vector<int> needed_permutations):a_permutations(needed_permutations){};
    // Permutation destructor
    Permutation::~Permutation(){};

    // Returns the needed identity permutation
    std::vector<int> Permutation::identity_permutations(){std::vector<int>a=std::vector<int>(a_permutations.size());for(std::size_t i=0;i<a_permutations.size();i++){a[i]=i;}return a;};

    // Returns the permutation in an std::string
    std::string Permutation::to_std_string() {
        std::string to_return = std::string();

        for(std::size_t i = 0;i<a_permutations.size();i++) {
            to_return += std::to_string(a_permutations.at(i));
            if(i != a_permutations.size() - 1){
                to_return += std::string_view(" - ");
            }
        }

        return to_return;
    }

    // Transposition decomposition
    std::vector<Permutation> Permutation::transposition_decomposition() {
        // Datas
        std::vector<Permutation> to_return;

        // Random shuffle
        std::vector<int> a = a_permutations;

        // Re-arrangment
        for(std::size_t i = 0;i<a.size();i++) {
            if(static_cast<std::size_t>(a.at(i)) != i) {
                // Get the object
                int needed_index = 0;
                for(std::size_t j = i + 1;j<a.size();j++) {
                    if(static_cast<std::size_t>(a.at(j)) == i) {
                        needed_index = j;break;
                    }
                }

                // Add the transposition
                std::vector<int> current = identity_permutations();
                current[i] = needed_index;
                current[needed_index] = i;
                to_return.push_back(current);
                a[needed_index] = a.at(i);
                a[i] = i;
            }
        }

        return to_return;
    }
}
