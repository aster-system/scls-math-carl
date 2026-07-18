//******************
//
// scls_math_3d_geometry_core.cpp
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
#include "../scls_math_directory/scls_math_3d_core.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    // Applies a rotation to the point
    Point_3D Point_3D::rotated(Point_3D rotation) const {
        double* rotated_point = __rotate_vector_3d(a_x, a_y, a_z, rotation.a_x, rotation.a_y, rotation.a_z);
        scls::Point_3D to_return;
        to_return.a_x = rotated_point[0]; to_return.a_y = rotated_point[1]; to_return.a_z = rotated_point[2];
        delete rotated_point; rotated_point = 0; return to_return;
    };
    void Point_3D::rotate(scls::Point_3D rotation) {rotation = rotated(rotation);a_x = rotation.a_x; a_y = rotation.a_y; a_z = rotation.a_z;};
    void Point_3D::rotate_x(double rotation) {rotate(Point_3D(rotation, 0, 0));};
    void Point_3D::rotate_y(double rotation) {rotate(Point_3D(0, rotation, 0));};

    // Returns the associated rotation
    double Point_3D::rotation_xz() {
        Point_2D needed_point = Point_2D(std::sqrt(a_x * a_x + a_z * a_z), a_y);
        return vector_2d_angle(needed_point);
    }

    // Absolute position handling
    // Returns the absolute X position
	Point_3D Transform_Object_3D::absolute_position() const{return Point_3D(absolute_x(), absolute_y(), absolute_z());}
    double Transform_Object_3D::absolute_x() const {if(parent() == 0){return x();}return parent()->absolute_x() + a_real_local_parent_x;};
    double Transform_Object_3D::absolute_y() const {if(parent() == 0){return y();}return parent()->absolute_y() + a_real_local_parent_y;};
    double Transform_Object_3D::absolute_z() const {if(parent() == 0){return z();}return parent()->absolute_z() + a_real_local_parent_z;};
    //double Transform_Object_3D::absolute_x() const {if(parent() == 0){return x();}return parent()->absolute_x() + x() * parent()->absolute_scale_x();};
    //double Transform_Object_3D::absolute_y() const {if(parent() == 0){return y();}return parent()->absolute_y() + y() * parent()->absolute_scale_y();};
    //double Transform_Object_3D::absolute_z() const {if(parent() == 0){return z();}return parent()->absolute_z() + z() * parent()->absolute_scale_z();};

    // Absolute velocity handling
    Point_3D Transform_Object_3D::absolute_velocity() const{return Point_3D(absolute_velocity_x(), absolute_velocity_y(), absolute_velocity_z());}
    double Transform_Object_3D::absolute_velocity_x() const{if(parent() == 0){return velocity_x();}return parent()->absolute_velocity_x() + a_real_local_parent_velocity_x;};
    double Transform_Object_3D::absolute_velocity_y() const{if(parent() == 0){return velocity_y();}return parent()->absolute_velocity_y() + a_real_local_parent_velocity_y;};
    double Transform_Object_3D::absolute_velocity_z() const{if(parent() == 0){return velocity_z();}return parent()->absolute_velocity_z() + a_real_local_parent_velocity_z;};

    // Extremums
    // TO OPTIMISE
    double Transform_Object_3D::max_absolute_x() const {Point_3D absolute_max = absolute_scale()/2.0;absolute_max.rotate(absolute_rotation());return absolute_x() + absolute_max.x();};
    double Transform_Object_3D::max_absolute_y() const {Point_3D absolute_max = absolute_scale()/2.0;absolute_max.rotate(absolute_rotation());return absolute_y() + absolute_max.y();};
    double Transform_Object_3D::max_absolute_z() const {Point_3D absolute_max = absolute_scale()/2.0;absolute_max.rotate(absolute_rotation());return absolute_z() + absolute_max.z();};
    double Transform_Object_3D::min_absolute_x() const {Point_3D absolute_max = absolute_scale()/2.0;absolute_max.rotate(absolute_rotation());return absolute_x() - absolute_max.x();};
    double Transform_Object_3D::min_absolute_y() const {Point_3D absolute_max = absolute_scale()/2.0;absolute_max.rotate(absolute_rotation());return absolute_y() - absolute_max.y();};
    double Transform_Object_3D::min_absolute_z() const {Point_3D absolute_max = absolute_scale()/2.0;absolute_max.rotate(absolute_rotation());return absolute_z() - absolute_max.z();};

    // Precise next movement
    double Transform_Object_3D::max_absolute_x_next() const {return max_absolute_x() + next_movement_x();};
    double Transform_Object_3D::max_absolute_y_next() const {return max_absolute_y() + next_movement_y();};
    double Transform_Object_3D::max_absolute_z_next() const {return max_absolute_z() + next_movement_z();};
    double Transform_Object_3D::min_absolute_x_next() const {return min_absolute_x() + next_movement_x();};
    double Transform_Object_3D::min_absolute_y_next() const {return min_absolute_y() + next_movement_y();};
    double Transform_Object_3D::min_absolute_z_next() const {return min_absolute_z() + next_movement_z();};
    Point_3D Transform_Object_3D::position_next() const {return Point_3D(x() + next_movement_x(), y() + next_movement_y(), z() + next_movement_z());};

    // Next movement generated by the velocity
    double Transform_Object_3D::next_movement_x()const{return velocity().x() * a_delta_time.to_double();};
    double Transform_Object_3D::next_movement_y()const{return velocity().y() * a_delta_time.to_double();};
    double Transform_Object_3D::next_movement_z()const{return velocity().z() * a_delta_time.to_double();};

    // Rotate a point locally
    Point_3D Transform_Object_3D::rotate_local(Point_3D to_rotate){
        double* new_vector = rotate_vector_3d_cylindrical(to_rotate.x(), to_rotate.y(), to_rotate.z(), rotation_x(), rotation_y(), rotation_z());
		to_rotate.set_x(new_vector[0]); to_rotate.set_y(new_vector[1]); to_rotate.set_z(new_vector[2]);delete[] new_vector;
		if(parent() != 0){to_rotate = parent()->rotate_local(to_rotate);}
		return to_rotate;
    }

    // Set a new parent for the object
    void Transform_Object_3D::set_parent(std::shared_ptr<Transform_Object_3D> new_parent) {
        if(a_parent.get() != 0){a_parent.get()->remove_child(this);}
        a_parent = new_parent;
        if(a_parent.get() != 0){a_parent.get()->add_child(a_this_object.lock());}
        update_vectors();
    };
    void Transform_Object_3D::set_parent(std::shared_ptr<Transform_Object_3D>* new_parent) {
        if(new_parent==0) {
            if(a_parent.get() != 0) a_parent.get()->remove_child(this);
            a_parent.reset();
            update_vectors();
        }
        else{set_parent(*new_parent);}
    };
    void Transform_Object_3D::set_parent_without_modification(){set_parent_without_modification(std::shared_ptr<Transform_Object_3D>());}
    void Transform_Object_3D::set_parent_without_modification(std::shared_ptr<Transform_Object_3D> new_parent) {
        // Get the datas
        Point_3D last_position = absolute_position();
        Point_3D last_rotation = absolute_rotation();
        Point_3D last_scale = absolute_scale();
        Point_3D last_velocity = absolute_velocity();
        set_parent(new_parent);
        Point_3D current_position = absolute_position();
        Point_3D current_rotation = absolute_rotation();
        Point_3D current_scale = absolute_scale();

        // Set the datas
        set_rotation(Point_3D(rotation_x() + (last_rotation.x() - current_rotation.x()), rotation_y() + (last_rotation.y() - current_rotation.y()), rotation_z() + (last_rotation.z() - current_rotation.z())));
        set_scale(Point_3D(scale_x() * (last_scale.x() / current_scale.x()), scale_y() * (last_scale.y() / current_scale.y()), scale_z() * (last_scale.z() / current_scale.z())));
        current_scale = Point_3D(absolute_scale_x() / scale_x(), absolute_scale_y() /scale_y(), absolute_scale_z() / scale_z());
        set_position(Point_3D(x() + (last_position.x() - current_position.x()) / current_scale.x(), y() + (last_position.y() - current_position.y()) / current_scale.y(), z() + (last_position.z() - current_position.z()) / current_scale.z()));
        set_velocity(last_velocity);
    }

    // Next position
    double Transform_Object_3D::x_next() const {return x() + next_movement_x();};
    double Transform_Object_3D::y_next() const {return y() + next_movement_y();};
    double Transform_Object_3D::z_next() const {return z() + next_movement_z();};

    // Update the real local position of the children
    void Transform_Object_3D::update_children_real_local_position() {
        for(int i = 0;i<static_cast<int>(children().size());i++){
            Transform_Object_3D* current_child = children()[i].lock().get();
            if(current_child != 0) {current_child->update_real_local_position();}
        }
    };
    // Update the real local position of the object
    void Transform_Object_3D::update_real_local_position() {
        // Calculate the real local parent position
        if(parent() != 0) {
            // Rotate the vector
            double* rotated = rotate_vector_3d_cylindrical(x() * parent()->absolute_scale_x(), y() * parent()->absolute_scale_y(), z() * parent()->absolute_scale_z(), parent()->absolute_rotation_x(), parent()->absolute_rotation_y(), parent()->absolute_rotation_z());

            // Calculate the final positions
            a_real_local_parent_x = rotated[0];
            a_real_local_parent_y = rotated[1];
            a_real_local_parent_z = rotated[2];
            delete[] rotated;

            // TEMP
            Point_3D p = parent()->x_vector() * x() * parent()->absolute_scale_x() + parent()->y_vector() * y() * parent()->absolute_scale_y() + parent()->z_vector() * z() * parent()->absolute_scale_z();
            a_real_local_parent_x = p.x();
            a_real_local_parent_y = p.y();
            a_real_local_parent_z = p.z();//*/

            // Velocity
            rotated = rotate_vector_3d_cylindrical(velocity_x() * parent()->absolute_scale_x(), velocity_y() * parent()->absolute_scale_y(), velocity_z() * parent()->absolute_scale_z(), parent()->absolute_rotation_x(), parent()->absolute_rotation_y(), parent()->absolute_rotation_z());
            a_real_local_parent_velocity_x = rotated[0];
            a_real_local_parent_velocity_y = rotated[1];
            a_real_local_parent_velocity_z = rotated[2];
            delete[] rotated;
        }
        else {
            a_real_local_parent_x = x();
            a_real_local_parent_y = y();
            a_real_local_parent_z = z();

            // Velocity
            a_real_local_parent_x = velocity_x();
            a_real_local_parent_y = velocity_y();
            a_real_local_parent_z = velocity_z();
        }

        update_children_real_local_position();
    }

    // Update each vectors
	void Transform_Object_3D::update_vectors() {
		// Update the directions vector
		// Calculate the forward vector
		double* new_forward_vector = __rotate_vector_3d(0, 0, 1, absolute_rotation_x(), absolute_rotation_y(), absolute_rotation_z());
		a_forward_vector_x = new_forward_vector[0]; a_forward_vector_y = new_forward_vector[1]; a_forward_vector_z = new_forward_vector[2];
		// Calculate the right vector
		double* new_right_vector = __rotate_vector_3d(a_forward_vector_x, 0, a_forward_vector_z, 0, -90, 0);
		a_right_vector_x = new_right_vector[0]; a_right_vector_y = new_right_vector[1]; a_right_vector_z = new_right_vector[2];
		// Free the memory
		delete new_forward_vector; new_forward_vector = 0;
		delete new_right_vector; new_right_vector = 0;

		// Create the top vector
		double* new_top_vector = rotate_vector_3d_cylindrical(0, 1, 0, rotation_x(), rotation_y(), rotation_z());
		Point_3D parent_top = Point_3D(new_top_vector[0], new_top_vector[1], new_top_vector[2]);
		if(parent() != 0){parent_top = parent()->rotate_local(parent_top);}
		a_top_vector_x = parent_top.x(); a_top_vector_y = parent_top.y(); a_top_vector_z = parent_top.z();
		delete new_top_vector; new_top_vector = 0;






		// Create the X vector
		Point_3D parent_x = rotate_local(Point_3D(1, 0, 0));
		a_x_vector_x = parent_x.x(); a_x_vector_y = parent_x.y(); a_x_vector_z = parent_x.z();

		// Create the Y vector
		Point_3D parent_y = rotate_local(Point_3D(0, 1, 0));
		a_y_vector_x = parent_y.x(); a_y_vector_y = parent_y.y(); a_y_vector_z = parent_y.z();

		// Create the Z vector
		Point_3D parent_z = rotate_local(Point_3D(0, 0, 1));
		a_z_vector_x = parent_z.x(); a_z_vector_y = parent_z.y(); a_z_vector_z = parent_z.z();

		// Update the real position
		update_real_local_position();
		update_rotation();

		// Update children
		for(int i = 0;i<static_cast<int>(children().size());i++) {
			Transform_Object_3D* current_child = children()[i].lock().get();
			if(current_child != 0){current_child->update_vectors();}
		}
	}

    // Returns the absolute X scale
    Point_3D Transform_Object_3D::absolute_scale() const {return Point_3D(absolute_scale_x(), absolute_scale_y(), absolute_scale_z());};
    double Transform_Object_3D::absolute_scale_x() const {if(parent() == 0 || !parent()->a_inherit_scale){return scale_x();}return parent()->absolute_scale_x() * scale_x();};
    double Transform_Object_3D::absolute_scale_y() const {if(parent() == 0 || !parent()->a_inherit_scale){return scale_y();}return parent()->absolute_scale_y() * scale_y();};
    double Transform_Object_3D::absolute_scale_z() const {if(parent() == 0 || !parent()->a_inherit_scale){return scale_z();}return parent()->absolute_scale_z() * scale_z();};
}
