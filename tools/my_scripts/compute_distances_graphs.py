import os
import math

def read_adjacency_matrix(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    num_vertices = int(lines[0].strip())
    adjacency_matrix = [list(map(int, line.split())) for line in lines[1:]]

    return num_vertices, adjacency_matrix

def calculate_degree_sequence(adjacency_matrix):
    return [sum(row) for row in adjacency_matrix]

def calculate_distance(sequ1, sequ2):
    return math.sqrt(sum((a - b) ** 2 for a, b in zip(sequ1, sequ2)))

def process_graph_files(base_file_path):
    base_num_vertices, base_matrix = read_adjacency_matrix(base_file_path)

    base_degree_sequence = calculate_degree_sequence(base_matrix)
    base_sorted_sequence = sorted(enumerate(base_degree_sequence), key=lambda x: x[1])

    output_dir = 'output'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    result_file_path = os.path.join(output_dir, os.path.basename(base_file_path) + '_distance.txt')

    with open(result_file_path, 'w') as result_file:
        result_file.write("NOME_DO_ARQUIVO GRAUS_DOS_VERTICES_ORDENADOS DISTÂNCIA_PARA_GRAFO_BASE\n")

        current_index = 2  # Start from graph 2
        while True:
            current_file_path = f"{os.path.splitext(base_file_path)[0]}-{current_index}.txt"

            if not os.path.exists(current_file_path):
                break

            current_num_vertices, current_matrix = read_adjacency_matrix(current_file_path)

            current_degree_sequence = calculate_degree_sequence(current_matrix)
            current_sorted_sequence = sorted(enumerate(current_degree_sequence), key=lambda x: x[1])

            distance = calculate_distance(base_degree_sequence, current_degree_sequence)

            result_file.write(f"{os.path.basename(current_file_path)} "
                              f"{','.join(map(str, current_sorted_sequence))} "
                              f"{distance}\n")

            current_index += 1

if __name__ == "__main__":
    print("Diretório corrente: ", os.getcwd())
    base_file_path = input("Enter the base file path (directory/file): ")

    process_graph_files(base_file_path)
    print("Processing complete.")
