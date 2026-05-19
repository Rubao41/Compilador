#include <cmath>

// execução do while
void Evaluator::executeWhile(const WhileStmt* stmt){
    //avalia a condição
    while (std::abs(eval(stmt->condition.get())) != 0){
        run(stmt->body.get());
    }
}

//execução do for
void Evaluator::executeFor(const ForStmt* stmt){
    // roda o init 1 vez
    if (stmt->init){
        run(stmt->init.get());
    }

    //testa a condição
    while ( std::abs(eval(stmt->condition.get())) != 0){

        //executa o body
        run(stmt->body.get());

        //executa o final do bloco
        if (stmt->increment){
            run(stmt->increment.get());
        }
    }
}