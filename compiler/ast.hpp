#pragma once
#include <memory>

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