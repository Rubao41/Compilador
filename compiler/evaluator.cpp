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

        if (left.type != right.type) {
            throw std::runtime_error("Erro Semantico: tipos incompativeis para a operacao matematica.");
        }
        if (bin->op == "+") return {OdysseyValue::INT, left.numVal + right.numVal, "", false};
        if (bin->op == "-") return {OdysseyValue::INT, left.numVal - right.numVal, "", false};
        if (bin->op == "*") return {OdysseyValue::INT, left.numVal * right.numVal, "", false};
        if (bin->op == "/") return {OdysseyValue::INT, left.numVal / right.numVal, "", false};
        if (bin->op == "**") return {OdysseyValue::INT, std::pow(left.numVal, right.numVal), "", false};
        if (bin->op == ">") return {OdysseyValue::INT, (left.numVal > right.numVal) ? 1 : 0, "", false};
        if (bin->op == "<") return {OdysseyValue::INT, (left.numVal < right.numVal) ? 1 : 0, "", false};
        if (bin->op == "==") return {OdysseyValue::INT, (left.numVal == right.numVal) ? 1 : 0, "", false};
        if (bin->op == "!=") return {OdysseyValue::INT, (left.numVal != right.numVal) ? 1 : 0, "", false};
    }

    throw std::runtime_error("No de expressao desconhecido");
}

//execução do while
void Evaluator::executeWhile(const WhileStmt* stmt){
    //avalia a condição
    while (std::abs(eval(stmt->condition.get())) !=0){
        run(stmt->body.get());
    }
}

//execução do for
void Evaluator::executeFor(const ForStmt* stmt){
    // roda o init 1 vez
    if (stmt->init){
        run(stmt->init.get());
    }

    //testa a condição
    while ( std::abs(eval(stmt->condition.get())) !=0){

        //executa o body
        run(stmt->body.get());

        //executa o final do bloco
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