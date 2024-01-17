#!/bin/bash

# Diretório onde os arquivos estão localizados (diretório atual)
diretorio="$(pwd)"

# Renomeia os arquivos
for arquivo in "$diretorio"/*; do
  novo_nome="${diretorio}/g$(basename "$arquivo")"
  mv "$arquivo" "$novo_nome"
  echo "Renomeado: $arquivo para $novo_nome"
done

echo "Renomeação concluída."

