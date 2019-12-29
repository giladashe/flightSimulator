#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "VarData.h"
#include "Parser.h"
#include "Data.h"


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
    //make array of tokens from the text file
    vector<string> arrayOfTokens = lexer.makeTokensArray(filePointer);
    filePointer.close();
    //do commands according to the array
    Parser parser(arrayOfTokens);
    parser.parse();
    //delete all the allocated memory in the singleton
    delete Data::getInstance();

    return 0;
}
//