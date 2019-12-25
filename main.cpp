#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "VarData.h"
#include "Parser.h"


using namespace std;
int main(int argc, char *argv[]){
    Lexer lexer;
    ifstream filePointer;
    filePointer.open(argv[1],std::ifstream::in);
    vector<string> arrayOfTokens = lexer.lexer(filePointer);
    Parser parser(arrayOfTokens);
    parser.parse();

    return 0;
}
//