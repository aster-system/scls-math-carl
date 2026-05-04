//******************
//
// scls_math_algebra_vector.h
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
// This file contains some tools to use algebra vector in SCLS.
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

// The namespace "scls" is used to simplify the all.
namespace scls {
    /*using namespace scls;
    class Vector : public Algebra_Element {
        // Class representing a Vector
    public:
        // Vector constructor
        Vector(){};
        virtual ~Vector(){};

        // Canonical decomposition
        virtual std::vector<std::shared_ptr<Algebra_Element>> canonical_decomposition() = 0;

        // Copy the vector
        virtual std::shared_ptr<Vector> new_vector_element(){std::shared_ptr<Vector> to_return = std::make_shared<Vector>();return to_return;};
        virtual std::shared_ptr<Vector> vector_clone(){std::shared_ptr<Vector> to_return = std::make_shared<Vector>();return to_return;};
    };
    class N_Uplet : public Vector {
        // Class representing an n-uplet
    public:
        // N_Uplet constructor
        N_Uplet(int length):a_length(length),a_elements(std::vector<std::shared_ptr<Algebra_Element>>(length)){};
        virtual ~N_Uplet(){};

        // Herited methods
        virtual std::shared_ptr<Vector> new_vector_element(){std::shared_ptr<N_Uplet> to_return = std::make_shared<N_Uplet>(a_length);return to_return;};

        // Getters and setters
        inline Algebra_Element* element_at(int i){return a_elements.at(i).get();};
        inline int length() const {return a_length;};
    private:
        // Length of the n-uplet
        int a_length = 0;

        // Elements in the n-uplet
        std::vector<std::shared_ptr<Algebra_Element>> a_elements;
    };
    class Vector_Base {
    public:
        // Plane_Base constructor
        Vector_Base();
        Vector_Base();

        // Add a vector
        void add_vector(std::shared_ptr<Vector> vector){a_vectors.push_back(vector);}

        // Vector with those coordinates in the base
        std::shared_ptr<Vector> vector_from_coordinates(N_Uplet* v){
            // Asserts
            if(a_vectors.size() == 0){return std::shared_ptr<Vector>();}

            // Create element
            std::shared_ptr<Vector> to_return = a_vectors.at(0).get()->new_vector_element();
            for(int i = 0;i<v->length();i++) {
                std::shared_ptr<Vector> w = a_vectors.at(i).get()->vector_clone();
                w.get()->operate(v->element_at(i), "*");
                to_return.get()->operate(w.get(), "+");
            }

            return to_return;
        };

        // Coordinates of a vector in the base
        std::shared_ptr<N_Uplet> coordinates_in_base(Vector* v){

        };

    private:
        // Vectors
        std::vector<std::shared_ptr<Vector>> a_vectors;
    };//*/
}
