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
    T_GREATER, // >
    T_LESS, // <
    T_EQUAL, // ==
    T_NOT_EQUAL, // !=
    T_GREATER_EQUAL, // >=
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
    const char* names [] = {
        "T_INT_TYPE", 
        "T_STRING_TYPE", 
        "T_BOOL_TYPE",
        "T_FUNC", 
        "T_RETURN", 
        "T_PRINT",
        "T_INPUT", 
        "T_FOR", 
        "T_WHILE", 
        "T_TRUE", 
        "T_FALSE", 
        "T_NULL",
        "T_IDENTIFIER", 
        "T_NUMBER", 
        "T_STRING_LIT",
        "T_ASSIGN", 
        "T_PLUS",  
        "T_MINUS",
        "T_DIVIDE", 
        "T_MULTIPLY", 
        "T_EXPONENT",
        "T_GREATER",
        "T_LESS", 
        "T_EQUAL", 
        "T_NOT_EQUAL", 
        "T_GREATER_EQUAL", 
        "T_LESS_EQUAL", 
        "T_SEMICOLON", 
        "T_OPEN_CURLY", 
        "T_CLOSE_CURLY", 
        "T_OPEN_PAREN", 
        "T_CLOSE_PAREN", 
        "T_EOF",
        "T_ERROR"
    };
    return names[type];
}