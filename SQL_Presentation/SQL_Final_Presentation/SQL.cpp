#include "SQL.h"


SQL::SQL() {
	get_all_tables();

}

void SQL::SQL_interactive() {
	string command;
	while (true) {
		cout << "> ";
		getline(cin, command);
		run_command(command);
	}
}

void SQL::run_file(const string& filename) {
	ifstream fin;
	fin.open(filename);
	string command;
	int j = 0;
	for (int i = 0; getline(fin, command); i++) {
		if (command.size() > 0 && (command.substr(0, 2) == "//" || command.at(0) == '#')) {
			cout << command << '\n';
		} else if (command.size() == 0 || command == "") {
			//skip and don't run ANYTHING
		} else {
			cout << j++ << ": " << command << '\n';
			run_command(command);
		}
		
	}
	fin.close();
}

//this runs a string command
bool SQL::run_command(const string& command) {
	p.set_string(command);
	MMap<string, string> ptree;
	bool parsed = p.parse_tree(ptree); //this parses the tree and puts into ptree mmap
	//cout << ptree << endl;
	//we want to check for if the user did a select command
	//cout << "\n\nTable: " << ptree["table_name"][0] << endl;
	if (ptree["commands"][0] == "select") {
		//this will get the ptree["table_name"][0], which is the table name, then it'll call that table name
		//and then call select it from there
		_tables["tables.txt"] = _tables[ptree["table_name"][0]].select(ptree["fields"], ptree["where"]);
		cout << _tables[ptree["table_name"][0]] << endl;
	}
	//handle the make/create
	else if (ptree["commands"][0] == "create" || ptree["commands"][0] == "make") {
		if (!_tables.contains(ptree["table_name"][0])) {
			ofstream out;
			out.open("tables.txt", ios::app);

			out << ptree["table_name"][0];
			out.close();
		}
		_tables[ptree["table_name"][0]] = Table(ptree["table_name"][0], ptree["fields"]);
		cout << _tables[ptree["table_name"][0]] << endl;
	}
	//handle the insert into
	else if (ptree["commands"][0] == "insert") {
		if (!_tables.contains(ptree["table_name"][0])) {
			cout << "\nThere is no table with this name.\n";
		} else {
			_tables[ptree["table_name"][0]].insert_into(ptree["values"]);
			cout << _tables[ptree["table_name"][0]] << endl;
		}
	}
	//exit the program
	else if (ptree["commands"][0] == "exit") {
		exit(0);
	}
	return false;
}

void SQL::get_all_tables() {
	//create the file if doesn't exist
	ofstream out;
	out.open("tables.txt", ios::app);
	out.close();

	ifstream fin;
	fin.open("tables.txt");
	string table_name;
	while (fin >> table_name) {
		_tables[table_name] = Table(table_name);
	}
	fin.close();
}

void SQL::print_tables() {
	int num = 1;
	for (Map<string, Table>::Iterator it = _tables.begin(); it != Map<string, Table>::Iterator(nullptr); it++) {
		cout << setw(20) << left << it.get_key();
		if (num > 0 && num % 4 == 0) {
			cout << '\n';
		}
	}
}
