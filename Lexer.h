//
// Created by giladasher on 12/12/2019.
//

#ifndef FLIGHTSIMULATOR_LEXER_H
#define FLIGHTSIMULATOR_LEXER_H
#include <string>
#include <vector>
using namespace std;

class Lexer {
public:
    vector<string> lexer(ifstream & filePointer);
    vector<string> splitByDelimiter(string &s, const string & delimiter);

    bool isParentheses(char token);

    bool isOperator(char token);
};


#endif //FLIGHTSIMULATOR_LEXER_H