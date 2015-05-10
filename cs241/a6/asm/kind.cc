#include "kind.h"
#include <sstream>
using std::string;
using std::istringstream;

// Use the annonymous namespace to prevent external linking
namespace {
  // Strings representing the Kind of Token for printing
  const string kindStrings[] ={
    "ID",           // ID
    "NUM",          // NUM
    "LPAREN",       // LPAREN
    "RPAREN",       // RPAREN
    "LBRACE",       // LBRACE
    "RBRACE",       // RBRACE
    "RETURN",       // RETURN
    "IF",           // IF
    "ELSE",         // ELSE
    "WHILE",        // WHILE
    "PRINTLN",      // PRINTLN
    "WAIN",         // WAIN
    "BECOMES",      // BECOMES
    "INT",          // INT
    "EQ",           // EQ
    "NE",           // NE
    "LT",           // LT
    "GT",           // GT
    "LE",           // LE
    "GE",           // GE
    "PLUS",         // PLUS
    "MINUS",        // MINUS
    "STAR",         // STAR
    "SLASH",        // SLASH
    "PCT",          // PCT
    "COMMA",        // COMMA
    "SEMI",         // SEMI
    "NEW",          // NEW
    "DELETE",       // DELETE
    "LBRACK",       // LBRACK
    "RBRACK",       // RBRACK
    "AMP",          // AMP
    "NULL",         // NULLL
    "WHITESPACE",   // WHITESPACE
    "ERR"           // ERR
  };
}

ASM::Token::Token(ASM::Kind kind, string lexeme)
: kind(kind), lexeme(lexeme){}

string ASM::Token::toString() const{
  return ::kindStrings[kind];
}

string ASM::Token::getLexeme() const{
  return lexeme;
}

ASM::Kind ASM::Token::getKind() const{
  return kind;
}

// Print the pertinent information to the ostream
std::ostream& ASM::operator<<(std::ostream& out, const ASM::Token& t){
  out << t.toString() << " " << t.getLexeme();
  return out;
}

// Make a new Token depending on the Kind provided
// Integral Kinds correspond to the appropriate Token type
ASM::Token* ASM::Token::makeToken(ASM::Kind kind, string lexeme){
  return new Token(kind,lexeme);
}
