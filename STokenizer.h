#pragma once
#include <iostream>
#include "token.h"
#include "CONSTANTS.h"
#include "tableHelper.h"
#include <string.h>
class STokenizer {
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t) {
        string tokenString = "";
        s.get_token(0, tokenString);
        t = Token(tokenString, s._last_state);
        return s;
    }
    bool get_token(int start_state, string& token);
    //set a new string as the input string
    void set_string(const char str[]);

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //---------------------------------
    char _buffer[MAX_BUFFER+1];       //input string
    int _pos;                       //current position in the string
    int _last_state;                //the last success state the character was in
    static int _table[MAX_ROWS][MAX_COLUMNS];
};
