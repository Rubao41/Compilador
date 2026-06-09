#include <iostream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Evaluator.hpp"

int main() {
    // teste da odyssey For e While
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

    try {
        // 1) Apenas tokenizar (debug do lexer)
        Lexer lexer(codetest);
        Token t;
        std::cout << "--Odyssey (lexer) --\n";
        do {
            t = lexer.nextToken();
            std::cout << "linha " << t.line << "\ttoken: " << tokenTypeName(t.type)
                      << "\tvalor: '" << t.value << "'\n";
        } while (t.type != T_EOF && t.type != T_ERROR);

        // 2) Recria o lexer para o parser (o lexer anterior já consumiu o input)
        Lexer parserLexer(codetest);
        Parser parser(parserLexer);

        std::cout << "--- Compilando a Odyssey ---\n";
        auto arvoreSintatica = parser.parseProgram();

        std::cout << "--- Executando a Odyssey ---\n";
        Evaluator evaluator;
        evaluator.run(arvoreSintatica.get());
    }
    catch (const std::exception& ex) {
        std::cerr << "Erro: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}