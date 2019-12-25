//
// Created by giladasher on 25/12/2019.
//

#include "ConditionParserCommand.h"
#include "Command.h"
#include "Data.h"
#include "Lexer.h"

using namespace std;

ConditionParserCommand::ConditionParserCommand(const string &leftStr, const string &rightStr) : leftStr(leftStr),
                                                                                                rightStr(rightStr) {}

int ConditionParserCommand::execute(int index, vector<string> &lexer) {}

const list<Command> &ConditionParserCommand::getCommandList() const {
    return this->commandList;
}

bool ConditionParserCommand::checkCondition(int index, vector<string> &lexer) {
    Data::getInstance()->updateVariables(index, lexer);

    // find the operator
    int i = index;
    while ((lexer[i] != "!=") && (lexer[i] != "==")
           && (lexer[i] != ">=") && (lexer[i] != "<=")
           && (lexer[i] != ">") && (lexer[i] != "<")) {
        i++;
    }
    string op = lexer[i];

    // finds leftStr & rightStr of the condition
    int j = index;
    string sidesStr;
    while (lexer[j + 1] != "{") {
        sidesStr += lexer[j + 1];
        j++;
    }
    vector<string> sidesVector = Lexer::splitByDelimiter(sidesStr, op);
    this->leftStr = sidesVector[0];
    this->rightStr = sidesVector[1];

    double leftStrVal = Data::getInstance()->calculate(this->leftStr);
    double rightStrVal = Data::getInstance()->calculate(this->rightStr);
    Expression *left = new Value(leftStrVal);
    Expression *right = new Value(rightStrVal);
    Expression *e = new Condition(left, right, op);
    bool result = (bool) (e->calculate());
    return result;
}

int ConditionParserCommand::stepsToFirstCommand(int index, vector<string> &lexer) {
    int i = index;
    int steps = 0;
    while (lexer[i] != "\n") {
        i++;
        steps++;
    }
    return steps + 1;
}

int ConditionParserCommand::stepOutOfTheScope(int index, vector<string> &lexer) {
    int i = index;
    int steps = 0;
    while (lexer[i] != "}") {
        i++;
        steps++;
    }
    return steps + 2;
}