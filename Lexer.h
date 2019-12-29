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
    vector<string> _arrayOfTokens;

    vector<string> makeTokensArray(ifstream &filePointer); //make array of tokens from a text file

    static vector<string> splitByDelimiter(string &s, const string &delimiter); //split string by a delimiter

    static bool isParentheses(char token); //checks if char it's '(' or ')'

    static bool isOperator(char token); //checks if char is an operator

};

//
#endif //FLIGHTSIMULATOR_LEXER_H
