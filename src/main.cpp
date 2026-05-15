#include <cstdio>
#include "../include/parser.h"

int main() {
    BinOpPrecedence['<'] = 10;
    BinOpPrecedence['+'] = 20;
    BinOpPrecedence['-'] = 20;
    BinOpPrecedence['*'] = 40;
    BinOpPrecedence['/'] = 40;

    getNextToken();
    while (CurTok != tok_eof) {
        if (CurTok == ';') {
            getNextToken();
            continue;
        }

        if (!ParseTopLevel())
            return 1;

        if (CurTok == ';')
            getNextToken();
    }

    std::puts("Parse completed.");
    return 0;
}
