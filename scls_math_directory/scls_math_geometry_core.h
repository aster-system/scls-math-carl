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

#define SCLS_MATH_TOLERANCE 0.00000001

// Include the good header file
#include "scls_math_numbers.h"
#include "scls_math_polymonial.h"

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
        Point_2D():Point_2D(0,0){};
        Point_2D(double x, double y):a_x(x),a_y(y){};
        Point_2D(const Point_2D& point_copy):Point_2D(point_copy.a_x,point_copy.a_y){};

        //*********
        //
        // Point_2D position system
        //
        //*********

        // Returns if the point is in a rect
        bool in_rect(double tested_x, double tested_y, double tested_width, double tested_height);
        bool in_rect(Point_2D tested_pos, Point_2D tested_scale);

        // Move easily the object
        inline void move_xy(double movement_x, double movement_y) {move_x(movement_x);move_y(movement_y);};
        inline void move_x(double movement) {set_x(x() + movement);};
        inline void move_y(double movement) {set_y(y() + movement);};

        // Getters
        inline void set_x(double new_x) {a_x = new_x;};
        inline void set_y(double new_y) {a_y = new_y;};
        inline double x() const {return a_x;};
        inline double y() const {return a_y;};

        //*********
        //
        // Point_2D vectorial manipulation
        //
        //*********

        // Distance from another object
        inline double distance(Point_2D point) const {return std::sqrt(std::pow((a_x - point.a_x), 2) + std::pow((a_y - point.a_y), 2));};
        // Returns the norm of the vector
        inline double norm() const {return std::sqrt(std::pow(a_x, 2) + std::pow(a_y, 2));};
        // Normalizes the vector
        inline void normalize() {double divisor = norm();a_x /= divisor;a_y /= divisor;};
        inline Point_2D normalized() {double divisor = norm();return Point_2D(a_x / divisor, a_y / divisor);};

        // Applies a rotation to the point
        inline Point_2D rotated(double rotation) const {
            double* rotated_point = __rotate_vector_3d(a_x, 0, a_y, 0, rotation, 0);
            Point_2D to_return;to_return.a_x = rotated_point[0]; to_return.a_y = rotated_point[2];
            delete rotated_point; return to_return;
        };
        inline void rotate(double rotation) {if(rotation != 0){Point_2D new_point = rotated(rotation);a_x = new_point.a_x; a_y = new_point.a_y;}};

        // Adds a vector to this vector with another
        inline void __add(Point_2D object) {set_x(x()+object.x());set_y(y()+object.y());};
        inline Point_2D __add_without_modification(Point_2D object) const {Point_2D to_return;to_return.set_x(x()+object.x());to_return.set_y(y()+object.y());return to_return;};
        // Divides a vector to this vector with another
        inline void __divide(double value) {set_x(x()/value);set_y(y()/value);};
        // Multiplies a vector to this vector with another
        inline void __multiply(double value) {set_x(x()*value);set_y(y()*value);};
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
        // With fraction
        inline Point_2D operator/(double object)const{Point_2D to_return=*this;to_return.__divide(object);return to_return;};
        inline Point_2D operator*(double object)const{Point_2D to_return=*this;to_return.__multiply(object);return to_return;};
        inline Point_2D& operator/=(double object){__divide(object);return *this;};
        inline Point_2D& operator*=(double object){__multiply(object);return *this;};
    private:
        //*********
        //
        // Point_2D position system
        //
        //*********

        // X position of the point
        double a_x = 0;
        // Y position of the point
        double a_y = 0;
    }; typedef Point_2D Vector_2D;

    // Operators
    bool operator==(const Point_2D object_1, const Point_2D object_2);

    class __Point_2D_Formula {
        // Class representing a 2D point with formulas
    public:

        //*********
        //
        // Main __Point_2D_Formula members
        //
        //*********

        // Simplest __Point_2D_Formula constructor
        __Point_2D_Formula(__Formula_Base::Formula x, __Formula_Base::Formula y):a_x(x.formula_base_shared_ptr()),a_y(y.formula_base_shared_ptr()){};
        __Point_2D_Formula(std::shared_ptr<__Formula_Base> x, std::shared_ptr<__Formula_Base> y):a_x(x),a_y(y){};
        __Point_2D_Formula(__Formula_Base x, __Formula_Base y):__Point_2D_Formula(std::make_shared<__Formula_Base>(x), std::make_shared<__Formula_Base>(y)){};
        __Point_2D_Formula(__Fraction_Base x, __Fraction_Base y):__Point_2D_Formula(std::make_shared<__Formula_Base>(x), std::make_shared<__Formula_Base>(y)){};
        __Point_2D_Formula(Fraction x, Fraction y):__Point_2D_Formula(std::make_shared<__Formula_Base>(x), std::make_shared<__Formula_Base>(y)){};
        __Point_2D_Formula(double x, double y):__Point_2D_Formula(std::make_shared<__Formula_Base>(x), std::make_shared<__Formula_Base>(y)){};
        __Point_2D_Formula(Point_2D point_2d):__Point_2D_Formula(Fraction::from_double(point_2d.x()),Fraction::from_double(point_2d.y())){};
        __Point_2D_Formula():__Point_2D_Formula(0,0){};
        __Point_2D_Formula(const __Point_2D_Formula& point_copy):__Point_2D_Formula(point_copy.a_x.get()->formula_copy(),point_copy.a_y.get()->formula_copy()){};

        // Returns a copy of the __Point_2D_Formula point
        inline __Point_2D_Formula point_copy() const {return __Point_2D_Formula(a_x.get()->formula_copy(), a_y.get()->formula_copy());};
        // Returns the __Point_2D_Formula to a Point_2D
        inline Point_2D to_point_2d(scls::__Formula_Base::Unknowns_Container* values) const {return Point_2D(a_x.get()->value_to_double(values), a_y.get()->value_to_double(values));};
        // Returns the point to XML text
        std::string to_xml_text();

        //*********
        //
        // __Point_2D_Formula position system
        //
        //*********

        // Move easily the object
        inline void move_xy(Fraction movement_x, Fraction movement_y) {move_x(movement_x);move_y(movement_y);};
        inline void move_x(Fraction movement) {(*a_x.get()) += movement;};
        inline void move_y(Fraction movement) {(*a_y.get()) += movement;};

        // Getters
        inline void set_x(__Formula_Base::Formula new_x) {a_x = new_x.formula_base_shared_ptr();};
        inline void set_x(std::shared_ptr<__Formula_Base> new_x) {a_x = new_x;};
        inline void set_y(__Formula_Base::Formula new_y) {a_y = new_y.formula_base_shared_ptr();};
        inline void set_y(std::shared_ptr<__Formula_Base> new_y) {a_y = new_y;};
        inline __Formula_Base::Formula x() const {return a_x;};
        inline __Formula_Base::Formula y() const {return a_y;};

        // Rotates the formula
        __Point_2D_Formula rotated(double rotation) const;
        void rotate(double rotation);

        //*********
        //
        // __Point_2D_Formula vectorial manipulation
        //
        //*********

        // Adds a vector to this vector with another
        inline void __add(Point_2D value) {(*a_x.get())+=Fraction::from_double(value.x());(*a_y.get())+=Fraction::from_double(value.y());};
        inline void __add(__Point_2D_Formula value) {x()+=value.x();y()+=value.y();};
        inline __Point_2D_Formula __add_without_modification(Point_2D object) const {__Point_2D_Formula to_return = *this;to_return.__add(object);return to_return;};
        // Divides a vector to this vector with another
        inline void __divide(Fraction value) {(*a_x.get())/=value;(*a_y.get())/=value;};
        // Multiplies a vector to this vector with another
        inline void __multiply(Fraction value) {(*a_x.get())*=value;(*a_y.get())*=value;};
        inline void __multiply(Point_2D value) {(*a_x.get())*=Fraction::from_double(value.x());(*a_y.get())*=Fraction::from_double(value.y());};
        inline void __multiply(__Point_2D_Formula value) {(*a_x.get())*=(*value.x().formula_base());(*a_y.get())*=(*value.y().formula_base());};
        // Returns a substraction of this vector with another
        inline void __substract(Point_2D value) {(*a_x.get())-=Fraction::from_double(value.x());(*a_y.get())-=Fraction::from_double(value.y());};
        inline __Point_2D_Formula __substract_without_modification(Point_2D object) const {__Point_2D_Formula to_return = *this;to_return.__substract(object);return to_return;};

        // Built-in operators
        // With __Point_2D_Formula or Point_2D
        inline __Point_2D_Formula operator-(Point_2D object)const{return __substract_without_modification(object);};
        inline __Point_2D_Formula operator+(Point_2D object)const{return __add_without_modification(object);};
        inline __Point_2D_Formula& operator+=(Point_2D object){__add(object);return *this;};
        inline __Point_2D_Formula& operator+=(__Point_2D_Formula object){__add(object);return *this;};
        inline __Point_2D_Formula& operator-=(Point_2D object){__substract(object);return *this;};
        inline __Point_2D_Formula operator*(Point_2D object){__Point_2D_Formula temp = *this;temp.__multiply(object);return temp;};
        inline __Point_2D_Formula operator*(__Point_2D_Formula object){__Point_2D_Formula temp = *this;temp.__multiply(object);return temp;};
        inline __Point_2D_Formula& operator*=(Point_2D object){__multiply(object);return *this;};
        inline bool operator==(Point_2D object){return object.x() == x().value_to_double(0) && object.y() == y().value_to_double(0);};
        // With fraction
        inline __Point_2D_Formula operator/(Fraction object)const{__Point_2D_Formula to_return = *this;to_return.__divide(object);return to_return;};
        inline __Point_2D_Formula operator*(Fraction object)const{__Point_2D_Formula to_return = *this;to_return.__multiply(object);return to_return;};
        inline __Point_2D_Formula& operator/=(Fraction object){__divide(object);return *this;};
        inline __Point_2D_Formula& operator*=(Fraction object){__multiply(object);return *this;};
    private:
        //*********
        //
        // __Point_2D_Formula position system
        //
        //*********

        // X position of the point
        std::shared_ptr<__Formula_Base> a_x = std::make_shared<__Formula_Base>();
        // Y position of the point
        std::shared_ptr<__Formula_Base> a_y = std::make_shared<__Formula_Base>();
    }; typedef __Point_2D_Formula Point_2D_Formula;

    // Returns the datas point of two crossing lines
    struct Crossing_Datas {bool crossed = false;double crossing_x = 0;double crossing_y = 0;bool same_lines = false;};
    Crossing_Datas check_crossing(double first_point_x, double first_point_y, double second_point_x, double second_point_y, double third_point_x, double third_point_y, double fourth_point_x, double fourth_point_y);

    // Returns the datas point of two crossing segments
    struct Crossing_Datas_Segment {bool crossed_in_segment = false;Crossing_Datas crossing_datas;};
    Crossing_Datas_Segment check_crossing_segment(double first_point_x, double first_point_y, double second_point_x, double second_point_y, double third_point_x, double third_point_y, double fourth_point_x, double fourth_point_y, bool check_first_and_second_point = true, bool check_third_and_fourth_point = true);

    // Returns the point of an orthogonal projection of a point on a line
    Point_2D orthogonal_projection(Point_2D point_line_1, Point_2D point_line_2, Point_2D point_to_project);

    //*********
    //
    // The Transform_Object_3D class
    //
    //*********

    // Normalizes a 3D vector
    void normalize_3d(double& vector_x, double& vector_y, double& vector_z);
    void normalize_3d(Fraction& vector_x, Fraction& vector_y, Fraction& vector_z);

    // Returns the angle in radians for a vector 3D
    double vector_2d_angle(__Formula_Base::Formula vector_x, __Formula_Base::Formula vector_y);
    double vector_2d_angle(Point_2D vector);
    // Return the vector created by an angle
    Point_2D vector_2d_with_angle(double angle);

    class __Transform_Object_2D_Base {
        // Class representing the base of a 2D transfort
    public:

        //*********
        //
        // Transform_Object_2D main members
        //
        //*********

        // Transform_Object_2D constructor
        __Transform_Object_2D_Base();

        //*********
        //
        // Transform_Object_2D position system
        //
        //*********

        // Get the absolute position
        Point_2D absolute_position() const;
        virtual double absolute_x() const = 0;
        virtual double absolute_y() const = 0;
        // Get the position
        Point_2D position() const;
        virtual double x() const = 0;
        virtual double y() const = 0;
    };

    class Transform_Object_2D : public __Transform_Object_2D_Base {
        // Class representing a 2D object transform
    public:

        //*********
        //
        // Transform_Object_2D main members
        //
        //*********

        // Transform_Object_2D constructor
        Transform_Object_2D();
        // Copy constructor
        Transform_Object_2D(Point_2D point_2d);
        Transform_Object_2D(__Point_2D_Formula point_2d);
        // Transform_Object_2D destructor
        ~Transform_Object_2D();

        // Soft reset the transform
        inline void soft_reset(){a_moved_during_this_frame = false;};

        // Getters and setters
        inline std::vector<std::shared_ptr<Transform_Object_2D>>& children() {return a_children;};
        inline Fraction delta_time() const {return a_delta_time;};
        inline unsigned int id() const {return a_id;};
        inline bool moved_during_this_frame() const {return a_moved_during_this_frame;};
        inline Transform_Object_2D* parent() const {return a_parent.lock().get();};
        inline void remove_child(Transform_Object_2D* child){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children.at(i).get()==child){a_children.erase(a_children.begin()+i);return;}};};
        inline void set_delta_time(Fraction new_delta_time){a_delta_time = new_delta_time;};
        inline void set_parent(Transform_Object_2D* new_parent){set_parent(new_parent->a_this_object.lock());};
        inline void set_parent(std::shared_ptr<Transform_Object_2D> new_parent) {
            if(parent() != 0){parent()->remove_child(this);}
            a_parent = new_parent;
            parent()->children().push_back(a_this_object.lock());
        };
        inline void set_parent(std::shared_ptr<Transform_Object_2D>* new_parent) {if(new_parent==0) {if(parent() != 0){parent()->remove_child(this);}a_parent.reset();}else{set_parent(*new_parent);}};
        inline void set_this_object(std::weak_ptr<Transform_Object_2D> this_object){a_this_object=this_object;};
        inline void set_unknowns(std::shared_ptr<__Formula_Base::Unknowns_Container> new_unknowns){a_unknowns = new_unknowns;};

        //*********
        //
        // Transform_Object_2D position system
        //
        //*********

        // Absolute position handling
        // Returns the absolute X position
        virtual double absolute_x() const;
        __Formula_Base::Formula absolute_x_formula() const;
        virtual double absolute_y() const;
        __Formula_Base::Formula absolute_y_formula() const;

        // Accelerates the object
        void accelerate(Point_2D_Formula acceleration);
        void accelerate(Point_2D acceleration);
        void accelerate_x(double acceleration);
        void accelerate_y(double acceleration);

        // Adds a value to x / y
        void add_x(double x_to_add);
        void add_y(double y_to_add);

        // Returns the distance from an another object
        double distance(Point_2D point);
        double distance(const Transform_Object_2D& object);

        // Returns if a transform object touches an another
        bool touch(Transform_Object_2D* object);

        // Move easily the object
        void move_x(Fraction movement);
        void move_y(Fraction movement);

        // Extremums
        double max_absolute_x() const;
        double max_absolute_y() const;
        double min_absolute_x() const;
        double min_absolute_y() const;
        double max_x() const;
        double max_y() const;
        double min_x() const;
        double min_y() const;

        // Precise next movement
        double max_absolute_x_next() const;
        double max_absolute_y_next() const;
        double min_absolute_x_next() const;
        double min_absolute_y_next() const;
        Point_2D position_next() const;
        // Next movement generated by the velocity
        double next_movement_x()const;
        double next_movement_y()const;
        // Next position
        double x_next() const;
        double y_next() const;

        // Update the raw velocity
        void update_raw_velocity();

        // Getters and setters
        Point_2D_Formula position_formula() const;
        Point_2D raw_velocity() const;
        void set_position(Fraction new_x, Fraction new_y);
        void set_position(Point_2D_Formula new_position);
        void set_raw_velocity(Point_2D new_raw_velocity);
        virtual void set_x(__Formula_Base::Formula new_x);
        virtual void set_y(__Formula_Base::Formula new_y);
        void set_velocity(Point_2D_Formula new_velocity);
        void set_velocity_x(__Formula_Base::Formula new_x);
        void set_velocity_y(__Formula_Base::Formula new_y);
        Point_2D velocity() const;
        Point_2D_Formula velocity_formula() const;
        double velocity_x() const;
        double velocity_y() const;
        virtual double x() const;
        __Formula_Base::Formula x_formula() const;
        virtual double y() const;
        __Formula_Base::Formula y_formula() const;

        //*********
        //
        // Transform_Object_3D rotation system
        //
        //*********

        // Returns the absolute X rotation of the object
        inline double absolute_rotation() const {if(parent() == 0){return rotation();}return parent()->absolute_rotation() + rotation();};

        // Rotate easily the object
        inline void rotate(Fraction movement){set_rotation(rotation() + movement.to_double());};

        // Returns a vector to an object
        inline Point_2D vector_to(Point_2D object) {Point_2D to_return = position().__substract_without_modification(object);to_return.normalize();return to_return;};
        inline Point_2D vector_to(Transform_Object_2D object) {return vector_to(object.position());};

        // Getters
        inline double rotation() const {return a_rotation.value_to_double(a_unknowns.get());};
        inline __Formula_Base::Formula rotation_formula() const {return a_rotation;};
        inline void set_rotation(double new_rotation){a_rotation = new_rotation;};
        inline void set_rotation(__Fraction_Base new_rotation){a_rotation = new_rotation;};
        inline void set_rotation(Fraction new_rotation){a_rotation = new_rotation;};
        inline void set_rotation(__Formula_Base::Formula new_rotation){a_rotation = new_rotation;};
        inline void set_rotation(__Formula_Base new_rotation){a_rotation = new_rotation;};
        inline void set_rotation(__Formula_Base* new_rotation){a_rotation = new_rotation->formula_copy();};

        //*********
        //
        // Transform_Object_2D scaling system
        //
        //*********

        // Returns the absolute X scale
        inline Point_2D absolute_scale() const {return Point_2D(absolute_scale_x(), absolute_scale_y());};
        inline Point_2D_Formula absolute_scale_formula() const {return Point_2D_Formula(absolute_scale_x_formula(), absolute_scale_y_formula());};
        inline double absolute_scale_x() const {if(parent() == 0){return scale_x();}else return parent()->absolute_scale_x() * scale_x();};
        inline __Formula_Base::Formula absolute_scale_x_formula() const {if(parent() == 0){return scale_x_formula().formula_copy();} return parent()->absolute_scale_x_formula() * scale_x_formula();};
        inline double absolute_scale_y() const {if(parent() == 0){return scale_y();}else return parent()->absolute_scale_y() * scale_y();};
        inline __Formula_Base::Formula absolute_scale_y_formula() const {if(parent() == 0){return scale_y_formula().formula_copy();} return parent()->absolute_scale_y_formula() * scale_y_formula();};

        // Getters
        inline Point_2D scale() const {return a_scale.to_point_2d(a_unknowns.get());};
        inline double scale_x() const {return a_scale.x().value_to_double(a_unknowns.get());};
        inline __Formula_Base::Formula scale_x_formula() const {return a_scale.x();};
        inline double scale_y() const {return a_scale.y().value_to_double(a_unknowns.get());};
        inline __Formula_Base::Formula scale_y_formula() const {return a_scale.y();};
        inline void set_scale(Fraction new_scale_x, Fraction new_scale_y) {set_scale_x(new_scale_x);set_scale_y(new_scale_y);};
        inline void set_scale(Fraction new_scale) {set_scale_x(new_scale);set_scale_y(new_scale);};
        inline void set_scale(Point_2D new_scale) {a_scale = new_scale;};
        inline void set_scale_x(Fraction new_scale_x) {a_scale.set_x(new_scale_x);};
        inline void set_scale_x(__Formula_Base::Formula new_scale_x) {a_scale.set_x(new_scale_x);};
        inline void set_scale_y(Fraction new_scale_y) {a_scale.set_y(new_scale_y);};
        inline void set_scale_y(__Formula_Base::Formula new_scale_y) {a_scale.set_y(new_scale_y);};

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
        Fraction a_delta_time = Fraction(1, 100);
        // If the transform has been moved during this frame
        bool a_moved_during_this_frame = false;

        // Children of the object
        std::vector<std::shared_ptr<Transform_Object_2D>> a_children = std::vector<std::shared_ptr<Transform_Object_2D>>();
        // Parent of the object
        std::weak_ptr<Transform_Object_2D> a_parent;
        // Shared ptr to this object
        std::weak_ptr<Transform_Object_2D> a_this_object;
        // Unknowns for this transform object
        std::shared_ptr<__Formula_Base::Unknowns_Container> a_unknowns;

        //*********
        //
        // Transform_Object_2D position system
        //
        //*********

        // Position of the object
        __Point_2D_Formula a_position;

        // Velocity (and raw velocity) of the object
        Point_2D_Formula a_raw_velocity;
        Point_2D_Formula a_velocity;

        //*********
        //
        // Transform_Object_2D rotation system
        //
        //*********

        // Rotation of the object
        Formula::Formula a_rotation = 0;

        //*********
        //
        // Transform_Object_2D scaling system
        //
        //*********

        // Scale of the object
        __Point_2D_Formula a_scale = __Point_2D_Formula(1, 1);
    };

    //*********
    //
    // Other geometrical features
    //
    //*********

    // Returns the angle (between 0 and Pi) needed for a certain X value
    double circle_angle_at_x(double needed_x, double oval_x, double oval_x_radius);
    double circle_angle_at_x(double needed_x_proportion);
    double oval_angle_at_x(double oval_x, double oval_y, double needed_x);
    // Returns the Y position needed for a certain angle
    double oval_y_at_angle(double oval_x, double oval_y, double needed_angle);

    // Returns the radius of an oval at a certain angle
    double oval_radius(double scale_x, double scale_y, double angle_in_radians);
    double oval_radius(Fraction scale_x, Fraction scale_y, double angle_in_radians);
    double oval_radius_proportion_x(double scale_x, double scale_y, double angle_in_radians);
    double oval_radius_proportion_y(double scale_x, double scale_y, double angle_in_radians);
    Point_2D oval_vector_x(double scale_x, double scale_y, double angle_in_radians);
    Point_2D oval_vector_y(double scale_x, double scale_y, double angle_in_radians);
}

#endif // SCLS_MATH_GEOMETRY_CORE
