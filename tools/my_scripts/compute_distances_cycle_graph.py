import os
import math

def ler_grafo(arquivo):
    with open(arquivo, 'r') as file:
        # Ignora linhas em branco
        linhas = [linha.strip() for linha in file if linha.strip()]
        # Lê o número de vértices
        num_vertices = int(linhas[0])
        # Lê a matriz de adjacência
        matriz_adj = [list(map(int, linha.split())) for linha in linhas[1:]]
    return num_vertices, matriz_adj

def calcular_graus(matriz_adj):
    return [sum(linha) for linha in matriz_adj]

def calcular_distancia(grafo_base, grafo_atual):
    graus_base_ordenados = sorted(grafo_base, reverse=True)
    graus_atual_ordenados = sorted(grafo_atual, reverse=True)
    distancia = math.sqrt(sum((a - b) ** 2 for a, b in zip(graus_base_ordenados, graus_atual_ordenados)))
    return distancia

def verificar_arquivo_saida(caminho_arquivo_saida):
    if os.path.exists(caminho_arquivo_saida):
        opcao = input("O arquivo de saída já existe. Deseja sobrescrever (S) ou fazer append (A)? ").upper()
        if opcao == "S":
            return 'w'
        elif opcao == "A":
            return 'a'
        else:
            print("Opção inválida. Sobrecrevendo por padrão.")
            return 'w'
    else:
        return 'w'

def main():
    # Diretório de entrada
    diretorio = input("Digite o caminho do diretório: ")

    # Nome do arquivo base
    nome_arquivo_base = input("Digite o nome do arquivo base (sem extensão e TAG): ")

    # Caminho completo do arquivo base
    caminho_arquivo_base = os.path.join(diretorio, f"{nome_arquivo_base}-1.txt")

    # Verifica se o arquivo base existe
    if not os.path.exists(caminho_arquivo_base):
        print(f"Arquivo base {caminho_arquivo_base} não encontrado.")
        return

    # Lê o grafo base
    num_vertices_base, matriz_adj_base = ler_grafo(caminho_arquivo_base)
    graus_base = [2] * num_vertices_base

    # Cria o diretório de saída se não existir
    diretorio_saida = os.path.join(diretorio, "output")
    if not os.path.exists(diretorio_saida):
        os.makedirs(diretorio_saida)

    # Nome do arquivo de saída
    nome_arquivo_saida = input("Digite o nome do arquivo de saída (sem extensão): ")

    # Caminho completo do arquivo de saída
    caminho_arquivo_saida = os.path.join(diretorio_saida, f"{nome_arquivo_base}_distancia.txt")

    # Verifica se o arquivo de saída já existe e decide sobre escrever ou fazer append
    modo_abertura = verificar_arquivo_saida(caminho_arquivo_saida)

    # Abre o arquivo de saída para escrita ou append
    with open(caminho_arquivo_saida, modo_abertura) as file_saida:
        if modo_abertura == 'w':
            file_saida.write("ARQUIVO;GRAUS;DIST\n")

        # Processa os demais arquivos
        x = 2
        while True:
            # Nome do arquivo atual
            nome_arquivo_atual = f"{nome_arquivo_base}-{x}.txt"
            caminho_arquivo_atual = os.path.join(diretorio, nome_arquivo_atual)

            # Verifica se o arquivo atual existe
            if not os.path.exists(caminho_arquivo_atual):
                break

            # Lê o grafo atual
            num_vertices_atual, matriz_adj_atual = ler_grafo(caminho_arquivo_atual)
            graus_atual = calcular_graus(matriz_adj_atual)

            # Calcula a distância e escreve no arquivo de saída
            distancia = calcular_distancia(graus_base, graus_atual)
            distancia_formatada = "{:.2f}".format(distancia)
            file_saida.write(f"{nome_arquivo_atual};{' '.join(map(str, sorted(graus_atual, reverse=True)))};{distancia_formatada}\n")

            # Incrementa o contador
            x += 1

    print(f"Arquivo de saída gerado em: {caminho_arquivo_saida}")

if __name__ == "__main__":
    main()
