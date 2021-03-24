#pragma once
const int MAX_BUFFER = 600;
//apart of the old string tokenizer
//const int MAX_COLUMNS = 127;
//const int MAX_ROWS = 8;

const int MAX_COLUMNS = 128;
const int MAX_ROWS = 50;

const int DECIMAL_STATE = 3;
const int NUMBER_AFTER_DECIMAL_STATE = 4;
const int APOS_STATE = 7;
const int LETTER_AFTER_APOS_STATE = 8;
const int UNKNOWN_STATE = 7;

//tables/record stuff
static const int MAX_ROW = 30; 
static const int MAX_COL = 30; 