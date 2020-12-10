#include "STokenizer.h"
int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer() {
    char str[] = {'\0'};
    set_string(str);
    make_table(_table);
}
STokenizer::STokenizer(char str[]) {
    set_string(str);
    make_table(_table);
}
bool STokenizer::get_token(int start_state, string& token) {
    string tempResult = "";
    int current_state = start_state;
    int counter = _pos;
    //Check until either the state is -1 or you're at the end of the string
    while (current_state != -1 && !done()) {
        char currentChar = _buffer[counter];//obtain the first character
        if (currentChar < 0) {
            current_state = -1; //we know the current state is invalid
            if (tempResult.size() == 0) { //checks if theres anything in tempResult
                tempResult = currentChar; // if theres nothing, then set the currentchar to tempresult
                _pos++; //increment the position
                counter++; //increment the counter (don't really see how this is necessary though)
                _last_state = -1; //make the last state to -1 so it can trigger token.str() to be SPECIAL
                //meant for when the tokenizer sees a token that's a special character so we just spit 
                //it out
            }
        }else if (current_state == 0) {
            //Have this so that we could initialize the current state of the buffer
            //if you have it not in an if statement, it sets the current state regardless of anything
            current_state = _table[current_state][currentChar];
        }
        else if (is_success(_table, current_state) && _table[current_state][currentChar] == current_state) {
            //add the character and increment counters and pos
            tempResult += currentChar;
            _pos++;
            counter++;
            //update the last state that it was in
            _last_state = current_state;
            //updates the current state after we confirmed that it's a success state
            current_state = _table[current_state][currentChar];
        }
        else if (_table[current_state][currentChar] == DECIMAL_STATE) { //To represent theres a decimal
            counter++;
            current_state = _table[current_state][currentChar];
        }
        else if (_table[current_state][currentChar] == NUMBER_AFTER_DECIMAL_STATE) {
            //this shows theres a number after the decimal

            tempResult += _buffer[_pos]; //gets the decimal
            tempResult += _buffer[counter]; //gets the number after the decimal
            counter++; //increment the counter
            _pos = counter; //set the position to the new counter
            current_state = _table[current_state][currentChar];//update the current state
        }
        else if (_table[current_state][currentChar] == APOS_STATE) { //To represent theres a decimal
            counter++;
            current_state = _table[current_state][currentChar];
        } else if (_table[current_state][currentChar] == LETTER_AFTER_APOS_STATE) {
            //this shows theres a number after the decimal

            tempResult += _buffer[_pos]; //gets the decimal
            tempResult += _buffer[counter]; //gets the number after the decimal
            counter++; //increment the counter
            _pos = counter; //set the position to the new counter
            current_state = _table[current_state][currentChar];//update the current state
        }
        else {
            //this means it experienced another character that wasn't of the same type
            current_state = -1;
        }

    }
    token = tempResult; //set the found token to the token from the parameter
    //checks if theres actually a string to tokenize.
    return token.length() > 0;
}

void STokenizer::make_table(int table[][MAX_COLUMNS]) {
    init_table(table);
    //initialize all the tables
    const char punctuation[] = {
            '!','"','#','$','%','&','\'','(',')','*','+',
            ',','-','.','/',':',';','<','=','>','?','@','[',
            ']','\\','^','_','`','{','}','|','~','\0'
    };
    //punctuation marking cells
    mark_cells(0, table, punctuation, 5);
    mark_cells(5, table, punctuation, 5);
    //Alphabets marking cells
    //capitals
    mark_cells(0, table, 65, 90, 1);
    mark_cells(1, table, 65, 90, 1);
    //lowercase
    mark_cells(0, table, 97, 122, 1);
    mark_cells(1, table, 97, 122, 1);

    //mark the apostrophe state and stuff
    mark_cell(1, table, 39, 7); //apostrophe
    mark_cells(7, table, 65, 90, 8); //capitals
    mark_cells(7, table, 97, 122, 8); //lowers

    //stuff after the apostrophe
    mark_cells(7, table, 65, 90, 8); //capitals
    mark_cells(7, table, 97, 122, 8); //lowers

    //Digits marking cells
    mark_cells(0, table, 48, 57, 2);
    mark_cells(2, table, 48, 57, 2);
    //digits after digits
    mark_cells(3, table, 48, 57, 4);
    //digits after decimal point indicating a decimal
    mark_cells(4, table, 48, 57, 4);
    //digits to decimals
    mark_cell(2, table, 46, 3);
    //Spaces
    mark_cell(0, table, 32, 6); //space into a space
    mark_cell(0, table, 10, 6);//new line into a space 
    mark_cell(0, table, 9, 6); //making a tab into a space

    mark_cell(6, table, 32, 6);//space into a space
    mark_cell(6, table, 10, 6);//new line into a space
    mark_cell(6, table, 9, 6);//making a tab into a space


    //marking success states
    mark_cell(0, table, 0, 0); //nothing
    mark_cell(1, table, 0, 1); //Alpha token
    mark_cell(2, table, 0, 1); //this is the digit first: 2
    mark_cell(3, table, 0, 0); //this is the decimal: 2.
    mark_cell(4, table, 0, 1); //this is the digit after the decimal: 2.0
    mark_cell(5, table, 0, 1); //this is the punc
    mark_cell(6, table, 0, 1); //this is the space
    mark_cell(7, table, 0, 0); //this is the apos state
    mark_cell(8, table, 0, 0); //this is the letter state after the apos
    
}

bool STokenizer::done() {
    return _buffer[_pos] == '\0';
}

bool STokenizer::more() {
    return !done();
}
void STokenizer::set_string(const char str[]) {
    strncpy_s(_buffer, str, MAX_BUFFER); //make a copy of str[] and copy all the characters up until
                                         //max_buffer into _buffer
    _pos = 0;
}