//******************
//
// scls_math_set.cpp
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
// This file contains the source code of "scls_math_set.h".
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

	//*********
	//
	// The sets number class
	//
	//*********

	// Interval constructor
	Interval::Interval(Fraction start, bool start_included, Fraction end, bool end_included):a_end(end),a_end_included(end_included),a_start(start),a_start_included(start_included){};
	Interval::Interval(Fraction start, Fraction end):Interval(start, true, end, true){};
	Interval::Interval(Fraction value):Interval(value, true, value, true){};
	Interval::Interval():Interval(Fraction(0), Fraction(0)){};
	Interval::Interval(const Interval& other):a_end(other.a_end),a_end_included(other.a_end_included),a_end_infinite(other.a_end_infinite),a_start(other.a_start),a_start_included(other.a_start_included),a_start_infinite(other.a_start_infinite){}

	// Clears the interval
	void Interval::clear(){a_end=0;a_end_included=false;a_end_infinite=false;a_start=0;a_start_included=false;a_start_infinite=false; }
	// Compares this definition set with another
	bool Interval::compare(Interval value){return ((a_end_infinite == value.a_end_infinite) && (a_end_infinite || a_end == value.a_end)) && ((a_start_infinite == value.a_start_infinite) && (a_start_infinite || a_start == value.a_start)); };
	// Returns if the interval is empty or not
	bool Interval::is_empty() const {return a_start == a_end && (!a_start_included || !a_end_included) && !a_start_infinite && !a_end_infinite;};
	// Returns if a value is in an interval
	bool Interval::is_in(Fraction value){return ((a_end_infinite || value < a_end || (value == a_end && a_end_included)) && (a_start_infinite || value > a_start || (value == a_start && a_start_included)));};
	// Returns if an interval is in an interval
	bool Interval::is_in(Interval* value){return (((a_end_infinite) || (!value->a_end_infinite && value->a_end <= a_end)) && ((a_start_infinite) || (!value->a_start_infinite && value->a_start >= a_start)));};
	// Returns the intersection between this interval and an another interval
	Interval Interval::intersection(Fraction needed_start, bool needed_start_included, Fraction needed_end, bool needed_end_included){return intersection(Interval(needed_start, needed_start_included, needed_end, needed_end_included));};
	Interval Interval::intersection(Interval other) {
		// Empty interval
		if((other.a_start <= a_start && other.a_end < a_start && !a_start_infinite && !other.a_end_infinite) || (a_start <= other.a_start && a_end < other.a_start && !a_end_infinite && !other.a_start_infinite)){return Interval(0, false, 0, false);}
		else if(other.a_end == a_start && !a_start_infinite && !other.a_end_infinite) {
			if(other.a_end_included & a_start_included){return Interval(a_start);}
			else{return Interval(0, false, 0, false);}
		}
		else if(other.a_start == a_end && !other.a_start_infinite && !a_end_infinite) {
			if(other.a_start_included & a_end_included){return Interval(a_end);}
			else{return Interval(0, false, 0, false);}
		}

		// There is an intersection
		// Get the end
		Fraction needed_end; bool needed_end_included = false; bool needed_end_infinite = false;
		if(other.a_end_infinite && a_end_infinite){needed_end_infinite=true;}
		else if(other.a_end > a_end || other.a_end_infinite){needed_end=a_end;needed_end_included=a_end_included;}
		else if(a_end > other.a_end ||a_end_infinite){needed_end=other.a_end;needed_end_included=other.a_end_included;}
		else {needed_end=a_end;needed_end_included=a_end_included&&other.a_end_included;}
		// Get the start
		Fraction needed_start; bool needed_start_included = false; bool needed_start_infinite = false;
		if(other.a_start_infinite && a_start_infinite){needed_start_infinite=true;}
		else if(other.a_start < a_start || other.a_start_infinite){needed_start=a_start;needed_start_included=a_start_included;}
		else if(a_start < other.a_start || a_start_infinite){needed_start=other.a_start;needed_start_included=other.a_start_included;}
		else {needed_start=a_start;needed_start_included=a_start_included&&other.a_start_included;}

		// Returns the result
		Interval to_return = Interval(needed_start, needed_start_included, needed_end, needed_end_included);
		to_return.set_end_infinite(needed_end_infinite);to_return.set_start_infinite(needed_start_infinite);
		return to_return;
	}

	// Returns the Interval to an std::string
	std::string Interval::end_to_std_string(Textual_Math_Settings* settings)const{if(a_end_infinite){return std::string("+inf");}return a_end.to_std_string(settings);}
	std::string Interval::start_to_std_string(Textual_Math_Settings* settings)const{if(a_start_infinite){return std::string("-inf");}return a_start.to_std_string(settings);}
	std::string Interval::to_std_string(Textual_Math_Settings* settings) {
		std::string to_return = std::string();

		if(a_start_infinite||!a_start_included){to_return+=std::string("]");}
		else{to_return+=std::string("[");}

		to_return+=start_to_std_string(settings);
		to_return+=std::string(";");
		to_return+=end_to_std_string(settings);

		if(a_end_infinite||!a_end_included){to_return+=std::string("[");}
		else{to_return+=std::string("]");}

		return to_return;
	};

	// Compares this definition set with another
	bool Set_Number::compare(Set_Number value) {
		// Compare size
		if(a_numbers.size() != value.a_numbers.size() || a_intervals.size() != value.a_intervals.size()){return false;}

		// Compare intervals and values
		for(int i = 0;i<static_cast<int>(a_intervals.size())&&i<static_cast<int>(value.a_intervals.size());i++){if(a_intervals[i] != value.a_intervals[i]){return false;}}
		for(int i = 0;i<static_cast<int>(a_numbers.size())&&i<static_cast<int>(value.a_numbers.size());i++){if(a_intervals[i] != value.a_intervals[i]){return false;}}
		return true;
	}

	// Checks the intervals / numbers
	void Set_Number::check_intervals() {
		// Check the end (infinite)
		for(int i = 1;i<static_cast<int>(a_intervals.size());i++) {
			int current_i = (a_intervals.size() - (i + 1));
			if(a_intervals.at(current_i).end_infinite() && a_intervals.at(current_i + 1).end_infinite()) {
				if(a_intervals.at(current_i).start_infinite() || a_intervals.at(current_i + 1).start_infinite()){a_intervals.at(current_i).set_start_infinite(true);}
				else if(a_intervals.at(current_i).start() > a_intervals.at(current_i + 1).start()){
					a_intervals.at(current_i).set_start(a_intervals.at(current_i + 1).start());
					a_intervals.at(current_i).set_start_included(a_intervals.at(current_i + 1).start_included());
				}
				else if(a_intervals.at(current_i).start() == a_intervals.at(current_i + 1).start()){a_intervals.at(current_i).set_start_included(a_intervals.at(current_i).start_included() || a_intervals.at(current_i + 1).start_included());}
				a_intervals.pop_back();i--;
			}
		}

		// Check the start (infinite)
		for(int i = 1;i<static_cast<int>(a_intervals.size());i++) {
			if(a_intervals.at(i - 1).start_infinite() && a_intervals.at(i).start_infinite()) {
				if(a_intervals.at(i - 1).end_infinite() || a_intervals.at(i).end_infinite()){a_intervals.at(i - 1).set_end_infinite(true);}
				else if(a_intervals.at(i - 1).end() < a_intervals.at(i).end()){
					a_intervals.at(i - 1).set_end(a_intervals.at(i).end());
					a_intervals.at(i - 1).set_end_included(a_intervals.at(i).end_included());
				}
				else if(a_intervals.at(i - 1).end() == a_intervals.at(i).end()){a_intervals.at(i - 1).set_end_included(a_intervals.at(i - 1).end_included() || a_intervals.at(i).end_included());}
				a_intervals.erase(a_intervals.begin() + i);i--;
			}
		}

		for(int i = 0;i<static_cast<int>(a_intervals.size() - 1);i++) {
			if(a_intervals[i].is_empty() || (a_intervals[i].start() > a_intervals[i].end() && !a_intervals[i].start_infinite() && !a_intervals[i].end_infinite())){a_intervals.erase(a_intervals.begin() + i);i--;}
			else if(a_intervals[i].end() > a_intervals[i + 1].start() || (a_intervals[i].end() == a_intervals[i + 1].start() && (a_intervals[i].end_included() || a_intervals[i + 1].start_included()))){
				scls::Interval new_interval = scls::Interval(a_intervals[i].start(), a_intervals[i].start_included(), a_intervals[i + 1].end(), a_intervals[i + 1].end_included());
				// Set the start
				if(a_intervals[i].start_infinite() || a_intervals[i + 1].start_infinite()){new_interval.set_start_infinite(true);}
				else if(a_intervals[i + 1].start() < a_intervals[i].start()){new_interval.set_start(a_intervals[i + 1].start());new_interval.set_start_included(a_intervals[i + 1].start_included());}
				else if(a_intervals[i + 1].start() == a_intervals[i].start()){new_interval.set_start_included(new_interval.start_included() && a_intervals[i + 1].start_included());}
				// Set the end
				if(a_intervals[i].end_infinite() || a_intervals[i + 1].end_infinite()){new_interval.set_end_infinite(true);}
				else if(a_intervals[i].end() > a_intervals[i + 1].end()){new_interval.set_end(a_intervals[i].end());new_interval.set_end_included(a_intervals[i].end_included());}
				else if(a_intervals[i + 1].end() == a_intervals[i].end()){new_interval.set_end_included(new_interval.start_included() && a_intervals[i].start_included());}
				// Finalize the creation
				a_intervals[i] = new_interval;
				a_intervals.erase(a_intervals.begin() + i + 1);
				i--;
			}
		}
	}
	// Excludes a value
	void Set_Number::exclude(scls::Fraction number) {
		for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
			if(a_intervals.at(i).end() >= number || a_intervals.at(i).end_infinite()) {
				if(a_intervals.at(i).start() <= number || a_intervals.at(i).start_infinite()) {
					if(a_intervals.at(i).start() != a_intervals.at(i).end() || a_intervals.at(i).end_infinite() || a_intervals.at(i).start_infinite()) {
						// Create a new interval
						a_intervals.insert(a_intervals.begin() + i + 1, a_intervals.at(i));

						// Edit the first interval
						a_intervals.at(i).set_end(number);
						a_intervals.at(i).set_end_included(false);
						a_intervals.at(i).set_end_infinite(false);

						// Edit the second interval
						a_intervals.at(i + 1).set_start(number);
						a_intervals.at(i + 1).set_start_included(false);
						a_intervals.at(i + 1).set_start_infinite(false);
					}
					else {a_intervals.erase(a_intervals.begin() + i);}

					// Checks the interval
					check_intervals();
					return;
				}
				else{return;}
			}
		}
	}
	void Set_Number::exclude(Interval interval) {
		if(interval.start_infinite() && interval.end_infinite()){clear();}
		else if(interval.start_infinite()) {
			// Start infinite
			for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
				if(a_intervals.at(i).start() > interval.end() && !a_intervals.at(i).start_infinite()){break;}
				else if(a_intervals.at(i).start() == interval.end() && !a_intervals.at(i).start_infinite()){break;}
				else if((a_intervals.at(i).start() < interval.end() || a_intervals.at(i).start_infinite()) && (interval.end() < a_intervals.at(i).end() || a_intervals.at(i).end_infinite())){
					a_intervals[i].set_start(interval.end());
					a_intervals[i].set_start_included(!interval.end_included());
					a_intervals[i].set_start_infinite(false);
					break;
				}
				else{a_intervals.erase(a_intervals.begin());i--;}
			}
		}
		else if(interval.end_infinite()) {
			// End infinite
			for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
				int current_i = static_cast<int>(a_intervals.size()) - (i + 1);
				if(a_intervals.at(current_i).end() < interval.start() && !a_intervals.at(current_i).end_infinite()){break;}
				else if(a_intervals.at(current_i).end() == interval.start() && !a_intervals.at(current_i).end_infinite()){break;}
				else if((a_intervals.at(current_i).end() > interval.start() || a_intervals.at(current_i).end_infinite()) && (a_intervals.at(current_i).start() < interval.start() || a_intervals.at(current_i).start_infinite())){
					a_intervals[current_i].set_end(interval.start());
					a_intervals[current_i].set_end_included(!interval.start_included());
					a_intervals[current_i].set_end_infinite(false);
					break;
				}
				else{a_intervals.pop_back();}
			}
		}
		else {
			// Go to the next interval
			int i = 0;
			while(i < static_cast<int>(a_intervals.size()) && a_intervals.at(i).end() < interval.start()){i++;}
			while(i < static_cast<int>(a_intervals.size()) && a_intervals.at(i).start() < interval.end()){
				if(a_intervals.at(i).start() < interval.start()){
					a_intervals[i].set_end(interval.start());a_intervals[i].set_end_included(interval.start_included());
					i++;
				}
				else{a_intervals.erase(a_intervals.begin() + i);}
			}
		}
	}
	void Set_Number::exclude(scls::Set_Number* set_number) {for(int i = 0;i<static_cast<int>(set_number->a_intervals.size());i++){exclude(set_number->a_intervals.at(i));}}
	// Returns the intersection between this set and an interval
	Set_Number Set_Number::intersection(Interval other) {
		Set_Number to_return;

		// Calculate all the intersections
		for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
			Interval current_intersection = a_intervals[i].intersection(other);
			if(!current_intersection.is_empty()){to_return.add_interval(current_intersection);}
		}

		return to_return;
	}
	Set_Number Set_Number::intersection(Set_Number other) {
		Set_Number to_return;

		// Calculate all the intersections
		for(int i = 0;i<static_cast<int>(a_intervals.size());i++) {
			Set_Number current_intersection = other.intersection(a_intervals[i]);
			if(!current_intersection.is_empty()){for(int j = 0;j<static_cast<int>(current_intersection.a_intervals.size());j++){to_return.add_interval(current_intersection.a_intervals.at(j));}}
		}

		// Calculate all the isolated values
		for(int i = 0;i<static_cast<int>(to_return.a_intervals.size());i++) {
			for(int j = 0;j<static_cast<int>(a_numbers.size());j++) {
				Set_Number current_intersection = other.intersection(a_intervals[i]);
				if(!current_intersection.is_empty()){for(int j = 0;j<static_cast<int>(current_intersection.a_intervals.size());j++){to_return.add_interval(current_intersection.a_intervals.at(j));}}
			}
		}

		return to_return;
	}
	// Comparaison function for the interval sorting
	bool __sort_interval_function(const Interval& i_1, const Interval& i_2) {if(!i_1.start_infinite() && i_2.start_infinite()){return false;} return i_1.start() < i_2.start() || (i_1.start() == i_2.start() && (i_2.start_included() || !i_1.start_included()));};
	// Comparaison function for the numbers sorting
	bool __sort_numbers_function(const Complex& i_1, const Complex& i_2) {return i_1.real() < i_2.real();};
	// Sort the intervals / numbers in the set
	void Set_Number::__sort_interval() {std::sort(a_intervals.begin(), a_intervals.end(), __sort_interval_function);};
	void Set_Number::__sort_numbers() {std::sort(a_numbers.begin(), a_numbers.end(), __sort_numbers_function);};
	// Returns the set in a std::string
	std::string Set_Number::to_std_string(Textual_Math_Settings* settings) {
		std::string to_return = "";

		// Add the isolated elements
		for(int i = 0;i<static_cast<int>(numbers().size());i++) {
			to_return += numbers().at(i).to_std_string(settings);
			if(i < static_cast<int>(numbers().size()) - 1){to_return += std::string(";");}
		}

		// Add the intervals
		if(to_return != std::string()){to_return = std::string("{") + to_return + std::string("}");}
		for(int i = 0;i<static_cast<int>(intervals().size());i++) {
			if(to_return != std::string()){to_return += std::string(" U ");}
			to_return += a_intervals.at(i).to_std_string(settings);
		}

		return to_return;
	}
	// Unites with an another set
	void Set_Number::unite(Interval number) {add_interval(number);}
	void Set_Number::unite(Set_Number* set_number) {for(int i = 0;i<static_cast<int>(set_number->a_intervals.size());i++){if(!set_number->a_intervals.at(i).is_empty()){a_intervals.push_back(set_number->a_intervals.at(i));}}__sort_interval();check_intervals();}

	// Returns the complement between this sets and the real set (and another one if needed)
	Set_Number Set_Number::complement_real(Set_Number* set_number) {return scls::Set_Number::real().exclusion(reunion(set_number));}
	// Returns the complement between two sets
	Set_Number Set_Number::complement_relative(Set_Number* set_number) {Set_Number to_return = *this;return to_return;}
	Set_Number Set_Number::complement_relative_symetrical(Set_Number* set_number){return intersection(*set_number).reunion(complement_real(set_number));};
}
