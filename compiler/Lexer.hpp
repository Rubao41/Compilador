#pragma once
#include "Token.hpp"
#include <string>
#include <unordered_map>

class Lexer{
    private:
        std::string source;
        int pos;
        int currentLine; //Aponta a linha do erro 
        std::unordered_map<std::string, TokenType> keywords;

        //Declara funções auxiliares
        char peek();
        char advance();
        void skipWhitespace();

    public:
        //Declara o construtor 
        Lexer(std::string sourceCode);
        Token nextToken();
};
