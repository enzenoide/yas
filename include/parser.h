#ifndef YAS_PARSER_H
#define YAS_PARSER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ast.h"
#include "lexer.h"

extern int CurTok;
extern std::map<char, int> BinOpPrecedence;

int getNextToken();
int getTokPrecedence();

std::unique_ptr<ExprAST> ParseExpression();
std::unique_ptr<ExprAST> ParseTopLevel();
std::unique_ptr<PrototypeAST> ParsePrototype();
std::unique_ptr<ExprAST> ParseBlock();
std::unique_ptr<ExprAST> ParseReturn();
std::unique_ptr<ExprAST> ParseReadExpr();
std::unique_ptr<ExprAST> ParseRepExpr();
std::unique_ptr<ExprAST> ParseVectorExpr();
std::unique_ptr<FunctionAST> ParseFunctionDefinition();

std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);

#endif // YAS_PARSER_H
