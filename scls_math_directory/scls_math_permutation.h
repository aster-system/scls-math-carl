//******************
//
// scls_math_permutation.h
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
// This file contains some tools to use permutations in C++.
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

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The "Permutation" class
	//
	//*********

	class Permutation {
    public:
        // Permutation constructor
        Permutation();
        Permutation(std::vector<int> needed_permutations);
        // Permutation destructor
        virtual ~Permutation();

        // Returns the needed identity permutation
        std::vector<int> identity_permutations();

        // Returns the permutation in an std::string
        std::string to_std_string();

        // Transposition decomposition
        std::vector<Permutation> transposition_decomposition();

    private:
        // List of the start - end index permutations
        std::vector<int> a_permutations;
    };
}
