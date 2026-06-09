#pragma once
#include "ast.hpp"
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cmath>

struct OdysseyValue {
    enum Type { INT, STRING, BOOL} type;
    double numVal = 0;
    std::string strVal = "";
    bool boolVal = false;
};

class Evaluator {
    private:
        std::map<std::string, OdysseyValue> env;
    
    public:
        void run(Node* node);
        OdysseyValue eval(Node* node);

        void executeProgram(const Program* prog);
        void executeWhile(const WhileStmt* stmt);
        void executeFor(const ForStmt* stmt);
        void executeDecl(const Decl* stmt);
        void executeAssign(const AssignStmt* stmt);
        void executePrint(const PrintStmt* stmt);
        void executeInput(const InputStmt* stmt);
        void executeBlock(const BlockStmt* stmt);
};

OdysseyValue Evaluator::eval(Node* node) {
    if (auto* num = dynamic_cast<NumLiteral*>(node)) {
        return {OdysseyValue::INT, num->value, "", false};
    }

    if (auto* str = dynamic_cast<StringLiteral*>(node)) {
        return {OdysseyValue::STRING, 0, str->value, false};
    }

    if (auto* var = dynamic_cast<VarRef*>(node)) {
        if (env.find(var->name) == env.end()) {
            throw std::runtime_error("Erro Semantico: Variavel '" + var->name + "' nao declarada");
        }
        return env[var->name];
    }

    if (auto* bin = dynamic_cast<BinOp*>(node)) {
        OdysseyValue left = eval(bin->left.get());
        OdysseyValue right = eval(bin->right.get());

        if (bin->op == "+"){
            if (left.type == OdysseyValue::STRING && right.type == OdysseyValue::STRING){
                return {OdysseyValue::STRING, 0, left.strVal + right.strVAl, false};
            }
            if (left.type == OdysseyValue::INT && right.type == OdysseyValue::INT){
                return {OdysseyValue::INT, left.numVal + right.numVal, "", false};
            }
            throw std::runtime_error("Erro Semantico: tipos incompativeis para a operacao +.");
        }
        
        //Operações Matemáticas de Inteiros
        if (left.type != OdysseyValue::INT || right.type != OdysseyValue::INT) {
            throw std::runtime_error("Erro Semantico: operandos matematicos devem ser inteiros.");
        }
        if (bin->op == "-") return {OdysseyValue::INT, left.numVal - right.numVal, "", false};
        if (bin->op == "*") return {OdysseyValue::INT, left.numVal * right.numVal, "", false};
        if (bin->op == "/") return {OdysseyValue::INT, left.numVal / right.numVal, "", false};
        if (bin->op == "**") return {OdysseyValue::INT, std::pow(left.numVal, right.numVal), "", false};

        //RElacionais (BOOL)
        if (bin->op == ">") return {OdysseyValue::BOOL, 0, "", (left.numVal > right.numVal)};
        if (bin->op == ">=") return {OdysseyValue::BOOL, 0, "", (left.numVal >= right.numVal)};
        if (bin->op == "<") return {OdysseyValue::BOOL, 0, "", (left.numVal < right.numVal)};
        if (bin->op == "<=") return {OdysseyValue::BOOL, 0, "", (left.numVal <= right.numVal)};
        if (bin->op == "==") return {OdysseyValue::BOOL, 0, "",(left.numVal == right.numVal)};
        if (bin->op == "!=") return {OdysseyValue::BOOL, 0, "",(left.numVal != right.numVal)};
    }

    throw std::runtime_error("Nó de expressao desconhecido");
}

//Execução do while
void Evaluator::executeWhile(const WhileStmt* stmt){
    while (true) {
        OdysseyValue cond = eval(stmt-> condition.get());
        if (cond.type != OdysseyValue::BOOL){
            throw std::runtime_error("Erro: A condeição de while deve ser do tipo booleano.");
        }
        if (!cond.boolVal) break; //Sai do laço se for false

        run(stmt->body.get());
    }
}

//Execução do for
void Evaluator::executeFor(const ForStmt* stmt){
    if (stmt->init){
        run (stmt->init.get());
    }
    while (true) {
        OdysseyValue cond = eval(stmt->condition.get());
        if (cond.type != OdysseyValue::BOOL){
            throw std::runtime_error("Erro: a condição do for deve ser do tipo booleano.");
        }
        if (!cond.boolVal) break;

        run(stmt->body.get());

        if (stmt->increment){
            run(stmt->increment.get());
        }
    }
}

void Evaluator::executeDecl(const Decl* stmt) {
    OdysseyValue val = eval(stmt->expr.get());

    if (stmt->typeName == "int" && val.type != OdysseyValue::INT) {
        throw std::runtime_error("Erro: tentando colocar valor nao inteiro");
    }
    if (stmt->typeName == "string" && val.type != OdysseyValue::STRING){
        throw std::runtime_error("Erro: tentando colocar valor nao string");
    }
    if (stmt->typeName == "bool" && val.type != OdysseyValue::BOOL){
        throw std::runtime_error("Erro: tentando colocar valor nao booleano");
    }

    env[stmt->varName] = val;
}

void Evaluator::executeAssign(const AssignStmt* stmt) {
    if (env.find(stmt->varName) == env.end()) {
        throw std::runtime_error("Erro: Variavel '" + stmt->varName + "' não declarada");
    }
    env[stmt->varName] = eval(stmt->expr.get());
}

void Evaluator::executePrint(const PrintStmt* stmt) {
    OdysseyValue val = eval(stmt->expr.get());
    
    if (val.type == OdysseyValue::INT) std::cout << val.numVal << "\n";
    else if (val.type == OdysseyValue::STRING) std::cout << val.strVal << "\n";
    else if (val.type == OdysseyValue::BOOL) std::cout << (val.boolVal ? "true" : "false") << "\n";
}

void Evaluator::executeProgram(const Program* prog) {
    for (const auto& stmt : prog->statements) {
        run(stmt.get());
    }
}

void Evaluator::executeInput(const InputStmt* stmt) {
    if (env.find(stmt->varName) == env.end()) {
        throw std::runtime_error("Erro: Variavel '" + stmt->varName + "' nao declarada antes do input.");
    }

    std::string entrada;
    std::getline(std::cin, entrada);

    if (env[stmt->varName].type == OdysseyValue::STRING) {
        env[stmt->varName].strVal = entrada;
    } 
    else if (env[stmt->varName].type == OdysseyValue::INT) {
        try {
            env[stmt->varName].numVal = std::stod(entrada);
        } catch (...) {
            throw std::runtime_error("Erro Semantico: Voce tentou digitar um texto numa variavel do tipo int.");
        }
    }
}

void Evaluator::run(Node* node) {
    if (!node) return;

    if (auto* prog = dynamic_cast<Program*>(node)) executeProgram(prog);
    else if (auto* blk = dynamic_cast<BlockStmt*>(node)) executeBlock(blk);
    else if (auto* decl = dynamic_cast<Decl*>(node)) executeDecl(decl);
    else if (auto* assign = dynamic_cast<AssignStmt*>(node)) executeAssign(assign);
    else if (auto* print = dynamic_cast<PrintStmt*>(node)) executePrint(print);
    else if (auto* input = dynamic_cast<InputStmt*>(node)) executeInput(input);
    else if (auto* whileStmt = dynamic_cast<WhileStmt*>(node)) executeWhile(whileStmt);
    else if (auto* forStmt = dynamic_cast<ForStmt*>(node)) executeFor(forStmt);
}

void Evaluator::executeBlock(const BlockStmt* stmt) {
    for (const auto& s : stmt-> statements){
        run(s.get());
    }
}