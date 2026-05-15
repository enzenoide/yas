#ifndef YAS_LEXER_H
#define YAS_LEXER_H

#include <string>

enum Token {
    tok_eof = -1,
    tok_void = -2,
    tok_identifier = -3,
    tok_number = -4,
    tok_rep = -5,
    tok_vetor = -6,
    tok_ler = -7,
    tok_return = -8,
    tok_def = -9,
};

extern std::string identifierStr;
extern long long num;

int gettok();

#endif // YAS_LEXER_H
