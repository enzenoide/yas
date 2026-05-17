#include "../include/ast.h"
#include <set> 

static std::set<std::string> VariaveisDeclaradas;

std::string NumberExprAST::transpilar() {
    return std::to_string(val); 
}

std::string VariableExprAST::transpilar() {
    return name;
}

std::string AssignmentExprAST::transpilar() {
    std::string codigo = "";

    if (VariaveisDeclaradas.find(variable) == VariaveisDeclaradas.end()) {
        codigo += "long long ";
        VariaveisDeclaradas.insert(variable);
    }
    codigo += variable + " = " + val->transpilar() + ";";
    
    return codigo;
}

std::string ReturnExprAST::transpilar() {
    return "return " + Expr->transpilar() + ";";
}

std::string PrototypeAST::transpilar() {
    std::string tipoRetorno = ReturnType;

    if (Name == "main") {
        tipoRetorno = "int";
    }

    std::string codigo = tipoRetorno + " " + Name + "(";

    for (size_t i = 0; i < Args.size(); ++i) {
        std::string nomeArg = Args[i].first;
        std::string tipoArg = Args[i].second; 

        VariaveisDeclaradas.insert(nomeArg);

        codigo += tipoArg + " " + nomeArg;
        if (i < Args.size() - 1)
            codigo += ", ";
    }
    codigo += ")";
    return codigo;
}
std::string FunctionAST::transpilar() {
    VariaveisDeclaradas.clear();
    
    std::string assinatura = Proto->transpilar();
    std::string corpo = Body->transpilar();
    
    return assinatura + "\n" + corpo;
}

std::string BlockExprAST::transpilar() {
    std::string codigo = "{\n";

    for (size_t i = 0; i < Block.size(); ++i) { 
        codigo += "    " + Block[i]->transpilar();
        codigo += "\n";
    }
    codigo += "}";
    return codigo;
}

std::string LerExprAST::transpilar() {
    std::string declaracoes = ""; 
    std::string leituras = "std::cin";

    for (size_t i = 0; i < Vars.size(); ++i) {
        std::string nomeVar = Vars[i]->transpilar();

        if (VariaveisDeclaradas.find(nomeVar) == VariaveisDeclaradas.end()) {
            declaracoes += "long long " + nomeVar + ";\n";
            VariaveisDeclaradas.insert(nomeVar);
        }

        leituras += " >> " + nomeVar;
    }

    leituras += ";";

    return declaracoes + leituras;
}

std::string RepExprAST::transpilar() {
    std::string declaracao = "for(long long __i = 0; __i < " + Count->transpilar() + "; ++__i)\n";
    std::string body = Body->transpilar();
    
    return declaracao + body;
}

std::string BinaryExprAST::transpilar() {
    std::string L = LHS->transpilar();
    std::string R = RHS->transpilar();
    
   
    return "(" + L + " " + std::string(1, Op) + " " + R + ")";
}
std::string CallExprAST::transpilar() {
    std::string codigo = Callee + "(";

    for (size_t i = 0; i < Args.size(); ++i) {
        
        codigo += Args[i]->transpilar();

        if (i < Args.size() - 1) {
            codigo += ", ";
        }
    }
    
    codigo += ")";
    return codigo;
}
std::string VectorExprAST::transpilar() {
    std::string codigo = "std::vector<long long>{";

    for (size_t i = 0; i < Args.size(); ++i) {
        codigo += Args[i]->transpilar();

        if (i < Args.size() - 1) {
            codigo += ", ";
        }
    }
    
    codigo += "}";
    return codigo;
}
std::string EscExprAST::transpilar() {
    std::string codigo = "std::cout";

    for (size_t i = 0; i < Args.size(); ++i) {
        codigo += " << " + Args[i]->transpilar();
        
        if (i < Args.size() - 1) {
            codigo += " << \" \"";
        }
    }
    
    codigo += " << std::endl;";
    return codigo;
}
