#pragma once
// ============================================
// ARQUIVO: ast.hpp
// PROPÓSITO: Definir a estrutura da Árvore Sintática Abstrata (AST)
// A AST representa o código compilado em forma de árvore
// ============================================

#include <memory>   // Para std::unique_ptr (apontadores inteligentes)
#include <string>   // Para std::string (texto)
#include <vector>   // Para std::vector (listas dinâmicas)

// ============================================
// CLASSE BASE: Node
// ============================================
// Esta é a classe PAI de todos os nós da árvore
// Todos os tipos de nós (NumLiteral, VarRef, etc) herdam dessa classe
// Assim, podemos tratar todos como "Node" através de polimorfismo
struct Node {
    // Destrutor virtual: permite deletar qualquer tipo de nó corretamente
    // virtual = permite que cada classe filha tenha sua própria versão
    // ~Node() = função que limpa a memória quando o nó é destruído
    virtual ~Node() = default;
};

// ============================================
// COMANDOS DE REPETIÇÃO (LOOPS)
// ============================================

// WHILE: while (condição) { corpo }
// Executa o corpo enquanto a condição for verdadeira
struct WhileStmt : Node {
    // A condição que é avaliada (ex: x < 10)
    // std::unique_ptr = apontador inteligente (limpa memória automaticamente)
    std::unique_ptr<Node> condition;
    
    // O corpo do while (comandos dentro das chaves)
    std::unique_ptr<Node> body;

    // CONSTRUTOR: função chamada quando criamos um WhileStmt
    // Recebe:
    //   - c = a condição
    //   - b = o corpo
    WhileStmt(std::unique_ptr<Node> c, std::unique_ptr<Node> b)
        : condition(std::move(c)),      // Armazena a condição
          body(std::move(b)) {}         // Armazena o corpo
};

// FOR: for (init; condição; incremento) { corpo }
// Inicializa, depois repete enquanto a condição é verdadeira
struct ForStmt : Node {
    // Inicialização: executada 1 vez no começo (ex: int i = 0)
    std::unique_ptr<Node> init;
    
    // Condição: avaliada a cada iteração (ex: i < 10)
    std::unique_ptr<Node> condition;
    
    // Incremento: executado a cada iteração (ex: i = i + 1)
    std::unique_ptr<Node> increment;
    
    // Corpo: os comandos dentro do for
    std::unique_ptr<Node> body;

    // CONSTRUTOR: recebe todos os 4 componentes do for
    ForStmt(std::unique_ptr<Node> i, std::unique_ptr<Node> c,
            std::unique_ptr<Node> inc, std::unique_ptr<Node> b)
            : init(std::move(i)),
              condition(std::move(c)),
              increment(std::move(inc)),
              body(std::move(b)) {}
};

// ============================================
// LITERAIS (Valores Concretos)
// ============================================

// NÚMERO: um valor numérico como 42 ou 3.14
// Um literal é um valor que você escreve direto no código
struct NumLiteral : Node {
    // Armazena o valor do número
    // double = pode ser inteiro ou decimal (ex: 42.0 ou 3.14)
    double value;

    // CONSTRUTOR: recebe um número e armazena
    NumLiteral(double v) : value(v) {}
};

// STRING: um texto como "olá mundo"
struct StringLiteral : Node {
    // Armazena o texto
    std::string value;

    // CONSTRUTOR: recebe um texto e armazena
    // std::move = otimização (não copia o texto, apenas move)
    StringLiteral(std::string v) : value(std::move(v)) {}
};

// BOOLEANO: true ou false
struct BoolLiteral : Node {
    // Armazena se é true ou false
    bool value;

    // CONSTRUTOR: recebe true/false e armazena
    BoolLiteral(bool v) : value(v) {}
};

// ============================================
// EXPRESSÕES (Cálculos e Referências)
// ============================================

// REFERÊNCIA A VARIÁVEL: usar uma variável (ex: x)
// Diferente de Decl que CRIA a variável
// VarRef é quando você USA uma variável já criada
struct VarRef : Node {
    // Nome da variável
    std::string name;

    // CONSTRUTOR: recebe o nome da variável
    VarRef(std::string n) : name(std::move(n)) {}
};

// OPERAÇÃO BINÁRIA: uma operação com 2 operandos (esquerda e direita)
// Exemplos: 5 + 3, x < 10, "olá" + " mundo"
struct BinOp : Node {
    // O operador ("+", "-", "*", "/", "<", ">", "==", etc)
    std::string op;
    
    // O lado ESQUERDO da operação (ex: 5 em "5 + 3")
    std::unique_ptr<Node> left;
    
    // O lado DIREITO da operação (ex: 3 em "5 + 3")
    std::unique_ptr<Node> right;

    // CONSTRUTOR: recebe operador e os 2 operandos
    BinOp(std::string o, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
        : op(std::move(o)),
          left(std::move(l)),
          right(std::move(r)) {}
};

// ============================================
// COMANDOS SIMPLES
// ============================================

// DECLARAÇÃO DE VARIÁVEL: int x = 5
// Cria uma variável, define seu tipo e valor inicial
struct Decl : Node {
    // O tipo da variável ("int", "string", "bool")
    std::string typeName;
    
    // O nome da variável (ex: "x")
    std::string varName;
    
    // O valor inicial (ex: NumLiteral(5))
    std::unique_ptr<Node> expr;

    // CONSTRUTOR: recebe tipo, nome e valor inicial
    Decl(std::string t, std::string n, std::unique_ptr<Node> e)
        : typeName(std::move(t)),
          varName(std::move(n)),
          expr(std::move(e)) {}
};

// ATRIBUIÇÃO: x = 10
// Muda o valor de uma variável já existente
// IMPORTANTE: diferente de Decl que CRIA a variável
// AssignStmt MUDA uma variável já criada
struct AssignStmt : Node {
    // Nome da variável que vai receber o novo valor
    std::string varName;
    
    // O novo valor (ex: NumLiteral(10))
    std::unique_ptr<Node> expr;

    // CONSTRUTOR: recebe nome da variável e novo valor
    AssignStmt(std::string n, std::unique_ptr<Node> e)
        : varName(std::move(n)),
          expr(std::move(e)) {}
};

// PRINT: print x
// Comando para mostrar algo na tela
struct PrintStmt : Node {
    // O que vai ser impresso (pode ser uma variável, número, expressão, etc)
    std::unique_ptr<Node> expr;

    // CONSTRUTOR: recebe a expressão que vai ser impressa
    PrintStmt(std::unique_ptr<Node> e) : expr(std::move(e)) {}
};

// INPUT: input x
// Comando para ler dados do teclado
struct InputStmt : Node {
    // Nome da variável que vai receber o valor digitado
    std::string varName;

    // CONSTRUTOR: recebe o nome da variável
    InputStmt(std::string v) : varName(std::move(v)) {}
};

// ============================================
// ESTRUTURAS COMPOSTAS
// ============================================

// BLOCO DE CÓDIGO: { comando1 ... comando2 }
// Um grupo de comandos entre chaves
// Usado em loops, ifs, etc
struct BlockStmt : Node {
    // Lista de todos os comandos dentro do bloco
    // std::vector = lista dinâmica (pode crescer)
    std::vector<std::unique_ptr<Node>> statements;

    // CONSTRUTOR: recebe a lista de comandos
    BlockStmt(std::vector<std::unique_ptr<Node>> stmts)
        : statements(std::move(stmts)) {}
};

// PROGRAMA: todo o código compilado
// A raiz da árvore sintática
struct Program : Node {
    // Lista de TODOS os comandos do programa
    // Cada statement é um comando de nível superior
    std::vector<std::unique_ptr<Node>> statements;

    // CONSTRUTOR: recebe a lista de todos os comandos
    Program(std::vector<std::unique_ptr<Node>> stmts)
        : statements(std::move(stmts)) {}
};

// ============================================
// ESTRUTURAS ESPECIAIS (Ainda em desenvolvimento)
// ============================================

// NÓ VAZIO: um comando que não faz nada
// Exemplo: um ponto-e-vírgula sozinho ;
struct NullStmt : Node {
    // Não armazena nada porque não faz nada
};

// DECLARAÇÃO DE FUNÇÃO: func saudacao() { ... }
// Ainda não totalmente implementado
// Armazena o nome da função
struct FuncDecl : Node {
    // Nome da função (ex: "saudacao")
    std::string name;

    // CONSTRUTOR: recebe o nome da função
    FuncDecl(std::string n) : name(std::move(n)) {}
};

// COMANDO RETURN: return valor
// Ainda não totalmente implementado
// Usado para retornar um valor de uma função
struct ReturnStmt : Node {
    // Nó vazio por enquanto
};

// ============================================
// RESUMO DA ESTRUTURA
// ============================================
// Node (classe base)
//  ├─ NumLiteral, StringLiteral, BoolLiteral  (literais/dados)
//  ├─ VarRef, BinOp                            (expressões)
//  ├─ Decl, AssignStmt, PrintStmt, InputStmt   (comandos)
//  ├─ WhileStmt, ForStmt                        (loops)
//  ├─ BlockStmt, Program                        (estruturas)
//  ├─ NullStmt                                  (vazio)
//  └─ FuncDecl, ReturnStmt                      (funções - não impl.)
// ============================================
