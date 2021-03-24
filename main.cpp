// Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <random>

#include <iostream>
#include "headers/Token.h"
#include "headers/STokenizer.h"
#include "headers/Parser.h"
#include "headers/Table.h"
#include "headers/shuntingyard.h"
#include "headers/SQL.h"
#include "headers/BTreeFunctions.h"
using namespace std;
int main()
{
    SQL sql;
    sql.SQL_interactive();

    return 0;
}
