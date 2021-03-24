#pragma once

#include <iostream>
#include <vector>

#include "BPlusTrees.h"
#include "STokenizer.h"
#include "MMap.h"
#include "Map.h"
#include "Pair.h"
#include "MPair.h"
#include "keywords.h"
#include "CONSTANTS.h"
#include "Queue.h"

using namespace std;
class Parser {
public:
	Parser();
	Parser(const string& s);
	void set_string(const string& command);
	bool parse_tree(MMap<string, string>& map);
private:
	string _command;
	Queue<Token> tokens; //a queue of tokens
	STokenizer _stk;

	//stuff with the state machine
	Map<string, int> _keywords;
	int _last_state = 0;

	int update_state(string token); //returns the updated state as last_State
	void make_table(int _table[][MAX_COLUMNS]);
	void make_machine();
	void init_keywords();
	static int _table[MAX_ROWS][MAX_COLUMNS]; //the state machine table

};