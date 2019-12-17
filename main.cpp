#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Lexer.h"
#include "VarData.h"
#include "Command.h"
#include "Parser.h"

using namespace std;
int main(){
    Lexer lexer;
    ifstream filePointer;
    filePointer.open("/home/shaiyis/CLionProjects/flightSimulator/fly.txt",std::ifstream::in);
    vector<string> arrayOfTokens = lexer.lexer(filePointer);

    Parser parser(arrayOfTokens);
    parser.parse();

    return 0;
}
