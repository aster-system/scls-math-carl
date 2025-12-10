// Needed includes
#include "../scls_math_directory/scls_math_geometry_core.h"

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
                //std::shared_ptr<Collision_Circle> to_return_1 = std::make_shared<Collision_Circle>(collision_1);
                //std::shared_ptr<Collision_Circle> to_return_2 = std::make_shared<Collision_Circle>(collision_2);

                // Calculate the collision in the objects
                //__check_collision_circle_circle_maths(position_1, line_start, Point_2D(width_circle, width_circle), Point_2D(0.1, 0.1), velocity_circle, Point_2D(0, 0), 0, object_circle, object_line, to_return_1);
            }
            else if(d_2 < width_circle / 2.0){
                // Get the datas about the collision
                // Collision 1
                //std::shared_ptr<Collision_Circle> to_return_1 = std::make_shared<Collision_Circle>(collision_1);
                //std::shared_ptr<Collision_Circle> to_return_2 = std::make_shared<Collision_Circle>(collision_2);

                // Calculate the collision in the objects
                //__check_collision_circle_circle_maths(position_1, line_end, Point_2D(width_circle, width_circle), Point_2D(0.1, 0.1), velocity_circle, Point_2D(0, 0), 0, object_circle, object_line, to_return_1);
            }
        }

        // Get the datas about the collision
        // Collision 1
        //std::shared_ptr<Collision_Circle> to_return_1 = std::make_shared<Collision_Circle>(collision_1);
        //std::shared_ptr<Collision_Circle> to_return_2 = std::make_shared<Collision_Circle>(collision_2);

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
}
