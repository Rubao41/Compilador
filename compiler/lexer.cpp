#include "Lexer.hpp"
#include <cctype>

public:
    //Construtor 
    Lexer::Lexer(std::string sourceCode){
        source = sourceCode;
        pos = 0;
        currentLine = 1;
    
        keywords ={
            {"int", T_INT_TYPE},
            {"string", T_STRING_TYPE},
            {"bool", T_BOOL_TYPE},
            {"func", T_FUNC},
            {"return", T_RETURN},
            {"print", T_PRINT},
            {"input", T_INPUT},
            {"for", T_FOR},
            {"while", T_WHILE},
            {"true", T_TRUE},
            {"false", T_FALSE},
            {"null", T_NULL}
    };
}
    //Função para ler caracter sem avanaçar
    char Lexer::peek() {
        if (pos >= source.length()) return '\0';
        return source[pos];
    }

    //Função para pegar caracter e avançar
    char Lexer::advance() {
        char c = peek();
        pos++;
        return c;
    }

    //Ignorar espaços em branco e quebras de linha
    void Lexer::skipWhitespace() {
        while (pos < source.length() && isspace(static_cast<unsigned char>(peek()))) {
            if (peek() == '\n') {
                currentLine++; //Conta as linhas 
            }
            advance();
        }
    }

//Pede a proxíma "palavra"
Token Lexer::nextToken(){
    skipWhitespace();

        if (pos >= source.length()) {
            return {T_EOF, "EOF", currentLine};
        }

    char c = peek();

    // identifica simbolos e delimitadores
    switch (c){
        case ';': advance(); return {T_SEMICOLON, ";", currentLine};
        case '{': advance(); return {T_OPEN_CURLY, "{", currentLine};
        case '}': advance(); return {T_CLOSE_CURLY, "}", currentLine};
        case '(': advance(); return {T_OPEN_PAREN, "(", currentLine};
        case ')': advance(); return {T_CLOSE_PAREN, ")", currentLine};
        case '+': advance(); return {T_PLUS, "+", currentLine};
        case '-': advance(); return {T_MINUS, "-", currentLine};
        case '/': advance(); return {T_DIVIDE, "/", currentLine};
    }

    if (c == '=') {
        advance();

        if (peek() == '=') {
            advance();
            return {T_EQUAL, "==", currentLine};
        }
        return {T_ASSIGN, "=", currentLine};
    }
    if ( c == '*') {
        advance();

            if (peek() == '*'){ //Olha o próximo caracter
                advance();
                return {T_EXPONENT, "**", currentLine};
            }

        //Se não for '**', é apenas '*'
            return {T_MULTIPLY, "*", currentLine};
    }
    if (c == '>') {
        advance();

            if (peek() == '=') {
                advance();
                return {T_GRATER_EQUAL, ">=", currentLine};
            }
            return {T_GRATER, ">", currentLine};
    }
    if (c == '<') {
        advance();

            if (peek() == '=') {
                advance();
                return {T_LESS_EQUAL, "<=", currentLine};
            }
            return {T_LESS, "<", currentLine};
    }
    if (c == '!') {
        advance();

            if (peek() == '=') {
                advance();
                return {T_NOT_EQUAL, "!=", currentLine};
            }
            return {T_ERROR, "!", currentLine};
    }

    //Indentifica números
        if (isdigit(static_cast<unsigned char>(c))) {
            std::string num = "";
            while (isdigit(static_cast<unsigned char>(peek()))) {
                num += advance();
            }
            return {T_NUMBER, num, currentLine};
        }

    //Identifica palavras reservadas
        if (isalpha(static_cast<unsigned char>(c)) || c == '_') {
            std::string word = "";
            while (isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
                word += advance();
            }

            //Verifica se é reservada
            if (keywords.find(word) != keywords.end()) {
                return {keywords[word], word, currentLine};
            }
            //Se não, é variável
            return {T_IDENTIFIER, word, currentLine};
        }

    //Identifica strings 
        if (c == '"') {
            std::string str = "";
            advance();
            while (peek() != '"' && peek() != '\0') {
                str += advance();
            }
            if (peek() == '"')
                advance();
            return {T_STRING_LIT, str, currentLine};
        }

    //Erro léxico ou caracter inválido
        std::string errorChar = "";
        errorChar += advance();
        return {T_ERROR, errorChar, currentLine};
    }

