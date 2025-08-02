#include "tokenizer.hpp"
#include <iostream>
#include <cctype>

using namespace std;

Tokenizer::Tokenizer(const string& source) : source(source) {}

vector<Token> Tokenizer::tokenize() {
    vector<Token> tokens;

    while (!isAtEnd()) {
        skipWhitespace();
        start = current;

        if (isAtEnd()) break;

        char c = advance();
        switch (c) {
            case '(': tokens.push_back(makeToken(LParen)); break;
            case ')': tokens.push_back(makeToken(RParen)); break;
            case ':': tokens.push_back(makeToken(Colon)); break;
            case ',': tokens.push_back(makeToken(Comma)); break;
            case '+': tokens.push_back(makeToken(Plus)); break;
            case '-': tokens.push_back(makeToken(Minus)); break;
            case '*': tokens.push_back(makeToken(Star)); break;
            case '/': tokens.push_back(makeToken(Slash)); break;
            case '=':
                tokens.push_back(makeToken(match('=') ? DoubleEqual : Equal));
                break;
            case '>':
                tokens.push_back(makeToken(match('=') ? GreaterEqual : Greater));
                break;
            case '<':
                tokens.push_back(makeToken(match('=') ? LessEqual : Less));
                break;
            case '!':
                tokens.push_back(makeToken(match('=') ? NotEqual : Unknown));
                break;
            case '"':
                tokens.push_back(stringLiteral()); break;
            case '%':
                tokens.push_back(makeToken(Percent));
                break;

            default:
                if (std::isdigit(c)) {
                    tokens.push_back(number());
                } else if (std::isalpha(c)) {
                    tokens.push_back(identifier());
                } else {
                    tokens.push_back(makeToken(Unknown));
                }
        }
    }

    tokens.push_back(Token(EndOfFile, "", line, column));
    return tokens;
}

bool Tokenizer::isAtEnd() const {
    return current >= source.length();
}

char Tokenizer::advance() {
    current++;
    column++;
    return source[current-1];
}

char Tokenizer::peek() const {
    return isAtEnd() ? '\0' : source[current];
}

char Tokenizer::peekNext() const {
    return (current + 1 >= source.size()) ? '\0' : source[current + 1];
}

char Tokenizer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++;
    column++;
    return true;
}

void Tokenizer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t') {
            advance();
        } else if (c == '\n') {
            line++;
            column = 1;
            advance();
        } else {
            break;
        }
    }
}

Token Tokenizer::makeToken(TokenType type) {
    string lexeme = source.substr(start, current - start);
    return Token(type, lexeme, line, column);
}

Token Tokenizer::identifier() {
    while (isalnum(peek()) || peek() == '_') advance();
    string lexeme = source.substr(start, current - start);
    TokenType type = checkKeyword(lexeme);
    return Token(type, lexeme, line, column);
}

TokenType Tokenizer::checkKeyword(const string& keyword) {
    if (keyword == "strategy") return Strategy;
    if (keyword == "entry") return Entry;
    if (keyword == "exit") return Exit;
    if (keyword == "risk") return Risk;
    if (keyword == "let") return Let;
    if (keyword == "def") return Def;
    if (keyword == "after") return After;
    if (keyword == "and") return And;
    if (keyword == "or") return Or;
    if (keyword == "true") return Boolean;
    if (keyword == "false") return Boolean;
    if (keyword == "null") return Null;
    return Identifier;
}

Token Tokenizer::number() {
    while (isdigit(peek())) advance();
    if (peek() == '.' && isdigit(peekNext())) {
        advance();
        while (isdigit(peek())) advance();
    }
    return makeToken(Number);
}

Token Tokenizer::stringLiteral() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
            column = 1;
        }
        advance();
    }
    if (isAtEnd()) return Token(Unknown, "Unterminated string", line, column);

    advance();
    string lexeme = source.substr(start + 1, current - start - 2);
    return Token(String, lexeme, line, column);
}

string Tokenizer::tokenTypeToString(TokenType type) {
    switch (type) {
        case Strategy: return "Strategy";
        case Entry: return "Entry";
        case Exit: return "Exit";
        case Risk: return "Risk";
        case Let: return "Let";
        case Def: return "Def";
        case After: return "After";
        case And: return "And";
        case Or: return "Or";
        case Identifier: return "Identifier";
        case String: return "String";
        case Number: return "Number";
        case Boolean: return "Boolean";
        case Null: return "Null";
        case Equal: return "Equal";
        case Plus: return "Plus";
        case Minus: return "Minus";
        case Star: return "Star";
        case Slash: return "Slash";
        case Percent: return "Percent";
        case Greater: return "Greater";
        case Less: return "Less";
        case GreaterEqual: return "GreaterEqual";
        case LessEqual: return "LessEqual";
        case DoubleEqual: return "DoubleEqual";
        case NotEqual: return "NotEqual";
        case Colon: return "Colon";
        case Comma: return "Comma";
        case LParen: return "LParen";
        case RParen: return "RParen";
        case EndOfFile: return "EndOfFile";
        case Unknown: return "Unknown";
        default: return "Unknown";
    }
}

