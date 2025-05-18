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
#define SCLS_3D_POINT_ATTRIBUTES_POSITION_IN_VECTOR 5
#define SCLS_3D_SOLID_MAP_CHUNK_X 0
#define SCLS_3D_SOLID_MAP_CHUNK_Z 1

// Include the good header file
#include "scls_math_geometry_core.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
    //
    // The Point_3D class
    //
    //*********

    class Point_3D {
        // Class representing a 3D point
    public:

        //*********
        //
        // Main Point_3D members
        //
        //*********

        // Simplest Point_3D constructor
        Point_3D(double x, double y, double z):a_x(x),a_y(y),a_z(z){};
        Point_3D():Point_3D(0,0,0){};
        Point_3D(const Point_3D& point_copy):Point_3D(point_copy.a_x,point_copy.a_y,point_copy.a_z){};

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
        inline void set_x(double new_x) {a_x = new_x;};
        inline void set_y(double new_y) {a_y = new_y;};
        inline void set_z(double new_z) {a_z = new_z;};
        inline double x() const {return a_x;};
        inline double y() const {return a_y;};
        inline double z() const {return a_z;};

        //*********
        //
        // Point_3D vectorial manipulation
        //
        //*********

        // Distance from another object
        inline double distance(Point_3D point) const {return std::sqrt(std::pow(a_x - point.a_x, 2) + std::pow(a_y - point.a_y, 2) + std::pow(a_z - point.a_z, 2));};
        // Returns the norm of the vector
        inline double norm() const {return std::sqrt(std::pow(a_x, 2) + std::pow(a_y, 2) + std::pow(a_z, 2));};
        // Normalizes the vector
        inline void normalize() {double divisor = (1.0/norm());a_x *= divisor;a_y *= divisor;a_z *= divisor;};
        inline Point_3D normalized() {double divisor = (1.0/norm());return Point_3D(a_x * divisor, a_y * divisor, a_z * divisor);};

        // Applies a rotation to the point
        inline Point_3D rotated(Point_3D rotation) const {
            double* rotated_point = __rotate_vector_3d(a_x, a_y, a_z, rotation.a_x, rotation.a_y, rotation.a_z);
            scls::Point_3D to_return;
            to_return.a_x = rotated_point[0]; to_return.a_y = rotated_point[1]; to_return.a_z = rotated_point[2];
            delete rotated_point; rotated_point = 0; return to_return;
        };
        inline void rotate(scls::Point_3D rotation) {rotation = rotated(rotation);a_x = rotation.a_x; a_y = rotation.a_y; a_z = rotation.a_z;};
        inline void rotate_y(double rotation) {rotate(scls::Point_3D(0, rotation, 0));};

        // Adds a vector to this vector with another
        inline void __add(Point_3D object) {set_x(x()+object.x());set_y(y()+object.y());set_z(z()+object.z());};
        inline Point_3D __add_without_modification(Point_3D object) const {Point_3D to_return;to_return.set_x(x()+object.x());to_return.set_y(y()+object.y());to_return.set_z(z()+object.z());return to_return;};
        // Divides a vector to this vector with another
        inline void __divide(double value) {set_x(x()/value);set_y(y()/value);set_z(z()/value);};
        // Multiplies a vector to this vector with another
        inline void __multiply(double value) {set_x(x()*value);set_y(y()*value);set_z(z()*value);};
        inline void __multiply(Point_3D value) {set_x(x()*value.x());set_y(y()*value.y());set_z(z()*value.z());};
        // Returns a substraction of this vector with another
        inline void __substract(Point_3D object) {set_x(x()-object.x());set_y(y()-object.y());set_z(z()-object.z());};
        inline Point_3D __substract_without_modification(Point_3D object) const {Point_3D to_return;to_return.set_x(x()-object.x());to_return.set_y(y()-object.y());to_return.set_z(z()-object.z());return to_return;};

        // Built-in operators
        // With Point_3D
        inline Point_3D operator-(Point_3D object)const{return __substract_without_modification(object);};
        inline Point_3D operator+(Point_3D object)const{return __add_without_modification(object);};
        inline Point_3D& operator+=(Point_3D object){__add(object);return *this;};
        inline Point_3D& operator-=(Point_3D object){__substract(object);return *this;};
        inline Point_3D operator*(Point_3D object){Point_3D temp(*this);temp.__multiply(object);return temp;};
        inline Point_3D& operator*=(Point_3D object){__multiply(object);return *this;};
        inline bool operator==(Point_3D object){return object.x() == x() && object.y() == y() && object.z() == z();};
        // With double
        inline Point_3D operator/(double object)const{Point_3D to_return=*this;to_return.__divide(object);return to_return;};
        inline Point_3D operator*(double object)const{Point_3D to_return=*this;to_return.__multiply(object);return to_return;};
        inline Point_3D& operator/=(double object){__divide(object);return *this;};
        inline Point_3D& operator*=(double object){__multiply(object);return *this;};
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
    }; typedef Point_3D Vector_3D;

    // Returns a point 3D with an angle (on y axis)
    Point_3D vector_with_angle(double angle);
    Point_3D vector_with_angle_degrees(double angle);

    //*********
    //
    // The Transform_Object_3D class
    //
    //*********

    class Transform_Object_3D {
        // Class representing a 3D object transform
    public:

        //*********
        //
        // Transform_Object_3D main members
        //
        //*********

        // Transform_Object_3D constructor
        Transform_Object_3D(){};
        // Copy constructor
        Transform_Object_3D(Point_3D point_3d):a_position(point_3d){};
        // Transform_Object_3D destructor
        ~Transform_Object_3D(){if(a_parent.get() != 0) a_parent.get()->remove_child(this);};

        // Adds a children
        void add_child(std::shared_ptr<Transform_Object_3D> children_to_add) {a_children.push_back(children_to_add);};
        // Removes a children
        void remove_child(Transform_Object_3D* children_to_remove){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children[i].lock().get() ==children_to_remove)a_children.erase(a_children.begin()+i,a_children.begin()+i+1);}};
        void remove_child(std::shared_ptr<Transform_Object_3D> children_to_remove){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children[i].lock().get()==children_to_remove.get())a_children.erase(a_children.begin()+i,a_children.begin()+i+1);}};

        // Getters and setters
        inline std::vector<std::weak_ptr<Transform_Object_3D>>& children() {return a_children;};
        inline unsigned int id() const {return a_id;};
        inline Transform_Object_3D* parent() const {return a_parent.get();};
        inline void set_parent(const std::shared_ptr<Transform_Object_3D>& new_parent) {
            if(a_parent.get() != 0) a_parent.get()->remove_child(this);
            a_parent = new_parent;
            a_parent.get()->add_child(a_this_object.lock());
            update_vectors();
        };
        inline void set_parent(std::shared_ptr<Transform_Object_3D>* new_parent) {
            if(new_parent==0) {
                if(a_parent.get() != 0) a_parent.get()->remove_child(this);
                a_parent.reset();
                update_vectors();
            }
            else set_parent(*new_parent);
        };
        inline void set_this_object(std::weak_ptr<Transform_Object_3D> this_object){a_this_object=this_object;};

        //*********
        //
        // Transform_Object_3D position system
        //
        //*********

        // Returns the distance from an another object
        inline double distance(Point_3D point) { return std::sqrt((pow(point.x() - absolute_inner_x(), 2) + pow(point.y() - absolute_inner_y(), 2)) + pow(point.z() - absolute_inner_z(), 2)); };
        inline double distance(const Transform_Object_3D& object) { return std::sqrt((pow(object.absolute_inner_x() - absolute_inner_x(), 2) + pow(object.absolute_inner_y() - absolute_inner_y(), 2)) + pow(object.absolute_inner_z() - absolute_inner_z(), 2)); };

        // Absolute position handling
        // Returns the absolute X position
        //inline double absolute_x() const {if(parent() == 0){return a_real_local_parent_x;}return parent()->absolute_x() + a_real_local_parent_x;};
        inline double absolute_x() const {if(parent() == 0){return a_real_local_parent_x;}return parent()->absolute_x() + x() * parent()->absolute_scale_x();};
        inline double absolute_y() const {if(parent() == 0){return a_real_local_parent_y;}return parent()->absolute_y() + a_real_local_parent_y;};
        //inline double absolute_z() const {if(parent() == 0){return a_real_local_parent_z;}return parent()->absolute_z() + a_real_local_parent_z;};
        inline double absolute_z() const {if(parent() == 0){return a_real_local_parent_z;}return parent()->absolute_z() + z() * parent()->absolute_scale_z();};
        // Returns the real local parent position
        inline double __real_local_parent_x() const {return a_real_local_parent_x;};
        inline double __real_local_parent_y() const {return a_real_local_parent_y;};
        inline double __real_local_parent_z() const {return a_real_local_parent_z;};

        // Absolute inner position handling
        // Returns the absolute inner X position
        //inline double absolute_inner_x() const { return absolute_x() + a_real_local_anchored_x; };
        inline double absolute_inner_x() const { return absolute_x(); };
        // Returns the absolute inner Y position
        inline double absolute_inner_y() const { return absolute_y() + a_real_local_anchored_y; };
        // Returns the absolute inner Z position
        //inline double absolute_inner_z() const { return absolute_z() + a_real_local_anchored_z; };
        inline double absolute_inner_z() const { return absolute_z(); };

        // Anchored position
        // Returns the X anchored position of the point
        inline double anchored_x() const {return a_anchored_x;};
        // Returns the Y anchored position of the point
        inline double anchored_y() const {return a_anchored_y;};
        // Returns the Z anchored position of the point
        inline double anchored_z() const {return a_anchored_z;};

        // Returns the X of the forward vector of the object
        inline scls::Point_3D forward_vector() const {return Point_3D(a_forward_vector_x, a_forward_vector_y, a_forward_vector_z);};
        inline double forward_vector_x() const {return a_forward_vector_x;};
        inline double forward_vector_y() const {return a_forward_vector_y;};
        inline double forward_vector_z() const {return a_forward_vector_z;};
        // Returns the X of the right vector of the object
        inline double right_vector_x() const {return a_right_vector_x;};
        inline double right_vector_y() const {return a_right_vector_y;};
        inline double right_vector_z() const {return a_right_vector_z;};
        // Returns the X of the top vector of the object
        inline double top_vector_x() const {return a_top_vector_x;};
        inline double top_vector_y() const {return a_top_vector_y;};
        inline double top_vector_z() const {return a_top_vector_z;};

        // Update the real local position of the children
        void update_children_real_local_position() {
            for(int i = 0;i<static_cast<int>(children().size());i++){
                Transform_Object_3D* current_child = children()[i].lock().get();
                if(current_child != 0) {current_child->update_real_local_position();}
            }
        };
        // Update the real local position of the object
        void update_real_local_position() {
            // Calculate the real local parent position
            if(parent() != 0) {
                // Rotate the vector
                double* rotated = __rotate_vector_3d(x() * parent()->absolute_scale_x(), y() * parent()->absolute_scale_y(), z() * parent()->absolute_scale_z(), parent()->absolute_rotation_x(), parent()->absolute_rotation_y(), parent()->absolute_rotation_z());

                // Calculate the final positions

                a_real_local_parent_x = rotated[0];
                a_real_local_parent_y = rotated[1];
                a_real_local_parent_z = rotated[2];
                delete rotated; rotated = 0;
            }
            else {
                a_real_local_parent_x = x();
                a_real_local_parent_y = y();
                a_real_local_parent_z = z();
            } update_children_real_local_position();
        }

        // Update the rotation of the object
        void update_rotation() {
            // Rotate the vector
            double* rotated = __rotate_vector_3d(-anchored_x() * absolute_scale_x(), -anchored_y() * absolute_scale_y(), -anchored_z() * absolute_scale_z(), absolute_rotation_x(), absolute_rotation_y(), absolute_rotation_z());

            a_real_local_anchored_x = rotated[0];
            a_real_local_anchored_y = rotated[1];
            a_real_local_anchored_z = rotated[2];

            delete rotated; rotated = 0;
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
                Transform_Object_3D* current_child = children()[i].lock().get();
                if(current_child != 0){current_child->update_vectors();}
            }
        }

        // Move on the forward axis
        inline void move_forward(double force) {move_xyz(forward_vector_x() * force, forward_vector_y() * force, forward_vector_z() * force);};
        // Move on the forward XZ axis
        inline void move_forward_xz(double force) {
            // double multiplier = std::sqrt(forward_vector_x() * forward_vector_x() + forward_vector_z() * forward_vector_z());
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
        // Move easily the object
        inline void move_xyz(double movement_x, double movement_y, double movement_z) {move_x(movement_x);move_y(movement_y);move_z(movement_z);};
        inline void move_xyz(Point_3D movement) {move_x(movement.x());move_y(movement.y());move_z(movement.z());};
        inline void move_x(double movement) {set_x(x() + movement);};
        inline void move_y(double movement) {set_y(y() + movement);};
        inline void move_z(double movement) {set_z(z() + movement);};

        // Getters and setters
        inline Point_3D position() const {return a_position;};
        inline void set_anchored_x(double new_anchored_x) {a_anchored_x = new_anchored_x;};
        inline void set_anchored_y(double new_anchored_y) {a_anchored_y = new_anchored_y;};
        inline void set_anchored_z(double new_anchored_z) {a_anchored_z = new_anchored_z;};
        inline void set_position(double new_x, double new_y, double new_z){a_position.set_x(new_x);a_position.set_y(new_y);a_position.set_z(new_z);update_real_local_position();};
        inline void set_position(Point_3D new_position){a_position.set_x(new_position.x());a_position.set_y(new_position.y());a_position.set_z(new_position.z());update_real_local_position();};
        virtual void set_x(double new_x) {a_position.set_x(new_x);update_real_local_position();};
        virtual void set_y(double new_y) {a_position.set_y(new_y);update_real_local_position();};
        virtual void set_z(double new_z) {a_position.set_z(new_z);update_real_local_position();};
        inline double x() const {return a_position.x();};
        inline double y() const {return a_position.y();};
        inline double z() const {return a_position.z();};

        //*********
        //
        // Transform_Object_3D rotation system
        //
        //*********

        // Returns the absolute X rotation of the object
        inline double absolute_rotation_x() const {if(parent() == 0)return a_rotation_x;return parent()->absolute_rotation_x() + a_rotation_x;};
        inline double absolute_rotation_y() const {if(parent() == 0)return a_rotation_y;return parent()->absolute_rotation_y() + a_rotation_y;};
        inline double absolute_rotation_z() const {if(parent() == 0)return a_rotation_z;return parent()->absolute_rotation_z() + a_rotation_z;};

        // Rotate easily the object
        inline void rotate(Point_3D movement){set_rotation(rotation() + movement);};
        inline void rotate_x(double movement) {set_rotation_x(rotation_x() + movement);};
        inline void rotate_y(double movement) {set_rotation_y(rotation_y() + movement);};
        inline void rotate_z(double movement) {set_rotation_z(rotation_z() + movement);};

        // Returns a vector to an object
        inline Point_3D vector_to(Point_3D object) {Point_3D to_return = a_position.__substract_without_modification(object);to_return.normalize();return to_return;};
        inline Point_3D vector_to(Transform_Object_3D object) {return vector_to(object.a_position);};

        // Getters
        inline void set_rotation(Point_3D new_rotation){a_rotation_x = new_rotation.x();a_rotation_y = new_rotation.y();a_rotation_z = new_rotation.z();update_vectors();};
        inline void set_rotation_x(double new_rotation_x) {a_rotation_x = new_rotation_x;update_vectors();};
        inline void set_rotation_y(double new_rotation_y) {a_rotation_y = new_rotation_y;update_vectors();};
        inline void set_rotation_z(double new_rotation_z) {a_rotation_z = new_rotation_z;update_vectors();};
        inline Point_3D rotation() const {return Point_3D(a_rotation_x, a_rotation_y, a_rotation_z);};
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
        inline double absolute_scale_y() const {if(parent() == 0)return scale_y();else return parent()->absolute_scale_y() * scale_y();};
        inline double absolute_scale_z() const {if(parent() == 0)return scale_z();else return parent()->absolute_scale_z() * scale_z();};

        // Getters
        inline void set_scale(double new_scale_x, double new_scale_y, double new_scale_z) {set_scale_x(new_scale_x);set_scale_y(new_scale_y);set_scale_z(new_scale_z);};
        inline void set_scale(double new_scale) {set_scale_x(new_scale);set_scale_y(new_scale);set_scale_z(new_scale);};
        inline void set_scale(Point_3D new_scale) {set_scale_x(new_scale.x());set_scale_y(new_scale.y());set_scale_z(new_scale.z());};
        inline void set_scale_x(double new_scale_x) {a_scale_x = new_scale_x;};
        inline void set_scale_y(double new_scale_y) {a_scale_y = new_scale_y;};
        inline void set_scale_z(double new_scale_z) {a_scale_z = new_scale_z;};
        inline Point_3D scale() const {return Point_3D(a_scale_x, a_scale_y, a_scale_z);};
        inline double scale_x() const {return a_scale_x;};
        inline double scale_y() const {return a_scale_y;};
        inline double scale_z() const {return a_scale_z;};

        //*********
        //
        // Transform_Object_3D operators
        //
        //*********

        // Adds a vector to this vector with another
        inline void __add(Point_3D object) {a_position.__add(object);};

        // Built-in operator
        inline Transform_Object_3D& operator+=(Point_3D object){__add(object);return *this;};

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
        std::vector<std::weak_ptr<Transform_Object_3D>> a_children = std::vector<std::weak_ptr<Transform_Object_3D>>();
        // Parent of the object
        std::shared_ptr<Transform_Object_3D> a_parent;
        // Shared ptr to this object
        std::weak_ptr<Transform_Object_3D> a_this_object;

        //*********
        //
        // Transform_Object_3D position system
        //
        //*********

        // Position of the object
        Point_3D a_position;

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
