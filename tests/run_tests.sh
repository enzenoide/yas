#!/usr/bin/env bash
set -euo pipefail

# Localiza o executável do projeto (procura em ../yas ou ../build/yas)
if [ -x "../yas" ]; then
  YAS="../yas"
elif [ -x "../build/yas" ]; then
  YAS="../build/yas"
else
  echo "Executável 'yas' não encontrado. Compile o projeto com cmake/make a partir da raiz." >&2
  exit 1
fi

for f in ./*.yas; do
  echo "==> Executando $f"
  "$YAS" "$f" || echo "Falha ao executar $f"
  echo
done

echo "Execução dos testes finalizada."
