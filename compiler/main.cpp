#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Evaluator.hpp"

int main(int argc, char* argv[]) {
    // 1. Verifica se o usuário passou um arquivo
    if (argc < 2) {
        std::cerr << "Erro: Nenhum arquivo passado.\nUso correto: ./compilador.exe arquivo.cml\n";
        return 1;
    }

    // 2. Tenta abrir e ler o arquivo passado no terminal
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << argv[1] << "\n";
        return 1;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();

    try {
        // 3. Compilação e Execução Limpas
        Lexer parserLexer(sourceCode);
        Parser parser(parserLexer);

        auto arvoreSintatica = parser.parseProgram();

        Evaluator evaluator;
        evaluator.run(arvoreSintatica.get());
    }
    catch (const std::exception& ex) {
        // Imprime os erros bonitos da Odyssey
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}