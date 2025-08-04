#include "token.hpp"
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <optional>

using namespace std;

enum BinaryOperator {
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulus,    
    GreaterThan,
    LessThan,
    LogicalAnd,
    LogicalOr,
    Not,
    If
};

struct Expr;
using ExprPtr = unique_ptr<Expr>;

struct LiteralExpr;
struct BinaryExpr;
struct FunctionCallExpr;

// ----------------------------
// Expr Variants
// ----------------------------

struct LiteralExpr {
    double value;
    explicit LiteralExpr(double val) : value(val) {}
};

struct BinaryExpr {
    ExprPtr left;
    ExprPtr right;
    BinaryOperator op;
    explicit BinaryExpr(ExprPtr left, ExprPtr right, BinaryOperator op) : left(std::move(left)), right(std::move(right)), op(op) {}
};

struct FunctionCallExpr {
    string name;
    vector<ExprPtr> args;
    explicit FunctionCallExpr(string name, vector<ExprPtr> args) : name(std::move(name)), args(std::move(args)) {}
};

// ----------------------------
// Expr Variant Definition
// ----------------------------

struct Expr {
    using Variant = variant<LiteralExpr, BinaryExpr, FunctionCallExpr>;
    Variant value;
    explicit Expr(Variant val) : value(std::move(val)) {}
};

// ----------------------------
// Statements
// ----------------------------

struct Statement {
    virtual ~Statement() {}
};

struct EntryStmt : public Statement {
    ExprPtr condition;
    explicit EntryStmt(ExprPtr condition) : condition(std::move(condition)) {}
};

struct ExitStmt : public Statement {
    ExprPtr condition;
    explicit ExitStmt(ExprPtr condition) : condition(std::move(condition)) {}
};

struct RiskStmt : public Statement {
    double stopLossPercent;
    explicit RiskStmt(double stopLossPercent) : stopLossPercent(stopLossPercent) {}
};

// ----------------------------
// Strategy Root Node
// ----------------------------

struct Strategy {
    string name;
    unique_ptr<EntryStmt> entry;
    unique_ptr<ExitStmt> exit;
    unique_ptr<RiskStmt> risk;
    explicit Strategy(string name = "", unique_ptr<EntryStmt> entry = nullptr, unique_ptr<ExitStmt> exit = nullptr, unique_ptr<RiskStmt> risk = nullptr) : name(std::move(name)), entry(std::move(entry)), exit(std::move(exit)), risk(std::move(risk)) {}
};

// ----------------------------
// Print Functions
// ----------------------------

void printExpression(const Expr& expr);
void printStrategy(const struct Strategy& strategy);



