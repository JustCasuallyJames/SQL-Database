#pragma once
#include <iostream>
#include <vector>

#include "BPlusTrees.h"
#include "MPair.h"
using namespace std;

template <typename K, typename V>
class MMap {
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator {
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it) {
            _it = it;
        }
        Iterator operator ++(int unused) { //postfix
            Iterator temp = _it;
            _it++;
            return temp;
        }
        Iterator operator ++() { //prefix
            _it++;
            return *this;
        }
        vector<V> operator *() {
            return _it->_value_list;
        }
        const vector<V>* operator ->() {
            return &_it.operator->()->_value_list;
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
        typename map_base::Iterator _it;
    };

    MMap() {}

    //  Iterators
    Iterator begin();
    Iterator end();

    //  Capacity
    int size() ;
    bool empty() ;

    //  Element Access
    const vector<V>& operator[](const K& key) const;
    vector<V>& operator[](const K& key);

    //  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

    //  Operations:
    bool contains(const K& key) ;
    vector<V>& get(const K& key);
    const vector<V>& get(const K& key) const;

    vector<V> find(const K& key);

    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);


    bool is_valid() { return _mmap.is_valid(); }
    friend ostream& operator<<(ostream& outs, const vector<V>& list) {
        for (int i = 0; i < list.size(); i++) {
            outs << list[i];
            if (list.size() != i + 1) {
                outs << ",";
            }
        }
        return outs;
    }
    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me) {
        outs << print_me._mmap << endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > _mmap;
};

template<typename K, typename V>
typename MMap<K,V>::Iterator MMap<K, V>::begin() {
    return Iterator(_mmap.begin());
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::end() {
    return Iterator(_mmap.end());
}

template<typename K, typename V>
inline int MMap<K, V>::size() {
    return _mmap.size();
}

template<typename K, typename V>
inline bool MMap<K, V>::empty(){
    return _mmap.empty();
}

template<typename K, typename V>
const vector<V>& MMap<K, V>::operator[](const K& key) const {

    return _mmap.get(key)._value_list;
}

template<typename K, typename V>
vector<V>& MMap<K, V>::operator[](const K& key) {
    if (!_mmap.contains(key)) {
        _mmap.insert(key);
    }
    return _mmap.get(key)._value_list;
}

template<typename K, typename V>
void MMap<K, V>::insert(const K& k, const V& v) {
    _mmap.insert(MPair<K, V>(k, v));
}

template<typename K, typename V>
inline void MMap<K, V>::erase(const K& key) {

}

template<typename K, typename V>
void MMap<K, V>::clear() {
    _mmap.clear_tree();
}

template<typename K, typename V>
bool MMap<K, V>::contains(const K& key){
    return _mmap.find(MPair<K, V>(key)) != Iterator(nullptr);
    //return _mmap.find_ptr(MPair<K,V>(key)) != nullptr;
    //return _mmap.contains(MPair<K,V>(key,V()));
}

template<typename K, typename V>
std::vector<V>& MMap<K, V>::get(const K& key) {
    return _mmap.get(MPair<K, V>(key))._value_list;
}

template<typename K, typename V>
const vector<V>& MMap<K,V>::get(const K& key) const {
    return _mmap.get(MPair<K, V>(key))._value_list;
}

template<typename K, typename V>
typename vector<V> MMap<K, V>::find(const K& key) {
    BPlusTree<MPair<K, V> >* found = _mmap.find_ptr(MPair<K, V>(key));
    if (found) {
        return *found->_value_list;
        //return *(_mmap.find(MPair<K, V>(key)))._value_list; //returns the value 
    } else {
        return V(); //returns default value if it cannot find the key within the tree (probably won't have this case though)
    }
}

template<typename K, typename V>
typename MMap<K,V>::Iterator MMap<K, V>::lower_bound(const K& key) {
    return Iterator(_mmap.lower_bound(key));
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::upper_bound(const K& key) {
    return Iterator(_mmap.upper_bound(key));
}


