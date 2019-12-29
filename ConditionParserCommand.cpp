//
// Created by giladasher on 25/12/2019.
//

#include "ConditionParserCommand.h"
#include "Command.h"
#include "Data.h"
#include "Lexer.h"

using namespace std;

ConditionParserCommand::ConditionParserCommand(const string &leftStr, const string &rightStr) : _leftStr(leftStr),
                                                                                                _rightStr(rightStr) {}

bool ConditionParserCommand::checkCondition(int index, vector<string> &lexer) {
    // update the variables values on "setVariables" at Interpreter
    Data::getInstance()->updateVariables(index, lexer);

    // find the operator
    int i = index;
    while ((lexer[i] != "!=") && (lexer[i] != "==")
           && (lexer[i] != ">=") && (lexer[i] != "<=")
           && (lexer[i] != ">") && (lexer[i] != "<")) {
        i++;
    }
    string op = lexer[i];

    // finds _leftStr & _rightStr of the condition
    int j = index;
    string sidesStr;
    while (lexer[j + 1] != "{") {
        sidesStr += lexer[j + 1];
        j++;
    }

    // split by the operator
    vector<string> sidesVector = Lexer::splitByDelimiter(sidesStr, op);
    this->_leftStr = sidesVector[0];
    this->_rightStr = sidesVector[1];

    // calculating both sides
    double leftStrVal = Data::getInstance()->calculate(this->_leftStr);
    double rightStrVal = Data::getInstance()->calculate(this->_rightStr);
    Expression *left = new Value(leftStrVal);
    Expression *right = new Value(rightStrVal);
    Expression *e = new Condition(left, right, op);
    bool result = (bool) (e->calculate());
    delete e;
    return result;
}

// calculates steps to the first command
int ConditionParserCommand::stepsToFirstCommand(int index, vector<string> &lexer) {
    int i = index;
    int steps = 0;
    while (lexer[i] != "\n") {
        i++;
        steps++;
    }
    return steps + 1;
}

// calculates steps out of the scope
int ConditionParserCommand::stepOutOfTheScope(int index, vector<string> &lexer) {
    int i = index;
    int steps = 0;
    while (lexer[i] != "}") {
        i++;
        steps++;
    }
    return steps + 2;
}