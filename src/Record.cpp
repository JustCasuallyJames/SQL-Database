#include "../headers/Record.h"

long Record::write(fstream& outs) {

    //write to the end of the file.
    outs.seekg(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer

    for (int i = 0; i < MAX_ROW; i++) {
        outs.write(_record[i], MAX_COL);
    }
    return pos / sizeof(_record[0]);  //record number
}

long Record::read(fstream& ins, long recno) {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            _record[i][j] = NULL;
        }
    }
    this->recno = recno; //sets the recno
    ins.seekg(MAX_ROW * MAX_COL * recno);

    for (int i = 0; i < MAX_ROW; i++) {
        ins.read(_record[i], MAX_COL);
    }
    return ins.gcount();

}
bool Record::write_row(const char* str) {
    for (int i = 0; i < MAX_ROW; i++) {
        if (_record[i][0] == '\0') {
            strcpy_s(_record[i], str);
            return true;
        }
    }
    return false;
}
vector<string> Record::get_record() {
    //this is going to be needed so when we have a vector of records, we can save it into the table
    vector<string> row;

    for (int i = 0; i < MAX_ROW && _record[i][0] != '\0'; i++) {
        char field[MAX_COL] = "";
        strcpy_s(field, _record[i]);
        row.push_back(_record[i]);
    }
    return row;
}
//appends all the values within the record into the new file
long Record::append_file(fstream& f) {
    f.seekp(0, std::ios::end);

    long pos = f.tellp() / (MAX_ROW * MAX_COL);

    for (int i = 0; i < MAX_ROW; i++) {
        f.write(_record[i], MAX_COL); //writes it into the file you give it
    }
    return pos; //returns the f.tellp() part
}
//[    |    |     |    |    |     |]
//-------------------------------------------------
bool file_exists(const char filename[]) {
    const bool debug = false;
    fstream ff;
    ff.open(filename, std::fstream::in | std::fstream::binary);
    if (ff.fail()) {
        if (debug) cout << "file_exists(): File does NOT exist: " << filename << endl;
        return false;
    }
    if (debug) cout << "file_exists(): File DOES exist: " << filename << endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*) {
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)) {
        //create the file
        f.open(filename, std::fstream::out | std::fstream::binary);
        if (f.fail()) {
            cout << "file open (RW) failed: with out|" << filename << "]" << endl;
            throw("file RW failed  ");
        } else {
            if (debug) cout << "open_fileRW: file created successfully: " << filename << endl;
        }
    } else {
        f.open(filename,
            std::fstream::in | std::fstream::out | std::fstream::binary);
        if (f.fail()) {
            cout << "file open (RW) failed. [" << filename << "]" << endl;
            throw("file failed to open.");
        }
    }

}
void open_fileW(fstream& f, const char filename[]) {
    f.open(filename,
        std::fstream::out | std::fstream::binary);
    if (f.fail()) {
        cout << "file open failed: " << filename << endl;
        throw("file failed to open.");
    }

}

void write_info(fstream& f, const vector<string>& items) {

}

void read_info(fstream& f, vector<string>& items) {

}
