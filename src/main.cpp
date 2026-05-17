#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "../include/parser.h"
#include "../include/ast.h"

static std::ofstream ArquivoSaida;

static void HandleDefinition() {
    if (auto FnAST = ParseFunctionDefinition()) {
        ArquivoSaida << FnAST->transpilar() << "\n\n";
    } else {
        getNextToken();
    }
}

static void MainLoop() {
    while (true) {
        switch (CurTok) {
        case tok_eof:
            return;
        case ';':
            getNextToken();
            break;
        case tok_def:
        case tok_void:
            HandleDefinition();
            break;
        default:
            fprintf(stderr, "Erro: Esperado 'def' ou 'void' no escopo global.\n");
            getNextToken(); 
            break;
        }
    }
}

//Aceitargumentos do terminal!
int main(int argc, char** argv) {
    // Verifica se o usuário passou o nome do arquivo ao rodar o compilador
    if (argc < 2) {
        std::cerr << "Uso correto: " << argv[0] << " <arquivo.yas>\n";
        return 1;
    }

    // Redireciona a entrada do Lexer para o arquivo.
    if (!freopen(argv[1], "r", stdin)) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << argv[1] << "'\n";
        return 1;
    }

    BinOpPrecedence['='] = 2;
    BinOpPrecedence['<'] = 10;
    BinOpPrecedence['+'] = 20;
    BinOpPrecedence['-'] = 20;
    BinOpPrecedence['*'] = 40;
    BinOpPrecedence['/'] = 40;

    ArquivoSaida.open("saida.cpp");
    if (!ArquivoSaida.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo saida.cpp!\n";
        return 1;
    }

    ArquivoSaida << "// Arquivo gerado automaticamente pelo seu Transpilador!\n";
    ArquivoSaida << "#include <iostream>\n";
    ArquivoSaida << "#include <vector>\n\n";

    
    getNextToken();
    MainLoop();

    ArquivoSaida.close();
    

    int resultadoCompilacao = system("g++ saida.cpp -o programa");

    if (resultadoCompilacao == 0) {
        system("./programa"); 
    } else {
        fprintf(stderr, "Erro: O compilador C++ encontrou um problema no código gerado.\n");
    }


    return 0;
}
