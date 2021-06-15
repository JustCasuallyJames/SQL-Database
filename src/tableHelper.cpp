#include "../headers/tableHelper.h"
//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]) {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            _table[i][j] = -1;
        }
    }
}
/*
 * confusing, never used.
 */
 //Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state) {
    for (int i = 0; i < MAX_COLUMNS; i++) {
        _table[state][i] = 1;
    }
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state) {
    for (int i = 0; i < MAX_COLUMNS; i++) {
        _table[state][i] = 0;
    }
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state) {
    return _table[state][0];
}

//Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state) {
    for (int i = from; i <= to; i++) {
        _table[row][i] = state;
    }
}

//Mark columns represented by the string columns[] for this row
//row : 2
//columns: "./\][!@#$%^&*()_-+=\0"
//used for to set a single type
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state) {
    int counter = 0;
    while (columns[counter] != '\0') {
        _table[row][columns[counter]] = state;
        counter++;
    }
}

//Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state) {
    _table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]) {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            std::cout << _table[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos) {
    int counter = 0;
    while (s[counter] != '\0') {
        std::cout << s[counter];
        counter++;
    }
}
