#include "kind.h"
#include "lexer.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>
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
        int pc = 0;
        std::map<string, int> labels;
        
        // Iterate over the lines of tokens and print them
        // to standard error
        for(int i = 0; i < tokLines.size() && !error; i++){
            vector<Token*> tokLine = tokLines.at(i);
            for(int j = 0; j < tokLine.size(); ++j){
                //cerr << "  Token: "  << *(tokLine.at(j)) << " " << tokLine.at(j)->getKind() << " " << tokLine.at(j)->getLexeme() << " " << i << endl;
                if (word && tokLine.at(j)->getKind() != 1 && tokLine.at(j)->getKind() != 2) {
                    cerr << "ERROR: no int or hex after .word" << endl;
                    error = true;
                    break;
                } else if(!word && (tokLine.at(j)->getKind() == 1 || tokLine.at(j)->getKind() == 2)) {
                    cerr << "ERROR: int or hex before .word" << endl;
                    error = true;
                    break;
                }  else if(command && tokLine.at(j)->getKind() == 7) {
                    cerr << "ERROR: label after command on the same line" << endl;
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
                    pc += 4;
                } else if (tokLine.at(j)->getKind() == 1 || tokLine.at(j)->getKind() == 2) {
                } else if (tokLine.at(j)->getKind() == 7) {
                    if (labels.count(tokLine.at(j)->getLexeme().substr(0, tokLine.at(j)->getLexeme().size()-1)) == 1) {
                        cerr << "ERROR: duplicate label detected" << endl;
                        error = true;
                        break;
                    }
                    labels[tokLine.at(j)->getLexeme().substr(0, tokLine.at(j)->getLexeme().size()-1)] = pc;
                    //cerr << "label: " << tokLine.at(j)->getLexeme().substr(0, tokLine.at(j)->getLexeme().size()-1) << " " << pc << endl;
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
        for (it = tokLines.begin(); it != tokLines.end() && !error; ++it){
            vector<Token*>::iterator it2;
            for (it2 = it->begin(); it2!=it->end(); ++it2){
                if ((*(*it2)).getKind()==8) {
                    continue;
                } else if ((*(*it2)).getKind()==7) {
                    if (labels.count((*(*it2)).getLexeme().substr(0, (*(*it2)).getLexeme().size()-1)) == 0) {
                        cerr << "ERROR: uninitialized label used" << endl;
                        error = true;
                        break;
                    }
                    cerr << (*(*it2)).getLexeme().substr(0, (*(*it2)).getLexeme().size()-1) << " " << labels[(*(*it2)).getLexeme().substr(0, (*(*it2)).getLexeme().size()-1)] << endl;
                } else {
		    int num = (*(*it2)).toInt();
                    putchar((num>>24) & 0xff);
                    putchar((num>>16) & 0xff);
                    putchar((num>>8) & 0xff);
                    putchar(num & 0xff);
		}
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
