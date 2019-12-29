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

    vector<string> makeTokensArray(ifstream &filePointer);

    static vector<string> splitByDelimiter(string &s, const string &delimiter);

    static bool isParentheses(char token);

    static bool isOperator(char token);

    static string convertToString(char *a, int size);
};

//
#endif //FLIGHTSIMULATOR_LEXER_H
