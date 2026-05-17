# Testes da linguagem

Este diretório contém exemplos e casos de teste para a linguagem.

Observações sobre a linguagem (detectadas a partir do código-fonte):

- Funções: definidas com `def nome(args) { ... }` ou `void nome(args) { ... }`.
- O programa deve conter definições de função no nível global; `main` deve ser `void main()` ou `def main()`.
- Tipos primitivos: apenas números inteiros (`long long`) suportados.
- Atribuição: `x = expr;` declara a variável na primeira atribuição (sem `let`).
- Impressão: use `esc(...)` (cada argumento é uma expressão); não há suporte a strings literais.
- Leitura: `ler a, b` produz `std::cin >> a >> b;`.
- Laços: `rep N { ... }` repete N vezes.
- Vetores: `vetor [1, 2, 3]` produz `std::vector<long long>{...}`.

Arquivos de teste:

- `hello.yas` — exemplo simples: atribuição e `esc`.
- `arithmetic.yas` — operações aritméticas e precedência.
- `control_flow.yas` — uso de `rep` para repetição.
- `functions.yas` — definição de função (`def`) e chamada.
- `error.yas` — caso com erro de sintaxe intencional.

Como executar (exemplo):

1. Compile o projeto (usa CMake):

```bash
mkdir -p build && cd build
cmake ..
make
```

2. Execute o interpretador/compilador com um arquivo de teste:

```bash
./yas ../tests/hello.yas
```

Ou, estando na raiz do projeto, use o script `run_tests.sh` para executar todos os testes (ver abaixo).
