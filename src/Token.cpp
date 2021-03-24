#include "../headers/Token.h"

Token::Token() {
    _token = "";
    _type = 0;
}

Token::Token(string str, int type) {
    _token = str;
    _type = type;
}

ostream& operator<<(ostream& outs, const Token& t) {
    outs << "{" << t._token << "}" << endl;
    return outs;
}

int Token::type() {
    return _type;
}

string Token::type_string() {
    switch (_type) {
    case 1:
        return "ALPHA";
    case 2:
    case 4:
        return "NUMBER";
    case 5:
        return "PUNCTUATION";
    case 6:
        return "SPACE";
    default:
        return "SPECIAL";
    }
}

string Token::token_str() {
    return _token;
}
