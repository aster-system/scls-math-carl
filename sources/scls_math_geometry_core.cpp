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
    double vector_2d_angle(Point_2D vector){return vector_2d_angle(vector.x().to_double(), vector.y().to_double());}
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

    // Returns a point 3D with an angle (on y axis)
    Point_3D vector_with_angle(double angle){return scls::Point_3D(cos(angle), 0, sin(angle));};
    Point_3D vector_with_angle_degrees(double angle){angle *= SCLS_PI/180.0; return scls::Point_3D(cos(angle), 0, sin(angle));};
}
