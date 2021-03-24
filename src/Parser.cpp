#include "../headers/Parser.h"

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
	string batch_name = "";
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
		if (tokens.empty() && t.type_string() == "SPACE") { //maybe you're at the end of the sentence and it has a space
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
					if (t1.type_string() == "PUNCTUATION" && map["batch"].empty()) {

						if (t1.token_str().size() > 1 && t1.token_str()[0] != '\"') {
							string temp = "";
							for (int i = 0; t1.token_str()[i] != '\"'; i++) {
								temp += t1.token_str()[i]; //keeps adding until you see a quote
							}
							quote_state = false;
							for (int j = 0; j < temp.size(); j++) {
								s += temp[j]; //this will add all punctuations to the token
							}
						}  else {
							quote_state = false; //this just means something if it's "make table "barkeshli's students" fields...
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
					//case 5: //from
					case 13: //create/make
					case 19:
					case 25: //batch
					case 27:
						map.insert("commands", tokenstr);
						break;

					case 2: //fields
					case 4: // asteriks (*)
					case 17: //actual field_names when doing create/make
					case 18: {
						if (tokenstr != ",") {
							map.insert("fields", tokenstr);
						}
						break;
					}
					case 6: //table_name
					case 15: //table name for make/create
					case 21:
					case 28:
						map.insert("table_name", tokenstr);
						break;

					case 8: //this is a field for after where
					case 9: //this is where you get a relational operator
					case 10://value
					case 11://OR
					case 12://And
						map.insert("where", tokenstr);
						break;

					case 23://these are the values
						map.insert("values", tokenstr);
						break;

					case 26: { //represents all the strings after batch
						if (tokenstr[0] == '\"') {//if running batch, this if will run: batch "_!select.txt"
							batch_name += tokenstr.substr(1); //it skips the quote and moves to the end
							while (!tokens.empty()) {
								string temp = tokens.pop().token_str();
								if (temp != "\"") {
									batch_name += temp;
								}
							}
						} else { //if you're running the batch, this else statement will run: batch _!select.txt
							batch_name += tokenstr;
							while (!tokens.empty()) {
								string temp = tokens.pop().token_str();
								if (temp != "\"") {
									batch_name += temp;
								}
							}
						}
						break;
					}
				}
				if (_last_state == -1) {//if it goes to a fail state, just stop it
					map.clear();
					return false;
				}
			}
		}
	}
	
	if (batch_name.size() > 1) { //if the batchname has more than a certain amount of characters, then it will push it into the map.
		map.insert("table_name", batch_name);
	}
	return true;
}

void Parser::make_machine(){

	//HONESTLY IF YOU'RE LOOKING AT THIS CODE, DON'T BECAUSE YOU NEED THE DIAGRAM TO UNDERSTAND WHAT THIS MEANS
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
	mark_cell(16, _table, COMMA, 17); //example: fields "first name", "last name" [so it will go to the symbol statea from the comma state]
	mark_cell(17, _table, COMMA, 18);
	mark_cell(17, _table, SYMBOL, 18); 
	mark_cell(18, _table, SYMBOL, 17);
	mark_cell(18, _table, COMMA, 17);

	//marking the insert stuff
	mark_cell(0, _table, INSERT, 19); //insert
	mark_cell(19, _table, SYMBOL, 20); //into
	mark_cell(20, _table, SYMBOL, 21); //table name
	mark_cell(21, _table, VALUES, 22); //values
	mark_cell(22, _table, SYMBOL, 23); //symbol
	mark_cell(23, _table, COMMA, 24); //comma
	mark_cell(24, _table, SYMBOL, 23); //symbol
	mark_cell(23, _table, SYMBOL, 23); //symbol

	//marking the batch state
	mark_cell(0, _table, BATCH, 25); //going to the batch
	mark_cell(25, _table, SYMBOL, 26); //going to the symbol
	mark_cell(26, _table, SYMBOL, 26); //you shouldn't get anything after the ".txt" but whatever

	//quote states (not used yet)
	mark_cell(5, _table, QUOTE, 27);
	mark_cell(27, _table, SYMBOL, 28);
	mark_cell(28, _table, QUOTE, 6);

	mark_cell(7, _table, QUOTE, 29);
	mark_cell(29, _table, SYMBOL, 30);
	mark_cell(29, _table, SYMBOL, 30);

	//These are the drop table
	mark_cell(0, _table, DROP, 27);
	mark_cell(27, _table, SYMBOL, 28);
	/*
		-----------------------------------------------------
						   SUCCESS STATES
		-----------------------------------------------------
	*/
	//does a simple select statement
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

	//marking the batch
	/*
		as of 12/7/2020, barkeshli said that we did not test reserved words for the text file
		ie: CANNOT run "batch batch.txt"
			YES run "batch batch1.txt"
	*/
	mark_cell(25, _table, 0, 0); //represents "batch batch1.txt" 
	mark_cell(26, _table, 0, 1); 

	//marking the drop
	mark_cell(26, _table, 0, 0);
	mark_cell(27, _table, 0, 1);

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
	_keywords["BATCH"] = BATCH;
	_keywords["DROP"] = DROP;
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

