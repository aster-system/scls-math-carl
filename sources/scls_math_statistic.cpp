//******************
//
// scls_math_string.h
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
// This file contains the source code of scls_math_statistic.h.
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

#include "../scls_math_directory/scls_math_statistic.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The "Statistics" class
	//
	//*********

	// Adds data
    void Statistics::add_data(std::string name){
        Datas* needed_data = data(name);
        if(needed_data != 0){needed_data->set_size(needed_data->size() + 1);}
        else{
            int i = 0;
            for(;i<static_cast<int>(a_samplings.size());i++){if(std::stod(a_samplings.at(i).get()->data()) > std::stod(name)){break;}}
            a_samplings.insert(a_samplings.begin() + i, std::make_shared<Datas>(name, a_this_object));
        }
        update();
    }

    // Gets a data from the sampling
    Statistics::Datas* Statistics::data(std::string name){for(int i = 0;i<static_cast<int>(a_samplings.size());i++){if(a_samplings.at(i).get()->data() == name){return a_samplings.at(i).get();}}return 0;}

    // Returns an std::string containing the data structured
    std::string Statistics::structured_datas() {
        std::string to_return = std::string();

        for(int i = 0;i<static_cast<int>(a_samplings.size());i++) {
            to_return += a_samplings.at(i).get()->data();
            to_return += std::string(" ") + std::to_string(a_samplings.at(i).get()->frequency());
            to_return += std::string("\n");
        }

        return to_return;
    }

    // Update the statistics
    void Statistics::update() {
        // Calculate the size
        a_size = 0;for(int i = 0;i<static_cast<int>(a_samplings.size());i++){a_size += a_samplings.at(i).get()->size();}

        // Calculate the average
        a_average = 0;
        for(int i = 0;i<static_cast<int>(a_samplings.size());i++){
            a_average += std::stod(a_samplings.at(i).get()->data()) * a_samplings.at(i).get()->size();
        }
        a_average /= a_size;

        // Get the frequency
        double accumulated_frequency = 0;
        bool median_used = false;
        for(int i = 0;i<static_cast<int>(a_samplings.size());i++){
            double current_frequency = static_cast<double>(a_samplings.at(i).get()->size()) / a_size;
            a_samplings.at(i).get()->set_frequency(current_frequency);
            accumulated_frequency += current_frequency;

            if(accumulated_frequency >= 0.5 && !median_used){a_median = a_samplings.at(i).get()->data();median_used = true;}
        }
    }
}
