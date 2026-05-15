#ifndef YAS_AST_H
#define YAS_AST_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

class ExprAST {
public:
    virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST {
    double val;

public:
    explicit NumberExprAST(double Val) : val(Val) {}
};

class VariableExprAST : public ExprAST {
    std::string name;
    std::string type;

public:
    VariableExprAST(const std::string &name, const std::string &type = "long long")
        : name(name), type(type) {}
};

class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args)
        : Callee(Callee), Args(std::move(Args)) {}
};

class PrototypeAST {
    std::string Name;
    std::string ReturnType;
    std::vector<std::pair<std::string, std::string>> Args;

public:
    PrototypeAST(const std::string &Name,
                 std::vector<std::pair<std::string, std::string>> Args,
                 const std::string &ReturnType = "long long")
        : Name(Name), ReturnType(ReturnType), Args(std::move(Args)) {}

    const std::string &getName() const { return Name; }
};

class FunctionAST : public ExprAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

class BlockExprAST : public ExprAST {
    std::vector<std::unique_ptr<ExprAST>> Block;

public:
    explicit BlockExprAST(std::vector<std::unique_ptr<ExprAST>> Block)
        : Block(std::move(Block)) {}
};

class ReturnExprAST : public ExprAST {
    std::unique_ptr<ExprAST> Expr;

public:
    explicit ReturnExprAST(std::unique_ptr<ExprAST> Expr)
        : Expr(std::move(Expr)) {}
};

class LerExprAST : public ExprAST {
    std::vector<std::unique_ptr<VariableExprAST>> Vars;

public:
    explicit LerExprAST(std::vector<std::unique_ptr<VariableExprAST>> Vars)
        : Vars(std::move(Vars)) {}
};

#endif // YAS_AST_H
