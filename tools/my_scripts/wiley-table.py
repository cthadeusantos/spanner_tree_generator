import math
import os
import re
import sys
import csv
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import to_rgba_array, to_hex
from mpl_toolkits.mplot3d import Axes3D

GLOBAL_MAX_COLUMNS = 16     # Max number of columns at output table
GLOBAL_DIR_DATA = 'data'
class InputParameters:
    def __init__(self):
        self.directories = []


def extrair_rotulo(instancia):
    return instancia.split('-')[-1]

def get_last_word(input_string):
    match = re.search(r'\b\w+\b$', input_string)
    if match:
        return match.group(0)
    else:
        return None

def extract_last_words(list1, list2):
    try:
        result = []
        for index in list2:
            if 0 <= index < len(list1):
                last_word = re.search(r'\b\w+\b$', list1[index])
                if last_word:
                    result.append(last_word.group(0))
                else:
                    raise ValueError(f"Unable to extract last word from string at index {index}")
            else:
                raise IndexError(f"Index {index} is out of range for list1")
        return result
    except Exception as e:
        raise e


def main_plot_graph_trees_times(header, matrix, yscale='linear', start=0, end=None, filename='trees'):
    if end is None:
        end = len(matrix)
    cabecalho = extract_last_words(header, [1, 2, 3, 4])
    instancias = [linha[0] for linha in matrix[start:end]]
    distribution = [linha[14] for linha in matrix[start:end]]
    # rotulos_x = [extrair_rotulo(instancia) for instancia in instancias]
    rotulos_x = instancias

    tempos = [list(map(float, linha[8:12])) for linha in matrix[start:end]]
    # labels_y = cabecalho[2:6]
    labels_y = cabecalho
    cores = ['b', 'g', 'r', 'c']
    plotar_grafico2(rotulos_x, tempos, labels_y, cores, type_yscale=yscale, title='Trees per second', yaxislabel='Trees per second', distribution=distribution)

    # Criar o diretório de saída se não existir
    output_dir = "output"
    os.makedirs(output_dir, exist_ok=True)

    nome_arquivo_png = ''.join((output_dir, '/', os.path.splitext(filename)[0] + '_grafico.png'))
    salvar_grafico(rotulos_x, tempos, labels_y, cores, nome_arquivo_png)

def main_plot_graph_times(header, matrix, yscale='linear', start=0, end=None, filename='time'):
    if end is None:
        end = len(matrix)
    cabecalho = extract_last_words(header, [1, 2, 3, 4])
    instancias = [linha[0] for linha in matrix[start:end]]
    #rotulos_x = [extrair_rotulo(instancia) for instancia in instancias]
    rotulos_x = instancias

    tempos = [list(map(float, linha[1:5])) for linha in matrix[start:end]]
    #labels_y = cabecalho[2:6]
    labels_y = cabecalho
    cores = ['b', 'g', 'r', 'c']

    plotar_grafico(rotulos_x, tempos, labels_y, cores, type_yscale=yscale, title='Average times', yaxislabel='Time(s)')

    # Criar o diretório de saída se não existir
    output_dir = "output"
    os.makedirs(output_dir, exist_ok=True)

    nome_arquivo_png = ''.join((output_dir, '/', os.path.splitext(filename)[0] + '_grafico.png'))
    salvar_grafico(rotulos_x, tempos, labels_y, cores, nome_arquivo_png)

def main_plot_graph_cosine(header, matrix, yscale='linear', start=0, end=None, filename='time'):
    if end is None:
        end = len(matrix)
    cabecalho = extract_last_words(header, [1, 2, 3, 4])
    instancias = [linha[0] for linha in matrix[start:end]]
    #rotulos_x = [extrair_rotulo(instancia) for instancia in instancias]

    cosine_similarity = [linha[15] for linha in matrix[start:end]]

    tempos = [list(map(float, linha[1:5])) for linha in matrix[start:end]]
    #labels_y = cabecalho[2:6]
    labels_y = cabecalho
    cores = ['b', 'g', 'r', 'c']

    rotulos_x = cosine_similarity
    rotulos_y = instancias
    plotar_cosine_similarity(rotulos_x, matrix, labels_y, cores, type_yscale=yscale, title='???????', yaxislabel='Time(s)', instances=instancias)

    # Criar o diretório de saída se não existir
    output_dir = "output"
    os.makedirs(output_dir, exist_ok=True)

    nome_arquivo_png = ''.join((output_dir, '/', os.path.splitext(filename)[0] + '_grafico.png'))
    salvar_grafico(rotulos_x, tempos, labels_y, cores, nome_arquivo_png)


def plotar_grafico(x, y, labels, cores, type_yscale='linear', title='None', yaxislabel='None'):
    plt.figure(figsize=(10, 6))

    # convert x-axis to Logarithmic scale
    plt.yscale(type_yscale)

    for i in range(len(y[0])):
        plt.plot(x, [tempo[i] for tempo in y], label=labels[i], color=cores[i], marker='o')
        # changing the fontsize of yticks
        plt.xticks(fontsize=6)

    plt.xlabel('Instance')
    plt.ylabel(yaxislabel)
    plt.title(title)
    plt.xticks(rotation=90)
    plt.xticks(fontsize=6)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()


def criar_dicionario(lista, matrix):
    columns = len(matrix[0])
    if len(lista) != len(matrix[0]):
        raise ValueError("A lista e a matriz devem ter o mesmo número de colunas.")

    dicionario = {}
    for line in matrix:
        for index, value in enumerate(line):
            if lista[index] not in dicionario:
                dicionario[lista[index]] = (value,)
            else:
                dicionario[lista[index]] = dicionario[lista[index]] + (value, )
    return dicionario

def plotar_cosine_similarity(x, y, labels, cores, type_yscale='linear', title='None', yaxislabel='None', distribution=None, instances=None):
    #species = x
    #penguin_means = criar_dicionario(labels , y)

    # Marker size in units of points^2
    #volume = (15 * price_data["volume"][:-2] / price_data["volume"][0]) ** 2
    #close = 0.003 * price_data["close"][:-2] / 0.003 * price_data["open"][:-2]
    fig = plt.figure()  # Here is your error
    ax = fig.add_subplot(111, projection='3d')

    # Invertendo a ordem das linhas
    #y = y[::-1]
    x = x[::-1]
    #cores = ['b', 'g', 'r', 'c']

    # Escolha um mapa de cores do Matplotlib
    cmap = plt.get_cmap("viridis")

    # Escolha o número de cores desejado
    num_colors = len(x)

    header = num_colors
    # Crie uma lista de valores de 0 a 1 para representar a posição no mapa de cores
    positions = [i / (num_colors - 1) for i in range(num_colors)]

    # Use o mapa de cores para obter uma lista de cores RGBA
    colors_rgba = to_rgba_array(cmap(positions))

    # Converta as cores RGBA para uma lista de cores hexadecimais
    cores = [to_hex(color) for color in colors_rgba]

    #x_axis_rotulo = [value + " " + distribution[index] for index, value in enumerate(x)]
    for index, valuey in enumerate(instances):
            #ax.scatter(value, x[indexx], s=60 + (indexy % 2 == 0 * (10+indexy) - indexy % 2 != 0 * (10 + indexy) ), alpha=0.70, label=value,)
        for index2 in range(len(labels)):
            pos = index2 + 1
            ax.scatter(instances, x, y[index][pos], s=60, )
    #ax.set_xlim(50000, 450000)
    #ax.set_box_aspect(0.5625)
    #plt.yticks(fontsize=6)
    #plt.xticks(fontsize=6)
    #plt.yticks(rotation=45)
    #plt.xticks(rotation=45)
    #plt.legend(labels=labels, loc='center right', bbox_to_anchor=(0.5, 0., 0.5, 0.5))

    # ratio = 0.56
    # xleft, xright = plt.get_xlim()
    # ybottom, ytop = plt.get_ylim()
    # plt.set_aspect(abs((xright - xleft) / (ybottom - ytop)) * ratio)
    #plt.set_xlabel(r'$Trees$', fontsize=6)
    #ax.set_ylabel(r'$\Delta_{i+1}$', fontsize=5)
    ax.set_title('Trees per second')
    #ax.legend()
    ax.grid(True)
    #fig.tight_layout()

    plt.show()


def plotar_grafico2(x, y, labels, cores, type_yscale='linear', title='None', yaxislabel='None', distribution=None):
    #species = x
    #penguin_means = criar_dicionario(labels , y)

    # Marker size in units of points^2
    #volume = (15 * price_data["volume"][:-2] / price_data["volume"][0]) ** 2
    #close = 0.003 * price_data["close"][:-2] / 0.003 * price_data["open"][:-2]

    fig, ax = plt.subplots()

    # Invertendo a ordem das linhas
    y = y[::-1]
    x = x[::-1]
    #cores = ['b', 'g', 'r', 'c']

    # Escolha um mapa de cores do Matplotlib
    cmap = plt.get_cmap("viridis")

    # Escolha o número de cores desejado
    num_colors = len(y[0])

    # Crie uma lista de valores de 0 a 1 para representar a posição no mapa de cores
    positions = [i / (num_colors - 1) for i in range(num_colors)]

    # Use o mapa de cores para obter uma lista de cores RGBA
    colors_rgba = to_rgba_array(cmap(positions))

    # Converta as cores RGBA para uma lista de cores hexadecimais
    cores = [to_hex(color) for color in colors_rgba]

    #x_axis_rotulo = [value + " " + distribution[index] for index, value in enumerate(x)]
    for indexx, line in enumerate(y):
        for indexy, value in enumerate(line):
            #ax.scatter(value, x[indexx], s=60 + (indexy % 2 == 0 * (10+indexy) - indexy % 2 != 0 * (10 + indexy) ), alpha=0.70, label=value,)
            ax.scatter(value, x[indexx], s=60, label=value,  color=cores[indexy], )
    ax.set_xlim(50000, 450000)
    ax.set_box_aspect(0.5625)
    plt.yticks(fontsize=6)
    plt.xticks(fontsize=6)
    #plt.yticks(rotation=45)
    plt.xticks(rotation=45)
    plt.legend(labels=labels, loc='center right', bbox_to_anchor=(0.5, 0., 0.5, 0.5))

    # ratio = 0.56
    # xleft, xright = plt.get_xlim()
    # ybottom, ytop = plt.get_ylim()
    # plt.set_aspect(abs((xright - xleft) / (ybottom - ytop)) * ratio)
    #plt.set_xlabel(r'$Trees$', fontsize=6)
    #ax.set_ylabel(r'$\Delta_{i+1}$', fontsize=5)
    ax.set_title('Trees per second')
    #ax.legend()
    ax.grid(True)
    #fig.tight_layout()

    plt.show()
                    # x1 = np.arange(len(species)) # the label locations
                    # width = 0.25 # the width of the bars
                    # multiplier = 0
                    #
                    # fig, ax = plt.subplots(layout='constrained')
                    #
                    # for attribute, measurement in penguin_means.items():
                    #     offset = width * multiplier
                    #     rects = ax.bar(x1 + offset, measurement, width, label=attribute)
                    #     ax.bar_label(rects, padding=3)
                    #     multiplier += 1
                    #
                    # # Add some text for labels, title and custom x-axis tick labels, etc.
                    # ax.set_ylabel('Length (mm)')
                    # ax.set_title('Penguin attributes by species')
                    # ax.set_xticks(x1 + width, species)
                    # ax.legend(loc='upper left', ncols=4)
                    # plt.show()

    # plt.figure(figsize=(10, 6))
    #
    # # convert x-axis to Logarithmic scale
    # plt.yscale(type_yscale)
    #
    # for i in range(len(y[0])):
    #     plt.bar(x, [tempo[i] for tempo in y], label=labels[i], color=cores[i])
    #     # changing the fontsize of yticks
    #     plt.xticks(fontsize=6)
    #
    # plt.xlabel('Instance')
    # plt.ylabel(yaxislabel)
    # plt.title(title)
    # plt.xticks(rotation=75)
    # plt.legend()
    # plt.grid(True)
    # plt.tight_layout()
    # plt.show()


def salvar_grafico(x, y, labels, cores, nome_arquivo):
    plt.figure(figsize=(10, 6))

    for i in range(len(y[0])):
        plt.plot(x, [tempo[i] for tempo in y], label=labels[i], color=cores[i], marker='o')

    plt.xlabel('Instance')
    plt.ylabel('Time (s)')
    plt.title('Average Times per Instance')
    plt.xticks(rotation=90)
    plt.yticks(fontsize=6)
    plt.xticks(fontsize=6)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    plt.savefig(nome_arquivo)
    print(f"Chart saved as {nome_arquivo}")

def merge_matrices_euclidiana(matrix_a, matrix_b):
    for row_a in matrix_a:
        for row_b in matrix_b:
            # Comparar as colunas 0 de ambas as matrizes
            if row_a[0] == row_b[0]:
                # Inserir os valores das colunas 1 e 2 da matriz B nas colunas 13 e 14 da matriz A
                row_a[14] = row_b[1]
                row_a[15] = row_b[2]

    return matrix_a

def read_input_parameters():
    params = InputParameters()

    if len(sys.argv) > 1:
        params.directories = sys.argv[1:]

    return params


def list_subdirectories(data_dir, directories):
    if not directories:
        directories = [d for d in os.listdir(data_dir) if os.path.isdir(os.path.join(data_dir, d))]

    return directories


def read_summary_file(subdirectory):
    summary_file_path = os.path.join("data", subdirectory, "result_summary.txt")

    if not os.path.exists(summary_file_path):
        raise FileNotFoundError(f"File not found: {summary_file_path}")

    with open(summary_file_path, 'r') as file:
        return file.read()


def check_keywords_and_line_presence(text):
    keywords = [
        "Instance Name",
        "Subdir",
        "#Successes/#TotalRuns",
        "Avg running time (s) (MIN/MAX) [std_dev]",
        "Avg stretch index (MIN/MAX) [std_dev]",
        "#Vertex",
        "#Edges",
        "Lower Bound",
        "Total trees"
    ]

    header_line = "+---------------------------------------+----------------------------------+-----------------------+------------------------------------------+---------------------------------------+---------+--------+-------------+-------------+"

    return all(keyword in text for keyword in keywords) or header_line in text or text.strip() == ""



def extract_execution_type(summary_text):
    match = re.search(r'Original binary file name: (.+)', summary_text)
    if match:
        return match.group(1).strip()
    else:
        raise ValueError("Execution type not found in summary file.")


# def extract_data_from_summary(summary_text):
#     # Extract relevant data from the summary text (you may need to adjust the regex patterns)
#     # ...
#     pass
def extract_instance_name(file_path):
    base_name = os.path.basename(file_path)
    instance_name, _ = os.path.splitext(base_name)
    return instance_name


# def read_csv_file(file_path, delimiter=';'):
#     data_matrix = []
#     file_path = GLOBAL_DIR_DATA + '/' + file_path
#
#     with open(file_path, 'r', newline='') as csvfile:
#         csv_reader = csv.reader(csvfile, delimiter=delimiter)
#
#         # Ignorar cabeçalho
#         next(csv_reader, None)
#
#         for row in csv_reader:
#             instance_name = row[0].strip()
#             vertex_degrees = row[1].strip()
#             euclidean_distance = float(row[2])
#
#             data_matrix.append([instance_name, vertex_degrees, euclidean_distance])
#
#     return data_matrix
def remove_extension(file_name):
    """
    Remove a extensão de um nome de arquivo.

    Args:
    - file_name (str): O nome do arquivo.

    Returns:
    - str: O nome do arquivo sem a extensão.
    """
    # Utilizando uma expressão regular para encontrar a extensão
    match = re.match(r'^(.+)\.[^.]+$', file_name)
    if match:
        return match.group(1)
    else:
        # Se não encontrar uma extensão, retorna o nome original
        return file_name

def remove_extension(file_name):
    """
    Remove a extensão de um nome de arquivo.

    Args:
    - file_name (str): O nome do arquivo.

    Returns:
    - str: O nome do arquivo sem a extensão.
    """
    base_name, _ = os.path.splitext(file_name)
    return base_name

def read_csv_file(file_path, delimiter=';'):
    data_matrix = []
    seen_instances = set()
    file_path = GLOBAL_DIR_DATA + '/' + file_path

    with open(file_path, 'r', newline='') as csvfile:
        csv_reader = csv.reader(csvfile, delimiter=delimiter)

        # Ignorar cabeçalho
        next(csv_reader, None)

        for row in csv_reader:
            instance_name = remove_extension(row[0].strip())

            # Verificar se a instância já foi processada
            if instance_name not in seen_instances:
                seen_instances.add(instance_name)

                vertex_degrees = row[1].strip()
                euclidean_distance = float(row[2])

                data_matrix.append([instance_name, vertex_degrees, euclidean_distance])

    return data_matrix


def extract_fields_from_file_content(file_content):
    # Divide o conteúdo do arquivo em linhas
    lines = file_content.split('\n')

    # Lista para armazenar todos os campos
    all_fields = []

    # Loop através de cada linha
    for line in lines:
        # Remove o prefixo '!+' se presente
        line = line.lstrip('!+')
        # Divide a linha pelos caracteres '|'
        fields = line.split('|')
        # Remove espaços em branco ao redor de cada campo
        fields = [field.strip() for field in fields]
        # Remove campos vazios
        fields = [field for field in fields if field]

        # Adiciona os campos à lista principal
        all_fields.append(fields)

    return all_fields

def find_element_row(matrix, column, element):
    for i, row in enumerate(matrix):
        if row[column] == element:
            return i
    return None  # Retorna None se o elemento não for encontrado em nenhuma linha da coluna


def calculate_speedup(matrix, base_column, time_columns, result_columns):
    for i in range(len(matrix)):
        T1 = float(matrix[i][base_column])
        for j in range(len(time_columns)):
            TN = float(matrix[i][time_columns[j]])
            if TN == 0:
                speedup = 0
            else:
                speedup = T1 / TN
            matrix[i][result_columns[j]] = speedup
    return matrix

def calculate_trees_per_second(matrix, tree_column, time_columns, result_columns):
    for i in range(len(matrix)):
        T1 = float(matrix[i][tree_column])
        for j in range(len(time_columns)):
            TN = float(matrix[i][time_columns[j]])
            if TN == 0:
                speedup = 0
            else:
                speedup = T1 / TN
            matrix[i][result_columns[j]] = speedup
    return matrix

def extract_data_from_summary(summary_text, matrix_temporaria=[]):
    # Expressão regular para extrair os dados entre '|'
    #pattern = re.compile(r'\|([^|]+)\|')
    #pattern = re.compile(r'\|([^|]*)\|')
    pattern = re.compile(r'\|\s*(!?\+?[^|]*)\s*\|')
    execution_type = extract_execution_type(summary_text)
    if execution_type == 'app_BF-SEQ':
        pos = 1
    elif execution_type == 'app_BF-ADJACENCY':
        pos = 2
    elif execution_type == 'app_BF-EDGES':
        pos = 3
    elif execution_type == 'app_BF-CYCLES':
        pos = 4

    for line in summary_text.split('\n'):
        matches = pattern.findall(line)
        if matches:
            # Use split('|') para dividir a linha em campos
            matches = [field.strip() for field in line.split('|') if field.strip()]

        if matches and not check_keywords_and_line_presence(line):
            # Remove espaços em branco ao redor de cada match
            data = [match.strip() for match in matches]
            instance = extract_instance_name(data[0])
            position = find_element_row(matrix_temporaria, 0, instance)
            # Expressão regular para extrair os valores antes do primeiro parêntese
            time = float(re.match(r'^\s*([^\(]+)', data[3]).group(1).strip())
            stretch = int(re.match(r'^\s*([^\(]+)', data[4]).group(1).strip())
            trees = int(data[8])
            if position is not None:
                matrix_temporaria[position][pos] = time
                matrix_temporaria[position][12] = stretch
                matrix_temporaria[position][13] = trees
            else:
                matrix_local = [None] * GLOBAL_MAX_COLUMNS
                matrix_local[0] = instance
                matrix_local[pos] = float(re.match(r'^\s*([^\(]+)', data[3]).group(1).strip())
                matrix_local[12] = int(re.match(r'^\s*([^\(]+)', data[4]).group(1).strip())
                matrix_local[13] = int(data[8])
                matrix_temporaria.append(matrix_local)

    return matrix_temporaria

def ordenar_matriz(matriz, coluna, ordem='nd'):
    """
    Ordena uma matriz por uma coluna específica.

    Args:
    - matriz (list): A matriz a ser ordenada.
    - coluna (int): O índice da coluna pela qual a matriz será ordenada.
    - ordem (str): A ordem desejada ('nc' para não-crescente, 'nd' para não-decrescente).
                  O padrão é 'nc'.

    Returns:
    - list: A matriz ordenada.
    """
    # Validando a ordem
    if ordem not in ['nc', 'nd']:
        raise ValueError("A ordem deve ser 'nc' ou 'nd'.")

    # Usando a função sorted para ordenar a matriz
    matriz_ordenada = sorted(matriz, key=lambda x: x[coluna], reverse=(ordem == 'nc'))

    return matriz_ordenada

def process_data(directories):
    matrix_header = ["Instance", "Avg time sequencial", "Avg time adjacency", "Avg time edges",
                     "Avg time cycles", "speedup adjacency", "speedup edges", "speedup cycles",
                     "Trees per second - seq", "Trees per second - adj", "Trees per second - edg",
                     "Trees per second - cyc", "stretch index", "Total trees", "eucl dist"]
    matrix_data = []
    matrix_temporaria = []

    for subdirectory in directories:
        summary_text = read_summary_file(subdirectory)
        execution_type = extract_execution_type(summary_text)
        matrix_data = extract_data_from_summary(summary_text, matrix_temporaria)

        # Extract other relevant data from summary_text
        # ...

        # Update matrix_data accordingly
        # ...


    return matrix_header, matrix_data

def map_positions(A, B):
    # Crie um dicionário onde as chaves são os elementos de B
    # e os valores são as posições correspondentes em A
    positions = {element: A.index(element) if element in A else None for element in B}
    return positions

def rearrange_columns(matrix, order):
    new_matrix = []
    for row in matrix:
        temp = []
        for value in order:
            temp.append(row[value])
        new_matrix.append(temp)
    # new_matrix = [list(row[i] for i in order) for row in matrix]
    return new_matrix
def transform_matrix(matrix, columns_order_matrix, columns_order_latex):
    # Mapear as colunas da matriz para a ordem da tabela LaTeX
    positions = map_positions(columns_order_matrix, columns_order_latex)
    matrix_latex = []
    for key in columns_order_latex:
        matrix_latex.append(positions[key])

    # Selecionar colunas na ordem desejada e reorganizar
    transformed_matrix = rearrange_columns(matrix, matrix_latex)
    return transformed_matrix

def replace_underscore(input_string):
    return input_string.replace("_", "\\_")

def generate_tables2(matrix):
    # Colunas na ordem específica para a tabela LaTeX
    columns_order_matrix =[
        "Instance",
        "Avg time sequential",
        "Avg time adjacency",
        "Avg time edges",
        "Avg time cycles",
        "Speedup adjacencies",
        "Speedup edges",
        "Speedup cycles",
        "Trees per minute(sequential)",
        "Trees per minute(adjacency)",
        "Trees per minute(edges)",
        "Trees per minute(cycles)",
        "Stretch Index",
        "Total trees generated",
        "Degrees distribution",
        "Euclidian Distance",
    ]

    columns_order_latex = [
        "Instance",
        "Degrees distribution",
        "Avg time sequential",
        "Avg time adjacency",
        "Avg time edges",
        "Avg time cycles",
        "Total trees generated",
        "Euclidian Distance"
    ]

    # Mapear as colunas da matriz para a ordem da tabela LaTeX
    matrix_latex = transform_matrix(matrix, columns_order_matrix, columns_order_latex)

    # Criar o diretório de saída se não existir
    output_dir = "output"
    os.makedirs(output_dir, exist_ok=True)

    # Gerar tabela LaTeX
    latex_table = "\\begin{tabular}{ll|cccc|ll}\n\\cline{3-6}\n"
    latex_table += " & & \\multicolumn{4}{c|}{Average Time (s)} & & \\\\\n\\hline\n"

    # Adicionar cabeçalhos de coluna
    for column in columns_order_latex:
        latex_table += "\\multicolumn{1}{|c|}{" + column.replace('_', '\\_') + "} & "

    latex_table = latex_table[:-2]  # Remover o último "&"
    latex_table += " \\\\\n\\hline\n"

    # Adicionar dados da matriz à tabela LaTeX
    for row in matrix_latex:
        #latex_table += "\\multicolumn{1}{|l|}{} & "
        latex_table += ""

        for column, string in enumerate(columns_order_latex):
            if column in [0, 1]:
                row[column] = replace_underscore(row[column])
                if column == 0:
                    latex_table += "\\multicolumn{1}{l|}{" + str(row[column]) + "} & "
                else:
                    latex_table += "\\multicolumn{1}{r|}{" + str(row[column]) + "} & "
            else:
                if row[column] < 1:
                    latex_table += "\\multicolumn{1}{r|}{" + str(round(row[column], 5)) + "} & "
                else:
                    latex_table += "\\multicolumn{1}{r|}{" + str(round(row[column], 2)) + "} & "

        latex_table = latex_table[:-2]  # Remover o último "&"
        latex_table += " \\\\\n\\hline\n"

    latex_table += "\\end{tabular}"

    # Salvar tabela LaTeX em um arquivo .tex
    latex_file_path = os.path.join(output_dir, "output_table.tex")
    with open(latex_file_path, "w") as latex_file:
        latex_file.write(latex_table)

    print(f"Tabela LaTeX salva em: {latex_file_path}")

    # Gerar tabela de texto
    text_table = " ; ".join(columns_order_latex) + "\n"

    for row in matrix_latex:
        text_table += " ; ".join(str(row[column]) for column,string in enumerate(columns_order_latex)) + "\n"

    # Salvar tabela de texto em um arquivo .txt
    text_file_path = os.path.join(output_dir, "output_table.txt")
    with open(text_file_path, "w") as text_file:
        text_file.write(text_table)

    print(f"Tabela de texto salva em: {text_file_path}")


def generate_tables(matrix):
    # Colunas a serem incluídas na tabela
    columns_to_include = [
        "Instance",
        "Degrees distribution",
        "Avg time sequential",
        "Avg time adjacency",
        "Avg time edges",
        "Avg time cycles",
        "Total trees generated",
        "Euclidean Distance"
    ]

    # Criar o diretório de saída se não existir
    output_dir = "output"
    os.makedirs(output_dir, exist_ok=True)

    # Gerar tabela LaTeX
    latex_table = "\\begin{tabular}{ll|cccc|ll}\n\\cline{3-6}\n"
    latex_table += " & & \\multicolumn{4}{c|}{Average Time (s)} & & \\\\\n\\hline\n"

    # Adicionar cabeçalhos de coluna
    for column in columns_to_include:
        latex_table += "\\multicolumn{1}{|c|}{" + column.replace('_', '\\_') + "} & "

    latex_table = latex_table[:-2]  # Remover o último "&"
    latex_table += " \\\\\n\\hline\n"

    # Adicionar dados da matriz à tabela LaTeX
    for row in matrix:
        latex_table += "\\multicolumn{1}{|l|}{} & "

        for column, string in enumerate(columns_to_include):
            latex_table += "\\multicolumn{1}{c|}{" + str(row[column]) + "} & "

        latex_table = latex_table[:-2]  # Remover o último "&"
        latex_table += " \\\\\n\\hline\n"

    latex_table += "\\end{tabular}"

    # Salvar tabela LaTeX em um arquivo .tex
    latex_file_path = os.path.join(output_dir, "output_table.tex")
    with open(latex_file_path, "w") as latex_file:
        latex_file.write(latex_table)

    print(f"Tabela LaTeX salva em: {latex_file_path}")

    # Gerar tabela de texto
    text_table = "\t".join(columns_to_include) + "\n"

    for row in matrix:
        text_table += "\t".join(str(row[column]) for column, string in enumerate(columns_to_include)) + "\n"

    # Salvar tabela de texto em um arquivo .txt
    text_file_path = os.path.join(output_dir, "output_table.txt")
    with open(text_file_path, "w") as text_file:
        text_file.write(text_table)

    print(f"Tabela de texto salva em: {text_file_path}")


def main():
    input_params = read_input_parameters()
    directories = list_subdirectories(GLOBAL_DIR_DATA, input_params.directories)
    matrix_header, matrix_data = process_data(directories)
    matrix_data = calculate_speedup(matrix_data, 1, [2, 3, 4],[5, 6, 7])
    matrix_data = calculate_trees_per_second(matrix_data, 13, [1, 2, 3, 4],[8, 9, 10, 11])
    matrix_euclidiana = read_csv_file('euclidean_distance.csv')
    #matrix_euclidiana = read_csv_file('cosine_similarity.csv')
    matrix_data = merge_matrices_euclidiana(matrix_data, matrix_euclidiana)
    matrix_data = ordenar_matriz(matrix_data, 15, ordem='nd')
    generate_tables2(matrix_data)
    #main_plot_graph_times(matrix_header, matrix_data,'linear', start=0, end=23, filename='graph-0-22')
    #main_plot_graph_times(matrix_header, matrix_data, 'linear', start=23, filename='graph-23-end')
    main_plot_graph_times(matrix_header, matrix_data, 'linear', filename='graph-0-end')
    main_plot_graph_trees_times(matrix_header, matrix_data, 'linear', filename='trees-0-end')
    #main_plot_graph_cosine(matrix_header, matrix_data, 'linear', filename='graph-0-end')
    # Display or save the matrix as needed
    # ...


if __name__ == "__main__":
    main()
