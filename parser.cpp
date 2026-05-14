#include "ast.cpp"
#include "lexer.cpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

static int CurTok;

static int getNextToken() {
  return CurTok = gettok();
}

static std::map<char, int> BinOpPrecedence;

static int getTokPrecedence() {
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

static std::unique_ptr<ExprAST> ParseExpression();
static std::unique_ptr<ExprAST> ParseRepExpr();
static std::unique_ptr<ExprAST> ParseVectorExpr();

static std::unique_ptr<ExprAST> ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAST>(NumVal);
  getNextToken(); 
  return std::move(Result);
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
  std::string IdName = IdentifierStr;

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

static std::unique_ptr<ExprAST> ParseReadExpr();

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
  case tok_vector:
    return ParseVectorExpr();
  case tok_read:
    return ParseReadExpr();
  case '(':
    return ParseParenExpr();
  }
}

static std::unique_ptr<ExprAST> ParseBinOpRHS(int expPrec, std::unique_ptr<ExprAST> LHS) {
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
    LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

static std::unique_ptr<ExprAST> ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS)
    return nullptr;
  return ParseBinOpRHS(0, std::move(LHS));
}

static std::unique_ptr<PrototypeAST> ParsePrototype() {
  if (CurTok != tok_identifier)
    return LogErrorP("Expected function name.");

  std::string FnName = IdentifierStr;
  getNextToken();

  if (CurTok != '(')
    return LogErrorP("Expected '('.");
  
  std::vector<std::string> Args;
  while (getNextToken() == tok_identifier)
    Args.push_back(IdentifierStr);

  if (CurTok != ')')
    return LogErrorP("Expected ')'.");
  
  getNextToken();
  return std::make_unique<PrototypeAST>(FnName, std::move(Args));
}

static std::unique_ptr<ExprAST> ParseReturn() {
  getNextToken();
  std::unique_ptr<ExprAST> expression = nullptr;

  if (CurTok != ';') {
    expression = ParseExpression();
    if (!expression)
      return nullptr;
  }
  return std::make_unique<ReturnExprAST>(std::move(expression));
}

static std::unique_ptr<ExprAST> ParseReadExpr() {
  getNextToken();
  std::vector<std::unique_ptr<VariableExprAST>> vars;
  while (CurTok == tok_identifier) {
    vars.push_back(std::make_unique<VariableExprAST>(IdentifierStr));
    getNextToken();
    if (CurTok == ',')
      getNextToken();
  }
  return std::make_unique<LerExprAST>(std::move(vars));
}

static std::unique_ptr<ExprAST> ParseBlock() {
  getNextToken();
  std::vector<std::unique_ptr<ExprAST>> block;
  while (CurTok != '}' && CurTok != tok_eof) {
    std::unique_ptr<ExprAST> blck;

    if (CurTok == tok_return)
      blck = ParseReturn();
    else if (CurTok == tok_rep)
      blck = ParseRepExpr();
    else if (CurTok == tok_read)
      blck = ParseReadExpr();
    else {
      blck = ParseExpression();
    }

    if (!blck)
      return nullptr;

    block.push_back(std::move(blck));
    
    if (CurTok == ';')
      getNextToken();
  }
  
  if (CurTok != '}')
    return nullptr;
    
  getNextToken();
  return std::make_unique<BlockExprAST>(std::move(block));
}
