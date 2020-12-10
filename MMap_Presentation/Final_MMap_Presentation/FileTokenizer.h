#pragma once
#include <iostream>
#include <fstream>
#include "CONSTANTS.h"
#include "token.h"
#include "STokenizer.h"

class FileTokenizer {
public:
    const int MAX_BLOCK = MAX_BUFFER;
    FileTokenizer(string fname);
    ~FileTokenizer();
    bool done();
    bool more();
    friend FileTokenizer& operator >> (FileTokenizer& f, Token& t) {
        //if it's done then grab a new block
        if (f._stk.done()) { //checks if the tokenizer is done tokenizing the block
            f.get_new_block(); //if so, grab a new block.
        }
        f._stk >> t; //grabs a new token
        return f;
    }
   
private:
    bool get_new_block(); //gets the new block from the file
    std::ifstream _f;   //file being tokenized
    std::ofstream _w;
    STokenizer _stk;     //The STokenizer object to tokenize current block
    bool _done;
};

