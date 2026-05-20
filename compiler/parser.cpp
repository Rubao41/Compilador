//parse while
std::unique_ptr<Node> Parser::parseWhileStmt(){
    expect(T_WHILE);
    expect(T_OPEN_PAREN);

    auto cond = parseExpression();
    expect(T_CLOSE_PAREN);

    auto body = parseBlock();
    return std::make_unique<WhileStmt>(std::move(cond), std::move(body));
}

//parse for
std::unique_ptr<Node> Parser::parseForStmt(){
    expect(T_FOR);
    expect(T_OPEN_PAREN);

    std::unique_ptr<Node> init = nullptr;
    if (current().type == T_INT_TYPE || current().type == T_STRING_TYPE || current().type == T_BOOL_TYPE || current().type == T_IDENTIFIER) {
        if (current().type == T_INT_TYPE || current().type == T_STRING_TYPE || current().type == T_BOOL_TYPE) {
            // parseDecl sem consumir ; no final
            std::string typeName = current().value;
            advance();
            std::string varName = current().value;
            expect(T_IDENTIFIER);
            expect(T_ASSIGN);
            auto expr = parseComparison();
            init = std::make_unique<Decl>(typeName, varName, std::move(expr));
        } else {
            // parseAssign sem consumir ; no final
            std::string varName = current().value;
            expect(T_IDENTIFIER);
            expect(T_ASSIGN);
            auto expr = parseComparison();
            init = std::make_unique<AssignStmt>(varName, std::move(expr));
        }
    }
    expect(T_SEMICOLON);

    auto cond = parseExpression();
    expect(T_SEMICOLON);

    auto inc = parseExpression();
    expect(T_CLOSE_PAREN);

    auto body = parseBlock();

    return std::make_unique<ForStmt> (std::move(init), std::move(cond), std::move(inc), std::move(body));
}

//Comparações (>,<, ==)
std::unique_ptr<Node> Parser::parseComparison() {
    auto left = parseAdditive();

    while (current().type == T_GREATER || current().type == T_LESS || current().type == T_EQUAL || current().type == T_NOT_EQUAL) {
        std::string op = current().value;
        advance();
        auto right = parseAdditive();
        left = std::make_unique<BinOp>(op, std::move(left), std::move(right));
    }
    return left;
}

//(+, -)
std::unique_ptr<Node> Parser::parseAdditive() {
    auto left = parseTerm();

    while (current().type == T_PLUS || current().type == T_MINUS) {
        std::string op = current().value;
        advance();
        auto right = parseTerm();
        left = std::make_unique<BinOp>(op, std::move(left), std::move(right));
    }
    return left;
}

//(*, /)
std::unique_ptr<Node> Parser::parseTerm() {
    auto left = parseExponent();

    while (current().type == T_MULTIPLY || current().type == T_DIVIDE) {
        std::string op = current().value;
        advance();
        auto right = parseExponent();
        left = std::make_unique<BinOp>(op, std::move(left), std::move(right));
    }
    return left;
}

//(**)
std::unique_ptr<Node> Parser::parseExponent() {
    auto left = parseFactor();

    while (current().type == T_EXPONENT){
        std::string op = current().value;
        advance();
        auto right = parseFactor();
        left = std::make_unique<BinOp>(op, std::move(left), std::move(right));
    }
    return left;
}

//fatores
std::unique_ptr<Node> Parser::parseFactor() {
    Token t = current();

    if (t.type == T_NUMBER) {
        advance ();
        return std::make_unique<NumLiteral>(std::stod(t.value));
    }
    else if (t.type == T_STRING_LIT) {
        advance();
        return std::make_unique<StringLiteral>(t.value);
    }
    else if (t.type == T_IDENTIFIER) {
        advance();
        return std::make_unique<VarRef>(t.value);
    }
    else if (t.type == T_OPEN_PAREN) {
        advance();
        auto expr = parseComparison();
        expect(T_CLOSE_PAREN);
        return expr;
    }
    throw std::runtime_error("Erro de sintaxe na linha " + std::string(t.line));
}

std::unique_ptr<Node> Parser::parseStatement() {
    Token t = current();
    if (t.type == T_INT_TYPE || t.type == T_STRING_TYPE || t.type == T_BOOL_TYPE) {
        return parseDecl();
    }
    else if (t.type == T_IDENTIFIER) {
        return parseAssign();
    }
    else if (t.type == T_PRINT) {
        return parsePrint();
    }
    else if (t.type == T_INPUT) {
        return parseInput();
    }
    else if (t.type == T_FOR) {
        return parseForStmt();
    }
    else if (t.type == T_WHILE) {
        return parseWhileStmt();
    }

    throw std::runtime_error("Erro de sintaxe na linha " + std::to_string(t.line));
}

std::unique_ptr<Node> Parser::parseDecl() {
    std::string typeName = current().value;
    advance();

    std::string varName = current().value;
    expect(T_IDENTIFIER);
  
    expect(T_ASSIGN);

    auto expr = parseComparison(); 

    expect(T_SEMICOLON);

    return std::make_unique<Decl>(typeName, varName, std::move(expr));
}

std::unique_ptr<Node> Parser::parseAssign() {
    std::string varName = current().value;
    expect(T_IDENTIFIER);

    expect(T_ASSIGN);

    auto expr = parseComparison();

    expect(T_SEMICOLON);

    return std::make_unique<AssignStmt>(varName, std::move(expr));
}

std::unique_ptr<Node> Parser::parseInput() {
    expect(T_INPUT);

    expect(T_OPEN_PAREN);

    std::string varName = current().value;
    expect(T_IDENTIFIER);

    expect(T_CLOSE_PAREN);

    expect(T_SEMICOLON);

    return std::make_unique<InputStmt>(varName);
}

std::unique_ptr<Node> Parser::parsePrint() {
    expect(T_PRINT);

    expect(T_OPEN_PAREN);

    auto expr = parseComparison();

    expect(T_CLOSE_PAREN);

    expect(T_SEMICOLON);

    return std::make_unique<PrintStmt>(std::move(expr));
}

std::unique_ptr<Node> Parser::parseProgram() {
    std::vector<std::unique_ptr<Node>> statements;
    
    while (current().type != T_EOF) {
        statements.push_back(parseStatement());
    }
    
    return std::make_unique<Program>(std::move(statements));
}

std::unique_ptr<Node> Parser::parseBlock() {
    expect(T_OPEN_CURLY); 
    
    std::vector<std::unique_ptr<Node>> statements;
    
    while (current().type != T_CLOSE_CURLY && current().type != T_EOF) {
        statements.push_back(parseStatement());
    }
    
    expect(T_CLOSE_CURLY); 
    
    return std::make_unique<Program>(std::move(statements));
}