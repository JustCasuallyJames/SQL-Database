#include "../headers/string_utility.h"

string uppercase(string word) {
	string result = word;
	for (int i = 0; i < word.size(); i++) {
		result[i] = toupper(word[i]);
	}
	return result;
}

string lowercase(string word) {
	string result = word;
	for (int i = 0; i < word.size(); i++) {
		result[i] = tolower(word[i]);
	}
	return result;
}
