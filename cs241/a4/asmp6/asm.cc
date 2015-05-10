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
    const int COMMA = 4;
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
                            cerr << "ERROR: invalid operand for .word instruction" << endl;
                            error = true;
                            break;
                        }
                        
                        ++it2;
                        
                        if (it2 != it->end()) {
                            cerr << "ERROR: too many operands for .word instruction" << endl;
                            error = true;
                        }
                        
                        break;
                    } else {
                        cerr << "ERROR: not enough operands for .word instruction" << endl;
                        error = true;
                        break;
                    }
                } else if((*(*it2)).getKind() == ID) {
                    if ((*(*it2)).getLexeme() == "jr" || (*(*it2)).getLexeme() == "jalr" || (*(*it2)).getLexeme() == "lis" || (*(*it2)).getLexeme() == "mfhi" || (*(*it2)).getLexeme() == "mflo") {
                        ++it2;
                        if (it2 != it->end()) {
                            //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                            
                            pc += 4;
                            
                            if ((*(*it2)).getKind() != REGISTER) {
                                cerr << "ERROR: invalid operand for jr or jalr or lis or mflo or mfhi instruction" << endl;
                                error = true;
                                break;
                            }
                            
                            ++it2;
                            
                            if (it2 != it->end()) {
                                cerr << "ERROR: too many operands for jr or jalr or lis or mflo or mfhi instruction" << endl;
                                error = true;
                            }
                            
                            break;
                        } else {
                            cerr << "ERROR: not enough operands for jr or jalr or lis or mflo or mfhi instruction" << endl;
                            error = true;
                            break;
                        }
                    } else if ((*(*it2)).getLexeme() == "add" || (*(*it2)).getLexeme() == "sub" || (*(*it2)).getLexeme() == "slt" || (*(*it2)).getLexeme() == "sltu") {
                        ++it2;
                        if (it2 != it->end()) {
                            //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                            
                            pc += 4;
                            
                            if ((*(*it2)).getKind() != REGISTER) {
                                cerr << "ERROR: invalid operands for add or sub or slt or sltu instruction" << endl;
                                error = true;
                                break;
                            }
                            
                            ++it2;
                            
                            if (it2 != it->end()) {
                                //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                
                                if ((*(*it2)).getKind() != COMMA) {
                                    cerr << "ERROR: operand not seperated by comma for add or sub or slt or sltu instruction" << endl;
                                    error = true;
                                    break;
                                }
                                
                                ++it2;
                                
                                if (it2 != it->end()) {
                                    //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                    
                                    if ((*(*it2)).getKind() != REGISTER) {
                                        cerr << "ERROR: invalid operands for add or sub or slt or sltu instruction" << endl;
                                        error = true;
                                        break;
                                    }
                                    
                                    ++it2;
                                    
                                    if (it2 != it->end()) {
                                        //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                        
                                        if ((*(*it2)).getKind() != COMMA) {
                                            cerr << "ERROR: operand not seperated by comma for add or sub or slt or sltu instruction" << endl;
                                            error = true;
                                            break;
                                        }
                                        
                                        ++it2;
                                        
                                        if (it2 != it->end()) {
                                            //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                            
                                            if ((*(*it2)).getKind() != REGISTER) {
                                                cerr << "ERROR: invalid operands for add or sub or slt or sltu instruction" << endl;
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
                                            cerr << "ERROR: not enough operands for add or sub or slt or sltu instruction" << endl;
                                            error = true;
                                            break;
                                        }
                                    } else {
                                        cerr << "ERROR: not enough operands for add or sub or slt or sltu instruction" << endl;
                                        error = true;
                                        break;
                                    }
                                } else {
                                    cerr << "ERROR: not enough operands for add or sub or slt or sltu instruction" << endl;
                                    error = true;
                                    break;
                                }
                            } else {
                                cerr << "ERROR: not enough operands for add or sub or slt or sltu instruction" << endl;
                                error = true;
                                break;
                            }
                        } else {
                            cerr << "ERROR: not enough operands for add or sub or slt or sltu instruction" << endl;
                            error = true;
                            break;
                        }
                    } else if ((*(*it2)).getLexeme() == "mult" || (*(*it2)).getLexeme() == "multu" || (*(*it2)).getLexeme() == "div" || (*(*it2)).getLexeme() == "divu") {
                        ++it2;
                        if (it2 != it->end()) {
                            //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                            
                            pc += 4;
                            
                            if ((*(*it2)).getKind() != REGISTER) {
                                cerr << "ERROR: invalid operands for mult or multu or div or divu instruction" << endl;
                                error = true;
                                break;
                            }
                            
                            ++it2;
                            
                            if (it2 != it->end()) {
                                //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                
                                if ((*(*it2)).getKind() != COMMA) {
                                    cerr << "ERROR: operand not seperated by comma for mult or multu or div or divu instruction" << endl;
                                    error = true;
                                    break;
                                }
                                
                                ++it2;
                                
                                if (it2 != it->end()) {
                                    //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                    
                                    if ((*(*it2)).getKind() != REGISTER) {
                                        cerr << "ERROR: invalid operands for mult or multu or div or divu instruction" << endl;
                                        error = true;
                                        break;
                                    }
                                    
                                    ++it2;
                                    
                                    if (it2 != it->end()) {
                                        cerr << "ERROR: too many parameters for mult or multu or div or divu instruction" << endl;
                                        error = true;
                                    }
                                    
                                    break;
                                } else {
                                    cerr << "ERROR: not enough operands for mult or multu or div or divu instruction" << endl;
                                    error = true;
                                    break;
                                }
                            } else {
                                cerr << "ERROR: not enough operands for mult or multu or div or divu instruction" << endl;
                                error = true;
                                break;
                            }
                        } else {
                            cerr << "ERROR: not enough operands for mult or multu or div or divu instruction" << endl;
                            error = true;
                            break;
                        }
                    } else if ((*(*it2)).getLexeme() == "beq" || (*(*it2)).getLexeme() == "bne") {
                        ++it2;
                        if (it2 != it->end()) {
                            //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                            
                            pc += 4;
                            
                            if ((*(*it2)).getKind() != REGISTER) {
                                cerr << "ERROR: invalid operands for beq or bne instruction" << endl;
                                error = true;
                                break;
                            }
                            
                            ++it2;
                            
                            if (it2 != it->end()) {
                                //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                
                                if ((*(*it2)).getKind() != COMMA) {
                                    cerr << "ERROR: operand not seperated by comma for beq or bne instruction" << endl;
                                    error = true;
                                    break;
                                }
                                
                                ++it2;
                                
                                if (it2 != it->end()) {
                                    //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                    
                                    if ((*(*it2)).getKind() != REGISTER) {
                                        cerr << "ERROR: invalid operands for beq or bne instruction" << endl;
                                        error = true;
                                        break;
                                    }
                                    
                                    ++it2;
                                    
                                    if (it2 != it->end()) {
                                        //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                        
                                        if ((*(*it2)).getKind() != COMMA) {
                                            cerr << "ERROR: operand not seperated by comma for beq or bne instruction" << endl;
                                            error = true;
                                            break;
                                        }
                                        
                                        ++it2;
                                        
                                        if (it2 != it->end()) {
                                            //cerr << "  Token: "  << *(*it2) << " " << (*(*it2)).getKind() << " " << (*(*it2)).getLexeme() << endl;
                                            
                                            if ((*(*it2)).getKind() != INT && (*(*it2)).getKind() != HEXINT && (*(*it2)).getKind() != ID) {
                                                cerr << "ERROR: invalid operands for beq or bne instruction" << endl;
                                                error = true;
                                                break;
                                            } else if ((*(*it2)).getKind() == INT && ((*(*it2)).toInt() < -32768 || (*(*it2)).toInt() > 32767)) {
                                                cerr << "ERROR: int value specified for operand i is not in range for beq or bne instruction" << endl;
                                                error = true;
                                                break;
                                            } else if ((*(*it2)).getKind() == HEXINT && ((*(*it2)).toInt() < 0 || (*(*it2)).toInt() > 65535)) {
                                                cerr << "ERROR: hex value specified for operand i is not in range for beq or bne instruction" << endl;
                                                error = true;
                                                break;
                                            }
                                            
                                            ++it2;
                                            
                                            if (it2 != it->end()) {
                                                cerr << "ERROR: too many parameters for beq or bne instruction" << endl;
                                                error = true;
                                            }
                                            
                                            break;
                                        } else {
                                            cerr << "ERROR: not enough operands for beq or bne instruction" << endl;
                                            error = true;
                                            break;
                                        }
                                    } else {
                                        cerr << "ERROR: not enough operands for beq or bne instruction" << endl;
                                        error = true;
                                        break;
                                    }
                                } else {
                                    cerr << "ERROR: not enough operands for beq or bne instruction" << endl;
                                    error = true;
                                    break;
                                }
                            } else {
                                cerr << "ERROR: not enough operands for beq or bne instruction" << endl;
                                error = true;
                                break;
                            }
                        } else {
                            cerr << "ERROR: not enough operands for beq or bne instruction" << endl;
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
        
        pc = 0;
        
        vector<vector<Token*> >::iterator it3;
        for(it3 = tokLines.begin(); !error & it3 != tokLines.end(); ++it3){
            vector<Token*>::iterator it4;
            for(it4 = it3->begin(); it4 != it3->end(); ++it4){
                int word;
                string instruction;
                
                if ((*(*it4)).getKind() == DOTWORD) {
                    pc += 4;
                    
                    ++it4;
                    if ((*(*it4)).getKind() == INT || (*(*it4)).getKind() == HEXINT) {
                        word = (*(*it4)).toInt();
                        
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
                        
                        word = labels[(*(*it4)).getLexeme()];
                        putchar((word>>24) & 0xff);
                        putchar((word>>16) & 0xff);
                        putchar((word>>8) & 0xff);
                        putchar(word & 0xff);
                    }
                } else if ((*(*it4)).getKind() == ID) {
                    if ((*(*it4)).getLexeme() == "jr" || (*(*it4)).getLexeme() == "jalr" || (*(*it4)).getLexeme() == "lis" || (*(*it4)).getLexeme() == "mfhi" || (*(*it4)).getLexeme() == "mflo") {
                        pc += 4;
                        
                        if ((*(*it4)).getLexeme() == "jr") {
                            instruction = "jr";
                        } else if ((*(*it4)).getLexeme() == "jalr"){
                            instruction = "jalr";
                        } else if ((*(*it4)).getLexeme() == "lis"){
                            instruction = "lis";
                        } else if ((*(*it4)).getLexeme() == "mfhi"){
                            instruction = "mfhi";
                        } else {
                            instruction = "mflo";
                        }
                        
                        ++it4;
                        
                        int reg = (*(*it4)).toInt();
                        
                        if (instruction == "jr") {
                            word = (0 << 26) | (reg << 21) | (8 & 0x1fffff);
                        } else if (instruction == "jalr") {
                            word = (0 << 26) | (reg << 21) | (9 & 0x1fffff);
                        } else if (instruction == "lis"){
                            word = (0 << 16) | (reg << 11) | (20 & 0x7ff);
                        } else if (instruction == "mfhi"){
                            word = (0 << 16) | (reg << 11) | (16 & 0x7ff);
                        } else {
                            word = (0 << 16) | (reg << 11) | (18 & 0x7ff);
                        }
                        
                        putchar((word>>24) & 0xff);
                        putchar((word>>16) & 0xff);
                        putchar((word>>8) & 0xff);
                        putchar(word & 0xff);
                    } else if ((*(*it4)).getLexeme() == "add" || (*(*it4)).getLexeme() == "sub" || (*(*it4)).getLexeme() == "slt" || (*(*it4)).getLexeme() == "sltu") {
                        pc += 4;
                        
                        if ((*(*it4)).getLexeme() == "add") {
                            instruction = "add";
                        } else if((*(*it4)).getLexeme() == "sub") {
                            instruction = "sub";
                        } else if((*(*it4)).getLexeme() == "slt") {
                            instruction = "slt";
                        } else {
                            instruction = "sltu";
                        }
                        
                        ++it4;
                        
                        int reg1 = (*(*it4)).toInt();
                        
                        ++it4;
                        ++it4;
                        
                        int reg2 = (*(*it4)).toInt();
                        
                        ++it4;
                        ++it4;
                        
                        int reg3 = (*(*it4)).toInt();
                        
                        if (instruction == "add") {
                            word = (0 << 26) | (reg2 << 21) | (reg3 << 16) | (reg1 << 11) | (32 & 0x7ff);
                        } else if(instruction == "sub") {
                            word = (0 << 26) | (reg2 << 21) | (reg3 << 16) | (reg1 << 11) | (34 & 0x7ff);
                        } else if(instruction == "slt") {
                            word = (0 << 26) | (reg2 << 21) | (reg3 << 16) | (reg1 << 11) | (42 & 0x7ff);
                        } else {
                            word = (0 << 26) | (reg2 << 21) | (reg3 << 16) | (reg1 << 11) | (43 & 0x7ff);
                        }
                        
                        putchar((word>>24) & 0xff);
                        putchar((word>>16) & 0xff);
                        putchar((word>>8) & 0xff);
                        putchar(word & 0xff);
                    } else if ((*(*it4)).getLexeme() == "mult" || (*(*it4)).getLexeme() == "multu" || (*(*it4)).getLexeme() == "div" || (*(*it4)).getLexeme() == "divu") {
                        pc += 4;
                        
                        if ((*(*it4)).getLexeme() == "mult") {
                            instruction = "mult";
                        } else if((*(*it4)).getLexeme() == "multu") {
                            instruction = "multu";
                        } else if((*(*it4)).getLexeme() == "div") {
                            instruction = "div";
                        } else {
                            instruction = "divu";
                        }
                        
                        ++it4;
                        
                        int reg1 = (*(*it4)).toInt();
                        
                        ++it4;
                        ++it4;
                        
                        int reg2 = (*(*it4)).toInt();
                        
                        if (instruction == "mult") {
                            word = (0 << 26) | (reg1 << 21) | (reg2 << 16) | (24 & 0xffff);
                        } else if(instruction == "multu") {
                            word = (0 << 26) | (reg1 << 21) | (reg2 << 16) | (25 & 0xffff);
                        } else if(instruction == "div") {
                            word = (0 << 26) | (reg1 << 21) | (reg2 << 16) | (26 & 0xffff);
                        } else {
                            word = (0 << 26) | (reg1 << 21) | (reg2 << 16) | (27 & 0xffff);
                        }
                        
                        putchar((word>>24) & 0xff);
                        putchar((word>>16) & 0xff);
                        putchar((word>>8) & 0xff);
                        putchar(word & 0xff);
                    } else if ((*(*it4)).getLexeme() == "beq" || (*(*it4)).getLexeme() == "bne") {
                        pc += 4;
                        
                        if ((*(*it4)).getLexeme() == "beq") {
                            instruction = "beq";
                        } else {
                            instruction = "bne";
                        }
                        
                        ++it4;
                        
                        int reg1 = (*(*it4)).toInt();
                        
                        ++it4;
                        ++it4;
                        
                        int reg2 = (*(*it4)).toInt();
                        
                        ++it4;
                        ++it4;
                        
                        int i;
                        
                        if ((*(*it4)).getKind() == INT || (*(*it4)).getKind() == HEXINT) {
                            i = (*(*it4)).toInt();
                        } else if ((*(*it4)).getKind() == ID) {
                            if (labels.count((*(*it4)).getLexeme()) == 0) {
                                cerr << "ERROR: undefined label used in the beq or bne instruction" << endl;
                                error = true;
                                break;
                            } else {
                                i = (labels[(*(*it4)).getLexeme()] - pc) / 4;
                                
                                if (i < -32768 || i > 32767) {
                                    cerr << "ERROR: encoded label value for operand i is not in range for beq or bne instruction" << endl;
                                    error = true;
                                    break;
                                }
                            }
                        }
                        
                        if (instruction == "beq") {
                            word = (4 << 26) | (reg1 << 21) | (reg2 << 16) | (i & 0xffff);
                        } else {
                            word = (5 << 26) | (reg1 << 21) | (reg2 << 16) | (i & 0xffff);
                        }
                        
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