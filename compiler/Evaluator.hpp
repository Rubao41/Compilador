#pragma once
#include "ast.hpp"
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <vector>

struct OdysseyValue {
    enum Type { INT, STRING, BOOL} type;
    double numVal = 0;
    std::string strVal = "";
    bool boolVal = false;
};

class Evaluator {
    private:
        std::vector<std::map<std::string, OdysseyValue>> envStack;

        OdysseyValue& getVarRef(const std::string& name);
        void declareVar(const std::string& name, const OdysseyValue& val);
        void setVar(const std::string& name, const OdysseyValue& val);
        
        void pushScope() { envStack.emplace_back(); }
        void popScope() { if (!envStack.empty()) envStack.pop_back(); }

        bool varExistsInCurrentScope(const std::string& name) const {
            if (envStack.empty()) return false;
            return envStack.back().find(name) != envStack.back().end();
        }

        bool varExistsAnywhere(const std::string& name) const {
            for (auto it = envStack.rbegin(); it != envStack.rend(); ++it) {
                if (it->find(name) != it->end()) return true;
            }
            return false;
        }
    
    public:
        void run(Node* node);
        OdysseyValue eval(Node* node);

        Evaluator();
        void executeProgram(const Program* prog);
        void executeWhile(const WhileStmt* stmt);
        void executeFor(const ForStmt* stmt);
        void executeDecl(const Decl* stmt);
        void executeAssign(const AssignStmt* stmt);
        void executePrint(const PrintStmt* stmt);
        void executeInput(const InputStmt* stmt);
        void executeBlock(const BlockStmt* stmt);
};
