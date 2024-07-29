//******************
//
// scls_math_3d_core.h
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
// This file contains the core of the use of 3D features.
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

#ifndef SCLS_MATH_3D_CORE
#define SCLS_MATH_3D_CORE

// Define each attributes values
#define SCLS_3D_POINT_ATTRIBUTES_TEXTURE_MULTIPLIER_X 0
#define SCLS_3D_POINT_ATTRIBUTES_TEXTURE_MULTIPLIER_Y 1
#define SCLS_3D_POINT_ATTRIBUTES_TEXTURE_MULTIPLIER_Z 2
#define SCLS_3D_POINT_ATTRIBUTES_TEXTURE_X 3
#define SCLS_3D_POINT_ATTRIBUTES_TEXTURE_Y 4
#define SCLS_3D_POINT_ATTRIBUTES_GRID_X 0
#define SCLS_3D_POINT_ATTRIBUTES_GRID_Z 1
#define SCLS_3D_SOLID_MAP_CHUNK_X 0
#define SCLS_3D_SOLID_MAP_CHUNK_Z 1

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
    //
    // The Point_3D class
    //
    //*********

    // Convert degrees to radians
    inline double degrees_to_radians(double degrees) {return (degrees / 180.0) * SCLS_PI;};

    class Point_3D {
        // Class representing a 3D point
    public:

        //*********
        //
        // Main Point_3D members
        //
        //*********

        // Simplest Point_3D constructor
        Point_3D(){};

        //*********
        //
        // Point_3D position system
        //
        //*********

        // Move easily the object
        inline void move_xyz(double movement_x, double movement_y, double movement_z) {move_x(movement_x);move_y(movement_y);move_z(movement_z);};
        inline void move_x(double movement) {set_x(x() + movement);};
        inline void move_y(double movement) {set_y(y() + movement);};
        inline void move_z(double movement) {set_z(z() + movement);};

        // Getters
        virtual void set_x(double new_x) {a_x = new_x;};
        virtual void set_y(double new_y) {a_y = new_y;};
        virtual void set_z(double new_z) {a_z = new_z;};
        inline double x() const {return a_x;};
        inline double y() const {return a_y;};
        inline double z() const {return a_z;};

    private:
        //*********
        //
        // Point_3D position system
        //
        //*********

        // X position of the point
        double a_x = 0;
        // Y position of the point
        double a_y = 0;
        // Z position of the point
        double a_z = 0;
    };

    //*********
    //
    // The Transform_Object_3D class
    //
    //*********

    // Normalizes a 3D vector
    static void normalize_3d(double& vector_x, double& vector_y, double& vector_z) {
        double distance_xz = vector_x * vector_x + vector_z * vector_z;
        double distance = std::sqrt(distance_xz + vector_y * vector_y);

        // Calculate the proportionality constant
        double divisor = 1.0 / distance;
        divisor = sqrt(divisor);
        vector_x *= divisor;
        vector_y *= divisor;
        vector_z *= divisor;
    };

    // Rotates a 3D vector and returns it normalized
    static double* __rotate_vector_3d(double vector_x, double vector_y, double vector_z, double rotation_x, double rotation_y, double rotation_z) {
        double* to_return = new double[3];

        // Calculate the first XZ angle
        double total_xz_length = std::sqrt(vector_x * vector_x + vector_z * vector_z);
        double to_add = 0;
        if(total_xz_length > 0) to_add = std::acos(std::abs(vector_z) / total_xz_length);
        // Get the current angle
        double current_angle = 0;
        if(vector_z >= 0 && vector_x >= 0) {current_angle = to_add;}
        else if (vector_z < 0 && vector_x >= 0) {current_angle = 3.1415 - to_add;}
        else if(vector_z < 0 && vector_x < 0) {current_angle = 3.1415 + to_add;}
        else {current_angle = 3.1415 * 2.0 - to_add;}
        // Get the final XZ position
        current_angle += degrees_to_radians(rotation_y);
        while(current_angle < 0) current_angle += 3.1415 * 2.0;
        while(current_angle >= 3.1415 * 2) current_angle -= 3.1415 * 2.0;
        to_return[2] = std::cos(current_angle);
        to_return[0] = std::sin(current_angle);

        // Calculate the real local Y anchored position
        double total_length = std::sqrt(total_xz_length * total_xz_length + vector_y * vector_y);
        if(total_length > 0) {
            // Calculate the Y multiplier
            // Calculate the current angle
            double y_sin = std::abs(vector_y) / total_length;
            // Apply an X transformation
            double current_angle = std::asin(y_sin);
            // Get the Y position
            double final_angle = current_angle + degrees_to_radians(rotation_x);
            if(vector_y > 0) final_angle += 3.1415;
            y_sin = std::sin(final_angle);
            to_return[1] = -y_sin * total_length;

            // Calculate the XZ multiplier
            while(current_angle > 3.1415 / 2.0 && current_angle < 3.1415 * 1.5) current_angle += 3.1415;
            while(current_angle >= 3.1415 * 2) current_angle -= 3.1415 * 2;
            final_angle = current_angle + degrees_to_radians(rotation_x);
            while(final_angle < 0) final_angle += 3.1415 * 2;
            while(final_angle >= 3.1415 * 2) final_angle -= 3.1415 * 2;
            const double real_final_angle = final_angle;
            while(final_angle > 3.1415 / 2.0 && final_angle < 3.1415 * 1.5) final_angle += 3.1415;
            while(final_angle >= 3.1415 * 2) final_angle -= 3.1415 * 2;
            // Apply the XZ multiplier
            double y_cos = std::abs(std::cos(final_angle));
            if(real_final_angle > 3.1415 / 2.0 && real_final_angle < 3.1415 * 1.5) y_cos = -y_cos;
            to_return[0] *= y_cos;
            to_return[2] *= y_cos;
        }
        else to_return[1] = 0; //*/

        // Scale each vectors
        to_return[2] *= total_length;
        to_return[0] *= total_length;

        return to_return;
    }

    class Transform_Object_3D : public Point_3D {
        // Class representing a 3D object transform
    public:

        //*********
        //
        // Transform_Object_3D main members
        //
        //*********

        // Transform_Object_3D constructor
        Transform_Object_3D(){};

        // Adds a children
        void add_child(const std::shared_ptr<Transform_Object_3D>& children_to_add) {a_children.push_back(children_to_add);};
        // Removes a children
        void remove_child(const std::shared_ptr<Transform_Object_3D>& children_to_add){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children[i].get()==children_to_add.get())a_children.erase(a_children.begin()+i,a_children.begin()+i+1);}};

        // Getters and setters
        inline std::vector<std::shared_ptr<Transform_Object_3D>>& children() {return a_children;};
        inline unsigned int id() const {return a_id;};
        inline Transform_Object_3D* parent() const {return a_parent.get();};
        inline void set_parent(const std::shared_ptr<Transform_Object_3D>& new_parent) {
            a_parent = new_parent;
            update_vectors();
        };
        inline void set_parent(std::shared_ptr<Transform_Object_3D>* new_parent) {
            if(new_parent==0) {
                a_parent.reset();
                update_vectors();
            }
            else set_parent(*new_parent);
        };

        //*********
        //
        // Transform_Object_3D position system
        //
        //*********

        // Returns the distance from an another object
        inline double distance(const Transform_Object_3D& object) { return std::sqrt((pow(object.absolute_inner_x() - absolute_inner_x(), 2) + pow(object.absolute_inner_y() - absolute_inner_y(), 2)) + pow(object.absolute_inner_z() - absolute_inner_z(), 2)); };

        // Absolute position handling
        // Returns the absolute X position
        inline double absolute_x() const {
            if(parent() == 0)return a_real_local_parent_x;
            return parent()->absolute_x() + a_real_local_parent_x ;
        };
        // Returns the absolute Y position
        inline double absolute_y() const {
            if(parent() == 0)return a_real_local_parent_y;
            return parent()->absolute_y() + a_real_local_parent_y;
        };
        // Returns the absolute Z position
        inline double absolute_z() const {
            if(parent() == 0)return a_real_local_parent_z;
            return parent()->absolute_z() + a_real_local_parent_z;
        }
        // Returns the real local parent position
        inline double __real_local_parent_x() const {return a_real_local_parent_x;};
        inline double __real_local_parent_y() const {return a_real_local_parent_y;};
        inline double __real_local_parent_z() const {return a_real_local_parent_z;};

        // Absolute inner position handling
        // Returns the absolute inner X position
        inline double absolute_inner_x() const { return absolute_x() + a_real_local_anchored_x; };
        // Returns the absolute inner Y position
        inline double absolute_inner_y() const { return absolute_y() + a_real_local_anchored_y; };
        // Returns the absolute inner Z position
        inline double absolute_inner_z() const { return absolute_z() + a_real_local_anchored_z; };

        // Anchored position
        // Returns the X anchored position of the point
        inline double anchored_x() const {return a_anchored_x;};
        // Returns the Y anchored position of the point
        inline double anchored_y() const {return a_anchored_y;};
        // Returns the Z anchored position of the point
        inline double anchored_z() const {return a_anchored_z;};

        // Returns the X of the forward vector of the object
        inline double forward_vector_x() const {return a_forward_vector_x;};
        // Returns the Y of the forward vector of the object
        inline double forward_vector_y() const {return a_forward_vector_y;};
        // Returns the Z of the forward vector of the object
        inline double forward_vector_z() const {return a_forward_vector_z;};
        // Returns the X of the right vector of the object
        inline double right_vector_x() const {return a_right_vector_x;};
        // Returns the Y of the right vector of the object
        inline double right_vector_y() const {return a_right_vector_y;};
        // Returns the Z of the right vector of the object
        inline double right_vector_z() const {return a_right_vector_z;};
        // Returns the X of the top vector of the object
        inline double top_vector_x() const {return a_top_vector_x;};
        // Returns the Y of the top vector of the object
        inline double top_vector_y() const {return a_top_vector_y;};
        // Returns the Z of the top vector of the object
        inline double top_vector_z() const {return a_top_vector_z;};

        // Update the real local position of the object
        void update_real_local_position() {
            // Calculate the real local parent position
            if(parent() != 0) {
                double total_xz_length_with_scale = sqrt(x() * x() * parent()->absolute_scale_x() * parent()->absolute_scale_x() + z() * z() * parent()->absolute_scale_z() * parent()->absolute_scale_z());
                double total_length_with_scale = sqrt(total_xz_length_with_scale * total_xz_length_with_scale + y() * y() * parent()->absolute_scale_y() * parent()->absolute_scale_y());

                // Rotate the vector
                double* rotated = __rotate_vector_3d(x() * parent()->absolute_scale_x(), y() * parent()->absolute_scale_y(), z() * parent()->absolute_scale_z(), parent()->absolute_rotation_x(), parent()->absolute_rotation_y(), parent()->absolute_rotation_z());

                // Calculate the final positions
                a_real_local_parent_x = rotated[0];
                a_real_local_parent_y = rotated[1];
                a_real_local_parent_z = rotated[2];
            }
            else {
                a_real_local_parent_x = x();
                a_real_local_parent_y = y();
                a_real_local_parent_z = z();
            }
        }

        // Update the rotation of the object
        void update_rotation() {
            // Calculate the real local Y anchored position
            double total_xz_length_with_scale = anchored_x() * absolute_scale_x() * anchored_x() * absolute_scale_x() + anchored_z() * absolute_scale_z() * anchored_z() * absolute_scale_z();
            double total_length_with_scale = std::sqrt(total_xz_length_with_scale + anchored_y() * absolute_scale_y() * anchored_y() * absolute_scale_y());

            // Rotate the vector
            double* rotated = __rotate_vector_3d(-anchored_x() * absolute_scale_x(), -anchored_y() * absolute_scale_y(), -anchored_z() * absolute_scale_z(), absolute_rotation_x(), absolute_rotation_y(), absolute_rotation_z());

            a_real_local_anchored_x = rotated[0];
            a_real_local_anchored_y = rotated[1];
            a_real_local_anchored_z = rotated[2];
        }

        // Update each vectors
        void update_vectors() {
            // Update the directions vector
            // Calculate the forward vector
            double* new_forward_vector = __rotate_vector_3d(0, 0, 1, absolute_rotation_x(), absolute_rotation_y(), absolute_rotation_z());
            a_forward_vector_x = new_forward_vector[0]; a_forward_vector_y = new_forward_vector[1]; a_forward_vector_z = new_forward_vector[2];
            // Calculate the right vector
            double* new_right_vector = __rotate_vector_3d(a_forward_vector_x, 0, a_forward_vector_z, 0, -90, 0);
            a_right_vector_x = new_right_vector[0]; a_right_vector_y = new_right_vector[1]; a_right_vector_z = new_right_vector[2];
            // Create the top vector
            double* new_top_vector = __rotate_vector_3d(0, 1, 0, absolute_rotation_x(), absolute_rotation_y(), absolute_rotation_z());
            a_top_vector_x = new_top_vector[0]; a_top_vector_y = new_top_vector[1]; a_top_vector_z = new_top_vector[2];
            // Free the memory
            delete new_forward_vector; new_forward_vector = 0;
            delete new_right_vector; new_right_vector = 0;
            delete new_top_vector; new_top_vector = 0;

            // Update the real position
            update_real_local_position();
            update_rotation();

            // Update children
            for(int i = 0;i<static_cast<int>(children().size());i++) {
                children()[i].get()->update_vectors();
            }
        }

        // Move on the forward axis
        inline void move_forward(double force) {move_xyz(forward_vector_x() * force, forward_vector_y() * force, forward_vector_z() * force);};
        // Move on the forward XZ axis
        inline void move_forward_xz(double force) {
            double multiplier = std::sqrt(forward_vector_x() * forward_vector_x() + forward_vector_z() * forward_vector_z());
            double final_x = forward_vector_x();
            double final_y = 0;
            double final_z = forward_vector_z();
            normalize_3d(final_x, final_y, final_z);
            move_xyz(final_x * force, final_y, final_z * force);
        };
        // Move on the right axis
        inline void move_right(double force) {move_xyz(right_vector_x() * force, right_vector_y() * force, right_vector_z() * force);};
        // Move on the top axis
        inline void move_top(double force) {move_xyz(top_vector_x() * force, top_vector_y() * force, top_vector_z() * force);};

        // Getters and setters
        inline void set_anchored_x(double new_anchored_x) {a_anchored_x = new_anchored_x;};
        inline void set_anchored_y(double new_anchored_y) {a_anchored_y = new_anchored_y;};
        inline void set_anchored_z(double new_anchored_z) {a_anchored_z = new_anchored_z;};
        virtual void set_x(double new_x) {Point_3D::set_x(new_x);update_real_local_position();};
        virtual void set_y(double new_y) {Point_3D::set_y(new_y);update_real_local_position();};
        virtual void set_z(double new_z) {Point_3D::set_z(new_z);update_real_local_position();};

        //*********
        //
        // Transform_Object_3D rotation system
        //
        //*********

        // Returns the absolute X rotation of the object
        inline double absolute_rotation_x() const {if(parent() == 0)return a_rotation_x;return parent()->absolute_rotation_x() + a_rotation_x;};
        // Returns the absolute Y rotation of the object
        inline double absolute_rotation_y() const {if(parent() == 0)return a_rotation_y;return parent()->absolute_rotation_y() + a_rotation_y;};
        // Returns the absolute Z rotation of the object
        inline double absolute_rotation_z() const {if(parent() == 0)return a_rotation_z;return parent()->absolute_rotation_z() + a_rotation_z;};

        // Rotate easily the object
        inline void rotate_x(double movement) {set_rotation_x(rotation_x() + movement);};
        // Rotate easily the object
        inline void rotate_y(double movement) {set_rotation_y(rotation_y() + movement);};
        // Rotate easily the object
        inline void rotate_z(double movement) {set_rotation_z(rotation_z() + movement);};

        // Getters
        inline void set_rotation_x(double new_rotation_x) {a_rotation_x = new_rotation_x;update_vectors();};
        inline void set_rotation_y(double new_rotation_y) {a_rotation_y = new_rotation_y;update_vectors();};
        inline void set_rotation_z(double new_rotation_z) {a_rotation_z = new_rotation_z;update_vectors();};
        inline double rotation_x() const {return a_rotation_x;};
        inline double rotation_y() const {return a_rotation_y;};
        inline double rotation_z() const {return a_rotation_z;};

        //*********
        //
        // Transform_Object_3D scaling system
        //
        //*********

        // Returns the absolute X scale
        inline double absolute_scale_x() const {if(parent() == 0)return scale_x();else return parent()->absolute_scale_x() * scale_x();};
        // Returns the absolute Y scale
        inline double absolute_scale_y() const {if(parent() == 0)return scale_y();else return parent()->absolute_scale_y() * scale_y();};
        // Returns the absolute Z scale
        inline double absolute_scale_z() const {if(parent() == 0)return scale_z();else return parent()->absolute_scale_z() * scale_z();};

        // Getters
        inline void set_scale(double new_scale_x, double new_scale_y, double new_scale_z) {set_scale_x(new_scale_x);set_scale_y(new_scale_y);set_scale_z(new_scale_z);};
        inline void set_scale_x(double new_scale_x) {a_scale_x = new_scale_x;};
        inline void set_scale_y(double new_scale_y) {a_scale_y = new_scale_y;};
        inline void set_scale_z(double new_scale_z) {a_scale_z = new_scale_z;};
        inline double scale_x() const {return a_scale_x;};
        inline double scale_y() const {return a_scale_y;};
        inline double scale_z() const {return a_scale_z;};

    protected:

        //*********
        //
        // Transform_Object_3D mains members
        //
        //*********

        // ID of the transform object
        unsigned int a_id = 0;

    private:

        //*********
        //
        // Transform_Object_3D mains members
        //
        //*********

        // Children of the object
        std::vector<std::shared_ptr<Transform_Object_3D>> a_children;
        // Parent of the object
        std::shared_ptr<Transform_Object_3D> a_parent;

        //*********
        //
        // Transform_Object_3D position system
        //
        //*********

        // Anchored position
        // X anchored position of the point
        double a_anchored_x = 0;
        // Y anchored position of the point
        double a_anchored_y = 0;
        // Z anchored position of the point
        double a_anchored_z = 0;

        // Forward vector
        // Forward x
        double a_forward_vector_x = 0;
        // Forward y
        double a_forward_vector_y = 0;
        // Forward z
        double a_forward_vector_z = 1;
        // Right vector
        // Right x
        double a_right_vector_x = -1;
        // Right y
        double a_right_vector_y = 0;
        // Right z
        double a_right_vector_z = 0;
        // Top vector
        // Top x
        double a_top_vector_x = 0;
        // Top y
        double a_top_vector_y = 1;
        // Top z
        double a_top_vector_z = 0;

        // Real local anchored position
        // Real X anchored position of the point
        double a_real_local_anchored_x = 0;
        // Real Y anchored position of the point
        double a_real_local_anchored_y = 0;
        // Real Z anchored position of the point
        double a_real_local_anchored_z = 0;

        // Real local parent position
        // Real X parent position of the point
        double a_real_local_parent_x = 0;
        // Real Y parent position of the point
        double a_real_local_parent_y = 0;
        // Real Z parent position of the point
        double a_real_local_parent_z = 0;

        // Rotated position
        // X position of the point after a rotation
        double a_rotated_x = 0;
        // Y position of the point after a rotation
        double a_rotated_y = 0;
        // Z position of the point after a rotation
        double a_rotated_z = 0;

        //*********
        //
        // Transform_Object_3D rotation system
        //
        //*********

        // X rotation of the object
        double a_rotation_x = 0;
        // Y rotation of the object
        double a_rotation_y = 0;
        // Z rotation of the object
        double a_rotation_z = 0;

        //*********
        //
        // Transform_Object_3D scaling system
        //
        //*********

        // X scale of the object
        double a_scale_x = 1;
        // Y scale of the object
        double a_scale_y = 1;
        // Z scale of the object
        double a_scale_z = 1;
    };
}

#endif // SCLS_MATH_3D_CORE
