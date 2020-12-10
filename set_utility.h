#pragma once
#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;
template<typename T>
vector<T> vector_instersection(vector<T>& v1, vector<T>& v2) {

    vector<T> v3;


    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());


    set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

    return v3;
}
template<typename T>
vector<T> vector_union(vector<T>& v1, vector<T>& v2) {

    vector<T> v3;


    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    

    set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

    return v3;
}