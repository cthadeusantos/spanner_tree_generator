import sys
import os
import math
import statistics

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

def main():
    print("Diretório corrente: ", os.getcwd())

    if len(sys.argv) != 2:
        print("Por favor, forneça o caminho do arquivo como argumento.")
        sys.exit(1)

    input_file_path = sys.argv[1]
    instance_name_prefix = input_file_path.split('/')[-1].split('-')[0].split('.')[0]

    # Extrai o número do arquivo base
    base_number = int(input_file_path.split('-')[-1].split('.')[0])

    # Lê a matriz de adjacência do arquivo base
    base_num_vertices, base_matrix = read_adjacency_matrix(input_file_path)

    #output_file_path = f"{os.path.dirname(input_file_path)}/output.txt"
    output_file_name = f"{os.path.dirname(input_file_path)}/output.txt"

    # Verificar se o arquivo de saída já existe
    if os.path.exists(output_file_name):
        overwrite = input(f"O arquivo '{output_file_name}' já existe. Deseja sobrescrevê-lo? (S/N): ").strip().lower()
        if overwrite[0].lower() != 's':
            print("Operação cancelada.")
            sys.exit(0)

    #with open(output_file_path, 'w') as output_file:
    instances_data = []

    i = 1
    while True:
        # Formata o nome do arquivo para encontrar o próximo
        current_file_path = input_file_path.replace(f"-{base_number}.", f"-{i}.")

        # Se o arquivo não existir, sai do loop
        if not os.path.exists(current_file_path):
            break

        # Lê a matriz de adjacência do arquivo atual
        base_num_vertices, current_matrix = read_adjacency_matrix(current_file_path)

        # Calcula os vetores de graus
        base_degree_vector = calculate_degree_vector(base_matrix)
        current_degree_vector = calculate_degree_vector(current_matrix)

        # Calcula a similaridade do cosseno
        similarity_vector = []
        for j in range(len(base_matrix)):
            similarity = cosine_similarity(base_matrix[j], current_matrix[j])
            similarity_vector.append(similarity)
            #similarity = cosine_similarity(base_degree_vector, current_degree_vector)
        similarity = statistics.mean(similarity_vector)

        # Armazenar os dados da instância
        # instances_data.append((f"g{base_num_vertices}_{file_number}-{i}", current_degrees, jaccard_index))
        instances_data.append((f"{instance_name_prefix}-{i}", {sort_degrees(current_degree_vector)}, round(similarity, 7)))

            # Escreve no arquivo de saída
            #output_file.write(f"{i} ; {sort_degrees(current_degree_vector)} ; {similarity}\n")

        i += 1

    # Ordenar os dados das instâncias por Jaccard Index em ordem não-crescente
    instances_data.sort(key=lambda x: x[2], reverse=True)
    # Escrever no arquivo de saída
    with open(output_file_name, 'w') as output_file:
        # Escrever a linha de cabeçalho
        output_file.write("Instance; degrees ; Cosine similarity\n")

        # Escrever os dados ordenados no arquivo de saída
        for instance, degrees, similarity in instances_data:
            output_file.write(f"{instance}; {sort_degrees(degrees)} ; {similarity}\n")

    print(f"O arquivo de saída foi gravado em: {output_file_name}")

if __name__ == "__main__":
    main()
