//******************
//
// scls_math_statistic.h
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
// This file contains some functions to handle statistics with mathematics.
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

#ifndef SCLS_MATH_STATISTIC
#define SCLS_MATH_STATISTIC

#include "scls_math_numbers.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// The "Statistics" class
	//
	//*********

	class Statistics {
    public:
        // Data
        class Datas {
        public:
            // Datas constructor
            Datas(std::string needed_data, std::weak_ptr<Statistics> needed_stats):a_data(needed_data),stats(needed_stats){}

            // Update the statistics
            void update_stats() const {stats.lock().get()->update();};

            // Getters and setters
            inline double frequency() const {return a_frequency;}
            inline std::string data() const {return a_data;}
            inline void set_frequency(double new_frequency){a_frequency = new_frequency;}
            inline double size() const {return a_size;};
            inline void set_size(double new_size){a_size = new_size;};

        private:
            std::string a_data = std::string();
            double a_frequency = 0;
            double a_size = 1;
            std::weak_ptr<Statistics> stats;
        };

        // Statistics constructor
        Statistics(){}
        // Statistics destructor
        virtual ~Statistics(){};

        // Adds data
        void add_data(std::string name);

        // Gets a data from the sampling
        Datas* data(std::string name);

        // Returns an std::string containing the data structured
        std::string structured_datas();

        // Update the statistics
        void update();

        // Getters and setters
        inline double average() const {return a_average;}
        inline std::string median() const {return a_median;}
        inline const std::vector<std::shared_ptr<Datas>>& samplings() const {return a_samplings;};
        inline void set_this_object(std::weak_ptr<Statistics> this_object){a_this_object = this_object;}
        double size() const {return a_size;};
    protected:
        // This object
        std::weak_ptr<Statistics> a_this_object;

    private:
        // Needed datas
        // Average
        double a_average = 0;
        // Median
        std::string a_median = std::string();
        // Size
        double a_size = 0;

        // Samplings
        std::vector<std::shared_ptr<Datas>> a_samplings;
    };
}

#endif // SCLS_MATH_STATISTIC
