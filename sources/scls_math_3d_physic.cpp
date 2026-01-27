//******************
//
// scls_math_3d_physic.cpp
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
// This file contains the source code of scls_math_3d_physic.h.
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

// Needed includes
#include "../scls_math_directory/scls_math_3d_physic.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //******************
    // Physic object
    //******************

    // Getters and setters
    scls::Fraction Collision_3D::absolute_height() const {return attached_transform()->absolute_scale_y();};
    scls::Fraction Collision_3D::absolute_width() const {return attached_transform()->absolute_scale_x();};
    double Collision_3D::absolute_x() const {return attached_transform()->absolute_x();};
    double Collision_3D::absolute_x_1() const{return a_x_1.to_double();};
    double Collision_3D::absolute_x_2() const{return a_x_2.to_double();};
    double Collision_3D::absolute_y() const {return attached_transform()->absolute_y();};
    double Collision_3D::absolute_y_1() const{return a_y_1.to_double();};
    double Collision_3D::absolute_y_2() const{return a_y_2.to_double();};
    Physic_Object_3D* Collision_3D::attached_physic()const{return a_attached_physic.lock().get();};
    scls::Transform_Object_3D* Collision_3D::attached_transform()const{return a_attached_transform.lock().get();};
    double Collision_3D::direct_x_1() const {return a_x_1.to_double();};
    double Collision_3D::direct_x_2() const {return a_x_2.to_double();};
    double Collision_3D::direct_y_1() const {return a_y_1.to_double();};
    double Collision_3D::direct_y_2() const {return a_y_2.to_double();};
    double Collision_3D::max_absolute_x() const {return attached_transform()->max_absolute_x();};
    double Collision_3D::max_absolute_x_next() const {return attached_transform()->max_absolute_x_next();};
    double Collision_3D::max_absolute_y() const {return attached_transform()->max_absolute_y();};
    double Collision_3D::max_absolute_y_next() const {return attached_transform()->max_absolute_y_next();};
    double Collision_3D::max_absolute_z_next() const {return attached_transform()->max_absolute_z_next();};
    double Collision_3D::min_absolute_x() const {return attached_transform()->min_absolute_x();};
    double Collision_3D::min_absolute_x_next() const {return attached_transform()->min_absolute_x_next();};
    double Collision_3D::min_absolute_y() const {return attached_transform()->min_absolute_y();};
    double Collision_3D::min_absolute_y_next() const {return attached_transform()->min_absolute_y_next();};
    double Collision_3D::min_absolute_z_next() const {return attached_transform()->min_absolute_z_next();};
    Point_3D Collision_3D::position_next() const {return attached_transform()->position_next();};
    void Collision_3D::set_type(Collision_Type_3D new_type){a_type = new_type;};
    void Collision_3D::set_x_1(scls::Fraction new_x_1){a_x_1 = new_x_1;};
    void Collision_3D::set_x_2(scls::Fraction new_x_2){a_x_2 = new_x_2;};
    void Collision_3D::set_y_1(scls::Fraction new_y_1){a_y_1 = new_y_1;};
    void Collision_3D::set_y_2(scls::Fraction new_y_2){a_y_2 = new_y_2;};
    Collision_Type_3D Collision_3D::type()const{return a_type;};
    double Collision_3D::x_1() const {return attached_transform()->x() + a_x_1.to_double();};
    double Collision_3D::x_2() const {return attached_transform()->x() + a_x_2.to_double();};
    double Collision_3D::y_1() const {return attached_transform()->y() + a_y_1.to_double();};
    double Collision_3D::y_2() const {return attached_transform()->y() + a_y_2.to_double();};

    // Add a line / rect collision to the graphic object
    void Physic_Object_3D::add_collision(std::shared_ptr<Collision_3D> collision){a_collisions.push_back(collision);};

    // Checks if a collision occurs with an another object
    struct Collision_Result_3D{
        Collision_Result_3D(){};
        Collision_Result_3D(std::shared_ptr<Collision_3D::Collision_Event> c_1):Collision_Result_3D(c_1, std::shared_ptr<Collision_3D::Collision_Event>()){}
        Collision_Result_3D(std::shared_ptr<Collision_3D::Collision_Event> c_1, std::shared_ptr<Collision_3D::Collision_Event> c_2):collision_1(c_1),collision_2(c_2){
            c_1.get()->happens = true;if(c_2.get() != 0){c_2.get()->happens = true;}
            if(c_1.get() != 0 && c_2.get() != 0){c_1.get()->set_other_collision_event(c_2);c_2.get()->set_other_collision_event(c_1);}
        };

        // Needed collisions
        std::shared_ptr<Collision_3D::Collision_Event> collision_1;std::shared_ptr<Collision_3D::Collision_Event> collision_2;
    };
    Collision_Result_3D __check_collision_cube_cube(std::shared_ptr<Collision_3D> collision_1, std::shared_ptr<Collision_3D> collision_2, Physic_Object_3D* dynamic_object_1, Physic_Object_3D* object_2){
        // Check X
        bool x_1 = (collision_1->max_absolute_x_next() > collision_2->min_absolute_x_next() && collision_2->max_absolute_x_next() > collision_1->max_absolute_x_next());
        bool x_2 = (collision_2->max_absolute_x_next() > collision_1->min_absolute_x_next() && collision_1->max_absolute_x_next() > collision_2->max_absolute_x_next());
        bool x_3 = (collision_2->max_absolute_x_next() >= collision_1->max_absolute_x_next() && collision_1->min_absolute_x_next() >= collision_2->min_absolute_x_next());
        bool x_4 = (collision_1->max_absolute_x_next() >= collision_2->max_absolute_x_next() && collision_2->min_absolute_x_next() >= collision_1->min_absolute_x_next());
        if(!(x_1 || x_2 || x_3 || x_4)){return Collision_Result_3D();}

        // Check Y
        bool y_1 = (collision_1->max_absolute_y_next() > collision_2->min_absolute_y_next() && collision_2->max_absolute_y_next() > collision_1->max_absolute_y_next());
        bool y_2 = (collision_2->max_absolute_y_next() > collision_1->min_absolute_y_next() && collision_1->max_absolute_y_next() > collision_2->max_absolute_y_next());
        bool y_3 = (collision_2->max_absolute_y_next() >= collision_1->max_absolute_y_next() && collision_1->min_absolute_y_next() >= collision_2->min_absolute_y_next());
        bool y_4 = (collision_1->max_absolute_y_next() >= collision_2->max_absolute_y_next() && collision_2->min_absolute_y_next() >= collision_1->min_absolute_y_next());
        if(!(y_1 || y_2 || y_3 || y_4)){return Collision_Result_3D();}

        // Check Z
        bool z_1 = (collision_1->max_absolute_z_next() > collision_2->min_absolute_z_next() && collision_2->max_absolute_z_next() > collision_1->max_absolute_z_next());
        bool z_2 = (collision_2->max_absolute_z_next() > collision_1->min_absolute_z_next() && collision_1->max_absolute_z_next() > collision_2->max_absolute_z_next());
        bool z_3 = (collision_2->max_absolute_z_next() >= collision_1->max_absolute_z_next() && collision_1->min_absolute_z_next() >= collision_2->min_absolute_z_next());
        bool z_4 = (collision_1->max_absolute_z_next() >= collision_2->max_absolute_z_next() && collision_2->min_absolute_z_next() >= collision_1->min_absolute_z_next());
        if(!(z_1 || z_2 || z_3 || z_4)){return Collision_Result_3D();}

        // Load some datas
        Transform_Object_3D* transform_1 = collision_1.get()->attached_transform();
        Transform_Object_3D* transform_2 = collision_2.get()->attached_transform();

        // Get the differences
        std::shared_ptr<Collision_3D::Collision_Event_Rect_Rect> to_return_1 = std::make_shared<Collision_3D::Collision_Event_Rect_Rect>(collision_1);
        std::shared_ptr<Collision_3D::Collision_Event_Rect_Rect> to_return_2 = std::make_shared<Collision_3D::Collision_Event_Rect_Rect>(collision_2);
        to_return_1.get()->happens = true;to_return_2.get()->happens = true;

        // X
        double x_diff = collision_2->min_absolute_x_next() - collision_1->max_absolute_x_next();
        double x_diff_temp = collision_1->min_absolute_x_next() - collision_2->max_absolute_x_next();
        if(std::abs(x_diff) > std::abs(x_diff_temp)){x_diff = x_diff_temp;}
        // Y
        double y_diff = collision_2->min_absolute_y_next() - collision_1->max_absolute_y_next();
        double y_diff_temp = collision_1->min_absolute_y_next() - collision_2->max_absolute_y_next();
        if(std::abs(y_diff) > std::abs(y_diff_temp)){y_diff = y_diff_temp;}
        // Z
        double z_diff = collision_2->min_absolute_z_next() - collision_1->max_absolute_z_next();
        double z_diff_temp = collision_1->min_absolute_z_next() - collision_2->max_absolute_z_next();
        if(std::abs(z_diff) > std::abs(z_diff_temp)){z_diff = z_diff_temp;}

        // Returns the good value
        if(std::abs(x_diff) > std::abs(y_diff) && std::abs(z_diff) > std::abs(y_diff)){
            // Returns a Y
            if(y_1 || y_4){to_return_1.get()->side_top = true;}
            else{to_return_1.get()->side_bottom = true;}

            // Set the distance
            to_return_1.get()->distance = std::abs(y_diff);
        }
        else if(std::abs(y_diff) > std::abs(x_diff) && std::abs(z_diff) > std::abs(x_diff)) {
            // Returns a X
            if(x_2 || x_3){to_return_1.get()->side_left = true;}
            else{to_return_1.get()->side_right = true;}

            // Set the distance
            to_return_1.get()->distance = std::abs(x_diff);
        }
        else {
            // Returns a X
            if(x_1 || x_4){to_return_1.get()->side_forward = true;}
            else{to_return_1.get()->side_backward = true;}

            // Set the distance
            to_return_1.get()->distance = std::abs(z_diff);
        }

        // Check the movement
        scls::Point_3D final_acceleration = scls::Point_3D(0, 0, 0);
        if(to_return_1.get()->side_bottom || to_return_1.get()->side_top){
            // Handle the velocity
            double final_y = (-transform_1->velocity().y()) * (1.0 + object_2->restitution());
            final_acceleration.set_y(final_y);
            final_acceleration.set_x(-transform_1->velocity().x() * (1.0 - object_2->restitution()));
        }
        else if(to_return_1.get()->side_left || to_return_1.get()->side_right){
            double final_x = -transform_1->velocity().x() * (1.0 + object_2->restitution());
            final_acceleration.set_x(final_x);
        }
        else{
            double final_z = -transform_1->velocity().z() * (1.0 + object_2->restitution());
            final_acceleration.set_z(final_z);
        }
        to_return_1.get()->acceleration = final_acceleration;

        // Return the result
        return Collision_Result_3D(to_return_1, to_return_2);
    };
    Collision_Result_3D __check_collision(std::shared_ptr<Collision_3D> collision_1, std::shared_ptr<Collision_3D> collision_2, Physic_Object_3D* object_1, Physic_Object_3D* object_2) {
        // Asserts
        if(collision_2 == 0 || collision_2->attached_transform() == collision_1->attached_transform()){return Collision_Result_3D();}

        // Both objects are cubes
        if(collision_1->type() == Collision_Type_3D::CT_3D_Cube && collision_2->type() == Collision_Type_3D::CT_3D_Cube) {
            Collision_Result_3D current_result = __check_collision_cube_cube(collision_1, collision_2, object_1, object_2);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }

        return Collision_Result_3D();
    }
    void Physic_Object_3D::check_collision(std::shared_ptr<Collision_3D> collision, Physic_Object_3D* other_object) {
        // Asserts
        if(collision == 0 || collision->attached_transform() == attached_transform() || other_object == 0 || other_object->attached_transform() == 0){return;}

        // Check each collision
        for(int i = 0;i<static_cast<int>(a_collisions.size());i++) {
            // TEMP
            bool already_in = false;
            for(int j = 0;j<static_cast<int>(a_current_collisions_results.size());j++){if(collision.get() == a_current_collisions_results.at(j).get()->other_collision()){already_in=true;break;}}
            if(already_in){continue;}

            Collision_Result_3D current_result = __check_collision(a_collisions.at(i), collision, this, other_object);
            if(current_result.collision_1.get() != 0 && current_result.collision_1.get()->happens){a_current_collisions_results.push_back(current_result.collision_1);}
            if(current_result.collision_2.get() != 0 && current_result.collision_2.get()->happens){other_object->a_current_collisions_results.push_back(current_result.collision_2);}
        }//*/
    }

    // Deletes the object
    void Physic_Object_3D::delete_object(){a_attached_transform.reset();};

    // Returns a new a collision to the graphic object
    std::shared_ptr<Collision_3D> Physic_Object_3D::new_collision(Collision_Type_3D type){
        std::shared_ptr<Collision_3D>to_return=std::make_shared<Collision_3D>(a_this_object, a_attached_transform);
        to_return.get()->set_type(type);add_collision(to_return);
        return to_return;
    };

    // Returns if the object should be deleted or not
    bool Physic_Object_3D::should_delete() const {return attached_transform()==0;};

    // Soft resets the object
    void Physic_Object_3D::soft_reset(){a_current_collisions_results.clear();if(attached_transform() != 0){attached_transform()->soft_reset();}};

    //******************
    // Physic engine
    //******************

    // Deletes the physic in a case
    void Physic_Engine_3D::delete_physic_object_case(Physic_Object_3D* to_delete) {
        for(int j = 0;j<static_cast<int>(to_delete->used_physic_case().size());j++) {
            for(int k = 0;k<static_cast<int>(to_delete->collisions().size());k++) {
                to_delete->used_physic_case().at(j)->delete_static_object_collision(to_delete->collisions().at(k).get());
            }
        }
        to_delete->used_physic_case().clear();
    }

    // Loads 100 X 100 physic map
    void Physic_Engine_3D::load_physic_map(int middle_loading_x, int middle_loading_y, int middle_loading_z) {
        // Get the needed datas
        int depht = 100;int height = 100;int width = 100;
        a_physic_map_start_x = middle_loading_x - width / 2;
        a_physic_map_start_y = middle_loading_y - height / 2;
        a_physic_map_start_z = middle_loading_z - depht / 2;

        // Create the cases
        a_physic_map = std::vector<std::vector<std::vector<std::shared_ptr<Physic_Case_3D>>>>(width, std::vector<std::vector<std::shared_ptr<Physic_Case_3D>>>(depht, std::vector<std::shared_ptr<Physic_Case_3D>>(height)));
        for(int i = 0;i<width;i++){
            for(int j = 0;j<depht;j++){
                for(int k = 0;k<height;k++){
                    int current_x = i;int current_y = j;int current_z = k;
                    a_physic_map[current_x][current_y][current_z]=std::make_shared<Physic_Case_3D>();a_physic_map[current_x][current_y][current_z].get()->position = scls::Point_3D(a_physic_map_start_x + current_x, current_y, current_z);
                }
            }
        }
    }

    // Creates and return a new physic object
    std::shared_ptr<Physic_Object_3D> Physic_Engine_3D::new_physic_object(std::weak_ptr<Transform_Object_3D> object) {
        if(physic_map().size() <= 0){load_physic_map(0, 0, 0);}
        std::shared_ptr<Physic_Object_3D> physic = std::make_shared<Physic_Object_3D>(object);
        physic.get()->set_this_object(physic);add_physic_object(physic);
        return physic;
    }

    // Returns a physic case by its coordinates
    Physic_Object_3D::Physic_Case* Physic_Engine_3D::physic_case(int x, int y, int z){
        if((-a_physic_map_start_x) + x < 0 || (-a_physic_map_start_x) + x >= static_cast<int>(a_physic_map.size())){return 0;}
        if((-a_physic_map_start_y) + y < 0 || (-a_physic_map_start_y) + y >= static_cast<int>(a_physic_map[(-a_physic_map_start_x) + x].size())){return 0;}
        if((-a_physic_map_start_z) + z < 0 || (-a_physic_map_start_z) + z >= static_cast<int>(a_physic_map[(-a_physic_map_start_x) + x][(-a_physic_map_start_y) + y].size())){return 0;}
        return a_physic_map[(-a_physic_map_start_x) + x][(-a_physic_map_start_y) + y][(-a_physic_map_start_z) + z].get();
    };

    // Soft reset the engine
    void Physic_Engine_3D::soft_reset(double used_delta_time){
        scls::Fraction delta_time_fraction = scls::Fraction::from_double(used_delta_time);

        // Soft-reset the physic
        for(int i = 0;i<static_cast<int>(physic_objects().size());i++) {
            if(physic_objects().at(i).get()->should_delete()){
                delete_physic_object_case(physic_objects().at(i).get());
                physic_objects().erase(physic_objects().begin() + i);i--;
            }
            else{physic_objects().at(i).get()->soft_reset();physic_objects().at(i).get()->set_delta_time(delta_time_fraction);}
        }
    }

    // Updates the physic
    scls::Point_3D gravity_3d = scls::Point_3D(0, -9.8, 0);
    int Physic_Engine_3D::update_physic(double used_delta_time) {int needed_upate = update_physic_early(used_delta_time);needed_upate += update_physic_late(used_delta_time);return needed_upate;};
    int Physic_Engine_3D::update_physic_early(double used_delta_time) {
        scls::Fraction delta_time_fraction = scls::Fraction::from_double(used_delta_time);

        // Realised updates
        int needed_update = 0;

        // Soft-reset the physic
        for(int i = 0;i<static_cast<int>(physic_objects().size());i++) {
            if(physic_objects().at(i).get()->should_delete()){
                delete_physic_object_case(physic_objects().at(i).get());
                physic_objects().erase(physic_objects().begin() + i);i--;
            }
            else{physic_objects().at(i).get()->soft_reset();physic_objects().at(i).get()->set_delta_time(delta_time_fraction);}
        }

        // Apply gravity
        for(int i = 0;i<static_cast<int>(physic_objects().size());i++) {if(physic_objects().at(i).get()->use_gravity()){physic_objects().at(i).get()->accelerate(gravity_3d * used_delta_time);needed_update++;}}

        // Update each objects in the case
        std::vector<std::shared_ptr<Physic_Object_3D>> dynamic_objects_physic;
        for(int i = 0;i<static_cast<int>(physic_objects().size());i++) {
            // Asserts
            if(physic_objects().at(i).get()->collisions().size() <= 0){continue;}

            if(!physic_objects().at(i)->is_static()) {dynamic_objects_physic.push_back(physic_objects().at(i));}
            else{
                // Get the basic datas
                int x_start = physic_objects().at(i)->collision_x_start()-1;
                int y_start = physic_objects().at(i)->collision_y_start()-1;
                int z_start = physic_objects().at(i)->collision_z_start()-1;
                bool good_position = (physic_objects().at(i)->used_physic_case().size() > 0 && (physic_objects().at(i)->used_physic_case().at(0)->x() != x_start));

                if(!physic_objects().at(i)->loaded_in_map() || physic_objects().at(i)->moved_during_this_frame() || good_position) {
                    // Delete the last cases
                    delete_physic_object_case(physic_objects().at(i).get());

                    // Get the needed datas
                    physic_objects().at(i)->set_loaded_in_map(true);
                    int needed_height = physic_objects().at(i)->collision_height() + 2;
                    int needed_width = physic_objects().at(i)->collision_width() + 2;
                    int needed_depht = physic_objects().at(i)->collision_depht() + 2;

                    // Add the cases
                    for(int j = 0;j<needed_width;j++) {
                        for(int h = 0;h<needed_height;h++) {
                            for(int k = 0;k<needed_depht;k++) {
                                Physic_Case_3D* current_case = physic_case(x_start + j, y_start + h, z_start + k);
                                if(current_case != 0){
                                    for(int l = 0;l<static_cast<int>(physic_objects().at(i)->collisions().size());l++){
                                        current_case->static_objects_collisions.push_back(physic_objects().at(i)->collisions()[l]);
                                        current_case->static_objects_collisions_physic.push_back(physic_objects().at(i));
                                    }
                                    physic_objects().at(i)->used_physic_case().push_back(current_case);
                                }
                            }
                        }
                    }
                }
            }
        }

        // Dynamic objects
        for(int i = 0;i<static_cast<int>(dynamic_objects_physic.size());i++) {
            // Get the needed datas
            int needed_depht = std::ceil(dynamic_objects_physic.at(i)->max_absolute_z_next()) - std::floor(dynamic_objects_physic.at(i)->min_absolute_z_next());
            int needed_height = std::ceil(dynamic_objects_physic.at(i)->max_absolute_y_next()) - std::floor(dynamic_objects_physic.at(i)->min_absolute_y_next());
            int needed_width = std::ceil(dynamic_objects_physic.at(i)->max_absolute_x_next()) - std::floor(dynamic_objects_physic.at(i)->min_absolute_x_next());
            if(needed_width <= 0){needed_width = 1;};
            int x_start = std::floor(dynamic_objects_physic.at(i)->min_absolute_x_next());
            int y_start = std::floor(dynamic_objects_physic.at(i)->min_absolute_y_next());
            int z_start = std::floor(dynamic_objects_physic.at(i)->min_absolute_z_next());

            // Check the cases
            for(int j = 0;j<needed_width;j++) {
                for(int h = 0;h<needed_height;h++) {
                    for(int k = 0;k<needed_depht;k++) {
                        Physic_Case_3D* current_case = physic_case(x_start + j, y_start + h, z_start + k);
                        if(current_case != 0 && current_case->static_objects_collisions.size() > 0){
                            for(int h = 0;h<static_cast<int>(current_case->static_objects_collisions.size());h++) {
                                dynamic_objects_physic.at(i)->check_collision(current_case->static_objects_collisions.at(h).lock(), current_case->static_objects_collisions_physic[h].lock().get());
                            }
                        }
                    }
                }
            }

            // Check the dynamics collisions
            if(!dynamic_objects_physic.at(i).get()->ignore_dynamic_collisions()){
                for(int j = i + 1;j<static_cast<int>(dynamic_objects_physic.size());j++){
                    for(int k = 0;k<static_cast<int>(dynamic_objects_physic.at(j).get()->collisions().size());k++){
                        if(dynamic_objects_physic.at(i).get() != dynamic_objects_physic.at(j).get()) {
                            dynamic_objects_physic.at(i)->check_collision(dynamic_objects_physic.at(j).get()->collisions().at(k), dynamic_objects_physic.at(j).get());
                        }
                    }
                }
            }
        }//*/

        return needed_update;
    }
    int Physic_Engine_3D::update_physic_late(double used_delta_time) {
        // Realised updates
        int needed_update = 0;

        // Apply collisions
        for(int i = 0;i<static_cast<int>(physic_objects().size());i++) {
            for(int j = 0;j<static_cast<int>(physic_objects().at(i).get()->current_collisions_results().size());j++) {
                physic_objects().at(i)->accelerate(physic_objects().at(i).get()->current_collisions_results().at(j).get()->acceleration / physic_objects().at(i).get()->current_collisions_results().size());
            }
        }

        // Apply next movement
        for(int i = 0;i<static_cast<int>(physic_objects().size());i++) {physic_objects().at(i).get()->__move();needed_update++;}

        return needed_update;
    }
}
