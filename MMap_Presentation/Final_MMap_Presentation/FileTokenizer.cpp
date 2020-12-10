#include "FileTokenizer.h"


FileTokenizer::FileTokenizer(string fname) {
    _f.open(fname); //open the file
    get_new_block(); //Grab a new block
}
bool FileTokenizer::done() {
    return _f.eof() && _stk.done(); //checks if it's at the end of the file and if the tokenizer
                                    //is done tokenizing.
}

bool FileTokenizer::get_new_block() {
    char token[MAX_BUFFER+1]; //char array of 500 characters plus the null character at the end
    _f.read(token, MAX_BUFFER); // Only read the 500 characters since read doesn't include the null
    token[_f.gcount()] = '\0'; //_f.gcount() returns the amount of characters _f read from the file
    _stk.set_string(token); //sets the token, inclusive of the null, to the stk string
    _done = _stk.done(); //set done to the stk.done()
    return _done; //return it
}

FileTokenizer::~FileTokenizer() {
    _f.close(); //closes when you're done reading.
}
bool FileTokenizer::more() {
    return !done();
}