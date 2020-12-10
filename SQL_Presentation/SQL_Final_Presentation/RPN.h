#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#include "Queue.h"
#include "Stack.h"
#include "BPlusTrees.h"
#include "Map.h"
#include "MMap.h"
#include "string_utility.h"
using namespace std;

template <typename T>
class RPN {
public:
    RPN(Queue<string> post, Map<string, MMap<string, long>> indices);
    vector<T> vector_instersection(vector<T>& v1, vector<T>& v2) {

        vector<T> v3;

        //    sort(v1.begin(), v1.end());
        //    sort(v2.begin(), v2.end());

        set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

        return v3;
    }
    vector<T> vector_union(vector<T>& v1, vector<T>& v2) {

        vector<T> v3;

        //    sort(v1.begin(), v1.end());
        //    sort(v2.begin(), v2.end());

        set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

        return v3;
    }
    //to do rpn
    vector<long> evaluate(Queue<string>& conditions);
    //gets ALL of the values that are greater than and equal
    vector<long> get_greater_equal(string sfield, string svalue);
    //gets all the values that are greater
    vector<long> get_greater(string sfield, string svalue);
    //gets all the values that are less than
    vector<long> get_less(string sfield, string svalue);
    //gets all the values that are less than or equal
    vector<long> get_less_equal(string s1, string svalue);
    vector<long> get_equal(string sfield, string svalue);

private:
    Queue<string> _postfix;
    Map<string, MMap<string, long>> _indices;
};
template<typename T>
RPN<T>::RPN(Queue<string> post, Map<string, MMap<string, long>> indices) {
    _postfix = post;
    _indices = indices;
}
template<typename T>
vector<long> RPN<T>::evaluate(Queue<string>& conditions) {
    Stack<vector<long> > results;
    while (!conditions.empty()) {
        /*
            so theres going to be at least three things 
            its always going to be in this format
            [field] [value] [op]
            last vo =
        */
        //we want to check if it's not a logical operator
        if (uppercase(conditions.top()) != "AND" && uppercase(conditions.top()) != "OR") {
            //then we KNOW we're at the situation where it's [field] [value] [op]
            string field = conditions.pop();
            string value = conditions.pop();
            string op = conditions.pop();
            if (op == "<") {
                //this will push in a vector of long of all the recno's that are less than the value
                //first < james
                results.push(get_less(field, value));
            } else if (op == "<=") {
                //this will push in a vector of long of all the recno's that are less than/equal to the value
                //last <= vo
                results.push(get_less_equal(field, value));
            } else if (op == ">") {
                //this will push in a vector of long of all the recno's that are greater than the value
                //salary > 1000000
                results.push(get_greater(field, value));
            } else if (op == ">=") {
                //this will push in a vector of long of all the recno's that are greater than/equal to the value
                //salary >= 1000000
                results.push(get_greater_equal(field, value));
            } else if (op == "=") {
                //this will push in values that had the specific value 
                //last = vo
                results.push(get_equal(field, value));
            }
        } else { //this will mean that this is logical operators like and/or
            string logic_op = conditions.pop();
            if (uppercase(logic_op) == "AND") {
                vector<long> first = results.pop();
                vector<long> second = results.pop();
                results.push(vector_instersection(first, second));
            } else if (uppercase(logic_op) == "OR") {
                vector<long> first = results.pop();
                vector<long> second = results.pop();
                results.push(vector_union(first, second));
            }
        }
    }
    return results.pop(); //this will be the only evaluated condition with everything included with logical operators
}

//gets ALL of the values that are greater than and equal
template<typename T>
vector<long> RPN<T>::get_greater_equal(string sfield, string svalue) {
    vector<long> results;
    for (MMap<string,long>::Iterator it = _indices[sfield].lower_bound(svalue); it != MMap<string,long>::Iterator(nullptr); it++) {
        for (int i = 0; i < it->size(); i++) { // i< it->size() is the vector size of the number of records
            results.push_back(it->at(i)); //this grabs the values within the vector<long> to be pushed into our results vector
        }
    }
    return results;
}
//gets all the values that are greater
template<typename T>
vector<long> RPN<T>::get_greater(string s1, string s2) {
    vector<long> results;
    for (MMap<string, long>::Iterator it = _indices[s1].lower_bound(s2); it != MMap<string, long>::Iterator(nullptr); it++) {
        for (int i = 0; i < it->size(); i++) { // i< it->size() is the vector size of the number of records
            results.push_back(it->at(i)); //this grabs the values within the vector<long> to be pushed into our results vector
        }
    }
    return results;
}
//gets all the values that are less than
template<typename T>
vector<long> RPN<T>::get_less(string sfields, string svalues) {
    vector<long> results;
    MMap<string, long> _index = _indices[sfields]; //this obtains which index field they're referring to
    for (MMap<string, long>::Iterator it = _index.begin(); it != _index.lower_bound(svalues); it++) {
        //we will start at the beginning of that field and increment upwards UNTIL we reach the value before
        //the svalue 
        //example, lets say we want to find last < vo, we would go through last, then we will start at the beginning of
        //the last's field category and go up until we reach the last name before "vo"
        for (int i = 0; i < (*it).size(); i++) { //then we add all the values into the results vector
            results.push_back(it->at(i));
        }
    }
    return results;
}
//gets all the values that are less than or equal
template<typename T>
vector<long> RPN<T>::get_less_equal(string sfields, string svalues) {
    vector<long> results;
    MMap<string, long> _index = _indices[sfields]; //this obtains which index field they're referring to
    for (MMap<string, long>::Iterator it = _index.begin(); it != _index.upper_bound(svalues); it++) {
        //we will start at the beginning of that field and increment upwards UNTIL we reach the value before AND including
        //the svalue 
        //example, lets say we want to find last <= vo, we would go through last, then we will start at the beginning of
        //the last's field category and go up until we reach the last name vo (inclusive)
        for (int i = 0; i < it->size(); i++) { //then we add all the values into the results vector
            results.push_back(it->at(i));
        }
    }
    return results;
}
//gets all the value that are equal
template<typename T>
vector<long> RPN<T>::get_equal(string sfields, string svalues) {
    vector<long> results;
    //we want to find the specific value in that field that matches it
    cout << endl << "printing out indices:\n\n" << _indices[sfields] << endl;
    if (_indices[sfields].contains(svalues)) { //if the svalue is within the _indices[sfields], then it will return true
        for (int i = 0; i < _indices[sfields][svalues].size(); i++) { //loops through the svalues vector size
            results.push_back(_indices[sfields][svalues][i]); //gets everything within the svalues vector
        }
    }
    return results;
}
