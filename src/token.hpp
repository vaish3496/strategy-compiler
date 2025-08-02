#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum TokenType {
    // Keywords
    Strategy, Entry, Exit, Risk, Let, Def, After, And, Or,

    // Literals
    Identifier, String, Number, Boolean, Null,

    // Operators
    Equal, Plus, Minus, Star, Slash, Percent,
    Greater, Less, GreaterEqual, LessEqual,
    DoubleEqual, NotEqual,

    // Delimiters
    Colon, Comma, LParen, RParen,

    // Special
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    
    Token(TokenType type, const std::string& lexeme = "", int line = 0, int column = 0) : type(type), lexeme(std::move(lexeme)), line(line), column(column) {}
};

#endif