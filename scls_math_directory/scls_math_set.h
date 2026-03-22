//******************
//
// scls_math_set.h
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
// This file contains some functions to handle sets.
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

#ifndef SCLS_MATH_SET
#define SCLS_MATH_SET

#include "../scls_math.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

	//*********
	//
	// The sets class
	//
	//*********

	class Set {
		// Class representing a set
	public:
		// Set constructor
		Set(){};
		// Set destructor
		~Set(){};

	private:
		// Union set
		std::vector<std::shared_ptr<Set>> a_unions;
	};

	//*********
	//
	// The sets number class
	//
	//*********

	class Interval {
	    // Class representating an interval of real numbers
    public:

        // Interval constructor
        Interval(Fraction start, bool start_included, Fraction end, bool end_included);
        Interval(Fraction start, Fraction end);
        Interval(Fraction value);
        Interval();
        Interval(const Interval& other);

        // Clears the interval
        void clear();
        // Compares this definition set with another
        bool compare(Interval value);
        // Returns if the interval is empty or not
        bool is_empty() const;
        // Returns if a value is in an interval
        bool is_in(Fraction value);
        // Returns if an interval is in an interval
        bool is_in(Interval* value);
        // Returns the intersection between this interval and an another interval
        Interval intersection(Interval other);
        Interval intersection(Fraction needed_start, bool needed_start_included, Fraction needed_end, bool needed_end_included);

        // Returns the interval to an std::string
        std::string end_to_std_string(Textual_Math_Settings* settings) const;
        std::string start_to_std_string(Textual_Math_Settings* settings) const;
        std::string to_std_string(Textual_Math_Settings* settings);

        // Getters and setters
        inline Fraction end() const {return a_end;};
        inline bool end_included() const {return a_end_included;};
        inline bool end_infinite() const {return a_end_infinite;};
        inline void set_end(Fraction new_end) {a_end=new_end;};
        inline void set_end_included(bool new_end_included) {a_end_included=new_end_included;};
        inline void set_start(Fraction new_start) {a_start=new_start;};
        inline void set_start_included(bool new_start_included) {a_start_included=new_start_included;};
        inline Fraction start() const {return a_start;};
        inline bool start_included() const {return a_start_included;};
        inline void set_end_infinite(bool new_end_infinite) {a_end_infinite=new_end_infinite;};
        inline void set_start_infinite(bool new_start_infinite) {a_start_infinite=new_start_infinite;};
        inline bool start_infinite() const {return a_start_infinite;};

        // Operators
        // With Interval
        bool operator==(Interval value) {return compare(value);};
        bool operator!=(Interval value) {return !compare(value);};

    private:
        // End of the interval
        Fraction a_end = 0;
        // If the end is included or not
        bool a_end_included = false;
        // If the end is + infinite or not
        bool a_end_infinite = false;
        // Start of the interval
        Fraction a_start = 0;
        // If the start is included or not
        bool a_start_included = false;
        // If the start is + infinite or not
        bool a_start_infinite = false;
	};

	class Set_Number {
        // Class representating a set of numbers
    public:

        // Set_Number constructor
        Set_Number(){};
        Set_Number(Interval interval){a_intervals.push_back(interval);};
        Set_Number(const Set_Number& other):a_intervals(other.a_intervals),a_numbers(other.a_numbers){}

        // Add an interval to the set
        inline void add_interval(Interval to_add) {if(!to_add.is_empty()){a_intervals.push_back(to_add);__sort_interval();check_intervals();};};
        // Add a number to the set
        inline void add_number(Fraction to_add) {if(!is_in(to_add)){add_interval(Interval(to_add));}};

        // Clears the interval
        inline void clear(){a_intervals.clear();a_numbers.clear();};
        // Returns the cardinal of the set (only works for denombrable sets)
        inline int cardinal()const{return a_intervals.size();};
        // Compares this definition set with another
        bool compare(Set_Number value);

        // Set algebra
        // Excludes a value
        void exclude(scls::Fraction number);
        void exclude(Interval interval);
        void exclude(Set_Number* set_number);
        inline Set_Number exclusion(scls::Fraction number){Set_Number temp = *this;temp.exclude(number);return temp;};
        inline Set_Number exclusion(Interval interval){Set_Number temp = *this;temp.exclude(interval);return temp;};
        inline Set_Number exclusion(Set_Number set_number){Set_Number temp = *this;temp.exclude(&set_number);return temp;};
        // Returns the intersection between this set and an interval
        Set_Number intersection(Interval other);
        Set_Number intersection(Set_Number other);
        // Returns if the set is empty or not
        inline bool is_empty() const {return a_intervals.size()<=0;};
        // Returns if a value is in the set numbers
        inline bool is_in_intervals(scls::Fraction value){for(int i = 0;i<static_cast<int>(a_intervals.size());i++){if(a_intervals[i].is_in(value)){return true;}}return false;};
        inline bool is_in_numbers(scls::Fraction value){for(int i = 0;i<static_cast<int>(a_numbers.size());i++){if(a_numbers[i]==value){return true;}}return false;};
        inline bool is_in(scls::Fraction value){return is_in_intervals(value) || is_in_numbers(value);};
        // Returns if an interval is in the set
        inline bool is_in_intervals(Interval value){for(int i = 0;i<static_cast<int>(a_intervals.size());i++){if(a_intervals[i].is_in(&value)){return true;}}return false;};
        inline bool is_in(Interval value){return is_in_intervals(value);};
        // Returns if the set is infinite or not
        inline bool is_infinite() const {return a_intervals.size() > 0 && a_intervals.at(0).start_infinite() && a_intervals.at(0).end_infinite();};
        // Get the number in the set
        inline Fraction number(){if(intervals().size() == 1){return intervals().at(0).start();}return 0;};
        // Unites with an another set
        inline Set_Number reunion(Interval number){Set_Number temp = *this;temp.unite(number);return temp;};
        inline Set_Number reunion(Set_Number* set_number){Set_Number temp = *this;temp.unite(set_number);return temp;};
        inline Set_Number reunion(Set_Number set_number){return reunion(&set_number);};
        void unite(Interval number);
        void unite(Set_Number* set_number);
        inline void unite(Set_Number set_number){unite(&set_number);};

        // Sub-parties algebra
        // Returns the complement between this sets and the real set (and another one if needed)
        Set_Number complement_real(Set_Number* set_number);
        // Returns the complement between two sets
        Set_Number complement_relative(Set_Number* set_number);
        Set_Number complement_relative_symetrical(Set_Number* set_number);

        // Returns the set in a std::string
        std::string to_std_string(Textual_Math_Settings* settings);

        // Checks the intervals / numbers
        void check_intervals();
        // Sort the intervals / numbers in the set
        void __sort_interval();
        void __sort_numbers();

        // Predefined sets
        // "After" set
        static Interval after_strictly(scls::Fraction value){Interval it;it.set_start(value);it.set_start_included(false);it.set_end_infinite(true);return it;};
        // "Before" set
        static Interval before_strictly(scls::Fraction value){Interval it;it.set_end(value);it.set_end_included(false);it.set_start_infinite(true);return it;};
        // "Between" set
        static Interval between_strictly(scls::Fraction after, scls::Fraction before){if(after==before){return Interval();} Interval it;it.set_end(before);it.set_end_included(false);it.set_start(after);it.set_start_included(false);it.set_end_infinite(true);return it;};
        // Real set
        static Set_Number real(){return set_real();};
        static Set_Number set_real() {Set_Number to_return;Interval i;i.set_end_infinite(true);i.set_start_infinite(true);to_return.add_interval(i);return to_return;};

        // Operators
        // With Set_Number
        bool operator==(Set_Number value) {return compare(value);};
        bool operator!=(Set_Number value) {return !compare(value);};

        // Getters and setters
        const std::vector<Interval>& intervals() const {return a_intervals;};
        const std::vector<Fraction>& numbers() const {return a_numbers;};

    private:
        // Intervals of fractions in this set
        std::vector<Interval> a_intervals;
        // Numbers of fractions in this set
        std::vector<Fraction> a_numbers;
	};
}

#endif // SCLS_MATH_SET
