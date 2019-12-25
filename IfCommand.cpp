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

    if ((bool) this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            Command *command = (Data::getInstance()->getCommandMap().find(lexer[i]))->second;
            if (command != nullptr) {
                i += command->execute(i, lexer);
            }
                // assignmentCommand
            else {
                Command *assignmentCommand = Data::getInstance()->getCommandMap().find("assign")->second;
                i += assignmentCommand->execute(i, lexer);
            }
            //TODO FREE ALL COMMANDS
        }
    }
    return stepOutOfTheScope;
}