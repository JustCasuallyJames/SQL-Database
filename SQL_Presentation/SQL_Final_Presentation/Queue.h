/*
 * Author: James Vo
 * Project: Stack and Queue
 * Purpose: To learn more about how data structures are used in the real world
 * Notes: Iterators were implemented.
*/
#ifndef STACKANDQUEUES_QUEUE_H
#define STACKANDQUEUES_QUEUE_H
#include "node.h"
template <class T>
class Queue{
public:
    class Iterator{
    public:
        friend class Queue;

        Iterator(node<T>* p = nullptr) : _ptr(p) {}     //constructor

        Iterator next(){ //returns the iterator to the next location in the list
            assert(_ptr != nullptr && "Queue next(): Cannot find the next ptr on a nullptr");
            return Iterator(_ptr->_next);
        }
        //returns the previous value of the ptr
        Iterator prev(){
            assert(_ptr!=nullptr);
            return Iterator(_ptr->_prev);
        }
        const T &operator*() const {        //dereference operator
            assert(_ptr && "* operator: cannot dereference a nullptr");
            return _ptr->_item;
        }
        operator bool(){
            return _ptr != nullptr;
        }
        const T* operator ->()const{
            assert(_ptr && "-> operator: cannot this a nullptr");
            return &_ptr->_item; //returns the reference of the item within the node
        }
        friend Iterator operator++(Iterator &it, int unused) {//it++
            Iterator hold(it);
            it = it._ptr->_next;
            return hold;
        }
        Iterator &operator++() {//++it
            _ptr = _ptr->_next;
            return *this;
        }

        friend bool operator!=(const Iterator &left, const Iterator &right) {//it != it
            return left._ptr != right._ptr;
        }
        friend bool operator==(const Iterator &left, const Iterator &right) {//it != it
            return left._ptr == right._ptr;
        }
    private:
        node<T> *_ptr;//pointer being encapsulated
    };
    Queue():_front(nullptr), _tail(nullptr),_size(0){}

    //the big 3
    ~Queue();
    Queue(const Queue<T> &other);
    Queue<T> &operator=(const Queue& rhs);

    void push(const T& item);
    bool empty();
    T pop();
    T top();
    int getSize() const;
    typename Queue<T>::Iterator begin() const;
    typename Queue<T>::Iterator end() const;


    friend ostream& operator<<(ostream& outs, Queue& q){
        return _print_list(q._front,outs);
    }

private:
    node<T>* _front;
    node<T>* _tail;
    int _size;
};
//destructor
template<typename T>
Queue<T>::~Queue(){
    delete_all(_front);
}
//copy constructor
template <typename T>
Queue<T>::Queue(const Queue<T> &other){
    init_head(_front);
    _tail = copy_list(_front,other._front);
    _size = other._size;
}
//assignment constructor
template <typename T>
Queue<T> &Queue<T>::operator=(const Queue& rhs){
    //check for self-reference
    if(this == &rhs){
        return *this;
    }
    //call function to clear this object.
    delete_all(_front);
    //call function to copy rhs into this object
    _tail = copy_list(_front, rhs._front);
    _size = rhs._size;
    return *this;
}
//pushes an item at the tail
template <typename T>
void Queue<T>::push(const T& item){
    _tail = insert_after(_front,_tail,item);
    _size++;
}
//checks if the size of the queue is empty
template <typename T>
bool Queue<T>::empty(){
    return _size == 0;
}
//pops from the head
template <typename T>
T Queue<T>::pop(){
    assert(!empty() && "queue: empty object cannot be popped.");
    _size--;
    return delete_head(_front);
}
//returns the item at the front
template <typename T>
T Queue<T>::top(){
    assert(!empty() && "queue: top of empty object does not exist.");
    //grab the first item and return it.
    return _front->_item;
}
//returns an Iterator of the head of the queue
template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const{
    return Iterator(_front);
}
//returns the end of a queue, a nullptr
template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const{
    return Iterator(nullptr);
}
//returns the size of the queue
template <typename T>
int Queue<T>::getSize() const{
    return _size;
}

//-------------------------------
//         Test Functions
//-------------------------------
//void test_queue_push(){
//    cout<<"queue_push: pushing 0-9 into queue here: "<<endl;
//    Queue<int> q;
//    for (int i= 0; i<10; i++){
//        q.push(i);
//    }
//    cout<<q<<endl;
//}
//void test_queue_pop_empty(){
//    cout<<"queue_pop: popping emtpy queue"<<endl;
//    Queue<int> q;
//    q.pop();
//}
//void test_queue_top_empty(){
//    cout<<"queue: top of an empty queue."<<endl;
//    Queue<int> q;
//    cout<<q.top()<<endl;;
//}
//void test_big_three_queue(){
//    cout <<"Queue-big three function: " << endl;
//    Queue<int> q;
//    for (int i= 0; i<10; i++){
//        q.push(i);
//    }
//    cout << "big three function: testing the copy constructor " << endl;
//    Queue<int> qCopy(q);
//    cout << "Original: "<< q << endl;
//    cout << "Copy: " << qCopy << endl;
//
//    cout << "Pushing 12345 into the q Queue.....\n"
//            "and expecting for qCopy to have 12345 at the rear" << endl;
//    q.push(12345);
//    qCopy = q;
//    cout << "Original: "<< q << endl;
//    cout << "Copy: " << qCopy << endl;
//}
//void test_begin_and_end(){
//    cout <<"\nQueue: Testing the begin and end: " << endl;
//    Queue<int> q;
//    for (int i= 0; i<10; i++){
//        q.push(i);
//    }
//    cout << "First testing if it can print in order from the beginning. " << endl;
//    Queue<int>::Iterator it=q.begin();
//    Queue<int>::Iterator lastNode;
//    for(; it!= q.end(); it=it.next()){
//        if(it.next() == q.end()){
//            lastNode = it;
//        }
//        cout << *it << " ";
//    }
//    cout << "\nThen testing if it can print backwards. " << endl;
//    for(; lastNode!= q.end(); lastNode = lastNode.prev()){
//        cout << *lastNode<< " ";
//    }
//}
//
//void test_all_queue(){
//    cout << "Testing all of the Queue functions. " << endl;
//    Queue<int> q;
//    for (int i = 0; i < 10; i++) {
//        q.push(i);
//    }
//    Queue<int> qCopy(q);
//    cout << "q: " << q << endl;
//    cout << "qCopy: " << qCopy << endl << endl;
//    while(!qCopy.empty()){
//        cout << "qCopy: { " << qCopy.pop() << " } ";
//        cout << qCopy << endl;
//    }
//    cout << "Assigning q back to qCopy:\n";
//    qCopy = q;
//    cout << "q: " << q << endl;
//    cout << "qCopy: " << qCopy << endl;
//    cout << "<-----------------------END OF QUEUE FUNCTIONS--------------------->" << endl;
//}
#endif //STACKANDQUEUES_QUEUE_H
