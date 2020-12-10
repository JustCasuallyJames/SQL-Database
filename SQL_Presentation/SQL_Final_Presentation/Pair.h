#pragma once

#include <iostream>
using namespace std;

template<typename K,typename V>
struct Pair {
	K _key;
	V _value;

	Pair();
	Pair(const K& key, const V& value);
	
	friend ostream& operator <<(ostream& outs, const Pair<K,V>& print_me) {
		outs << print_me._key << ":" << print_me._value ;
		return outs;
	}
	friend bool operator <(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs._key < rhs._key;
	}
	friend bool operator >(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs._key > rhs._key;
	}
	friend bool operator <=(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs._key <= rhs._key;
	}
	friend bool operator >=(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs._key >= rhs._key;
	}
	friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs._key == rhs._key;
	}
	friend bool operator !=(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		return lhs._key != rhs._key;
	}
	friend Pair<K,V> operator +(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
		//this essentially just returns the rhs side of what you're trying to add.
		return rhs;
	}
};

template<typename K, typename V>
Pair<K, V>::Pair() :_key(K()), _value(V()) {}

template<typename K, typename V>
Pair<K, V>::Pair(const K& key, const V& value) {
	_key = key;
	_value = value;
}