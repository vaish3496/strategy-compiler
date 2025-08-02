#pragma once

#include "ast.hpp"
#include "tokenizer.hpp"

class Parser {
    public:
        explicit Parser(const vector<Token>& tokens);

        struct Strategy parseStrategy();
    
    private:
        const vector<Token>& tokens;
        int current = 0;

        const Token& peek() const;
        const Token& peekNext() const;
        const Token& advance();
        bool check(TokenType type) const;
        bool match(TokenType type);
        
        
        string parseIdentifier();
        unique_ptr<EntryStmt> parseEntry();
        unique_ptr<ExitStmt> parseExit();
        unique_ptr<RiskStmt> parseRisk();
        ExprPtr parseExpression();
        ExprPtr parseBinaryExpression();
        ExprPtr parsePrimary();
        ExprPtr parseFunctionCallOrIdentifier();
        double parseNumber();

        BinaryOperator tokenToBinaryOperator(const Token& type) const;
};