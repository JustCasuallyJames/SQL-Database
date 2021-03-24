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
    SQL sql;
    sql.SQL_interactive();

    return 0;
}
