#include "tokenizer.hpp"
#include <iostream>

using namespace std;

int main() {
    string input = 
        "strategy \"VWAP Breakout\"\n"
        "entry: price() > vwap(20) and returns(5) > 0.02\n"
        "exit: price() < vwap(20)\n"
        "risk: stopLoss = 0.5%\n";

    Tokenizer tokenizer(input);
    vector<Token> tokens = tokenizer.tokenize();

    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens[i];
        cout << Tokenizer::tokenTypeToString(token.type) << " → '" << token.lexeme << "'\n";
    }

    return 0;
}
