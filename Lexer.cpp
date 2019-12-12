//
// Created by giladasher on 12/12/2019.
//
#include <fstream>
#include <regex>
#include "Lexer.h"
#include <iostream>

using namespace std;

vector<string> Lexer::lexer(ifstream &filePointer) {
    vector<string> arrayOfTokens = {};
    if (!filePointer.is_open()) {
        cout << "Error opening file" << endl;
        return arrayOfTokens;
    }
    string line;
    string token;
    string inParentheses;
    //flag if the operators need to be together like >=
    bool together = false;
    while (getline(filePointer, line)) {
        string copyOfLine = string(line);
        string big = string(line);
        string small = string(line);
        //splits by '=' || '>' || '<'
        vector<string> splitLine = this->splitByDelimiter(copyOfLine, "=");
        vector<string> splitLineBig = this->splitByDelimiter(big, ">");
        vector<string> splitLineSmall = this->splitByDelimiter(small, "<");

        //if it's a line like: variable = value
        if (splitLine.size() > 1 && splitLineBig.size() == 1 && splitLineSmall.size() == 1) {
            for (int i = 0; i < splitLine.size(); i++) {
                //erase spaces - https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
                splitLine[i].erase(remove(splitLine[i].begin(), splitLine[i].end(), ' '), splitLine[i].end());
                splitLine[i].erase(remove(splitLine[i].begin(), splitLine[i].end(), '\t'), splitLine[i].end());
                //if it's the variable name
                if (i == 0) {
                    arrayOfTokens.push_back(splitLine[i]);
                    arrayOfTokens.emplace_back("=");
                } else {
                    for (int j = 0; j < splitLine[i].size(); j++) {
                        if (isParentheses(splitLine[i][j]) || isOperator(splitLine[i][j])) {
                            arrayOfTokens.emplace_back(string(1, splitLine[i][j]));
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
                        arrayOfTokens.push_back(token);
                    }
                }
            }
        } else {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ' || line[i] == ')' || line[i] == '\t') {
                    continue;
                } else if (isalpha(line[i]) || line[i] == '_') {
                    token = string(1, line[i]);
                    i++;
                    while (i < (int) line.length() && !isParentheses(line[i]) && !isOperator(line[i]) &&
                           line[i] != ' ') {
                        token += line[i];
                        i++;
                    }
                    i--;
                    arrayOfTokens.push_back(token);
                } else if (line[i] == '(') {
                    i++;
                    inParentheses = string(1, line[i]);
                    i++;
                    while (i < (int) line.length() && !isParentheses(line[i])) {
                        inParentheses += line[i];
                        i++;
                    }
                    //except of "print" all expressions in () need to be without spaces
                    if (arrayOfTokens.back() != "Print") {
                        inParentheses.erase(remove(inParentheses.begin(), inParentheses.end(), ' '),
                                            inParentheses.end());
                    }
                    vector<string> paren = splitByDelimiter(inParentheses, ",");
                    arrayOfTokens.insert(arrayOfTokens.end(), paren.begin(), paren.end());
                    i--;
                } else if (isOperator(line[i])) {
                    token = string(1, line[i]);

                    //all possible operators that needs to be together: -> , <- , <= , >=
                    if ((line[i] == '-' && line[i + 1] == '>') ||
                        (line[i] == '<' && (line[i + 1] == '-' || line[i + 1] == '='))
                        || (line[i] == '>' && line[i + 1] == '=')) {
                        together = true;
                    }
                    if (together) {
                        token += line[i + 1];
                        i++;
                        together = false;
                    }
                    arrayOfTokens.push_back(token);
                } else if (line[i] == '{' || line[i] == '}') {
                    arrayOfTokens.emplace_back(string(1, line[i]));
                } else { //it's a number
                    token = string(1, line[i]);
                    i++;
                    while (i < (int) line.length() && (line[i] == '.' || isdigit(line[i]))) {
                        token += line[i];
                        i++;
                    }
                    i--;
                    arrayOfTokens.push_back(token);
                }
            }
        }
        arrayOfTokens.emplace_back("\n");
    }

    filePointer.close();

    return arrayOfTokens;
}

//split a string by delimiter
vector<string> Lexer::splitByDelimiter(string &s, const string &delimiter) {
    //based on https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    size_t pos = 0;
    string token;
    vector<string> splitString;
    while ((pos = s.find(delimiter)) != std::string::npos) {
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
    //# is a symbol for UPlus and $ is a symbol for UMinus
    return token == '+' || token == '-' || token == '*' || token == '/' || token == '<' || token == '>' || token == '=';
}
