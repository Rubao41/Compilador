#pragma once
#include <memory>
#include <string>
#include <vector>

struct Node {
    virtual ~Node() = default;
};

struct WhileStmt : Node{
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> body;

    WhileStmt(std::unique_ptr<Node> c, std::unique_ptr<Node> b)
        : condition(std::move(c)), 
        body(std::move(b)) {}
};

struct ForStmt : Node{
    std::unique_ptr<Node> init;
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> increment;
    std::unique_ptr<Node> body;

    ForStmt(std::unique_ptr<Node> i, std::unique_ptr<Node> c,
            std::unique_ptr<Node> inc, std::unique_ptr<Node> b)
            : init(std::move(i)), 
            condition(std::move(c)),
            increment(std::move(inc)), 
            body(std::move(b)) {}
};

struct NumLiteral : Node {
    double value;
    NumLiteral(double v) : value(v) {}
};

struct StringLiteral : Node {
    std::string value;
    StringLiteral(std::string v) : value(std::move(v)) {}
};

struct VarRef : Node {
    std::string name;
    VarRef(std::string n) : name(std::move(n)) {}
};

struct BinOp : Node {
    std::string op;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    BinOp(std::string o, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
};

struct Decl : Node {
    std::string typeName;
    std::string varName;
    std::unique_ptr<Node> expr;
    Decl(std::string t, std::string n, std::unique_ptr<Node> e)
        : typeName(std::move(t)), varName(std::move(n)), expr(std::move(e)) {}
};

struct AssignStmt : Node {
    std::string varName;
    std::unique_ptr<Node> expr;
    AssignStmt(std::string n, std::unique_ptr<Node> e)
        : varName(std::move(n)), expr(std::move(e)) {}
};

struct PrintStmt : Node {
    std::unique_ptr<Node> expr;
    PrintStmt(std::unique_ptr<Node> e) : expr(std::move(e)) {}
};

struct InputStmt : Node {
    std::string varName;
    InputStmt(std::string v) : varName(std::move(v)) {}
};

struct Program : Node {
    std::vector<std::unique_ptr<Node>> statements;
    Program(std::vector<std::unique_ptr<Node>> stmts) : statements(std::move(stmts)) {}
};
