/*
 * Author: James Vo
 * Project: Stack and Queue
 * Purpose: To learn more about how data structures are used in the real world
 * Notes: Iterators were implemented.
*/
#ifndef STACKANDQUEUES_STACK_H
#define STACKANDQUEUES_STACK_H
#include "node.h"

template <typename T>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;

        Iterator(node<T>* p = nullptr) : _ptr(p) {}     //constructor

        Iterator next(){ //returns the iterator to the next location in the list
            assert(_ptr != nullptr);
            return Iterator(_ptr->_next);
        }
        const T &operator*() const {        //dereference operator
            assert(_ptr);
            return _ptr->_item;
        }
        operator bool(){
            return _ptr != nullptr;
        }
        const T* operator ->()const{
            assert(_ptr);
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
    Stack();
    //big three functions:
    Stack(const Stack& other);
    Stack& operator=(const Stack& rhs);
    ~Stack();

    void push(const T& item);
    T pop();
    bool empty();
    T top();
    typename Stack<T>::Iterator begin() const;
    friend ostream& operator <<(ostream& outs, const Stack& s){
        _print_list(s._top, outs);
        return outs;
    }
private:
    node<T>* _top;
};
//constructor
template <typename T>
Stack<T>::Stack(){
    init_head(_top);
}
//big three functions:
template <typename T>
Stack<T>::Stack(const Stack& other){
    //call a function to copy other into this object.
    init_head(_top);
    copy_list(_top,other._top);
}
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& rhs){
    //check for self-reference
    if(this == &rhs){
        return *this;
    }
    //call function to clear this object.
    delete_all(_top);
    //call function to copy rhs into this object
    copy_list(_top,rhs._top);
    return *this;
}

template <typename T>
Stack<T>::~Stack(){
    //call function to clear this object.
    delete_all(_top);
}
//pushes an item to the top
template <typename T>
void Stack<T>::push(const T& item){
    _insert_head(_top, item);
}
//pops an item from the top and returns that item
template <typename T>
T Stack<T>::pop(){
    assert(!empty() && "stack: empty object cannot be popped.");
    //call function to remove node at head (_top) and return the item.
    return delete_head(_top);
}
//checks if the stack is empty
template <typename T>
bool Stack<T>::empty(){
    return _top == nullptr;
}
//returns the item at the top
template <typename T>
T Stack<T>::top(){
    assert(!empty() && "stack: top of empty object does not exist.");
    //grab the first item and return it.
    return _top->_item;
}
//returns an Iterator to the top of the Stack
template <typename T>
typename Stack<T>::Iterator Stack<T>::begin() const{
    return Iterator(_top);
}

//------------------------------------------------------------------
//  test functions:
//------------------------------------------------------------------

//void test_stack_push(){
//    cout<<"stack: pushing 0..9 into stack here: "<<endl;
//    Stack<int> s;
//    for (int i= 0; i<10; i++){
//        s.push(i);
//    }
//    cout<<s<<endl;
//}
//void test_stack_pop_empty(){
//    cout<<"stack: popping emtpy stack"<<endl;
//    Stack<int> s;
//    s.pop();
//}
//void test_stack_top_empty(){
//    cout<<"stack: top of an empty stack."<<endl;
//    Stack<int> s;
//    cout<<s.top()<<endl;;
//}
//void test_big_three_stack(){
//    cout <<"stack-big three function: " << endl;
//    Stack<int> s;
//    for (int i= 0; i<10; i++){
//        s.push(i);
//    }
//    cout << "big three function: testing the copy constructor " << endl;
//    Stack<int> sCopy(s);
//    cout << "Original: "<< s << endl;
//    cout << "Copy: " << sCopy << endl;
//
//    cout << "Pushing 12345 into the s Stack.....\n"
//            "and expecting for sCopy to have 12345 at the top" << endl;
//    s.push(12345);
//    sCopy = s;
//    cout << "Original: "<< s << endl;
//    cout << "Copy: " << sCopy << endl;
//}
//void test_begin(){
//    cout << "Stack: Testing Begin function: " << endl;
//    Stack<int> s;
//    for (int i= 0; i<10; i++){
//        s.push(i);
//    }
//    for(Stack<int>::Iterator it=s.begin(); it!= nullptr; it = it.next()){
//        cout << *it << " ";
//    }
//}
//void test_all_stack(){
//    cout << "Testing all of the Stack functions. " << endl;
//    Stack<int> s;
//    for (int i = 0; i < 10; i++) {
//        s.push(i);
//    }
//    Stack<int> sCopy(s);
//    cout << "s: " << s << endl;
//    cout << "sCopy: " << sCopy << endl << endl;
//    while(!sCopy.empty()){
//        cout << "sCopy: { " << sCopy.pop() << " } ";
//        cout << sCopy << endl;
//    }
//    cout << "Assigning s back to sCopy:\n";
//    sCopy = s;
//    cout << "s: " << s << endl;
//    cout << "sCopy: " << sCopy << endl;
//    cout << "<-----------------------END OF STACK FUNCTIONS--------------------->\n\n";
//}
#endif //STACKANDQUEUES_STACK_H
