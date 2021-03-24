// Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <random>

#include <iostream>
#include "Token.h"
#include "STokenizer.h"
#include "Parser.h"
#include "Table.h"
#include "shuntingyard.h"
#include "SQL.h"
#include "BTreeFunctions.h"
using namespace std;
int main()
{
    //SQL sql;
    //sql.SQL_interactive();
    
    int temp = 20; //integer that is equal to 20
    int* int_ptr;

    int_ptr = &temp;
    //int_ptr = 20
    //temp = 20
    //int* is a pointer which means that it's a memory address

    //char[] == char*
    //int temp[9] = 1->9
    // arrays are actually pointers
    // [2] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
    // [1] [6] [3] [4] [5] [ ] [ ] [ ] [ ]
    // [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
    // [ ] [ ] [ ] [ ] [ ] [7] [ ] [ ] [9]

    //1->2->3->4->5->6->7->9

    cout << "Temp pointer: " << int_ptr << endl; //prints out memory address
    cout << "Temp variable: " << &temp << endl; //this prints out 20 
    return 0;
}
