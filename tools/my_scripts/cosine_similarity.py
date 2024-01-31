import sys
import os
import math
import statistics
from datetime import date


def sum_matrix_lines(matrix):
    # Inicializa um vetor para armazenar as somas das linhas
    sum_lines = []

    # Itera sobre cada linha da matriz
    for line in matrix:
        # Calcula a soma dos valores da linha e adiciona ao vetor
        sum_line = sum(line)
        sum_lines.append(sum_line)

    # Retorna o vetor com as somas das linhas
    return sum_lines

def sort_degrees(vertices):
    sorted_degrees = sorted(vertices, reverse=True)
    return ' '.join(map(str, sorted_degrees))

# def read_adjacency_matrix(file_path):
#     with open(file_path, 'r') as file:
#         lines = file.readlines()
#
#     # Remove linhas em branco e converte os valores para inteiros
#     matrix = [list(map(int, line.split())) for line in lines if line.strip()]
#
#     return matrix

def read_adjacency_matrix(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Ignorar linhas em branco
    lines = [line.strip() for line in lines if line.strip()]

    # Obter o número de vértices
    num_vertices = int(lines[0])

    # Obter a matriz de adjacência
    adjacency_matrix = [list(map(int, line.split())) for line in lines[1:]]

    return num_vertices, adjacency_matrix

def ler_grafo(arquivo):
    with open(arquivo, 'r') as file:
        # Ignora linhas em branco
        linhas = [linha.strip() for linha in file if linha.strip()]
        # Lê o número de vértices
        num_vertices = int(linhas[0])
        # Lê a matriz de adjacência
        matriz_adj = [list(map(int, linha.split())) for linha in linhas[1:]]
    return num_vertices, matriz_adj

def calculate_degree_vector(matrix):
    # Calcula a soma de cada coluna para obter o vetor de graus
    #degree_vector = [sum(column) for column in zip(*matrix)]
    degree_vector = sum_matrix_lines(matrix)
    return degree_vector

def cosine_similarity(vector1, vector2):
    # Calcula a similaridade do cosseno entre dois vetores
    dot_product = sum(x * y for x, y in zip(vector1, vector2))
    magnitude1 = math.sqrt(sum(x**2 for x in vector1))
    magnitude2 = math.sqrt(sum(x**2 for x in vector2))
    similarity = dot_product / (magnitude1 * magnitude2)
    return similarity



def calcular_graus(matriz_adj):
    return [sum(linha) for linha in matriz_adj]

def verificar_arquivo_saida(caminho_arquivo_saida):
    if os.path.exists(caminho_arquivo_saida):
        opcao = input("O arquivo de saída já existe. Deseja sobrescrever (S) ou fazer append (A)? ").upper()
        if opcao[0].upper() == "S":
            return 'w'
        elif opcao[0].upper() == "A":
            return 'a'
        else:
            print("Opção inválida. Sobrecrevendo por padrão.")
            return 'w'
    else:
        return 'w'

def read_directory(diretorio):
    # Lista para armazenar os arquivos
    listagem_arquivos = []

    # Loop para processar cada arquivo
    for arquivo in os.listdir(diretorio):
        # Verifica se o arquivo é um arquivo TXT
        if arquivo.endswith(".txt"):
            # Caminho completo do arquivo
            listagem_arquivos.append(arquivo)

    return listagem_arquivos

def main():
    print("Diretório corrente: ", os.getcwd())

    # if len(sys.argv) != 2:
    #     print("Por favor, forneça o caminho do arquivo como argumento.")
    #     sys.exit(1)

    # Diretório de entrada
    diretorio = input("Digite o caminho do diretório: ")

    # Nome do arquivo base
    nome_arquivo_base = input("Digite o nome do arquivo base: ")

    # Caminho completo do arquivo base
    caminho_arquivo_base = os.path.join(diretorio, f"{nome_arquivo_base}")

    # # Verifica se o arquivo base existe
    # if not os.path.exists(caminho_arquivo_base):
    #     print(f"Arquivo base {caminho_arquivo_base} não encontrado.")
    #     return
    #
    # input_file_path = sys.argv[1]
    # instance_name_prefix = input_file_path.split('/')[-1].split('-')[0].split('.')[0]
    #
    # # Extrai o número do arquivo base
    # base_number = int(input_file_path.split('-')[-1].split('.')[0])

    # Lê o grafo base
    num_vertices_base, matriz_adj_base = ler_grafo(caminho_arquivo_base)
    graus_base = calcular_graus(matriz_adj_base)

    # Cria o diretório de saída se não existir
    diretorio_saida = os.path.join(diretorio, "results")
    if not os.path.exists(diretorio_saida):
        os.makedirs(diretorio_saida)

    # # Lê a matriz de adjacência do arquivo base
    # base_num_vertices, base_matrix = read_adjacency_matrix(input_file_path)

    # Nome do arquivo de saída
    nome_arquivo_saida = "cosine_similarity.csv"
    #filename = 'cosine_similarity_'

    # Caminho completo do arquivo de saída
    caminho_arquivo_saida = os.path.join(diretorio_saida, f"{nome_arquivo_saida}")

    #output_file_path = f"{os.path.dirname(input_file_path)}/output.txt"
    #output_file_name = f"{os.path.dirname(input_file_path)}/{filename}output.txt"

    # Verificar se o arquivo de saída já existe
    # if os.path.exists(output_file_name):
    #     overwrite = input(f"O arquivo '{output_file_name}' já existe. Deseja sobrescrevê-lo? (S/N): ").strip().lower()
    #     if overwrite[0].lower() != 's':
    #         print("Operação cancelada.")
    #         sys.exit(0)

    # Verifica se o arquivo de saída já existe e decide sobre escrever ou fazer append
    modo_abertura = verificar_arquivo_saida(caminho_arquivo_saida)

    listagem_diretorio = read_directory(diretorio)

    # Abre o arquivo de saída para escrita ou append
    with open(caminho_arquivo_saida, modo_abertura) as file_saida:
        if modo_abertura == 'w':
            file_saida.write("instance ; degree ; cosine similarity\n")
            file_saida.write(f"{nome_arquivo_base} ; {' '.join(map(str, sorted(graus_base, reverse=True)))}; 0.00\n")
            #file_saida.write(f"{nome_arquivo_base}-1.txt {' '.join(map(str, graus_base))} 0.00\n")

        # Processa os demais arquivos
        x = 2
        #while True:
        for arquivo in listagem_diretorio:
            # Nome do arquivo atual
            #nome_arquivo_atual = f"{nome_arquivo_base}-{x}.txt"
            if arquivo.split('.')[-1] != 'txt':
                print(f"Pulando : {arquivo}, não é um arquivo válido!")
                continue
            nome_arquivo_atual = arquivo
            caminho_arquivo_atual = os.path.join(diretorio, nome_arquivo_atual)

            # Verifica se o arquivo atual existe
            if not os.path.exists(caminho_arquivo_atual):
                break

            # Lê o grafo atual
            num_vertices_atual, matriz_adj_atual = ler_grafo(caminho_arquivo_atual)

            # Calcula os vetores de graus
            base_degree_vector = calculate_degree_vector(matriz_adj_base)
            current_degree_vector = calculate_degree_vector(matriz_adj_atual)

            # Calcula a similaridade do cosseno
            similarity_vector = []
            for j in range(len(matriz_adj_base)):
                similarity = cosine_similarity(matriz_adj_base[j], matriz_adj_atual[j])
                similarity_vector.append(similarity)
                # similarity = cosine_similarity(base_degree_vector, current_degree_vector)
            similarity = statistics.mean(similarity_vector)

            similarity_formatada = "{:.7f}".format(round(similarity, 7))
            file_saida.write(f"{nome_arquivo_atual}{';'} {' '.join(map(str, sorted(current_degree_vector, reverse=True)))}{';'} {similarity_formatada}\n")

            #file_saida.write(f"{nome_arquivo_atual}{','} {' '.join(map(str, graus_atual))}{','} {distancia_formatada}\n")

            # Incrementa o contador
            x += 1

    print(f"Arquivo de saída gerado em: {caminho_arquivo_saida}")





    ###############################################
    #with open(output_file_path, 'w') as output_file:
    # instances_data = []
    #
    # i = 1
    # while True:
    #     # Formata o nome do arquivo para encontrar o próximo
    #     current_file_path = input_file_path.replace(f"-{base_number}.", f"-{i}.")
    #
    #     # Se o arquivo não existir, sai do loop
    #     if not os.path.exists(current_file_path):
    #         break
    #
    #     # Lê a matriz de adjacência do arquivo atual
    #     base_num_vertices, current_matrix = read_adjacency_matrix(current_file_path)
    #
    #     # Calcula os vetores de graus
    #     base_degree_vector = calculate_degree_vector(base_matrix)
    #     current_degree_vector = calculate_degree_vector(current_matrix)
    #
    #     # Calcula a similaridade do cosseno
    #     similarity_vector = []
    #     for j in range(len(base_matrix)):
    #         similarity = cosine_similarity(base_matrix[j], current_matrix[j])
    #         similarity_vector.append(similarity)
    #         #similarity = cosine_similarity(base_degree_vector, current_degree_vector)
    #     similarity = statistics.mean(similarity_vector)
    #
    #     # Armazenar os dados da instância
    #     # instances_data.append((f"g{base_num_vertices}_{file_number}-{i}", current_degrees, jaccard_index))
    #     instances_data.append((f"{instance_name_prefix}-{i}", {sort_degrees(current_degree_vector)}, round(similarity, 7)))
    #
    #         # Escreve no arquivo de saída
    #         #output_file.write(f"{i} ; {sort_degrees(current_degree_vector)} ; {similarity}\n")
    #
    #     i += 1
    #
    # # Ordenar os dados das instâncias por Jaccard Index em ordem não-crescente
    # instances_data.sort(key=lambda x: x[2], reverse=True)
    # # Escrever no arquivo de saída
    # with open(output_file_name, 'w') as output_file:
    #     # Escrever a linha de cabeçalho
    #     output_file.write("Instance; degrees ; Cosine similarity\n")
    #
    #     # Escrever os dados ordenados no arquivo de saída
    #     for instance, degrees, similarity in instances_data:
    #         output_file.write(f"{instance}; {sort_degrees(degrees)} ; {similarity}\n")
    #
    # print(f"O arquivo de saída foi gravado em: {output_file_name}")

if __name__ == "__main__":
    main()
