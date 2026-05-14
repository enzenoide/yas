#include <string>
#include <cctype>
#include <cstdlib>
#include <cstdio>

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

static std::string identifierStr;
static long long num;

static int gettok() {
    static int LastChar = ' ';

    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) {
        identifierStr = LastChar;

        while (isalnum((LastChar = getchar())))
            identifierStr += LastChar;

        if (identifierStr == "void")
            return tok_void;
        if (identifierStr == "rep")
            return tok_rep;
        if (identifierStr == "vetor")
            return tok_vetor;
        if (identifierStr == "ler")
            return tok_ler;
        if (identifierStr == "return")
            return tok_return;
        if (identifierStr == "def")
            return tok_def;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        num = strtoll(NumStr.c_str(), nullptr, 10);
        return tok_number;
    }

    if (LastChar == '#') {
        do {
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) {
            return gettok();
        }
    }

    if (LastChar == EOF) {
        return tok_eof;
    }

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
