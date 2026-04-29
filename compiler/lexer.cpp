#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>

//Enumeração e os tipos de tokens da Odyssey 
enum TokenType{
    //Palavras reservadas e tipos de dados
    T_INT_TYPE, T_STRING_TYPE, T_BOOL_TYPE,
    T_FUNC, T_RETURN, T_PRINT, T_INPUT, T_FOR, T_WHILE, T_TRUE, T_FALSE, T_NULL,

    //Identificadores e Literais 
    T_IDENTIFIER, T_NUMBER, T_STRING_LIT,

    //Operadores e Símbolos
    T_ASSIGN, //=
    T_PLUS,  //+
    T_MINUS, //-
    T_DIVIDE, // /
    T_MULTIPLY, // *
    T_EXPONENT, // **
    T_GRATER, // >
    T_LESS, // <
    T_EQUAL, // ==
    T_NOT_EQUAL, // !=
    T_GRATER_EQUAL, // >=
    T_LESS_EQUAL, // <=
    T_SEMICOLON, // ;
    T_OPEN_CURLY, // {
    T_CLOSE_CURLY, // }
    T_OPEN_PAREN, // (
    T_CLOSE_PAREN, // )

    //Controle 
    T_EOF, // Fim do arquivo
    T_ERROR // Token de erro
};

//Estrutura que guarda as informações lidas
struct Token{
    TokenType type;
    std::string value;
    int line;
};

class Lexer{
    private:
        std::string source;
        int pos;
        int currentLine; //Aponta a linha do erro 

//Mapa de palavras reservadas
    std::unordered_map<std::string, TokenType> keywords ={
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

    //Função para ler caracter sem avanaçar
    char peek() {
        if (pos >= source.length()) return '\0';
        return source[pos];
    }

    //Função para pegar caracter e avançar
    char advance() {
        char c = peek();
        pos++;
        return c;
    }

    //Ignorar espaços em branco e quebras de linha
    void skipWhitespace() {
        while (isspace(static_cast<unsigned char>(peek()))) {
            if (peek() == '\n') {
                currentLine++; //Conta as linhas 
            }
            advance();
        }
    }

public:
    //Construtor 
    Lexer(std::string sourceCode){
        source = sourceCode;
        pos = 0;
        currentLine = 1;
    }

//Pede a proxíma "palavra"
Token nextToken(){
    skipWhitespace();

        if (pos >= source.length()) {
            return {T_EOF, "EOF", currentLine};
        }

    char c = peek();

    // identifica simbolos e delimitadores
    if (c == ';') {
        advance();
        return {T_SEMICOLON, ";", currentLine};
    }
    if (c == '=') {
        advance();

        if (peek() == '=') {
            advance();
            return {T_EQUAL, "==", currentLine};
        }
        return {T_ASSIGN, "=", currentLine};
    }
        if (c == '{') {
            advance();
            return {T_OPEN_CURLY, "{", current_line};
        }
        if (c == '}') {
            advance();
            return {T_CLOSE_CURLY, "}", current_line};
        }
        if (c == '(') {
            advance();
            return {T_OPEN_PAREN, "(", current_line};
        }
        if (c == ')') {
            advance();
            return {T_CLOSE_PAREN, ")", current_line};
        }
        if (c == '+') {
            advance();
            return {T_PLUS, "+", current_line};
        }
        if (c == '-') {
            advance();
            return {T_MINUS, "-", current_line};
        }
        if (c == '/') {
            advance();
            return {T_DIVIDE, "/", current_line};
        }
    if ( c == '*') {
        advance();

            if (peek() == '*'){ //Olha o próximo caracter
                advance();
                return {T_EXPONENT, "**", current_line};
            }

        //Se não for '**', é apenas '*'
            return {T_MULTIPLY, "*", current_line};
    }
    if (c == '>') {
        advance();

            if (peek() == '=') {
                advance();
                return {T_GRATER_EQUAL, ">=", current_line};
            }
            return {T_GRATER, ">", current_line};
    }
    if (c == '<') {
        advance();

            if (peek() == '=') {
                advance();
                return {T_LESS_EQUAL, "<=", current_line};
            }
            return {T_LESS, "<", current_line};
    }
    if (c == '!') {
        advance();

            if (peek() == '=') {
                advance();
                return {T_NOT_EQUAL, "!=", current_line};
            }
            return {T_ERROR, "!", current_line};
    }

    //Indentifica números
        if (isdigit(static_cast<unsigned char>(c))) {
            std::string num = "";
            while (isdigit(static_cast<unsigned char>(peek()))) {
                num += advance();
            }
            return {T_NUMBER, num, current_line};
        }

    //Identifica palavras reservadas
        if (isalpha(static_cast<unsigned char>(c)) || c == '_') {
            std::string word = "";
            while (isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
                word += advance();
            }

            //Verifica se é reservada
            if (keywords.find(word) != keywords.end()) {
                return {keywords[word], word, current_line};
            }
            //Se não, é variável
            return {T_IDENTIFIER, word, current_line};
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
            return {T_STRING_LIT, str, current_line};
        }

    //Erro léxico ou caracter inválido
        std::string errorChar = "";
        errorChar += advance();
        return {T_ERROR, errorChar, current_line};
    }
};
