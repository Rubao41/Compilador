#include <iostream>
#include "Lexer.hpp"

int main(){
    //teste da odyssey For e While
    std::string codetest = R"(
    //Teste do while
    int count = 0;
    while (count < 3){
        print(count);
        count = count + 1;
    }

    //Teste do for
    string salute = "Olá, alunos!";
    for (int i = 0; i < 2; i = i + 1) {
        print(salute);
    }
)";

Lexer lexer(codetest);
Token t;
std::cout << "--Odyssey--\n";
do{
    t = lexer.nextToken();
    std::cout << "linha " << t.line << "\ttoken: " << tokenTypeName(t.type) << "\tvalor: '" << t.value << "'\n";
} while (t.type != T_EOF && t.type != T_ERROR);

    return 0;
}

Lexer lexer(codigoTeste);
    Parser parser(lexer);
    
    std::cout << "--- Compilando a Odyssey ---\n";
    auto arvoreSintatica = parser.parseProgram(); 
    
    std::cout << "--- Executando a Odyssey ---\n";
    Evaluator evaluator;
    evaluator.run(arvoreSintatica.get()); 
