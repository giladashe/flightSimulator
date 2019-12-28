//
// Created by giladasher on 25/12/2019.
//

#include "IfCommand.h"
#include "Data.h"

IfCommand::IfCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}

int IfCommand::execute(int index, vector <string> &lexer) {
    int stepsToFirstCommand;
    stepsToFirstCommand = this->stepsToFirstCommand(index, lexer);
    int i = index + stepsToFirstCommand;
    int stepOutOfTheScope;
    stepOutOfTheScope = this->stepOutOfTheScope(index, lexer);
    auto data = Data::getInstance();
    if (this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            Command *command = (Command*)data->getCommandMap(lexer[i]);
            if (command != nullptr) {
                i += command->execute(i, lexer);
            }
                // assignmentCommand
            else {
                Command *assignmentCommand = (Command*)data->getCommandMap("assign");
                i += assignmentCommand->execute(i, lexer);
            }
        }
    }
    return stepOutOfTheScope;
}