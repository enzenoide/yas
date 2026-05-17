# Reconhecimento

## Texto síntese

No cenário atual da programação competitiva, os estudantes iniciantes enfrentam uma barreira de entrada severa devido à complexidade sintática das linguagens predominantemente aceitas e eficientes no setor, como o C++. Para competir em alto nível, o participante precisa lidar simultaneamente com duas frentes complexas: o raciocínio lógico-matemático para a resolução do problema (algoritmos e estruturas de dados) e a sobrecarga sintática (boilerplate code) exigida pela linguagem (como inclusão manual de bibliotecas, gerenciamento de escopo, gerenciamento de memória e sintaxes de entrada/saída verbosas).

A ausência de uma linguagem que combine a simplicidade de escrita com a alta performance de execução gera desmotivação e atrasa o progresso de novos programadores. Linguagens de alto nível e baixa verbosidade, como o Python, embora excelentes para o aprendizado inicial, falham sistematicamente em ambientes competitivos devido ao seu tempo de execução elevado, estourando os limites de tempo (Time Limit Exceeded) impostos pelas plataformas de julgamento online (como Beecrowd, Codeforces e LeetCode). Portanto, há uma clara lacuna de uma ferramenta que otimize o tempo de codificação do usuário sem sacrificar a velocidade de execução da máquina. Para preencher essa lacuna, este projeto apresenta o Yas, uma linguagem minimalista focada no aprendizado prático. O Yas atua como um transpiler que converte uma sintaxe simples e direta automaticamente para C++, aproveitando as otimizações de compilação do g++. Além disso, para reduzir a sobrecarga cognitiva e prevenir falhas comuns de limite de memória (overflow), o Yas abstrai a tipagem complexa, definindo todas as variáveis numéricas como long long por padrão. Dessa forma, o estudante pode focar 100% na lógica do algoritmo, gerando um código final que atende aos rigorosos limites de tempo das plataformas de julgamento.

## Tabela de similares

| Linguagem | Descrição | Diferenciais |
| --- | --- | --- |
| Nim | Linguagem com sintaxe similar ao Python que compila para C/C++, mantendo boa performance, mas que exige configurar tipos e I/O para funcionar bem em competições. | O Yas é muito mais minimalista. Em vez de lidar com vários tipos, o Yas padroniza os dados como `long long` para evitar erros de overflow e foca apenas na lógica, gerando C++ silenciosamente. |
| Go (Golang) | Linguagem rápida criada pelo Google, com sintaxe mais limpa que o C++, mas que ainda exige declaração explícita de tipos e criação manual de rotinas de leitura de dados. | O Yas remove a burocracia de tipagem e I/O. Enquanto no Go o aluno perde tempo tipando variáveis, no Yas a conversão direta para C++ garante a velocidade do `g++` sem que o aluno precise escrever código boilerplate. |
| Kotlin | Linguagem moderna que roda na máquina virtual do Java (JVM), removendo a verbosidade do Java clássico, mas que ainda possui estrutura orientada a objetos. | O Kotlin consome a memória pesada de uma máquina virtual. O Yas atua apenas como um tradutor supersimples para C++, entregando um código final leve, rápido e sem exigir que o aluno entenda sobre classes ou objetos. |

## Lista de demandas

- Criação da sintaxe da estrutura de `print`
- Criação da sintaxe da estrutura de repetição, diminuindo a quantidade de código digitado
- Criação da sintaxe de vetores, diminuindo a quantidade de código digitado
- Criação da padronização de tipos, estabelecendo por padrão `long long`

## Tabela de ferramentas

| Ferramenta | Funcionalidade |
| --- | --- |
| C++ | Linguagem base utilizada para desenvolver a estrutura interna do transpiler (criando a análise léxica, sintática e a geração de código do zero), além de atuar como a linguagem alvo para a qual o código Yas é traduzido. |
| CMake | Sistema de automação utilizado para gerenciar o processo de compilação e build do executável do transpiler, facilitando a configuração do projeto em diferentes ambientes. |

# Definição

## Tipos

| Nome do tipo | Descrição |
| --- | --- |
| Automático (Numérico) | O Yas não exige declaração explícita de tipos. Para facilitar o aprendizado e focar apenas na lógica, qualquer variável criada é automaticamente transpilada para o tipo `long long` do C++. Isso previne naturalmente erros de overflow em cálculos matemáticos grandes, muito comuns em competições. |

## Operadores

| Operador | Descrição |
| --- | --- |
| `;` | Fim de um comando |
| `+` | Soma |
| `-` | Subtração |
| `*` | Multiplicação |
| `/` | Divisão |

## Estruturas da linguagem

| Estrutura da linguagem | Descrição | Exemplo |
| --- | --- | --- |
| Declaração de variável | Armazenamento de valores da linguagem em um espaço de memória | `a = 1 + 1;` |
| Laço | Estrutura de repetição de um bloco de código | `rep 10 { ... }` |
| Leitura de dados | Leitura de dados digitados pelo usuário | `ler(variaveis);` |
| Declaração de vetor | Declaração da estrutura vetor | `vetor(1,2,3,4);` |
| Saída de dados | Executa a saída de dados do programa | `esc(variavel);` |

## Palavras reservadas

| Palavra-chave | Descrição | Exemplo de uso |
| --- | --- | --- |
| `rep` | Estrutura de repetição | `rep 10 { ... }` |
| `ler` | Entrada de dados | `ler(variavel);` |
| `esc` | Saída de dados | `esc(variavel);` |
| `vetor` | Declaração de vetor | `vetor(1,2,3,4);` |

# Estruturação

## Documento de ambiente de trabalho

| Utilidade | Ferramenta |
| --- | --- |
| Ferramenta de versionamento e armazenamento de código | Git / GitHub |
| Editor de código / ambiente de desenvolvimento | Neovim (NvChad) |
| Sistema operacional | Linux (Ubuntu) |

## Diagrama de diretórios

![Diagrama de diretórios](image.png)
