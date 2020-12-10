#pragma once
#include <iostream>
#include <iomanip>

#include "BTreeFunctions.h"

using namespace std;
template <typename T>
class BPlusTree {
public:
    
    class Iterator {
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it = NULL, int _key_ptr = 0) :it(_it), key_ptr(_key_ptr) {}

        T operator *() {
            assert(key_ptr < it->data_count);
            return it->data[key_ptr];
        }

        const T* operator ->() {
            return &it->data[key_ptr];
            
        }

        Iterator operator++(int un_used) { //this is the postfix operator, this gets the previous value and not the recently updated value
            Iterator temp = *this;
            if (key_ptr + 1 < it->data_count) { //so that we know we won't go out of bounds
                key_ptr++;
            } else { //that means we're essentially one off from the end, data_count, so we set the key_ptr to 0 and move onto the next
                    //node
                key_ptr = 0;
                it = it->next;
            }
            return temp; //return the of the previous state
        }

        Iterator operator++() { //this is the prefix operator, so it directly accesses the variable you're modifying
            if (key_ptr + 1 < it->data_count) { //so that we know we won't go out of bounds
                key_ptr++;
            } else { //that means we're essentially one off from the end, data_count, so we set the key_ptr to 0 and move onto the next
                    //node
                key_ptr = 0;
                it = it->next;
            }
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            if (rhs.it == NULL) {
                return lhs.it != NULL;
            } else {
                return lhs.it != rhs.it || lhs.key_ptr != rhs.key_ptr;
            }
        }
        void print_Iterator() {
            cout << it->data[key_ptr] << "->";
        }
        bool is_null() { return !it; }
    private:
        const BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false);
    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    BPlusTree<T>* copy_tree_leaves(BPlusTree<T>* previous = nullptr);  //copy other into this object

    bool contains(const T& entry) const;        //true if entry can be found in the array

    T& get(const T& entry);                     //return a reference to entry in the tree
    const T& get(const T& entry)const;          //return a reference to entry in the tree
    T& get_existing(const T& entry);            //return a reference to entry in the tree

    Iterator find(const T& entry);              //return an iterator to this key. NULL if not there.


    Iterator lower_bound(const T& key);  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist >= entry
    Iterator upper_bound(const T& key);  //return first that goes AFTER key

    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty

    void print_tree(int level = 0, ostream& outs = cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me) {
        print_me.print_tree(0, outs);
        return outs;
    }
    bool is_valid();

    Iterator begin();
    Iterator end();


private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM + 1];                //holds the keys
    int child_count;                    //number of children
    BPlusTree* subset[MAXIMUM + 2];     //subtrees
    BPlusTree* next;
    bool is_leaf() const {return child_count == 0;}        //true if this is a leaf node

    T* find_ptr(const T& entry);        //return a pointer to this key. NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);  //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);             //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage of data elements in child i
                                        // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    BPlusTree<T>* get_biggest_node();
    void get_smallest(T& entry);        //entry := leftmost leaf
    void get_biggest(T& entry);         //entry := rightmost leaf
    void remove_biggest(T& entry);      //remove the biggest child of this tree->entry
    void transfer_left(int i);          //transfer one element LEFT from child i
    void transfer_right(int i);         //transfer one element RIGHT from child i
    void rotate_right(int i);           //rotate right
    void rotate_left(int i);            //rotate left
    BPlusTree<T>* merge_with_next_subset(int i);    //merge subset i with subset i+1

    void clear_node(); //clears a node, so it makes the entire data = T() and the subsets equal to nullptrs


};

template<typename T>
//initializes a brand new node with no data, no children, and the ability to have duplicates or not.
BPlusTree<T>::BPlusTree(bool dups):data_count(0),child_count(0),dups_ok(dups) {}

template<typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other) {
    copy_tree(other);
}

template<typename T>
BPlusTree<T>::~BPlusTree() {
    clear_tree();
}

template<typename T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree<T>& RHS) {
    if (this == &RHS) {
        return *this;
    }
    clear_tree();
    copy_tree(RHS);

    return *this;
}



template<typename T>
//this utilizes the clear_node function and clears the entire tree by deleting the subset[i] and setting it to nullptrs
void BPlusTree<T>::clear_tree() {
    for (int i = 0; i < child_count; i++) { //This is our base case, if theres no more children, then you can just set the data count to 0
        if (subset[i] != nullptr) {
            subset[i]->clear_tree(); //clear the tree at that children's subset[i]
            delete subset[i]; //
            subset[i] = nullptr;
        }
    }
    //data_count = 0;
    //child_count = 0;
    //for (int i = child_count; i < MAXIMUM + 2; i++) {
    //    subset[i] = nullptr;
    //}
    clear_node(); //What this will do is esssentially set data_count = 0 and child_count = 0, and it will
                    //set all values within the node to be it's type default values.
}

template<typename T>
//essentially clears an entire BPlusTree node so that the subset's point to nullptrs and there is no data.
void BPlusTree<T>::clear_node() {
    data_count = 0;
    next = nullptr;
    child_count = 0;
    for (int i = 0; i < (MAXIMUM + 2); i++) {//go from the begining to the subset capacity, max+2
        if (i < (MAXIMUM + 1)) {//needed because it may go out of bounds since subset is bigger
            /*data_count = 0;*/
            data[i] = T(); //set it to the default value of the type
        }
        subset[i] = nullptr; //valid b/c it's relative to the subset capacity so it sets each child to a nullptr
    }
}

template<typename T>
bool BPlusTree<T>::is_valid() {
    if (is_leaf()) //check if we're at the leaves
        return true;

    for (int i = 0; i < data_count - 1; i++) {
        if (!(data[i] < data[i + 1])) {//check if the i+1 is greater than i
            return false;
        }
            
    }

    //========== check if data is smaller than right most subset =========

    //Checks if the right most data is less than the right most child data
    if (child_count > 0) {
        //Goes through the data of the right most child
        //go through right most child data count
        for (int i = 0; i < subset[child_count - 1]->data_count; i++) {
            //check if the right most data is less than right most node
            if (!(data[data_count - 1] <= subset[child_count - 1]->data[i]))
                return false;
        }

    }

    //========= checks if data is bigger than anything before right most =
    if (child_count > 0) {
        //run through the current data count
        for (int k = 0; k < data_count; k++) {
            if (!(data[k] >= subset[k]->data[0])) //making sure if data[k] is bigger or equal to it's children
                return false;
        }
    }

    //Check if data[i] is equal to subset[i + 1] smallest
    if (child_count > 0) {
        //Runs through the current data's
        for (int i = 0; i < data_count; i++) {
            //Incase the subset[i + 1] is already the smallest
            //ex.
            /*
             *      10
             *     /  \
             *    5   10 15
            */
            if (subset[i + 1]->is_leaf()) {
                if (!(data[i] == subset[i + 1]->data[0]))
                    return false;
            }
            //goes to subset i + 1 and finds the smallest node
            else {
                if (!(data[i] == subset[i + 1]->get_smallest_node()->data[0]))
                    return false;
            }
        }
    }

    //Goes down all the way till there is no child left
    //Then on the recursive call back it starts deleting the pointers
    for (int i = child_count - 1; i > -1; i--) {
        if (!subset[i]->is_valid())
            return false;
    }
}

template<typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other) {
    //it's similar to BTree but now we gotta connect the links
    copy_array(other.data, data, other.data_count, data_count); //set the other data into the current data.
    dups_ok = other.dups_ok; //set the values of the other dups_ok to be this current dups_ok
    child_count = other.child_count; //set the values of the other child count to be this current child count
    for (int i = 0; i < other.child_count; i++) {
        if (other.subset[i] != nullptr) { //our base case to signify that we're at the bottom of the tree
            subset[i] = new BPlusTree<T>(other.dups_ok); //we set the current index of subset[i] to be an entire new BTree
            subset[i]->copy_tree(*(other.subset[i]));//recurse and copy the data from other.subset[i] into subset[i]
                                                    //CRUCIAL that we dereference it because it's a BTree* not a regular old BTree
        }
    }
    copy_tree_leaves();
}

template<typename T>
BPlusTree<T>* BPlusTree<T>::copy_tree_leaves(BPlusTree<T>* previous) {
    if (is_leaf()) {
        return nullptr;
    }
    //if the child has children, and it's a leaf, then we have to link it
    if (child_count > 0 && subset[0]->is_leaf()) {
        if (previous != nullptr) {
            previous->next = subset[0];
            // the previous->next is the left most node as previous starts at nullptr (since the node previous
            //to the left most node is nullptr)
        }
        for (int i = 0; i < child_count - 1; i++) {
            subset[i]->next = subset[i + 1];
            //make subset[i]'s next to be it's neighbor to right
        }
        return subset[child_count - 1]; //return the far most right ptr
    } else {//recurse down the tree
        BPlusTree<T>* last_link = previous;
        for (int i = 0; i < (child_count - 1); i++) {
            BPlusTree<T>* left = subset[i]; //just to differentiate what's left and right node
            BPlusTree<T>* right = subset[i + 1];
            last_link = left->copy_tree_leaves(last_link); //recurse on the last_link being your previous
            //whatever is returned (the subset[child_count -1]) will be saved into last link
            //so that copy_tree_leaves can link the left and right node together
            BPlusTree<T>* right_link = right->copy_tree_leaves(last_link);
            last_link = right_link; //the last link will be the last link in that iteration (end of iteration will be the end of the leaves)
        }
        return last_link;
    }
}

template<typename T>
bool BPlusTree<T>::contains(const T& entry) const {
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count&& data[i] == entry);
    
    if (found) { //if we found the entry
        if (is_leaf()) { //check if it's a leaf so that we know that the tree contains it
            return true;
        } else {
            subset[i + 1]->contains(entry); //check the right side of the node
        }
    } else {
        if (is_leaf()) { //check if it's a leaf, if it is, that means it's not in here, return false
            return false;
        } else { //check the left side of the node
            subset[i]->contains(entry);
        }
    }
}

template<typename T>
T& BPlusTree<T>::get(const T& entry) {
    bool found = find(entry) == Iterator(nullptr);
    if (found) { //If we can't find it, insert it
        insert(entry);
    }
    return get_existing(entry); //found then we find the get_existing
}

template<typename T>
const T& BPlusTree<T>::get(const T& entry) const {
    //WE WILL NEVER INSERT IN HERE, NEVER IN A MILLION YEARS SINCE IT'S CONST
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count&& data[i] == entry); //if we found it or not

    if (found) { //we've found it
        if (is_leaf()) { //if it's a leaf, then it returns a the leaves value
            return data[i];
        } else {
            subset[i + 1]->get(entry); //go down the right tree
        }
    } else {
        if (!is_leaf()) { //if it ISN'T a leaf, then we go down the left side
            subset[i]->get(entry);
        }
    }
}

template<typename T>
T& BPlusTree<T>::get_existing(const T& entry) {
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count&& data[i] == entry); //if we found it or not

    if (found) { //we've found it
        if (is_leaf()) { //if it's a leaf, then it returns a the leaves value
            return data[i];
        } else {
            subset[i + 1]->get(entry); //go down the right tree
        }
    } else {
        if (!is_leaf()) { //if it ISN'T a leaf, then we go down the left side
            subset[i]->get(entry);
        }
    }
}

template<typename T>
void BPlusTree<T>::insert(const T& entry) {
    loose_insert(entry);

    if (data_count > MAXIMUM) { //if the parent is in excess, then we have to grow the tree.
        //create a new tree
        BPlusTree<T>* newTree = new BPlusTree<T>(dups_ok);
        //copy all the contents of the data into the new tree data, and copy its children over to the newtree
        copy_array(data, newTree->data, data_count, newTree->data_count);
        copy_array(subset, newTree->subset, child_count, newTree->child_count);

        //this clears the tree
        clear_node(); //clears the first node to make the other values garbage 

        //now point the subset[0] to the newTree
        subset[0] = newTree;
        child_count = 1;

        fix_excess(0); //since it's heavy, you're repairing the root
    }
}

template<typename T>
void BPlusTree<T>::remove(const T& entry) {
    loose_remove(entry);
    if (data_count < MINIMUM && child_count > 0) {
        //shrink tree
        BPlusTree<T>* temp = subset[0];
        subset[0] = nullptr;
        copy_array(temp->data, data, temp->data_count, data_count);
        copy_array(temp->subset, subset, temp->child_count, child_count);

        child_count = temp->child_count;
        temp->child_count = 0;
        delete temp;
    }
}

template<typename T>
T* BPlusTree<T>::find_ptr(const T& entry) {
    return NULL;
}

template<typename T>
void BPlusTree<T>::loose_insert(const T& entry) {
    /*
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       three cases:
         found
         a. found/leaf: deal with duplicates: call +
         b. found/no leaf: subset[i+1]->loose_insert(entry)
                           fix_excess(i+1) if there is a need

         ! found:
         c. !found / leaf : insert entry in data at position i
         c. !found / !leaf: subset[i]->loose_insert(entry)
                            fix_excess(i) if there is a need

            |   found          |   !found         |
      ------|------------------|------------------|-------
      leaf  |a. dups? +        | c: insert entry  |
            |                  |    at data[i]    |
      ------|------------------|------------------|-------
            | b.               | d.               |
            |subset[i+1]->     | subset[i]->      |
      !leaf | loose_insert(i+1)|  loose_insert(i) |
            |fix_excess(i+1)   | fix_excess(i)    |
            |                  |                  |
      ------|------------------|------------------|-------
    */
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry); //if we found it then we go onto three cases

    if (found) {
        if (is_leaf()) { //if it is a leaf
            //deal with duplicates
            data[i] = (data[i] + entry); //The overloaded + operator will handle whether it appends or replaces it
                
        } else { //if it isn't a leaf
            subset[i+1]->loose_insert(entry); //go down right side to find out where to insert
                                              
            if (subset[i+1]->data_count > MAXIMUM) { //that means child in excess, fat boy
                fix_excess(i+1);
            }
        }
    } else {
        if (is_leaf()) { // if it's a leaf
            //just insert at i
            insert_item(data, i, data_count, entry);
            return;
        } else { //if it isn't a leaf, keep going
            subset[i]->loose_insert(entry); //go down left side
            if (subset[i]->data_count > MAXIMUM) { //that means child in excess, fat boy
                fix_excess(i);
            }
        }
    }
}

template<typename T>
void BPlusTree<T>::fix_excess(int i) {
    
    //create two BPlusTree pointer nodes, one indicating the newNode you want to split into
    //and another one to indicate the subset[i]
    BPlusTree<T>* newNode = new BPlusTree<T>(dups_ok);
    BPlusTree<T>* currentNode = subset[i]; //makes it easier for me to think about

    //split the values
    //we want to split the currentNode and save the other half into the newNode
    //save the data and its subset
    split_vals(currentNode->data, currentNode->data_count, newNode->data, newNode->data_count);
    split_vals(currentNode->subset, currentNode->child_count, newNode->subset, newNode->child_count);

    //now we want to detach the item from the currentNode to place it with its parent
    T detachedItem;
    detach_item(currentNode->data,currentNode->data_count, detachedItem);
    //do an ordered insert (I did an ordered insert just to be safe, but insert_item will work as well)
    ordered_insert(data, data_count, detachedItem); 

    //insert the newNode into the tree
    insert_item(subset, i + 1, child_count, newNode);

    //now we have to deal with the leaves and how it traces back to the breadcrumbs (inner nodes)
    //do an insert at the beginning, i=0, for the currentNode and the newNode
    if (newNode->is_leaf()) { //we only want to insert the item if it's a leaf, not if it's a bread crumb
        insert_item(newNode->data, 0, newNode->data_count, detachedItem);

        //now we have to link the leaves together
        //so we make the newNodes->next to be the subset[i]->next since we're trying to insert it.
        newNode->next = currentNode->next;
        currentNode->next = newNode;
        //^^^^^This is the process to insert it, we make currentNodes next to be the newNode and the newNode to be currentNodes previous next.
    }
}

template<typename T>
void BPlusTree<T>::loose_remove(const T& entry) {
    /* four cases:
           leaves:
                a. not found: there is nothing to do
                b. found    : just remove the target
           non leaf:
                c. not found: subset[i]->loose_remove, fix_shortage(i)
                d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                    (No More remove_biggest)

             |   !found               |   found                 |
       ------|------------------------|-------------------------|-------
       leaf  |  a: nothing            | b: delete               |
             |     to do              |    target               |
       ------|------------------------|-------------------------|-------
       !leaf | c:                     | d: B_PLUS_TREE          |
             |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
             |  fix_shortage(i)       | fix_shortage(i+1) [...] |
       ------|------------------------|-------------------------|-------


     */
    /*
        1st recurisve i = 1
        2nd recursive i = 0
        3rd recursive i =
    */
    int i = first_ge(data, data_count, entry);
    bool found = i < data_count&& data[i] == entry;
    if (!is_leaf()) { //needs to check if it's not a leaf, so we don't have to keep calling recursively | !is_leaf()
        if (found) { //!is_leaf() && found
            assert(i < child_count - 1);

            subset[i + 1]->loose_remove(entry);
            
            subset[i + 1]->get_smallest(data[i]); //this means the thing you wanted to get the smallest of didn't move
            //i = first_ge(data, data_count, entry);
            
            if (child_count > 1) {
                if (subset[i + 1]->data_count < MINIMUM) {
                    BPlusTree<T>* temp_ptr = fix_shortage(i + 1);
                    for (int j = 0; j < data_count; j++) {
                        if (data[j] == entry) { //if it's within your data, that means no rotations have happened
                            subset[i + 1]->get_smallest(data[j]);
                        }
                    }

                    //well now after the for loop where we check into data, let's say it isn't in data[] 
                    //possible errors within merge/rotate due to this method (hopefully it's fixed) JT
                    for (int j = 0; j < temp_ptr->data_count; j++) {
                        if (temp_ptr->data[j] == entry) {
                            if (temp_ptr->is_leaf()) {
                                temp_ptr->data[j] = data[i];
                            } else {
                                int index = first_ge(temp_ptr->data, temp_ptr->data_count, entry);
                                temp_ptr->subset[index + 1]->get_smallest(temp_ptr->data[j]);
                            }
                        }
                    }

                } else {
                    subset[i + 1]->get_smallest(data[i]);
                }
            }
            
        } else { //!is_leaf() && !found
            //if it doesn't run the first if statement, it's not found so that means we have to go through the left node to go remove it from the leaves
            subset[i]->loose_remove(entry);
            
            if (child_count > 1) {
                if (subset[i]->data_count < MINIMUM) {
                    fix_shortage(i);
                }
            }
        }
    } else { //then we know at this point, we have POSSIBLY found it | is_leaf()
        if (found) { // is_leaf() && found
            T deletedItem;
            //delete said item
            delete_item(data, i, data_count, deletedItem);
            return;
        } else { // is_leaf() && !found
            return;
        }
    }
}

template<typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i) {
    //transfer left and rotate left, as long as right has enough data to give to left
    if (subset[i+1] != nullptr && subset[i+1]->data_count > MINIMUM && i + 1 < child_count) {
        //i+1 < childcount so that we don't go out of bounds
        transfer_left(i + 1);
        return subset[i];
    } 
    //transfer right and rotate right, as long as left has enough data to give to right
    else if (i != 0 && subset[i-1]->data_count > MINIMUM) {
        transfer_right(i - 1);
        return subset[i];
    }
    //right merge
    else if (i > 0 && subset[i-1] != nullptr) {
        return merge_with_next_subset(i-1);
    } 
    //left merge
    else {
        return merge_with_next_subset(i);
    }
    
}

template<typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node() {
    //BPlusTree<T>* temp = (BPlusTree<T>*)this;
    //while (!temp->is_leaf()) {//check whether or not you're at the bottom
    //    temp = temp->subset[0];//keep going to the left side
    //}
    //return temp; //return it after while is terminated
    if (!is_leaf()) {
        return subset[0]->get_smallest_node();
    }
    return this;
}

template<typename T>
inline BPlusTree<T>* BPlusTree<T>::get_biggest_node() {
    BPlusTree<T>* temp = (BPlusTree<T>*)this;
    while (!temp->is_leaf()) {
        temp = temp->subset[temp->child_count - 1];
    }
    return temp;
}

template<typename T>
void BPlusTree<T>::get_smallest(T& entry) {
    entry = get_smallest_node()->data[0];
}

template<typename T>
void BPlusTree<T>::get_biggest(T& entry) {
    entry = get_biggest_node()->data[data_count - 1];
}

template<typename T>
void BPlusTree<T>::remove_biggest(T& entry) {
    if (is_leaf()) { //base case
        T detachedItem;
        detach_item(data, data_count, detachedItem); //detaches the ending of data
        entry = detachedItem; //now we know this is the biggest value of the susbet's
        return;
    }
    subset[child_count - 1]->remove_biggest(entry); //recursive call on the last child of each subset

    fix_shortage(data_count - 1); //fix the shortage when we detach an item, cuz it might be skinny
    return;
}

template<typename T>
void BPlusTree<T>::transfer_left(int i) { //i+1
    //we need to check if we can actually transfer to the right with sufficient data within the left
    //assert(subset[i]->data_count > 1);
    //make sure it's on a leaf and not a non leaf
    //assert(subset[i]->is_leaf());

    if (subset[i]->is_leaf()) {
        T deletedItem;
        //transfer subset[i+1]->data[0] to the end of [i-1]->data
        delete_item(subset[i]->data, 0, subset[i]->data_count, deletedItem);
        //transfer the deletedItem to the end
        attach_item(subset[i - 1]->data, subset[i - 1]->data_count, deletedItem);
        //ordered_insert(subset[i]->data, subset[i]->data_count, deletedItem);

        //ordered_insert(data, data_count, subset[i]->data[0]);
        data[i - 1] = subset[i]->data[0];
    } else {
        rotate_left(i);
    }
}

template<typename T>
void BPlusTree<T>::transfer_right(int i) { //this is i-1
    //we need to check if we can actually transfer to the right with sufficient data within the left
    //assert(subset[i]->data_count > 1);
    //make sure it's on a leaf and not a non leaf
    //assert(subset[i]->is_leaf());

    if (subset[i]->is_leaf()) {
        T deletedItem;
        //transfer the last data item from subset[i-1] to subset[i]
        detach_item(subset[i]->data, subset[i]->data_count, deletedItem); //detach the item from the subset[i]'s data

        //transferring the deletedItem to the subset[i]->data
        ordered_insert(subset[i+1]->data, subset[i+1]->data_count, deletedItem); //insert it into the right side

        //T temp;
        //delete_item(data, i, data_count, temp);
        //ordered_insert(data, data_count, deletedItem);

        data[i] = subset[i + 1]->data[0]; //make the parent the same as the children's first value
    } else {
        rotate_right(i);
    }
}

template<typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i) {
    assert(i + 1 < child_count); //make sure you're actually merging with the correct subset and it's within the number of children

    //    * 1. remove data[i] from this object
    //    * 2. if not a leaf, append it to child[i]->data:
    //    * 3. Move all data items from subset[i + 1]->data to right of subset[i]->data
    //    * 4. Move all subset pointers from subset[i + 1]->subset to
    //    * right of subset[i]->subset
    //    * 5. delete subset[i + 1](store in a temp ptr)
    //    * 6. if a leaf, point subset[i]->next to temp_ptr->next
    //    * 6. delete temp ptr


    //1.
    T deletedItem;
    delete_item(data, i, data_count, deletedItem); //remove data[i]
    
    //2.
    if (!subset[i]->is_leaf()) {
        //insert_item(subset[i]->data, 0, subset[i]->data_count, deletedItem);
        
        attach_item(subset[i]->data, subset[i]->data_count, deletedItem);
    }
    //so at this point, we would need to merge the bigger boy into the smaller boy and since
    //subset[i+1] has the values, we want to merge that into subset[i], the one with no values. Then since now they're merged,
    //merge these bad boys together, so merge the data and the subset into the next subset

    //3.src->dest
    merge(subset[i + 1]->data, subset[i + 1]->data_count, subset[i]->data, subset[i]->data_count);//merge data
    //4.src->dest
    merge(subset[i + 1]->subset, subset[i + 1]->child_count, subset[i]->subset, subset[i]->child_count); //merge children

    if (subset[i]->is_leaf()) {
        subset[i]->next = subset[i+1]->next;
    }
    BPlusTree<T>* deleted_ptr;
    delete_item(subset, i + 1, child_count, deleted_ptr); //delete subset[i+1] ptr since you merged already

     //delete the ptr
    deleted_ptr = nullptr;

    return subset[i];
}

template<class T>
void BPlusTree<T>::rotate_right(int i) {//i-1

    //detach from i-1 to move to data[i]
    T deleted_item;
    detach_item(subset[i]->data, subset[i]->data_count, deleted_item);

    //move the data[i] to the right side
    insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, data[i]);

    //now move the deleted_item to the data[i]
    data[i] = deleted_item;

    if (subset[i + 1]->is_leaf()) {
        data[i] = subset[i + 1]->data[0];
    }

    //rotate right, if we had 1 node and 3 children nodes, we give it to the other person
    //rotate pointers
    if (!is_leaf() && subset[i]->child_count > subset[i]->data_count+1) {
        BPlusTree<T>* deleted_item = nullptr;
        //grab the last element within i-1's subset
        delete_item(subset[i]->subset, subset[i]->child_count - 1, subset[i]->child_count, deleted_item);

        //insert the deletedItem at the beginning of the i+1's subset
        insert_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, deleted_item);
    }
}

template<class T>
void BPlusTree<T>::rotate_left(int i) { //i+1

    //delete the first value within subset[i]'s data
    T deleted_item;
    delete_item(subset[i]->data, 0, subset[i]->data_count, deleted_item);

    //now move the first item within data[] to the end of i-1
    attach_item(subset[i - 1]->data, subset[i - 1]->data_count, data[i-1]);

    //now move the deleted_item to the data[i-1]
    data[i - 1] = deleted_item;

    if (subset[i]->is_leaf()) { //check if it's a leaf to put it in inner nodes
        data[i-1] = subset[i]->data[0];
    }
    
    //move subset ptrs
    if (!is_leaf() && subset[i]->child_count > subset[i]->data_count + 1) {
        BPlusTree<T>* deleted_entry = nullptr;
        delete_item(subset[i]->subset, 0, subset[i]->child_count, deleted_entry);

        //attaches the left child of subset to the right side of i-1
        insert_item(subset[i - 1]->subset, subset[i - 1]->child_count, subset[i - 1]->child_count, deleted_entry);
    }
}

template<typename T>
void BPlusTree<T>::print_tree(int level, ostream& outs) const {
    const int INDENTATION = 1;
    if (!is_leaf()) { //we gotta check if it's not a leaf so that we can recurse down to the last child.
        subset[child_count - 1]->print_tree(level + INDENTATION, outs);
    }
    //print data from the root
    for (int i = data_count - 1; i >= 0; i--) {
        outs << std::setw(4 * level) << "" << "[" << data[i] << "]" << std::endl; //prints out the parent at that point
        if (!is_leaf()) {
            subset[i]->print_tree(level + INDENTATION, outs);//this will recurse to print out the childrens of that parent
        }
    }
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin() {
    //work on later
    //iterator already defaults the 2nd param to be key_ptr = 0;
    //returns Iterator(get_smallest_node()); --> returns the left most node
    return BPlusTree<T>::Iterator(get_smallest_node());
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end() {
    return BPlusTree<T>::Iterator(nullptr);
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry) {
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count&& data[i] == entry);

    if (found) { //if we found the entry
        if (is_leaf()) { //check if it's a leaf so that we know that the tree contains it
            return BPlusTree<T>::Iterator(this, i);
        } else {
            subset[i + 1]->find(entry); //check the right side of the node
        }
    } else {
        if (is_leaf()) { //check if it's a leaf, if it is, that means it's not in here, return false
            return nullptr;
        } else { //check the left side of the node
            subset[i]->find(entry);
        }
    }
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key) {
    for (BPlusTree<T>::Iterator it = begin(); it != end(); it++) {
        if (*it >= key) {
            return it;
        }
    }
    return nullptr;
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key) {
    //start at the beginning and go until the end of the leaves list,
//find the position where it's first greater than/equal to the key
    for (BPlusTree<T>::Iterator it = begin(); it != end(); it++) {
        if (*it > key) {
            return it;
        }
    }
    return nullptr;
}

template<typename T>
inline int BPlusTree<T>::size() const {
    int total = 0;
    for (BPlusTree<T>::Iterator it = begin(); it != end(); it++) {
        total++;
    }
    return total;
}

template<typename T>
inline bool BPlusTree<T>::empty() const {
    //if the size == 0, it will return false
//if the size is anything but 0, it will return true indicating that it's not empty 
    return size() == 0;
}
