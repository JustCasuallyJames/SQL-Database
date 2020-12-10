#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>

#include "Map.h"
#include "MMap.h"
#include "Record.h"
#include "shuntingyard.h"
#include "set_utility.h"
#include "RPN.h"
using namespace std;

class Table {
public:
	Table();
	Table(string name);
	Table(string name, const vector<string>& field_vals);

	Table select_all();
	Table select(const vector<string>& fields, const vector<string>& where);
	Table select_simple(const vector<string>& fields);
	Table select_where(const vector<string>& fields, const vector<string>& conditions);
	void insert_into(vector<string> fields);

	void reindex();
	
	friend ostream& operator << (ostream& outs, const Table& print_me) {
		const int number_width = 8;
		fstream f;
		string filename = print_me._name + ".txt";
		open_fileRW(f, filename.c_str());
		for (int i = 0; i < print_me._fields.size(); i++) {
			outs << setw(MAX_COL) << left << print_me._fields[i]; //print fields
		}
		outs << "\n";
		//print out a line
		for (int i = 0; i < print_me._fields.size() * MAX_COL; i++) {
			outs << "-";
		}
		outs << "\n";

		//now print out the contents
		Record r;
		for (long i = 1; r.read(f, i) > 0; i++) {
			r.read(f, i);
			vector<string> record = r.get_record();
			for (int j = 0; j < record.size(); j++) {//prints out the records
				outs << setw(MAX_COL) << left << record[j]; 
			}
			outs << "\n";
		}
		f.close();
		return outs;
	}

private:
	int _serial;
	string _name; //the filename
	vector<string> _fields;
	//Map<string, long> _indices;
	Map<string, MMap<string,long> > _indices; //basically your index
	Map<string, long> _field_map;
	
};