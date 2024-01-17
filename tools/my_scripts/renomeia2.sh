#!/bin/bash

# Diretório onde os arquivos estão localizados
diretorio="/caminho/do/seu/diretorio"

# Verifica se o diretório existe
if [ -d "$diretorio" ]; then
  # Navega até o diretório
  cd "$diretorio" || exit

  # Renomeia os arquivos
  for arquivo in *; do
    novo_nome="g${arquivo}"
    mv "$arquivo" "$novo_nome"
    echo "Renomeado: $arquivo para $novo_nome"
  done

  echo "Renomeação concluída."
else
  echo "O diretório não existe."
fi
