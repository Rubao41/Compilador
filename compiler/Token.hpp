#pragma once
#include <string>

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

inline std::string tokenTypeName(TokenType type){
    switch (type) {
        case T_INT_TYPE: return "T_INT_TYPE";
        case T_STRING_TYPE: return "T_STRING_TYPE";
        case T_BOOL_TYPE: return "T_BOOL_TYPE";
        case T_FUNC: return "T_FUNC";
        case T_RETURN: return "T_RETURN";
        case T_PRINT: return "T_PRINT";
        case T_INPUT: return "T_INPUT";
        case T_FOR: return "T_FOR";
        case T_WHILE: return "T_WHILE";
        case T_TRUE: return "T_TRUE";
        case T_FALSE: return "T_FALSE";
        case T_NULL: return "T_NULL";

        case T_IDENTIFIER: return "T_IDENTIFIER";
        case T_NUMBER: return "T_NUMBER";
        case T_STRING_LIT: return "T_STRING_LIT";

        case T_ASSIGN: return "T_ASSIGN";
        case T_PLUS: return "T_PLUS";
        case T_MINUS: return "T_MINUS";
        case T_DIVIDE: return "T_DIVIDE";
        case T_MULTIPLY: return "T_MULTIPLY";
        case T_EXPONENT: return "T_EXPONENT";
        case T_GRATER: return "T_GRATER";
        case T_LESS: return "T_LESS";
        case T_EQUAL: return "T_EQUAL";
        case T_NOT_EQUAL: return "T_NOT_EQUAL";
        case T_GRATER_EQUAL: return "T_GRATER_EQUAL";
        case T_LESS_EQUAL: return "T_LESS_EQUAL";
        case T_SEMICOLON: return "T_SEMICOLON";
        case T_OPEN_CURLY: return "T_OPEN_CURLY";
        case T_CLOSE_CURLY: return "T_CLOSE_CURLY";
        case T_OPEN_PAREN: return "T_OPEN_PAREN";
        case T_CLOSE_PAREN: return "T_CLOSE_PAREN";

        case T_EOF: return "T_EOF";
        case T_ERROR: return "T_ERROR";
        default: return "T_(unknown)";
    }
}