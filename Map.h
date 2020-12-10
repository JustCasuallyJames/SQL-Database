#pragma once

#include <iostream>
#include "Pair.h"
#include "BPlusTrees.h"
template<typename K, typename V>
class Map {
public:
    
    class Iterator {

    public:
        friend class Map;
        Iterator (typename BPlusTree<Pair<K,V> >::Iterator it) {
            _it = it;
        }

        Iterator operator ++(int unused) { //postfix operator
            _it++;
            return *this;

        }
        Iterator operator ++() { //prefix operator
            ++_it;
            return *this;

        }
        Pair<K,V> operator *() {
            return *_it; //obtains the pair's value
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }

        K get_key() {
            return _it->_key;
        }
    private:
        typename BPlusTree<Pair<K,V> >::Iterator _it;
    };

    Map();
    Map(const Map<K, V>& other);
    //  Iterators
    Iterator begin();
    Iterator end();

    //  Capacity
    int size() ;
    bool empty() ;

    //  Element Access

    //non-const version of the [] operator, this will allows us to modify the value at that point too 
    const V& get(const K& key) const;
    V& operator[](const K& key);
    const V& operator[](const K& key) const;
    V& at(const K& key);


    //  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

    //  Operations:
    Iterator find(const K& key);
    bool contains(const K& key) const;

    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);

    bool is_valid() { return _map.is_valid(); }

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me) {
        outs << print_me._map << endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K,V> > _map;
};

template<typename K, typename V>
Map<K, V>::Map() {
    key_count = 0;
}
template<typename K, typename V>
Map<K,V>::Map(const Map<K, V>& other) {
    _map.copy_tree(other._map);
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::begin() {
    return Iterator(_map.begin());
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::end() {
    return Iterator(_map.end());
}

template<typename K, typename V>
inline int Map<K, V>::size(){
    return _map.size();
}

template<typename K, typename V>
inline bool Map<K, V>::empty() {
    return _map.empty();
}

template<typename K, typename V>
const V& Map<K, V>::get(const K& key) const {
    return _map.get(Pair<K,V>(key,V()))._value;
}

template<typename K, typename V>
//non constant version of the brackets so it can be modified.
V& Map<K, V>::operator[](const K& key) { //if it's not there, it will add it.
    if (_map.find(Pair<K, V>(key, V())) == Iterator(nullptr)) {
        _map.insert(Pair<K, V>(key, V()));
    }
    //if (!_map.contains(Pair<K,V>(key,V()))) {
    //    _map.insert(Pair<K, V>(key, V()));
    //}
    return _map.get(Pair<K, V>(key, V()))._value;
}

template<typename K, typename V>
const V& Map<K, V>::operator[](const K& key) const {

    return _map.get(Pair<K,V>(key,V()))._value;
}

template<typename K, typename V>
V& Map<K, V>::at(const K& key) {
    if (_map.contains(Pair<K, V>(key, V()))) {
        return _map.get(Pair<K, V>(key, V()))._value;
    }
    else {
        _map.insert(Pair<K, V>(key, V()));
        return _map.get(Pair<K, V>(key, V()))._value;
    }
}

template<typename K, typename V>
void Map<K, V>::insert(const K& k, const V& v) {
    _map.insert(Pair<K, V>(k, v));
}

template<typename K, typename V>
//this is the remove 
void Map<K, V>::erase(const K& key) {

}

template<typename K, typename V>
//clears the ENTIRE map.
void Map<K, V>::clear() {
    _map.clear_tree();
}

template <typename K, typename V>
V Map<K, V>::get(const K& key) {
    return _map.get(Pair<K, V>(key, V()))._value;
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::find(const K& key) {
    return (_map.find(Pair<K,V>(key, V())));
}

template<typename K, typename V>
inline bool Map<K, V>::contains(const K& key) const {
    return _map.contains(Pair<K,V>(key,V()));
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::lower_bound(const K& key) {
    return Iterator(_map.lower_bound(key));
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::upper_bound(const K& key) {
    return Iterator(_map.upper_bound(key));
}

