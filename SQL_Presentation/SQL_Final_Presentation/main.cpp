// Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Token.h"
#include "STokenizer.h"
#include "Parser.h"
#include "Table.h"
#include "shuntingyard.h"
#include "SQL.h"

#include "BTreeFunctions.h"
using namespace std;
//int main()
//{
//    SQL sql;
//    sql.SQL_interactive();
//    //sql.run_file("_!select.txt");
//    //Parser p("select first,last from employees where first >= \"Mohamad Jimmy.\" or (last = Elmoussawi and major = CS) or major = McDonalds");
//    //Parser p("create table employees fields age,first,last");
//    Parser p("insert into employee values Blow,     Joe,        \"CS\", 100000, 2018");
//    //Parser p("select first,last,age from employees where age >= 69");
//    MMap<string, string> mp;
//    p.parse_tree(mp);
//
//    cout << mp << endl;
//    cout << mp["fields"] << endl;
//    vector<string> fields;
//    fields.push_back("last");
//    fields.push_back("first");
//    fields.push_back("age");
//    fields.push_back("major");
//
//    Table t("employees", fields);
//
//    vector<string> person1 = { "Elmoussawi","Mohamad Jimmy.", "69", "CS" };
//    vector<string> person2 = { "Vo","James", "129", "McDonalds" };
//    vector<string> person3 = { "Smith","Bobette", "3", "Celtic Studies" };
//
//    t.insert_into(person1);
//    t.insert_into(person2);
//    t.insert_into(person3);
//
//    cout << t << endl;
//
//    if (mp["commands"][0] == "select") {
//        string table_name = mp["table_name"][0];
//        Table t1(table_name);
//        vector<string> empty;
//        /*cout << t1.select(mp["fields"], empty);*/
//        cout << t1.select(mp["fields"], mp["where"]);
//    }
//
//
//    return 0;
//}
int Random(int lo, int hi) {
    int r = rand() % (hi - lo + 1) + lo;

    return r;
}

void test_iterator() {
    const bool debug = false;
    BPlusTree<int> bpt;
    for (int i = 0; i < 125; i++)
        bpt.insert(Random(0, 100));
    cout << bpt << endl;


    cout << "------------------------------------------------------------" << endl;
    for (BPlusTree<int>::Iterator it = bpt.begin();
        it != bpt.end(); ++it) {
        if (debug) it.print_Iterator();
        cout << "[" << *it << "] ";
        if (debug) cout << endl;
    }
    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "test ++ operator: " << endl;
    BPlusTree<int>::Iterator it = bpt.begin();
    cout << "{" << *(it++) << "}" << endl;
    cout << "{" << *it << "}" << endl;
    for (int i = 60; i < 75; i++) {
        it = bpt.find(i);
        if (!it.is_null())
            cout << *it << " was found." << endl;
        else
            cout << i << " was not found" << endl;
    }
    cout << "====================================================================" << endl;

}

bool test_bplustree_auto(int how_many, bool report) {
    const int MAX = 10000;
    assert(how_many < MAX);
    BPlusTree<int> bpt;
    int a[MAX];
    int original[MAX];
    int deleted_list[MAX];

    int original_size;
    int size;
    int deleted_size = 0;

    //fill a[ ]
    for (int i = 0; i < how_many; i++) {
        a[i] = i;
    }
    //shuffle a[ ]: Put this in a function!
    for (int i = 0; i < how_many; i++) {
        int from = Random(0, how_many - 1);
        int to = Random(0, how_many - 1);
        int temp = a[to];
        a[to] = a[from];
        a[from] = temp;
    }
    //copy  a[ ] -> original[ ]:
    copy_array(a, original, how_many, original_size);
    size = how_many;
    original_size = how_many;
    for (int i = 0; i < size; i++) {
        bpt.insert(a[i]);
    }
    cout << bpt << endl;
    if (report) {
        cout << "========================================================" << endl;
        cout << "  " << endl;
        cout << "========================================================" << endl;
        cout << bpt << endl << endl;
    }
    for (int i = 0; i < how_many; i++) {
        int r = Random(0, how_many - i - 1);
        if (report) {
            cout << "========================================================" << endl;
            cout << bpt << endl;
            cout << ". . . . . . . . . . . . . . . . . . . . . . . . . . . . " << endl;
            cout << "deleted: "; print_array(deleted_list, deleted_size);
            cout << "   from: "; print_array(original, original_size);
            cout << endl;
            cout << "  REMOVING [" << a[r] << "]" << endl;
            cout << "========================================================" << endl;
        }

        //bpt.remove(a[r]);


        delete_item(a, r, size, deleted_list[deleted_size++]);
        if (!bpt.is_valid()) {
            cout << setw(6) << i << " I N V A L I D   T R E E" << endl;
            cout << "Original Array: "; print_array(original, original_size);
            cout << "Deleted Items : "; print_array(deleted_list, deleted_size);
            cout << endl << endl << bpt << endl << endl;
            return false;
        }

    }
    if (report) cout << " V A L I D    T R E E" << endl;

    return true;
}
void test_bplustree_auto(int tree_size, int how_many, bool report) {
    bool verified = true;
    for (int i = 0; i < how_many; i++) {
        if (report) {
            cout << "*********************************************************" << endl;
            cout << " T E S T:    " << i << endl;
            cout << "*********************************************************" << endl;
        }
        if (!test_bplustree_auto(tree_size, report)) {
            cout << "T E S T :   [" << i << "]    F A I L E D ! ! !" << endl;
            verified = false;
            return;
        }

    }
    cout << "**************************************************************************" << endl;
    cout << "**************************************************************************" << endl;
    cout << "             E N D     T E S T: " << how_many << " tests of " << tree_size << " items: ";
    cout << (verified ? "VERIFIED" : "VERIFICATION FAILED") << endl;
    cout << "**************************************************************************" << endl;
    cout << "**************************************************************************" << endl;

}



int main(int argc, char* argv[]) {
    cout << endl << endl << endl << "===============================" << endl << endl << endl << endl;

    //------------------------------------------
    srand(time(0));
    //------------------------------------------



    //test_bplustree_insert_random();
//    test_bplustree_remove();
//    test_bplustree_interactive();
//    test_bplustree_big_three();
    test_bplustree_auto(1000, 100, false);

    //    test_map();
    //    test_map_interactive();

    //    test_multimap();
    //    test_multimap_interactive();
    //    test_multimap_auto();

    //test_iterator();
    cout << endl << endl << endl << "===============================" << endl;
    return 0;
}

