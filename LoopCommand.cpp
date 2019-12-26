//
// Created by giladasher on 25/12/2019.
//

#include "LoopCommand.h"
#include "Data.h"

LoopCommand::LoopCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}

int LoopCommand::execute(int index, vector <string> &lexer) {
    int stepsToFirstCommand;
    stepsToFirstCommand = this->stepsToFirstCommand(index, lexer);
    int i = index + stepsToFirstCommand;
    int stepOutOfTheScope;
    stepOutOfTheScope = this->stepOutOfTheScope(index, lexer);
    auto data = Data::getInstance();
    while (this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            Command *command = (Command *)data->getCommandMap(lexer[i]);
            if (command != nullptr) {
                    i += command->execute(i, lexer);
            }
                // assignmentCommand
            else {
                Command *assignmentCommand = (Command*)data->getCommandMap("assign");
                i += assignmentCommand->execute(i, lexer);
            }
        }
        i = index + stepsToFirstCommand;
    }
    return stepOutOfTheScope;
}