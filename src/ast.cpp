#include "ast.hpp"
#include <iostream>
using namespace std;

// Helper function to print expressions
void printExpression(const Expr& expr) {
    if (auto* literal = std::get_if<LiteralExpr>(&expr.value)) {
        cout << literal->value;
    } else if (auto* binary = std::get_if<BinaryExpr>(&expr.value)) {
        printExpression(*binary->left);
        
        switch (binary->op) {
            case GreaterThan: cout << " > "; break;
            case LessThan: cout << " < "; break;
            case LogicalAnd: cout << " and "; break;
            case LogicalOr: cout << " or "; break;
            case Add: cout << " + "; break;
            case Subtract: cout << " - "; break;
            case Multiply: cout << " * "; break;
            case Divide: cout << " / "; break;
            case Modulus: cout << " % "; break;
            default: cout << " ? "; break;
        }
        
        printExpression(*binary->right);
    } else if (auto* funcCall = std::get_if<FunctionCallExpr>(&expr.value)) {
        cout << funcCall->name << "(";
        for (size_t i = 0; i < funcCall->args.size(); i++) {
            if (i > 0) cout << ", ";
            printExpression(*funcCall->args[i]);
        }
        cout << ")";
    }
}

// Function to print the strategy in human-readable format
void printStrategy(const struct Strategy& strategy) {
    cout << "\n=== PARSED STRATEGY ===\n";
    cout << "Strategy: " << strategy.name << "\n";
    
    cout << "Entry: ";
    if (strategy.entry && strategy.entry->condition) {
        printExpression(*strategy.entry->condition);
    }
    cout << "\n";
    
    cout << "Exit: ";
    if (strategy.exit && strategy.exit->condition) {
        printExpression(*strategy.exit->condition);
    }
    cout << "\n";
    
    cout << "Risk: ";
    if (strategy.risk) {
        cout << strategy.risk->stopLossPercent << "%";
    }
    cout << "\n";
    cout << "======================\n\n";
}