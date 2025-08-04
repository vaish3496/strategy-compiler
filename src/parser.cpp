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
    if (current < tokens.size()) {
        cout << "DEBUG: current incremented to value: " << current+1 << endl;
        current++;
    }
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

    strategy.name = peek().lexeme;

    advance();
    
    
    
    if (peek().type != Entry) {
        throw runtime_error("Expected 'entry' keyword");
    }

    advance();

    if (peek().type != Colon) {
        throw runtime_error("Expected ':' after strategy name");
    }

    advance(); // Consume the ':'

    strategy.entry = make_unique<EntryStmt>(parseExpression());

    if (peek().type != Exit) {
        throw runtime_error("Expected 'exit' keyword");
    }

    advance(); // Consume the 'exit' keyword

    if (peek().type != Colon) {
        throw runtime_error("Expected ':' after strategy name");
    }

    advance(); // Consume the ':'

    strategy.exit = make_unique<ExitStmt>(parseExpression());

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

ExprPtr Parser::parseExpression() {
    cout << "DEBUG: parseExpression() called at current=" << current << endl;
    auto result = parseLogicOr();
    cout << "DEBUG: parseExpression() finished at current=" << current << endl;
    return result;
}

ExprPtr Parser::parseLogicOr() {
    cout << "DEBUG: parseLogicOr() called at current=" << current << endl;
    auto expr = parseLogicAnd();
    while (peek().type == TokenType::Or) {
        Token op = advance();
        auto right = parseLogicAnd();
        expr = std::make_unique<Expr>(BinaryExpr(std::move(expr), std::move(right), LogicalOr));
    }
    cout << "DEBUG: parseLogicOr() finished at current=" << current << endl;
    return expr;
}

ExprPtr Parser::parseLogicAnd() {
    auto expr = parseEquality();
    while (peek().type == TokenType::And) {
        Token op = advance();
        auto right = parseEquality();
        expr = std::make_unique<Expr>(BinaryExpr(std::move(expr), std::move(right), LogicalAnd));
    }
    return expr;
}

ExprPtr Parser::parseEquality() {
    auto expr = parseComparison();
    while (peek().type == TokenType::Equal) {
        Token op = advance();
        auto right = parseComparison();
        expr = std::make_unique<Expr>(BinaryExpr(std::move(expr), std::move(right), tokenToBinaryOperator(op)));
    }
    return expr;
}

ExprPtr Parser::parseComparison() {
    auto expr = parsePrimary();
    while (peek().type == TokenType::Greater || peek().type == TokenType::Less) {
        Token op = advance();
        auto right = parsePrimary();
        expr = std::make_unique<Expr>(BinaryExpr(std::move(expr), std::move(right), tokenToBinaryOperator(op)));
    }
    return expr;
}

ExprPtr Parser::parsePrimary() {
    if (peek().type == TokenType::Identifier) {
        Token name = advance();

        if (peek().type == TokenType::LParen) {
            advance(); // consume '('

            std::vector<ExprPtr> args;
            if (peek().type != TokenType::RParen) {
                do {
                    args.push_back(parseExpression());
                } while (match(TokenType::Comma));
            }

            if (!check(TokenType::RParen)) {
                throw runtime_error("Expected ')' after function arguments");
            }
            advance(); // consume ')'
            return std::make_unique<Expr>(FunctionCallExpr(name.lexeme, std::move(args)));
        } else {
            // Simple identifier - treat as function call with no args
            return std::make_unique<Expr>(FunctionCallExpr(name.lexeme, std::vector<ExprPtr>()));
        }
    }

    if (peek().type == TokenType::Number) {
        return std::make_unique<Expr>(LiteralExpr(stod(advance().lexeme)));
    }

    throw std::runtime_error("Unexpected token in expression: " + peek().lexeme);
}

double Parser::parseNumber() {
    if (!check(Number)) {
        throw runtime_error("Expected number");
    }
    return stod(advance().lexeme);
}

BinaryOperator Parser::tokenToBinaryOperator(const Token& token) const {
    switch (token.type) {
        case Greater: return GreaterThan;
        case Less: return LessThan;
        case And: return LogicalAnd;
        case Or: return LogicalOr;
        case Plus: return Add;
        case Minus: return Subtract;
        case Star: return Multiply;
        case Slash: return Divide;
        case Percent: return Modulus;
        default:
            throw runtime_error("Unknown binary operator: " + token.lexeme);
    }
}
