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
// This file contains the core of the geometry part.
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

#ifndef SCLS_MATH_GEOMETRY_CORE
#define SCLS_MATH_GEOMETRY_CORE

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
#include "scls_math_numbers.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
    //
    // The Point_2D class
    //
    //*********

    // Convert degrees to radians
    inline double degrees_to_radians(double degrees) {return (degrees / 180.0) * SCLS_PI;};

    // Rotates a 3D vector and returns it normalized
    double* __rotate_vector_3d(double vector_x, double vector_y, double vector_z, double rotation_x, double rotation_y, double rotation_z, double anchor_x, double anchor_y, double anchor_z);
    inline double* __rotate_vector_3d(double vector_x, double vector_y, double vector_z, double rotation_x, double rotation_y, double rotation_z){return __rotate_vector_3d(vector_x, vector_y, vector_z, rotation_x, rotation_y, rotation_z, 0, 0, 0);}

    class Point_2D {
        // Class representing a 2D point
    public:

        //*********
        //
        // Main Point_2D members
        //
        //*********

        // Simplest Point_2D constructor
        Point_2D(Fraction x, Fraction y):a_x(x),a_y(y){};
        Point_2D():Point_2D(0,0){};
        Point_2D(const Point_2D& point_copy):Point_2D(point_copy.a_x,point_copy.a_y){};

        //*********
        //
        // Point_2D position system
        //
        //*********

        // Move easily the object
        inline void move_xy(scls::Fraction movement_x, scls::Fraction movement_y) {move_x(movement_x);move_y(movement_y);};
        inline void move_x(scls::Fraction movement) {set_x(x() + movement);};
        inline void move_y(scls::Fraction movement) {set_y(y() + movement);};

        // Getters
        inline void set_x(scls::Fraction new_x) {a_x = new_x;};
        inline void set_y(scls::Fraction new_y) {a_y = new_y;};
        inline scls::Fraction x() const {return a_x;};
        inline scls::Fraction y() const {return a_y;};

        //*********
        //
        // Point_2D vectorial manipulation
        //
        //*********

        // Distance from another object
        inline double distance(Point_2D point) const {return std::sqrt(std::pow((a_x - point.a_x).to_double(), 2) + std::pow((a_y - point.a_y).to_double(), 2));};
        // Returns the norm of the vector
        inline double norm() const {return std::sqrt(std::pow(a_x.to_double(), 2) + std::pow(a_y.to_double(), 2));};
        // Normalizes the vector
        inline void normalize() {scls::Fraction divisor = scls::Fraction(1) / Fraction::from_double(norm());a_x *= divisor;a_y *= divisor;};
        inline Point_2D normalized() {scls::Fraction divisor = scls::Fraction(1) / Fraction::from_double(norm());return Point_2D(a_x * divisor, a_y * divisor);};

        // Applies a rotation to the point
        inline Point_2D rotated(double rotation) const {
            double* rotated_point = __rotate_vector_3d(a_x.to_double(), 0, a_y.to_double(), 0, rotation, 0);
            scls::Point_2D to_return;to_return.a_x = Fraction::from_double(rotated_point[0]); to_return.a_y = Fraction::from_double(rotated_point[2]);
            delete rotated_point; rotated_point = 0; return to_return;
        };
        inline void rotate(double rotation) {Point_2D new_point = rotated(rotation);a_x = new_point.a_x; a_y = new_point.a_y;};

        // Adds a vector to this vector with another
        inline void __add(Point_2D object) {set_x(x()+object.x());set_y(y()+object.y());};
        inline Point_2D __add_without_modification(Point_2D object) const {Point_2D to_return;to_return.set_x(x()+object.x());to_return.set_y(y()+object.y());return to_return;};
        // Divides a vector to this vector with another
        inline void __divide(Fraction value) {set_x(x()/value);set_y(y()/value);};
        // Multiplies a vector to this vector with another
        inline void __multiply(Fraction value) {set_x(x()*value);set_y(y()*value);};
        inline void __multiply(Point_2D value) {set_x(x()*value.x());set_y(y()*value.y());};
        // Returns a substraction of this vector with another
        inline void __substract(Point_2D object) {set_x(x()-object.x());set_y(y()-object.y());};
        inline Point_2D __substract_without_modification(Point_2D object) const {Point_2D to_return;to_return.set_x(x()-object.x());to_return.set_y(y()-object.y());return to_return;};

        // Built-in operators
        // With Point_2D
        inline Point_2D operator-(Point_2D object)const{return __substract_without_modification(object);};
        inline Point_2D operator+(Point_2D object)const{return __add_without_modification(object);};
        inline Point_2D& operator+=(Point_2D object){__add(object);return *this;};
        inline Point_2D& operator-=(Point_2D object){__substract(object);return *this;};
        inline Point_2D operator*(Point_2D object){Point_2D temp(*this);temp.__multiply(object);return temp;};
        inline Point_2D& operator*=(Point_2D object){__multiply(object);return *this;};
        inline bool operator==(Point_2D object){return object.x() == x() && object.y() == y();};
        // With fraction
        inline Point_2D operator/(Fraction object)const{Point_2D to_return=*this;to_return.__divide(object);return to_return;};
        inline Point_2D operator*(Fraction object)const{Point_2D to_return=*this;to_return.__multiply(object);return to_return;};
        inline Point_2D& operator/=(Fraction object){__divide(object);return *this;};
        inline Point_2D& operator*=(Fraction object){__multiply(object);return *this;};
    private:
        //*********
        //
        // Point_2D position system
        //
        //*********

        // X position of the point
        scls::Fraction a_x = 0;
        // Y position of the point
        scls::Fraction a_y = 0;
    }; typedef Point_2D Vector_2D;

    // Returns the datas point of two crossing lines
    struct Crossing_Datas {bool crossed = false;double crossing_x = 0;double crossing_y = 0;bool same_lines = false;};
    Crossing_Datas check_crossing(double first_point_x, double first_point_y, double second_point_x, double second_point_y, double third_point_x, double third_point_y, double fourth_point_x, double fourth_point_y);

    // Returns the datas point of two crossing segments
    struct Crossing_Datas_Segment {bool crossed_in_segment = false;Crossing_Datas crossing_datas;};
    Crossing_Datas_Segment check_crossing_segment(double first_point_x, double first_point_y, double second_point_x, double second_point_y, double third_point_x, double third_point_y, double fourth_point_x, double fourth_point_y, bool check_first_and_second_point = true, bool check_third_and_fourth_point = true);

    //*********
    //
    // The Transform_Object_3D class
    //
    //*********

    // Normalizes a 3D vector
    void normalize_3d(double& vector_x, double& vector_y, double& vector_z);
    void normalize_3d(scls::Fraction& vector_x, scls::Fraction& vector_y, scls::Fraction& vector_z);

    // Returns the angle in radians for a vector 3D
    double vector_2d_angle(double vector_x, double vector_y);
    double vector_2d_angle(Point_2D vector);

    class Transform_Object_2D {
        // Class representing a 2D object transform
    public:

        //*********
        //
        // Transform_Object_2D main members
        //
        //*********

        // Transform_Object_2D constructor
        Transform_Object_2D(){};
        // Copy constructor
        Transform_Object_2D(Point_2D point_2d):a_position(point_2d){};
        // Transform_Object_2D destructor
        ~Transform_Object_2D(){if(parent() != 0){parent()->remove_child(this);}};

        // Soft reset the transform
        inline void soft_reset(){a_moved_during_this_frame = false;};

        // Getters and setters
        inline std::vector<std::shared_ptr<Transform_Object_2D>>& children() {return a_children;};
        inline unsigned int id() const {return a_id;};
        inline bool moved_during_this_frame() const {return a_moved_during_this_frame;};
        inline Transform_Object_2D* parent() const {return a_parent.lock().get();};
        inline void remove_child(Transform_Object_2D* child){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children.at(i).get()==child){a_children.erase(a_children.begin()+i);return;}};};
        inline void set_delta_time(scls::Fraction new_delta_time){a_delta_time = new_delta_time;};
        inline void set_parent(std::shared_ptr<Transform_Object_2D> new_parent) {
            if(parent() != 0){parent()->remove_child(this);}
            a_parent = new_parent;
            parent()->children().push_back(a_this_object.lock());
        };
        inline void set_parent(std::shared_ptr<Transform_Object_2D>* new_parent) {if(new_parent==0) {if(parent() != 0){parent()->remove_child(this);}a_parent.reset();}else{set_parent(*new_parent);}};
        inline void set_this_object(std::weak_ptr<Transform_Object_2D> this_object){a_this_object=this_object;};

        //*********
        //
        // Transform_Object_2D position system
        //
        //*********

        // Returns the distance from an another object
        inline double distance(Point_2D point) {return std::sqrt((pow(point.x().to_double() - absolute_x().to_double(), 2) + pow(point.y().to_double() - absolute_y().to_double(), 2))); };
        inline double distance(const Transform_Object_2D& object) {return std::sqrt((pow((object.absolute_x() - absolute_x()).to_double(), 2) + pow((object.absolute_y() - absolute_y()).to_double(), 2)));};

        // Absolute position handling
        // Returns the absolute X position
        inline scls::Fraction absolute_x() const {if(parent() == 0){return x();}return parent()->absolute_x() + x() * parent()->absolute_scale_x();};
        inline scls::Fraction absolute_y() const {if(parent() == 0){return y();}return parent()->absolute_y() + y() * parent()->absolute_scale_y();};

        // Move easily the object
        inline void move_x(scls::Fraction movement) {set_x(x() + movement);};
        inline void move_y(scls::Fraction movement) {set_y(y() + movement);};

        // Extremums
        inline scls::Fraction max_x() const {return x() + scale_x() / 2;};
        inline scls::Fraction max_y() const {return y() + scale_y() / 2;};
        inline scls::Fraction min_x() const {return x() - scale_x() / 2;};
        inline scls::Fraction min_y() const {return y() - scale_y() / 2;};

        // Accelerates the object
        inline void accelerate(scls::Point_2D acceleration){a_velocity += acceleration;};
        inline void accelerate_x(scls::Fraction acceleration){a_velocity.set_x(a_velocity.x() + acceleration);};
        inline void accelerate_y(scls::Fraction acceleration){a_velocity.set_y(a_velocity.y() + acceleration);};

        // Precise next movement
        inline scls::Fraction max_x_next() const {return max_x() + next_movement_x();};
        inline scls::Fraction max_y_next() const {return max_y() + next_movement_y();};
        inline scls::Fraction min_x_next() const {return min_x() + next_movement_x();};
        inline scls::Fraction min_y_next() const {return min_y() + next_movement_y();};
        inline scls::Point_2D position_next() const {return scls::Point_2D(x() + next_movement_x(), y() + next_movement_y());};
        // Next movement generated by the velocity
        inline scls::Fraction next_movement_x()const{return (velocity().x() * a_delta_time).normalized(5);};
        inline scls::Fraction next_movement_y()const{return (velocity().y() * a_delta_time).normalized(5);};

        // Update the raw velocity
        inline void update_raw_velocity(){a_raw_velocity = a_velocity;};

        // Getters and setters
        inline Point_2D position() const {return a_position;};
        inline Point_2D raw_velocity() const {return a_raw_velocity;};
        inline void set_position(scls::Fraction new_x, scls::Fraction new_y){a_position.set_x(new_x);a_position.set_y(new_y);a_moved_during_this_frame = true;};
        inline void set_position(Point_2D new_position){a_position.set_x(new_position.x());a_position.set_y(new_position.y());a_moved_during_this_frame = true;};
        inline void set_raw_velocity(Point_2D new_raw_velocity){a_raw_velocity.set_x(new_raw_velocity.x());a_raw_velocity.set_y(new_raw_velocity.y());};
        virtual void set_x(scls::Fraction new_x) {a_position.set_x(new_x);a_moved_during_this_frame = true;};
        virtual void set_y(scls::Fraction new_y) {a_position.set_y(new_y);a_moved_during_this_frame = true;};
        inline void set_velocity(Point_2D new_velocity){a_velocity.set_x(new_velocity.x());a_velocity.set_y(new_velocity.y());};
        inline void set_velocity_x(scls::Fraction new_x){a_velocity.set_x(new_x);};
        inline void set_velocity_y(scls::Fraction new_y){a_velocity.set_y(new_y);};
        inline Point_2D velocity() const {return a_velocity;};
        inline scls::Fraction x() const {return a_position.x();};
        inline scls::Fraction y() const {return a_position.y();};

        //*********
        //
        // Transform_Object_3D rotation system
        //
        //*********

        // Returns the absolute X rotation of the object
        inline scls::Fraction absolute_rotation() const {if(parent() == 0){return a_rotation;}return parent()->absolute_rotation() + a_rotation;};

        // Rotate easily the object
        inline void rotate(scls::Fraction movement){set_rotation(movement);};

        // Returns a vector to an object
        inline Point_2D vector_to(Point_2D object) {Point_2D to_return = a_position.__substract_without_modification(object);to_return.normalize();return to_return;};
        inline Point_2D vector_to(Transform_Object_2D object) {return vector_to(object.a_position);};

        // Getters
        inline void set_rotation(scls::Fraction new_rotation){a_rotation = new_rotation;};
        inline scls::Fraction rotation() const {return a_rotation;};

        //*********
        //
        // Transform_Object_2D scaling system
        //
        //*********

        // Returns the absolute X scale
        inline scls::Fraction absolute_scale_x() const {if(parent() == 0)return scale_x();else return parent()->absolute_scale_x() * scale_x();};
        inline scls::Fraction absolute_scale_y() const {if(parent() == 0)return scale_y();else return parent()->absolute_scale_y() * scale_y();};

        // Getters
        inline void set_scale(scls::Fraction new_scale_x, scls::Fraction new_scale_y) {set_scale_x(new_scale_x);set_scale_y(new_scale_y);};
        inline void set_scale(scls::Fraction new_scale) {set_scale_x(new_scale);set_scale_y(new_scale);};
        inline void set_scale(Point_2D new_scale) {a_scale = new_scale;};
        inline void set_scale_x(scls::Fraction new_scale_x) {a_scale.set_x(new_scale_x);};
        inline void set_scale_y(scls::Fraction new_scale_y) {a_scale.set_y(new_scale_y);};
        inline Point_2D scale() const {return a_scale;};
        inline scls::Fraction scale_x() const {return a_scale.x();};
        inline scls::Fraction scale_y() const {return a_scale.y();};

        //*********
        //
        // Transform_Object_2D operators
        //
        //*********

        // Adds a vector to this vector with another
        inline void __add(Point_2D object) {a_position.__add(object);};

        // Built-in operator
        inline Transform_Object_2D& operator+=(Point_2D object){__add(object);return *this;};

    protected:

        //*********
        //
        // Transform_Object_2D mains members
        //
        //*********

        // ID of the transform object
        unsigned int a_id = 0;

    private:

        //*********
        //
        // Transform_Object_2D mains members
        //
        //*********

        // Delta time
        scls::Fraction a_delta_time = scls::Fraction(1, 100);
        // If the transform has been moved during this frame
        bool a_moved_during_this_frame = false;

        // Children of the object
        std::vector<std::shared_ptr<Transform_Object_2D>> a_children = std::vector<std::shared_ptr<Transform_Object_2D>>();
        // Parent of the object
        std::weak_ptr<Transform_Object_2D> a_parent;
        // Shared ptr to this object
        std::weak_ptr<Transform_Object_2D> a_this_object;

        //*********
        //
        // Transform_Object_2D position system
        //
        //*********

        // Position of the object
        Point_2D a_position;

        // Velocity (and raw velocity) of the object
        Point_2D a_raw_velocity;
        Point_2D a_velocity;

        //*********
        //
        // Transform_Object_2D rotation system
        //
        //*********

        // Rotation of the object
        scls::Fraction a_rotation = 0;

        //*********
        //
        // Transform_Object_2D scaling system
        //
        //*********

        // Scale of the object
        Point_2D a_scale = Point_2D(1, 1);
    };
}

#endif // SCLS_MATH_GEOMETRY_CORE
