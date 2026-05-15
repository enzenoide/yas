#include "../include/parser.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>

int CurTok;
std::map<char, int> BinOpPrecedence;

int getNextToken() {
    return CurTok = gettok();
}

int getTokPrecedence() {
    if (!isascii(CurTok))
        return -1;

    int tokPrec = BinOpPrecedence[CurTok];
    if (tokPrec <= 0)
        return -1;
    return tokPrec;
}

std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

static std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(num);
    getNextToken();
    return Result;
}

static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken();
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken();
    return V;
}

static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = identifierStr;
    getNextToken();

    if (CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);

    getNextToken();
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')') {
        while (true) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    getNextToken();
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<ExprAST> ParseReadExpr() {
    getNextToken();
    std::vector<std::unique_ptr<VariableExprAST>> vars;

    while (CurTok == tok_identifier) {
        vars.push_back(std::make_unique<VariableExprAST>(identifierStr));
        getNextToken();
        if (CurTok == ',')
            getNextToken();
    }

    return std::make_unique<LerExprAST>(std::move(vars));
}

std::unique_ptr<ExprAST> ParseRepExpr() {
    getNextToken();
    std::vector<std::unique_ptr<ExprAST>> Args;

    if (auto Count = ParseExpression()) {
        Args.push_back(std::move(Count));
        return std::make_unique<CallExprAST>("rep", std::move(Args));
    }

    return nullptr;
}

std::unique_ptr<ExprAST> ParseVectorExpr() {
    getNextToken();
    std::vector<std::unique_ptr<ExprAST>> Args;

    if (CurTok == '(') {
        getNextToken();
        if (CurTok != ')') {
            while (true) {
                if (auto Arg = ParseExpression())
                    Args.push_back(std::move(Arg));
                else
                    return nullptr;

                if (CurTok == ')')
                    break;

                if (CurTok != ',')
                    return LogError("Expected ')' or ',' in vector list");
                getNextToken();
            }
        }
        getNextToken();
    }

    return std::make_unique<CallExprAST>("vetor", std::move(Args));
}

static std::unique_ptr<ExprAST> ParsePrimary() {
    switch (CurTok) {
    default:
        return LogError("unknown token when expecting an expression");
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case tok_rep:
        return ParseRepExpr();
    case tok_vetor:
        return ParseVectorExpr();
    case tok_ler:
        return ParseReadExpr();
    case '(':
        return ParseParenExpr();
    }
}

static std::unique_ptr<ExprAST> ParseBinOpRHS(int expPrec,
                                              std::unique_ptr<ExprAST> LHS) {
    while (true) {
        int tokPrec = getTokPrecedence();
        if (tokPrec < expPrec)
            return LHS;

        int BinOp = CurTok;
        getNextToken();

        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        int NextPrec = getTokPrecedence();
        if (NextPrec > tokPrec) {
            RHS = ParseBinOpRHS(tokPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
                                              std::move(RHS));
    }
}

std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;
    return ParseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<PrototypeAST> ParsePrototype() {
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name.");

    std::string FnName = identifierStr;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '('.");

    getNextToken();
    std::vector<std::pair<std::string, std::string>> Args;
    while (CurTok == tok_identifier) {
        Args.emplace_back(identifierStr, "long long");
        getNextToken();
        if (CurTok == ',')
            getNextToken();
    }

    if (CurTok != ')')
        return LogErrorP("Expected ')'.");

    getNextToken();
    return std::make_unique<PrototypeAST>(FnName, std::move(Args));
}

std::unique_ptr<ExprAST> ParseReturn() {
    getNextToken();
    std::unique_ptr<ExprAST> expression = nullptr;

    if (CurTok != ';') {
        expression = ParseExpression();
        if (!expression)
            return nullptr;
    }
    return std::make_unique<ReturnExprAST>(std::move(expression));
}

std::unique_ptr<ExprAST> ParseBlock() {
    getNextToken();
    std::vector<std::unique_ptr<ExprAST>> block;

    while (CurTok != '}' && CurTok != tok_eof) {
        std::unique_ptr<ExprAST> stmt;

        if (CurTok == tok_return)
            stmt = ParseReturn();
        else if (CurTok == tok_rep)
            stmt = ParseRepExpr();
        else if (CurTok == tok_ler)
            stmt = ParseReadExpr();
        else
            stmt = ParseExpression();

        if (!stmt)
            return nullptr;

        block.push_back(std::move(stmt));
        if (CurTok == ';')
            getNextToken();
    }

    if (CurTok != '}')
        return nullptr;

    getNextToken();
    return std::make_unique<BlockExprAST>(std::move(block));
}

std::unique_ptr<FunctionAST> ParseFunctionDefinition() {
    if (CurTok != tok_void && CurTok != tok_def)
        return nullptr;

    getNextToken();
    if (CurTok != tok_identifier)
        return nullptr;

    std::string FnName = identifierStr;
    getNextToken();

    if (CurTok != '(')
        return nullptr;

    getNextToken();
    std::vector<std::pair<std::string, std::string>> Args;
    while (CurTok == tok_identifier) {
        Args.emplace_back(identifierStr, "long long");
        getNextToken();
        if (CurTok == ',')
            getNextToken();
    }

    if (CurTok != ')')
        return nullptr;

    getNextToken();
    if (CurTok != '{')
        return nullptr;

    auto Body = ParseBlock();
    if (!Body)
        return nullptr;

    auto Proto = std::make_unique<PrototypeAST>(FnName, std::move(Args));
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(Body));
}

std::unique_ptr<ExprAST> ParseTopLevel() {
    if (CurTok == tok_void || CurTok == tok_def)
        return ParseFunctionDefinition();
    if (CurTok == '{')
        return ParseBlock();
    return ParseExpression();
}
