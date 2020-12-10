#include "Parser.h"

int Parser::_table[MAX_ROWS][MAX_COLUMNS];

Parser::Parser() {

}

Parser::Parser(const string& s) {
	make_table(_table);
	set_string(s);

}

void Parser::set_string(const string& command) { //sets the command
	_command = command;
	_stk.set_string(_command.c_str());
	Token t;
	while (_stk.more()) {
		//process token here...
		_stk >> t;
		tokens.push(t);
		t = Token();
	}
}

bool Parser::parse_tree(MMap<string, string>& map) {
	bool quote_state = false;
	bool completeDone = true;
	map.clear(); //clear the map
	_last_state = 0;
	//create the machine first
	make_machine();
	//initialize the keywords
	init_keywords();

	while (!tokens.empty() && completeDone) { //till the tokens vector are empty, popping it
		Token t = tokens.pop(); //pop
		//check if the token is a space, if it is, keep popping till you don't encounter a space
		while (std::isspace(t.token_str()[0]) && !tokens.empty()) {
			t = tokens.pop();
		}//after this loop is done, you'll be given a nonspace
		if (tokens.empty() && t.type_string() == "SPACE") {
			completeDone = false;
		}
		if (completeDone) { //this is going to be when theres a space at the end of the command
			string tokenstr = t.token_str();
			if (tokenstr == "\"") {
				quote_state = true;
			}
			if (quote_state) {
				string s = "";
				while (!tokens.empty() && quote_state) {
					Token t1 = tokens.pop();
					if (t1.type_string() == "PUNCTUATION") {
						if (t1.token_str().size() > 1 && t1.token_str()[0] != '\"') {
							string temp = "";
							for (int i = 0; t1.token_str()[i] != '\"'; i++) {
								temp += t1.token_str()[i]; //keeps adding until you see a quote
							}
							quote_state = false;
							for (int j = 0; j < temp.size(); j++) {
								s += temp[j]; //this will add all punctuations to the token
							}
						} else {
							//s += tokenstr[0]; //this knows that it will be ", which is a quote, we disregard everything after the quote
							quote_state = false;
						}
					} else {
						s += t1.token_str();
					}
				}
				tokenstr = s;
			}

			if (tokenstr == "(" || tokenstr == ")") {
				map.insert("where", tokenstr);
			} else {
				_last_state = update_state(tokenstr);
				switch (_last_state) {
				case 1: //select command
				case 5: //from
				case 13: //create/make
				case 19:
					map.insert("commands", tokenstr);
					break;

				case 2: //fields
				case 4: // asteriks (*)
				case 17: //actual field_names when doing create/make
					map.insert("fields", tokenstr);
					break;

				case 6: //table_name
				case 15: //table name for make/create
				case 21:
					map.insert("table_name", tokenstr);
					break;

				case 8: //this is a field for after where
				case 9: //this is where you get a relational operator
				case 10://value
				case 11://OR
				case 12://And
					map.insert("where", tokenstr);
					break;

				case 23:
					//these are the values
					map.insert("values", tokenstr);
					break;

				}
				if (_last_state == -1) {//if it goes to a fail state, just stop it
					map.clear();
					return false;
				}
			}
		}
	}
	return true;
}

void Parser::make_machine() {
	//initialize the table
	init_table(_table);

	//mark the keyword states

	//SELECT marks
	mark_cell(0, _table, SELECT, 1);

	//marking the "FROM"
	mark_cell(2, _table, FROM, 5); //goes from field to from
	mark_cell(4, _table, FROM, 5); //marks from *'s to from

	//marking the "*"
	mark_cell(1, _table, ASTERISK, 4);

	//marking the "SYMBOL", basically any words
	mark_cell(1, _table, SYMBOL, 2); //marks a field
	mark_cell(5, _table, SYMBOL, 6); //marks when you've arrived at a table

	//marking the "COMMA", the thing after the symbol
	mark_cell(2, _table, COMMA, 3); //MAKE SURE TO CHANGE THE 7 AS THE TABLE KEEPS GROWING
	mark_cell(3, _table, SYMBOL, 2); //goes back from field to a comma to a field

	//marking the "WHERE" 
	mark_cell(6, _table, WHERE, 7); //marking where ex: select last from employees where salary > 100,000
	mark_cell(7, _table, SYMBOL, 8); //marking the field
	mark_cell(8, _table, RELATIONAL_OPERATOR, 9); //relational is =,<= etc
	//mark_cell(8, _table, SYMBOL, 9);
	mark_cell(9, _table, SYMBOL, 10);

	//marking the OR/AND
	mark_cell(10, _table, LOGICAL_OPERATOR, 11); //OR
	mark_cell(10, _table, LOGICAL_OPERATOR, 12); //AND
	//mark_cell(10, _table, RELATIONAL_OPERATOR, 8); //relational op

	mark_cell(11, _table, SYMBOL, 8); //marking it to go get more conditions FROM OR
	mark_cell(12, _table, SYMBOL, 8); //marking it to go get more conditions FROM AND

	//this is going to be the make/create table
	mark_cell(0, _table, CREATE, 13);
	mark_cell(13, _table, TABLE, 14);
	mark_cell(14, _table, SYMBOL, 15); //this means this will be the table name
	mark_cell(15, _table, FIELDS, 16); //this will know to look for fields or like symbols essentially
	mark_cell(16, _table, SYMBOL, 17); 
	mark_cell(17, _table, COMMA, 18);
	mark_cell(18, _table, SYMBOL, 17);

	//marking the insert stuff
	mark_cell(0, _table, INSERT, 19); //insert
	mark_cell(19, _table, SYMBOL, 20); //into
	mark_cell(20, _table, SYMBOL, 21); //table name
	mark_cell(21, _table, VALUES, 22); //values
	mark_cell(22, _table, SYMBOL, 23); //symbol
	mark_cell(23, _table, COMMA, 24); //comma
	mark_cell(24, _table, SYMBOL, 23); //symbol
	mark_cell(23, _table, SYMBOL, 23); //symbol

	//SUCESS STATES
	mark_cell(0, _table, 0, 0);
	mark_cell(1, _table, 0, 0);
	mark_cell(2, _table, 0, 0);
	mark_cell(3, _table, 0, 0);
	mark_cell(4, _table, 0, 0);
	mark_cell(5, _table, 0, 0);
	mark_cell(6, _table, 0, 1); //select last from employees << VALID STATEMENT

	//everything after the first success state above
	mark_cell(7, _table, 0, 0); //select last from employees where 
	mark_cell(8, _table, 0, 0); //select last from employees where last
	mark_cell(9, _table, 0, 0); //select last from employees where last = 
	mark_cell(10, _table, 0, 1); //select last from employees where last = vo << VALID STATEMENT

	//marking everything after a single where condition
	mark_cell(11, _table, 0, 0); //OR
	mark_cell(12, _table, 0, 0); //AND

	//marking the make/create type of state
	mark_cell(13, _table, 0, 0);
	mark_cell(14, _table, 0, 0);
	mark_cell(15, _table, 0, 0);
	mark_cell(16, _table, 0, 0);
	mark_cell(17, _table, 0, 1); //this is when it's like create table employees fields age
	//mark_cell(17, _table, 0, 1); //this is when it's like create table employees fields age,first,last
	mark_cell(18, _table, 0, 0);

	//marking the insert stuff
	mark_cell(19, _table, 0, 0);
	mark_cell(20, _table, 0, 0);
	mark_cell(21, _table, 0, 0);
	mark_cell(22, _table, 0, 0);
	mark_cell(23, _table, 0, 1);
	mark_cell(24, _table, 0, 0);

}

void Parser::init_keywords() {

	_keywords["SELECT"] = SELECT;
	_keywords["FROM"] = FROM;
	_keywords["VALUES"] = VALUES;
	_keywords["WHERE"] = WHERE;
	_keywords["AND"] = LOGICAL_OPERATOR;
	_keywords["OR"] = LOGICAL_OPERATOR;
	_keywords["CREATE"] = CREATE;
	_keywords["MAKE"] = CREATE;
	_keywords["TABLE"] = TABLE;
	_keywords["FIELDS"] = FIELDS;
	_keywords["INSERT"] = INSERT;
	_keywords[">"] = RELATIONAL_OPERATOR;
	_keywords[">="] = RELATIONAL_OPERATOR;
	_keywords["<="] = RELATIONAL_OPERATOR;
	_keywords["<"] = RELATIONAL_OPERATOR;
	_keywords["="] = RELATIONAL_OPERATOR;
	_keywords["*"] = ASTERISK;
	_keywords[","] = COMMA;
	_keywords["("] = OPEN_PARENTHESES;
	_keywords[")"] = CLOSE_PARENTHESES;

}

int Parser::update_state(string token) {
	int keywordType = SYMBOL; //just the base case
	//we're going to capitalize every letter in token
	for (int i = 0; i < token.size(); i++) { 
		token[i] = toupper(token[i]);
	} //just want it to be consistent.
	if (_keywords.contains(token)) {
		keywordType = _keywords[token];
	}
	_last_state = _table[_last_state][keywordType];
	return _last_state;
}

void Parser::make_table(int _table[][MAX_COLUMNS]) {
	for (int i = 0; i < MAX_ROWS; i++) {
		for (int j = 0; j < MAX_COLUMNS; j++) {
			_table[i][j] = -1;
		}
	}
}

