#include <iostream>
#include <sstream>
#include <string>
#include "Lexer.hpp"
#include "Token.hpp"

int main() {
    std::ostringstream buf;
    buf << std::cin.rdbuf();
    std::string source = buf.str();

    Lexer lexer(source);

    while (true) {
        Token t = lexer.nextToken();

        std::cout
            << tokenTypeName(t.type)
            << " value=\"" << t.value << "\""
            << " line=" << t.line
            << "\n";

        if (t.type == T_EOF) break;
    }

    return 0;
}