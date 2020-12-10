#pragma once

#include <iostream>
#include <string>

#include "Parser.h"
#include "Table.h"
#include "MMap.h"


class SQL {
public:
	SQL();
	void SQL_interactive(); //interactive tests
	void run_file(const string& filename); //runs a specific file name
	bool run_command(const string& command); //returns true if successfully ran the command

private:
	Parser p;
	Map<string, Table> _tables; //have a map of a bunch of tables
	void get_all_tables();
	void print_tables();
};