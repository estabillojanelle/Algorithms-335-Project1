///////////////////////////////////////////////////////////////////////////////
// algorithms.hpp
//
// Definitions for three algorithms:
//
// find_dip
// longest_balanced_span
// telegraph_style
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <optional>
#include <string>
#include <vector>
#include <iterator>
#include <assert.h>
#include<iostream>

using namespace std;
namespace algorithms {

    // A "dip" is a series of three elements in a row, where the first and third
    // are equal to each other, and the middle element is less than the others.
    // For example, the values 8, 5, 8 are considered a dip. This function returns
    // an iterator indicating the location of the last dip in values, if any
    // exists.
    //
    // When values contains one dip, returns an iterator pointing to the start of
    // that dip (i.e. the first of the three elements in the dip).
    //
    // When values contains multiple dips, returns an iterator pointing to the
    // start of the LAST dip (i.e. the dip at the highest index).
    //
    // When values does not contain any dip, returns a past-the-end iterator,
    // i.e. values.end() .
    //
    // Note that when values has fewer than 3 elements, it cannot contain a dip, so
    // the function always returns values.end() in this case.
    std::vector<int>::const_iterator find_dip(const std::vector<int>& values) {
        int last_dip = -99;
        if (values.size() < 3) {
            return values.end();
        }
        else {
            for (int i = 0; i <= values.size() - 3; i++) {
                if (values.at(i) == values.at(i + 2) && values.at(i + 1) < values.at(i)) {
                    last_dip = i;
                }
            }
            std::vector<int>::const_iterator result = values.begin();
            if (last_dip >= 0) {
                cout << last_dip;
                advance(result, last_dip);
                return result;
            }
            else {
                cout << last_dip;
                return values.end();
            }
        }
    }

    // A span represents a non-empty range of indices inside of a vector of ints,
    // stored in a begin iterator and end iterator. Just like in the rest of the C++
    // standard library, the range includes all elements in [begin, end), or in
    // other words the range includes begin, and all elements up to BUT NOT
    // INCLUDING end itself.
    class span {
    private:
        std::vector<int>::const_iterator begin_, end_;

    public:

        // Create a span from two iterators. Both iterators must refer to the same
        // vector. begin must come before end.
        span(std::vector<int>::const_iterator begin,
            std::vector<int>::const_iterator end)
            : begin_(begin), end_(end) {
            assert(begin < end);
        }

        // Equality tests, two spans are equal when each of their iterators are equal.
        bool operator== (const span& rhs) const {
            return (begin_ == rhs.begin_) && (end_ == rhs.end_);
        }

        // Accessors.
        const std::vector<int>::const_iterator& begin() const { return begin_; }
        const std::vector<int>::const_iterator& end() const { return end_; }

        // Compute the number of elements in the span.
        size_t size() const { return end_ - begin_; }
    };

    // Find the longest "balanced" span in values.
    //
    // A span is balanced when its sum is zero. For example, the elements
    // 5, -8, 2, 1 constitute a balanced span because 5+(-8)+2+1 == 0. Also, the
    // elements 0, 0, 0 constitute a balanaced span because 0+0+0 == 0.
    //
    // When values contains only one balanced span, return that span.
    //
    // When values contains multiple balanced spans, return the span that is longest
    // i.e. contains the most elements. In the even of a tie between two different
    // spans of the same length, return whichever comes LAST, i.e. whichever starts
    // at the higher index.
    //
    // When values contains no balanced span, return an empty optional object.
    //
    // Note that when values is empty, it cannot have any balanced span, so the
    // function always returns an empty optional object in this case.
    std::optional<span> longest_balanced_span(const std::vector<int>& values) {
        if (values.size() == 0) {
            return std::nullopt;
        }

        int begin = 0; // record the begin of return span
        int end = 0;   // record the end of return span
        int best = -99;   // best span 
        int sum = 0;
        int quantity = 0;  // length of each found span

        for (int start = 0; start < values.size(); start++) {   // start from 1st element of vector
            sum = values.at(start);
            if (values.at(start) == 0) { // iff that element is 0
                quantity = 0;
                if ((best < 0) || (quantity >= best)) {    //check if have any
                    best = quantity;
                    begin = start;   // save the posistion of the start of return span
                    end = start + 1;

                }
            }
            for (int finish = start + 1; finish < values.size(); finish++) { // loop through the rest elements and add them up
                sum = sum + values.at(finish);  //add them up
                if (sum == 0) {  // if sum = 0
                    quantity = finish - start;   // get the length of found span

                    if ((best == 0) || (quantity >= best)) {    // determine if found span is the best
                        best = quantity;
                        begin = start;   // save the posistion of the start of return span
                        end = finish + 1;      // save the end
                    }
                }
            }
        }
        std::vector<int>::const_iterator begin_ = values.begin();
        std::vector<int>::const_iterator end_ = values.begin();
        advance(begin_, begin);  // move the iterator to the position of the begin of return span
        advance(end_, end);  // move to the end
        /* now print out for testing purpose
        int position = -1;
        for (auto& element : values)
        {
            position++;
            cout << position << "||||"<< element << endl;

        }
        cout << "From " << begin << " to " << end<< endl;
        cout << "longest streak = " << best << endl;
        // end the testing part */
        if (best >= 0) {
            span result = span(begin_, end_);
            return result;
        }
        else {
            return std::nullopt;
        }
    }

    // A "telegraph-style" string is suitable for transmission via
    // telegram. This function takes a string s as input, and returns a
    // version of the string converted to telegraph-style.
    //
    // This function makes the following changes:
    // - All lower-case letters are converted to upper-case.   done
    // - Punctuation characters !?; are converted to periods.  done
    // - Only some characters are allowed. After the conversions
    //   stated above, any character that does not match one of
    //   the following allowed categories, is removed:
    //   - upper-case letters  y
    //   - digits  y
    //   - space    
    //   - period y
    // - There can only ever be one space in a row. Multiple contiguous
    //   spaces are replace dwith a single space.   done
    // - The string must end in "STOP.". If s does not already end in
    //   "STOP." then add "STOP." to the end.
    std::string telegraph_style(const std::string& s) {

        bool flag = false;
        string result;
        int size = s.size();
        string sub; 
        if (size == 0) {
            return result = result + "STOP.";
        }
        for (int i = 0; i < size; i++) {

            if (s[i] == '!' || s[i] == '?' || s[i] == ';') { //Punctuation characters !? ; are converted to periods.
                result += ".";
                flag = false;
            }
            else if ((isalpha(s[i]))) { // upper - case letters

                result += toupper(s[i]);
                flag = false;
            }
            else if (isdigit(s[i]) || s[i] == '.') {     // digits and period.
                result += s[i];
                flag = false;
            }          
            else if (s[i] == ' ') {   //- There can only ever be one space in a row. Multiple contiguous //  spaces are replace dwith a single space.
                if (flag ==false)
                {
                    result += s[i];
                    flag = true;
                }
            }
               
        }        
        if (result.size() >= 5) {
            sub = result.substr((result.size() - 5), 5);        
            if (sub != "STOP.") {
                result = result + "STOP.";
            }
        }
        else {
            result = result + "STOP.";
        }
        //   cout << s << endl;
        // cout <<result <<endl;
        return result;
    }
}
