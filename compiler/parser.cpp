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

    auto init = parseStatement();
    // Init pode ser nullptr em casos como for (;i<10;i++)
    
    auto cond = parseExpression();
    expect(T_SEMICOLON);

    auto inc = parseExpression();
    expect(T_CLOSE_PAREN);

    auto body = parseBlock();

    return std::make_unique<ForStmt> (std::move(init), std::move(cond), std::move(inc), std::move(body));
}