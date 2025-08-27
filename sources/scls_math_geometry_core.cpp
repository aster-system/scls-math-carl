//******************
//
// scls_math_geometry_core.cpp
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
// This file contains the source code of scls_math_geometry_core.h.
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
#include "../scls_math.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    // Rotate a 3D vector
    double* __rotate_vector_3d(double vector_x, double vector_y, double vector_z, double rotation_x, double rotation_y, double rotation_z, double anchor_x, double anchor_y, double anchor_z) {
        double* to_return = new double[3];
        vector_x -= anchor_x;vector_y -= anchor_y;vector_z -= anchor_z;

        // Calculate the first XZ angle
        double total_xz_length = std::sqrt(vector_x * vector_x + vector_z * vector_z);
        double to_add = 0;
        if(total_xz_length > 0) {
            to_add = std::acos(std::abs(vector_z) / total_xz_length);
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
        }
        else {to_return[2] = 0; to_return[0] = 0;}

        // Calculate the real local Y anchored position
        double total_length = std::sqrt(total_xz_length * total_xz_length + vector_y * vector_y);
        if(total_length > 0 && rotation_x > 0) {
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
        }else{to_return[1] = vector_y;} //*/

        // Scale each vectors
        to_return[2] *= total_length;
        to_return[0] *= total_length;

        to_return[0] += anchor_x;to_return[1] += anchor_y;to_return[2] += anchor_z;
        return to_return;
    }

    // Returns the datas point of two crossing lines
    Crossing_Datas check_crossing(double first_point_x, double first_point_y, double second_point_x, double second_point_y, double third_point_x, double third_point_y, double fourth_point_x, double fourth_point_y) {
        Crossing_Datas to_return;

        // Get the carthesian coordonates of the given points
        // Get some basic informations
        double base_x = (second_point_x - first_point_x);
        double base_y = (second_point_y - first_point_y);
        double line_ratio = base_x / base_y;
        // Get the carthesian coordonates
        double a_part = base_y;
        double b_part = -base_x;
        double c_part = -(a_part * first_point_x + b_part * first_point_y);

        // Check the second sides with carthesian coordonates
        // Get the carthesian coordonates
        double current_base_x = (fourth_point_x - third_point_x);
        double current_base_y = (fourth_point_y - third_point_y);
        double current_line_ratio = current_base_x / current_base_y;
        // Check the value
        if(abs(current_base_y) < SCLS_MATH_TOLERANCE) {
            if(base_y > 0) {
                // Constant horizontal line
                to_return.crossed = true;
                to_return.crossing_x = ((b_part * third_point_y) + c_part) / (-a_part);
                to_return.crossing_y = third_point_y;
            }
            else if(abs(first_point_y - third_point_y) < SCLS_MATH_TOLERANCE) {
                // Same constant horizontal
                to_return.crossed = true;
                to_return.crossing_x = third_point_y;
                to_return.same_lines = true;
            }
        }
        else if(abs(base_y) < SCLS_MATH_TOLERANCE) {
            // Constant horizontal line
            to_return.crossed = true;
            // Get the carthesian coordonates
            double current_a_part = current_base_y;
            double current_b_part = -current_base_x;
            double current_c_part = -(current_a_part * third_point_x + current_b_part * third_point_y);
            to_return.crossing_x = ((current_b_part * first_point_y) + current_c_part) / (-current_a_part);
            to_return.crossing_y = first_point_y;
        }
        else if(abs(current_base_x) < SCLS_MATH_TOLERANCE) {
            if(base_x != 0) {
                // Constant vertical
                to_return.crossed = true;
                to_return.crossing_x = third_point_x;
                to_return.crossing_y = ((a_part * third_point_x) + c_part) / (-b_part);
            }
            else if(abs(first_point_x - third_point_x) < SCLS_MATH_TOLERANCE) {
                // Same constant vertical
                to_return.crossed = true;
                to_return.crossing_x = third_point_x;
                to_return.same_lines = true;
            }
        }
        else if(abs(base_x) < SCLS_MATH_TOLERANCE) {
            // Constant vertical line
            to_return.crossed = true;
            // Get the carthesian coordonates
            double current_a_part = current_base_y;
            double current_b_part = -current_base_x;
            double current_c_part = -(current_a_part * third_point_x + current_b_part * third_point_y);
            to_return.crossing_x = first_point_x;
            to_return.crossing_y = ((current_a_part * first_point_x) + current_c_part) / (-current_b_part);
        }
        else if(current_line_ratio != line_ratio) {
            to_return.crossed = true;
            // Get the carthesian coordonates
            double current_a_part = current_base_y;
            double current_b_part = -current_base_x;
            double current_c_part = -(current_a_part * third_point_x + current_b_part * third_point_y);
            // Resolve the equation for the intersection point
            to_return.crossing_y = (-current_a_part * c_part) + (a_part * current_c_part);
            to_return.crossing_y /= (current_a_part * b_part) - (a_part * current_b_part);
        }

        return to_return;
    };

    // Returns the datas point of two crossing segments
    Crossing_Datas_Segment check_crossing_segment(double first_point_x, double first_point_y, double second_point_x, double second_point_y, double third_point_x, double third_point_y, double fourth_point_x, double fourth_point_y, bool check_first_and_second_point, bool check_third_and_fourth_point) {
        // Get Y about the first segment
        double min_y = second_point_y; double max_y = first_point_y;
        if(min_y > max_y) {min_y = first_point_y; max_y = second_point_y;}
        // Get X about the first segment
        double min_x = second_point_x; double max_x = first_point_x;
        if(min_x > max_x) {min_x = first_point_x; max_x = second_point_x;}

        // Get Y about the second segment
        double current_min_y = fourth_point_y; double current_max_y = third_point_y;
        if(current_min_y > current_max_y) {current_min_y = third_point_y; current_max_y = fourth_point_y;}
        // Get X about the second segment
        double current_min_x = fourth_point_x; double current_max_x = third_point_x;
        if(current_min_x > current_max_x) {current_min_x = third_point_x; current_max_x = fourth_point_x;}

        // Get the crossing datas
        Crossing_Datas_Segment datas;
        datas.crossing_datas = check_crossing(first_point_x, first_point_y, second_point_x, second_point_y, third_point_x, third_point_y, fourth_point_x, fourth_point_y);
        bool on_first_segment = false;
        bool on_second_segment = false;
        // Check the segments
        if(datas.crossing_datas.crossed) {
            if(current_max_y - current_min_y < SCLS_MATH_TOLERANCE && max_y - min_y < SCLS_MATH_TOLERANCE) {
                // Two horizontal lines
                if(datas.crossing_datas.crossing_x - current_min_x >= 0 && current_max_x - datas.crossing_datas.crossing_x >= 0) on_first_segment = true;
                if(datas.crossing_datas.crossing_x - min_x >= 0 && max_x - datas.crossing_datas.crossing_x >= 0) on_second_segment = true;
            }
            else if(current_max_y - current_min_y < SCLS_MATH_TOLERANCE) {
                // Horizontal lines
                if(datas.crossing_datas.crossing_x - current_min_x >= 0 && current_max_x - datas.crossing_datas.crossing_x >= 0) on_first_segment = true;
                if(datas.crossing_datas.crossing_y - min_y >= 0 && max_y - datas.crossing_datas.crossing_y >= 0) on_second_segment = true;
            }
            else if(max_y - min_y < SCLS_MATH_TOLERANCE) {
                // Horizontal lines
                if(datas.crossing_datas.crossing_y - current_min_y >= 0 && current_max_y - datas.crossing_datas.crossing_y >= 0) on_first_segment = true;
                if(datas.crossing_datas.crossing_x - min_x >= 0 && max_x - datas.crossing_datas.crossing_x >= 0) on_second_segment = true;
            }
            else {
                // No horizontal lines
                if(datas.crossing_datas.crossing_y - current_min_y >= 0 && current_max_y - datas.crossing_datas.crossing_y >= 0) on_first_segment = true;
                if(datas.crossing_datas.crossing_y - min_y >= 0 && max_y - datas.crossing_datas.crossing_y >= 0) on_second_segment = true;
            }
        }

        datas.crossed_in_segment = (on_first_segment || !check_third_and_fourth_point) && (on_second_segment || !check_first_and_second_point);
        return datas;
    };

    // Returns the point of an orthogonal projection of a point on a line
    Point_2D orthogonal_projection(Point_2D point_line_1, Point_2D point_line_2, Point_2D point_to_project) {
        // Fast part
        if(point_line_1.x() == point_line_2.x()){return Point_2D(point_line_1.x(), point_to_project.y());}

        scls::Point_2D line_vector = point_line_2 - point_line_1;
        double line_vector_angle = scls::vector_2d_angle(line_vector);
        scls::Point_2D position_to_vector = point_to_project - point_line_1;
        double position_to_vector_angle = scls::vector_2d_angle(position_to_vector);
        double angle = position_to_vector_angle - line_vector_angle;

        // Get the contact point
        scls::Point_2D contact_point = point_line_1 + line_vector.normalized() * std::cos(angle) * position_to_vector.norm();
        return contact_point;
    }

    // Returns if the point is in a rect
    bool Point_2D::in_rect(Point_2D tested_pos, Point_2D tested_scale){return in_rect(tested_pos.x(), tested_pos.y(), tested_scale.x(), tested_scale.y());}
    bool Point_2D::in_rect(double tested_x, double tested_y, double tested_width, double tested_height){if(tested_height<0){tested_y+=tested_height;tested_height*=-1;}if(tested_width<0){tested_x+=tested_width;tested_width*=-1;}return (tested_x - x()) < SCLS_MATH_TOLERANCE && (tested_y - y())  < SCLS_MATH_TOLERANCE && (x() - (tested_x + tested_width)) < SCLS_MATH_TOLERANCE && (y() - (tested_y + tested_height)) < SCLS_MATH_TOLERANCE;}

    // Returns the point to XML text
    std::string __Point_2D_Formula::to_xml_text(){std::string needed_x = x().to_std_string(0);if(needed_x==std::string()){needed_x=std::string("0");}std::string needed_y = y().to_std_string(0);if(needed_y==std::string()){needed_y=std::string("0");} return std::string("(") + needed_x + std::string(",") + needed_y + std::string(")");}

    // Operators
    bool operator==(const Point_2D object_1, const Point_2D object_2){return object_1.x() == object_2.x() && object_1.y() == object_2.y();};

    //*********
    //
    // The Transform_Object_2D class
    //
    //*********

    // Transform_Object_2D constructor
    __Transform_Object_2D_Base::__Transform_Object_2D_Base(){};

    // Position system
    Point_2D __Transform_Object_2D_Base::absolute_position() const{return Point_2D(absolute_x(), absolute_y());}
    Point_2D __Transform_Object_2D_Base::position() const{return Point_2D(x(), y());}

    // Transform_Object_2D constructor
    Transform_Object_2D::Transform_Object_2D():__Transform_Object_2D_Base(){};
    // Copy constructor
    Transform_Object_2D::Transform_Object_2D(Point_2D point_2d):Transform_Object_2D(__Point_2D_Formula(point_2d)){};
    Transform_Object_2D::Transform_Object_2D(__Point_2D_Formula point_2d):a_position(point_2d){};
    // Transform_Object_2D destructor
    Transform_Object_2D::~Transform_Object_2D(){if(parent() != 0){parent()->remove_child(this);}};

    // Absolute position handling
    // Returns the absolute X position
    double Transform_Object_2D::absolute_x() const {
        if(parent() == 0){return x();}
        Point_2D current_position = position() * parent()->absolute_scale();
        if(parent()->absolute_rotation() != 0){current_position.rotate(parent()->absolute_rotation());}
        return parent()->absolute_x() + current_position.x();
    }
    __Formula_Base::Formula Transform_Object_2D::absolute_x_formula() const {
        if(parent() == 0){return x_formula().formula_copy();}
        Point_2D_Formula current_position = position_formula() * parent()->absolute_scale_formula();
        //if(parent()->absolute_rotation().to_double() != 0){current_position.rotate(parent()->absolute_rotation().to_double());}
        return parent()->absolute_x_formula() + current_position.x();
    }
    double Transform_Object_2D::absolute_y() const {
        if(parent() == 0){return y();}
        Point_2D current_position = position() * parent()->absolute_scale();
        if(parent()->absolute_rotation() != 0){current_position.rotate(parent()->absolute_rotation());}
        return parent()->absolute_y() + current_position.y();
    }
    __Formula_Base::Formula Transform_Object_2D::absolute_y_formula() const {
        if(parent() == 0){return y_formula().formula_copy();}
        Point_2D_Formula current_position = position_formula() * parent()->absolute_scale_formula();
        //if(parent()->absolute_rotation().to_double() != 0){current_position.rotate(parent()->absolute_rotation().to_double());}
        return parent()->absolute_y_formula() + current_position.y();
    }

    // Accelerates the object
    void Transform_Object_2D::accelerate(Point_2D_Formula acceleration){a_velocity += acceleration;};
    void Transform_Object_2D::accelerate(Point_2D acceleration){a_velocity += Point_2D_Formula(acceleration);};
    void Transform_Object_2D::accelerate_x(double acceleration){a_velocity.move_x(acceleration);};
    void Transform_Object_2D::accelerate_y(double acceleration){a_velocity.move_y(acceleration);};

    // Adds a value to x / y
    void Transform_Object_2D::add_x(double x_to_add){a_position.move_x(x_to_add);};
    void Transform_Object_2D::add_y(double y_to_add){a_position.move_y(y_to_add);};

    // Returns the distance from an another object
    double Transform_Object_2D::distance(Point_2D point) {return std::sqrt((pow(point.x() - absolute_x(), 2) + pow(point.y() - absolute_y(), 2))); };
    double Transform_Object_2D::distance(const Transform_Object_2D& object) {return std::sqrt((pow((object.absolute_x() - absolute_x()), 2) + pow((object.absolute_y() - absolute_y()), 2)));};

    // Returns if a transform object touches an another
    bool Transform_Object_2D::touch(Transform_Object_2D* object){return !((min_x() > object->max_x() || object->min_x() > max_x()) || (min_y() > object->max_y() || object->min_y() > max_y()));}

    // Move easily the object
    void Transform_Object_2D::move_x(Fraction movement) {set_x(x_formula() + movement);};
    void Transform_Object_2D::move_y(Fraction movement) {set_y(y_formula() + movement);};

    // Extremums
    double Transform_Object_2D::max_absolute_x() const {return absolute_x() + absolute_scale_x() / 2;};
    double Transform_Object_2D::max_absolute_y() const {return absolute_y() + absolute_scale_y() / 2;};
    double Transform_Object_2D::min_absolute_x() const {return absolute_x() - absolute_scale_x() / 2;};
    double Transform_Object_2D::min_absolute_y() const {return absolute_y() - absolute_scale_y() / 2;};
    double Transform_Object_2D::max_x() const {return x() + scale_x() / 2;};
    double Transform_Object_2D::max_y() const {return y() + scale_y() / 2;};
    double Transform_Object_2D::min_x() const {return x() - scale_x() / 2;};
    double Transform_Object_2D::min_y() const {return y() - scale_y() / 2;};

    // Precise next movement
    double Transform_Object_2D::max_x_next() const {return max_x() + next_movement_x();};
    double Transform_Object_2D::max_y_next() const {return max_y() + next_movement_y();};
    double Transform_Object_2D::min_x_next() const {return min_x() + next_movement_x();};
    double Transform_Object_2D::min_y_next() const {return min_y() + next_movement_y();};
    Point_2D Transform_Object_2D::position_next() const {return Point_2D(x() + next_movement_x(), y() + next_movement_y());};
    // Next movement generated by the velocity
    double Transform_Object_2D::next_movement_x()const{return velocity().x() * a_delta_time.to_double();};
    double Transform_Object_2D::next_movement_y()const{return velocity().y() * a_delta_time.to_double();};
    // Next position
    double Transform_Object_2D::x_next() const {return x() + next_movement_x();};
    double Transform_Object_2D::y_next() const {return y() + next_movement_y();};

    // Update the raw velocity
    void Transform_Object_2D::update_raw_velocity(){a_raw_velocity = a_velocity;};

    // Getters and setters
    Point_2D_Formula Transform_Object_2D::position_formula() const{return a_position;}
    Point_2D Transform_Object_2D::raw_velocity() const {return a_raw_velocity.to_point_2d(a_unknowns.get());}
    void Transform_Object_2D::set_position(scls::Fraction new_x, scls::Fraction new_y){a_position.set_x(new_x);a_position.set_y(new_y);a_moved_during_this_frame = true;}
    void Transform_Object_2D::set_position(Point_2D_Formula new_position){a_position.set_x(new_position.x());a_position.set_y(new_position.y());a_moved_during_this_frame = true;}
    void Transform_Object_2D::set_raw_velocity(Point_2D new_raw_velocity){a_raw_velocity.set_x(new_raw_velocity.x());a_raw_velocity.set_y(new_raw_velocity.y());}
    void Transform_Object_2D::set_x(scls::__Formula_Base::Formula new_x){a_position.set_x(new_x);a_moved_during_this_frame = true;}
    void Transform_Object_2D::set_y(scls::__Formula_Base::Formula new_y){a_position.set_y(new_y);a_moved_during_this_frame = true;}
    void Transform_Object_2D::set_velocity(Point_2D_Formula new_velocity){a_velocity.set_x(new_velocity.x());a_velocity.set_y(new_velocity.y());}
    void Transform_Object_2D::set_velocity_x(__Formula_Base::Formula new_x){a_velocity.set_x(new_x);}
    void Transform_Object_2D::set_velocity_y(__Formula_Base::Formula new_y){a_velocity.set_y(new_y);}
    Point_2D Transform_Object_2D::velocity() const {return a_velocity.to_point_2d(a_unknowns.get());}
    Point_2D_Formula Transform_Object_2D::velocity_formula() const{return a_velocity;}
    double Transform_Object_2D::velocity_x() const {return velocity().x();}
    double Transform_Object_2D::velocity_y() const {return velocity().y();}
    double Transform_Object_2D::x() const {return a_position.x().value_to_double(a_unknowns.get());}
    scls::__Formula_Base::Formula Transform_Object_2D::x_formula() const{return a_position.x();}
    double Transform_Object_2D::y() const {return a_position.y().value_to_double(a_unknowns.get());}
    scls::__Formula_Base::Formula Transform_Object_2D::y_formula() const{return a_position.y();}

    //*********
    //
    // The Transform_Object_3D class
    //
    //*********

    // Normalizes a 3D vector
    void normalize_3d(double& vector_x, double& vector_y, double& vector_z) {
        double distance_xz = vector_x * vector_x + vector_z * vector_z;
        double distance = std::sqrt(distance_xz + vector_y * vector_y);

        // Calculate the proportionality constant
        double divisor = 1.0 / distance;
        divisor = sqrt(divisor);
        vector_x *= divisor;
        vector_y *= divisor;
        vector_z *= divisor;
    };
    void normalize_3d(scls::Fraction& vector_x, scls::Fraction& vector_y, scls::Fraction& vector_z) {
        double distance_xz = (vector_x.to_double() * vector_x.to_double() + vector_z.to_double() * vector_z.to_double());
        double distance = std::sqrt(distance_xz + vector_y.to_double() * vector_y.to_double());

        // Calculate the proportionality constant
        double divisor_double = 1.0 / distance;divisor_double = sqrt(divisor_double);
        scls::Fraction divisor = scls::Fraction::from_double(sqrt(divisor_double));
        vector_x *= divisor;
        vector_y *= divisor;
        vector_z *= divisor;
    }

    // Returns the angle in radians for a vector 3D
    double vector_2d_angle(Point_2D needed_vector){return vector_2d_angle(needed_vector.x(), needed_vector.y());}
    double vector_2d_angle(double vector_x, double vector_y) {
        // Calculate the first XZ angle
        double total_length = std::sqrt(vector_x * vector_x + vector_y * vector_y);
        double to_add = 0;
        if(total_length > 0){to_add = std::asin(std::abs(vector_y) / total_length);}
        // Get the current angle
        double current_angle = 0;
        if(vector_y >= 0 && vector_x >= 0) {current_angle = to_add;}
        else if (vector_x < 0 && vector_y >= 0) {current_angle = 3.1415 - to_add;}
        else if(vector_x < 0 && vector_y < 0) {current_angle = 3.1415 + to_add;}
        else {current_angle = 3.1415 * 2.0 - to_add;}

        return current_angle;
    }
    // Return the vector created by an angle
    Point_2D vector_2d_with_angle(double angle){return Point_2D(std::cos(angle), std::sin(angle));}

    // Returns a point 3D with an angle (on y axis)
    Point_3D vector_with_angle(double angle){return scls::Point_3D(cos(angle), 0, sin(angle));};
    Point_3D vector_with_angle_degrees(double angle){angle *= SCLS_PI/180.0; return scls::Point_3D(cos(angle), 0, sin(angle));};

    //*********
    //
    // Other geometrical features
    //
    //*********

    // Returns the angle needed for a certain X value
    double circle_angle_at_x(double needed_x, double oval_x, double oval_x_radius){return std::acos((needed_x - oval_x) / oval_x_radius);}
    double circle_angle_at_x(double needed_x){return std::acos(needed_x);}
    double oval_angle_at_x(double oval_x, double oval_y, double needed_x){double circle_angle = circle_angle_at_x(needed_x / oval_x);return circle_angle * oval_radius_proportion_x(oval_x, oval_y, circle_angle);}
    double oval_angle_at_y(double needed_y){return std::asin(needed_y);}
    // Returns the Y position needed for a certain angle
    double oval_y_at_angle(double oval_x, double oval_y, double needed_angle){
        return std::sin(needed_angle) * oval_y;
    };

    // Returns the radius of an oval at a certain angle
    double oval_radius_proportion_x(double scale_x, double scale_y, double angle_in_radians) {return scale_x / oval_radius(scale_x, scale_y, angle_in_radians);}
    double oval_radius_proportion_y(double scale_x, double scale_y, double angle_in_radians) {return scale_y / oval_radius(scale_x, scale_y, angle_in_radians);}
    double oval_radius(double scale_x, double scale_y, double angle_in_radians) {return std::sqrt(std::pow(std::cos(angle_in_radians) * scale_x, 2) + std::pow(std::sin(angle_in_radians) * scale_y, 2));}
    double oval_radius(Fraction scale_x, Fraction scale_y, double angle_in_radians) {return oval_radius(scale_x.to_double(), scale_y.to_double(), angle_in_radians);}
    Point_2D oval_vector_x(double scale_x, double scale_y, double angle_in_radians){Point_2D direction = vector_2d_with_angle(angle_in_radians);return Point_2D(direction.x(), direction.y() * (scale_y / scale_x));}
    Point_2D oval_vector_y(double scale_x, double scale_y, double angle_in_radians){Point_2D direction = vector_2d_with_angle(angle_in_radians);return Point_2D(direction.x() * (scale_x / scale_y), direction.y());}
}
