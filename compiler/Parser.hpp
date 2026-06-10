#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Token.hpp"
#include "Lexer.hpp"
#include "ast.hpp"

// Declaração mínima da classe Parser usada pelo seu Parser.cpp
class Parser {
public:
    // construtor: recebe um Lexer por referência
    Parser(Lexer& lexer);

    // funções de parsing que seu Parser.cpp implementa
    std::unique_ptr<Node> parseProgram();
    std::unique_ptr<Node> parseStatement();
    std::unique_ptr<Node> parseDecl(bool consumeSemicolon = true);
    std::unique_ptr<Node> parseAssign(bool consumeSemicolon = true);
    std::unique_ptr<Node> parseInput();
    std::unique_ptr<Node> parsePrint();
    std::unique_ptr<Node> parseBlock();
    std::unique_ptr<Node> parseExpression();
    std::unique_ptr<Node> parseComparison();
    std::unique_ptr<Node> parseAdditive();
    std::unique_ptr<Node> parseTerm();
    std::unique_ptr<Node> parseExponent();
    std::unique_ptr<Node> parseFactor();
    std::unique_ptr<Node> parseForStmt();
    std::unique_ptr<Node> parseWhileStmt();

    // helpers que seu Parser.cpp provavelmente usa
    Token current() const;
    void advance();
    void expect(TokenType type);

private:
    Lexer& lexer;
    Token lookahead; // opcional: se seu Parser.cpp usa uma variável assim
    // você pode ajustar conforme a implementação real
};
