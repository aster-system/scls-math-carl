//******************
//
// scls_math_2d_physic.cpp
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
// This file contains the source code of scls_math_2d_physic.h.
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
#include "../scls_math_directory/scls_math_2d_physic.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    // Checks the collision between two circles
    void check_collision_circle_circle_maths(Point_2D position, Point_2D other_position, Point_2D scale, Point_2D other_scale, Point_2D velocity, Point_2D velocity_other, bool can_be_in_each_other, double object_restitution, double other_restitution, bool object_is_static, bool other_is_static){
        // Calculate the position
        bool circle_1_in_other = false;bool circle_2_in_other = false;
        double distance = position.distance(other_position);
        Point_2D position_from_here = (other_position - position).normalized();
        Point_2D position_from_other = (position - other_position).normalized();
        if(distance < other_scale.x() / 2.0 && can_be_in_each_other){position_from_here *= -1;circle_1_in_other=true;}
        if(distance < scale.x() / 2.0 && can_be_in_each_other){position_from_other *= -1;}
        Point_2D position_contact = position + (position_from_here) * oval_radius(scale.x() / 2.0, scale.y() / 2.0, vector_2d_angle(position_from_here));

        // Calculate the angle
        double angle_tangent = vector_2d_angle(position_from_other);
        double angle_tangent_other = vector_2d_angle(position_from_here);

        // TEMP
        //to_return.get()->angle = angle_tangent_other;

        // Calculate the velocity of the object
        double velocity_total = (velocity_other.norm() + velocity.norm());
        if(!object_is_static) {
            Point_2D new_velocity;
            if(other_is_static){
                // Use the physics caracteristics
                velocity_total *= other_restitution * object_restitution;

                // Calculate the new velocity
                new_velocity = position_from_other * velocity_total;
                if(circle_1_in_other){new_velocity = position_from_other * velocity_total;}
                new_velocity -= velocity;
            }
            else {
                new_velocity = (position_from_other * 2.0);
                if(circle_1_in_other){new_velocity = (position_from_other * -2.0);}
            }

            // TEMP
            //to_return.get()->acceleration = new_velocity;
        }
    }

    // Checks the collision between a circle and a line
    void check_collision_circle_line_maths(double x_circle, double y_circle, double width_circle, Point_2D position_next_circle, Point_2D velocity_circle, double x_1, double y_1, double x_2, double y_2){
        // Get the angle
        Point_2D line_end = Point_2D(x_2, y_2);
        Point_2D line_start = Point_2D(x_1, y_1);
        Point_2D line_vector = line_end - line_start;
        Point_2D position_1 = position_next_circle;

        // Get the contact point
        Point_2D contact_point = orthogonal_projection(line_start, line_end, position_1);
        // Get the distance
        double distance = contact_point.distance(position_1);
        double min_distance = std::abs(width_circle / 2.0);
        if(distance > min_distance){return;}
        if(!contact_point.in_rect(line_start, line_vector)){
            // Check if the collision is nearer from an extremity or not
            double d_1 = line_start.distance(position_1);
            double d_2 = line_end.distance(position_1);
            if(d_1 < width_circle / 2.0){
                // Get the datas about the collision
                // Collision 1
                //std::shared_ptr<Collision::Collision_Event_Circle> to_return_1 = std::make_shared<Collision::Collision_Event_Circle>(collision_1);
                //std::shared_ptr<Collision::Collision_Event_Circle> to_return_2 = std::make_shared<Collision::Collision_Event_Circle>(collision_2);

                // Calculate the collision in the objects
                //__check_collision_circle_circle_maths(position_1, line_start, Point_2D(width_circle, width_circle), Point_2D(0.1, 0.1), velocity_circle, Point_2D(0, 0), 0, object_circle, object_line, to_return_1);
            }
            else if(d_2 < width_circle / 2.0){
                // Get the datas about the collision
                // Collision 1
                //std::shared_ptr<Collision::Collision_Event_Circle> to_return_1 = std::make_shared<Collision::Collision_Event_Circle>(collision_1);
                //std::shared_ptr<Collision::Collision_Event_Circle> to_return_2 = std::make_shared<Collision::Collision_Event_Circle>(collision_2);

                // Calculate the collision in the objects
                //__check_collision_circle_circle_maths(position_1, line_end, Point_2D(width_circle, width_circle), Point_2D(0.1, 0.1), velocity_circle, Point_2D(0, 0), 0, object_circle, object_line, to_return_1);
            }
        }

        // Get the datas about the collision
        // Collision 1
        //std::shared_ptr<Collision::Collision_Event_Circle> to_return_1 = std::make_shared<Collision::Collision_Event_Circle>(collision_1);
        //std::shared_ptr<Collision::Collision_Event_Circle> to_return_2 = std::make_shared<Collision::Collision_Event_Circle>(collision_2);

        // Calculate the needed angle
        Point_2D line_vector_normal = line_vector.rotated(90);
        Point_2D position_from_1 = (contact_point - position_1).normalized();
        Point_2D position_from_2 = (position_1 - contact_point).normalized();
        if(distance < min_distance){position_from_2 *= -1;}
        //double kept_proportion = 1.0;
        Point_2D velocity_from_1 = velocity_circle;
        double angle_tangent_1 = vector_2d_angle(position_from_1);
        // TEMP
        // to_return_1.get()->angle = vector_2d_angle(position_from_1);
        // to_return_2.get()->angle = vector_2d_angle(position_from_2);

        double difference_angle = vector_2d_angle(velocity_from_1) - vector_2d_angle(line_vector_normal);
        Point_2D new_velocity_direction = line_vector_normal.rotated(difference_angle * (180.0/SCLS_PI)).normalized() * -1;

        // Calculate the velocity of the object 1
        double multiplier = velocity_from_1.norm();
        // TEMP
        // Point_2D new_velocity = new_velocity_direction * multiplier * object_line->restitution();
        // to_return_1.get()->acceleration = velocity_circle * -1 + new_velocity;
    }

    //******************
    // Physic object
    //******************

    // Getters and setters
    scls::Fraction Collision::absolute_height() const {return attached_transform()->absolute_scale_y();};
    scls::Point_2D Collision::absolute_scale() const {return attached_transform()->absolute_scale();};
    scls::Fraction Collision::absolute_width() const {return attached_transform()->absolute_scale_x();};
    double Collision::absolute_x() const {return attached_transform()->absolute_x();};
    double Collision::absolute_x_1() const{return a_x_1.to_double();};
    double Collision::absolute_x_2() const{return a_x_2.to_double();};
    double Collision::absolute_y() const {return attached_transform()->absolute_y();};
    double Collision::absolute_y_1() const{return a_y_1.to_double();};
    double Collision::absolute_y_2() const{return a_y_2.to_double();};
    Physic_Object* Collision::attached_physic()const{return a_attached_physic.lock().get();};
    scls::Transform_Object_2D* Collision::attached_transform()const{return a_attached_transform.lock().get();};
    double Collision::direct_x_1() const {return a_x_1.to_double();};
    double Collision::direct_x_2() const {return a_x_2.to_double();};
    double Collision::direct_y_1() const {return a_y_1.to_double();};
    double Collision::direct_y_2() const {return a_y_2.to_double();};
    double Collision::max_absolute_x() const {return attached_transform()->max_absolute_x();};
    double Collision::max_absolute_x_next() const {return attached_transform()->max_absolute_x_next();};
    double Collision::max_absolute_y() const {return attached_transform()->max_absolute_y();};
    double Collision::max_absolute_y_next() const {return attached_transform()->max_absolute_y_next();};
    double Collision::min_absolute_x() const {return attached_transform()->min_absolute_x();};
    double Collision::min_absolute_x_next() const {return attached_transform()->min_absolute_x_next();};
    double Collision::min_absolute_y() const {return attached_transform()->min_absolute_y();};
    double Collision::min_absolute_y_next() const {return attached_transform()->min_absolute_y_next();};
    scls::Point_2D Collision::position_next() const {return attached_transform()->position_next();};
    void Collision::set_type(Collision_Type new_type){a_type = new_type;};
    void Collision::set_x_1(scls::Fraction new_x_1){a_x_1 = new_x_1;};
    void Collision::set_x_2(scls::Fraction new_x_2){a_x_2 = new_x_2;};
    void Collision::set_y_1(scls::Fraction new_y_1){a_y_1 = new_y_1;};
    void Collision::set_y_2(scls::Fraction new_y_2){a_y_2 = new_y_2;};
    Collision_Type Collision::type()const{return a_type;};
    double Collision::x_1() const {return attached_transform()->x() + a_x_1.to_double();};
    double Collision::x_2() const {return attached_transform()->x() + a_x_2.to_double();};
    double Collision::y_1() const {return attached_transform()->y() + a_y_1.to_double();};
    double Collision::y_2() const {return attached_transform()->y() + a_y_2.to_double();};

    // Add a line / rect collision to the graphic object
    void Physic_Object::add_collision(std::shared_ptr<Collision> collision){a_collisions.push_back(collision);};
    void Physic_Object::add_collision(double x_1, double y_1, double x_2, double y_2){
        std::shared_ptr<Collision> collision = new_collision(Collision_Type::GCT_Line);
        collision.get()->set_x_1(x_1);collision.get()->set_y_1(y_1);
        collision.get()->set_x_2(x_2);collision.get()->set_y_2(y_2);
        collision.get()->set_type(Collision_Type::GCT_Line);
    };
    void Physic_Object::add_collision(double x_1, double y_1, double x_2, double y_2, double restitution){
        std::shared_ptr<Collision> collision = new_collision(Collision_Type::GCT_Line);
        collision.get()->set_x_1(x_1);collision.get()->set_y_1(y_1);
        collision.get()->set_x_2(x_2);collision.get()->set_y_2(y_2);
        collision.get()->set_type(Collision_Type::GCT_Line);
    };

    // Checks if a collision occurs with an another object
    struct Collision_Result{
        Collision_Result(){};
        Collision_Result(std::shared_ptr<Collision::Collision_Event> c_1):Collision_Result(c_1, std::shared_ptr<Collision::Collision_Event>()){}
        Collision_Result(std::shared_ptr<Collision::Collision_Event> c_1, std::shared_ptr<Collision::Collision_Event> c_2):collision_1(c_1),collision_2(c_2){
            c_1.get()->happens = true;if(c_2.get() != 0){c_2.get()->happens = true;}
            if(c_1.get() != 0 && c_2.get() != 0){c_1.get()->set_other_collision_event(c_2);c_2.get()->set_other_collision_event(c_1);}
        };

        // Needed collisions
        std::shared_ptr<Collision::Collision_Event> collision_1;std::shared_ptr<Collision::Collision_Event> collision_2;
    };
    void __check_collision_circle_circle_maths(scls::Point_2D position, scls::Point_2D other_position, scls::Point_2D scale, scls::Point_2D other_scale, scls::Point_2D velocity, scls::Point_2D velocity_other, bool can_be_in_each_other, Physic_Object* object, Physic_Object* other_object, std::shared_ptr<Collision::Collision_Event_Circle> to_return){
        // Calculate the position
        bool circle_1_in_other = false;bool circle_2_in_other = false;
        double distance = position.distance(other_position);
        scls::Point_2D position_from_here = (other_position - position).normalized();
        scls::Point_2D position_from_other = (position - other_position).normalized();
        if(distance < other_scale.x() / 2.0 && can_be_in_each_other){position_from_here *= -1;circle_1_in_other=true;}
        if(distance < scale.x() / 2.0 && can_be_in_each_other){position_from_other *= -1;}
        scls::Point_2D velocity_from_here = (velocity_other - velocity);
        scls::Point_2D velocity_from_here_normalized = velocity_from_here.normalized();
        scls::Point_2D velocity_from_other = (velocity - velocity_other);
        scls::Point_2D velocity_from_other_normalized = velocity_from_other.normalized();

        // Calculate the angle
        double angle_tangent = scls::vector_2d_angle(position_from_other);
        double angle_tangent_other = scls::vector_2d_angle(position_from_here);
        to_return.get()->angle = angle_tangent_other;

        // Calculate the velocity of the object
        double velocity_total = (velocity_other + velocity).norm();
        if(object != 0 && !object->is_static()) {
            scls::Point_2D new_velocity;
            if(other_object->is_static()){
                // Use the physics caracteristics
                velocity_total *= other_object->restitution() * object->restitution();

                // Calculate the new velocity
                new_velocity = position_from_other * velocity_total;
                if(circle_1_in_other){new_velocity = position_from_other * velocity_total;}
                new_velocity -= velocity;
            }
            else {
                Point_2D to_keep = velocity - velocity_from_other;
                velocity_total = (velocity_from_other + velocity_from_here).norm();
                double velocity_angle = angle_tangent - scls::vector_2d_angle(velocity_from_other);
                double final_angle = angle_tangent + velocity_angle;
                new_velocity = velocity * -1 + to_keep - scls::vector_2d_with_angle(final_angle) * velocity_total;
            }
            to_return.get()->acceleration = new_velocity;
        }
    }
    void __check_collision_circle_circle_maths(Collision* collision, Collision* collision_other, Physic_Object* object, Physic_Object* other_object, std::shared_ptr<Collision::Collision_Event_Circle> to_return){
        __check_collision_circle_circle_maths(collision->position_next(), collision_other->position_next(), collision->absolute_scale(), collision_other->absolute_scale(), object->velocity(), other_object->velocity(), true, object, other_object, to_return);
    }
    Collision_Result __check_collision_circle_circle(std::shared_ptr<Collision> collision_1, std::shared_ptr<Collision> collision_2, Physic_Object* object_1, Physic_Object* object_2){
        // Change the positions if needed
        scls::Point_2D position_1 = collision_1.get()->position_next();scls::Point_2D position_2 = collision_2.get()->position_next();
        position_1.set_y(position_1.y() * (collision_2.get()->attached_transform()->scale_x() / collision_2.get()->attached_transform()->scale_y()));

        // Get the distance
        double distance = position_1.distance(position_2);
        double max_distance = std::abs(collision_1.get()->attached_transform()->scale_x() / 2.0 + collision_2.get()->attached_transform()->scale_x() / 2.0);
        double min_distance = std::abs(collision_1.get()->attached_transform()->scale_x() / 2.0 - collision_2.get()->attached_transform()->scale_x() / 2.0);
        if(!(distance < max_distance && distance > min_distance)){return Collision_Result();}

        // Set collision_1 as the biggest collision
        bool inverse = true;
        if(collision_1->attached_transform()->scale_x() < collision_2->attached_transform()->scale_x()){std::shared_ptr<Collision> temp = collision_1;collision_1 = collision_2;collision_2=temp;Physic_Object* temp_2 = object_1;object_1 = object_2;object_2=temp_2;}
        else{inverse = false;}

        // Get the datas about the collision
        // Collision 1
        std::shared_ptr<Collision::Collision_Event_Circle> to_return_1 = std::make_shared<Collision::Collision_Event_Circle>(collision_1);
        std::shared_ptr<Collision::Collision_Event_Circle> to_return_2 = std::make_shared<Collision::Collision_Event_Circle>(collision_2);

        // Calculate the velocity of the objects
        __check_collision_circle_circle_maths(collision_1.get(), collision_2.get(), object_1, object_2, to_return_1);
        __check_collision_circle_circle_maths(collision_2.get(), collision_1.get(), object_2, object_1, to_return_2);

        // Return the result
        if(inverse){return Collision_Result(to_return_2, to_return_1);}
        return Collision_Result(to_return_1, to_return_2);
    }
    Collision_Result __check_collision_rect_line(std::shared_ptr<Collision> collision_rect, std::shared_ptr<Collision> collision_line, Physic_Object* dynamic_object_1){
        // Check bottom collision
        scls::Crossing_Datas_Segment datas_bottom_current = scls::check_crossing_segment(collision_rect.get()->min_absolute_x(), collision_rect.get()->min_absolute_y(), collision_rect.get()->max_absolute_x(), collision_rect.get()->min_absolute_y(), collision_line.get()->absolute_x_1(), collision_line.get()->absolute_y_1(), collision_line.get()->absolute_x_2(), collision_line.get()->absolute_y_2());
        scls::Crossing_Datas_Segment datas_bottom_next = scls::check_crossing_segment(collision_rect.get()->min_absolute_x_next(), collision_rect.get()->min_absolute_y_next(), collision_rect.get()->max_absolute_x_next(), collision_rect.get()->min_absolute_y_next(), collision_line.get()->absolute_x_1(), collision_line.get()->absolute_y_1(), collision_line.get()->absolute_x_2(), collision_line.get()->absolute_y_2());
        bool collision_bottom = (!datas_bottom_current.crossed_in_segment && datas_bottom_next.crossed_in_segment);
        if(!collision_bottom){
            scls::Point_2D base = scls::Point_2D(collision_line.get()->absolute_x_1(), collision_line.get()->absolute_y_1());
            scls::Point_2D p_1 = scls::Point_2D(collision_rect.get()->min_absolute_x(), collision_rect.get()->min_absolute_y()) - base;
            scls::Point_2D p_2 = scls::Point_2D(collision_rect.get()->min_absolute_x_next(), collision_rect.get()->min_absolute_y_next()) - base;
            double angle_1 = scls::vector_2d_angle(p_1);double angle_2 = scls::vector_2d_angle(p_2);
            while(angle_1 >= SCLS_PI){angle_1 -= SCLS_PI * 2.0;}while(angle_1 < -SCLS_PI){angle_1 += SCLS_PI * 2.0;}
            while(angle_2 >= SCLS_PI){angle_2 -= SCLS_PI * 2.0;}while(angle_2 < -SCLS_PI){angle_2 += SCLS_PI * 2.0;}
            collision_bottom = (scls::sign(angle_1) != scls::sign(angle_2));
        }

        // Check left / right collision
        scls::Point_2D start = Point_2D(collision_line.get()->absolute_x_1(), collision_line.get()->absolute_y_1());
        scls::Crossing_Datas_Segment datas_bottom = scls::check_crossing_segment(collision_rect.get()->min_absolute_x_next(), collision_rect.get()->min_absolute_y_next(), collision_rect.get()->max_absolute_x_next(), collision_rect.get()->min_absolute_y_next(), collision_line.get()->absolute_x_1(), collision_line.get()->absolute_y_1(), collision_line.get()->absolute_x_2(), collision_line.get()->absolute_y_2());
        scls::Crossing_Datas_Segment datas_left = scls::check_crossing_segment(collision_rect.get()->min_absolute_x_next(), collision_rect.get()->max_absolute_y_next(), collision_rect.get()->min_absolute_x_next(), collision_rect.get()->min_absolute_y_next(), collision_line.get()->absolute_x_1(), collision_line.get()->absolute_y_1(), collision_line.get()->absolute_x_2(), collision_line.get()->absolute_y_2());
        scls::Crossing_Datas_Segment datas_right = scls::check_crossing_segment(collision_rect.get()->max_absolute_x_next(), collision_rect.get()->max_absolute_y_next(), collision_rect.get()->max_absolute_x_next(), collision_rect.get()->min_absolute_y_next(), collision_line.get()->absolute_x_1(), collision_line.get()->absolute_y_1(), collision_line.get()->absolute_x_2(), collision_line.get()->absolute_y_2());
        scls::Crossing_Datas_Segment datas_top = scls::check_crossing_segment(collision_rect.get()->min_absolute_x_next(), collision_rect.get()->max_absolute_y_next(), collision_rect.get()->max_absolute_x_next(), collision_rect.get()->max_absolute_y_next(), collision_line.get()->absolute_x_1(), collision_line.get()->absolute_y_1(), collision_line.get()->absolute_x_2(), collision_line.get()->absolute_y_2());
        scls::Crossing_Datas_Segment* datas_final = &datas_bottom;
        if(!datas_final->crossed_in_segment || (datas_left.crossed_in_segment && start.distance(Point_2D(datas_left.crossing_datas.crossing_x, datas_left.crossing_datas.crossing_y)) < start.distance(Point_2D(datas_final->crossing_datas.crossing_x, datas_final->crossing_datas.crossing_y)))){datas_final = &datas_left;}
        if(!datas_final->crossed_in_segment || (datas_right.crossed_in_segment && start.distance(Point_2D(datas_right.crossing_datas.crossing_x, datas_right.crossing_datas.crossing_y)) < start.distance(Point_2D(datas_final->crossing_datas.crossing_x, datas_final->crossing_datas.crossing_y)))){datas_final = &datas_right;}
        if(!datas_final->crossed_in_segment || (datas_top.crossed_in_segment && start.distance(Point_2D(datas_top.crossing_datas.crossing_x, datas_top.crossing_datas.crossing_y)) < start.distance(Point_2D(datas_final->crossing_datas.crossing_x, datas_final->crossing_datas.crossing_y)))){datas_final = &datas_top;}

        // Assert
        if(!(collision_bottom || datas_final->crossed_in_segment)){return Collision_Result();}

        // Get the differences
        double slope_x = std::abs(collision_line.get()->absolute_x_2() - collision_line.get()->absolute_x_1());
        double slope_y = std::abs(collision_line.get()->absolute_y_2() - collision_line.get()->absolute_y_1());
        double temp = 0;scls::normalize_3d(slope_x, temp, slope_y);

        // Create the objects to return
        std::shared_ptr<Collision::Collision_Event_Rect_Rect> to_return_1 = std::make_shared<Collision::Collision_Event_Rect_Rect>(collision_rect);
        std::shared_ptr<Collision::Collision_Event_Rect_Rect> to_return_2 = std::make_shared<Collision::Collision_Event_Rect_Rect>(collision_rect);
        to_return_1.get()->happens = true;to_return_2.get()->happens = true;
        if(collision_bottom){to_return_1.get()->side_bottom = true;}
        if(datas_final->crossed_in_segment){to_return_1.get()->set_collision_position(Point_2D(datas_final->crossing_datas.crossing_x, datas_final->crossing_datas.crossing_y));to_return_1.get()->side_right = true;}

        // Check the movement
        if(dynamic_object_1 != 0){
            to_return_1.get()->acceleration = dynamic_object_1->velocity() * -1;
        }

        // Return the result
        return Collision_Result(to_return_1, to_return_2);
    }
    Collision_Result __check_collision_rect_rect(std::shared_ptr<Collision> collision_1, std::shared_ptr<Collision> collision_2, Physic_Object* dynamic_object_1, Physic_Object* object_2){
        // Check X
        bool x_1 = (collision_1->max_absolute_x_next() > collision_2->min_absolute_x_next() && collision_2->max_absolute_x_next() > collision_1->max_absolute_x_next());
        bool x_2 = (collision_2->max_absolute_x_next() > collision_1->min_absolute_x_next() && collision_1->max_absolute_x_next() > collision_2->max_absolute_x_next());
        bool x_3 = (collision_2->max_absolute_x_next() >= collision_1->max_absolute_x_next() && collision_1->min_absolute_x_next() >= collision_2->min_absolute_x_next());
        bool x_4 = (collision_1->max_absolute_x_next() >= collision_2->max_absolute_x_next() && collision_2->min_absolute_x_next() >= collision_1->min_absolute_x_next());
        if(!(x_1 || x_2 || x_3 || x_4)){return Collision_Result();}

        // Check Y
        bool y_1 = (collision_1->max_absolute_y_next() > collision_2->min_absolute_y_next() && collision_2->max_absolute_y_next() > collision_1->max_absolute_y_next());
        bool y_2 = (collision_2->max_absolute_y_next() > collision_1->min_absolute_y_next() && collision_1->max_absolute_y_next() > collision_2->max_absolute_y_next());
        bool y_3 = (collision_2->max_absolute_y_next() >= collision_1->max_absolute_y_next() && collision_1->min_absolute_y_next() >= collision_2->min_absolute_y_next());
        bool y_4 = (collision_1->max_absolute_y_next() >= collision_2->max_absolute_y_next() && collision_2->min_absolute_y_next() >= collision_1->min_absolute_y_next());
        if(!(y_1 || y_2 || y_3 || y_4)){return Collision_Result();}

        // Load some datas
        scls::Transform_Object_2D* transform_1 = collision_1.get()->attached_transform();
        scls::Transform_Object_2D* transform_2 = collision_2.get()->attached_transform();

        // Get the differences
        std::shared_ptr<Collision::Collision_Event_Rect_Rect> to_return_1 = std::make_shared<Collision::Collision_Event_Rect_Rect>(collision_1);
        std::shared_ptr<Collision::Collision_Event_Rect_Rect> to_return_2 = std::make_shared<Collision::Collision_Event_Rect_Rect>(collision_2);
        to_return_1.get()->happens = true;to_return_2.get()->happens = true;

        // X
        double x_diff = collision_2->min_absolute_x_next() - collision_1->max_absolute_x_next();
        double x_diff_temp = collision_1->min_absolute_x_next() - collision_2->max_absolute_x_next();
        if(std::abs(x_diff) > std::abs(x_diff_temp)){x_diff = x_diff_temp;}
        // Y
        double y_diff = collision_2->min_absolute_y_next() - collision_1->max_absolute_y_next();
        double y_diff_temp = collision_1->min_absolute_y_next() - collision_2->max_absolute_y_next();
        if(std::abs(y_diff) > std::abs(y_diff_temp)){y_diff = y_diff_temp;}

        // Returns the good value
        if(std::abs(x_diff) > std::abs(y_diff)){
            // Returns a Y
            if(y_1 || y_4){to_return_1.get()->side_top = true;}
            else{to_return_1.get()->side_bottom = true;}

            // Set the distance
            to_return_1.get()->distance = std::abs(y_diff);
        }
        else {
            // Returns a X
            if(x_2 || x_3){to_return_1.get()->side_left = true;}
            else{to_return_1.get()->side_right = true;}

            // Set the distance
            to_return_1.get()->distance = std::abs(x_diff);
        }

        // Check the movement
        scls::Point_2D final_acceleration = scls::Point_2D(0, 0);
        if(to_return_1.get()->side_bottom || to_return_1.get()->side_top){
            // Handle the velocity
            double final_y = (-transform_1->velocity().y()) * (1.0 + object_2->restitution());
            final_acceleration.set_y(final_y);
            final_acceleration.set_x(-transform_1->velocity().x() * (1.0 - object_2->restitution()));
        }
        else{
            double final_x = -transform_1->velocity().x() * (1.0 + object_2->restitution());
            final_acceleration.set_x(final_x);
        }
        to_return_1.get()->acceleration = final_acceleration;

        // Return the result
        return Collision_Result(to_return_1, to_return_2);
    };
    Collision_Result __check_collision_circle_line_maths(double x_circle, double y_circle, double width_circle, scls::Point_2D position_next_circle, scls::Point_2D velocity_circle, double x_1, double y_1, double x_2, double y_2, std::shared_ptr<Collision> collision_1, std::shared_ptr<Collision> collision_2, Physic_Object* object_circle, Physic_Object* object_line){
        // Get the angle
        scls::Point_2D line_end = scls::Point_2D(x_2, y_2);
        scls::Point_2D line_start = scls::Point_2D(x_1, y_1);
        scls::Point_2D line_vector = line_end - line_start;
        scls::Point_2D position_1 = position_next_circle;

        // Get the contact point
        scls::Point_2D contact_point = scls::orthogonal_projection(line_start, line_end, position_1);
        // Get the distance
        double distance = contact_point.distance(position_1);
        double min_distance = std::abs(width_circle / 2.0);
        if(distance > min_distance){return Collision_Result();}
        if(!contact_point.in_rect(line_start, line_vector)){
            // Check if the collision is nearer from an extremity or not
            double d_1 = line_start.distance(position_1);
            double d_2 = line_end.distance(position_1);
            if(d_1 < width_circle / 2.0){
                // Get the datas about the collision
                // Collision 1
                std::shared_ptr<Collision::Collision_Event_Circle> to_return_1 = std::make_shared<Collision::Collision_Event_Circle>(collision_1);
                std::shared_ptr<Collision::Collision_Event_Circle> to_return_2 = std::make_shared<Collision::Collision_Event_Circle>(collision_2);

                // Calculate the collision in the objects
                __check_collision_circle_circle_maths(position_1, line_start, scls::Point_2D(width_circle, width_circle), scls::Point_2D(0.1, 0.1), velocity_circle, scls::Point_2D(0, 0), 0, object_circle, object_line, to_return_1);

                // Return the result
                return Collision_Result(to_return_1, to_return_2);
            }
            else if(d_2 < width_circle / 2.0){
                // Get the datas about the collision
                // Collision 1
                std::shared_ptr<Collision::Collision_Event_Circle> to_return_1 = std::make_shared<Collision::Collision_Event_Circle>(collision_1);
                std::shared_ptr<Collision::Collision_Event_Circle> to_return_2 = std::make_shared<Collision::Collision_Event_Circle>(collision_2);

                // Calculate the collision in the objects
                __check_collision_circle_circle_maths(position_1, line_end, scls::Point_2D(width_circle, width_circle), scls::Point_2D(0.1, 0.1), velocity_circle, scls::Point_2D(0, 0), 0, object_circle, object_line, to_return_1);

                // Return the result
                return Collision_Result(to_return_1, to_return_2);
            }

            return Collision_Result();
        }

        // Get the datas about the collision
        // Collision 1
        std::shared_ptr<Collision::Collision_Event_Circle> to_return_1 = std::make_shared<Collision::Collision_Event_Circle>(collision_1);
        std::shared_ptr<Collision::Collision_Event_Circle> to_return_2 = std::make_shared<Collision::Collision_Event_Circle>(collision_2);

        // Calculate the needed angle
        scls::Point_2D line_vector_normal = line_vector.rotated(90);
        scls::Point_2D position_from_1 = (contact_point - position_1).normalized();
        scls::Point_2D position_from_2 = (position_1 - contact_point).normalized();
        if(distance < min_distance){position_from_2 *= -1;}
        //double kept_proportion = 1.0;
        scls::Point_2D velocity_from_1 = velocity_circle;
        double angle_tangent_1 = scls::vector_2d_angle(position_from_1);
        to_return_1.get()->angle = scls::vector_2d_angle(position_from_1);
        to_return_2.get()->angle = scls::vector_2d_angle(position_from_2);

        double difference_angle = scls::vector_2d_angle(velocity_from_1) - scls::vector_2d_angle(line_vector_normal);
        scls::Point_2D new_velocity_direction = line_vector_normal.rotated(difference_angle * (180.0/SCLS_PI)).normalized() * -1;

        // Calculate the velocity of the object 1
        double multiplier = velocity_from_1.norm();
        double needed_restitution = 1;//if(object_2 != 0){needed_restitution = object_2->restitution();}
        scls::Point_2D new_velocity = new_velocity_direction * multiplier * needed_restitution;
        to_return_1.get()->acceleration = velocity_circle * -1 + new_velocity;

        // Return the result
        return Collision_Result(to_return_1, to_return_2);
    }
    Collision_Result __check_collision_circle_line(std::shared_ptr<Collision> collision_1, std::shared_ptr<Collision> collision_2, Physic_Object* object_1, Physic_Object* object_2){
        // Get the angle
        scls::Point_2D line_end = scls::Point_2D(collision_2->direct_x_2(), collision_2->direct_y_2());
        scls::Point_2D line_start = scls::Point_2D(collision_2->direct_x_1(), collision_2->direct_y_1());
        scls::Point_2D line_vector = line_end - line_start;
        scls::Point_2D position_1 = collision_1->position_next();

        // Get the contact point
        scls::Point_2D contact_point = scls::orthogonal_projection(line_start, line_end, position_1);
        // Get the distance
        double distance = contact_point.distance(position_1);
        double min_distance = std::abs(collision_1->attached_transform()->scale_x() / 2.0);
        if(distance > min_distance){return Collision_Result();}
        if(!contact_point.in_rect(line_start, line_vector)){
            // Check the edge of the collision
            distance = position_1.distance(line_start);scls::Point_2D needed_point = line_start;
            if(distance > min_distance){
                distance = position_1.distance(line_end);needed_point = line_end;
                if(distance > min_distance){return Collision_Result();}
            }

            std::shared_ptr<Collision::Collision_Event_Circle> to_return_1 = std::make_shared<Collision::Collision_Event_Circle>(collision_1);
            std::shared_ptr<Collision::Collision_Event_Circle> to_return_2 = std::make_shared<Collision::Collision_Event_Circle>(collision_2);
            Point_2D velocity_1 = Point_2D(0, 0);if(object_1 != 0){velocity_1 = object_1->velocity();}Point_2D velocity_2 = Point_2D(0, 0);if(object_2 != 0){velocity_2 = object_2->velocity();}
            __check_collision_circle_circle_maths(position_1, needed_point, collision_1->absolute_scale(), scls::Point_2D(0.01, 0.01), velocity_1, velocity_2, false, object_1, object_2, to_return_1);
            __check_collision_circle_circle_maths(needed_point, position_1, scls::Point_2D(0.01, 0.01), collision_1->absolute_scale(), velocity_2, velocity_1, false, object_2, object_1, to_return_2);
            return Collision_Result(to_return_1, to_return_2);
        }

        // Get the datas about the collision
        // Collision 1
        std::shared_ptr<Collision::Collision_Event_Circle> to_return_1 = std::make_shared<Collision::Collision_Event_Circle>(collision_1);
        std::shared_ptr<Collision::Collision_Event_Circle> to_return_2 = std::make_shared<Collision::Collision_Event_Circle>(collision_2);

        // Calculate the needed angle
        scls::Point_2D line_vector_normal = line_vector.rotated(90);
        scls::Point_2D position_from_1 = (contact_point - position_1).normalized();
        scls::Point_2D position_from_2 = (position_1 - contact_point).normalized();
        if(distance < min_distance){position_from_2 *= -1;}
        //double kept_proportion = 1.0;
        scls::Point_2D velocity_from_1 = collision_1->attached_transform()->velocity();
        double angle_tangent_1 = scls::vector_2d_angle(position_from_1);
        to_return_1.get()->angle = scls::vector_2d_angle(position_from_1);
        to_return_2.get()->angle = scls::vector_2d_angle(position_from_2);

        double difference_angle = scls::vector_2d_angle(velocity_from_1) - scls::vector_2d_angle(line_vector_normal);
        scls::Point_2D new_velocity_direction = line_vector_normal.rotated(difference_angle * (180.0/SCLS_PI)).normalized() * -1;

        // Calculate the velocity of the object 1
        double multiplier = velocity_from_1.norm();
        double needed_restitution = 1;if(object_2 != 0){needed_restitution = object_2->restitution();}
        scls::Point_2D new_velocity = new_velocity_direction * multiplier * needed_restitution;
        if(object_1 != 0){to_return_1.get()->acceleration = object_1->velocity() * -1 + new_velocity;}

        // Return the result
        return Collision_Result(to_return_1, to_return_2);
    }
    Collision_Result __check_collision_circle_rect(std::shared_ptr<Collision> collision_circle, std::shared_ptr<Collision> collision_rect, Physic_Object* object_circle, Physic_Object* object_rect){
        // Check the collisions as line
        // Top collision
        double line_x_1 = collision_rect->min_absolute_x();
        double line_y_1 = collision_rect->min_absolute_y();
        double line_x_2 = collision_rect->max_absolute_x();
        double line_y_2 = collision_rect->max_absolute_y();
        Collision_Result result_top = __check_collision_circle_line_maths(collision_circle->absolute_x(), collision_circle->absolute_y(), collision_circle->absolute_width().to_double(), collision_circle->position_next(), object_circle->velocity(), line_x_1, line_y_2, line_x_2, line_y_2, collision_circle, collision_rect, object_circle, object_rect);
        // Bottom collision
        line_y_1 = collision_rect->min_absolute_y();
        line_y_2 = collision_rect->min_absolute_y();
        Collision_Result result_bottom = __check_collision_circle_line_maths(collision_circle->absolute_x(), collision_circle->absolute_y(), collision_circle->absolute_width().to_double(), collision_circle->position_next(), object_circle->velocity(), line_x_1, line_y_1, line_x_2, line_y_1, collision_circle, collision_rect, object_circle, object_rect);
        // Left collision
        line_x_1 = collision_rect->min_absolute_x();line_y_1 = collision_rect->max_absolute_y();
        line_x_2 = collision_rect->min_absolute_x();line_y_2 = collision_rect->min_absolute_y();
        Collision_Result result_left = __check_collision_circle_line_maths(collision_circle->absolute_x(), collision_circle->absolute_y(), collision_circle->absolute_width().to_double(), collision_circle->position_next(), object_circle->velocity(), line_x_1, line_y_1, line_x_2, line_y_2, collision_circle, collision_rect, object_circle, object_rect);
        // Right collision
        line_x_1 = collision_rect->max_absolute_x();
        line_x_2 = collision_rect->max_absolute_x();
        Collision_Result result_right = __check_collision_circle_line_maths(collision_circle->absolute_x(), collision_circle->absolute_y(), collision_circle->absolute_width().to_double(), collision_circle->position_next(), object_circle->velocity(), line_x_1, line_y_1, line_x_2, line_y_2, collision_circle, collision_rect, object_circle, object_rect);
        // Final collision
        Collision_Result final_result = result_top;
        if(final_result.collision_1.get() == 0 || !final_result.collision_1.get()->happens) {final_result = result_bottom;}
        if(final_result.collision_1.get() == 0 || !final_result.collision_1.get()->happens) {final_result = result_left;}
        if(final_result.collision_1.get() == 0 || !final_result.collision_1.get()->happens) {final_result = result_right;}
        if(final_result.collision_1.get() == 0 || !final_result.collision_1.get()->happens) {return Collision_Result();}

        // Add some needed datas
        //final_result.collision_1.get()->__other_object = collision_rect->attached_object_weak_ptr();
        //final_result.collision_2.get()->__other_object = collision_circle->attached_object_weak_ptr();

        // Return the result
        return final_result;
    }
    Collision_Result __check_collision(std::shared_ptr<Collision> collision_1, std::shared_ptr<Collision> collision_2, Physic_Object* object_1, Physic_Object* object_2) {
        // Asserts
        if(collision_2 == 0 || collision_2->attached_transform() == collision_1->attached_transform()){return Collision_Result();}

        // Both objects are rect
        if(collision_1->type() == Collision_Type::GCT_Rect && collision_2->type() == Collision_Type::GCT_Rect) {
            Collision_Result current_result = __check_collision_rect_rect(collision_1, collision_2, object_1, object_2);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Collision_Type::GCT_Rect && collision_2->type() == Collision_Type::GCT_Line) {
            Collision_Result current_result = __check_collision_rect_line(collision_1, collision_2, object_1);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Collision_Type::GCT_Line && collision_2->type() == Collision_Type::GCT_Rect) {
            Collision_Result current_result = __check_collision_rect_line(collision_2, collision_1, object_2);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Collision_Type::GCT_Circle && collision_2->type() == Collision_Type::GCT_Circle) {
            Collision_Result current_result = __check_collision_circle_circle(collision_1, collision_2, object_1, object_2);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Collision_Type::GCT_Circle && collision_2->type() == Collision_Type::GCT_Line) {
            Collision_Result current_result = __check_collision_circle_line(collision_1, collision_2, object_1, object_2);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Collision_Type::GCT_Circle && collision_2->type() == Collision_Type::GCT_Rect) {
            Collision_Result current_result = __check_collision_circle_rect(collision_1, collision_2, object_1, object_2);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Collision_Type::GCT_Rect && collision_2->type() == Collision_Type::GCT_Circle) {
            Collision_Result current_result = __check_collision_circle_rect(collision_2, collision_1, object_2, object_1);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Collision_Type::GCT_Line && collision_2->type() == Collision_Type::GCT_Circle) {
            Collision_Result current_result = __check_collision_circle_line(collision_2, collision_1, object_1, object_2);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }

        return Collision_Result();
    }
    //*/
    void Physic_Object::check_collision(std::shared_ptr<Collision> collision, Physic_Object* other_object) {
        // Asserts
        if(collision == 0 || collision->attached_transform() == attached_transform() || other_object == 0 || other_object->attached_transform() == 0){return;}

        // Check each collision
        for(int i = 0;i<static_cast<int>(a_collisions.size());i++) {
            // TEMP
            bool already_in = false;
            // for(int j = 0;j<static_cast<int>(a_current_collisions_results.size());j++){if(collision->attached_object() == a_current_collisions_results.at(j).get()->other_object()){already_in=true;break;}}
            for(int j = 0;j<static_cast<int>(a_current_collisions_results.size());j++){if(collision.get() == a_current_collisions_results.at(j).get()->other_collision()){already_in=true;break;}}
            if(already_in){continue;}

            Collision_Result current_result = __check_collision(a_collisions.at(i), collision, this, other_object);
            if(current_result.collision_1.get() != 0 && current_result.collision_1.get()->happens){a_current_collisions_results.push_back(current_result.collision_1);}
            if(current_result.collision_2.get() != 0 && current_result.collision_2.get()->happens){other_object->a_current_collisions_results.push_back(current_result.collision_2);}
        }
    }

    // Deletes the object
    void Physic_Object::delete_object(){a_attached_transform.reset();};

    // Returns a new a collision to the graphic object
    std::shared_ptr<Collision> Physic_Object::new_collision(Collision_Type type){
        std::shared_ptr<Collision>to_return=std::make_shared<Collision>(a_this_object, a_attached_transform);
        to_return.get()->set_type(type);add_collision(to_return);
        return to_return;
    };

    // Returns if the object should be deleted or not
    bool Physic_Object::should_delete() const {return attached_transform()==0;};

    // Soft resets the object
    void Physic_Object::soft_reset(){a_current_collisions_results.clear();if(attached_transform() != 0){attached_transform()->soft_reset();}};

    //******************
    // Physic engine
    //******************

    // Deletes the physic in a case
    void Physic_Engine::delete_physic_object_case(Physic_Object* to_delete) {
        for(int j = 0;j<static_cast<int>(to_delete->used_physic_case().size());j++) {
            for(int k = 0;k<static_cast<int>(to_delete->collisions().size());k++) {
                to_delete->used_physic_case().at(j)->delete_static_object_collision(to_delete->collisions().at(k).get());
            }
        }
        to_delete->used_physic_case().clear();
    }

    // Loads 100 X 100 physic map
    void Physic_Engine::load_physic_map(int middle_loading_x, int middle_loading_y) {
        // Get the needed datas
        int height = 100;int width = 100;
        a_physic_map_start_x = middle_loading_x - width / 2;
        a_physic_map_start_y = middle_loading_y - height / 2;

        // Create the cases
        a_physic_map = std::vector<std::vector<std::shared_ptr<Physic_Case>>>(width, std::vector<std::shared_ptr<Physic_Case>>(height));
        for(int i = 0;i<width;i++){for(int j = 0;j<height;j++){a_physic_map[i][j]=std::make_shared<Physic_Case>();a_physic_map[i][j].get()->position = scls::Point_2D(a_physic_map_start_x + i, j);}}
    }

    // Creates and return a new physic object
    std::shared_ptr<Physic_Object> Physic_Engine::new_physic_object(std::weak_ptr<Transform_Object_2D> object) {
        if(physic_map().size() <= 0){load_physic_map(0, 0);}
        std::shared_ptr<Physic_Object> physic = std::make_shared<Physic_Object>(object);
        physic.get()->set_this_object(physic);add_physic_object(physic);
        return physic;
    }

    // Returns a physic case by its coordinates
    Physic_Object::Physic_Case* Physic_Engine::physic_case(int x, int y){
        if((-a_physic_map_start_x) + x < 0 || (-a_physic_map_start_x) + x >= static_cast<int>(a_physic_map.size())){return 0;}
        if((-a_physic_map_start_y) + y < 0 || (-a_physic_map_start_y) + y >= static_cast<int>(a_physic_map[(-a_physic_map_start_x) + x].size())){return 0;}
        return a_physic_map[(-a_physic_map_start_x) + x][(-a_physic_map_start_y) + y].get();
    };

    // Returns a list of physic object in a rect
    std::vector<std::shared_ptr<Physic_Object>> Physic_Engine::physic_objects_in_rect(double x, double y, double width, double height) {
        std::vector<std::shared_ptr<Physic_Object>> to_return = std::vector<std::shared_ptr<Physic_Object>>();
        int needed_height = std::ceil(height);
        int needed_width = std::ceil(width);
        int x_start = std::floor(x);
        int y_start = std::ceil(y);

        for(int i = 0;i<static_cast<int>(needed_width);i++) {
            for(int j = 0;j<static_cast<int>(needed_height);j++) {
                // Get the needed case
                int current_x = x_start + i;
                int current_y = y_start + j;
                Physic_Object::Physic_Case* needed_case = physic_case(current_x, current_y);

                // Check the physic
                for(int k = 0;k<static_cast<int>(needed_case->static_objects_collisions_physic.size());k++){
                    bool good = true;
                    for(int l = 0;l<static_cast<int>(to_return.size());l++){
                        if(to_return.at(l).get() == needed_case->static_objects_collisions_physic.at(k).lock().get()){good = false;break;}
                    }
                    if(good){to_return.push_back(needed_case->static_objects_collisions_physic.at(k).lock());}
                }
            }
        }

        return to_return;
    }

    // Raycasts the map
    Physic_Engine::Raycast_Result Physic_Engine::raycast(double x_start, double y_start, double x_direction, double y_direction, double distance) {
        // Base of the calculation
        std::shared_ptr<scls::Transform_Object_2D> current_transform = std::make_shared<scls::Transform_Object_2D>();
        std::shared_ptr<Collision> needed_collision = std::make_shared<Collision>(std::weak_ptr<Physic_Object>(), current_transform);
        needed_collision.get()->set_x_1(x_start);needed_collision.get()->set_y_1(y_start);
        double adder_x = 1;if(x_direction < 0){adder_x = -1;}
        double adder_y = 1;if(y_direction < 0){adder_y = -1;}
        double x_y_ratio = std::abs(x_direction / y_direction);

        // Horizontal datas
        double horizontal_x = x_start;double horizontal_y = y_start;
        double diff = 0;
        if(x_direction > 0){diff = std::ceil(horizontal_x) - horizontal_x;horizontal_x = std::ceil(horizontal_x);}
        else{diff = horizontal_x - std::floor(horizontal_x);horizontal_x = std::floor(horizontal_x);}
        horizontal_y += adder_y * (diff * x_y_ratio);

        // Check cases X
        double horizontal_distance = std::sqrt(std::pow(horizontal_x - x_start, 2) + std::pow(horizontal_y - y_start, 2));
        Collision_Result result_x;
        while(horizontal_distance < distance) {
            Physic_Object::Physic_Case* current_case =  0;
            if(adder_x > 0){current_case = physic_case(std::floor(x_start), std::floor(y_start));}
            else{current_case = physic_case(std::floor(x_start - 1), std::floor(y_start));}
            if(current_case != 0) {
                needed_collision.get()->set_x_2(horizontal_x);needed_collision.get()->set_y_2(horizontal_y);
                for(int i = 0;i<static_cast<int>(current_case->static_objects_collisions.size());i++){
                    result_x = __check_collision(current_case->static_objects_collisions.at(i).lock(), needed_collision, 0, 0);
                    if(result_x.collision_1.get() != 0 && result_x.collision_1.get()->happens){break;}
                }
            }

            // Breaking
            if(result_x.collision_1.get() != 0 && result_x.collision_1.get()->happens){break;}

            // Pass to the next
            horizontal_x += adder_x;
            horizontal_y += adder_y / x_y_ratio;
            horizontal_distance = std::sqrt(std::pow(horizontal_x - x_start, 2) + std::pow(horizontal_y - y_start, 2));
        }

        // Vertical datas
        double vertical_x = x_start;double vertical_y = y_start;
        diff = 0;
        if(y_direction > 0){diff = std::ceil(vertical_y) - vertical_y;vertical_y = std::ceil(vertical_y);}
        else{diff = vertical_y - std::floor(vertical_y);vertical_y = std::floor(vertical_y);}
        vertical_x += adder_x * (diff / x_y_ratio);

        // Check cases Y
        double current_distance = std::sqrt(std::pow(vertical_x - x_start, 2) + std::pow(vertical_y - y_start, 2));
        Collision_Result result_y;
        while(current_distance < distance) {
            // Get the cases
            std::vector<Physic_Object::Physic_Case*> current_case = std::vector<Physic_Object::Physic_Case*>();
            for(int i = 0;i<std::ceil(std::abs(adder_x));i++){
                Physic_Object::Physic_Case* to_add = 0;
                if(adder_y > 0){to_add = physic_case(std::floor(vertical_x + adder_x), std::floor(vertical_y));}
                else{to_add = physic_case(std::floor(vertical_x + adder_x), std::floor(vertical_y  - 1));}
                if(to_add != 0){current_case.push_back(to_add);}
            }

            // Do the collisions
            needed_collision.get()->set_x_2(vertical_x);needed_collision.get()->set_y_2(vertical_y);
            for(int i = 0;i<static_cast<int>(current_case.size());i++) {
                for(int j = 0;j<static_cast<int>(current_case.at(i)->static_objects_collisions.size());j++){
                    result_y = __check_collision(current_case.at(i)->static_objects_collisions.at(j).lock(), needed_collision, 0, 0);
                    if(result_y.collision_1.get() != 0 && result_y.collision_1.get()->happens){break;}
                }
            }

            // Breaking
            if(result_y.collision_1.get() != 0 && result_y.collision_1.get()->happens){break;}

            // Pass to the next
            vertical_y += adder_y;
            vertical_x += adder_x * x_y_ratio;
            current_distance = std::sqrt(std::pow(vertical_x - x_start, 2) + std::pow(vertical_y - y_start, 2));
        }

        //std::cout << "J " << result_x.collision_1.get() << " " << result_y.collision_1.get() << " " << adder_x << " " << adder_y / x_y_ratio << " " << adder_y << " " << adder_x * x_y_ratio << " " << horizontal_x << " " << horizontal_y << " " << vertical_x << " " << vertical_y << std::endl;

        // Returns the result
        Physic_Engine::Raycast_Result to_return;
        if(result_x.collision_1.get() != 0 && result_y.collision_1.get() != 0) {
            if(horizontal_distance < current_distance){
                to_return.collision = result_x.collision_1;
                to_return.position = Point_2D(horizontal_x, horizontal_y);
            }
            else if(result_y.collision_1.get() != 0){
                to_return.collision = result_y.collision_1;
                to_return.position = Point_2D(vertical_x, vertical_y);
            }
        }
        else if(result_x.collision_1.get() != 0){
            to_return.collision = result_x.collision_1;
            to_return.position = Point_2D(horizontal_x, horizontal_y);
        }
        else if(result_y.collision_1.get() != 0){
            to_return.collision = result_y.collision_1;
            to_return.position = Point_2D(vertical_x, vertical_y);
        }
        return to_return;
    }

    // Soft reset the engine
    void Physic_Engine::soft_reset(double used_delta_time){
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
    scls::Point_2D gravity = scls::Point_2D(0, -9.8);
    int Physic_Engine::update_physic(double used_delta_time) {int needed_upate = update_physic_early(used_delta_time);needed_upate += update_physic_late(used_delta_time);return needed_upate;};
    int Physic_Engine::update_physic_early(double used_delta_time) {
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
        for(int i = 0;i<static_cast<int>(physic_objects().size());i++) {if(physic_objects().at(i).get()->use_gravity()){physic_objects().at(i).get()->accelerate(gravity * used_delta_time);needed_update++;}}

        // Update raw velocity
        for(int i = 0;i<static_cast<int>(physic_objects().size());i++) {physic_objects().at(i).get()->update_raw_velocity();}

        // Update each objects in the case
        std::vector<std::shared_ptr<Physic_Object>> dynamic_objects_physic;
        for(int i = 0;i<static_cast<int>(physic_objects().size());i++) {
            // Asserts
            if(physic_objects().at(i).get()->collisions().size() <= 0){continue;}

            if(!physic_objects().at(i)->is_static()) {dynamic_objects_physic.push_back(physic_objects().at(i));}
            else{
                // Get the basic datas
                int x_start = physic_objects().at(i)->collision_x_start()-1;
                int y_start = physic_objects().at(i)->collision_y_start()-1;
                bool good_position = (physic_objects().at(i)->used_physic_case().size() > 0 && (physic_objects().at(i)->used_physic_case().at(0)->x() != x_start || physic_objects().at(i)->used_physic_case().at(0)->y() != y_start));

                if(!physic_objects().at(i)->loaded_in_map() || physic_objects().at(i)->moved_during_this_frame() || good_position) {
                    // Delete the last cases
                    delete_physic_object_case(physic_objects().at(i).get());

                    // Get the needed datas
                    physic_objects().at(i)->set_loaded_in_map(true);
                    int needed_height = physic_objects().at(i)->collision_height() + 2;
                    int needed_width = physic_objects().at(i)->collision_width() + 2;
                    if(needed_height <= 0){needed_height = 1;};if(needed_width <= 0){needed_width = 1;};

                    // Add the cases
                    for(int j = 0;j<needed_width;j++) {
                        for(int h = 0;h<needed_height;h++) {
                            Physic_Case* current_case = physic_case(x_start + j, y_start + h);
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

        // Dynamic objects
        for(int i = 0;i<static_cast<int>(dynamic_objects_physic.size());i++) {
            // Get the needed datas
            int needed_height = std::ceil(dynamic_objects_physic.at(i)->max_absolute_y_next()) - std::floor(dynamic_objects_physic.at(i)->min_absolute_y_next());
            int needed_width = std::ceil(dynamic_objects_physic.at(i)->max_absolute_x_next()) - std::floor(dynamic_objects_physic.at(i)->min_absolute_x_next());
            if(needed_width <= 0){needed_width = 1;};
            int x_start = std::floor(dynamic_objects_physic.at(i)->min_absolute_x_next());
            int y_start = std::floor(dynamic_objects_physic.at(i)->min_absolute_y_next());

            // Check the cases
            for(int j = 0;j<needed_width;j++) {
                for(int h = 0;h<needed_height;h++) {
                    Physic_Case* current_case = physic_case(x_start + j, y_start + h);
                    if(current_case != 0 && current_case->static_objects_collisions.size() > 0){
                        for(int h = 0;h<static_cast<int>(current_case->static_objects_collisions.size());h++) {
                            dynamic_objects_physic.at(i)->check_collision(current_case->static_objects_collisions.at(h).lock(), current_case->static_objects_collisions_physic[h].lock().get());
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
        }

        return needed_update;
    }
    int Physic_Engine::update_physic_late(double used_delta_time) {
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
    //*/
}
