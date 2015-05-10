#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

struct Node {
	string rule;
	vector<string> tokens;
	vector<Node*> children;
};

struct Symbol {
	string name;
	string type;
	int location;
};

struct Procedure {
	string name;
	vector<string> params;
	map<string, Symbol> symbolTable;
};

map<string, Procedure> procedures;

vector<string> terminals;

string trim(const string &str) {
  size_t begin = str.find_first_not_of(" \t\n");
  if (begin == string::npos) return "";

  size_t end = str.find_last_not_of(" \t\n");

  return str.substr(begin, end - begin + 1);
}

vector<string> split(string rule) {
	vector<string> result;
	stringstream ss(rule);
	string item;
	
	while(getline(ss, item, ' ')) {
		if(!item.empty()) {
			result.push_back(item);
		}
	}
	return result;
}

bool find(vector<string> list, string search) {
	for(int i=0; i < list.size(); i++) {
		if (list.at(i) == search) {
			return true;
		}
	}
	return false;
}

void readTerminals() {
	string terminal;
	string fileName = "terminals.txt";
	ifstream inputFile(fileName.c_str());

	if (!inputFile) {
        cerr << "ERROR: can't open specified text file." << endl;
    }

    while(inputFile >> terminal) {
    	terminals.push_back(trim(terminal));
    }
}

void createParseTree(istream &in, Node* root) {
	string input;
	getline(in, input);
	vector<string> tokensVector = split(input);

	root->rule = input;
	root->tokens = tokensVector;

	for(int i=1; i < tokensVector.size(); i++) {
		Node* child = new Node();
		if(find(terminals, tokensVector.at(i))) {
			getline(in, input);

			child->rule = input;
			child->tokens = split(input);
		} else {
			createParseTree(in, child);
		}
		root->children.push_back(child);
	}
}

string addSymbol(Node* root, map<string, Symbol> &symbolTable) {
	string name = root->children.at(1)->tokens.at(1);
	string type;
	for (int i=0; i < root->children.at(0)->children.size(); i++) {
		type.append(root->children.at(0)->children.at(i)->tokens.at(1));
	}
		
	if(symbolTable.count(name) > 0) {
		cerr << "ERROR: identifier is declared more than once: " << name << endl;
	} else {
		Symbol* id = new Symbol();
		id->name = name;
		id->type = type;
		symbolTable[name] = *id;
	}

	return type;
}

void addParamsProc(Node* root, map<string, Symbol> &symbolTable, vector<string> &params) {
	if(root->tokens.at(0) == "dcl") {
		string type;

		type = addSymbol(root, symbolTable);
		params.push_back(type);
	}

	if(!root->children.empty()) {
		for(int i=0; i < root->children.size(); i++) {
			addParamsProc(root->children.at(i), symbolTable, params);
		}
	}
}

void addParams(Node* root, map<string, Symbol> &symbolTable, vector<string> &params) {
	if(root->tokens.at(0) == "main") {
		string type;
		
		type = addSymbol(root->children.at(3), symbolTable);
		params.push_back(type);
		type = addSymbol(root->children.at(5), symbolTable);
		params.push_back(type);
	} else {
		addParamsProc(root->children.at(3), symbolTable, params);
	}
}

void printParams(vector<string> params) {
	for (vector<string>::iterator it=params.begin(); it!=params.end(); ++it) {
    	cerr << " " << *it;
	}
	cerr << endl;
}

void addDcls(Node* root, map<string, Symbol> &symbolTable) {
	if(root->tokens.at(0) == "dcl") {
		addSymbol(root, symbolTable);
		return;
	}

	if(!root->children.empty()) {
		for(int i=0; i < root->children.size(); i++) {
			addDcls(root->children.at(i), symbolTable);
		}
	}
}

string getType(Node* root, Procedure* proc);

void getArgList(Node* root, Procedure* proc, vector<string> &argList) {
	argList.push_back(getType(root->children.at(0), proc));

	if(root->children.size() == 3) {
		getArgList(root->children.at(2), proc, argList);
	}
}

string getType(Node* root, Procedure* proc) {
	string rule = root->rule;
	map<string, Symbol> symbolTable = proc->symbolTable;

	if(rule == "expr term") {
		return getType(root->children.at(0), proc);
	} else if(rule == "expr expr PLUS term") {
		string exprType = getType(root->children.at(0), proc);
		string termType = getType(root->children.at(2), proc);
		if(exprType == "int" && termType == "int") {
			return "int";
		} else if(exprType == "int*" && termType == "int") {
			return "int*";
		} else if(exprType == "int" && termType == "int*") {
			return "int*";
		} else {
			cerr << "ERROR: cannot add or subtract two pointers" << endl;
		}
	} else if(rule == "expr expr MINUS term") {
		string exprType = getType(root->children.at(0), proc);
		string termType = getType(root->children.at(2), proc);
		if(exprType == "int" && termType == "int") {
			return "int";
		} else if(exprType == "int*" && termType == "int") {
			return "int*";
		} else if(exprType == "int*" && termType == "int*") {
			return "int";
		} else {
			cerr << "ERROR: cannot add or subtract two pointers" << endl;
		}
	} else if(rule == "term factor") {
		return getType(root->children.at(0), proc);
	} else if(rule == "term term STAR factor" || rule == "term term SLASH factor" || rule == "term term PCT factor") {
		string termType = getType(root->children.at(0), proc);
		string factorType = getType(root->children.at(2), proc);
		if(termType == "int" && factorType == "int") {
			return "int";
		} else {
			cerr << "ERROR: cannot multiply or divide two pointers" << endl;
		}
	} else if(root->rule == "factor ID") {
		string name = root->children.at(0)->tokens.at(1);
		if (symbolTable.count(name) == 0) {
			cerr << "ERROR: identifier is used withut being declared" << name << endl;
		} else {
			return symbolTable[name].type;
		}
	} else if(root->rule == "factor NUM") {
		return "int";
	} else if(root->rule == "factor NULL") {
		return "int*";
	} else if(root->rule == "factor LPAREN expr RPAREN") {
		return getType(root->children.at(1), proc);
	} else if(root->rule == "factor AMP lvalue") {
		string lvalueType = getType(root->children.at(1), proc);
		if(lvalueType == "int") {
			return "int*";
		} else {
			cerr << "ERROR: cannot reference a pointer" << endl;
		}
	} else if(root->rule == "factor STAR factor") {
		string factorType = getType(root->children.at(1), proc);
		if(factorType == "int*") {
			return "int";
		} else {
			cerr << "ERROR: cannot dereference a non-pointer" << endl;
		}
	} else if(root->rule == "factor NEW INT LBRACK expr RBRACK") {
		string exprType = getType(root->children.at(3), proc);
		if(exprType == "int") {
			return "int*";
		} else {
			cerr << "ERROR: cannot create an array with size as pointer" << endl;
		}
	} else if(root->rule == "factor ID LPAREN RPAREN") {
		string procName = root->children.at(0)->tokens.at(1);
		if(procedures.count(procName) > 0) {
			if(procedures[procName].params.size() == 0) {
				return "int";
			} else {
				cerr << "ERROR: procedure " << procName << "does not have an empty signature" << endl;
			}
		} else if(procName == proc->name) {
			if(proc->params.size() == 0) {
				return "int";
			} else {
				cerr << "ERROR: procedure " << procName << "does not have an empty signature" << endl;
			}
		} else {
			cerr << "ERROR: procedure is used without being declared: " << procName << endl;
		}
	} else if(root->rule == "factor ID LPAREN arglist RPAREN") {
		string procName = root->children.at(0)->tokens.at(1);
		vector<string> argList;
		getArgList(root->children.at(2), proc, argList);

		if(procedures.count(procName) > 0) {
			if(procedures[procName].params.size() == argList.size()) {
				for (int i = 0; i < argList.size(); ++i) {
					if (procedures[procName].params.at(i) != argList.at(i)) {
						cerr << "ERROR: parameter " << i << " for the procedure " << procName << " is of the wrong type " << argList.at(i) << endl;
						return "";
					}
				}
				return "int";
			} else {
				cerr << "ERROR: procedure " << procName << "does not have the correct number of parameters" << endl;
			}
		} else if(procName == proc->name) {
			if(proc->params.size() == argList.size()) {
				for (int i = 0; i < argList.size(); ++i) {
					if (proc->params.at(i) != argList.at(i)) {
						cerr << "ERROR: parameter " << i << " for the procedure " << procName << " is of the wrong type " << argList.at(i) << endl;
						return "";
					}
				}
				return "int";
			} else {
				cerr << "ERROR: procedure " << procName << "does not have the correct number of parameters" << endl;
			}
		} else {
			cerr << "ERROR: procedure is used without being declared: " << procName << endl;
		}
		return "";
	} else if(root->rule == "lvalue ID") {
		string name = root->children.at(0)->tokens.at(1);
		if(symbolTable.count(name) == 0) {
			cerr << "ERROR: identifier is used without being declared " << name <<  endl;
		} else {
			return symbolTable[name].type;
		}
	} else if(root->rule == "lvalue STAR factor") {
		string factorType = getType(root->children.at(1), proc);
		if(factorType == "int*") {
			return "int";
		} else {
			cerr << "ERROR: cannot dereference a non-pointer" << endl;
		}
	} else if(root->rule == "lvalue LPAREN lvalue RPAREN") {
		return getType(root->children.at(1), proc);
	}
	return "";
}
/*
bool wellTyped(Node* root, Procedure* proc) {
	string rule = root->rule;
	if(rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
		string type = proc->symbolTable[root->children.at(5)->children.at(1)->tokens.at(1)].type;
		if(type != "int") {
			cerr << "ERROR: the second parameter in the main function must be of type int" << endl;
			return false;
		}
		type = getType(root->children.at(11), proc);
		if(type != "int") {
			cerr << "ERROR: the return statement must be of type int" << endl;
			return false;
		}
		return wellTyped(root->children.at(8), proc) && wellTyped(root->children.at(9), proc);
	} else if(rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
		string type = getType(root->children.at(9), proc);
		if(type != "int") {
			cerr << "ERROR: the return statement must be of type int" << endl;
			return false;
		}
		return wellTyped(root->children.at(6), proc) && wellTyped(root->children.at(7), proc);
	} else if(rule == "statements statements statement") {
		return wellTyped(root->children.at(0), proc) && wellTyped(root->children.at(1), proc);
	} else if(rule == "test expr EQ expr" || rule == "test expr NE expr" || rule == "test expr LT expr" || rule == "test expr LE expr" || rule == "test expr GE expr" || rule == "test expr GT expr") {
		bool type = getType(root->children.at(0), proc) == getType(root->children.at(2), proc);
		if(!type) {
			cerr << "ERROR: can not perform equals operation with different types" << endl;
			return false;
		}
	} else if(rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
		return wellTyped(root->children.at(2), proc) && wellTyped(root->children.at(5), proc);
	} else if(rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
		return wellTyped(root->children.at(2), proc) && wellTyped(root->children.at(5), proc) && wellTyped(root->children.at(9), proc);
	} else if(rule == "statement DELETE LBRACK RBRACK expr SEMI") {
		bool type = getType(root->children.at(3), proc) == "int*";
		if(!type) {
			cerr << "ERROR: can not deallocate memory of non-pointer" << endl;
			return false;
		}
	} else if(rule == "statement lvalue BECOMES expr SEMI") {
		bool type = getType(root->children.at(0), proc) == getType(root->children.at(2), proc);
		if(!type) {
			cerr << "ERROR: can not perform equals operation with different types" << endl;
			return false;
		}
	} else if(rule == "dcls dcls dcl BECOMES NUM SEMI") {
		string type = proc->symbolTable[root->children.at(1)->children.at(1)->tokens.at(1)].type;
		if(type != "int") {
			cerr << "ERROR: can not initialize a ponter to a number" << endl;
			return false;
		}
		return wellTyped(root->children.at(0), proc);
	} else if(rule == "dcls dcls dcl BECOMES NULL SEMI") {
		string type = proc->symbolTable[root->children.at(1)->children.at(1)->tokens.at(1)].type;
		if(type != "int*") {
			cerr << "ERROR: can not initialize a non-ponter to null" << endl;
			return false;
		}
		return wellTyped(root->children.at(0), proc);
	}
	return true;
}
*/
void push(string location) {
	cout << "sw " << location << ", -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;
}

void pop(string location) {
	cout << "add $30, $30, $4" << endl;
	cout << "lw " << location << ", -4($30)" << endl;
}

void setLocations(map<string, Symbol> &symbolTable) {
	int i = 0;
	for (map<string, Symbol>::iterator it=symbolTable.begin(); it!=symbolTable.end(); ++it) {
    	it->second.location = i;
    	--i;
	}
}

int loopCounter = 0;
int ifCounter = 0;

void mipsCodeGenerator(Node* root, Procedure* proc) {
	string rule = root->rule;
	if(rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
		cout << "sw $1, " << proc->symbolTable[root->children.at(3)->children.at(1)->tokens.at(1)].location*4 << "($29)" << endl;
		cout << "sw $2, " << proc->symbolTable[root->children.at(5)->children.at(1)->tokens.at(1)].location*4 << "($29)" << endl;
		mipsCodeGenerator(root->children.at(8), proc);
		mipsCodeGenerator(root->children.at(9), proc);
		mipsCodeGenerator(root->children.at(11), proc);
	} else if(rule == "statements statements statement") {
		mipsCodeGenerator(root->children.at(0), proc);
		mipsCodeGenerator(root->children.at(1), proc);
	} else if(rule == "statement PRINTLN LPAREN expr RPAREN SEMI") {
		mipsCodeGenerator(root->children.at(2), proc);
		cout << "add $1, $3, $0" << endl;
		cout << "lis $5" << endl;
		cout << ".word print" << endl;
		cout << "jalr $5" << endl;
	} else if(rule == "expr term") {
		mipsCodeGenerator(root->children.at(0), proc);
	} else if(rule == "term factor") {
		mipsCodeGenerator(root->children.at(0), proc);
	} else if(rule == "factor ID") {
		cout << "lw $3, " << proc->symbolTable[root->children.at(0)->tokens.at(1)].location*4 << "($29)" << endl;
	} else if(rule == "factor LPAREN expr RPAREN") {
		mipsCodeGenerator(root->children.at(1), proc);
	} else if(rule == "factor NUM") {
		cout << "lis $3" << endl;
		cout << ".word " << root->children.at(0)->tokens.at(1) << endl;
	} else if(rule == "expr expr PLUS term" || rule == "expr expr MINUS term" || rule == "term term STAR factor" || rule == "term term SLASH factor" || rule == "term term PCT factor" || rule == "test expr LT expr" || rule == "test expr EQ expr" || rule == "test expr NE expr" || rule == "test expr LE expr" || rule == "test expr GE expr" || rule == "test expr GT expr") {
		mipsCodeGenerator(root->children.at(0), proc);
		push("$3");
		mipsCodeGenerator(root->children.at(2), proc);
		pop("$5");
		if (rule == "expr expr PLUS term") {
			if(getType(root->children.at(0), proc) == "int" && getType(root->children.at(2), proc) == "int") {
				cout << "add $3, $5, $3" << endl;
			} else if(getType(root->children.at(0), proc) == "int*" && getType(root->children.at(2), proc) == "int") {
				cout << "mult $3, $4" << endl;
				cout << "mflo $3" << endl;
				cout << "add $3, $5, $3" << endl;
			} else if(getType(root->children.at(0), proc) == "int" && getType(root->children.at(2), proc) == "int*") {
				cout << "mult $5, $4" << endl;
				cout << "mflo $5" << endl;
				cout << "add $3, $3, $5" << endl;
			}
		} else if(rule == "expr expr MINUS term") {
			if(getType(root->children.at(0), proc) == "int" && getType(root->children.at(2), proc) == "int") {
				cout << "sub $3, $5, $3" << endl;
			} else if(getType(root->children.at(0), proc) == "int*" && getType(root->children.at(2), proc) == "int") {
				cout << "mult $3, $4" << endl;
				cout << "mflo $3" << endl;
				cout << "sub $3, $5, $3" << endl;
			} else if(getType(root->children.at(0), proc) == "int*" && getType(root->children.at(2), proc) == "int*") {
				cout << "sub $3, $5, $3" << endl;
				cout << "div $3, $4" << endl;
				cout << "mflo $3" << endl;
			}
		} else if(rule == "term term STAR factor") {
			cout << "mult $5, $3" << endl;
			cout << "mflo $3" << endl;
		} else if(rule == "term term SLASH factor") {
			cout << "div $5, $3" << endl;
			cout << "mflo $3" << endl;
		} else if(rule == "term term PCT factor") {
			cout << "div $5, $3" << endl;
			cout << "mfhi $3" << endl;
		} else if(rule == "test expr LT expr") {
			cout << "slt $3, $5, $3" << endl;
		} else if(rule == "test expr EQ expr") {
			cout << "slt $6, $3, $5" << endl;
			cout << "slt $7, $5, $3" << endl;
			cout << "add $3, $6, $7" << endl;
			cout << "sub $3, $11, $3" << endl;
		} else if(rule == "test expr NE expr") {
			cout << "slt $6, $3, $5" << endl;
			cout << "slt $7, $5, $3" << endl;
			cout << "add $3, $6, $7" << endl;
		} else if(rule == "test expr LE expr") {
			cout << "slt $3, $3, $5" << endl;
			cout << "sub $3, $11, $3" << endl;
		} else if(rule == "test expr GE expr") {
			cout << "slt $3, $5, $3" << endl;
			cout << "sub $3, $11, $3" << endl;
		} else if(rule == "test expr GT expr") {
			cout << "slt $3, $3, $5" << endl;
		}
	} else if(rule == "dcls dcls dcl BECOMES NUM SEMI") {
		mipsCodeGenerator(root->children.at(0), proc);
		cout << "lis $3" << endl;
		cout << ".word " << root->children.at(3)->tokens.at(1) << endl;
		cout << "sw $3, " << proc->symbolTable[root->children.at(1)->children.at(1)->tokens.at(1)].location*4 << "($29)" << endl;
	} else if(rule == "statement lvalue BECOMES expr SEMI") {
		mipsCodeGenerator(root->children.at(2), proc);
		mipsCodeGenerator(root->children.at(0), proc);
	} else if(rule == "lvalue ID") {		
		cout << "sw $3, " << proc->symbolTable[root->children.at(0)->tokens.at(1)].location*4 << "($29)" << endl;
	} else if(rule == "lvalue LPAREN lvalue RPAREN") {
		mipsCodeGenerator(root->children.at(1), proc);
	} else if(rule == "lvalue STAR factor") {
		cout << "add $6, $3, $0" << endl;
		mipsCodeGenerator(root->children.at(1), proc);
		cout << "sw $6, 0($3)" << endl;
	} else if(rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
		int currentCounter = loopCounter;
		++loopCounter;
		cout << "loop" << currentCounter << ":" << endl;
		mipsCodeGenerator(root->children.at(2), proc);
		cout << "bne $3, $0, 3" << endl;
		cout << "lis $6" << endl;
		cout << ".word endloop"<< currentCounter << endl;
		cout << "jr $6" << endl;
		mipsCodeGenerator(root->children.at(5), proc);
		cout << "lis $6" << endl;
		cout << ".word loop"<< currentCounter << endl;
		cout << "jr $6" << endl;
		cout << "endloop" << currentCounter << ":" << endl;
	} else if(rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
		int currentCounter = ifCounter;
		++ifCounter;
		mipsCodeGenerator(root->children.at(2), proc);
		cout << "beq $3, $0, 3" << endl;
		cout << "lis $6" << endl;
		cout << ".word ifpart" << currentCounter << endl;
		cout << "jr $6" << endl;
		mipsCodeGenerator(root->children.at(9), proc);
		cout << "lis $6" << endl;
		cout << ".word endif"<< currentCounter << endl;
		cout << "jr $6" << endl;
		cout << "ifpart" << currentCounter << ":" << endl;
		mipsCodeGenerator(root->children.at(5), proc);
		cout << "endif" << currentCounter << ":" << endl;
	} else if(rule == "factor NULL") {
		cout << "add $3, $0, $11" << endl;
	} else if(rule == "factor STAR factor") {
		mipsCodeGenerator(root->children.at(1), proc);
		cout << "add $1, $3, $0" << endl;
		cout << "lis $5" << endl;
		cout << ".word print" << endl;
		cout << "jalr $5" << endl; 
 		cout << "lw $3, 0($3)" << endl;
	} else if(rule == "factor AMP lvalue") {
		lookAhead:
		root = root->children.at(1);
		if(root->rule == "lvalue ID") {
			cout << "lis $3" << endl;
			cout << ".word " << proc->symbolTable[root->children.at(0)->tokens.at(1)].location*4 << endl;
			cout << "add $3, $29, $3" << endl;
		} else if(root->rule == "lvalue STAR factor") {
			mipsCodeGenerator(root->children.at(1), proc);
		} else if(root->rule == "lvalue LPAREN lvalue RPAREN") {
			goto lookAhead;
		}
	} else if(rule == "dcls dcls dcl BECOMES NULL SEMI") {
		mipsCodeGenerator(root->children.at(0), proc);
		cout << "add $3, $0, $11" << endl;
		cout << "sw $3, " << proc->symbolTable[root->children.at(1)->children.at(1)->tokens.at(1)].location*4 << "($29)" << endl;
	} else if(rule == "factor NEW INT LBRACK expr RBRACK") {
		mipsCodeGenerator(root->children.at(3), proc);
		cout << "add $1, $3, $0" << endl;
		cout << "lis $5" << endl;
		cout << ".word new" << endl;
		cout << "jalr $5" << endl;
		cout << "bne $3, $0, 1" << endl;
		cout << "add $3, $11, $0" << endl;
	} else if(rule == "statement DELETE LBRACK RBRACK expr SEMI") {
		mipsCodeGenerator(root->children.at(3), proc);
		cout << "beq $3, $11, 4" << endl;
		cout << "add $1, $3, $0" << endl;
		cout << "lis $5" << endl;
		cout << ".word delete" << endl;
		cout << "jalr $5" << endl;
	}
}

void mipsCode(Node* root, Procedure* proc) {
	cout << ".import print" << endl;
	cout << ".import init" << endl;
	cout << ".import new" << endl;
	cout << ".import delete" << endl;
	cout << "lis $4" << endl;
	cout << ".word 4" << endl;
	cout << "lis $11" << endl;
	cout << ".word 1" << endl;
	push("$31");
	if(proc->name == "wain" && proc->params.at(0) == "int") {
		cout << "add $5, $2, $0" << endl;
		cout << "add $2, $0, $0" << endl;
		cout << "lis $6" << endl;
		cout << ".word init"<< endl;
		cout << "jalr $6" << endl;
		cout << "add $2, $5, $0" << endl;
	} else if(proc->name == "wain" && proc->params.at(0) == "int*") {
		cout << "lis $6" << endl;
		cout << ".word init"<< endl;
		cout << "jalr $6" << endl;
	}
	cout << "add $29, $30, $0" << endl;
	cout << "sub $29, $29, $4" << endl;
	setLocations(proc->symbolTable);
	cout << "lis $5" << endl;
	cout << ".word " << proc->symbolTable.size()*4 << endl;
	cout << "sub $30, $30, $5" << endl;
	mipsCodeGenerator(root, proc);
	cout << "lis $5" << endl;
	cout << ".word " << proc->symbolTable.size()*4 << endl;
	cout << "add $30, $30, $5" << endl;
	pop("$31");
	cout << "add $29, $30, $0" << endl;
	cout << "jr $31" << endl;
}
/*
void checkIDs(Node* root, Procedure* proc) {
	string procName = proc->name;
	map<string, Symbol> symbolTable = proc->symbolTable;

	if(root->tokens.at(0) == "factor") {
		string token1 = root->children.at(0)->tokens.at(0);
		if(token1 == "ID") {
			string name = root->children.at(0)->tokens.at(1);
			if(root->children.size() == 1) {
				if(symbolTable.count(name) == 0) {
					cerr << "ERROR: identifier is used without being declared: " << name << endl;
				}
			} else if(root->children.size() == 3) {
				if(symbolTable.count(name) > 0 || (procedures.count(name) == 0 && name != procName)) {
					cerr << "ERROR: procedure is used without being declared: " << name << endl;
				}
			} else if(root->children.size() == 4) {
				if(symbolTable.count(name) > 0 || (procedures.count(name) == 0 && name != procName)) {
					cerr << "ERROR: procedure is used without being declared: " << name << endl;
				}
			}
		}
	} else if(root->tokens.at(0) == "lvalue") {
		string token1 = root->children.at(0)->tokens.at(0);
		if(token1 == "ID") {
			string name = root->children.at(0)->tokens.at(1);
			if(symbolTable.count(name) == 0) {
				cerr << "ERROR: identifier is used without being declared: " << name << endl;
			}
		}

		getType(root, proc);
	} else if(root->tokens.at(0) == "expr") {
		getType(root, proc);
	}

	if(!root->children.empty()) {
		for(int i=0; i < root->children.size(); i++) {
			checkIDs(root->children.at(i), proc);
		}
	}
}

void checkIDsProc(Node* root, Procedure* proc) {
	string name = root->tokens.at(0);
	if(name == "main") {
		checkIDs(root->children.at(9), proc);
		checkIDs(root->children.at(11), proc);
	} else {
		checkIDs(root->children.at(7), proc);
		checkIDs(root->children.at(9), proc);
	}
	wellTyped(root, proc);
	mipsCode(root, proc);
}
*/
void printSymbolTable(map<string, Symbol> table) {
	for (map<string, Symbol>::iterator it=table.begin(); it!=table.end(); ++it) {
    	cerr << it->first << " " << it->second.type << '\n';
	}
}

void createProcedures(Node* root) {
	if(root->tokens.at(0) == "main") {
		Procedure* wain = new Procedure();
		
		wain->name = "wain";
		cerr << wain->name;
		addParams(root, wain->symbolTable, wain->params);
		printParams(wain->params);

		addDcls(root->children.at(8), wain->symbolTable);
		//checkIDsProc(root, wain);
		mipsCode(root, wain);
		printSymbolTable(wain->symbolTable);
		procedures[wain->name] = *wain;
		return;
	} else if(root->tokens.at(0) == "procedure") {
		string name = root->children.at(1)->tokens.at(1);

		if(procedures.count(name) > 0) {
			cerr << "ERROR: procedure is declared more than once: " << name << endl;
		} else {
			Procedure* proc = new Procedure();
		
			cerr << name;
			proc->name = name;
			addParams(root, proc->symbolTable, proc->params);
			printParams(proc->params);

			addDcls(root->children.at(6), proc->symbolTable);
			//checkIDsProc(root, proc); 
			mipsCode(root, proc);
			printSymbolTable(proc->symbolTable);
			cerr << endl;
			procedures[proc->name] = *proc;
			return;
		}
	}

	if(!root->children.empty()) {
		for(int i=0; i < root->children.size(); i++) {
			createProcedures(root->children.at(i));
		}
	}
}

int main() {
	readTerminals();

	Node* start = new Node();
	createParseTree(cin, start);
	createProcedures(start);
	return 0;
}