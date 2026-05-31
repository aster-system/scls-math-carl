//******************
//
// scls_math_model.cpp
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
// This file contains the source code of scls_math_model.h (or scls_math_3d_model.h in the older versions).
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
    namespace model_maker {

        // Needed variables to handle the grids
        int __current_point_in_grid_x = 0;
        int __current_point_in_grid_z = 0;

        //*********
        //
        // The Point class
        //
        //*********

        // Returns the point as a binary
        std::shared_ptr<Bytes_Set> Point::binary() {
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
        }
        // Returns the point as a binary for face
        std::shared_ptr<Bytes_Set> Point::binary_for_face(unsigned int asker_id){
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
        }
        // Returns the point as a binary STL
        std::shared_ptr<Bytes_Set> Point::binary_stl(double multiplication) {
            std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

            // Add the positions
            to_return.get()->add_float(absolute_x() * multiplication);
            to_return.get()->add_float(absolute_y() * multiplication);
            to_return.get()->add_float(absolute_z() * multiplication);

            return to_return;
        }
        // Returns the point as binary VBO
        std::shared_ptr<Bytes_Set> Point::binary_vbo(unsigned int asker_id, double normal_x, double normal_y, double normal_z, double texture_rect_x, double texture_rect_y, double texture_rect_width, double texture_rect_height, _VBO_Types vbo_type) {
            std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

            // Add the positions
            to_return.get()->add_double(absolute_inner_x());
            to_return.get()->add_double(absolute_inner_y());
            to_return.get()->add_double(absolute_inner_z());

            // Add the normals
            to_return.get()->add_double(normal_x);
            to_return.get()->add_double(normal_y);
            to_return.get()->add_double(normal_z);

            // Add the texture positions
            to_return.get()->add_double(texture_x(asker_id));
            to_return.get()->add_double(texture_y(asker_id));

            if(vbo_type == _VBO_Types::_VT_Normal || vbo_type == _VBO_Types::_VT_Curved) {
                // Add the texture rect
                to_return.get()->add_double(texture_rect_x);
                to_return.get()->add_double(texture_rect_y);
                to_return.get()->add_double(texture_rect_width);
                to_return.get()->add_double(texture_rect_height);

                // Add the texture multiplier
                to_return.get()->add_double(texture_multiplier_x(asker_id));
                to_return.get()->add_double(texture_multiplier_y(asker_id));
                to_return.get()->add_double(texture_multiplier_z(asker_id));

                if(vbo_type == _VBO_Types::_VT_Curved) {
                    // Add the curve multiplier
                    if(absolute_inner_z() > 0) {to_return.get()->add_double(1);}
                    else{to_return.get()->add_double(-1);}
                }
            }

            return to_return;
        }

        // Returns a pointer to some datas, and create them if necessary
        __Point_Datas_By_Face& Point::face_datas(unsigned int face_id) {
            for(int i = 0;i<static_cast<int>(a_faces_datas.size());i++) { if(a_faces_datas[i].face_id == face_id) return a_faces_datas[i]; }
            __Point_Datas_By_Face new_datas; new_datas.face_id = face_id;
            a_faces_datas.push_back(new_datas);
            return a_faces_datas[a_faces_datas.size() - 1];
        };

        //*********
        //
        // The Face class
        //
        //*********

        // Returns the datas point of two crossing lines
        Crossing_Datas __check_crossing(const Point* first_point, const Point* second_point, const Point* third_point, const Point* fourth_point) {return scls::check_crossing(first_point->x(), first_point->z(), second_point->x(), second_point->z(), third_point->x(), third_point->z(), fourth_point->x(), fourth_point->z());}
        // Returns the datas point of two crossing segments
        Crossing_Datas_Segment __check_crossing_segment(const Point* first_point, const Point* second_point, const Point* third_point, const Point* fourth_point, bool check_first_and_second_point, bool check_third_and_fourth_point) {return scls::check_crossing_segment(first_point->x(), first_point->z(), second_point->x(), second_point->z(), third_point->x(), third_point->z(), fourth_point->x(), fourth_point->z(), check_first_and_second_point, check_third_and_fourth_point);}
        Crossing_Datas_Segment __check_crossing_segment(const std::shared_ptr<Point>& first_point, const std::shared_ptr<Point>& second_point, const std::shared_ptr<Point>& third_point, const std::shared_ptr<Point>& fourth_point, bool check_first_and_second_point, bool check_third_and_fourth_point) {return __check_crossing_segment(first_point.get(), second_point.get(), third_point.get(), fourth_point.get(), check_first_and_second_point, check_third_and_fourth_point);}

        // Returns if a point is in a set of point
        bool __check_shape_content(const std::vector<std::shared_ptr<Point>>& points, double point_to_test_x, double point_to_test_y, bool last_point_is_first_point) {
            // Check each sides
            Crossing_Datas_Segment datas;
            unsigned int top_collision = 0;
            for(int i = 0;i<static_cast<int>(points.size() - 1);i++) {
                datas = scls::check_crossing_segment(point_to_test_x, point_to_test_y, point_to_test_x, point_to_test_y + 1.0, points.at(i).get()->x(), points.at(i).get()->z(), points.at(i + 1).get()->x(), points.at(i + 1).get()->z(), false, true);
                if(datas.crossed_in_segment) {
                    if(datas.crossing_datas.crossing_y >= point_to_test_y) top_collision++;
                }
            }
            // Check the last border
            if(last_point_is_first_point && static_cast<int>(points.size()) > 1) {
                datas = scls::check_crossing_segment(point_to_test_x, point_to_test_y, point_to_test_x, point_to_test_y + 1.0, points[points.size() - 1].get()->x(), points[points.size() - 1].get()->z(), points[0].get()->x(), points[0].get()->z(), false, true);
                if(datas.crossed_in_segment) {
                    if(datas.crossing_datas.crossing_y >= point_to_test_y) top_collision++;
                }
            }
            return top_collision % 2 == 1;
        }
        bool __check_shape_content(const std::vector<std::shared_ptr<Point>>& points, Point* point_to_test) {return __check_shape_content(points, point_to_test->x(), point_to_test->z());}
        bool __check_shape_content(const std::vector<std::shared_ptr<Point>>& points, std::shared_ptr<Point> point_to_test) {return __check_shape_content(points, point_to_test.get());}
        // Check if two triangles are opposed to their sharing side or not
        bool __check_triangle_opposed(const Point* first_point, const Point* second_point, const Point* first_top, const Point* second_top) {
            // Get the crossing datas
            Crossing_Datas_Segment datas = __check_crossing_segment(first_point, second_point, first_top, second_top, true, false);
            return datas.crossed_in_segment;
        }
        bool __check_triangle_opposed(const std::shared_ptr<Point>& first_point, const std::shared_ptr<Point>& second_point, const std::shared_ptr<Point>& third_point, const std::shared_ptr<Point>& fourth_point) {return __check_triangle_opposed(first_point.get(), second_point.get(), third_point.get(), fourth_point.get());}

        // Returns if a side of points cross the boundary of a set of points or not
        bool __cross_boundary(const std::vector<std::shared_ptr<Point>>& points, std::shared_ptr<Point> first_point, std::shared_ptr<Point> second_point, bool last_point_is_first_point) {
            // Check each sides
            for(int i = 0;i<static_cast<int>(points.size() - 1);i++) {
                if(first_point.get() == points.at(i).get() && second_point.get() == points.at(i + 1).get()) { return true; }
                else if(first_point.get() == points.at(i + 1).get() && second_point.get() == points.at(i).get()) { return true; }
                // A point is a part of the boundary
                else if(first_point.get() == points.at(i).get() || first_point.get() == points.at(i + 1).get()) { continue; }
                else if(second_point.get() == points.at(i).get() || second_point.get() == points.at(i + 1).get()) { continue; }
                // The points does not belong to the boundary
                else {
                    // Check each sides with carthesian coordonates
                    Crossing_Datas_Segment datas = __check_crossing_segment(first_point.get(), second_point.get(), points.at(i).get(), points.at(i + 1).get());
                    if(datas.crossed_in_segment) return true;
                }
            }
            // Check the last points
            if(last_point_is_first_point && static_cast<int>(points.size()) > 1) {
                if(first_point.get() == points.at(points.size() - 1).get() && second_point.get() == points.at(0).get()) { return true; }
                else if(first_point.get() == points.at(0).get() && second_point.get() == points.at(points.size() - 1).get()) { return true; }
                // A point is a part of the boundary
                else if(first_point.get() == points.at(0).get() || first_point.get() == points.at(points.size() - 1).get()) { return false; }
                else if(second_point.get() == points.at(0).get() || second_point.get() == points.at(points.size() - 1).get()) { return false; }
                // The points does not belong to the boundary
                else if(first_point.get() != points.at(0).get() && first_point.get() != points.at(points.size() - 1).get() && second_point.get() != points.at(0).get() && second_point.get() != points.at(points.size() - 1).get()) {
                    if(__check_crossing_segment(first_point.get(), second_point.get(), points.at(points.size() - 1).get(), points.at(0).get()).crossed_in_segment) return true;
                }
            }
            return false;
        }

        // Creates and return a new face object
        std::shared_ptr<Face> Face::new_face_object(){std::shared_ptr<Face> f = std::shared_ptr<Face>(new Face());f.get()->set_this_object(f);return f;}

        // Returns if a side of points cross the boundary or not
        bool model_maker::Face::cross_boundary(std::shared_ptr<Point> first_point, std::shared_ptr<Point> second_point) {return __cross_boundary(points(), first_point, second_point);}
        // Returns if a side of points cross the boundary of the exclusion part or not
        bool model_maker::Face::__cross_exclusion_boundary(std::vector<std::vector<std::shared_ptr<Point>>>& exclusion_points_copy, std::shared_ptr<model_maker::Point> first_point, std::shared_ptr<model_maker::Point> second_point) {
            for(int i = 0;i<static_cast<int>(exclusion_points_copy.size());i++) {
                if(__cross_boundary(exclusion_points_copy[i], first_point, second_point)) return true;
            }
            return false;
        };
        // Returns if a point is in the exclusion part
        bool model_maker::Face::__in_exclusion_part(std::vector<std::vector<std::shared_ptr<Point>>>& exclusion_points_copy, double x_to_check, double y_to_check) {
            for(int i = 0;i<static_cast<int>(exclusion_points_copy.size());i++) {
                if(__check_shape_content(exclusion_points_copy[i], x_to_check, y_to_check)) {
                    return true;
                }
            }
            return false;
        };

        // Triangulation helper
        void model_maker::Face::__triangulation_arrange_exclusion_points(std::vector<std::shared_ptr<model_maker::Point>>& points_copy, std::vector<std::vector<std::shared_ptr<Point>>>& exclusion_points_copy) {
            for(int i = 0;i<static_cast<int>(exclusion_points_copy.size());i++) {
                // Find the index of the nearest point
                bool position_found = false;
                bool reverse_current_exclusion = false;
                unsigned int start_position = 0;
                unsigned int current_p = 0;
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
                            start_position = j;
                            break;
                        }
                    }

                    // Change the point if necessary
                    if(!position_found) {
                    	exclusion_points_copy[i].push_back(exclusion_points_copy[i][0]);
                        exclusion_points_copy[i].erase(exclusion_points_copy[i].begin());
                        current_p++;
                        if(current_p >= exclusion_points_copy[i].size()){
                        	break;
                        }
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
        bool model_maker::Face::__triangulation_check_line_wrong(std::vector<std::shared_ptr<model_maker::Point>>& points_copy, std::shared_ptr<model_maker::Point> first_point, std::shared_ptr<model_maker::Point> second_point) {
            // Normal Check
            char on_boundary = __triangulation_cross_boundary(first_point, second_point);
            if(on_boundary != 0) {return true; }
            bool in_shape = __triangulation_check_shape_content(points_copy, (second_point.get()->x() + first_point.get()->x()) / 2.0, (second_point.get()->z() + first_point.get()->z()) / 2.0);
            if(!in_shape) {return true; }
            return false;
        };
        bool model_maker::Face::__triangulation_check_shape_content(std::vector<std::shared_ptr<model_maker::Point>>& points_copy, double point_to_test_x, double point_to_test_y) {
            // Check the main shape
            if(!__check_shape_content(points_copy, point_to_test_x, point_to_test_y)) return false;
            // Check each out shape
            for(int i = 0;i<static_cast<int>(a_exclusion_points.size());i++) {
                if(__check_shape_content(a_exclusion_points[i], point_to_test_x, point_to_test_y)) return false;
            }
            return true;
        };
        char model_maker::Face::__triangulation_cross_boundary(std::shared_ptr<model_maker::Point> first_point, std::shared_ptr<model_maker::Point> second_point) {
            // Check the main shape
            if(__cross_boundary(points(), first_point, second_point)) return 1;
            // Check each out shape
            for(int i = 0;i<static_cast<int>(a_exclusion_points.size());i++) {if(__cross_boundary(a_exclusion_points[i], first_point, second_point)) return 2;}
            return 0;
        };
        // Do a triangulation of a face
        std::shared_ptr<model_maker::Face::__Triangulation_Datas> model_maker::Face::triangulate() {std::shared_ptr<model_maker::Face::__Triangulation_Datas> datas = triangulate_start_datas();while(!datas.get()->finished) {triangulation_step(datas);}return datas;};
        // Returns the needed datas to start a triangulation
        std::shared_ptr<model_maker::Face::__Triangulation_Datas> model_maker::Face::triangulate_start_datas() {
            std::shared_ptr<model_maker::Face::__Triangulation_Datas> to_return = std::make_shared<model_maker::Face::__Triangulation_Datas>();
            to_return.get()->exclusion_points_copy = a_exclusion_points;
            to_return.get()->points_copy = points();

            // Remove the useless points
            for(int i = 0;i<static_cast<int>(to_return.get()->points_copy.size());i++) {
                if(to_return.get()->points_copy[i].get() == 0) {to_return.get()->points_copy.erase(to_return.get()->points_copy.begin() + i);i--;}
            }
            for(int i = 0;i<static_cast<int>(to_return.get()->exclusion_points_copy.size());i++) {
            	for(int j = 0;j<static_cast<int>(to_return.get()->exclusion_points_copy[i].size());j++) {
                    if(to_return.get()->exclusion_points_copy[i][j].get() == 0) {to_return.get()->exclusion_points_copy[i].erase(to_return.get()->exclusion_points_copy[i].begin() + j);j--;}
                }
                if(to_return.get()->exclusion_points_copy[i].size() <= 0) {to_return.get()->exclusion_points_copy.erase(to_return.get()->exclusion_points_copy.begin() + i);i--;}
            }

            // Do the triangulation
            points_for_rendering().clear();
            __triangulation_arrange_exclusion_points(to_return.get()->points_copy, to_return.get()->exclusion_points_copy);

            // Index points
            to_return.get()->points_index = std::vector<int>(to_return.get()->points_copy.size());
            for(int i = 0;i<static_cast<int>(to_return.get()->points_index.size());i++){to_return.get()->points_index[i]=i;}

            return to_return;
        };
        // Check a step of triangulation and returns if a step occurs
        #define TRIANGULATION_STEP_TEST_SAME_POINT -2
        #define TRIANGULATION_STEP_TEST_WRONG -1
        int model_maker::Face::__triangulation_step_test(std::shared_ptr<model_maker::Face::__Triangulation_Datas> triangulation_datas, std::shared_ptr<model_maker::Point> current_point, std::shared_ptr<model_maker::Point> current_point_1, std::shared_ptr<model_maker::Point> current_point_2){
            // Needed datas
            std::vector<std::shared_ptr<Point>>& created_sides = triangulation_datas.get()->created_sides;
            std::vector<std::shared_ptr<model_maker::Point>>& points_copy = triangulation_datas.get()->points_copy;

            // Check if the points are the same
            if(current_point.get() == current_point_1.get() || current_point_1.get() == current_point_2.get()) {return TRIANGULATION_STEP_TEST_SAME_POINT;}

            // Check if the out points are in the triangle
            if(__triangulation_check_line_wrong(points_copy, current_point, current_point_2)) {return TRIANGULATION_STEP_TEST_WRONG;}

            // Check if the line is not already existing
            bool must_continue = false;
            for(int i = 0;i<static_cast<int>(created_sides.size() - 1);i+=2){
                if(current_point.get()==created_sides[i].get()&&current_point_2.get()==created_sides[i + 1].get()){must_continue=true;break;}
                else if(current_point.get()==created_sides[i + 1].get()&&current_point_2.get()==created_sides[i].get()){must_continue=true;break;}
            }
            if(must_continue){return TRIANGULATION_STEP_TEST_WRONG;}

            return true;
        }
        bool model_maker::Face::triangulation_step(std::shared_ptr<model_maker::Face::__Triangulation_Datas> triangulation_datas) {
            std::vector<std::shared_ptr<Point>>& created_sides = triangulation_datas.get()->created_sides;
            unsigned int& current_i = triangulation_datas.get()->current_i;
            // std::vector<std::vector<std::shared_ptr<Point>>>& exclusion_points_copy = triangulation_datas.get()->exclusion_points_copy;
            std::vector<std::shared_ptr<model_maker::Point>>& points_copy = triangulation_datas.get()->points_copy;
            std::vector<int>& points_index = triangulation_datas.get()->points_index;
            if(points_copy.size() > 3) {
                // Asserts
                if(current_i + 2 >= points_copy.size()){
                    current_i = 0;points_copy.erase(points_copy.begin());points_index.erase(points_index.begin());
                    print(std::string("SCLS Math"), std::string("An error occured in the triangulation : deleted the first point to triangulate"));
                    return false;
                }

                // Get the point to test
                int needed_i = (current_i + triangulation_datas.get()->offset_i) % points_copy.size();
                int needed_i_1 = (needed_i + 1) % points_copy.size();
                int needed_i_2 = (needed_i_1 + 1) % points_copy.size();
                std::shared_ptr<model_maker::Point> current_point = points_copy[needed_i];;
                int current_point_index = points_index.at(needed_i);
                std::shared_ptr<model_maker::Point> current_point_1 = points_copy[needed_i_1];
                int current_point_1_index = points_index.at(needed_i_1);
                std::shared_ptr<model_maker::Point> current_point_2 = points_copy[needed_i_2];
                int current_point_2_index = points_index.at(needed_i_2);

                // Check if the points are the same
                int test = __triangulation_step_test(triangulation_datas, current_point, current_point_1, current_point_2);
                if(test == TRIANGULATION_STEP_TEST_SAME_POINT){points_copy.erase(points_copy.begin() + needed_i);return false;}
                else if(test == TRIANGULATION_STEP_TEST_WRONG){current_i++;return false;}

                // Add the point to the rendering
                created_sides.push_back(current_point); created_sides.push_back(current_point_2);
                points_copy.erase(points_copy.begin() + needed_i_1);points_index.erase(points_index.begin() + needed_i_1);
                points_for_rendering().push_back(current_point);points_for_rendering_index().push_back(current_point_index);
                points_for_rendering().push_back(current_point_1);points_for_rendering_index().push_back(current_point_1_index);
                points_for_rendering().push_back(current_point_2);points_for_rendering_index().push_back(current_point_2_index);
                triangulation_datas.get()->points_for_rendering.push_back(current_point);
                triangulation_datas.get()->points_for_rendering.push_back(current_point_1);
                triangulation_datas.get()->points_for_rendering.push_back(current_point_2);
                triangulation_datas->inner_points_index.push_back(current_point_index);
                triangulation_datas->inner_points_index.push_back(current_point_2_index);
                current_i = 0;
                //triangulation_datas.get()->offset_i = 0; // TEMP
                triangulation_datas.get()->offset_i = scls::random_int_between_included(0, points_copy.size() - 1);
            }
            else {
                // Trace the last triangle
                points_for_rendering().push_back(points_copy[0]);points_for_rendering_index().push_back(points_index.at(0));
                points_for_rendering().push_back(points_copy[1]);points_for_rendering_index().push_back(points_index.at(1));
                points_for_rendering().push_back(points_copy[2]);points_for_rendering_index().push_back(points_index.at(2));
                triangulation_datas.get()->points_for_rendering.push_back(points_copy.at(0));
                triangulation_datas.get()->points_for_rendering.push_back(points_copy.at(1));
                triangulation_datas.get()->points_for_rendering.push_back(points_copy.at(2));
                triangulation_datas->inner_points_index.push_back(points_index.at(0));
                triangulation_datas->inner_points_index.push_back(points_index.at(2));

                triangulation_datas.get()->finished = true;
            }
            return true;
        };

        // Do a triangulation of a convex face
        void model_maker::Face::triangulate_convex() {
            // Add each points for rendering
            points_for_rendering().clear();
            for(unsigned short i = 0;i<points().size() - 2;i++) {
                points_for_rendering().push_back(points()[0]);
                points_for_rendering().push_back(points()[i + 1]);
                points_for_rendering().push_back(points()[i + 2]);
            }
        };

        // Do a triangulation of a full face
        void model_maker::Face::__triangulate_full(std::vector<std::shared_ptr<model_maker::Point>> points_copy, bool last_point_is_first_point) {
            points_for_rendering().clear();

            // Trace each triangles
            unsigned int current_i = 0;
            unsigned int erase_offset = 0;
            for(int i = 0;i<static_cast<int>(points_copy.size());i++) {
                if(points_copy[i].get() == 0) {points_copy.erase((points_copy.begin() + i) - erase_offset);erase_offset++;}
            }
            // Check each points
            int limit = 3; if(!last_point_is_first_point) limit = 4;
            while(static_cast<int>(points_copy.size()) > limit) {
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
        }
        void model_maker::Face::triangulate_full() {__triangulate_full(points());}

        // Returns the face as a binary
        std::shared_ptr<Bytes_Set> model_maker::Face::binary() {
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
            to_return.get()->add_double(normal_x(), true);
            to_return.get()->add_double(normal_y(), true);
            to_return.get()->add_double(normal_z(), true);
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
        std::shared_ptr<Bytes_Set> model_maker::Face::binary_for_solid(unsigned int asker_id) {
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
        std::shared_ptr<Bytes_Set> model_maker::Face::binary_stl(double multiplication, unsigned int& triangle_count) {
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

                to_return.get()->add_datas(*a_points_for_rendering.at(i).get()->binary_stl(multiplication).get());
            }

            if(static_cast<int>(a_points_for_rendering.size()) > 0) {
                to_return.get()->add_ushort(triangle_count);
                triangle_count++;
            }

            return to_return;
        };
        // Returns the face as binary for VBO
        std::shared_ptr<Bytes_Set> model_maker::Face::binary_vbo(unsigned int asker_id, unsigned int& total_points, Point::_VBO_Types vbo_type) {
            std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

            // Add each points
            __Face_Datas_By_Solid& datas = solid_datas(asker_id);
            for(int i = 0;i<static_cast<int>(a_points_for_rendering.size());i++) {
                Point* current_point = a_points_for_rendering.at(i).get();
                to_return.get()->add_datas(*current_point->binary_vbo(id(), normal_x(), normal_y(), normal_z(), datas.texture_rect_x, datas.texture_rect_y, datas.texture_rect_width, datas.texture_rect_height, vbo_type).get());
                total_points++;
            }

            return to_return;
        };

        // Create a copy of the face with new points
        std::shared_ptr<Face> model_maker::Face::copy_with_new_points() {
            std::shared_ptr<Face> new_face = Face::new_face_object();
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

        // Set the exclusion points
        void model_maker::Face::set_exclusion_points(std::vector<std::shared_ptr<Point>> needed_points) {
            if(a_exclusion_points.size() <= 0){a_exclusion_points.push_back(std::vector<std::shared_ptr<Point>>());}
            a_exclusion_points[0] = needed_points;
            triangulate();
        }

        //*********
        //
        // The Solid class
        //
        //*********

        // Creates and return a new solid object
        std::shared_ptr<Solid> Solid::new_solid_object(){std::shared_ptr<Solid> to_return = std::shared_ptr<Solid>(new Solid());to_return.get()->set_this_object(to_return);return to_return;}

        // Solid constructor
        Solid::Solid() : Transform_Object_3D() { a_id = a_solids_number; a_solids_number++; };

        // Adds a face to the solid
        void Solid::add_face(std::string face_name, std::shared_ptr<Face> face_to_add){add_face(face_name, face_to_add, a_this_object.lock());}
        void Solid::add_face(std::string face_name, std::shared_ptr<Face> face_to_add, std::shared_ptr<Transform_Object_3D> face_parent){
            faces().push_back(face_to_add);
            face_to_add.get()->set_parent(face_parent);
            faces_by_name()[face_name] = face_to_add;
        };

        // Returns all the points in the solid
        std::vector<std::shared_ptr<Point>> Solid::all_points() {
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
        std::shared_ptr<Point> Solid::point_in_grid(int x, int z) {
            // Create the search function
            std::vector<std::shared_ptr<Point>> points = all_points();
            return point_in_grid(points, x, z);
        };
        // Returns a point in a grid with a given set of point
        std::shared_ptr<Point> Solid::point_in_grid(std::vector<std::shared_ptr<Point>>& points, int x, int z) {
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

        // Fills between a face and a point
        void Solid::fill_faces_between_face_point_one_texture(std::shared_ptr<Face> face_1, double point_x, double point_y, double point_z, std::shared_ptr<Transform_Object_3D> face_parent) {
            // Needed datas
            std::shared_ptr<Point> needed_point = std::make_shared<Point>();
            needed_point.get()->move_xyz(point_x, point_y, point_z);

            // Create the side faces
            unsigned int face_number = 0;
            for(int i = 0;i<static_cast<int>(face_1.get()->points().size()) - 1;i++) {
                // Get the needed points
                std::shared_ptr<Face> face_center = Face::new_face_object();
                std::shared_ptr<Point>& first_point = face_1.get()->points()[i];
                first_point.get()->set_texture_x(face_center.get()->id(), 1); first_point.get()->set_texture_y(face_center.get()->id(), 1);
                first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& second_point = face_1.get()->points()[i + 1];
                second_point.get()->set_texture_x(face_center.get()->id(), 0); second_point.get()->set_texture_y(face_center.get()->id(), 1);
                second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& third_point = needed_point;
                third_point.get()->set_texture_x(face_center.get()->id(), 1); third_point.get()->set_texture_y(face_center.get()->id(), 0);
                third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);

                // Create the face
                face_center.get()->points().push_back(first_point);
                face_center.get()->points().push_back(second_point);
                face_center.get()->points().push_back(third_point);
                face_center.get()->points_for_rendering().push_back(first_point);
                face_center.get()->points_for_rendering().push_back(second_point);
                face_center.get()->points_for_rendering().push_back(third_point);
                face_center.get()->set_normal_x(1); face_center.get()->set_normal_z(1);
                face_center.get()->set_parent(face_parent);
                faces().push_back(face_center);
                faces_by_name()["face-" + std::to_string(face_number)] = face_center;
                face_number++;
                // Get the normal of the face
                double x_value = face_1.get()->points()[i + 1].get()->x() - face_1.get()->points()[i].get()->x();
                double z_value = face_1.get()->points()[i + 1].get()->z() - face_1.get()->points()[i].get()->z();
                face_center.get()->set_normal_x(z_value); face_center.get()->set_normal_z(-x_value);
            }
        }

        // Fills between two faces
        void Solid::fill_faces_point_by_point(std::shared_ptr<Face> face_1, std::shared_ptr<Face> face_2, std::shared_ptr<Transform_Object_3D> face_parent) {
            // Create the side faces
            unsigned int face_number = 0;
            for(int i = 0;i<static_cast<int>(face_1.get()->points().size()) - 1;i++) {
                // Get the needed points
                std::shared_ptr<Face> face_center = Face::new_face_object();
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
                face_center.get()->set_normal_x(1); face_center.get()->set_normal_z(1);
                face_center.get()->set_parent(face_parent);
                faces().push_back(face_center);
                faces_by_name()["face-" + std::to_string(face_number)] = face_center;
                face_number++;
                // Get the normal of the face
                double x_value = face_1.get()->points()[i + 1].get()->x() - face_1.get()->points()[i].get()->x();
                double z_value = face_1.get()->points()[i + 1].get()->z() - face_1.get()->points()[i].get()->z();
                face_center.get()->set_normal_x(z_value); face_center.get()->set_normal_z(-x_value);
            }

            // Create the last face
            // Get the needed points
            std::shared_ptr<Face> face_center = Face::new_face_object();
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
            faces_by_name()["face-" + std::to_string(face_number)] = face_center;
            face_number++;
            // Get the normal of the face
            double x_value = face_1.get()->points()[face_1.get()->points().size() - 1].get()->x() - face_1.get()->points()[0].get()->x();
            double z_value = face_1.get()->points()[face_1.get()->points().size() - 1].get()->z() - face_1.get()->points()[0].get()->z();
            face_center.get()->set_normal_x(-z_value); face_center.get()->set_normal_z(x_value);

            // Create the faces of each holes
            for(int i = 0;i<static_cast<int>(face_1.get()->exclusion_points().size()) - 1;i++) {
                // Get the needed points
                std::shared_ptr<Face> face_center = Face::new_face_object();
                std::shared_ptr<Point>& first_point = face_1.get()->exclusion_points()[i];
                first_point.get()->set_texture_x(face_center.get()->id(), 1); first_point.get()->set_texture_y(face_center.get()->id(), 1);
                first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& second_point = face_1.get()->exclusion_points()[i + 1];
                second_point.get()->set_texture_x(face_center.get()->id(), 0); second_point.get()->set_texture_y(face_center.get()->id(), 1);
                second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& third_point = face_2.get()->exclusion_points()[i];
                third_point.get()->set_texture_x(face_center.get()->id(), 1); third_point.get()->set_texture_y(face_center.get()->id(), 0);
                third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& fourth_point = face_2.get()->exclusion_points()[i + 1];
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
                faces_by_name()["face-" + std::to_string(face_number)] = face_center;
                face_number++;
            }

            if(static_cast<int>(face_1.get()->exclusion_points().size()) > 1) {
                // Create the last face
                // Get the needed points
                std::shared_ptr<Face> face_center = Face::new_face_object();
                std::shared_ptr<Point>& first_point = face_1.get()->exclusion_points()[face_1.get()->exclusion_points().size() - 1];
                first_point.get()->set_texture_x(face_center.get()->id(), 1); first_point.get()->set_texture_y(face_center.get()->id(), 1);
                first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& second_point = face_1.get()->exclusion_points()[0];
                second_point.get()->set_texture_x(face_center.get()->id(), 0); second_point.get()->set_texture_y(face_center.get()->id(), 1);
                second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& third_point = face_2.get()->exclusion_points()[face_2.get()->exclusion_points().size() - 1];
                third_point.get()->set_texture_x(face_center.get()->id(), 1); third_point.get()->set_texture_y(face_center.get()->id(), 0);
                third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& fourth_point = face_2.get()->exclusion_points()[0];
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
                faces_by_name()["face-" + std::to_string(face_number)] = face_center;
                face_number++;
            }
        }
        // Fills between two faces
        void Solid::fill_faces_point_by_point_one_texture(std::shared_ptr<Face> face_1, std::shared_ptr<Face> face_2){fill_faces_point_by_point_one_texture(face_1, face_2,std::shared_ptr<Transform_Object_3D>());}
        void Solid::fill_faces_point_by_point_one_texture(std::shared_ptr<Face> face_1, std::shared_ptr<Face> face_2, std::shared_ptr<Transform_Object_3D> face_parent){fill_faces_point_by_point_one_texture(face_1, face_2, 0, 0, 1, 1, face_parent);}
        void Solid::fill_faces_point_by_point_one_texture(std::shared_ptr<Face> face_1, std::shared_ptr<Face> face_2, double texture_x, double texture_y, double texture_width, double texture_height, std::shared_ptr<Transform_Object_3D> face_parent) {
            fill_faces_point_by_point_one_texture(face_1, face_2, texture_x, texture_y, texture_width, texture_height, false, face_parent);
        }
        void Solid::fill_faces_point_by_point_one_texture(std::shared_ptr<Face> face_1, std::shared_ptr<Face> face_2, double texture_x, double texture_y, double texture_width, double texture_height, bool reverse_texture_y, std::shared_ptr<Transform_Object_3D> face_parent) {
            // Needed datas
            double current_face_part = 0;
            double face_number = static_cast<int>(face_1.get()->points().size());
            double face_width = 1.0 / face_number;
            double inner_texture_position = 0;
            double inner_texture_width = 1;
            double inner_face_width = 1.0 / (face_1.get()->exclusion_points().size());

            // Create the side faces
            double current_texture_position = 0;face_number = 0;
            for(int i = 0;i<static_cast<int>(face_1.get()->points().size()) - 1;i++) {
                // Get the needed points
                std::shared_ptr<Face> face_center = Face::new_face_object();
                std::shared_ptr<Point>& first_point = face_1.get()->points()[i];
                std::shared_ptr<Point>& second_point = face_1.get()->points()[i + 1];
                std::shared_ptr<Point>& third_point = face_2.get()->points()[i];
                std::shared_ptr<Point>& fourth_point = face_2.get()->points()[i + 1];

                // Texturing
                if(reverse_texture_y) {
                    first_point.get()->set_texture_x(face_center.get()->id(), texture_width - current_texture_position); first_point.get()->set_texture_y(face_center.get()->id(), texture_y + texture_height);
                    first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                    second_point.get()->set_texture_x(face_center.get()->id(), texture_width - (current_texture_position + face_width)); second_point.get()->set_texture_y(face_center.get()->id(), texture_y + texture_height);
                    second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                    third_point.get()->set_texture_x(face_center.get()->id(), texture_width - current_texture_position); third_point.get()->set_texture_y(face_center.get()->id(), texture_y);
                    third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                    fourth_point.get()->set_texture_x(face_center.get()->id(), texture_width - (current_texture_position + face_width)); fourth_point.get()->set_texture_y(face_center.get()->id(), texture_y);
                    fourth_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); fourth_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                }
                else {
                    first_point.get()->set_texture_x(face_center.get()->id(), texture_width - current_texture_position); first_point.get()->set_texture_y(face_center.get()->id(), texture_y);
                    first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                    second_point.get()->set_texture_x(face_center.get()->id(), texture_width - (current_texture_position + face_width)); second_point.get()->set_texture_y(face_center.get()->id(), texture_y);
                    second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                    third_point.get()->set_texture_x(face_center.get()->id(), texture_width - current_texture_position); third_point.get()->set_texture_y(face_center.get()->id(), texture_y + texture_height);
                    third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                    fourth_point.get()->set_texture_x(face_center.get()->id(), texture_width - (current_texture_position + face_width)); fourth_point.get()->set_texture_y(face_center.get()->id(), texture_y + texture_height);
                    fourth_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); fourth_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                }

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
                face_center.get()->set_normal_x(1); face_center.get()->set_normal_z(1);
                face_center.get()->set_parent(face_parent);
                faces().push_back(face_center);
                faces_by_name()["face-" + std::to_string(face_number)] = face_center;
                face_number++;current_texture_position += face_width;
                // Get the normal of the face
                double x_value = face_1.get()->points()[i + 1].get()->x() - face_1.get()->points()[i].get()->x();
                double z_value = face_1.get()->points()[i + 1].get()->z() - face_1.get()->points()[i].get()->z();
                face_center.get()->set_normal_x(z_value); face_center.get()->set_normal_z(-x_value);
            }

            // Create the last face
            // Get the needed points
            std::shared_ptr<Face> face_center = Face::new_face_object();
            std::shared_ptr<Point>& first_point = face_1.get()->points()[face_1.get()->points().size() - 1];
            std::shared_ptr<Point>& second_point = face_1.get()->points()[0];
            std::shared_ptr<Point>& third_point = face_2.get()->points()[face_2.get()->points().size() - 1];
            std::shared_ptr<Point>& fourth_point = face_2.get()->points()[0];

            // Texturing
            if(reverse_texture_y) {
                first_point.get()->set_texture_x(face_center.get()->id(), texture_width - current_texture_position); first_point.get()->set_texture_y(face_center.get()->id(), texture_y + texture_height);
                first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                second_point.get()->set_texture_x(face_center.get()->id(), texture_width - 1); second_point.get()->set_texture_y(face_center.get()->id(), texture_y + texture_height);
                second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                third_point.get()->set_texture_x(face_center.get()->id(), texture_width - current_texture_position); third_point.get()->set_texture_y(face_center.get()->id(), texture_y);
                third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                fourth_point.get()->set_texture_x(face_center.get()->id(), texture_width - 1); fourth_point.get()->set_texture_y(face_center.get()->id(), texture_y);
                fourth_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); fourth_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
            }
            else {
                first_point.get()->set_texture_x(face_center.get()->id(), texture_width - current_texture_position); first_point.get()->set_texture_y(face_center.get()->id(), texture_y);
                first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                second_point.get()->set_texture_x(face_center.get()->id(), texture_width - 1); second_point.get()->set_texture_y(face_center.get()->id(), texture_y);
                second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                third_point.get()->set_texture_x(face_center.get()->id(), texture_width - current_texture_position); third_point.get()->set_texture_y(face_center.get()->id(), texture_y + texture_height);
                third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                fourth_point.get()->set_texture_x(face_center.get()->id(), texture_width - 1); fourth_point.get()->set_texture_y(face_center.get()->id(), texture_y + texture_height);
                fourth_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); fourth_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
            }

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
            faces_by_name()["face-" + std::to_string(face_number)] = face_center;
            face_number++;
            // Get the normal of the face
            double x_value = face_1.get()->points()[face_1.get()->points().size() - 1].get()->x() - face_1.get()->points()[0].get()->x();
            double z_value = face_1.get()->points()[face_1.get()->points().size() - 1].get()->z() - face_1.get()->points()[0].get()->z();
            face_center.get()->set_normal_x(-z_value); face_center.get()->set_normal_z(x_value);

            // Create the faces of each holes
            double current_inner_texture_position = inner_texture_position;
            for(int i = 0;i<static_cast<int>(face_1.get()->exclusion_points().size()) - 1;i++) {
                // Get the needed points
                std::shared_ptr<Face> face_center = Face::new_face_object();
                std::shared_ptr<Point>& first_point = face_1.get()->exclusion_points()[i];
                first_point.get()->set_texture_x(face_center.get()->id(), current_inner_texture_position); first_point.get()->set_texture_y(face_center.get()->id(), 1);
                first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& second_point = face_1.get()->exclusion_points()[i + 1];
                second_point.get()->set_texture_x(face_center.get()->id(), current_inner_texture_position + inner_face_width); second_point.get()->set_texture_y(face_center.get()->id(), 1);
                second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& third_point = face_2.get()->exclusion_points()[i];
                third_point.get()->set_texture_x(face_center.get()->id(), current_inner_texture_position); third_point.get()->set_texture_y(face_center.get()->id(), 0);
                third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& fourth_point = face_2.get()->exclusion_points()[i + 1];
                fourth_point.get()->set_texture_x(face_center.get()->id(), current_inner_texture_position + inner_face_width); fourth_point.get()->set_texture_y(face_center.get()->id(), 0);
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
                faces_by_name()["face-" + std::to_string(face_number)] = face_center;
                face_number++;current_inner_texture_position += inner_face_width;
            }

            if(static_cast<int>(face_1.get()->exclusion_points().size()) > 1) {
                // Create the last face
                // Get the needed points
                std::shared_ptr<Face> face_center = Face::new_face_object();
                std::shared_ptr<Point>& first_point = face_1.get()->exclusion_points()[face_1.get()->exclusion_points().size() - 1];
                first_point.get()->set_texture_x(face_center.get()->id(), current_inner_texture_position); first_point.get()->set_texture_y(face_center.get()->id(), 1);
                first_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); first_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& second_point = face_1.get()->exclusion_points()[0];
                second_point.get()->set_texture_x(face_center.get()->id(), current_inner_texture_position + inner_face_width); second_point.get()->set_texture_y(face_center.get()->id(), 1);
                second_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); second_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& third_point = face_2.get()->exclusion_points()[face_2.get()->exclusion_points().size() - 1];
                third_point.get()->set_texture_x(face_center.get()->id(), current_inner_texture_position); third_point.get()->set_texture_y(face_center.get()->id(), 0);
                third_point.get()->set_texture_multiplier_x(face_center.get()->id(), 1); third_point.get()->set_texture_multiplier_y(face_center.get()->id(), 2);
                std::shared_ptr<Point>& fourth_point = face_2.get()->exclusion_points()[0];
                fourth_point.get()->set_texture_x(face_center.get()->id(), current_inner_texture_position + inner_face_width); fourth_point.get()->set_texture_y(face_center.get()->id(), 0);
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
                faces_by_name()["face-" + std::to_string(face_number)] = face_center;
                face_number++;
            }//*/
        }

        // Returns the solid as binary
        std::shared_ptr<Bytes_Set> Solid::binary() {
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
        std::shared_ptr<Bytes_Set> Solid::binary_complete() {
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
        // Returns the solid as binary STL format
        std::shared_ptr<Bytes_Set> Solid::binary_stl(double multiplication, unsigned int& total_triangle) {
            // Get the size of the binary
            std::vector<std::shared_ptr<Bytes_Set>> binaries = std::vector<std::shared_ptr<Bytes_Set>>();
            unsigned int current_position = 0;
            unsigned int total_size = 8;
            for(int i = 0;i<static_cast<int>(a_faces.size());i++) {
                std::shared_ptr<Bytes_Set> current_binary = a_faces[i].get()->binary_stl(multiplication, total_triangle);
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
        std::shared_ptr<Bytes_Set> Solid::binary_stl_complete(double multiplication) {
            std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>();

            // Add the header of the file
            std::string header = "";
            for(int i = 0;i<80;i++) header += " ";
            to_return.get()->add_string(header);
            to_return.get()->add_uint(0);

            // Add each points
            unsigned int total_triangle = 0;
            to_return.get()->add_datas(*binary_stl(multiplication, total_triangle).get());
            to_return.get()->put_uint(total_triangle, 80);

            return to_return;
        };
        std::shared_ptr<Bytes_Set> Solid::binary_stl_complete() {return binary_stl_complete(1);};
        // Returns the solid as binary VBO
        std::shared_ptr<Bytes_Set> Solid::__binary_vbo(unsigned int& total_points, Point::_VBO_Types vbo_type) {
            // Get the size of the binary
            std::vector<std::shared_ptr<Bytes_Set>> binaries = std::vector<std::shared_ptr<Bytes_Set>>();
            unsigned int current_position = 0;
            unsigned int total_size = 0;
            for(int i = 0;i<static_cast<int>(a_faces.size());i++) {
                std::shared_ptr<Bytes_Set> current_binary = a_faces[i].get()->binary_vbo(id(), total_points, vbo_type);
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
        std::shared_ptr<Bytes_Set> Solid::binary_vbo_complete(Point::_VBO_Types vbo_type) {
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

        //*********
        //
        // The Solid_Group class
        //
        //*********

        // Returns all the faces in each solids
        std::vector<std::shared_ptr<Face>> Solid_Group::all_faces() {
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
        }

        // Returns all the points in each solids
        std::vector<std::shared_ptr<Point>> Solid_Group::all_points() {
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
        }

        // Add a solid to the group
        void Solid_Group::add_solid(const std::shared_ptr<Solid>& solid_to_add) {solid_to_add.get()->set_parent(attached_transformation());a_solid.push_back(solid_to_add);}

        // Returns the face as binary
        std::shared_ptr<Bytes_Set> Solid_Group::binary() {
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
        }

        // Returns the solid group as binary STL format
        std::shared_ptr<Bytes_Set> Solid_Group::binary_stl(double multiplication) {
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
                std::shared_ptr<Bytes_Set> current_binary = a_solid[i].get()->binary_stl(multiplication, total_triangle);
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
        }

        // Returns the solid as binary VBO
        std::shared_ptr<Bytes_Set> Solid_Group::binary_vbo(Point::_VBO_Types vbo_type) {
            std::shared_ptr<Bytes_Set> to_return = Solid::__shader_arguments(vbo_type);

            // Add each points
            unsigned int total_points = 0;
            unsigned int total_points_position = to_return.get()->datas_size();
            to_return.get()->add_uint(0, true);
            for(int i = 0;i<static_cast<int>(a_solid.size());i++) {
                to_return.get()->add_datas(*a_solid[i].get()->__binary_vbo(total_points, vbo_type).get());
            }
            to_return.get()->put_uint(total_points, total_points_position, true);

            return to_return;
        }

        // Loads the solid group from binary
        std::shared_ptr<Solid_Group> load_solid_group_from_binary(std::shared_ptr<Bytes_Set>& file_content) {
            // Loader for the model
            std::shared_ptr<__Model_Loader> loader = __model_loader_filled(file_content);
            std::shared_ptr<Solid_Group> to_return = std::make_shared<Solid_Group>();

            // Create each solid
            for(std::map<unsigned int, __Model_Loader::Solid_For_Loading>::iterator it = loader.get()->solids().begin();it!=loader.get()->solids().end();it++) {
                std::shared_ptr<Solid> current_solid = Solid::__load_from_loader(it->first, it->second, loader);
                to_return.get()->add_solid(current_solid);
            }

            return to_return;
        }
        std::shared_ptr<Solid_Group> load_solid_group_from_binary(std::string file_path){std::shared_ptr<Bytes_Set> bytes = std::make_shared<Bytes_Set>();bytes.get()->load_from_file(file_path);return load_solid_group_from_binary(bytes);};

        //*********
        //
        // Polygon handling
        //
        //*********

        // Loads a polygon from binary
        std::shared_ptr<Polygon> load_polygon_binary(std::shared_ptr<Bytes_Set> binary);
        // Returns a polygon in binary
        std::shared_ptr<Bytes_Set> polygon_binary(std::string path, Polygon* polygon_to_save);

        // Returns a face which make a simple polygon
        std::shared_ptr<Face> polygon(std::shared_ptr<Polygon> polygon, bool reverse_texture_x, bool reverse_texture_z, double y) {
            std::shared_ptr<Face> to_return = Face::new_face_object();
            to_return.get()->set_y(y);
            to_return.get()->set_normal_y(1);

            // Add each points
            std::vector<std::shared_ptr<Point>>& points = polygon.get()->points;
            for(int i = 0;i<static_cast<int>(points.size());i++) {
                std::shared_ptr<Point> to_add = std::make_shared<Point>(*points.at(i).get());
                to_add.get()->set_this_object(to_add);
                to_add.get()->reset();
                to_add.get()->set_parent(to_return); to_return.get()->add_child(to_add);

                // Calculate the texture position of the point
                double x_texture = to_add.get()->x();
                double y_texture = to_add.get()->z();
                x_texture -= polygon.get()->min_x;
                y_texture -= polygon.get()->min_z;
                if(!reverse_texture_x) {x_texture *= -1;} x_texture++;
                if(!reverse_texture_z) {y_texture *= -1;} y_texture++;
                to_add.get()->set_texture_x(to_return.get()->id(), x_texture);
                to_add.get()->set_texture_y(to_return.get()->id(), y_texture);
                to_add.get()->set_texture_multiplier_x(to_return.get()->id(), 1);
                to_add.get()->set_texture_multiplier_y(to_return.get()->id(), 0);
                to_add.get()->set_texture_multiplier_z(to_return.get()->id(), 2);

                to_return.get()->points().push_back(to_add);
            }
            to_return.get()->triangulate();

            return to_return;
        }
        std::shared_ptr<Face> polygon(std::vector<Point> points, bool reverse_texture_x, bool reverse_texture_z, double y) {
            // Create the polygon
            std::shared_ptr<Polygon> created_polygon = std::make_shared<Polygon>();
            for(int i = 0;i<static_cast<int>(points.size());i++) {
                Point& current_point = points[i];
                created_polygon.get()->points.push_back(std::make_shared<Point>(current_point));
                if(current_point.x() > created_polygon.get()->max_x) created_polygon.get()->max_x = current_point.x();
                if(current_point.x() < created_polygon.get()->min_x) created_polygon.get()->min_x = current_point.x();
                if(current_point.z() > created_polygon.get()->max_z) created_polygon.get()->max_z = current_point.z();
                if(current_point.z() < created_polygon.get()->min_z) created_polygon.get()->min_z = current_point.z();
            }
            created_polygon.get()->size_x = created_polygon.get()->max_x - created_polygon.get()->min_x;
            created_polygon.get()->min_z = created_polygon.get()->max_z - created_polygon.get()->min_z;

            return polygon(created_polygon, reverse_texture_x, reverse_texture_z, y);
        }

        // Returns a face which make a simple polygon in 3D
        std::shared_ptr<Solid> polygon_3d(std::vector<Point> points) {
            // Create the base faces
            std::shared_ptr<Face> face_1 = polygon(points, false, false, 0.5); face_1.get()->set_normal_y(1);
            std::shared_ptr<Face> face_2 = polygon(points, false, true, -0.5); face_2.get()->set_normal_y(-1);

            // Create the solid
            std::shared_ptr<Solid> to_return = Solid::new_solid_object();
            to_return.get()->add_face("top", face_1, to_return);
            to_return.get()->add_face("bottom", face_2, to_return);
            to_return.get()->fill_faces_point_by_point(face_1, face_2, to_return);

            return to_return;
        }
        std::shared_ptr<Solid> polygon_3d(std::shared_ptr<Polygon> points) {return polygon_3d(points.get()->points);}
        std::shared_ptr<Solid> polygon_3d(std::vector<std::shared_ptr<Point>> points) {std::vector<Point> points_to_pass = std::vector<Point>();for(int i = 0;i<static_cast<int>(points.size());i++) {points_to_pass.push_back(*points[i].get());}return polygon_3d(points_to_pass);}

        //*********
        //
        // Regular polygons handling
        //
        //*********

        // Returns a simple regular polygon in points 2D
        std::vector<scls::Point_2D> regular_polygon_points_2d(unsigned short side_number) {
            // Configurate the creation
            double angle_sum = static_cast<double>((side_number - 2) * SCLS_PI);
            double current_angle = 0;
            double one_angle = SCLS_PI - angle_sum / static_cast<double>(side_number);
            std::vector<scls::Point_2D> to_return = std::vector<scls::Point_2D>(side_number);

            // Calculate each positions
            double angle_offset = 0;//-SCLS_PI / (3.0);
            for(unsigned short i = 0;i<side_number;i++) {
                // Calculate the position of the point
                double x = std::cos(current_angle + angle_offset);
                double y = std::sin(current_angle + angle_offset);
                to_return[i] = scls::Point_2D(x, y);
                current_angle += one_angle;
            }

            return to_return;
        }

        // Returns a simple regular polygon
        std::shared_ptr<Polygon> regular_polygon_points(unsigned short side_number, int needed_side_number, int side_start, double y, double scale) {
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
                if(x > max_x) {max_x = x;} if(x < min_x) {min_x = x;}
                if(z > max_z) {max_z = z;} if(z < min_z) {min_z = z;}

                if((i >= side_start && i < needed_side_number + side_start) || (i >= side_number + side_start)) {
                    // Create the point
                    std::shared_ptr<Point> point = std::make_shared<Point>();
                    point.get()->move_xyz(x, y, z);
                    to_return.get()->points.push_back(point);
                }

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
                Point* point = to_return.get()->points[i].get();
                point->set_x(point->x() * multiplier);
                point->set_z(point->z() * multiplier);
            }
            to_return.get()->max_x *= multiplier;
            to_return.get()->max_z *= multiplier;
            to_return.get()->min_x *= multiplier;
            to_return.get()->min_z *= multiplier;

            return to_return;
        }
        std::shared_ptr<Polygon> regular_polygon_points(unsigned short side_number, double y, double scale){return regular_polygon_points(side_number, side_number, 0, y, scale);};
        std::shared_ptr<Polygon> regular_polygon_points(unsigned short side_number){return regular_polygon_points(side_number, side_number, 0, 0, 1);};
        // Returns a face which make a simple regular polygon
        std::shared_ptr<Face> regular_polygon(unsigned short side_number, unsigned short needed_side_number, int side_start, bool reverse_texture_x, bool reverse_texture_z, double y) {
            std::shared_ptr<Face> to_return = Face::new_face_object();
            to_return.get()->set_y(y);
            to_return.get()->set_normal_y(1);

            // Add each points
            std::shared_ptr<Polygon> points = regular_polygon_points(side_number, needed_side_number, side_start, 0, 1);
            for(int i = 0;i<static_cast<int>(points.get()->points.size());i++) {
                std::shared_ptr<Point> to_add = std::make_shared<Point>(*points.get()->points.at(i).get());
                to_add.get()->set_this_object(to_add);
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
                to_add.get()->set_texture_multiplier_x(to_return.get()->id(), 1);
                to_add.get()->set_texture_multiplier_y(to_return.get()->id(), 0);
                to_add.get()->set_texture_multiplier_z(to_return.get()->id(), 2);

                to_return.get()->points().push_back(to_add);
            }
            to_return.get()->triangulate_convex();

            return to_return;
        }
        std::shared_ptr<Face> regular_polygon(unsigned short side_number){return regular_polygon(side_number, side_number, 0, true, true, 0);}

        // Returns a face which make a simple regular polygon in 3D
        std::shared_ptr<Solid> regular_polygon_3d(unsigned short side_number, unsigned short needed_side_number, int side_start){
            // Create the base faces
            std::shared_ptr<Face> face_1 = regular_polygon(side_number, needed_side_number, side_start, true, true, 0.5); face_1.get()->set_normal_y(1);
            std::shared_ptr<Face> face_2 = regular_polygon(side_number, needed_side_number, side_start, true, true, -0.5); face_2.get()->set_normal_y(-1);

            // Create the solid
            std::shared_ptr<Solid> to_return = Solid::new_solid_object();
            to_return.get()->add_face("top", face_1, to_return);
            to_return.get()->add_face("bottom", face_2, to_return);
            to_return.get()->fill_faces_point_by_point_one_texture(face_1, face_2, to_return);

            return to_return;
        }
        std::shared_ptr<Solid> regular_polygon_3d(unsigned short side_number){return regular_polygon_3d(side_number, side_number, 0);}

        // Returns a squared-solid with a frame of faces in its top
        std::shared_ptr<Solid> frame_polyhedron_in_top(unsigned int frame_width, unsigned int frame_height) {
            // Create the solid
            std::shared_ptr<Solid> to_return = Solid::new_solid_object();

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
        }
    }
}
