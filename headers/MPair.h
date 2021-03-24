#pragma once
#include <iostream>
#include <vector>

#include "BTreeFunctions.h"
using namespace std;
template <typename K, typename V>
struct MPair {
    K _key;
    std::vector<V> _value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
     //--------------------------------------------------------------------------------
    MPair(const K& k = K());
    MPair(const K& k, const V& v);
    MPair(const K& k, const vector<V>& vlist);
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me) {
        outs << print_me._key << ":";
        for (int i = 0; i < print_me._value_list.size(); i++) {
            outs << print_me._value_list[i];
            if (print_me._value_list.size() != i + 1) {
                outs << ",";
            }
        }
        return outs;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs._key == rhs._key;
    }
    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs._key != rhs._key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs._key < rhs._key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs._key <= rhs._key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs._key > rhs._key;
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs._key >= rhs._key;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        MPair<K, V> temp;
        temp._key = lhs._key;
        for (int i = 0; i < lhs._value_list.size(); i++) {
            temp._value_list.push_back(lhs._value_list[i]);
        }
        for (int i = 0; i < rhs._value_list.size(); i++) {
            temp._value_list.push_back(rhs._value_list[i]);
        }
        return temp;
    }

    friend MPair<K, V> operator +=(const MPair<K, V>& lhs, const V& value) {
        lhs._value_list.push_back(value);
        return lhs;
    }
    MPair<K, V> operator +=(const MPair<K, V>& rhs);
};
template <typename K, typename V>
MPair<K, V>::MPair(const K& k):_key(k) {}
template <typename K, typename V>
MPair<K, V>::MPair(const K& k, const V& v):_key(k) {
    _value_list.push_back(v);
}
template <typename K, typename V>
MPair<K,V>::MPair(const K& k, const vector<V>& vlist) {
    //set one vector to the other vector
    _value_list = vlist;
}

template<typename K, typename V>
MPair<K, V> MPair<K, V>::operator+=(const MPair<K, V>& rhs) {
    for (int i = 0; i < rhs._value_list.size(); i++) {
        _value_list.push_back(rhs._value_list[i]);
    }
    return _value_list;
}



