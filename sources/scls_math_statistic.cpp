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

	// Loads the statistics XML balises
    void load_balises_statistics(__Balise_Container* defined_balises) {
        //std::shared_ptr<scls::Balise_Style_Datas> current_balise;
        //current_balise = std::make_shared<scls::Balise_Style_Datas>();
        //current_balise.get()->has_content = true;
        //defined_balises->set_defined_balise("", current_balise);
    }

	// Adds data
	void Statistics::add_data(std::string name){add_data(name, 1);}
    void Statistics::add_data(std::string name, double number){
        Datas* needed_data = data(name);
        if(needed_data != 0){needed_data->set_size(needed_data->size() + number);}
        else{
            int i = 0;
            for(;i<static_cast<int>(a_samplings.size());i++){if(std::stod(a_samplings.at(i).get()->data()) > std::stod(name)){break;}}
            a_samplings.insert(a_samplings.begin() + i, std::make_shared<Datas>(name, a_this_object));
            a_samplings.at(i).get()->set_size(number - 1);
        }
        update();
    }

    // Adds sampling
    void Statistics::add_sampling(std::string name) {
    	Datas* needed_data = data(name);
		if(needed_data == 0) {
			int i = 0;
			for(;i<static_cast<int>(a_samplings.size());i++){if(std::stod(a_samplings.at(i).get()->data()) > std::stod(name)){break;}}
			a_samplings.insert(a_samplings.begin() + i, std::make_shared<Datas>(name, a_this_object));
			a_samplings.at(i).get()->set_size(0);
		}
		update();
    }

    // Gets a data from the sampling
    Statistics::Datas* Statistics::data(std::string name){for(int i = 0;i<static_cast<int>(a_samplings.size());i++){if(a_samplings.at(i).get()->data() == name){return a_samplings.at(i).get();}}return 0;}

    // Loads the datas from XML
    std::shared_ptr<__Balise_Container> statistics_balise;
    void Statistics::load_from_xml(std::string current_text){if(statistics_balise.get() == 0){statistics_balise = std::make_shared<__Balise_Container>();load_balises_statistics(statistics_balise.get());}load_from_xml(xml(statistics_balise, current_text));}
    void Statistics::load_from_xml(std::shared_ptr<XML_Text_Base> content) {
        // Handle a lot of balises
	    for(int i = 0;i<static_cast<int>(content->sub_texts().size());i++){
            std::string current_balise_name = content->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = content->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == std::string_view("data")){
                std::string content = std::string();double number = 0;
                for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                    if(attributes[i].name == std::string_view("content") || attributes[i].name == std::string_view("name")){content = attributes.at(i).value;}
                    else if(attributes.at(i).value == std::string_view("number")){number = scls::string_to_double(attributes.at(i).value);}
                }

                // Add the stat
                add_data(content, number);
            }
        }
    }

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
