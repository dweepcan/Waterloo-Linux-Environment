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
using std::map;
using ASM::Token;
using ASM::Lexer;


int main(int argc, char* argv[]){
    const int ID = 0;
    const int INT = 1;
    const int HEXINT = 2;
    const int REGISTER = 3;
    const int LABEL = 7;
    const int DOTWORD = 8;
    
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
        
        bool error = false;
        int pc = 0;
        map<string, int> labels;
        
        // Iterate over the lines of tokens and print them
        // to standard error
        vector<vector<Token*> >::iterator it;
        for(it = tokLines.begin(); !error && it != tokLines.end(); ++it){
            vector<Token*>::iterator it2;
            for(it2 = it->begin(); !error && it2 != it->end(); ++it2){
                //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                
                if ((*(*it2)).getKind() == DOTWORD) {
                    ++it2;
                    if (it2 != it->end()) {
                        //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                        
                        pc += 4;
                        
                        if ((*(*it2)).getKind() != INT && (*(*it2)).getKind() != HEXINT && (*(*it2)).getKind() != ID) {
                            cerr << "ERROR: invalid parameters for .word instruction" << endl;
                            error = true;
                            break;
                        }
                        
                        ++it2;
                        
                        if (it2 != it->end()) {
                            cerr << "ERROR: too many parameters for .word instruction" << endl;
                            error = true;
                        }
                        
                        break;
                    } else {
                        cerr << "ERROR: not enough parameters for .word instruction" << endl;
                        error = true;
                        break;
                    }
                } else if((*(*it2)).getKind() == ID) {
                    if ((*(*it2)).getLexeme() == "jr" || (*(*it2)).getLexeme() == "jalr") {
                        ++it2;
                        if (it2 != it->end()) {
                            //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                            
                            pc += 4;
                            
                            if ((*(*it2)).getKind() != REGISTER) {
                                cerr << "ERROR: invalid parameters for jr or jalr instruction" << endl;
                                error = true;
                                break;
                            }
                            
                            ++it2;
                            
                            if (it2 != it->end()) {
                                cerr << "ERROR: too many parameters for jr or jalr instruction" << endl;
                                error = true;
                            }
                            
                            break;
                        } else {
                            cerr << "ERROR: not enough parameters for jr or jalr instruction" << endl;
                            error = true;
                            break;
                        }
                    } else {
                        cerr << "ERROR: unknown instruction" << endl;
                        error = true;
                        break;
                    }
                } else if((*(*it2)).getKind() == LABEL) {
                    if (labels.count((*(*it2)).getLexeme().substr(0, (*(*it2)).getLexeme().length()-1)) >= 1) {
                        cerr << "ERROR: duplicate label defined" << endl;
                        error = true;
                        break;
                    }
                    
                    string label = (*(*it2)).getLexeme().substr(0, (*(*it2)).getLexeme().length()-1);
                    
                    labels[label] = pc;
                    
                    cerr << label << " " << pc << endl;
                } else {
                    cerr << "ERROR: unknown instruction" << endl;
                    error = true;
                    break;
                }
            }
        }
        
        vector<vector<Token*> >::iterator it3;
        for(it3 = tokLines.begin(); !error & it3 != tokLines.end(); ++it3){
            vector<Token*>::iterator it4;
            for(it4 = it3->begin(); it4 != it3->end(); ++it4){
                if ((*(*it4)).getKind() == DOTWORD) {
                    ++it4;
                    if ((*(*it4)).getKind() == INT || (*(*it4)).getKind() == HEXINT) {
                        int word = (*(*it4)).toInt();
                        
                        putchar((word>>24) & 0xff);
                        putchar((word>>16) & 0xff);
                        putchar((word>>8) & 0xff);
                        putchar(word & 0xff);
                    } else if ((*(*it4)).getKind() == ID) {
                        if (labels.count((*(*it4)).getLexeme()) == 0) {
                            cerr << "ERROR: undefined label used as an operand" << endl;
                            error = true;
                            break;
                        }
                        
                        int word = labels[(*(*it4)).getLexeme()];
                        putchar((word>>24) & 0xff);
                        putchar((word>>16) & 0xff);
                        putchar((word>>8) & 0xff);
                        putchar(word & 0xff);
                    }
                } else if ((*(*it4)).getKind() == ID) {
                    if ((*(*it4)).getLexeme() == "jr") {
                        ++it4;
                        
                        int reg = (*(*it4)).toInt();
                        
                        int word = (0 << 26) | (reg << 21) | (8 & 0x1fffff);
                        
                        putchar((word>>24) & 0xff);
                        putchar((word>>16) & 0xff);
                        putchar((word>>8) & 0xff);
                        putchar(word & 0xff);
                    } else if ((*(*it4)).getLexeme() == "jalr") {
                        ++it4;
                        
                        int reg = (*(*it4)).toInt();
                        
                        int word = (0 << 26) | (reg << 21) | (9 & 0x1fffff);
                        
                        putchar((word>>24) & 0xff);
                        putchar((word>>16) & 0xff);
                        putchar((word>>8) & 0xff);
                        putchar(word & 0xff);
                    }
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
