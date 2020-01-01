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

    static void removeSpacesFromBegin(string &str); //removes spaces from the begin of the line

    //classify line to assignment line or other
    void classifyLine(string &str, string &firstWord, bool &ifOrWhile, bool &hadFunction, bool &isFunction, bool &PrintOrSleep);

    void insertAssignmentLine(vector<string> splitLine); //insert assignment line to array of tokens

    void insertRegularLine(string &line, bool &ifOrWhile, bool &hadFunction, bool &isFunction, bool &PrintOrSleep);

    static bool togetherOperator(char first, char second); //if operator's size is 2 (like >=) returns true

    int insertVariableCommandAndFunc(string &line, int i); //insert "word" like to the array of tokens

    int insertNumber(string &line, int i); // insert numner to array of tokens

    int insertInsideParentheses(string &line, int i, bool PrintOrSleep); //insert the expression inside the parenthesis
};

//
#endif //FLIGHTSIMULATOR_LEXER_H
