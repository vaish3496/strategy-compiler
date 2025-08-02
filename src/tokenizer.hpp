#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include "token.hpp"

using namespace std;

class Tokenizer {
    public:
        explicit Tokenizer(const string& source);
        vector<Token> tokenize();
        static string tokenTypeToString(TokenType type);

    private:
        const string source;
        int start = 0;
        int current = 0;
        int line = 1;
        int column = 1;

        bool isAtEnd() const;
        char advance();
        char peek() const;
        char peekNext() const;
        char match(char expected);
        void skipWhitespace();

        Token makeToken(TokenType type);
        Token identifier();
        Token number();
        Token stringLiteral();

        TokenType checkKeyword(const string& keyword);
};

#endif
