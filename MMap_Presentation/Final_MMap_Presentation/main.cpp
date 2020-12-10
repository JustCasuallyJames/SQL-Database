// Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Token.h"
#include "STokenizer.h"
#include "Parser.h"
#include "Table.h"
#include "shuntingyard.h"
#include "SQL.h"

#include "FileTokenizer.h"
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

MMap<string, long> get_word_indices(char* file_name);


int main(int argc, char* argv[]) {
    char temp[14] = { 's', 'o', 'l', 'i', 't', 'u', 'd', 'e', '.', 't', 'x', 't' };
    ofstream myfile;
    myfile.open("MMap_presentation_output.txt");
    
    MMap<string, long> word_indices;
    word_indices = get_word_indices(temp);
    myfile << endl << endl << endl;

    //list all nodes of the index mmap:
    for (MMap<string, long>::Iterator it = word_indices.begin();
        it != word_indices.end(); it++) {
        myfile << "[" << it.get_key() << "]" << endl << endl;
        myfile << *it << endl;
    }


    myfile << endl << endl << endl;
    myfile << "---------------------------------------------------" << endl;
    string this_word = "ice";
    myfile << "---------------------------------------------------" << endl;
    myfile << "Indices of \"" << this_word << "\"" << endl;
    //list indices of this_word:
    if (word_indices.contains(this_word)) {
        myfile << this_word << ": " << word_indices[this_word] << endl;
    }
    myfile << endl << endl << endl;

    myfile << "---------------------------------------------------" << endl;
    string from = "ask";
    string to = "asker";
    //list from .. to:
    myfile << "listing indices from \"" << from << "\" to \"" << to << "\"" << endl;
    myfile << "---------------------------------------------------" << endl;
    for (MMap<string, long>::Iterator it =
        word_indices.lower_bound(from); it != word_indices.upper_bound(to) && it != word_indices.end(); it++) {
        myfile << "[" << it.get_key() << "]" << endl << endl;        
        myfile << *it << endl;
    }
    myfile.close();
    myfile << endl << endl << endl << "========== E N D  ====================" << endl;
    return 0;
}


MMap<string, long> get_word_indices(char* file_name) {
    char temp[14] = { 's', 'o', 'l', 'i', 't', 'u', 'd', 'e', '.', 't', 'x', 't' };
    const bool debug = false;
    MMap<string, long> word_indices;
    FileTokenizer ftk(temp);
    Token t;
    long count = 0;

    ftk >> t;
    while (ftk.more()) {
        //only the "words"
        if (t.type_string() == "ALPHA") {
            string s;
            s = t.token_str();
            word_indices[s] += count;
            count++;
            if (debug)
                cout << "|" << t.token_str() << "|" << endl;
        }
        ftk >> t;
    }
    return word_indices;
}
