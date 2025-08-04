#include "tokenizer.hpp"
#include "parser.hpp"
#include <iostream>

using namespace std;

int main() {
    string input = 
        "strategy \"VWAP Breakout\"\n"
        "entry: price() > vwap(20) and returns(5) > 0.02\n"
        "exit: price() < vwap(20)\n"
        "risk: 0.5\n";

    Tokenizer tokenizer(input);
    vector<Token> tokens = tokenizer.tokenize();

    Parser parser(tokens);
    struct Strategy strategy = parser.parseStrategy();

    // Print tokens
    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens[i];
        cout << Tokenizer::tokenTypeToString(token.type) << " → '" << token.lexeme << "'\n";
    }

    // Print parsed strategy in human-readable format
    printStrategy(strategy);

    return 0;
}
