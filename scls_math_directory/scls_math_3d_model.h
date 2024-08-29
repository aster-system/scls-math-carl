//******************
//
// scls_math_fraction.h
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
// This file contains some features to handle 3D models.
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

#ifndef SCLS_MATH_3D_MODEL
#define SCLS_MATH_3D_MODEL

#define SCLS_MATH_TOLERANCE 0.00000001

#include "scls_math_3d_core.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    namespace model_maker {
        //*********
        //
        // The __Point_Datas_By_Face and Point class
        //
        //*********

        struct __Point_Datas_By_Face {
            // Datas of a point according to a face

            // Each attributes of the point
            std::map<unsigned int, double> attributes = std::map<unsigned int, double>();
            // Id of the attached face
            unsigned int face_id = 0;
        };

        class Point : public Transform_Object_3D {
            // Point in a model
        public:

            // Possibles way to draw a VBO, built-in
            enum _VBO_Types {
                // Normal VBO
                _VT_Normal,
                // Map VBO
                _VT_Map
            };

            //*********
            //
            // Point mains members
            //
            //*********

            // Point constructor
            Point() : Transform_Object_3D() { reset(); };
            // Point copy constructor
            Point(const Point& copy_point) : Transform_Object_3D(copy_point) {
                a_id = copy_point.a_id;
                a_faces_datas = copy_point.a_faces_datas;
            };

            // Returns the point as a binary
            std::shared_ptr<Bytes_Set> binary() {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the ID
                to_return.get()->add_uint(id(), true);
                // Add the positions
                to_return.get()->add_double(x(), true);
                to_return.get()->add_double(y(), true);
                to_return.get()->add_double(z(), true);
                if(parent() == 0) to_return.get()->add_uint(0, true);
                else to_return.get()->add_uint(parent()->id() + 1, true);
                // Add the others datas
                to_return.get()->add_uint(attributes().size(), true);
                for(std::map<unsigned int, double>::iterator it = attributes().begin();it!=attributes().end();it++) {
                    to_return.get()->add_uint(it->first, true);
                    to_return.get()->add_double(it->second, true);
                }

                return to_return;
            };
            // Returns the point as a binary for face
            std::shared_ptr<Bytes_Set> binary_for_face(unsigned int asker_id){
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the id
                to_return.get()->add_uint(id(), true);
                // Add the texture positions
                std::map<unsigned int, double>& attributes = face_datas(asker_id).attributes;
                to_return.get()->add_uint(attributes.size(), true);
                for(std::map<unsigned int, double>::iterator it = attributes.begin();it!=attributes.end();it++) {
                    to_return.get()->add_uint(it->first, true);
                    to_return.get()->add_double(it->second, true);
                }

                return to_return;
            };
            // Returns the point as a binary STL
            std::shared_ptr<Bytes_Set> binary_stl() {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the positions
                to_return.get()->add_float(absolute_x());
                to_return.get()->add_float(absolute_y());
                to_return.get()->add_float(absolute_z());

                return to_return;
            };
            // Returns the point as binary VBO
            std::shared_ptr<Bytes_Set> binary_vbo(unsigned int asker_id, double texture_rect_x, double texture_rect_y, double texture_rect_width, double texture_rect_height, _VBO_Types vbo_type = Point::_VT_Normal) {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the positions
                to_return.get()->add_double(absolute_inner_x());
                to_return.get()->add_double(absolute_inner_y());
                to_return.get()->add_double(absolute_inner_z());

                // Add the texture positions
                to_return.get()->add_double(texture_x(asker_id));
                to_return.get()->add_double(texture_y(asker_id));

                if(vbo_type == _VBO_Types::_VT_Normal) {
                    // Add the texture rect
                    to_return.get()->add_double(texture_rect_x);
                    to_return.get()->add_double(texture_rect_y);
                    to_return.get()->add_double(texture_rect_width);
                    to_return.get()->add_double(texture_rect_height);

                    // Add the texture multiplier
                    to_return.get()->add_double(texture_multiplier_x(asker_id));
                    to_return.get()->add_double(texture_multiplier_y(asker_id));
                    to_return.get()->add_double(texture_multiplier_z(asker_id));
                }

                return to_return;
            };

            // Resets the point
            void reset() { a_id = a_points_number; a_points_number++; };

            // Number of points created
            static int a_points_number;

            //*********
            //
            // Point faces datas
            //
            //*********

            // Returns if the point contains datas by id
            inline bool contains_face_datas(unsigned int id_to_check) const {
                for(int i = 0;i<static_cast<int>(a_faces_datas.size());i++) { if(a_faces_datas.at(i).face_id == id_to_check) return true; }
                return false;
            };
            // Returns a pointer to some datas, and create them if necessary
            inline __Point_Datas_By_Face& face_datas(unsigned int face_id) {
                for(int i = 0;i<static_cast<int>(a_faces_datas.size());i++) { if(a_faces_datas[i].face_id == face_id) return a_faces_datas[i]; }
                __Point_Datas_By_Face new_datas; new_datas.face_id = face_id;
                a_faces_datas.push_back(new_datas);
                return a_faces_datas[a_faces_datas.size() - 1];
            };

            // Getters
            inline std::vector<__Point_Datas_By_Face>& faces_datas() {return a_faces_datas;};

            //*********
            //
            // Point texture handling
            //
            //*********

            // Getters and setters
            inline std::map<unsigned int, double>& attributes() {return a_attributes;};
            inline void set_attribute(unsigned int attribute_name, double attribute_value) { attributes()[attribute_name] = attribute_value; };
            inline void set_face_attribute(unsigned int asker_id, unsigned int attribute_name, double attribute_value) { face_datas(asker_id).attributes[attribute_name] = attribute_value; };
            inline void set_texture_multiplier_x(unsigned int asker_id, double new_texture_multiplier_x) { face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_MULTIPLIER_X] = new_texture_multiplier_x; };
            inline void set_texture_multiplier_y(unsigned int asker_id, double new_texture_multiplier_y) { face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_MULTIPLIER_Y] = new_texture_multiplier_y; };
            inline void set_texture_multiplier_z(unsigned int asker_id, double new_texture_multiplier_z) { face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_MULTIPLIER_Z] = new_texture_multiplier_z; };
            inline void set_texture_x(unsigned int asker_id, double new_texture_x) { face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_X] = new_texture_x; };
            inline void set_texture_y(unsigned int asker_id, double new_texture_y) { face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_Y] = new_texture_y; };
            inline double texture_multiplier_x(unsigned int asker_id) {return face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_MULTIPLIER_X];};
            inline double texture_multiplier_y(unsigned int asker_id) {return face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_MULTIPLIER_Y];};
            inline double texture_multiplier_z(unsigned int asker_id) {return face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_MULTIPLIER_Z];};
            inline double texture_x(unsigned int asker_id) {return face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_X];};
            inline double texture_y(unsigned int asker_id) {return face_datas(asker_id).attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_Y];};

        private:

            //*********
            //
            // Point faces datas
            //
            //*********

            // Each attributes of the point
            std::map<unsigned int, double> a_attributes = std::map<unsigned int, double>();
            // Each datas, sorted by id
            std::vector<__Point_Datas_By_Face> a_faces_datas = std::vector<__Point_Datas_By_Face>();
        };

        //*********
        //
        // The Face class
        //
        //*********

        // Returns the datas point of two crossing lines
        struct __Crossing_Datas {bool crossed = false;double crossing_x = 0;double crossing_y = 0;bool same_lines = false;};
        static __Crossing_Datas __check_crossing(double first_point_x, double first_point_y, double second_point_x, double second_point_y, double third_point_x, double third_point_y, double fourth_point_x, double fourth_point_y) {
            __Crossing_Datas to_return;

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
            if(current_base_y == 0) {
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
            else if(base_y == 0) {
                // Constant horizontal line
                to_return.crossed = true;
                // Get the carthesian coordonates
                double current_a_part = current_base_y;
                double current_b_part = -current_base_x;
                double current_c_part = -(current_a_part * third_point_x + current_b_part * third_point_y);
                to_return.crossing_x = ((current_b_part * first_point_y) + current_c_part) / (-current_a_part);
                to_return.crossing_y = first_point_y;
            }
            else if(current_base_x == 0) {
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
            else if(base_x == 0) {
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
        static __Crossing_Datas __check_crossing(const Point* first_point, const Point* second_point, const Point* third_point, const Point* fourth_point) {
            return __check_crossing(first_point->x(), first_point->z(), second_point->x(), second_point->z(), third_point->x(), third_point->z(), fourth_point->x(), fourth_point->z());
        };
        // Returns the datas point of two crossing segments
        struct __Crossing_Datas_Segment {bool crossed_in_segment = false;__Crossing_Datas crossing_datas;};
        static __Crossing_Datas_Segment __check_crossing_segment(double first_point_x, double first_point_y, double second_point_x, double second_point_y, double third_point_x, double third_point_y, double fourth_point_x, double fourth_point_y, bool check_first_and_second_point = true, bool check_third_and_fourth_point = true) {
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
            __Crossing_Datas_Segment datas;
            datas.crossing_datas = __check_crossing(first_point_x, first_point_y, second_point_x, second_point_y, third_point_x, third_point_y, fourth_point_x, fourth_point_y);
            //print("Informations type 1", "Math", std::to_string(current_min_y) + " < " + std::to_string(current_max_y) + " & " + std::to_string(min_y) + " < " + std::to_string(max_y) + " -> " + std::to_string(datas.crossing_datas.crossing_y));
            bool on_first_segment = false;
            bool on_second_segment = false;
            // Check the segments
            if(datas.crossing_datas.crossed) {
                if(current_max_y - current_min_y < SCLS_MATH_TOLERANCE) {
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
        static __Crossing_Datas_Segment __check_crossing_segment(const Point* first_point, const Point* second_point, const Point* third_point, const Point* fourth_point, bool check_first_and_second_point = true, bool check_third_and_fourth_point = true) {
            return __check_crossing_segment(first_point->x(), first_point->z(), second_point->x(), second_point->z(), third_point->x(), third_point->z(), fourth_point->x(), fourth_point->z(), check_first_and_second_point, check_third_and_fourth_point);
        };
        static __Crossing_Datas_Segment __check_crossing_segment(const std::shared_ptr<Point>& first_point, const std::shared_ptr<Point>& second_point, const std::shared_ptr<Point>& third_point, const std::shared_ptr<Point>& fourth_point, bool check_first_and_second_point = true, bool check_third_and_fourth_point = true) {
            return __check_crossing_segment(first_point.get(), second_point.get(), third_point.get(), fourth_point.get(), check_first_and_second_point, check_third_and_fourth_point);
        };
        // Returns if a point is in a set of point
        static bool __check_shape_content(const std::vector<std::shared_ptr<Point>>& points, double point_to_test_x, double point_to_test_y, bool last_point_is_first_point = true) {
            // Check each sides
            __Crossing_Datas_Segment datas;
            unsigned int top_collision = 0;
            for(int i = 0;i<static_cast<int>(points.size() - 1);i++) {
                datas = __check_crossing_segment(point_to_test_x, point_to_test_y, point_to_test_x, point_to_test_y + 1.0, points.at(i).get()->x(), points.at(i).get()->z(), points.at(i + 1).get()->x(), points.at(i + 1).get()->z(), false, true);
                if(datas.crossed_in_segment) {
                    if(datas.crossing_datas.crossing_y >= point_to_test_y) top_collision++;
                }
            }
            // Check the last border
            if(last_point_is_first_point && static_cast<int>(points.size()) > 1) {
                datas = __check_crossing_segment(point_to_test_x, point_to_test_y, point_to_test_x, point_to_test_y + 1.0, points[points.size() - 1].get()->x(), points[points.size() - 1].get()->z(), points[0].get()->x(), points[0].get()->z(), false, true);
                if(datas.crossed_in_segment) {
                    if(datas.crossing_datas.crossing_y >= point_to_test_y) top_collision++;
                }
            }
            return top_collision % 2 == 1;
        };
        static bool __check_shape_content(const std::vector<std::shared_ptr<Point>>& points, Point* point_to_test) {return __check_shape_content(points, point_to_test->x(), point_to_test->z());};
        static bool __check_shape_content(const std::vector<std::shared_ptr<Point>>& points, std::shared_ptr<Point> point_to_test) {return __check_shape_content(points, point_to_test.get());};
        // Check if two triangles are opposed to their sharing side or not
        static bool __check_triangle_opposed(const Point* first_point, const Point* second_point, const Point* first_top, const Point* second_top) {
            // Get the crossing datas
            __Crossing_Datas_Segment datas = __check_crossing_segment(first_point, second_point, first_top, second_top, true, false);
            return datas.crossed_in_segment;
        };
        static bool __check_triangle_opposed(const std::shared_ptr<Point>& first_point, const std::shared_ptr<Point>& second_point, const std::shared_ptr<Point>& third_point, const std::shared_ptr<Point>& fourth_point) {
            return __check_triangle_opposed(first_point.get(), second_point.get(), third_point.get(), fourth_point.get());
        };

        // Returns if a side of points cross the boundary of a set of points or not
        static bool __cross_boundary(const std::vector<std::shared_ptr<Point>>& points, std::shared_ptr<Point> first_point, std::shared_ptr<Point> second_point, bool last_point_is_first_point = true) {
            // Check each sides
            for(int i = 0;i<static_cast<int>(points.size() - 1);i++) {
                if(first_point.get() == points.at(i).get() && second_point.get() == points.at(i + 1).get()) { return true; }
                else if(first_point.get() == points.at(i + 1).get() && second_point.get() == points.at(i).get()) { return true; }
                else if(first_point.get() != points.at(i).get() && first_point.get() != points.at(i + 1).get() && second_point.get() != points.at(i).get() && second_point.get() != points.at(i + 1).get()) {
                    // Check each sides with carthesian coordonates
                    //print("Informations type 0", "Math", std::to_string(i));
                    __Crossing_Datas_Segment datas = __check_crossing_segment(first_point.get(), second_point.get(), points.at(i).get(), points.at(i + 1).get());
                    if(datas.crossed_in_segment) return true;
                }
            }
            // Check the last points
            if(last_point_is_first_point && static_cast<int>(points.size()) > 1) {
                if(first_point.get() == points.at(points.size() - 1).get() && second_point.get() == points.at(0).get()) { return true; }
                else if(first_point.get() == points.at(0).get() && second_point.get() == points.at(points.size() - 1).get()) { return true; }
                else if(first_point.get() != points.at(0).get() && first_point.get() != points.at(points.size() - 1).get() && second_point.get() != points.at(0).get() && second_point.get() != points.at(points.size() - 1).get()) {
                    //print("Informations type Final", "Math", "Final");
                    if(__check_crossing_segment(first_point.get(), second_point.get(), points.at(points.size() - 1).get(), points.at(0).get()).crossed_in_segment) return true;
                }
            }
            return false;
        }

        struct __Face_Datas_By_Solid {
            // Datas of a point according to a face

            // Id of the attached face
            unsigned int solid_id = 0;

            //*********
            //
            // Point texture handling
            //
            //*********

            // Rect height position of the texture
            double texture_rect_height = 1;
            // Rect X position of the texture
            double texture_rect_x = 0;
            // Rect Y position of the texture
            double texture_rect_y = 0;
            // Rect width position of the texture
            double texture_rect_width = 1;
        };

        class Face : public Transform_Object_3D {
            // Face in a model
        public:

            //*********
            //
            // Face mains members
            //
            //*********

            // Face constructor
            Face() : Transform_Object_3D() { a_id = a_faces_number; a_faces_number++; set_can_print(false); };

            // Returns the face as a binary
            std::shared_ptr<Bytes_Set> binary() {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the ID of the face
                to_return.get()->add_uint(id(), true);
                // Add the position of the face
                to_return.get()->add_double(x(), true);
                to_return.get()->add_double(y(), true);
                to_return.get()->add_double(z(), true);
                to_return.get()->add_double(scale_x(), true);
                to_return.get()->add_double(scale_y(), true);
                to_return.get()->add_double(scale_z(), true);
                // Add the parent of the face
                if(parent() == 0) to_return.get()->add_uint(0, true);
                else to_return.get()->add_uint(parent()->id() + 1, true);
                // Add the number of points
                to_return.get()->add_uint(static_cast<unsigned int>(a_points.size()), true);
                // Add each points
                for(int i = 0;i<static_cast<int>(a_points.size());i++) {
                    to_return.get()->add_datas(*a_points.at(i)->binary_for_face(id()).get());
                }

                return to_return;
            };
            // Returns the face as a binary for a solid
            std::shared_ptr<Bytes_Set> binary_for_solid(unsigned int asker_id) {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the positions
                to_return.get()->add_uint(id(), true);
                // Add the rect of the texture
                to_return.get()->add_double(texture_rect_x(asker_id), true);
                to_return.get()->add_double(texture_rect_y(asker_id), true);
                to_return.get()->add_double(texture_rect_width(asker_id), true);
                to_return.get()->add_double(texture_rect_height(asker_id), true);

                return to_return;
            };
            // Returns the face as a binary STL format
            std::shared_ptr<Bytes_Set> binary_stl(unsigned int& triangle_count) {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add each points
                for(int i = 0;i<static_cast<int>(a_points_for_rendering.size());i++) {
                    if(i % 3 == 0) {
                        if(i > 0) {
                            to_return.get()->add_ushort(triangle_count);
                            triangle_count++;
                        }

                        // Add the normal
                        to_return.get()->add_float(normal_x());
                        to_return.get()->add_float(normal_y());
                        to_return.get()->add_float(normal_z());
                    }

                    to_return.get()->add_datas(*a_points_for_rendering.at(i).get()->binary_stl().get());
                }

                if(static_cast<int>(a_points_for_rendering.size()) > 0) {
                    to_return.get()->add_ushort(triangle_count);
                    triangle_count++;
                }

                return to_return;
            };
            // Returns the face as binary for VBO
            std::shared_ptr<Bytes_Set> binary_vbo(unsigned int asker_id, unsigned int& total_points, bool argument = true, Point::_VBO_Types vbo_type = Point::_VT_Normal) {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the needed shaders argument
                if(argument) {
                    unsigned int argument_size = 12;
                    to_return.get()->add_data(4);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(3, true);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(2, true);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(4, true);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(3, true);
                    to_return.get()->add_uint(argument_size * a_points_for_rendering.size(), true);
                }

                // Add each points
                __Face_Datas_By_Solid& datas = solid_datas(asker_id);
                for(int i = 0;i<static_cast<int>(a_points_for_rendering.size());i++) {
                    Point* current_point = a_points_for_rendering.at(i).get();
                    to_return.get()->add_datas(*current_point->binary_vbo(id(), datas.texture_rect_x, datas.texture_rect_y, datas.texture_rect_width, datas.texture_rect_height, vbo_type).get());
                    total_points++;
                }

                return to_return;
            };

            // Create a copy of the face with new points
            std::shared_ptr<Face> copy_with_new_points() {
                std::shared_ptr<Face> new_face = std::make_shared<Face>();
                new_face.get()->set_scale_x(scale_x());
                new_face.get()->set_scale_y(scale_y());
                new_face.get()->set_scale_z(scale_z());
                new_face.get()->set_x(x());
                new_face.get()->set_y(y());
                new_face.get()->set_z(z());

                // Create each exclusion points
                for(int i = 0;i<static_cast<int>(a_exclusion_points.size());i++) {
                    std::vector<std::shared_ptr<Point>> current_exclusion_part = std::vector<std::shared_ptr<Point>>();
                    for(int j = 0;j<static_cast<int>(a_exclusion_points[i].size());j++) {
                        std::shared_ptr<Point> new_point = std::make_shared<Point>();
                        new_point.get()->set_x(a_exclusion_points[i][j].get()->x());
                        new_point.get()->set_y(a_exclusion_points[i][j].get()->y());
                        new_point.get()->set_z(a_exclusion_points[i][j].get()->z());
                        new_point.get()->set_parent(new_face);
                        current_exclusion_part.push_back(new_point);
                    }
                    new_face.get()->a_exclusion_points.push_back(current_exclusion_part);
                }

                // Create each points
                for(int i = 0;i<static_cast<int>(points().size());i++) {
                    std::shared_ptr<Point> new_point = std::make_shared<Point>();
                    new_point.get()->set_x(points()[i].get()->x());
                    new_point.get()->set_y(points()[i].get()->y());
                    new_point.get()->set_z(points()[i].get()->z());
                    new_point.get()->set_parent(new_face);
                    new_face.get()->points().push_back(new_point);
                }
                new_face.get()->triangulate();

                return new_face;
            };

            // Number of faces created
            static int a_faces_number;

            // Getters and setters
            inline std::vector<std::shared_ptr<Point>>& exclusion_points() {if(a_exclusion_points.size() <= 0) a_exclusion_points.push_back(std::vector<std::shared_ptr<Point>>()); return a_exclusion_points[0];};
            inline std::vector<std::shared_ptr<Point>>& points() {return a_points;};
            inline std::vector<std::shared_ptr<Point>>& points_for_rendering() {return a_points_for_rendering;};

            //*********
            //
            // Face geometrical manipulation
            //
            //*********

            // Connect to a face
            enum __Face_Connection {FC_X_Plus, FC_Y_Plus, FC_Plus};
            void connect_to_face(Face* face_to_connect, __Face_Connection connection) {
                if(connection == __Face_Connection::FC_X_Plus || connection == __Face_Connection::FC_Plus) {
                    // Get the base datas
                    double texture_height_1 = points()[1].get()->texture_y(id()) - points()[0].get()->texture_y(id());
                    double texture_height_2 = points()[2].get()->texture_y(id()) - points()[3].get()->texture_y(id());

                    // Connect the face in the X axe from + to -
                    points()[0] = face_to_connect->points()[1];
                    points()[3] = face_to_connect->points()[2];

                    // Connect the texture
                    points()[0].get()->set_texture_y(id(), points()[1].get()->texture_y(id()) + texture_height_1);
                    points()[3].get()->set_texture_y(id(), points()[2].get()->texture_y(id()) + texture_height_2);
                }
                if(connection == __Face_Connection::FC_Y_Plus || connection == __Face_Connection::FC_Plus) {
                    // Get the base datas
                    double texture_width_1 = points()[3].get()->texture_x(id()) - points()[0].get()->texture_x(id());
                    double texture_width_2 = points()[2].get()->texture_x(id()) - points()[1].get()->texture_x(id());

                    // Connect the face in the Y axe from + to -
                    points()[0] = face_to_connect->points()[3];
                    points()[1] = face_to_connect->points()[2];

                    // Connect the texture
                    points()[0].get()->set_texture_x(id(), points()[3].get()->texture_x(id()) + texture_width_1);
                    points()[1].get()->set_texture_x(id(), points()[2].get()->texture_x(id()) + texture_width_2);
                }
            };

            // Returns if a side of points cross the boundary or not
            bool cross_boundary(std::shared_ptr<Point> first_point, std::shared_ptr<Point> second_point) {return __cross_boundary(points(), first_point, second_point);}
            // Returns if a side of points cross the boundary of the exclusion part or not
            bool __cross_exclusion_boundary(std::vector<std::vector<std::shared_ptr<Point>>>& exclusion_points_copy, std::shared_ptr<model_maker::Point> first_point, std::shared_ptr<model_maker::Point> second_point) {
                for(int i = 0;i<static_cast<int>(exclusion_points_copy.size());i++) {
                    if(__cross_boundary(exclusion_points_copy[i], first_point, second_point)) return true;
                }
                return false;
            };
            // Returns if a point is in the exclusion part
            bool __in_exclusion_part(std::vector<std::vector<std::shared_ptr<Point>>>& exclusion_points_copy, double x_to_check, double y_to_check) {
                for(int i = 0;i<static_cast<int>(exclusion_points_copy.size());i++) {
                    if(__check_shape_content(exclusion_points_copy[i], x_to_check, y_to_check)) {
                        return true;
                    }
                }
                return false;
            };

            //*********
            //
            // Triangulation handling
            //
            //*********

            // Datas about a triangulation
            struct __Triangulation_Datas {
                // Created sides in the triangulation
                std::vector<std::shared_ptr<Point>> created_sides = std::vector<std::shared_ptr<Point>>();
                // Current position of the triangulation
                unsigned int current_i = 0;
                // Exclusion points in a triangulation
                std::vector<std::vector<std::shared_ptr<model_maker::Point>>> exclusion_points_copy = std::vector<std::vector<std::shared_ptr<model_maker::Point>>>();
                // If the triangulation is finished or not
                bool finished = false;
                // Copy of the points for a triangulation
                std::vector<std::shared_ptr<model_maker::Point>> points_copy = std::vector<std::shared_ptr<model_maker::Point>>();
            };

            void __triangulation_arrange_exclusion_points(std::vector<std::shared_ptr<model_maker::Point>>& points_copy, std::vector<std::vector<std::shared_ptr<Point>>>& exclusion_points_copy) {
                for(int i = 0;i<static_cast<int>(exclusion_points_copy.size());i++) {
                    // Find the index of the nearest point
                    bool position_found = false;
                    bool reverse_current_exclusion = false;
                    unsigned int start_position = 0;
                    while(!position_found) {
                        // Check the current point
                        for(int j = 0;j<static_cast<int>(points_copy.size() - 1);j++) {
                            if(!__triangulation_check_line_wrong(points_copy, points_copy[j], exclusion_points_copy[i][0])) {
                                set_can_print(true);
                                if(!__triangulation_check_line_wrong(points_copy, points_copy[j], exclusion_points_copy[i][exclusion_points_copy[i].size() - 1]) &&
                                   !__triangulation_check_line_wrong(points_copy, points_copy[j + 1], exclusion_points_copy[i][exclusion_points_copy[i].size() - 1]) &&
                                   !__check_crossing_segment(points_copy[j], exclusion_points_copy[i][0], points_copy[j + 1], exclusion_points_copy[i][exclusion_points_copy[i].size() - 1]).crossed_in_segment &&
                                   __check_triangle_opposed(points_copy[j], exclusion_points_copy[i][0], points_copy[j + 1], exclusion_points_copy[i][exclusion_points_copy[i].size() - 1])) {
                                    reverse_current_exclusion = false;
                                }
                                else if(!__triangulation_check_line_wrong(points_copy, points_copy[j], exclusion_points_copy[i][1]) &&
                                        !__triangulation_check_line_wrong(points_copy, points_copy[j + 1], exclusion_points_copy[i][1]) &&
                                        !__check_crossing_segment(points_copy[j], exclusion_points_copy[i][0], points_copy[j + 1], exclusion_points_copy[i][1]).crossed_in_segment &&
                                        __check_triangle_opposed(points_copy[j], exclusion_points_copy[i][0], points_copy[j + 1], exclusion_points_copy[i][1])) {
                                    reverse_current_exclusion = true;
                                }
                                else continue;
                                position_found = true;
                                set_can_print(false);
                                start_position = j;
                                break;
                            }
                        }

                        // Change the point if necessary
                        if(!position_found) {
                            exclusion_points_copy[i].push_back(exclusion_points_copy[i][0]);
                            exclusion_points_copy[i].erase(exclusion_points_copy[i].begin());
                        }
                    }

                    // Add the exclusion point
                    std::shared_ptr<Point> base_point = points_copy[start_position];start_position++;
                    if(reverse_current_exclusion) {
                        points_copy.insert(points_copy.begin() + start_position, exclusion_points_copy[i][0]);
                        start_position++;
                    }
                    for(int j = 0;j<static_cast<int>(exclusion_points_copy[i].size());j++) {
                        int real_j = j;
                        if(reverse_current_exclusion) real_j = exclusion_points_copy[i].size() - (j + 1);
                        points_copy.insert(points_copy.begin() + start_position + j, exclusion_points_copy[i][real_j]);
                    }
                    if(!reverse_current_exclusion) {
                        points_copy.insert(points_copy.begin() + exclusion_points_copy[i].size() + start_position, exclusion_points_copy[i][0]);
                        start_position++;
                    }
                    points_copy.insert(points_copy.begin() + exclusion_points_copy[i].size() + start_position, base_point);
                }
                exclusion_points_copy.clear();
            };
            bool __triangulation_check_line_wrong(std::vector<std::shared_ptr<model_maker::Point>>& points_copy, std::shared_ptr<model_maker::Point> first_point, std::shared_ptr<model_maker::Point> second_point) {
                bool on_boundary = __triangulation_cross_boundary(first_point, second_point);
                if(on_boundary) return true;
                bool in_shape = __triangulation_check_shape_content((second_point.get()->x() + first_point.get()->x()) / 2.0, (second_point.get()->z() + first_point.get()->z()) / 2.0);
                if(!in_shape) return true;
                return false;
            };
            bool __triangulation_check_shape_content(double point_to_test_x, double point_to_test_y) {
                // Check the main shape
                if(!__check_shape_content(points(), point_to_test_x, point_to_test_y)) return false;
                // Check each out shape
                for(int i = 0;i<static_cast<int>(a_exclusion_points.size());i++) {
                    if(__check_shape_content(a_exclusion_points[i], point_to_test_x, point_to_test_y)) return false;
                }
                return true;
            };
            bool __triangulation_cross_boundary(std::shared_ptr<model_maker::Point> first_point, std::shared_ptr<model_maker::Point> second_point) {
                // Check the main shape
                if(__cross_boundary(points(), first_point, second_point)) return true;
                // std::cout << "Passage 0" << std::endl;
                // Check each out shape
                for(int i = 0;i<static_cast<int>(a_exclusion_points.size());i++) {if(__cross_boundary(a_exclusion_points[i], first_point, second_point)) return true;}
                // std::cout << "Passage 1" << std::endl;
                return false;
            };
            // Do a triangulation of a face
            void triangulate() {
                // Get the start datas
                std::shared_ptr<__Triangulation_Datas> datas = triangulate_start_datas();

                // Do the triangulation
                while(!datas.get()->finished) {
                    triangulation_step(datas);
                }
            };
            // Returns the needed datas to start a triangulation
            std::shared_ptr<__Triangulation_Datas> triangulate_start_datas() {
                std::shared_ptr<__Triangulation_Datas> to_return = std::make_shared<__Triangulation_Datas>();
                to_return.get()->exclusion_points_copy = a_exclusion_points;
                to_return.get()->points_copy = points();

                // Do the triangulation
                points_for_rendering().clear();
                __triangulation_arrange_exclusion_points(to_return.get()->points_copy, to_return.get()->exclusion_points_copy);

                // Remove the useless points
                for(int i = 0;i<static_cast<int>(to_return.get()->points_copy.size());i++) {
                    if(to_return.get()->points_copy[i].get() == 0) {to_return.get()->points_copy.erase(to_return.get()->points_copy.begin() + i);i--;}
                }
                for(int i = 0;i<static_cast<int>(to_return.get()->exclusion_points_copy.size());i++) {
                    for(int j = 0;j<static_cast<int>(to_return.get()->exclusion_points_copy[i].size());j++) {
                        if(to_return.get()->exclusion_points_copy[i][j].get() == 0) {to_return.get()->exclusion_points_copy[i].erase(to_return.get()->exclusion_points_copy[i].begin() + j);j--;}
                    }
                    if(to_return.get()->exclusion_points_copy[i].size() <= 0) {to_return.get()->exclusion_points_copy[i].erase(to_return.get()->exclusion_points_copy[i].begin() + i);i--;}
                }

                return to_return;
            };
            // Check a step of triangulation and returns if a step occurs
            bool triangulation_step(std::shared_ptr<__Triangulation_Datas> triangulation_datas) {
                std::vector<std::shared_ptr<Point>>& created_sides = triangulation_datas.get()->created_sides;
                unsigned int& current_i = triangulation_datas.get()->current_i;
                std::vector<std::vector<std::shared_ptr<Point>>>& exclusion_points_copy = triangulation_datas.get()->exclusion_points_copy;
                std::vector<std::shared_ptr<model_maker::Point>>& points_copy = triangulation_datas.get()->points_copy;
                if(points_copy.size() > 3) {
                    // Get the point to test
                    std::shared_ptr<model_maker::Point> current_point = points_copy[current_i];;
                    std::shared_ptr<model_maker::Point> current_point_1 = points_copy[current_i + 1];
                    std::shared_ptr<model_maker::Point> current_point_2 = points_copy[current_i + 2];

                    // Check if the points are the same
                    if(current_point.get() == current_point_1.get() || current_point_1.get() == current_point_2.get()) {
                        // std::cout << "OMG " << current_i << std::endl;
                        points_copy.erase(points_copy.begin() + current_i + 1);
                        return false;
                    }

                    // Check if the out points are in the triangle
                    if(__triangulation_check_line_wrong(points_copy, current_point, current_point_2)) {
                        current_i++;
                        return false;
                    }
                    // Check if the line is not already existing
                    bool must_continue = false;
                    for(int i = 0;i<static_cast<int>(created_sides.size() - 1);i+=2){
                        if(current_point.get()==created_sides[i].get()&&current_point_2.get()==created_sides[i + 1].get()){must_continue=true;break;}
                        else if(current_point.get()==created_sides[i + 1].get()&&current_point_2.get()==created_sides[i].get()){must_continue=true;break;}
                    }
                    if(must_continue){current_i++;return false;}
                    // Add the point to the rendering
                    created_sides.push_back(current_point); created_sides.push_back(current_point_2);
                    points_copy.erase(points_copy.begin() + current_i + 1);
                    points_for_rendering().push_back(current_point);
                    points_for_rendering().push_back(current_point_1);
                    points_for_rendering().push_back(current_point_2);
                    current_i = 0;
                }
                else {
                    // Trace the last triangle
                    points_for_rendering().push_back(points_copy[0]);
                    points_for_rendering().push_back(points_copy[1]);
                    points_for_rendering().push_back(points_copy[2]);

                    triangulation_datas.get()->finished = true;
                }
                return true;
            };
            // Do a triangulation of a convex face
            void triangulate_convex() {
                // Add each points for rendering
                points_for_rendering().clear();
                for(unsigned short i = 0;i<points().size() - 2;i++) {
                    points_for_rendering().push_back(points()[0]);
                    points_for_rendering().push_back(points()[i + 1]);
                    points_for_rendering().push_back(points()[i + 2]);
                }
            };
            // Do a triangulation of a full face
            void __triangulate_full(std::vector<std::shared_ptr<model_maker::Point>> points_copy, bool last_point_is_first_point = true) {
                points_for_rendering().clear();

                // Trace each triangles
                unsigned int current_i = 0;
                unsigned int erase_offset = 0;
                for(int i = 0;i<static_cast<int>(points_copy.size());i++) {
                    if(points_copy[i].get() == 0) {points_copy.erase((points_copy.begin() + i) - erase_offset);erase_offset++;}
                }
                // Check each points
                int limit = 3; if(!last_point_is_first_point) limit = 4;
                while(points_copy.size() > limit) {
                    // Get the point to test
                    std::shared_ptr<model_maker::Point> current_point = points_copy[current_i];
                    std::shared_ptr<model_maker::Point> current_point_1 = points_copy[current_i + 1];
                    std::shared_ptr<model_maker::Point> current_point_2 = points_copy[current_i + 2];

                    // Check if the out points are in the triangle
                    bool in_shape = __check_shape_content(points_copy, (current_point_2.get()->x() + current_point.get()->x()) / 2.0, (current_point_2.get()->z() + current_point.get()->z()) / 2.0, last_point_is_first_point);
                    if(__cross_boundary(points_copy, current_point, current_point_2, last_point_is_first_point) || !in_shape) {
                        current_i++;
                        continue;
                    }
                    points_copy.erase(points_copy.begin() + current_i + 1);
                    points_for_rendering().push_back(current_point);
                    points_for_rendering().push_back(current_point_1);
                    points_for_rendering().push_back(current_point_2);
                    current_i = 0;
                }
                // Trace the last triangle
                points_for_rendering().push_back(points_copy[0]);
                points_for_rendering().push_back(points_copy[1]);
                points_for_rendering().push_back(points_copy[2]);
            };
            void triangulate_full() {__triangulate_full(points());};

            //*********
            //
            // Face solids datas
            //
            //*********

            // Returns if the point contains datas by id
            inline bool contains_solid_datas(unsigned int id_to_check) const {
                for(int i = 0;i<static_cast<int>(a_solids_datas.size());i++) { if(a_solids_datas.at(i).solid_id == id_to_check) return true; }
                return false;
            };
            // Returns a pointer to some datas, and create them if necessary
            inline __Face_Datas_By_Solid& solid_datas(unsigned int solid_id) {
                for(int i = 0;i<static_cast<int>(a_solids_datas.size());i++) { if(a_solids_datas[i].solid_id == solid_id) return a_solids_datas[i]; }
                __Face_Datas_By_Solid new_datas; new_datas.solid_id = solid_id;
                a_solids_datas.push_back(new_datas);
                return a_solids_datas[a_solids_datas.size() - 1];
            };

            // Getters and setters
            inline void set_texture_rect(unsigned int asker_id, double new_trx, double new_try, double new_trw, double new_trh) {
                __Face_Datas_By_Solid& datas = solid_datas(asker_id);
                datas.texture_rect_x = new_trx;
                datas.texture_rect_y = new_try;
                datas.texture_rect_width = new_trw;
                datas.texture_rect_height = new_trh;
            };
            inline double texture_rect_height(unsigned int asker_id) {return solid_datas(asker_id).texture_rect_height;};
            inline double texture_rect_x(unsigned int asker_id) {return solid_datas(asker_id).texture_rect_x;};
            inline double texture_rect_y(unsigned int asker_id) {return solid_datas(asker_id).texture_rect_y;};
            inline double texture_rect_width(unsigned int asker_id) {return solid_datas(asker_id).texture_rect_width;};

            //*********
            //
            // Normal members
            //
            //*********

            // Getters and setters
            inline double normal_x() const {return a_normal_x;};
            inline double normal_y() const {return a_normal_y;};
            inline double normal_z() const {return a_normal_z;};
            inline void set_normal_x(double new_normal_x) {a_normal_x = new_normal_x;};
            inline void set_normal_y(double new_normal_y) {a_normal_y = new_normal_y;};
            inline void set_normal_z(double new_normal_z) {a_normal_z = new_normal_z;};

        private:
            //*********
            //
            // Face mains attributes
            //
            //*********

            // Each exclusion points in the face
            std::vector<std::vector<std::shared_ptr<Point>>> a_exclusion_points = std::vector<std::vector<std::shared_ptr<Point>>>();
            // Each points in the face
            std::vector<std::shared_ptr<Point>> a_points = std::vector<std::shared_ptr<Point>>();
            // Each points position in the face for rendering
            std::vector<std::shared_ptr<Point>> a_points_for_rendering = std::vector<std::shared_ptr<Point>>();

            //*********
            //
            // Face solids datas
            //
            //*********

            // Each datas, sorted by id
            std::vector<__Face_Datas_By_Solid> a_solids_datas = std::vector<__Face_Datas_By_Solid>();

            //*********
            //
            // Normal attributes
            //
            //*********

            // X position of the normal
            double a_normal_x = 0;
            // Y position of the normal
            double a_normal_y = 0;
            // Z position of the normal
            double a_normal_z = 0;
        };

        //*********
        //
        // The __Model_Loader class
        //
        //*********

        class __Model_Loader {
            // Usefull class to load a model
        public:

            //*********
            //
            // __Model_Loader needed structures
            //
            //*********

            struct Transform_Object_For_Loading {
                // Struct representing some basics datas about a face
                // ID of the parent of the object
                unsigned int parent_id = 0;
                // X scale of the point
                double scale_x = 1;
                // Y scale of the point
                double scale_y = 1;
                // Z scale of the point
                double scale_z = 1;
                // X position of the point
                double x = 0;
                // Y position of the point
                double y = 0;
                // Z position of the point
                double z = 0;
            };
            struct Face_For_Loading : public Transform_Object_For_Loading {
                // Struct representing the datas about a face
                // The ID of each points in the model
                std::vector<unsigned int> points = std::vector<unsigned int>();
                // Each datas by solids, sorted by id
                std::map<unsigned int, __Face_Datas_By_Solid> solids_datas = std::map<unsigned int, __Face_Datas_By_Solid>();
            };
            struct Point_For_Loading : public Transform_Object_For_Loading {
                // Struct representing the datas about a point
                // Each attributes of the point
                std::map<unsigned int, double> attributes = std::map<unsigned int, double>();
                // Each datas by faces, sorted by id
                std::map<unsigned int, __Point_Datas_By_Face> faces_datas = std::map<unsigned int, __Point_Datas_By_Face>();
            };
            struct Solid_For_Loading : public Transform_Object_For_Loading {
                // Struct representing the datas about a solid
                // Each attributes of the solid
                std::map<unsigned int, double> attributes = std::map<unsigned int, double>();
                // The ID of each faces in the solid
                std::vector<unsigned int> faces = std::vector<unsigned int>();
            };

            //*********
            //
            // __Model_Loader mains members
            //
            //*********

            // Most simple __Model_Loader constructor
            __Model_Loader(std::shared_ptr<Bytes_Set>& file_content) : a_file_content(file_content) {};

            // Add a created face to the loader
            inline void add_created_face(unsigned int id, std::shared_ptr<Face>& created_face) {a_created_faces[id]=created_face;};
            // Add a created point to the loader
            inline void add_created_point(unsigned int id, std::shared_ptr<Point>& created_point) {a_created_points[id]=created_point;};

            // Add a face to the loader
            inline void add_face(unsigned int id, const Face_For_Loading& face_to_add) {a_faces[id]=face_to_add;};
            // Add a point to the loader
            inline void add_point(unsigned int id, const Point_For_Loading& point_to_add) {a_points[id]=point_to_add;};
            // Add a solid to the loader
            inline void add_solid(unsigned int id, const Solid_For_Loading& solid_to_add) {a_solids[id]=solid_to_add;};

            // Returns a created point
            inline std::shared_ptr<Point>* created_point(unsigned int id) {
                for(std::map<unsigned int, std::shared_ptr<Point>>::iterator it = a_created_points.begin();it!=a_created_points.end();it++) {
                    if(it->first == id) return &it->second;
                }
                return 0;
            };
            // Returns a created point which MUST exists
            inline std::shared_ptr<Point>* created_point_existing(unsigned int id) { return &a_created_points[id]; };

            // Returns a face in the loader
            inline Face_For_Loading* face(unsigned int id) {
                for(std::map<unsigned int, Face_For_Loading>::iterator it = a_faces.begin();it!=a_faces.end();it++) {
                    if(it->first == id) return &it->second;
                }
                return 0;
            };
            // Returns a face which MUST exists in the loader
            inline Face_For_Loading* face_existing(unsigned int id) { return &a_faces[id]; };
            // Returns a face in the loader
            inline Point_For_Loading* point(unsigned int id) {
                for(std::map<unsigned int, Point_For_Loading>::iterator it = a_points.begin();it!=a_points.end();it++) {
                    if(it->first == id) return &it->second;
                }
                return 0;
            };
            // Returns a point which MUST exists in the loader
            inline Point_For_Loading* point_existing(unsigned int id) { return &a_points[id]; };

            // Getters and setters
            inline std::map<unsigned int, std::shared_ptr<Point>>& created_points() {return a_created_points;};
            inline std::shared_ptr<Bytes_Set>& file_content() {return a_file_content;};
            inline std::map<unsigned int, Solid_For_Loading>& solids() {return a_solids;};

        private:
            // Content of the loaded file
            std::shared_ptr<Bytes_Set>& a_file_content;

            // Each created faces in the model, with their stored ID as key
            std::map<unsigned int, std::shared_ptr<Face>> a_created_faces = std::map<unsigned int, std::shared_ptr<Face>>();
            // Each created points in the model, with their stored ID as key
            std::map<unsigned int, std::shared_ptr<Point>> a_created_points = std::map<unsigned int, std::shared_ptr<Point>>();

            // Each faces in the model, with their ID as key
            std::map<unsigned int, Face_For_Loading> a_faces = std::map<unsigned int, Face_For_Loading>();
            // Each points in the model, with their ID as key
            std::map<unsigned int, Point_For_Loading> a_points = std::map<unsigned int, Point_For_Loading>();
            // Each solids in the model, with their ID as key
            std::map<unsigned int, Solid_For_Loading> a_solids = std::map<unsigned int, Solid_For_Loading>();
        };

        // Fill a __Model_Loader
        static std::shared_ptr<__Model_Loader> __model_loader_filled(std::shared_ptr<Bytes_Set>& file_content) {
            // Loader for the model
            std::shared_ptr<__Model_Loader> loader = std::make_shared<__Model_Loader>(file_content);

            // Get the number of points
            unsigned int cursor_position = 0;
            unsigned int point_number = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
            // Get each point
            for(unsigned int i = 0;i<point_number;i++) {
                // Get the data of the point
                unsigned int current_id = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                __Model_Loader::Point_For_Loading current_point = __Model_Loader::Point_For_Loading();
                current_point.x = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_point.y = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_point.z = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_point.parent_id = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                // Get each datas of the face
                unsigned int attribute_size = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                for(unsigned int j = 0;j<attribute_size;j++) {
                    unsigned int attribute_number = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                    double attribute_value = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                    current_point.attributes[attribute_number] = attribute_value;
                }
                loader.get()->add_point(current_id, current_point);
            }

            // Get the number of faces
            unsigned int face_number = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
            // Get each face
            for(unsigned int i = 0;i<face_number;i++) {
                __Model_Loader::Face_For_Loading current_face = __Model_Loader::Face_For_Loading();

                // Get the data of the face
                unsigned int current_id = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                // Get the position of the face
                current_face.x = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_face.y = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_face.z = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_face.scale_x = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_face.scale_y = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_face.scale_z = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_face.parent_id = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                // Get the points of the face
                unsigned int current_size = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                for(unsigned int j = 0;j<current_size;j++) {
                    // Get each points in the face
                    unsigned int current_point_id = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                    current_face.points.push_back(current_point_id);
                    // Get the datas of the point for the face
                    unsigned int attribute_size = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                    __Point_Datas_By_Face datas;
                    for(unsigned int k = 0;k<attribute_size;k++) {
                        unsigned int attribute_number = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                        datas.attributes[attribute_number] = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                    }
                    loader.get()->point_existing(current_point_id)->faces_datas[current_id] = datas;
                }
                loader.get()->add_face(current_id, current_face);
            }

            // Get the datas of the solid
            unsigned int solid_number = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
            for(unsigned int i = 0;i<solid_number;i++) {
                // Get the data of the solid
                unsigned int current_id = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                __Model_Loader::Solid_For_Loading current_solid = __Model_Loader::Solid_For_Loading();
                 // Get the transformation of the solid
                current_solid.x = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_solid.y = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_solid.z = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_solid.scale_x = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_solid.scale_y = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                current_solid.scale_z = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                // Load the faces in the solid
                unsigned int current_size = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                for(unsigned int i = 0;i<current_size;i++) {
                    // Get each faces in the solid
                    unsigned int current_face_id = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                    current_solid.faces.push_back(current_face_id);
                    // Get the rect of the texture
                    __Face_Datas_By_Solid datas;
                    datas.texture_rect_x = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                    datas.texture_rect_y = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                    datas.texture_rect_width = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                    datas.texture_rect_height = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                    loader.get()->face_existing(current_face_id)->solids_datas[current_id] = datas;
                }
                // Get each attributes of the solid
                unsigned int attribute_size = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                for(unsigned int j = 0;j<attribute_size;j++) {
                    unsigned int attribute_number = file_content.get()->extract_uint(cursor_position, true); cursor_position += 4;
                    double attribute_value = file_content.get()->extract_double(cursor_position, true); cursor_position += 8;
                    current_solid.attributes[attribute_number] = attribute_value;
                }
                loader.get()->add_solid(current_id, current_solid);
            }

            return loader;
        };

        // Load a face from a loader
        static std::shared_ptr<Face> __load_face_from_loader(unsigned int id, unsigned int asker_id, const __Face_Datas_By_Solid& datas, __Model_Loader::Face_For_Loading& current_face, std::shared_ptr<__Model_Loader>& loader) {
            // Create the face
            std::shared_ptr<Face> to_return = std::make_shared<Face>();
            to_return.get()->set_x(current_face.x);
            to_return.get()->set_y(current_face.y);
            to_return.get()->set_z(current_face.z);
            to_return.get()->set_scale_x(current_face.scale_x);
            to_return.get()->set_scale_y(current_face.scale_y);
            to_return.get()->set_scale_z(current_face.scale_z);
            __Face_Datas_By_Solid& current_datas = to_return.get()->solid_datas(asker_id);
            current_datas = datas;
            current_datas.solid_id = asker_id;

            // Create each points
            for(int i = 0;i<static_cast<int>(current_face.points.size());i++) {
                unsigned int current_id = current_face.points[i];
                __Model_Loader::Point_For_Loading* current_point_ptr = loader.get()->point_existing(current_id);
                if(current_point_ptr != 0) {
                    std::shared_ptr<Point> final_point;
                    unsigned int parent_id = current_point_ptr->parent_id - 1;

                    if(parent_id == id || current_point_ptr->parent_id == 0) {
                        // Create the existing point
                        final_point = std::make_shared<Point>();
                        __Model_Loader::Point_For_Loading& current_point = *current_point_ptr;
                        // Apply the transformation of the point
                        final_point.get()->attributes() = current_point.attributes;
                        final_point.get()->set_x(current_point.x);
                        final_point.get()->set_y(current_point.y);
                        final_point.get()->set_z(current_point.z);
                        final_point.get()->set_parent(to_return); to_return.get()->add_child(final_point);
                        // Add the point to the loader
                        loader.get()->add_created_point(current_id, final_point);
                    }
                    else {
                        // Use an already created point
                        std::shared_ptr<Point>* final_point_ptr = loader.get()->created_point_existing(current_id);
                        if(final_point_ptr == 0) continue;
                        final_point = *final_point_ptr;
                    }

                    // Set the data for this face
                    __Point_Datas_By_Face& current_datas = current_point_ptr->faces_datas[id];
                    final_point.get()->set_texture_x(to_return.get()->id(), current_datas.attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_X]);
                    final_point.get()->set_texture_y(to_return.get()->id(), current_datas.attributes[SCLS_3D_POINT_ATTRIBUTES_TEXTURE_Y]);
                    /*// Calculate the texture position of the point
                    double x_texture = final_point.get()->x();
                    double y_texture = final_point.get()->z();
                    x_texture -= polygon.get()->min_x;
                    y_texture -= polygon.get()->min_z;
                    // if(!reverse_texture_x) x_texture *= -1; x_texture++;
                    // if(!reverse_texture_z) y_texture *= -1; y_texture++;
                    to_add.get()->set_texture_multiplier_x(1, to_return.get()->id());
                    to_add.get()->set_texture_multiplier_y(2, to_return.get()->id());
                    to_add.get()->set_texture_multiplier_z(0, to_return.get()->id()); //*/

                    to_return.get()->points().push_back(final_point);
                }
            }

            // Add each points for rendering
            for(unsigned short i = 0;i<to_return.get()->points().size() - 2;i++) {
                to_return.get()->points_for_rendering().push_back(to_return.get()->points()[0]);
                to_return.get()->points_for_rendering().push_back(to_return.get()->points()[i + 1]);
                to_return.get()->points_for_rendering().push_back(to_return.get()->points()[i + 2]);
            }

            // Add the face to the loader
            loader.get()->add_created_face(id, to_return);

            return to_return;
        };

        //*********
        //
        // The Solid class
        //
        //*********

        // Needed variables to handle the grids
        static int __current_point_in_grid_x;
        static int __current_point_in_grid_z;

        class Solid : public Transform_Object_3D {
            // Solid in a model
        public:

            //*********
            //
            // Solid mains members
            //
            //*********

            // Solid constructor
            Solid() : Transform_Object_3D() { a_id = a_solids_number; a_solids_number++; };

            // Returns all the points in the solid
            std::vector<std::shared_ptr<Point>> all_points() {
                std::vector<std::shared_ptr<Point>> to_return = std::vector<std::shared_ptr<Point>>();

                // Check each faces
                for(int i = 0;i<static_cast<int>(a_faces.size());i++) {
                    Face& current_face = *a_faces.at(i).get();
                    std::vector<std::shared_ptr<Point>> face_points = current_face.points();
                    // Add each points
                    for(int i = 0;i<static_cast<int>(face_points.size());i++) {
                        if(face_points[i].get()->parent()->id() == current_face.id()) to_return.push_back(face_points[i]);
                    }
                }

                return to_return;
            };
            // Returns a point in a grid
            std::shared_ptr<Point> point_in_grid(int x, int z) {
                // Create the search function
                std::vector<std::shared_ptr<Point>> points = all_points();
                return point_in_grid(points, x, z);
            };
            // Returns a point in a grid with a given set of point
            std::shared_ptr<Point> point_in_grid(std::vector<std::shared_ptr<Point>>& points, int x, int z) {
                // Create the search function
                __current_point_in_grid_x = x;
                __current_point_in_grid_z = z;
                std::vector<std::shared_ptr<Point>>::iterator it = std::find_if(points.begin(), points.end(), [](std::shared_ptr<Point>& it) {
                    return it.get()->attributes()[SCLS_3D_POINT_ATTRIBUTES_GRID_X] == __current_point_in_grid_x &&
                           it.get()->attributes()[SCLS_3D_POINT_ATTRIBUTES_GRID_Z] == __current_point_in_grid_z;
                });

                // Create the point to return
                std::shared_ptr<Point> final_point;
                if(it == points.end()) return final_point;
                final_point = *it;
                return final_point;
            };

            // Adds a face to the solid
            inline void add_face(std::string face_name, std::shared_ptr<Face> face_to_add, std::shared_ptr<Transform_Object_3D> face_parent){faces().push_back(face_to_add);face_to_add.get()->set_parent(face_parent);faces_by_name()[face_name]=face_to_add;};
            // Returns a face by its name
            inline Face* face_by_name(std::string face_name) {
                for(std::map<std::string, std::shared_ptr<Face>>::iterator it = a_faces_by_name.begin();it!=a_faces_by_name.end();it++) {
                    if(it->first == face_name) return it->second.get();
                }
                return 0;
            };
            // Returns a face by its name
            inline __Face_Datas_By_Solid* face_data_by_name(std::string face_name) {
                for(std::map<std::string, std::shared_ptr<Face>>::iterator it = a_faces_by_name.begin();it!=a_faces_by_name.end();it++) {
                    if(it->first == face_name) return &it->second.get()->solid_datas(id());
                }
                return 0;
            };
            // Returns a group of face
            inline std::vector<std::shared_ptr<Face>> face_group(std::string group_name, unsigned short group_position) {
                std::vector<std::shared_ptr<Face>> to_return = std::vector<std::shared_ptr<Face>>();
                for(std::map<std::string, std::shared_ptr<Face>>::iterator it = a_faces_by_name.begin();it!=a_faces_by_name.end();it++) {
                    std::vector<std::string> cutted = cut_string(it->first, "-");
                    if(cutted.size() > group_position && cutted[group_position] == group_name) to_return.push_back(it->second);
                }
                return to_return;
            };
            // Fills between two faces
            void fill_faces_point_by_point(std::shared_ptr<Face> face_1, std::shared_ptr<Face> face_2, std::shared_ptr<Transform_Object_3D> face_parent) {
                // Create the side faces
                for(int i = 0;i<static_cast<int>(face_1.get()->points().size()) - 1;i++) {
                    // Get the needed points
                    std::shared_ptr<Face> face_center = std::make_shared<Face>();
                    std::shared_ptr<Point>& first_point = face_1.get()->points()[i];
                    first_point.get()->set_texture_x(face_center.get()->id(), 1); first_point.get()->set_texture_y(face_center.get()->id(), 1);
                    first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                    std::shared_ptr<Point>& second_point = face_1.get()->points()[i + 1];
                    second_point.get()->set_texture_x(face_center.get()->id(), 0); second_point.get()->set_texture_y(face_center.get()->id(), 1);
                    second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                    std::shared_ptr<Point>& third_point = face_2.get()->points()[i];
                    third_point.get()->set_texture_x(face_center.get()->id(), 1); third_point.get()->set_texture_y(face_center.get()->id(), 0);
                    third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                    std::shared_ptr<Point>& fourth_point = face_2.get()->points()[i + 1];
                    fourth_point.get()->set_texture_x(face_center.get()->id(), 0); fourth_point.get()->set_texture_y(face_center.get()->id(), 0);
                    fourth_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); fourth_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);

                    // Create the face
                    face_center.get()->points().push_back(first_point);
                    face_center.get()->points().push_back(second_point);
                    face_center.get()->points().push_back(fourth_point);
                    face_center.get()->points().push_back(third_point);
                    face_center.get()->points_for_rendering().push_back(first_point);
                    face_center.get()->points_for_rendering().push_back(second_point);
                    face_center.get()->points_for_rendering().push_back(third_point);
                    face_center.get()->points_for_rendering().push_back(second_point);
                    face_center.get()->points_for_rendering().push_back(fourth_point);
                    face_center.get()->points_for_rendering().push_back(third_point);
                    face_center.get()->set_parent(face_parent);
                    faces().push_back(face_center);
                    faces_by_name()["face-" + std::to_string(i)] = face_center;
                }

                // Get the needed points
                std::shared_ptr<Face> face_center = std::make_shared<Face>();
                std::shared_ptr<Point>& first_point = face_1.get()->points()[face_1.get()->points().size() - 1];
                first_point.get()->set_texture_x(face_center.get()->id(), 1); first_point.get()->set_texture_y(face_center.get()->id(), 1);
                first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& second_point = face_1.get()->points()[0];
                second_point.get()->set_texture_x(face_center.get()->id(), 0); second_point.get()->set_texture_y(face_center.get()->id(), 1);
                second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& third_point = face_2.get()->points()[face_2.get()->points().size() - 1];
                third_point.get()->set_texture_x(face_center.get()->id(), 1); third_point.get()->set_texture_y(face_center.get()->id(), 0);
                third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& fourth_point = face_2.get()->points()[0];
                fourth_point.get()->set_texture_x(face_center.get()->id(), 0); fourth_point.get()->set_texture_y(face_center.get()->id(), 0);
                fourth_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); fourth_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);

                // Create the face
                face_center.get()->points().push_back(first_point);
                face_center.get()->points().push_back(second_point);
                face_center.get()->points().push_back(fourth_point);
                face_center.get()->points().push_back(third_point);
                face_center.get()->points_for_rendering().push_back(first_point);
                face_center.get()->points_for_rendering().push_back(second_point);
                face_center.get()->points_for_rendering().push_back(third_point);
                face_center.get()->points_for_rendering().push_back(second_point);
                face_center.get()->points_for_rendering().push_back(fourth_point);
                face_center.get()->points_for_rendering().push_back(third_point);
                face_center.get()->set_parent(face_parent);
                faces().push_back(face_center);
                faces_by_name()["face-" + std::to_string(faces().size() - 3)] = face_center;
            };
            // Sets the rect of a group of face
            inline void set_face_group_rect_in_texture(std::string group_name, unsigned short group_position, double image_width, double image_height, double texture_rect_x, double texture_rect_y, double texture_rect_width, double texture_rect_height) {
                std::vector<std::shared_ptr<Face>> faces = face_group(group_name, group_position);
                for(int i = 0;i<static_cast<int>(faces.size());i++) {
                    __Face_Datas_By_Solid& datas = faces[i].get()->solid_datas(id());
                    datas.texture_rect_x = texture_rect_x / static_cast<double>(image_width) + (1.0 / (static_cast<double>(image_width) * 5.0));
                    datas.texture_rect_y = texture_rect_y / static_cast<double>(image_height) + (1.0 / (static_cast<double>(image_height) * 5.0));
                    datas.texture_rect_width = texture_rect_width / static_cast<double>(image_width) - (1.0 / (static_cast<double>(image_width) * 2.0));
                    datas.texture_rect_height = texture_rect_height / static_cast<double>(image_height) - (1.0 / (static_cast<double>(image_height) * 2.0));
                }
            };
            // Sets the rect of the face by tis name
            inline void set_face_rect_in_texture_by_name(std::string face_name, double image_width, double image_height, double texture_rect_x, double texture_rect_y, double texture_rect_width, double texture_rect_height) {
                __Face_Datas_By_Solid* datas = face_data_by_name(face_name);
                if(datas == 0) return;
                datas->texture_rect_x = texture_rect_x / static_cast<double>(image_width) + (1.0 / (static_cast<double>(image_width) * 5.0));
                datas->texture_rect_y = texture_rect_y / static_cast<double>(image_height) + (1.0 / (static_cast<double>(image_height) * 5.0));
                datas->texture_rect_width = texture_rect_width / static_cast<double>(image_width) - (1.0 / (static_cast<double>(image_width) * 2.0));
                datas->texture_rect_height = texture_rect_height / static_cast<double>(image_height) - (1.0 / (static_cast<double>(image_height) * 2.0));
            };

            // Returns the value of an attribute
            inline double attribute(unsigned int attribute_id) const {return a_attributes.at(attribute_id);};
            // Set the value of an attribute
            inline void set_attribute(unsigned int attribute_id, double attribute_value) {attributes()[attribute_id] = attribute_value; };

            // Getters and setters
            inline std::map<unsigned int, double>& attributes() {return a_attributes;};
            inline std::vector<std::shared_ptr<Face>>& faces() {return a_faces;};
            inline std::map<std::string, std::shared_ptr<Face>>& faces_by_name() {return a_faces_by_name;};

            //*********
            //
            // Solid loading members
            //
            //*********

            // Returns the arguments for the shader
            static std::shared_ptr<Bytes_Set> __shader_arguments(Point::_VBO_Types vbo_type = Point::_VT_Normal) {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                if(vbo_type == Point::_VT_Normal) {
                    // Add the needed shaders argument
                    to_return.get()->add_data(4);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(3, true);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(2, true);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(4, true);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(3, true);
                }
                else if(vbo_type == Point::_VT_Map) {
                    // Add the needed shaders argument
                    to_return.get()->add_data(2);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(3, true);
                    to_return.get()->add_ushort(0x1406, true);
                    to_return.get()->add_ushort(2, true);
                }

                return to_return;
            }

            // Returns the solid as binary
            std::shared_ptr<Bytes_Set> binary() {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the id of the solid
                to_return.get()->add_uint(id(), true);
                // Add the needed transformation
                to_return.get()->add_double(x(), true);
                to_return.get()->add_double(y(), true);
                to_return.get()->add_double(z(), true);
                to_return.get()->add_double(scale_x(), true);
                to_return.get()->add_double(scale_y(), true);
                to_return.get()->add_double(scale_z(), true);
                // Add the number of faces
                to_return.get()->add_uint(static_cast<unsigned int>(a_faces.size()), true);
                // Add each faces
                for(int i = 0;i<static_cast<int>(a_faces.size());i++) {
                    Face& current_face = *a_faces.at(i).get();
                    std::shared_ptr<Bytes_Set> datas = current_face.binary_for_solid(id());
                    to_return.get()->add_datas(*datas.get());
                }
                // Add each attributes
                to_return.get()->add_uint(attributes().size(), true);
                for(std::map<unsigned int, double>::iterator it = attributes().begin();it!=attributes().end();it++) {
                    to_return.get()->add_uint(it->first, true);
                    to_return.get()->add_double(it->second, true);
                }

                return to_return;
            };
            // Returns the solid as complete binary
            std::shared_ptr<Bytes_Set> binary_complete() {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Store the number of points
                std::vector<std::shared_ptr<Point>> each_points = all_points();
                to_return.get()->add_uint(each_points.size(), true);
                // Store each points
                for(int i = 0;i<static_cast<int>(each_points.size());i++) {
                    std::shared_ptr<Point>& current_point = each_points[i];
                    to_return.get()->add_datas(*current_point.get()->binary().get());
                }

                // Store the number of faces
                std::vector<std::shared_ptr<Face>> each_faces = faces();
                to_return.get()->add_uint(each_faces.size(), true);
                // Store each faces
                for(int i = 0;i<static_cast<int>(each_faces.size());i++) {
                    std::shared_ptr<Face>& current_point = each_faces[i];
                    to_return.get()->add_datas(*current_point.get()->binary().get());
                }

                // Store the solid
                to_return.get()->add_uint(1, true);
                to_return.get()->add_datas(*binary().get());

                return to_return;
            };
            // Returns the solid as binary VBO
            std::shared_ptr<Bytes_Set> __binary_vbo(unsigned int& total_points, Point::_VBO_Types vbo_type = Point::_VT_Normal) {
                // Get the size of the binary
                std::vector<std::shared_ptr<Bytes_Set>> binaries = std::vector<std::shared_ptr<Bytes_Set>>();
                unsigned int current_position = 0;
                unsigned int total_size = 8;
                for(int i = 0;i<static_cast<int>(a_faces.size());i++) {
                    std::shared_ptr<Bytes_Set> current_binary = a_faces[i].get()->binary_vbo(id(), total_points, false, vbo_type);
                    binaries.push_back(current_binary);
                    total_size += current_binary.get()->datas_size();
                }
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>(total_size);

                // Add each points
                for(int i = 0;i<static_cast<int>(binaries.size());i++) {
                    to_return.get()->put_datas(binaries[i].get(), current_position);
                    current_position += binaries[i].get()->datas_size();
                }

                return to_return;
            }
            // Returns the solid as binary a complete VBO
            std::shared_ptr<Bytes_Set> binary_vbo_complete(Point::_VBO_Types vbo_type = Point::_VT_Normal) {
                std::shared_ptr<Bytes_Set> to_return = __shader_arguments(vbo_type);

                // Add each points
                unsigned int total_points = 0;
                unsigned int total_points_position = to_return.get()->datas_size();
                to_return.get()->add_uint(0, true);
                // Add the data for this solid
                std::shared_ptr<Bytes_Set> solid_datas = __binary_vbo(total_points, vbo_type);
                to_return.get()->add_datas(*solid_datas.get());
                to_return.get()->put_uint(total_points, total_points_position, true);

                return to_return;
            };
            // Returns the solid as binary STL format
            std::shared_ptr<Bytes_Set> binary_stl(unsigned int& total_triangle) {
                // Get the size of the binary
                std::vector<std::shared_ptr<Bytes_Set>> binaries = std::vector<std::shared_ptr<Bytes_Set>>();
                unsigned int current_position = 0;
                unsigned int total_size = 8;
                for(int i = 0;i<static_cast<int>(a_faces.size());i++) {
                    std::shared_ptr<Bytes_Set> current_binary = a_faces[i].get()->binary_stl(total_triangle);
                    binaries.push_back(current_binary);
                    total_size += current_binary.get()->datas_size();
                }
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>(total_size);

                // Add each points
                for(int i = 0;i<static_cast<int>(binaries.size());i++) {
                    to_return.get()->put_datas(binaries[i].get(), current_position);
                    current_position += binaries[i].get()->datas_size();
                }

                return to_return;
            };
            // Returns the solid as complete binary STL format
            std::shared_ptr<Bytes_Set> binary_stl_complete() {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the header of the file
                std::string header = "";
                for(int i = 0;i<80;i++) header += " ";
                to_return.get()->add_string(header);
                to_return.get()->add_uint(0);

                // Add each points
                unsigned int total_triangle = 0;
                to_return.get()->add_datas(*binary_stl(total_triangle).get());
                to_return.get()->put_uint(total_triangle, 80);

                return to_return;
            };

            // Number of solids created
            static int a_solids_number;

            // Load the solid from a complete binary
            static std::shared_ptr<Solid> load_from_binary_complete(std::shared_ptr<Bytes_Set>& file_content) {
                // Loader for the model
                std::shared_ptr<__Model_Loader> loader = __model_loader_filled(file_content);
                unsigned int id = loader.get()->solids().begin()->first;
                return __load_from_loader(id, loader.get()->solids()[id], loader);
            };
            static std::shared_ptr<Solid> load_from_binary_complete(std::string file_path){std::shared_ptr<Bytes_Set> bytes = std::make_shared<Bytes_Set>();bytes.get()->load_from_file(file_path);return load_from_binary_complete(bytes);};
            // Load the solid from a loader
            static std::shared_ptr<Solid> __load_from_loader(unsigned int id, __Model_Loader::Solid_For_Loading& current_solid, std::shared_ptr<__Model_Loader>& loader) {
                std::shared_ptr<Solid> to_return = std::make_shared<Solid>();

                // Set the transformation of the solid
                to_return.get()->attributes() = current_solid.attributes;
                to_return.get()->set_scale_x(current_solid.scale_x);
                to_return.get()->set_scale_y(current_solid.scale_y);
                to_return.get()->set_scale_z(current_solid.scale_z);
                to_return.get()->set_x(current_solid.x);
                to_return.get()->set_y(current_solid.y);
                to_return.get()->set_z(current_solid.z);

                // Create the faces
                for(int i = 0;i<static_cast<int>(current_solid.faces.size());i++) {
                    unsigned int current_id = current_solid.faces[i];
                    __Model_Loader::Face_For_Loading* current_face_ptr = loader.get()->face_existing(current_id);
                    if(current_face_ptr != 0) {
                        // Create the existing face
                        __Model_Loader::Face_For_Loading& current_face = *current_face_ptr;
                        std::shared_ptr<Face> final_face = __load_face_from_loader(current_id, to_return.get()->id(), current_face.solids_datas[id], current_face, loader);
                        to_return.get()->faces().push_back(final_face);
                        final_face.get()->set_parent(to_return);
                        to_return.get()->add_child(final_face);
                    }
                }

                return to_return;
            };
        private:
            //*********
            //
            // Solid mains members
            //
            //*********

            // Each attributes of the solid
            std::map<unsigned int, double> a_attributes = std::map<unsigned int, double>();
            // Each faces in the solid
            std::vector<std::shared_ptr<Face>> a_faces = std::vector<std::shared_ptr<Face>>();
            // Faces in the solid stored with their name for the solid
            std::map<std::string, std::shared_ptr<Face>> a_faces_by_name = std::map<std::string, std::shared_ptr<Face>>();
        };

        //*********
        //
        // The Solid_Group class
        //
        //*********

        class Solid_Group {
            // Class representating a group of solid in a model
        public:

            //*********
            //
            // Solid_Group mains members
            //
            //*********

            // Solid_Group constructor
            Solid_Group() {};

            // Returns all the faces in each solids
            std::vector<std::shared_ptr<Face>> all_faces() {
                std::vector<std::shared_ptr<Face>> to_return = std::vector<std::shared_ptr<Face>>();

                // Check each faces
                for(int i = 0;i<static_cast<int>(a_solid.size());i++) {
                    Solid& current_solid = *a_solid.at(i).get();
                    std::vector<std::shared_ptr<Face>> solid_faces = current_solid.faces();
                    // Add each faces
                    for(int i = 0;i<static_cast<int>(solid_faces.size());i++) {
                        if(solid_faces[i].get()->parent() != 0 && solid_faces[i].get()->parent()->id() == current_solid.id()) to_return.push_back(solid_faces[i]);
                    }
                }

                return to_return;
            };
            // Returns all the points in each solids
            std::vector<std::shared_ptr<Point>> all_points() {
                std::vector<std::shared_ptr<Point>> to_return = std::vector<std::shared_ptr<Point>>();

                // Check each faces
                for(int i = 0;i<static_cast<int>(a_solid.size());i++) {
                    Solid& current_solid = *a_solid.at(i).get();
                    std::vector<std::shared_ptr<Point>> solid_points = current_solid.all_points();
                    // Add each points
                    for(int i = 0;i<static_cast<int>(solid_points.size());i++) {
                        to_return.push_back(solid_points[i]);
                    }
                }

                // Sort the faces
                struct {
                    bool operator()(std::shared_ptr<Point>& a, std::shared_ptr<Point>& b) const {
                        return a.get()->absolute_inner_x() < b.get()->absolute_inner_x() || (a.get()->absolute_inner_x() == b.get()->absolute_inner_x() && a.get()->absolute_inner_z() < b.get()->absolute_inner_z());
                    }
                } sort_faces;
                std::sort(to_return.begin(), to_return.end(), sort_faces);

                return to_return;
            };

            // Add a solid to the group
            void add_solid(const std::shared_ptr<Solid>& solid_to_add) {
                solid_to_add.get()->set_parent(attached_transformation());
                a_solid.push_back(solid_to_add);
            };
            // Returns the face as binary
            std::shared_ptr<Bytes_Set> binary() {
                // Get the needed things
                std::vector<std::shared_ptr<Point>> each_points = all_points();
                std::vector<std::shared_ptr<Face>> each_faces = all_faces();
                unsigned int total_size = 12;

                // Get the faces
                std::vector<std::shared_ptr<Bytes_Set>> faces_binaries = std::vector<std::shared_ptr<Bytes_Set>>();
                for(int i = 0;i<static_cast<int>(each_faces.size());i++) {
                    std::shared_ptr<Bytes_Set> current_binary = each_faces[i].get()->binary();
                    faces_binaries.push_back(current_binary);
                    total_size += current_binary.get()->datas_size();
                }

                // Get the solids
                std::vector<std::shared_ptr<Bytes_Set>> solids_binaries = std::vector<std::shared_ptr<Bytes_Set>>();
                for(int i = 0;i<static_cast<int>(a_solid.size());i++) {
                    std::shared_ptr<Bytes_Set> current_binary = a_solid[i].get()->binary();
                    solids_binaries.push_back(current_binary);
                    total_size += current_binary.get()->datas_size();
                }

                // Get the size of the binary
                unsigned int current_position = 0;
                for(int i = 0;i<static_cast<int>(each_points.size());i++) total_size += each_points[i].get()->binary().get()->datas_size();
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>(total_size);

                // Store the number of points
                to_return.get()->put_uint(each_points.size(), current_position, true); current_position += 4;
                // Store each points
                for(int i = 0;i<static_cast<int>(each_points.size());i++) {
                    std::shared_ptr<Point>& current_point = each_points[i];
                    std::shared_ptr<Bytes_Set> datas = current_point.get()->binary();
                    to_return.get()->put_datas(datas.get(), current_position);
                    current_position += datas.get()->datas_size();
                }

                // Store the face
                to_return.get()->put_uint(static_cast<unsigned int>(faces_binaries.size()), current_position, true); current_position += 4;
                for(int i = 0;i<static_cast<int>(faces_binaries.size());i++) {
                    to_return.get()->put_datas(faces_binaries[i].get(), current_position);
                    current_position += faces_binaries[i].get()->datas_size();
                }

                // Store the solid
                to_return.get()->put_uint(static_cast<unsigned int>(solids_binaries.size()), current_position, true); current_position += 4;
                for(int i = 0;i<static_cast<int>(solids_binaries.size());i++) {
                    to_return.get()->put_datas(solids_binaries[i].get(), current_position);
                    current_position += solids_binaries[i].get()->datas_size();
                }

                return to_return;
            };
            // Returns the solid group as binary STL format
            std::shared_ptr<Bytes_Set> binary_stl() {
                std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

                // Add the header of the file
                std::string header = "";
                for(int i = 0;i<80;i++) header += " ";
                to_return.get()->add_string(header);
                to_return.get()->add_uint(0);

                // Get the size of the binary
                std::vector<std::shared_ptr<Bytes_Set>> binaries = std::vector<std::shared_ptr<Bytes_Set>>();
                unsigned int current_position = 0;
                unsigned int total_size = 8;
                unsigned int total_triangle = 0;
                for(int i = 0;i<static_cast<int>(a_solid.size());i++) {
                    std::shared_ptr<Bytes_Set> current_binary = a_solid[i].get()->binary_stl(total_triangle);
                    binaries.push_back(current_binary);
                    total_size += current_binary.get()->datas_size();
                }
                std::shared_ptr<Bytes_Set> all_binaries = std::make_shared<Bytes_Set>(total_size);

                // Add each solids
                for(int i = 0;i<static_cast<int>(binaries.size());i++) {
                    all_binaries.get()->put_datas(binaries[i].get(), current_position);
                    current_position += binaries[i].get()->datas_size();
                }
                to_return.get()->add_datas(*all_binaries.get());

                return to_return;
            };
            // Returns the solid as binary VBO
            std::shared_ptr<Bytes_Set> binary_vbo(Point::_VBO_Types vbo_type = Point::_VT_Normal) {
                std::shared_ptr<Bytes_Set> to_return = Solid::__shader_arguments(vbo_type);

                // Add each points
                unsigned int cursor_position = 0;
                unsigned int total_points = 0;
                to_return.get()->add_uint(0, true); cursor_position += 4;
                for(int i = 0;i<static_cast<int>(a_solid.size());i++) {
                    to_return.get()->add_datas(*a_solid[i].get()->__binary_vbo(total_points, vbo_type).get());
                }
                to_return.get()->put_uint(total_points, 17, true);

                return to_return;
            };

            // Getters
            inline std::shared_ptr<Transform_Object_3D> &attached_transformation() {return a_attached_transformation;};
            inline std::vector<std::shared_ptr<Solid>>& solids() {return a_solid;};

        private:

            //*********
            //
            // Solid_Group mains attributes
            //
            //*********

            // List of each solid in the group
            std::vector<std::shared_ptr<Solid>> a_solid;
            // Transformation of the group
            std::shared_ptr<Transform_Object_3D> a_attached_transformation = std::make_shared<Transform_Object_3D>();
        };

        // Loads the solid group from binary
        static std::shared_ptr<Solid_Group> load_solid_group_from_binary(std::shared_ptr<Bytes_Set>& file_content) {
            // Loader for the model
            std::shared_ptr<__Model_Loader> loader = __model_loader_filled(file_content);
            std::shared_ptr<Solid_Group> to_return = std::make_shared<Solid_Group>();

            // Create each solid
            for(std::map<unsigned int, __Model_Loader::Solid_For_Loading>::iterator it = loader.get()->solids().begin();it!=loader.get()->solids().end();it++) {
                std::shared_ptr<Solid> current_solid = Solid::__load_from_loader(it->first, it->second, loader);
                to_return.get()->add_solid(current_solid);
            }

            return to_return;
        };
        static std::shared_ptr<Solid_Group> load_solid_group_from_binary(std::string file_path){std::shared_ptr<Bytes_Set> bytes = std::make_shared<Bytes_Set>();bytes.get()->load_from_file(file_path);return load_solid_group_from_binary(bytes);};

        //*********
        //
        // Polygon handling
        //
        //*********

        struct Polygon {
            // Each exclusion points in the polygon
            std::vector<std::vector<Point>> exclusion_points = std::vector<std::vector<Point>>();
            // Maximum of the X axis
            double max_x = 0;
            // Maximum of the Z axis
            double max_z = 0;
            // Minimum of the X axis
            double min_x = 0;
            // Minimum of the Z axis
            double min_z = 0;
            // Struct representating a return of the "regular_polygon_points" function
            std::vector<Point> points;
            // Size of the X axis
            double size_x = 0;
            // Size of the Z axis
            double size_z = 0;
        };
        // Loads a polygon from binary
        inline std::shared_ptr<Polygon> load_polygon_binary(std::shared_ptr<Bytes_Set> binary) {
            std::shared_ptr<Polygon> loaded_polygon = std::make_shared<Polygon>();

            // Get basics datas
            unsigned int current_position = 0;
            loaded_polygon.get()->size_x = binary.get()->extract_double(current_position); current_position += 8;
            loaded_polygon.get()->size_z = binary.get()->extract_double(current_position); current_position += 8;
            loaded_polygon.get()->min_x = binary.get()->extract_double(current_position); current_position += 8;
            loaded_polygon.get()->max_x = binary.get()->extract_double(current_position); current_position += 8;
            loaded_polygon.get()->min_z = binary.get()->extract_double(current_position); current_position += 8;
            loaded_polygon.get()->max_z = binary.get()->extract_double(current_position); current_position += 8;
            // Get the points
            unsigned int point_number = binary.get()->extract_uint(current_position, true); current_position += 4;
            for(unsigned int i = 0;i<point_number;i++) {
                // Get the data of the point
                Point point_to_add = Point();
                unsigned int current_id = binary.get()->extract_uint(current_position, true); current_position += 4;
                // Get the position of the point
                point_to_add.set_x(binary.get()->extract_double(current_position, true)); current_position += 8;
                point_to_add.set_y(binary.get()->extract_double(current_position, true)); current_position += 8;
                point_to_add.set_z(binary.get()->extract_double(current_position, true)); current_position += 8;
                unsigned int parent_id = binary.get()->extract_uint(current_position, true); current_position += 4;
                // Get each datas of the face
                unsigned int attribute_size = binary.get()->extract_uint(current_position, true); current_position += 4;
                for(unsigned int j = 0;j<attribute_size;j++) {
                    unsigned int attribute_number = binary.get()->extract_uint(current_position, true); current_position += 4;
                    double attribute_value = binary.get()->extract_double(current_position, true); current_position += 8;
                    point_to_add.set_attribute(attribute_number, attribute_value);
                }
                loaded_polygon.get()->points.push_back(point_to_add);
            }

            return loaded_polygon;
        };
        // Returns a polygon in binary
        inline std::shared_ptr<Bytes_Set> polygon_binary(std::string path, Polygon* polygon_to_save) {
            unsigned int polygon_size = 52;

            // Get each points to save
            std::vector<std::shared_ptr<Bytes_Set>> points_datas = std::vector<std::shared_ptr<Bytes_Set>>();
            for(int i = 0;i<static_cast<int>(polygon_to_save->points.size());i++) {
                std::shared_ptr<Bytes_Set> point_binary = polygon_to_save->points[i].binary();
                points_datas.push_back(point_binary);
                polygon_size += point_binary.get()->datas_size();
            }

            // Create the set to return
            unsigned int current_position = 0;
            std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>(polygon_size);
            to_return.get()->put_double(polygon_to_save->size_x, current_position); current_position += 8;
            to_return.get()->put_double(polygon_to_save->size_z, current_position); current_position += 8;
            to_return.get()->put_double(polygon_to_save->min_x, current_position); current_position += 8;
            to_return.get()->put_double(polygon_to_save->max_x, current_position); current_position += 8;
            to_return.get()->put_double(polygon_to_save->min_z, current_position); current_position += 8;
            to_return.get()->put_double(polygon_to_save->max_z, current_position); current_position += 8;
            // Save the points
            to_return.get()->put_uint(points_datas.size(), current_position, true); current_position += 4;
            for(int i = 0;i<static_cast<int>(points_datas.size());i++) {
                to_return.get()->put_datas(points_datas[i].get(), current_position); current_position += points_datas[i].get()->datas_size();
            }

            return to_return;
        };

        // Returns a face which make a simple polygon
        static std::shared_ptr<Face> polygon(std::shared_ptr<Polygon> polygon, bool reverse_texture_x = false, bool reverse_texture_z = false, double y = 0) {
            std::shared_ptr<Face> to_return = std::make_shared<Face>();
            to_return.get()->set_y(y);
            if(y > 0) to_return.get()->set_normal_y(1);
            else to_return.get()->set_normal_y(-1);

            // Add each points
            std::vector<Point>& points = polygon.get()->points;
            for(int i = 0;i<static_cast<int>(points.size());i++) {
                std::shared_ptr<Point> to_add = std::make_shared<Point>(points.at(i));
                to_add.get()->reset();
                to_add.get()->set_parent(to_return); to_return.get()->add_child(to_add);

                // Calculate the texture position of the point
                double x_texture = to_add.get()->x();
                double y_texture = to_add.get()->z();
                x_texture -= polygon.get()->min_x;
                y_texture -= polygon.get()->min_z;
                if(!reverse_texture_x) x_texture *= -1; x_texture++;
                if(!reverse_texture_z) y_texture *= -1; y_texture++;
                to_add.get()->set_texture_x(to_return.get()->id(), x_texture);
                to_add.get()->set_texture_y(to_return.get()->id(), y_texture);
                to_add.get()->set_texture_multiplier_x(1, to_return.get()->id());
                to_add.get()->set_texture_multiplier_y(2, to_return.get()->id());
                to_add.get()->set_texture_multiplier_z(0, to_return.get()->id());

                to_return.get()->points().push_back(to_add);
            }
            to_return.get()->triangulate();

            return to_return;
        };
        // Returns a face which make a simple polygon
        static std::shared_ptr<Face> polygon(std::vector<Point> points, bool reverse_texture_x = false, bool reverse_texture_z = false, double y = 0) {
            // Create the polygon
            std::shared_ptr<Polygon> created_polygon = std::make_shared<Polygon>();
            for(int i = 0;i<static_cast<int>(points.size());i++) {
                Point& current_point = points[i];
                created_polygon.get()->points.push_back(current_point);
                if(current_point.x() > created_polygon.get()->max_x) created_polygon.get()->max_x = current_point.x();
                if(current_point.x() < created_polygon.get()->min_x) created_polygon.get()->min_x = current_point.x();
                if(current_point.z() > created_polygon.get()->max_z) created_polygon.get()->max_z = current_point.z();
                if(current_point.z() < created_polygon.get()->min_z) created_polygon.get()->min_z = current_point.z();
            }
            created_polygon.get()->size_x = created_polygon.get()->max_x - created_polygon.get()->min_x;
            created_polygon.get()->min_z = created_polygon.get()->max_z - created_polygon.get()->min_z;

            return polygon(created_polygon, reverse_texture_x, reverse_texture_z, y);
        };
        // Returns a face which make a simple polygon in 3D
        static std::shared_ptr<Solid> polygon_3d(std::vector<Point> points) {
            // Create the base faces
            std::shared_ptr<Face> face_1 = polygon(points, false, false, 0.5);
            std::shared_ptr<Face> face_2 = polygon(points, false, true, -0.5);

            // Create the solid
            std::shared_ptr<Solid> to_return = std::make_shared<Solid>();
            to_return.get()->add_face("top", face_1, to_return);
            to_return.get()->add_face("bottom", face_2, to_return);
            to_return.get()->fill_faces_point_by_point(face_1, face_2, to_return);

            return to_return;
        };
        // Returns a face which make a simple polygon in 3D
        static std::shared_ptr<Solid> polygon_3d(std::shared_ptr<Polygon> points) {return polygon_3d(points.get()->points);};
        // Returns a face which make a simple polygon in 3D
        static std::shared_ptr<Solid> polygon_3d(std::vector<std::shared_ptr<Point>> points) {
            std::vector<Point> points_to_pass = std::vector<Point>();
            for(int i = 0;i<static_cast<int>(points.size());i++) {points_to_pass.push_back(*points[i].get());}
            return polygon_3d(points_to_pass);
        };

        //*********
        //
        // Regular polygons handling
        //
        //*********

        // Returns a simple regular polygon
        static std::shared_ptr<Polygon> regular_polygon_points(unsigned short side_number, double y = 0, double scale = 1) {
            // Configurate the creation
            double angle_sum = static_cast<double>((side_number - 2) * SCLS_PI);
            double current_angle = 0;
            double max_x = 0; double min_x = 0;
            double max_z = 0; double min_z = 0;
            double one_angle = SCLS_PI - angle_sum / static_cast<double>(side_number);
            std::shared_ptr<Polygon> to_return = std::make_shared<Polygon>();

            // Calculate each positions
            double angle_offset = SCLS_PI * 1.5 - one_angle / 2.0;
            for(unsigned short i = 0;i<side_number;i++) {
                // Calculate the position of the point
                double x = std::cos(current_angle + angle_offset);
                double z = std::sin(current_angle + angle_offset);
                if(x > max_x) max_x = x; if(x < min_x) min_x = x;
                if(z > max_z) max_z = z; if(z < min_z) min_z = z;

                // Create the point
                Point point = Point();
                point.move_xyz(x, y, z);
                to_return.get()->points.push_back(point);

                current_angle += one_angle;
            }
            to_return.get()->max_x = max_x;
            to_return.get()->max_z = max_z;
            to_return.get()->min_x = min_x;
            to_return.get()->min_z = min_z;
            to_return.get()->size_x = max_x - min_x;
            to_return.get()->size_z = max_z - min_z;

            // Calculate the size for normalization
            double multiplier = (scale / to_return.get()->size_x);
            double size_ratio = to_return.get()->size_z / to_return.get()->size_x;
            to_return.get()->size_x = scale;
            to_return.get()->size_z = size_ratio;

            // Normalize each positions
            for(int i = 0;i<static_cast<int>(to_return.get()->points.size());i++) {
                // Normalize the point
                Point& point = to_return.get()->points[i];
                point.set_x(point.x() * multiplier);
                point.set_z(point.z() * multiplier);
            }
            to_return.get()->max_x *= multiplier;
            to_return.get()->max_z *= multiplier;
            to_return.get()->min_x *= multiplier;
            to_return.get()->min_z *= multiplier;

            return to_return;
        };
        // Returns a face which make a simple regular polygon
        static std::shared_ptr<Face> regular_polygon(unsigned short side_number, bool reverse_texture_x = false, bool reverse_texture_z = false, double y = 0) {
            std::shared_ptr<Face> to_return = std::make_shared<Face>();
            to_return.get()->set_y(y);
            if(y > 0) to_return.get()->set_normal_y(1);
            else to_return.get()->set_normal_y(-1);

            // Add each points
            std::shared_ptr<Polygon> points = regular_polygon_points(side_number, 0);
            for(int i = 0;i<static_cast<int>(points.get()->points.size());i++) {
                std::shared_ptr<Point> to_add = std::make_shared<Point>(points.get()->points.at(i));
                to_add.get()->reset();
                to_add.get()->set_parent(to_return);

                // Calculate the texture position of the point
                double x_texture = to_add.get()->x();
                double z_texture = to_add.get()->z();
                x_texture -= points.get()->min_x;
                z_texture -= points.get()->min_z;
                if(!reverse_texture_x) {x_texture *= -1; x_texture++;}
                if(!reverse_texture_z) {z_texture *= -1; z_texture++;}
                to_add.get()->set_texture_x(to_return.get()->id(), x_texture);
                to_add.get()->set_texture_y(to_return.get()->id(), z_texture);
                to_add.get()->set_texture_multiplier_x(1, to_return.get()->id());
                to_add.get()->set_texture_multiplier_y(0, to_return.get()->id());
                to_add.get()->set_texture_multiplier_z(2, to_return.get()->id());

                to_return.get()->points().push_back(to_add);
            }
            to_return.get()->triangulate_convex();

            return to_return;
        };
        // Returns a face which make a simple regular polygon in 3D
        static std::shared_ptr<Solid> regular_polygon_3d(unsigned short side_number){
            // Create the base faces
            std::shared_ptr<Face> face_1 = regular_polygon(side_number, true, true, 0.5);
            std::shared_ptr<Face> face_2 = regular_polygon(side_number, true, true, -0.5);

            // Create the solid
            std::shared_ptr<Solid> to_return = std::make_shared<Solid>();
            to_return.get()->add_face("top", face_1, to_return);
            to_return.get()->add_face("bottom", face_2, to_return);
            to_return.get()->fill_faces_point_by_point(face_1, face_2, to_return);

            return to_return;
        };

        // Returns a squared-solid with a frame of faces in its top
        static std::shared_ptr<Solid> frame_polyhedron_in_top(unsigned int frame_width, unsigned int frame_height) {
            // Create the solid
            std::shared_ptr<Solid> to_return = std::make_shared<Solid>();

            // Create the top face
            double current_x = -0.5;
            double current_z = -0.5;
            double face_height = 1.0 / (frame_height);
            double face_width = 1.0 / (frame_width);
            for(unsigned int i = 0;i<frame_width;i++) {
                for(unsigned int j = 0;j<frame_height;j++) {
                    std::shared_ptr<Face> current_face = regular_polygon(4);
                    current_face.get()->set_scale(face_width, 1, face_height);
                    current_face.get()->set_parent(to_return);
                    current_face.get()->set_x(current_x + face_width / 2.0);
                    current_face.get()->set_z(current_z + face_height / 2.0);
                    to_return.get()->faces().push_back(current_face); to_return.get()->add_child(current_face);

                    // Change the points
                    if(i > 0) {
                        current_face.get()->points()[0] = to_return.get()->faces()[(i * frame_height + j) - frame_height].get()->points()[1];
                        current_face.get()->points()[3] = to_return.get()->faces()[(i * frame_height + j) - frame_height].get()->points()[2];
                    }
                    if(j > 0) {
                        current_face.get()->points()[0] = to_return.get()->faces()[(i * frame_height + j) - 1].get()->points()[3];
                        current_face.get()->points()[1] = to_return.get()->faces()[(i * frame_height + j) - 1].get()->points()[2];
                    } //*/
                    // Set the attributes
                    current_face.get()->points()[0].get()->set_attribute(SCLS_3D_POINT_ATTRIBUTES_GRID_X, i);
                    current_face.get()->points()[1].get()->set_attribute(SCLS_3D_POINT_ATTRIBUTES_GRID_Z, j);

                    // Change the texture position
                    Point* current_point = current_face.get()->points()[0].get();
                    current_point->set_texture_x(current_face.get()->id(), current_x + 0.5);
                    current_point->set_texture_y(current_face.get()->id(), current_z + 0.5);
                    current_point = current_face.get()->points()[1].get();
                    current_point->set_texture_x(current_face.get()->id(), current_x + 0.5 + face_width);
                    current_point->set_texture_y(current_face.get()->id(), current_z + 0.5);
                    current_point = current_face.get()->points()[2].get();
                    current_point->set_texture_x(current_face.get()->id(), current_x + 0.5 + face_width);
                    current_point->set_texture_y(current_face.get()->id(), current_z + 0.5 + face_height);
                    current_point = current_face.get()->points()[3].get();
                    current_point->set_texture_x(current_face.get()->id(), current_x + 0.5);
                    current_point->set_texture_y(current_face.get()->id(), current_z + 0.5 + face_height);

                    current_z += face_height;
                }
                current_z = -0.5;
                current_x += face_width;
            }

            return to_return;
        };
    }
}

#endif // SCLS_MATH_3D_MODEL
