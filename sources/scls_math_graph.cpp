//******************
//
// scls_math_graph.cpp
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
// This file contains the source code of "scls_math_graph.h".
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

#include "../scls_math.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    // Use the djikstra algorithm in a 2D plane
    constexpr int djikstra_2d_untreated = 0;
    constexpr int djikstra_2d_wall = 1;
    void djikstra_2d_add_point(std::vector<std::vector<int>>& needed_map, std::list<Point_2D>& next_position, int number, Point_2D position_end, Point_2D current_position, bool& stop) {
        if(current_position == position_end){stop=true;}
        else if(needed_map.at(current_position.x()).at(current_position.y()) == djikstra_2d_untreated){
            next_position.push_back(scls::Point_2D(current_position.x(), current_position.y()));
        }
    }
    void djikstra_2d_check_point(std::vector<std::vector<int>>& needed_map, int& current_n, Point_2D current_position, bool& good) {
        if(needed_map.at(current_position.x()).at(current_position.y()) < 0 && needed_map.at(current_position.x()).at(current_position.y()) > current_n) {
            good = true;
        }
    }
    std::vector<char> djikstra_2d(std::vector<std::vector<int>> needed_map, Point_2D position_start, Point_2D position_end) {
        // Create the basic datas
        std::list<Point_2D> next_position;next_position.push_back(position_start);next_position.push_back(Point_2D(-1, -1));
        int number = 0;

        // Browse
        while(!next_position.empty()) {
            // Pre-handling
            scls::Point_2D current_position = next_position.front();next_position.pop_front();
            if(current_position.x() == -1 && current_position.y() == -1){if(!next_position.empty()){next_position.push_back(current_position);}number++;continue;}
            else if(needed_map.at(current_position.x()).at(current_position.y()) != djikstra_2d_untreated){continue;}
            else{needed_map[current_position.x()][current_position.y()] = number * -1 - 1;}

            bool stop = false;
            if(current_position.x() > 0){djikstra_2d_add_point(needed_map, next_position, number, position_end, scls::Point_2D(current_position.x() - 1, current_position.y()), stop);}
            if(stop){break;}
            if(current_position.x() < needed_map.size() - 1){djikstra_2d_add_point(needed_map, next_position, number, position_end, scls::Point_2D(current_position.x() + 1, current_position.y()), stop);}
            if(stop){break;}
            if(current_position.y() > 0){djikstra_2d_add_point(needed_map, next_position, number, position_end, scls::Point_2D(current_position.x(), current_position.y() - 1), stop);}
            if(stop){break;}
            if(current_position.y() < needed_map.at(0).size() - 1){djikstra_2d_add_point(needed_map, next_position, number, position_end, scls::Point_2D(current_position.x(), current_position.y() + 1), stop);}
            if(stop){break;}
        }

        // Browser the map to return
        int current_n = number * -1 - 2;
        scls::Point_2D current_position = position_end;
        std::vector<char> to_return = std::vector<char>(number + 1);
        for(std::size_t i = 0;i<to_return.size();i++) {
            bool good = false;
            if(current_position.x() > 0){djikstra_2d_check_point(needed_map, current_n, scls::Point_2D(current_position.x() - 1, current_position.y()), good);}
            if(good){to_return[to_return.size() - (i + 1)] = djikstra_2d_right;current_position = scls::Point_2D(current_position.x() - 1, current_position.y());}
            if(!good && current_position.x() < needed_map.size() - 1){
                djikstra_2d_check_point(needed_map, current_n, scls::Point_2D(current_position.x() + 1, current_position.y()), good);
                if(good){to_return[to_return.size() - (i + 1)] = djikstra_2d_left;current_position = scls::Point_2D(current_position.x() + 1, current_position.y());}
            }
            if(!good && current_position.y() > 0){
                djikstra_2d_check_point(needed_map, current_n, scls::Point_2D(current_position.x(), current_position.y() - 1), good);
                if(good){to_return[to_return.size() - (i + 1)] = djikstra_2d_bottom;current_position = scls::Point_2D(current_position.x(), current_position.y() - 1);}
            }
            if(!good && current_position.y() < needed_map.at(0).size() - 1){
                djikstra_2d_check_point(needed_map, current_n, scls::Point_2D(current_position.x(), current_position.y() + 1), good);
                if(good){to_return[to_return.size() - (i + 1)] = djikstra_2d_top;current_position = scls::Point_2D(current_position.x(), current_position.y() + 1);}
            }
            current_n++;
        }

        return to_return;
    }
}
