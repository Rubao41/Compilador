#include "Lexer.hpp"
#include <cctype>

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
//Função para ler caracter sem avançar
char Lexer::peek() {
    if (pos >= (int)source.length()) return '\0';
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
    while (pos < (int)source.length() && isspace(static_cast<unsigned char>(peek()))) {
        if (peek() == '\n') currentLine++;
        advance();
    }
}

//Pede a próxima "palavra"
Token Lexer::nextToken(){
    skipWhitespace();

    if (pos >= (int)source.length()) {
        return {T_EOF, "EOF", currentLine};
    }

    char c = peek();

    // símbolos
    switch (c){
        case ';': advance(); return {T_SEMICOLON, ";", currentLine};
        case '{': advance(); return {T_OPEN_CURLY, "{", currentLine};
        case '}': advance(); return {T_CLOSE_CURLY, "}", currentLine};
        case '(': advance(); return {T_OPEN_PAREN, "(", currentLine};
        case ')': advance(); return {T_CLOSE_PAREN, ")", currentLine};
        case '+': advance(); return {T_PLUS, "+", currentLine};
        case '-': advance(); return {T_MINUS, "-", currentLine};
    }

    // comentários ou divisão
    if (c == '/') {
        advance();
        if (peek() == '/') { while (peek()!='\n' && peek()!='\0') advance(); return nextToken(); }
        if (peek() == '*') {
            advance();
            while (peek()!='\0') {
                if (peek()=='*' && pos+1<(int)source.length() && source[pos+1]=='/') { advance(); advance(); break; }
                if (peek()=='\n') currentLine++;
                advance();
            }
            return nextToken();
        }
        return {T_DIVIDE, "/", currentLine};
    }

    if (c == '=') { advance(); if (peek()=='='){advance(); return {T_EQUAL,"==",currentLine};} return {T_ASSIGN,"=",currentLine}; }
    if (c == '*') { advance(); if (peek()=='*'){advance(); return {T_EXPONENT,"**",currentLine};} return {T_MULTIPLY,"*",currentLine}; }
    if (c == '>') { advance(); if (peek()=='='){advance(); return {T_GREATER_EQUAL,">=",currentLine};} return {T_GREATER,">",currentLine}; }
    if (c == '<') { advance(); if (peek()=='='){advance(); return {T_LESS_EQUAL,"<=",currentLine};} return {T_LESS,"<",currentLine}; }
    if (c == '!') { advance(); if (peek()=='='){advance(); return {T_NOT_EQUAL,"!=",currentLine};} return {T_ERROR,"!",currentLine}; }

    // números: dec / real / hex / bin / oct
    if (isdigit(static_cast<unsigned char>(c))) {
        std::string num;
        
        // Verifica se é uma base especial espiando a próxima letra
        if (c == '0' && pos + 1 < (int)source.length()) {
            char p = source[pos + 1]; 
            if (p == 'x' || p == 'X' || p == 'b' || p == 'B' || p == 'o' || p == 'O') {
                num += advance(); // consome o '0'
                char base = advance(); // consome a letra base
                num += base;
                
                if (base == 'x' || base == 'X') {
                    while (isxdigit(static_cast<unsigned char>(peek()))) num += advance();
                } else if (base == 'b' || base == 'B') {
                    while (peek() == '0' || peek() == '1') num += advance();
                } else if (base == 'o' || base == 'O') {
                    while (peek() >= '0' && peek() <= '7') num += advance();
                }
                return {T_NUMBER, num, currentLine};
            }
        }

        // Se não for base especial, processa como número normal
        while (isdigit(static_cast<unsigned char>(peek()))) num += advance();

        // parte fracionária opcional
        if (peek() == '.' && pos + 1 < (int)source.length() && isdigit(static_cast<unsigned char>(source[pos + 1]))) {
            num += advance(); // consome '.'
            while (isdigit(static_cast<unsigned char>(peek()))) num += advance();
        }
        return {T_NUMBER, num, currentLine};
    }

    // identificadores
    if (isalpha(static_cast<unsigned char>(c)) || c=='_') {
        std::string word="";
        while (isalnum(static_cast<unsigned char>(peek())) || peek()=='_') word += advance();
        if (keywords.find(word)!=keywords.end()) return {keywords[word],word,currentLine};
        return {T_IDENTIFIER,word,currentLine};
    }

    // strings "..." ou '...' com escape
    if (c=='"' || c=='\'') {
        char quote = c;
        std::string str;
        advance(); //Consome a aspa inicial
        while (peek()!= quote && peek() != '\0') {
            if (peek() == '\\') {
                advance();
                char e = peek();
                switch (e) { 
                    case 'n':str += '\n' ;break;
                    case 't':str += '\t';break;
                    case 'r':str += '\r';break;
                    case '\\':str += '\\';break;
                    case '"':str += '"';break;
                    case '\'':str += '\'';break;
                    default:str += e; break;
                 }
                advance();
            } else {
                if (peek() == '\n') currentLine++;
                str += advance();
            }
        }
        if (peek() == '\0'){
            return {T_ERROR, "String não fechado", currentLine};
        }
        advance(); 
        return {T_STRING_LIT, str, currentLine};
    }

    // token desconhecido
    return {T_ERROR, std::string(1, c), currentLine};
}
