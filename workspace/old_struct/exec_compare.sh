# Comando para rodar o programa que compara os resultados de H1, H2, sequencial, e Paralelo de vários grafos
#   Todos os grafos devem estar no mesma pasta, e possuir um nome padrão onde o ultimos caracteres antes do .txt deve ser um número inteiro.

# ------------------- OBS: Deve-se rodar o "make build_compare" caso o executavel "compare.out" não exista.

# O primeiro parâmetro> parâmetro é o caminho da pasta que estão os grafos. Não se esqueça de deixar a barra (/) no final

# O segundo parâmetro> é a parte do nome em comum entre os grafos.
#   Neste caso os nomes dos grafos são "gba_10_6_0.txt", "gba_10_6_1.txt", até "gba_10_6_9.txt".
#   Então o nome passado deve ser :  gba_10_6_

# O terceiro parâmetro> é a quantidade total de grafos que tem na pasta. Ao colocar o valor x, o programa avalia os grafos [0, x-1]

# A Saida do programa é um arquivo results.txt dentro da pasta passada no parâmetro 1

./compare.out grafos_erdos/ ger_13_30_  5
#                |               |        |
#                |               |        |
#                |               |        | O terceiro parâmetro é a quantidade de grafos na pasta, neste caso o valor é de 0 até 9
#                |               |
#                |               | O segundo parametro é o nome padrão, menos o número que identifica o grafo
#                |
#                |
#                |
#                | O primeiro parametro parâmetro é o caminho da pasta que estão os grafos. Não se esqueça de deixar a barra (/) no final