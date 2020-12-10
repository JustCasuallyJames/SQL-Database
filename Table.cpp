#include "Table.h"

Table::Table() {}

//for existing table
Table::Table(string name){
	_serial = 0;
	_name = name;
	string filename = _name + ".txt";
	fstream f;
	open_fileRW(f, filename.c_str());
	Record r;
	r.read(f, 0); //read from beginning
	_fields = r.get_record();
	for (int i = 0; i < _fields.size(); i++) {
		_field_map[_fields[i]] = i; //helps with mapping the indices
		/*
		example:
		Last name = 0
		First name = 1
		Age = 2
		..
		*/
	}

	reindex();
	f.close();
}

//make a new table with new fields
Table::Table(string name, const vector<string>& field_vals) {
	_serial = 0;
	_name = name;
	string filename = _name + ".txt";
	_fields = field_vals;
	fstream f;
	remove(filename.c_str());
	open_fileW(f, filename.c_str());
	Record r(field_vals);
	r.write(f);
	//map the field map, the indices
	for (int i = 0; i < field_vals.size(); i++) {
		_field_map[field_vals[i]] = i; //helps with mapping the indices
		/*
		example:
		Last name = 0
		First name = 1
		Age = 2
		..
		*/
	}

	f.close();

}
Table Table::select(const vector<string>& fields, const vector<string>& where) {
	if (fields.size() > 0 && fields[0] == "*" && where.size() == 0) {
		return select_all();
	} else if (where.size() == 0) {
		return select_simple(fields);
	} else if (where.size() > 0 && fields.size() > 1) { //meaning that there are conditions
		return select_where(fields, where);
	} else if (fields.size() == 1 && where.size() > 0) {
		return select_where(_fields, where);
	}
	return Table();
}

//gonna be using this with "*"
Table Table::select_all() {
	//open the original file that we want to select from 
	fstream f;
	string filename = _name + ".txt";
	open_fileRW(f, filename.c_str());// opens the original file

	//we're going to create a temp table
	string filename1 = _name + to_string(_serial++);
	Table temp(filename1, _fields);
	Record r;

	for (int i = 1; r.read(f, i) > 0; i++) {
		r.read(f, i);
		temp.insert_into(r.get_record());
	}
	f.close();
	return temp;
}

void Table::insert_into(vector<string> fields) {
	fstream f;
	string filename = _name + ".txt";
	open_fileRW(f, filename.c_str()); //open the file
	Record r(fields); //create record with vector of fields

	long record_num = r.append_file(f); //gets the position of where to start from (so we can append)

	for (int i = 0; i < fields.size(); i++) {
		//updates the index at which they are at
		/*
		So when you do _keywords[_fields[i]][r.get_record()[i]],
		you're first getting _keywords[_fields[i]] which is like _keywords["First"], so then you're obtaining
		the first name column first, which returns a MMap, then you want to index through that MMap which are
		full of the content. By doing the r.get_record[i], you're obtaining one portion of all the fields.
		Example: ["Vo", "James" , "19" , "100,000"]
		by indexing through r.get_record[i], you're modifying each thing and you're +='ing a record_num of where 
		it is located in the file. 
		*/
		_indices[_fields[i]][r.get_record()[i]] += record_num;
	}
	f.close();
}

void Table::reindex() {
	fstream f;
	f.open(_name + ".txt");
	Record r;
	for (long i = 1; r.read(f, i) > 0; i++) {
		r.read(f, i);
		vector<string> temp = r.get_record();
		for (int j = 0; j < _fields.size(); j++) {
			_indices[_fields[j]].insert(temp[j], i); //we will be inserting a record with it's given record number
			//this will be the one thing that will initialize the indices as well as be able to insert values
			//examples:
			//_indices["first"].insert("james",1)
			//_indices["last"].insert("vo",2)
		}
	}
	f.close();
}

int Table::get_record_serial() {
	return _serial;
}

vector<string> Table::get_fields() const {
	return _fields;
}


Table Table::select_simple(const vector<string>& fields) {
	fstream f;
	string actualfilename = _name + ".txt";
	open_fileRW(f, actualfilename.c_str());
	//open_fileRW(f, _name.c_str());

	//create a temporary table
	string filename = _name + to_string(_serial++);;
	Table temp(filename, fields);
	/*
	so theres two cases, theres a case where theres no where conditions
	example with no where: "SELECT last,first FROM employees"
	example with where: "SELECT last,first FROM employees WHERE last > lee AND first > johnson"
	*/
	//if theres no where, that means the vector is empty
	Record r;
	for (int i = 1; r.read(f, i) > 0; i++) {
		vector<string> row;
		r.read(f, i); //reads a block
		//wanting to get only certain values of fields and insert it into temp rows
		for (int j = 0; j < fields.size(); j++) {
			/*
			r.get_record() obtains a record
			now the _field_map is a map so by obtaining fields[i], we obtain
			one of the fields
			example:
			Last		First		Age		Salary
			Vo			James		19		100,000
			> select last from student
			this record above is the same thing as record[_field_map["Last"]] which will return the index of that field
			*/
			row.push_back(r.get_record()[_field_map[fields[j]]]);
		}
		temp.insert_into(row);
	}
	f.close();
	return temp;
}
Table Table::select_where(const vector<string>& fields, const vector<string>& conditions) {
	fstream f;
	string filename = _name + ".txt";
	open_fileRW(f, filename.c_str());
	//for where conditions
	//rpn<long> represents the values within the vector intersection
	shuntingyard sy;
	Queue<string> where_postfix = sy.infix_to_postfix(conditions);
	RPN<long> e(where_postfix, _indices);
	vector<long> where_indexes = e.evaluate(where_postfix);

	string filename1 = _name + to_string(_serial++);
	Table temp(filename1, fields); //initialize temporary field
	Record r;
	for (int i = 0; i < where_indexes.size(); i++) { //go up until the where_index.sizes
		r.read(f, where_indexes[i]); //read where the value of that string is at
		vector<string> temp_row;
		for (int j = 0; j < fields.size(); j++) {
			//cout << "Being pushed in: " << r.get_record()[_field_map[fields[j]]] << " ";
			temp_row.push_back(r.get_record()[_field_map[fields[j]]]); //push in a specific field each time into the vector of string
		}
		temp.insert_into(temp_row);
	}
	f.close();
	return temp;
}

