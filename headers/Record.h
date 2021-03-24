#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include "CONSTANTS.h"
using namespace std;

//utility functions
bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);

void write_info(fstream& f, const vector<string>& items);
void read_info(fstream& f, vector<string>& items);

class Record {
public:
    //when you construct a Record, it's either empty or it
//  contains a word
    Record() {
        //initialize the entire 2d record to be null
        for (int i = 0; i < MAX_ROW; i++) {
            for (int j = 0; j < MAX_COL; j++) {
                _record[i][j] = NULL;
            }
        }
        recno = -1;
        num_fields = 0;
    }

    Record(vector<string> field_list) {
        //init's all the records [i][j] to null
        for (int i = 0; i < MAX_ROW; i++) {
            for (int j = 0; j < MAX_COL; j++) {
                _record[i][j] = NULL;
            }
        }

        //copies all the values within the fieldlist into the records
        for (int i = 0; i < field_list.size(); i++) {
            strcpy_s(_record[i], field_list[i].c_str());
        }
        num_fields = field_list.size();
        recno = 0;
    }

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file
    //helper functions
    bool write_row(const char* str); //shows whether or not a row has been successfully written into
    vector<string> get_record(); //grabs a records values and pushes it into a vector
    long append_file(fstream& f); //appends to the end of the file

    friend ostream& operator<<(ostream& outs, const Record& r) {
        for (int i = 0; i < MAX_ROW; i++) {
            if (r._record[i][0] != NULL) {
                outs << setw(20) << r._record[i];
            }
        }
        return outs;
    }
private:
    int recno;      //represnets the number of records
    int num_fields; //represents the number of fields
    char _record[MAX_ROW][MAX_COL];
};