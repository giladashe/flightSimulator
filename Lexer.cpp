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
    string token;
    string inParentheses;
    vector<string> functions;
    //flag if the operators need to be together like >=
    bool together = false;
    bool ifOrWhileLine = false;
    bool hadThisFunction = false;
    bool isFunction = false;
    while (getline(filePointer, line)) {
        //remove tabs from all the line
        line.erase(remove(line.begin(), line.end(), '\t'), line.end());
        //remove spaces from begin
        for (int t = 0; t < (int)line.length(); t++) {
            if (line[t] == ' ') {
                line = line.substr(1, line.length() - 1);
            } else {
                break;
            }
        }
        if (line.empty()) {
            continue;
        }
        string copyOfLine = string(line);
        string big = string(line);
        string small = string(line);
        //splits by '=' || '>' || '<'
        vector<string> splitLine = Lexer::splitByDelimiter(copyOfLine, "=");
        vector<string> splitLineBig = Lexer::splitByDelimiter(big, ">");
        vector<string> splitLineSmall = Lexer::splitByDelimiter(small, "<");

        token.clear();
        //checks if line is a Function declaration/if line/while line
        for (int r = 0; r < (int)line.length(); r++) {
            if (isOperator(line[r]) || line[r] == '!') {
                break;
            }
            if (line[r] == '(') {
                token.erase(remove(token.begin(), token.end(), ' '), token.end());
                if (token != "if" && token != "while" && token != "Print" && token != "Sleep"
                    && token != "openDataServer" && token != "connectControlClient") {
                    //if the function is already declared than you proceed as usual
                    for (const auto& function:functions) {
                        if (function == token) {
                            hadThisFunction = true;
                            break;
                        }
                    }
                    //it's a function declaration
                    if(!hadThisFunction) {
                        isFunction = true;
                        functions.push_back(token);
                    }
                }
                break;
            }
            token += line[r];
            //if it's an if line
            if (token.length() == 2) {
                if (token == "if") {
                    ifOrWhileLine = true;
                    line = line.substr(3, line.length() - 3);
                    _arrayOfTokens.push_back(token);
                    break;
                }
            } else if (token.length() == 5) {
                //if it's a while line
                if (token == "while") {
                    line = line.substr(6, line.length() - 6);
                    ifOrWhileLine = true;
                    _arrayOfTokens.push_back(token);
                    break;
                    //if it's print or sleep you proceed as usual
                } else if (token == "Print" || token == "Sleep") {
                    break;
                }
            }
        }


        //if it's a line like: variable = value
        if (splitLine.size() > 1 && splitLineBig.size() == 1 && splitLineSmall.size() == 1 && !ifOrWhileLine) {
            for (int i = 0; i < (int)splitLine.size(); i++) {
                if (i == 0) {
                    while (splitLine[i][0] == ' ') {
                        splitLine[i] = splitLine[i].substr(1, splitLine[i].length() - 1);
                    }
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
                    for (int j = 0; j < (int)splitLine[i].length(); j++) {
                        if (isParentheses(splitLine[i][j]) || isOperator(splitLine[i][j])) {
                            _arrayOfTokens.emplace_back(string(1, splitLine[i][j]));
                            continue;
                            //it's a variable or a command
                        } else if (isalpha(splitLine[i][j]) || splitLine[i][j] == '_') {
                            token = string(1, splitLine[i][j]);
                            j++;
                            while (j < (int) splitLine[i].length() && !isParentheses(splitLine[i][j]) &&
                                   !isOperator(splitLine[i][j])) {
                                token += splitLine[i][j];
                                j++;
                            }
                            j--;
                            //if it's a number you take all the numbers / '.' after it together
                        } else if (isdigit(splitLine[i][j])) {
                            token = string(1, splitLine[i][j]);
                            j++;
                            while (j < (int) splitLine[i].length() &&
                                   (splitLine[i][j] == '.' || isdigit(splitLine[i][j]))) {
                                token += splitLine[i][j];
                                j++;
                            }
                            j--;
                        }
                        _arrayOfTokens.push_back(token);
                    }
                }
            }
        }

            //the rest (print,sleep,if,while,openDataServer,connectControlClient,functions)
        else {
            for (int i = 0; i < (int)line.length(); i++) {
                if (line[i] == ' ') {
                    continue;
                } else if (line[i] == ')') {
                    //insert '(' if it's a function or while or if
                    if (!ifOrWhileLine && !isFunction) {
                        continue;
                    }
                    _arrayOfTokens.emplace_back(string(1, line[i]));
                    //insert a variable
                } else if (isalpha(line[i]) || line[i] == '_') {
                    token = string(1, line[i]);
                    i++;
                    while (i < (int) line.length() && !isParentheses(line[i]) && !isOperator(line[i]) &&
                           line[i] != ' ') {
                        token += line[i];
                        i++;
                    }
                    i--;
                    _arrayOfTokens.push_back(token);
                } else if (line[i] == '(') {
                    //insert '(' if it's function/if/while line
                    if (ifOrWhileLine || isFunction) {
                        _arrayOfTokens.emplace_back(string(1, line[i]));
                        continue;
                    }
                    i++;
                    inParentheses = string(1, line[i]);
                    i++;
                    while (i < (int)line.length()) {
                        inParentheses += line[i];
                        i++;
                    }
                    inParentheses = inParentheses.substr(0, inParentheses.length() - 1);
                    //you have now what's inside the parenthesis
                    string copyOfInParen = string(inParentheses);
                    //split by ','
                    vector<string> paren = splitByDelimiter(inParentheses, ",");
                    //if it's connectcontrol client (has ',')
                    if (paren.size() > 1) {
                        inParentheses.erase(remove(copyOfInParen.begin(), copyOfInParen.end(), ' '),
                                            copyOfInParen.end());
                        paren = splitByDelimiter(copyOfInParen, ",");
                        for (const auto &obj:paren) {
                            _arrayOfTokens.push_back(obj);
                        }
                        break;
                    }
                    string copy2OfInParen = string(copyOfInParen);
                    //split by "
                    vector<string> quotationMarks = splitByDelimiter(copyOfInParen, "\"");
                    //except of "print" all expressions in () need to be without spaces
                    if (quotationMarks.size() == 1) {
                        inParentheses.erase(remove(copy2OfInParen.begin(), copy2OfInParen.end(), ' '),
                                            copy2OfInParen.end());
                    }
                    _arrayOfTokens.push_back(copy2OfInParen);
                    i--;
                    //put opertor in the array
                } else if (isOperator(line[i]) || line[i] == '!') {
                    token = string(1, line[i]);

                    //all possible operators that need to be together: -> , <- , <= , >=
                    if ((line[i] == '-' && line[i + 1] == '>') ||
                        (line[i] == '<' && (line[i + 1] == '-' || line[i + 1] == '='))
                        || (line[i] == '>' && line[i + 1] == '=') || (line[i] == '=' && line[i + 1] == '=')
                        || (line[i] == '!' && line[i + 1] == '=')) {
                        together = true;
                    }
                    if (together) {
                        token += line[i + 1];
                        i++;
                        together = false;
                    }
                    _arrayOfTokens.push_back(token);
                    //put { and } in the array
                } else if (line[i] == '{' || line[i] == '}') {
                    _arrayOfTokens.emplace_back(string(1, line[i]));
                } else { //put a number in array
                    token = string(1, line[i]);
                    i++;
                    while (i < (int) line.length() && (line[i] == '.' || isdigit(line[i]))) {
                        token += line[i];
                        i++;
                    }
                    i--;
                    _arrayOfTokens.push_back(token);
                }
            }
            ifOrWhileLine = false;
            isFunction = false;
            hadThisFunction = false;
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
    string token;
    vector<string> splitString;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        splitString.push_back(token);
    }
    splitString.push_back(s);
    return splitString;
}

bool Lexer::isParentheses(char token) {
    return token == '(' || token == ')';
}

bool Lexer::isOperator(char token) {
    return token == '+' || token == '-' || token == '*' || token == '/' || token == '<' || token == '>' || token == '=';
}