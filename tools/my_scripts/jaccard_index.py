import os
import sys

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

def calculate_jaccard_index(base_matrix, current_matrix):
    intersection = sum([1 for i in range(len(base_matrix)) for j in range(len(base_matrix[i])) if base_matrix[i][j] == current_matrix[i][j] == 1])
    union = sum([1 for i in range(len(base_matrix)) for j in range(len(base_matrix[i])) if base_matrix[i][j] == 1 or current_matrix[i][j] == 1])

    jaccard_index = intersection / union if union > 0 else 0
    return round(jaccard_index, 5)

def sort_degrees(vertices):
    sorted_degrees = sorted(vertices, reverse=True)
    return ' '.join(map(str, sorted_degrees))

def main():
    print("Diretório corrente: ", os.getcwd())

    if len(sys.argv) != 2:
        print("Uso: python script.py diretorio/arquivo-X.txt")
        sys.exit(1)

    input_file_path = sys.argv[1]
    instance_name_prefix = input_file_path.split('/')[-1].split('-')[0].split('.')[0]

    # Extrair o número do arquivo
    file_number = int(input_file_path.split('-')[-1].split('.')[0])

    # Ler a matriz de adjacência do arquivo base
    base_num_vertices, base_matrix = read_adjacency_matrix(input_file_path.replace('-' + str(file_number), '-1'))

    # Obter os graus dos vértices do arquivo base
    base_degrees = [sum(row) for row in base_matrix]

    # Construir o nome do arquivo de saída
    output_file_name = input_file_path.replace('-1', f'{file_number}_jaccard.txt')

    # Verificar se o arquivo de saída já existe
    if os.path.exists(output_file_name):
        overwrite = input(f"O arquivo '{output_file_name}' já existe. Deseja sobrescrevê-lo? (S/N): ").strip().lower()
        if overwrite[0].lower() != 's':
            print("Operação cancelada.")
            sys.exit(0)

    instances_data = []

    i = 1
    while True:
        current_matrix_path = input_file_path.replace('-' + str(file_number), f'-{i}')

        if not os.path.exists(current_matrix_path):
            break

        _, current_matrix = read_adjacency_matrix(current_matrix_path)

        # Calcular o índice de Jaccard
        jaccard_index = calculate_jaccard_index(base_matrix, current_matrix)

        # Obter os graus dos vértices do arquivo atual
        current_degrees = [sum(row) for row in current_matrix]

        # Armazenar os dados da instância
        #instances_data.append((f"g{base_num_vertices}_{file_number}-{i}", current_degrees, jaccard_index))
        instances_data.append((f"{instance_name_prefix}-{i}", current_degrees, jaccard_index))

        i += 1

    # Ordenar os dados das instâncias por Jaccard Index em ordem não-crescente
    instances_data.sort(key=lambda x: x[2], reverse=True)

    # Escrever no arquivo de saída
    with open(output_file_name, 'w') as output_file:
        # Escrever a linha de cabeçalho
        output_file.write("INSTANCIA; GRAUS; JACCARD\n")

        # Escrever os dados ordenados no arquivo de saída
        for instance, degrees, jaccard_index in instances_data:
            output_file.write(f"{instance}; {sort_degrees(degrees)} ; {jaccard_index}\n")

    print(f"Processo concluído. Resultados salvos em {output_file_name}.")

if __name__ == "__main__":
    main()
