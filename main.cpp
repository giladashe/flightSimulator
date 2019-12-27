#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "VarData.h"
#include "Parser.h"


using namespace std;

int main(int argc, char *argv[]) {
    Lexer lexer;
    ifstream filePointer;
    if (argc == 1) {
        cerr << "File Name Not Provided" << endl;
        exit(1);
    }
    filePointer.open(argv[1], std::ifstream::in);
    if(!filePointer.is_open()){
        cerr << "Error opening file" << endl;
        exit(1);
    }
    vector<string> arrayOfTokens = lexer.lexer(filePointer);
    filePointer.close();
    Parser parser(arrayOfTokens);
    parser.parse();

    return 0;
}
//