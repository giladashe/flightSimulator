//
// Created by giladasher on 12/12/2019.
//
#include <fstream>
#include <regex>
#include "Lexer.h"
#include <iostream>

using namespace std;

vector<string> Lexer::makeTokensArray(ifstream &filePointer) {
    if (!filePointer.is_open()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    string line;
    string firstWord;
    //flag if the operators need to be together like >=
    bool ifOrWhileLine = false;
    bool hadThisFunction = false;
    bool isFunction = false;
    bool isPrintOrSleep = false;
    while (getline(filePointer, line)) {
        //remove tabs from all the line
        line.erase(remove(line.begin(), line.end(), '\t'), line.end());
        //remove spaces from begin
        Lexer::removeSpacesFromBegin(line);
        if (line.empty()) {
            continue;
        }

        //splits by '=' || '>' || '<'
        vector<string> splitLine = Lexer::splitByDelimiter(line, "=");
        vector<string> splitLineBig = Lexer::splitByDelimiter(line, ">");
        vector<string> splitLineSmall = Lexer::splitByDelimiter(line, "<");


        //checks if line is a Function declaration/if line/while line
        this->classifyLine(line, firstWord, ifOrWhileLine, hadThisFunction, isFunction, isPrintOrSleep);

        //if it's a line like: variable = value
        if (splitLine.size() > 1 && splitLineBig.size() == 1 && splitLineSmall.size() == 1 && !ifOrWhileLine &&
            !isPrintOrSleep) {
            this->insertAssignmentLine(splitLine);
        }
            //the rest (print,sleep,if,while,openDataServer,connectControlClient,functions)
        else {
            Lexer::insertRegularLine(line, ifOrWhileLine, hadThisFunction, isFunction, isPrintOrSleep);
        }
        //put \n to know that it's another line
        _arrayOfTokens.emplace_back("\n");
    }
    return _arrayOfTokens;
}

//split a string by a delimiter
vector<string> Lexer::splitByDelimiter(string &s, const string &delimiter) {
    //based on https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    size_t pos = 0;
    string copyOfStr = string(s);
    string token;
    vector<string> splitString;
    while ((pos = copyOfStr.find(delimiter)) != string::npos) {
        token = copyOfStr.substr(0, pos);
        copyOfStr.erase(0, pos + delimiter.length());
        splitString.push_back(token);
    }
    splitString.push_back(copyOfStr);
    return splitString;
}

bool Lexer::isParentheses(char token) {
    return token == '(' || token == ')';
}

bool Lexer::isOperator(char token) {
    return token == '+' || token == '-' || token == '*' || token == '/' || token == '<' || token == '>' || token == '=';
}

void Lexer::removeSpacesFromBegin(string &str) {
    int i = 0;
    while (i < (int)str.length()) {
        if (str[0] == ' ') {
            str = str.substr(1, str.length() - 1);
        } else {
            break;
        }
        i++;
    }
}

void Lexer::classifyLine(string &str, string &firstWord, bool &ifOrWhile, bool &hadFunction, bool &isFunction,
                         bool &PrintOrSleep) {
    for (int r = 0; r < (int) str.length(); r++) {
        if (isOperator(str[r]) || str[r] == '!') {
            break;
        }
        if (str[r] == '(') {
            firstWord.erase(remove(firstWord.begin(), firstWord.end(), ' '), firstWord.end());
            if (firstWord != "if" && firstWord != "while" && firstWord != "Print" && firstWord != "Sleep"
                && firstWord != "openDataServer" && firstWord != "connectControlClient") {
                //if the function is already declared than you proceed as usual
                for (const auto &function:_functions) {
                    if (function == firstWord) {
                        hadFunction = true;
                        break;
                    }
                }
                //it's a function declaration
                if (!hadFunction) {
                    isFunction = true;
                    this->_functions.push_back(firstWord);
                }
            }
            break;
        }
        firstWord += str[r];
        //if it's an if line
        if (firstWord.length() == 2) {
            if (firstWord == "if") {
                ifOrWhile = true;
                str = str.substr(3, str.length() - 3);
                this->_arrayOfTokens.push_back(firstWord);
                break;
            }
        } else if (firstWord.length() == 5) {
            //if it's a while line
            if (firstWord == "while") {
                str = str.substr(6, str.length() - 6);
                ifOrWhile = true;
                this->_arrayOfTokens.push_back(firstWord);
                break;
                //if it's print or sleep you proceed as usual
            } else if (firstWord == "Print" || firstWord == "Sleep") {
                PrintOrSleep = true;
                break;
            }
        }
    }
    firstWord.clear();
}


void Lexer::insertAssignmentLine(vector<string> splitLine) {
    string token;
    for (int i = 0; i < (int) splitLine.size(); i++) {
        if (i == 0) {
            Lexer::removeSpacesFromBegin(splitLine[i]);
            //insert "var" as a token to the array
            if (splitLine[i][0] == 'v') {
                token = splitLine[i][0];
                token += splitLine[i][1];
                token += splitLine[i][2];
                _arrayOfTokens.push_back(token);
                splitLine[i] = splitLine[i].substr(3, splitLine[i].length() - 3);
            }
        }

        //erase spaces - https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
        splitLine[i].erase(remove(splitLine[i].begin(), splitLine[i].end(), ' '), splitLine[i].end());
        //if it's the variable name
        if (i == 0) {
            _arrayOfTokens.push_back(splitLine[i]);
            _arrayOfTokens.emplace_back("=");
        } else {
            //go over the complex expression on the right
            for (int j = 0; j < (int) splitLine[i].length(); j++) {
                if (isParentheses(splitLine[i][j]) || isOperator(splitLine[i][j])) {
                    _arrayOfTokens.emplace_back(string(1, splitLine[i][j]));
                    continue;
                    //it's a variable or a command
                } else if (isalpha(splitLine[i][j]) || splitLine[i][j] == '_') {
                    j = insertVariableCommandAndFunc(splitLine[i], j);
                    //if it's a number you take all the numbers / '.' after it together
                } else if (isdigit(splitLine[i][j])) {
                    j = insertNumber(splitLine[i], j);
                }
            }
        }
    }
}

void Lexer::insertRegularLine(string &line, bool &ifOrWhile, bool &hadFunction, bool &isFunction,
                              bool &PrintOrSleep) {
    string token;
    string inParentheses;
    for (int i = 0; i < (int) line.length(); i++) {
        token.clear();
        if (line[i] == ' ') {
            continue;
        } else if (line[i] == ')') {
            //insert '(' if it's a function or while or if
            if (!ifOrWhile && !isFunction) {
                continue;
            }
            _arrayOfTokens.emplace_back(string(1, line[i]));
            //insert a variable/function name
        } else if (isalpha(line[i]) || line[i] == '_') {
            i = insertVariableCommandAndFunc(line, i);
        } else if (line[i] == '(') {
            //insert '(' if it's function/if/while line
            if (ifOrWhile || isFunction) {
                _arrayOfTokens.emplace_back(string(1, line[i]));
                continue;
            }
            i++;
            i=insertInsideParentheses(line, i, PrintOrSleep);
            //put operator in the array
        } else if (isOperator(line[i]) || line[i] == '!') {
            token += line[i];

            //all possible operators that need to be together: -> , <- , <= , >=, ==, !=
            if (togetherOperator(line[i], line[i + 1])) {
                token += line[i + 1];
                i++;
            }
            _arrayOfTokens.push_back(token);
            //put { and } in the array
        } else if (line[i] == '{' || line[i] == '}') {
            _arrayOfTokens.emplace_back(string(1, line[i]));
        } else { //put a number in array
            i = insertNumber(line, i);
        }
    }
    ifOrWhile = false;
    isFunction = false;
    hadFunction = false;
    PrintOrSleep = false;
}

bool Lexer::togetherOperator(char first, char second) {
    return (first == '-' && second == '>') ||
           (first == '<' && (second == '-' || second == '='))
           || (first == '>' && second == '=') || (first == '=' && second == '=')
           || (first == '!' && second == '=');
}

int Lexer::insertVariableCommandAndFunc(string &line, int i) {
    string token = string(1, line[i]);
    i++;
    while (i < (int) line.length() && !isParentheses(line[i]) && !isOperator(line[i]) &&
           line[i] != ' ' && line[i] != '{' && line[i] != '}') {
        token += line[i];
        i++;
    }
    i--;
    _arrayOfTokens.push_back(token);
    return i;
}

int Lexer::insertNumber(string &line, int i) {
    string token = string(1, line[i]);
    i++;
    while (i < (int) line.length() && (line[i] == '.' || isdigit(line[i]))) {
        token += line[i];
        i++;
    }
    i--;
    _arrayOfTokens.push_back(token);
    return i;
}

int Lexer::insertInsideParentheses(string &line, int i, bool PrintOrSleep) {
    string inParentheses = string(1, line[i]);
    i++;
    while (i < (int) line.length()) {
        inParentheses += line[i];
        i++;
    }
    inParentheses = inParentheses.substr(0, inParentheses.length() - 1);
    //you have now what's inside the parenthesis
    //split by ','
    if (!PrintOrSleep) {
        vector<string> paren = splitByDelimiter(inParentheses, ",");
        //if it's connectcontrol client (has ',')
        if (paren.size() > 1) {
            inParentheses.erase(remove(inParentheses.begin(), inParentheses.end(), ' '),
                                inParentheses.end());
            paren = splitByDelimiter(inParentheses, ",");
            for (const auto &obj:paren) {
                _arrayOfTokens.push_back(obj);
            }
            return i;
        }
    }
    //split by "
    vector<string> quotationMarks = splitByDelimiter(inParentheses, "\"");
    //except of "print" all expressions in () need to be without spaces
    if (quotationMarks.size() == 1) {
        inParentheses.erase(remove(inParentheses.begin(), inParentheses.end(), ' '),
                            inParentheses.end());
    }
    _arrayOfTokens.push_back(inParentheses);
    i--;
    return i;
}