#pragma once
#include <cassert>
#include <vector>
using namespace std;
template <class T>
T maximal(const T& a, const T& b);                      //return the larger of the two items

template <class T>
void swap_val(T& a, T& b);  //swap the two items

template <class T>
int index_of_maximal(T data[], int n);                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[], int& n, T entry);        //insert entry into the sorted array
                                                        //data with length n
//declarations
template <typename T>
int first_ge(const T data[], int n, const T& entry);   //return the first index in data that is
                                                        //not less than entry
template <class T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

template <typename T>
void insert_item(T data[], int i, int& n, T entry);    //insert entry at index i in data
//--------------------------------------------------------------------------------------------------
template <typename T>
void delete_item(T data[], int i, int& n, T& entry);   //delete item at index i and place it in entry

template <typename T>
void attach_item(T data[], int& n, const T& entry);    //append entry to the right of data

template <typename T>
void detach_item(T data[], int& n, T& entry);          //remove the last element in data and place
                                                        //it in entry
//--------------------------------------------------------------------------------------------------
template <typename T>
void merge(T src[], const int& src_size, T dest[], int& dest_size);   //append data2 to the right of data1

template <typename T>
void split_vals(T src[], int& src_size, T dest[], int& dest_size);   //move n/2 elements from the right of data1
                                                        //and move to data2
//--------------------------------------------------------------------------------------------------
template <typename T>
void copy_array(const T src[], T dest[], const int src_size, int& dest_size);              //copy src[] into dest[]

template <typename T>
void print_array(const T data[], int n, int pos = 0);  //print array data



//definitions
template <typename T>
//return the first element in data that is not less than entry
int first_ge(const T data[], int n, const T& entry) {
    for (int i = 0; i < n; i++) {
        if (data[i] >= entry) { //returns the value that isn't less than entry
            return i;
        }
    }
    return n; //returns the end value if nothing else
}
template <typename T>
//insert entry at index i in data
void insert_item(T data[], int i, int& n, T entry) {
    n++;
    assert(i < n && i >= 0 && "insert_item: probably b/c (n < i) OR i>=0");
    //[1,2,3,70,90,0,0]
    //inserting 60 at index 2
    //[1,2,60,3,70,90]
    
    for (int j = n - 1; j > i; j--) {
        data[j] = data[j - 1];
    }
    data[i] = entry;
}

template <typename T>
//delete item at index i and place it in entry
void delete_item(T data[], int i, int& n, T& entry) {
    assert(n > 0 && "delete_item, n was less than 0");
    int counter = i + 1;
    T endVal = data[i];
    for (int j = i; j < n; j++, counter++) {
        data[j] = data[counter];
    }
    data[n] = T();
    data[--n] = T();
    entry = endVal;

    //[1,2,3,70,90,0,0]
    //delete 2 
    //[1,3,70,90,0,0,0]
    // && counter < n;
}


template <typename T>
//append entry to the right of data
//done and tested
void attach_item(T data[], int& n, const T& entry) {
    data[n++] = entry;
}    

template <typename T>
//remove the last element in data and place it into 'entry'
//done and tested.
void detach_item(T data[], int& n, T& entry) {
    assert(n > 0);
    entry = data[n - 1];
    n--;
}

template <typename T>
//append data2 to the right of data1
void merge(T src[], const int& src_size, T dest[], int& dest_size) {
    for (int j = 0; j < src_size; j++) {
        attach_item(dest, dest_size, src[j]);
    } 
}

template <typename T>
//move n/2 elements from the right of data1
//and move to data2
void split_vals(T src[], int& src_size, T dest[], int& dest_size) {
    int howManyToPluck = src_size / 2;
    int j = 0;
    for (int i = src_size-howManyToPluck; i < src_size; i++,j++) {
        dest[j] = src[i];
    }
    dest_size = howManyToPluck;
    src_size -= howManyToPluck;
}

template<typename T>
void copy_array(const T src[], T dest[], const int src_size, int& dest_size) {
    for (int i = 0; i < src_size; i++) {
        dest[i] = src[i];
    }
    dest_size = src_size;
}

template<typename T>
void print_array(const T data[], int n, int pos) {
    for (int i = pos; i < n; i++) {
        std::cout << data[i] << " ";
    }
}

template <class T>
//return the larger of the two items
T maximal(const T& a, const T& b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

template <class T>
//swap the two items
void swap_val(T& a, T& b) {
    T temp = b;
    b = a;
    a = temp;
}

template <class T>
//return index of the largest item in data
int index_of_maximal(T data[], int n) {
    int maxIndex = 0;
    for (int i = 0; i < n; i++)
        if (data[i] > data[maxIndex])
            maxIndex = i;
    return maxIndex;

}

template <class T>
//insert entry into the sorted array
//data with length n
void ordered_insert(T data[], int& n, T entry) {
    int i = 0;
    for (i; i < n; i++) {
        if (data[i] >= entry) {
            insert_item(data, i, n, entry);
            return;
        }
    }
    insert_item(data, i, n, entry);
}
template <class T>
//item > all data[i]
bool is_gt(const T data[], int n, const T& item) {
    for (int i = 0; i < n; i++) {
        if (item < data[i] || item == data[i]) {
            return false;
        }
    }
    return true;
}

template <class T>
//item <= all data[i]
bool is_le(const T data[], int n, const T& item) {
    for (int i = 0; i < n; i++) {
        if (item > data[i]) {
            return false;
        }
    }
    return true;
}

// print vector list
template <typename T>
std::ostream& operator<<(std::ostream& outs, const std::vector<T>& list) {
    for (size_t i = 0; i < list.size(); ++i) {
        outs << list[i];
        if (i < list.size() - 1) {
            outs << ',';
        }
    }
    return outs;
}

// list.push_back item
template <typename T, typename U>
std::vector<T>& operator+=(std::vector<T>& list, const U& item) {
    list.push_back(item);
    return list;
}