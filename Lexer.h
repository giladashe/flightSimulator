//
// Created by giladasher on 12/12/2019.
//

#ifndef FLIGHTSIMULATOR_LEXER_H
#define FLIGHTSIMULATOR_LEXER_H

#include <string>
#include <vector>

using namespace std;

class Lexer {
    vector<string> _functions;
public:
    vector<string> _arrayOfTokens;

    vector<string> makeTokensArray(ifstream &filePointer); //make array of tokens from a text file

    static vector<string> splitByDelimiter(string &s, const string &delimiter); //split string by a delimiter

    static bool isParentheses(char token); //checks if char it's '(' or ')'

    static bool isOperator(char token); //checks if char is an operator

    static void removeSpacesFromBegin(string &str);

    void classifyLine(string &str, string &firstWord, bool &ifOrWhile, bool &hadFunction, bool &isFunction, bool &PrintOrSleep);

    void insertAssignmentLine(vector<string> splitLine);

    void insertRegularLine(string &line, bool &ifOrWhile, bool &hadFunction, bool &isFunction, bool &PrintOrSleep);

    static bool togetherOperator(char first, char second);

    int insertVariableCommandAndFunc(string &line, int i);

    int insertNumber(string &line, int i);

    int insertInsideParentheses(string &line, int i, bool PrintOrSleep);
};

//
#endif //FLIGHTSIMULATOR_LEXER_H
