/*
 * Author: James Vo
 * Project: Stack and Queue
 * Purpose: To learn more about how data structures are used in the real world
 * Notes: Iterators were implemented.
*/
#ifndef STACKANDQUEUES_NODE_H
#define STACKANDQUEUES_NODE_H
#include <iostream>
#include <assert.h>
using namespace std;
template <typename T>
struct node{
    T _item;
    node<T>* _next;
    node<T>* _prev;
    //This sets item to the default constructor of the T type and sets next's default to nullptr
    //would probably be more useful to put _prev into the parameters
    node(const T& item=T(),node<T>* next=nullptr, node<T>* prev=nullptr):_item(item), _next(next), _prev(prev){}
    friend ostream& operator <<(ostream& outs, const node<T>& n){
        outs<<"["<<n._item<<"]->";
        return outs;
    }
};
//--------------------------------------------------------------------
//      declarations
//--------------------------------------------------------------------
//inserts an item at the head of the linked list
template<typename T>
node<T>* _insert_head(node<T>*& head, const T& item);

//initializes head to null
template <class T>
node<T>* init_head(node<T>* &head);

//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*&head);

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head);

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(node<T>* & dest, const node<T>* src);

//insert_after: if after is NULL, inserts at head
template <class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item);

//delete the node at the head of the list, reuturn the item:
template <class T>
T delete_head(node<T>* &head);

//prints out the linked list
template <class T>
ostream& _print_list(node<T>* head, ostream& outs=cout);
template<typename T>
ostream& _print_backwards_list(node<T>* head, ostream& outs=cout);

//--------------------------------------------------------------------
//  definitions:
//--------------------------------------------------------------------
/*
 * Done and Tested.
 */
template<typename T>
node<T>* _insert_head(node<T>*& head, const T& item){
    if(head == nullptr){
        head = new node<T>(item);
        return head;
    }else{
        //1. make a new node:
        node<T>* temp = new node<T>(item);
        //2. point this node to where head is pointing:
        temp->_next = head;
        //3. point the previous head to temp as prev
        head->_prev = temp;
        //4. point head to this new node:
        head = temp;

        return head;
    }
}
/*
 * Done and tested.
 */
template<typename T>
ostream& _print_list(node<T>* head, ostream& outs){
    //h->>[ ]->[ ]->[ ]->|||
    outs<<"H->";
    for (node<T>* walker = head; walker != nullptr; walker = walker->_next){
        outs<<*walker;
    }
    outs<<"|||";
    return outs;
}
template<typename T>
ostream& _print_backwards_list(node<T>* head, ostream& outs){

    node<T>* walker = head;
    //this may not set the walker to the last node, one node before the last node.
    while(walker->_next != nullptr){
        walker = walker->_next;
    }
    outs<<"H->";
    for (; walker != nullptr; walker = walker->_prev){
        outs<<*walker;
    }
    outs<<"|||";
    return outs;
}
//initializes head to null
/*
 * Done and tested.
 */
template <class T>
node<T>* init_head(node<T>* &head){
    head = nullptr;
    return head;
}

//deletes all the nodes in the list
/*
 * Done and Tested.
 */
template<class T>
void delete_all(node<T>*&head){
    //as long as the head isn't a nullptr
    while(head != nullptr){
        delete_head(head);
    }
}

//true if head is NULL, false otherwise.
/*
 * Done and tested.
 */
template <class T>
bool empty(const node<T>* head){
    return head == nullptr;
}

//makes a copy of the list, returns a pointer to the last node:
/*
 * Done and Tested.
 */

template <class T>
node<T>* copy_list(node<T>* & dest, const node<T>* src){
    node<T>* copyWalker = nullptr;
    const node<T>* headWalker = src;
    //to clear the list if theres things in there
    delete_all(dest);
    if(src != nullptr){
        //iterate until you're at the end of the head node<T>*
        while(headWalker != nullptr){
            //insert after the copyWalker and modifying the head everytime
            copyWalker = insert_after(dest, copyWalker, headWalker->_item);
            //increment both the headwalker
            headWalker = headWalker->_next;
        }
    }
    //if src == nullptr then it'll return copyWalker which is a nullptr
    //if src != nullptr then it'll return copyWalker which is at the end of the list
    return copyWalker;
}

//insert_after: if after is NULL, inserts at head
/*
 * Done and tested.
 */
template <class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item){
    if(head == nullptr){
        return _insert_head(head, item);
    }else{
        node<T>* temp = new node<T>(item, after->_next, after);
        after->_next = temp;
        return temp;
    }
}

//delete the node at the head of the list, reuturn the item:
/*
 * Done and Tested.
 */
template <class T>
T delete_head(node<T>* &head){
    assert(head!=nullptr && "list: attempted to delete an null node.");
    node<T>* temp = head->_next; //saves the headptr in a temp value
    T item = head->_item; //gives the item of the initial headptr
    delete head; //deleting this node
    head = temp;
    if(head!=nullptr){
        head->_prev = nullptr;
    }
    return item;
}
//--------------------------------------------------------------------
//  test functions:
//--------------------------------------------------------------------

//void test_copy_list(){
//    node<int>* head;
//    init_head(head);
//    for (int i=0; i<10; i+=2){
//        _insert_head(head, i);
//    }
//    cout << "testing copy_list: " << endl;
//    node<int>* newHead;
//    init_head(newHead);
//    _print_list(head);
//    cout << endl;
//    copy_list(newHead, head);
//    _print_list(newHead);
//    cout << endl;
//}
//void test_insert_after_and_init_head(){
//    node<int>* head;
//    init_head(head);
//    for (int i=0; i<10; i++){
//        _insert_head(head, i);
//    }
//    insert_after(head,head,12345);
//    cout << "\ntesting insert_after: Inserting after the head " << endl;
//    _print_list(head);
//    cout << endl;
//}
//void test_delete_all_and_empty(){
//    node<int>* head = nullptr;
//    for (int i=0; i<10; i++){
//        _insert_head(head, i);
//    }
//    cout << "testing delete_all: deleting all of the values from the linked list.\n";
//    cout << "Original: ";
//    _print_list(head);
//    delete_all(head);
//    cout << "\nAfter deleting: ";
//    _print_list(head);
//    cout << endl;
//    cout << "testing empty: should return true (1) if head is a nullptr\n";
//    cout << empty(head);
//}
//void test_delete_head(){
//
//    node<int>* head = nullptr;
//    for (int i=0; i<10; i++){
//        _insert_head(head, i);
//    }
//    cout << "testing delete_head: deleting 9 and 8 from the linked list\n";
//    delete_head(head);
//    delete_head(head);
//    _print_list(head);
//    cout << endl;
//}
//void test_delete_head_empty(){
//    cout << "Testing if it'll assert when the head is a nullptr\n";
//    node<int>* head = nullptr;
//    delete_head(head);
//}
//void test_node(){
//    cout<<"testing node: printing three nodes: "<<endl;
//    node<int> n1;           //0, nullptr
//    node<int> n2(5);        //5, nullptr
//    node<int> n3(6, &n1);   //6, address of n1
//    cout<<n1<<n2<<n3<<endl;
//}
//void test_insert_head(){
//    cout<<"testing insert_head: inserting 0..9 at head and printing: "<<endl;
//    node<int>* head = nullptr;
//    for (int i=0; i<10; i++){
//        _insert_head(head, i);
//    }
//    _print_list(head);
//    cout<<endl;
//}
//void test_backwards_list(){
//    cout << "testing backwards list: "<< endl;
//    node<int>* head;
//    init_head(head);
//    for (int i=0; i<10; i++){
//        _insert_head(head, i);
//    }
//    cout << "Original: ";
//    _print_list(head);
//    cout << "\nBackwards: ";
//    _print_backwards_list(head);
//
//}

#endif //STACKANDQUEUES_NODE_H
