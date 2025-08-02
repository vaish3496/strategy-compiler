#include "parser.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include <iostream>

using namespace std;

Parser::Parser(const vector<Token>& tokens) : tokens(tokens) {}

const Token& Parser::peek() const {
    if (current >= tokens.size()) {
        throw runtime_error("Unexpected end of file");
    }
    return tokens[current];
}

const Token& Parser::peekNext() const {
    if (current + 1 >= tokens.size()) {
        throw runtime_error("Unexpected end of file");
    }
    return tokens[current + 1];
}

const Token& Parser::advance() {
    if (current < tokens.size()) current++;
    return tokens[current - 1];
}

bool Parser::check(TokenType type) const {
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}


struct Strategy Parser::parseStrategy() {
    struct Strategy strategy;
    if (peek().type != Strategy) {
        throw runtime_error("Expected 'strategy' keyword");
    }

    advance(); // Consume the 'strategy' keyword

    if (peek().type != String) {
        throw runtime_error("Expected strategy name");
    } 

    strategy.name = advance().lexeme;
    
    
    
    if (peek().type != Entry) {
        throw runtime_error("Expected 'entry' keyword");
    }

    advance();

    if (peek().type != Colon) {
        throw runtime_error("Expected ':' after strategy name");
    }

    advance(); // Consume the ':'

    strategy.entry = make_unique<EntryStmt>(parseExpression());

    advance();

    if (peek().type != Exit) {
        throw runtime_error("Expected 'exit' keyword");
    }

    advance(); // Consume the 'exit' keyword

    if (peek().type != Colon) {
        throw runtime_error("Expected ':' after strategy name");
    }

    advance(); // Consume the ':'

    strategy.exit = make_unique<ExitStmt>(parseExpression());

    advance();

    if (peek().type != Risk) {
        throw runtime_error("Expected 'risk' keyword");
    }

    advance(); // Consume the 'risk' keyword

    if (peek().type != Colon) {
        throw runtime_error("Expected ':' after strategy name");
    }

    advance(); // Consume the ':'

    strategy.risk = make_unique<RiskStmt>(parseNumber());
    
    return strategy;
    
}