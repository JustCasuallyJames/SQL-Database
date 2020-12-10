#include "SQL.h"


SQL::SQL() {
	queryNum = 0;
	print_welcome();
	get_all_tables();

}

SQL::~SQL() {
	//fstream f;
	//open_fileW(f, "tables.txt");
	//int j = 0;
	//for (auto it = _table_names.begin(); it != _table_names.end(); ++it, ++j) { //used auto to make it easier
	//	f << it[j] << '\n';
	//}
	//f.close();
}

void SQL::SQL_interactive() {
	string command;
	while (true) {
		cout << "[" << queryNum++ << "] "<<"> ";
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
			cout << "[" << j++ << "] " << " " << command << '\n';
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
	//we want to check for if the user did a select command
	if (parsed) {
		string table_name = ptree["table_name"][0]; //represents the table name
		string first_command = ptree["commands"][0]; //represents the first token within the command string
		vector<string> fields = ptree["fields"];
		vector<string> values = ptree["values"];
		vector<string> conditions = ptree["where"];
		
		if (first_command == "select") {
			//this will get the ptree["table_name"][0], which is the table name, then it'll call that table name
			//and then call select it from there
			if (!_tables.contains(table_name)) {
				cout << "\nError [Select]: No associated table with that name.\n";
			} 
			else if (fields.size() > _tables[table_name].get_fields().size()) { //If the parser reads more fields than the given fields, then
				cout << "\nError [Select]: Exceeded max number of fields\n";
			}
			else {
				Table specificTable = _tables[table_name].select(fields, conditions);
				_tables["tables.txt"] = specificTable;
				cout << specificTable << endl;
			}
		}
		//handle the make/create
		else if (first_command == "create" || first_command == "make") {
			if (!_tables.contains(table_name)) {
				ofstream out;
				out.open("tables.txt", ios::app);

				out << table_name << endl;
				out.close();
				//saves it into the map of tables
				_tables[table_name] = Table(table_name, fields);
				//push back the table_name into the table_names string vector
				_table_names.push_back(table_name);
				cout << _tables[table_name] << endl;

			} else {
				cout << "\nError [Create|Make]: There is already a table with this name.\n";
			}
		}
		//handle the insert into
		else if (first_command == "insert") {
			if (!_tables.contains(table_name)) {
				cout << "\nThere is no table with this name.\n";
			} else {
				_tables[table_name].insert_into(values);
				cout << _tables[table_name] << endl;
			}
		} 
		else if (first_command == "batch") {
			run_file(table_name);
			cout << "\n\n-------------------------------BATCH PROCESS FINISHED-------------------------------\n\n";
		} 
		else if (first_command == "drop") {
			if (_tables.contains(table_name)) {
				std::vector<string>::iterator current_pos = find(_table_names.begin(), _table_names.end(), table_name);
				if (current_pos != _table_names.end()) {
					_table_names.erase(current_pos);
					_tables.erase(table_name);
					cout << "\nDropped: " << table_name << endl;
				}
				
				fstream f;
				open_fileW(f, "tables.txt");
				int j = 0;
				for (auto it = _table_names.begin(); it != _table_names.end(); it++) {
					f << it[j] << '\n';
				}
				f.close();

			} else {
				cout << "Error [Drop]: Could not find table_name: " << table_name << endl;
			}
		}
		
		cout << "\nSQL: DONE!\n";
		return true;
	}else if (command == "exit") {//exit the program
		cout << "\n\nThanks for running my program. The program will now terminate.\n\n";
	} else if (command == "tables") {
		cout << endl;
		print_table_list();
		cout << "\nSQL: DONE!\n";
		return true;
	} else {
		cout << "\nInvalid Command.\n";
		return true;
	}
	
	exit(0);
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
	print_table_list();
	while (fin >> table_name) {
		_tables[table_name] = Table(table_name);
		_table_names.push_back(table_name);
	}
	fin.close();
}

void SQL::print_tables() {
	int num = 1;
	for (Map<string, Table>::Iterator it = _tables.begin(); it != _tables.end(); it++) {
		cout << setw(20) << left << it.get_key();
		if (num > 0 && num % 4 == 0) {
			cout << '\n';
		}
	}
}

void SQL::print_table_list() {
	ifstream fin;
	fin.open("tables.txt");
	string table_name;
	/*
		----------------------------
		| List of available tables |
		----------------------------
		1. employees
		2. students

	*/
	int numTables = 1;
	cout << "----------------------------------------\n";
	cout << "| List of available tables	       |\n";
	cout << "| Type \"tables\" for a list of tables   |\n";
	cout << "----------------------------------------\n";	

	//for (auto it = _table_names.begin(); it != _table_names.end(); it++) {
	//	cout << numTables++ << ". " << table_name << endl;
	//}
	while (fin >> table_name) {
		cout << numTables++ << ". " << table_name << endl;
	}

	cout << endl << endl;
	fin.close();
}

void SQL::print_welcome() {
	/*
		-------------------------------------------------------------
		|				  Welcome to James's Program				|
		|															|
		|		CS8: SQL Project				Fall 2020			|
		|		Version 1.0											|
		-------------------------------------------------------------

	*/
	cout << "-----------------------------------------------------------------\n";
	cout << "|		   Welcome to James's Program			|\n";
	cout << "|								|\n";
	cout << "|		CS8: SQL Project	Fall 2020		|\n";
	cout << "|								|\n";
	cout << "|		Version 1.0					|\n";
	cout << "-----------------------------------------------------------------\n";
}
