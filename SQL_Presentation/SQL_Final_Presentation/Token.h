#pragma once
#include <string>
#include <iostream>
using namespace std;
class Token {
public:
    Token();
    Token(string str, int type);
    friend ostream& operator <<(ostream& outs, const Token& t);
    int type();
    string type_string();
    string token_str();
private:
    string _token;
    int _type;
};
