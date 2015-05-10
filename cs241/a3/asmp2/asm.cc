#include "kind.h"
#include "lexer.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
// Use only the neeeded aspects of each namespace
using std::string;
using std::vector;
using std::endl;
using std::cerr;
using std::cin;
using std::getline;
using ASM::Token;
using ASM::Lexer;

int main(int argc, char* argv[]){
    // Nested vector representing lines of Tokens
    // Needs to be used here to cleanup in the case
    // of an exception
    vector< vector<Token*> > tokLines;
    try{
        // Create a MIPS recognizer to tokenize
        // the input lines
        Lexer lexer;
        // Tokenize each line of the input
        string line;
        while(getline(cin,line)){
            tokLines.push_back(lexer.scan(line));
        }

        bool word = false;
        bool command = false;
        bool error = false;
        // Iterate over the lines of tokens and print them
        // to standard error
        for(int i = 0; i < tokLines.size() && !error; i++){
            vector<Token*> tokLine = tokLines.at(i);
            for(int j = 0; j < tokLine.size(); ++j){
                cerr << "  Token: "  << *(tokLine.at(j)) << " " << tokLine.at(j)->getKind() << " " << tokLine.at(j)->getLexeme() << endl;
                if (word && tokLine.at(j)->getKind() != 1 && tokLine.at(j)->getKind() != 2) {
                    cerr << "ERROR: no int or hex after .word" << endl;
                    error = true;
                    break;
                } else if(!word && (tokLine.at(j)->getKind() == 1 || tokLine.at(j)->getKind() == 2)) {
                    cerr << "ERROR: int or hex before .word" << endl;
                    error = true;
                    break;
                } else {
                    word = false;
                }
            
                if (tokLine.at(j)->getKind() == 8) {
                    if (command) {
                        cerr << "ERROR: Multiple .word statements on 1 line" << endl;
                        error = true;
                        break;
                    }
                    word = true;
                    command = true;
                } else if (tokLine.at(j)->getKind() == 1 || tokLine.at(j)->getKind() == 2) {
                } else {
                    cerr << "ERROR: Input not recongnized" << endl;
                    error = true;
                    break;
                }
            }
            if (word && !error) {
                cerr << "ERROR: missing parameter for .word" << endl;
                error = true;
                break;
            }
            word = false;
            command = false;
        }
        
        vector<vector<Token*> >::iterator it;
        for (it = tokLines.begin(); it != tokLines.end(); ++it){
            vector<Token*>::iterator it2;
            for (it2 = it->begin(); it2!=it->end(); ++it2){
                if ((*(*it2)).getKind()==8) {
                    continue;
                }
                int num = (*(*it2)).toInt();
                putchar((num>>24) & 0xff);
                putchar((num>>16) & 0xff);
                putchar((num>>8) & 0xff);
                putchar(num & 0xff);
            }
        }
    } catch(const string& msg){
        // If an exception occurs print the message and end the program
        cerr << msg << endl;
    }
    
    // Delete the Tokens that have been made
    vector<vector<Token*> >::iterator it;
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
        vector<Token*>::iterator it2;
        for(it2 = it->begin(); it2 != it->end(); ++it2){
            delete *it2;
        }
    }
}
