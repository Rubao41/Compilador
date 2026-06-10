#include <memory>
#include <string>
#include "ast.hpp"
#include "Token.hpp"
#include "Parser.hpp"
#include <stdexcept>


std::unique_ptr<Node> Parser::parseExpression(){
    return parseComparison();
}

//Parse while
std::unique_ptr<Node> Parser::parseWhileStmt(){
    expect(T_WHILE);
    expect(T_OPEN_PAREN);

    auto cond = parseExpression();
    expect(T_CLOSE_PAREN);

    auto body = parseBlock();
    return std::make_unique<WhileStmt>(std::move(cond), std::move(body));
}

//Parse for
std::unique_ptr<Node> Parser::parseForStmt(){
    expect(T_FOR);
    expect(T_OPEN_PAREN);

    std::unique_ptr<Node> init = nullptr;
    //Tenta declarar variável
    if (current().type == T_INT_TYPE || current().type == T_STRING_TYPE || current().type == T_BOOL_TYPE) {
        init = parseDecl(false); //Não exige ';'
    }
    //Tenta atualizar variável existente
    else if (current().type == T_IDENTIFIER){
        init = parseAssign(false); //Não exige ';'
    }
    expect(T_SEMICOLON);

    auto cond = parseExpression();
    expect(T_SEMICOLON);

    std::unique_ptr<Node> inc = nullptr;
    if (current().type == T_IDENTIFIER) {
        inc = parseAssign(false); //O incremento i = i + 1 é um Assignment, não uma Expression isolada!
    }
    expect(T_CLOSE_PAREN);

    auto body = parseBlock();

    return std::make_unique<ForStmt> (std::move(init), std::move(cond), std::move(inc), std::move(body));
}

//Comparações (>,<, ==)
std::unique_ptr<Node> Parser::parseComparison() {
    auto left = parseAdditive();

    while (current().type == T_GREATER || current().type == T_LESS || current().type == T_EQUAL || 
    current().type == T_NOT_EQUAL || current().type == T_GREATER_EQUAL || current().type == T_LESS_EQUAL) {
        std::string op = current().value;
        advance();
        auto right = parseAdditive();
        left = std::make_unique<BinOp>(op, std::move(left), std::move(right));
    }
    return left;
}

//(+, -)
std::unique_ptr<Node> Parser::parseAdditive() {
    auto left = parseTerm();

    while (current().type == T_PLUS || current().type == T_MINUS) {
        std::string op = current().value;
        advance();
        auto right = parseTerm();
        left = std::make_unique<BinOp>(op, std::move(left), std::move(right));
    }
    return left;
}

//(*, /)
std::unique_ptr<Node> Parser::parseTerm() {
    auto left = parseExponent();

    while (current().type == T_MULTIPLY || current().type == T_DIVIDE) {
        std::string op = current().value;
        advance();
        auto right = parseExponent();
        left = std::make_unique<BinOp>(op, std::move(left), std::move(right));
    }
    return left;
}

//(**)
std::unique_ptr<Node> Parser::parseExponent() {
    auto left = parseFactor();

    while (current().type == T_EXPONENT){
        std::string op = current().value;
        advance();
        auto right = parseFactor();
        left = std::make_unique<BinOp>(op, std::move(left), std::move(right));
    }
    return left;
}

//fatores
std::unique_ptr<Node> Parser::parseFactor() {
    Token t = current();

    if (t.type == T_ERROR) {
        throw std::runtime_error("Erro Lexico na linha " + std::to_string(t.line) + ": " + t.value);
    }

    if (t.type == T_NUMBER) {
        advance();
        double val = 0;
        
        if (t.value.length() > 2 && t.value[0] == '0') {
            char base = t.value[1];
            if (base == 'x' || base == 'X') {
                val = std::stoi(t.value.substr(2), nullptr, 16);
            } else if (base == 'b' || base == 'B') {
                val = std::stoi(t.value.substr(2), nullptr, 2);
            } else if (base == 'o' || base == 'O') {
                val = std::stoi(t.value.substr(2), nullptr, 8);
            } else {
                val = std::stod(t.value);
            }
        } else {
            val = std::stod(t.value);
        }
        
        return std::make_unique<NumLiteral>(val);
    }
    else if (t.type == T_TRUE) {
        advance();
        return std::make_unique<BoolLiteral>(true);
    }
    else if (t.type == T_FALSE) {
        advance();
        return std::make_unique<BoolLiteral>(false);
    }
    else if (t.type == T_STRING_LIT) {
        advance();
        return std::make_unique<StringLiteral>(t.value);
    }
    else if (t.type == T_IDENTIFIER) {
        advance();
        return std::make_unique<VarRef>(t.value);
    }
    else if (t.type == T_OPEN_PAREN) {
        advance();
        auto expr = parseComparison();
        expect(T_CLOSE_PAREN);
        return expr;
    }
    
    throw std::runtime_error("Erro de sintaxe na linha " + std::to_string(t.line));
}


std::unique_ptr<Node> Parser::parseStatement() {
    Token t = current();
    if (t.type == T_INT_TYPE || t.type == T_STRING_TYPE || t.type == T_BOOL_TYPE) {
        return parseDecl();
    }
    else if (t.type == T_IDENTIFIER) {
        return parseAssign();
    }
    else if (t.type == T_PRINT) {
        return parsePrint();
    }
    else if (t.type == T_INPUT) {
        return parseInput();
    }
    else if (t.type == T_FOR) {
        return parseForStmt();
    }
    else if (t.type == T_WHILE) {
        return parseWhileStmt();
    }
    else if (t.type == T_RETURN) {
        advance();
        expect(T_SEMICOLON);
        return std::make_unique<ReturnStmt>();
    }
    else if (t.type == T_NULL) {
    advance();
    expect(T_SEMICOLON);
    return std::make_unique<NullStmt>();
    }
    else if (t.type == T_FUNC) {
        advance();
        std::string funcName = current().value;
        expect(T_IDENTIFIER);
        expect(T_OPEN_PAREN);
        expect(T_CLOSE_PAREN);

    auto body = parseBlock(); // Vai ler o { return; }

    return std::make_unique<FuncDecl>(funcName);
}

    throw std::runtime_error("Erro de sintaxe na linha " + std::to_string(t.line));
}

std::unique_ptr<Node> Parser::parseDecl(bool consumeSemicolon) {
    std::string typeName = current().value;
    advance();

    std::string varName = current().value;
    expect(T_IDENTIFIER);
    expect(T_ASSIGN);

    auto expr = parseComparison(); 

    if (consumeSemicolon) expect(T_SEMICOLON);

    return std::make_unique<Decl>(typeName, varName, std::move(expr));
}

std::unique_ptr<Node> Parser::parseAssign(bool consumeSemicolon) {
    std::string varName = current().value;
    expect(T_IDENTIFIER);
    expect(T_ASSIGN);

    auto expr = parseComparison();

    if (consumeSemicolon) expect(T_SEMICOLON);

    return std::make_unique<AssignStmt>(varName, std::move(expr));
}

std::unique_ptr<Node> Parser::parseInput() {
    expect(T_INPUT);

    expect(T_OPEN_PAREN);

    std::string varName = current().value;
    expect(T_IDENTIFIER);

    expect(T_CLOSE_PAREN);

    expect(T_SEMICOLON);

    return std::make_unique<InputStmt>(varName);
}

std::unique_ptr<Node> Parser::parsePrint() {
    expect(T_PRINT);

    expect(T_OPEN_PAREN);

    auto expr = parseComparison();

    expect(T_CLOSE_PAREN);

    expect(T_SEMICOLON);

    return std::make_unique<PrintStmt>(std::move(expr));
}

std::unique_ptr<Node> Parser::parseProgram() {
    std::vector<std::unique_ptr<Node>> statements;
    
    while (current().type != T_EOF) {
        statements.push_back(parseStatement());
    }
    
    return std::make_unique<Program>(std::move(statements));
}

std::unique_ptr<Node> Parser::parseBlock() {
    expect(T_OPEN_CURLY); 
    
    std::vector<std::unique_ptr<Node>> statements;
    
    while (current().type != T_CLOSE_CURLY && current().type != T_EOF) {
        statements.push_back(parseStatement());
    }
    
    expect(T_CLOSE_CURLY); 
    
    return std::make_unique<BlockStmt>(std::move(statements));
}

// Implementações do Parser
Parser::Parser(Lexer& lexer) : lexer(lexer) {
    lookahead = lexer.nextToken();
}

Token Parser::current() const {
    return lookahead;
}

void Parser::advance() {
    lookahead = lexer.nextToken();
}

void Parser::expect(TokenType type) {
    if (current().type != type) {
        throw std::runtime_error("Erro de Sintaxe na linha " + std::to_string(current().line) + 
                                 ": Esperado " + tokenTypeName(type) + 
                                 " mas encontrou '" + current().value + "'");
    }
    advance();
}