#ifndef __KIND_H__
#define __KIND_H__
#include <string>
#include <iostream>
namespace ASM{
  // The different kinds of Tokens that are part of different WLP4 instructions
  // Used for determining the correct Token to create in the WLP4 recognizer
  enum Kind{
    ID,          // a string with a letter + zero or more letters and digits, but != "wain", "int", "if", "else", "while", "println", "return", "NULL", "new" or "delete"
    NUM,         // a string with a single digit or two+ digits the first != 0
    LPAREN,      // (
    RPAREN,      // )
    LBRACE,      // {
    RBRACE,      // }
    RETURN,      // return
    IF,          // if
    ELSE,        // else
    WHILE,       // while
    PRINTLN,     // println
    WAIN,        // wain
    BECOMES,     // =
    INT,         // int
    EQ,          // ==
    NE,          // !=
    LT,          // <
    GT,          // >
    LE,          // <=
    GE,          // >=
    PLUS,        // +
    MINUS,       // -
    STAR,        // *
    SLASH,       // /
    PCT,         // %
    COMMA,       // ,
    SEMI,        // ;
    NEW,         // new
    DELETE,      // delete
    LBRACK,      // [
    RBRACK,      // ]
    AMP,         // &
    NULLL,       // NULL
    WHITESPACE,  // Whitespace
    ERR          // Bad/Invalid Token
  };

  // A Token class representing the concrete functions we
  // might want to apply to a WLP4 Token

  class Token{
  protected:
    // The kind of the Token
    Kind kind;
    // The actual string representing the Token
    std::string lexeme;
  public:
    // A Factory Method that allows us to make specific Tokens
    // based upon the Kind
    static Token* makeToken(Kind kind, std::string lexeme);
    Token(Kind kind, std::string lexeme);
    // Convenience functions for operations we might like to
    // use on a Token
    std::string toString() const;
    std::string getLexeme() const;
    Kind getKind() const;
  };

  // Overload the output operator for Tokens
  std::ostream& operator<<(std::ostream& out, const Token& t);
}
#endif
