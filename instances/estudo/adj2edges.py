def converter_matriz_para_arestas(arquivo_entrada, arquivo_saida):
    with open(arquivo_entrada, 'r') as f:
        linhas = f.readlines()

    # Lê o tamanho da matriz
    n = int(linhas[0].strip())

    # Lê os valores da matriz em uma única linha
    valores = list(map(int, linhas[1].strip().split()))

    # Constrói a matriz de adjacência
    matriz = []
    for i in range(n):
        linha = valores[i * n:(i + 1) * n]
        matriz.append(linha)

    # Gera a lista de arestas (considerando grafo não direcionado e sem laços)
    arestas = []
    for i in range(n):
        for j in range(i + 1, n):
            if matriz[i][j] == 1:
                arestas.append((i, j))

    # Escreve o arquivo de saída
    with open(arquivo_saida, 'w') as f:
        f.write(f"{n}\n")
        for x, y in arestas:
            f.write(f"{x}, {y}\n")

# Exemplo de uso
converter_matriz_para_arestas('g6.txt', 'saida.txt')
