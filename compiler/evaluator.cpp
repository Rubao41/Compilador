#include "Evaluator.hpp"

//Construtor
Evaluator::Evaluator() { 
    pushScope(); } // cria escopo global

OdysseyValue& Evaluator::getVarRef(const std::string& name) {
    for (auto it = envStack.rbegin(); it != envStack.rend(); ++it){
        auto found = it->find(name);
        if (found != it->end()) return found->second;
    }
    throw std::runtime_error("Erro Semantico: Variavel '" + name + "' não declarada.");
}

void Evaluator::declareVar(const std::string& name, const OdysseyValue& val){
    if (envStack.empty()) pushScope();
    envStack.back()[name] = val;
}

void Evaluator::setVar(const std::string& name, const OdysseyValue& val){
    for (auto it = envStack.rbegin(); it != envStack.rend(); ++it){
        auto found = it->find(name);
        if (found != it->end()) { found->second = val; return;}
    }
    throw std::runtime_error("Erro Semantico: Variavel '" + name +"' não declarada.");
}

OdysseyValue Evaluator::eval(Node* node) {
    if (!node) throw std::runtime_error("Nó de expressao nulo");

    if (auto* b = dynamic_cast<BoolLiteral*>(node)) {
        return {OdysseyValue::BOOL, 0, "", b->value};
    }
    // Literais
    if (auto* num = dynamic_cast<NumLiteral*>(node)) {
        return {OdysseyValue::INT, num->value, "", false};
    }
    if (auto* str = dynamic_cast<StringLiteral*>(node)) {
        return {OdysseyValue::STRING, 0, str->value, false};
    }

    // Referência a variável
    if (auto* var = dynamic_cast<VarRef*>(node)) {
        return getVarRef(var->name);
    }

    // Operações binárias
    if (auto* bin = dynamic_cast<BinOp*>(node)) {
        OdysseyValue left = eval(bin->left.get());
        OdysseyValue right = eval(bin->right.get());

        // Soma / concatenação
        if (bin->op == "+") {
            if (left.type == OdysseyValue::STRING && right.type == OdysseyValue::STRING) {
                return {OdysseyValue::STRING, 0, left.strVal + right.strVal, false};
            }
            if (left.type == OdysseyValue::INT && right.type == OdysseyValue::INT) {
                return {OdysseyValue::INT, left.numVal + right.numVal, "", false};
            }
            throw std::runtime_error("Erro Semantico: tipos incompativeis para a operacao +.");
        }

        // Operações matemáticas (exigem INT)
        if (bin->op == "-" || bin->op == "*" || bin->op == "/" || bin->op == "**") {
            if (left.type != OdysseyValue::INT || right.type != OdysseyValue::INT) {
                throw std::runtime_error("Erro Semantico: operandos matematicos devem ser inteiros.");
            }
            if (bin->op == "-") return {OdysseyValue::INT, left.numVal - right.numVal, "", false};
            if (bin->op == "*") return {OdysseyValue::INT, left.numVal * right.numVal, "", false};
            if (bin->op == "/") {
                if (right.numVal == 0) throw std::runtime_error("Erro de execucao: divisao por zero.");
                return {OdysseyValue::INT, left.numVal / right.numVal, "", false};
            }
            if (bin->op == "**") {
                double res = std::pow(left.numVal, right.numVal);
                return {OdysseyValue::INT, res, "", false};
            }
        }

        // Igualdade / desigualdade (suporta int/string/bool)
        if (bin->op == "==" || bin->op == "!=") {
            if (left.type != right.type) throw std::runtime_error("Erro Semantico: comparacao entre tipos diferentes.");
            bool result = false;
            if (left.type == OdysseyValue::INT) result = (left.numVal == right.numVal);
            else if (left.type == OdysseyValue::STRING) result = (left.strVal == right.strVal);
            else if (left.type == OdysseyValue::BOOL) result = (left.boolVal == right.boolVal);
            if (bin->op == "==") return {OdysseyValue::BOOL, 0, "", result};
            else return {OdysseyValue::BOOL, 0, "", !result};
        }

        // Relacionais >, >=, <, <= (apenas para números)
        if (bin->op == ">" || bin->op == ">=" || bin->op == "<" || bin->op == "<=") {
            if (left.type != OdysseyValue::INT || right.type != OdysseyValue::INT) {
                throw std::runtime_error("Erro Semantico: operadores relacionais exigem operandos inteiros.");
            }
            if (bin->op == ">") return {OdysseyValue::BOOL, 0, "", (left.numVal > right.numVal)};
            if (bin->op == ">=") return {OdysseyValue::BOOL, 0, "", (left.numVal >= right.numVal)};
            if (bin->op == "<") return {OdysseyValue::BOOL, 0, "", (left.numVal < right.numVal)};
            if (bin->op == "<=") return {OdysseyValue::BOOL, 0, "", (left.numVal <= right.numVal)};
        }

        throw std::runtime_error("Erro Semantico: operador binario desconhecido '" + bin->op + "'.");
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

    declareVar(stmt->varName, val);
}

void Evaluator::executeAssign(const AssignStmt* stmt) {
    if (!varExistsAnywhere(stmt->varName)) {
        throw std::runtime_error("Erro: Variavel '" + stmt->varName + "' não declarada");
    }
    OdysseyValue current = getVarRef(stmt->varName);
    OdysseyValue newVal = eval(stmt->expr.get());
    if (current.type != newVal.type) {
        throw std::runtime_error("Erro: atrbuição com tipo diferente para variavel");
    }
    setVar(stmt->varName, newVal);
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
    if (!varExistsAnywhere(stmt->varName)) {
        throw std::runtime_error("Erro: Variavel '" + stmt->varName + "' nao declarada antes do input.");
    }

    std::string entrada;
    std::getline(std::cin, entrada);

    OdysseyValue &target = getVarRef(stmt->varName);
    if (target.type == OdysseyValue::STRING) {
        target.strVal = entrada;
    } else if (target.type == OdysseyValue::INT){
        //Validação estrita: aceita apenas inteiros
        std::string s = entrada;
        //Trim espaços 
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        if (start == std::string::npos)
            throw std::runtime_error("Erro Semantico: entrada vazia para variavel int '" + stmt->varName + "'.");
        s = s.substr(start, end - start + 1);

        bool neg = false;
        size_t i = 0;
        if (s[0] == '+' || s[0] == '-') {neg = (s[0] == '-'); i = 1;}
        if (i >= s.size()) 
            throw std::runtime_error("Erro Semantico: formato invalido para int em '" + stmt->varName + "'.");
        for (; i < s.size(); ++i){
            if (!std::isdigit(static_cast<unsigned char>(s[i])))
                throw std::runtime_error("Erro Semantico: Voce tentou digitar um texto numa variavel do tipo int.");
        }
        //Conversão segura
        try{
            target.numVal = std::stoll(s);
        } catch (...){
            throw std::runtime_error("Erro Semantico: conversão para int falhou em '" + stmt->varName + "'.");
        }
    } else if (target.type == OdysseyValue::BOOL){
        //Aceita "true/false"
        std::string lower;
        for (char ch : entrada) lower += std::tolower(static_cast<unsigned char>(ch));
        if (lower == "true") target.boolVal = true;
        else if (lower == "false") target.boolVal = false;
        else throw std::runtime_error("Erro Semantico: entrada invalida para bool em '" + stmt->varName + "'.");
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
    pushScope();
    for (const auto& s : stmt-> statements){
        run(s.get());
    }
    popScope();
}

