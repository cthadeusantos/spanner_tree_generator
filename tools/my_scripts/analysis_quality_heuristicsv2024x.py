# Python program to
# calculate average and deviation
# for t-admissibility
# and BUILD LATEX TABLES FROM result_summary.txt in directories ct-WA
import sys
from datetime import date
import math
import os

# import sys
# name_of_script = sys.argv[0]
# position = sys.argv[1]
# sample = sys.argv[2]

import argparse
parser = argparse.ArgumentParser()
parser.add_argument('-l', '--latex', dest='latex', action='store_true')

args = parser.parse_args()
latex = args.latex

GLOBAL_DIR_DATA = 'data'
class InputParameters:
    def __init__(self):
        self.directories = []

def read_input_parameters():
    params = InputParameters()

    if len(sys.argv) > 1:
        params.directories = sys.argv[1:]

    return params


def list_subdirectories(data_dir, directories):
    if not directories:
        directories = [d for d in os.listdir(data_dir) if os.path.isdir(os.path.join(data_dir, d))]

    return directories

# Python program to get average of a list
def average(lst):
    return sum(lst) / len(lst)


def truncate_float(num, digits):
    l = str(float(num)).split('.')
    digits = min(len(l[1]), digits)
    return l[0] + '.' + l[1][:digits]


def main(latex=False):
    FINISH = 8 # data at line from instance [ 0(instance) ... 8(threads) ]

    print("Diretório corrente: ", os.getcwd())
    input_params = read_input_parameters()
    directories = list_subdirectories(GLOBAL_DIR_DATA, input_params.directories)
    directories.sort()
    lista_diretorio = directories
    heuristic = ''
    tipo = ''
    dictionary = {}
    output = {}
    key1 = ['64']
    key2 = ['H1v3', 'H1v4',]
    key3 = ['100', '200', '300', '400', '500', '600', '700', '800', '900', '1000']
    key4 = ['Barabasi', 'Erdos', 'Watts', 'Bipartite']
    for k1 in key1:
        dictionary[k1] = {}
        for k2 in key2:
            dictionary[k1][k2] = {}
            for k3 in key3:
                dictionary[k1][k2][k3] = {}
                for k4 in key4:
                    dictionary[k1][k2][k3][k4] = {'v': [], 'e': [], 't': [], 'lb': [],  's': []}

    # root_dir = '../../workspace'
    # lista_diretorio = os.listdir(root_dir)

    for item in lista_diretorio:
        # if (item[:5] == 'ct-WA' or item[:5] == 'ct-ER' or item[:5] == 'ct-BI' or item[:5] == 'ct-BA')\
        #         and ('24032023-01' in item or '23032023-01' in item or '22032023-01' in item\
        #         or '21032023-01' in item or '20032023-01' in item or '19032023-01' in item\
        #         or '29032023-01' in item or '28032023-01' in item or '25032023-01' in item):
        item = item.split('/')
        filename = GLOBAL_DIR_DATA + '/' + item[0] + '/' + 'result_summary.txt'
        print(filename)
        file1 = open(filename, 'r')
        while True:

            # Get next line from file
            line = file1.readline()

            # if line is empty
            # end of file is reached
            if not line:
                break
            chave = 'Test description: Running heuristic '
            if chave in line:
                heuristic = line.split(chave)[1].strip()
                print("{}".format(heuristic))

            if line[0] == '|' and 'Instance' not in line and 'Subdir':
                line_splitted = line.split('|')
                if 'watts' in line_splitted[2]:
                    tipo = 'Watts'
                elif 'barabasi' in line_splitted[2]:
                    tipo = 'Barabasi'
                elif 'bi_partido' in line_splitted[2]:
                    tipo = 'Bipartite'
                elif 'erdos' in line_splitted[2]:
                    tipo = 'Erdos'

                num_thread = line_splitted[2].split('/')[1].split('-')
                if len(num_thread) == 1:
                    num_thread = '64'
                else:
                    auxiliar={'02': '8', '03': '16', '04': '32', '05': '64'}
                    num_thread = auxiliar[num_thread[1]]

                vertices = line_splitted[6].strip()
                edges = line_splitted[7].strip()
                lower_bound = line_splitted[8].strip()
                clock = line_splitted[4].strip().split('(')[0].strip()
                stretch = line_splitted[5].strip().split('(')[0].strip()
                dictionary[num_thread][heuristic][vertices][tipo]['v'].append(int(vertices))
                dictionary[num_thread][heuristic][vertices][tipo]['e'].append(int(edges))
                dictionary[num_thread][heuristic][vertices][tipo]['lb'].append(int(lower_bound))
                dictionary[num_thread][heuristic][vertices][tipo]['t'].append(float(clock))
                dictionary[num_thread][heuristic][vertices][tipo]['s'].append(int(stretch))

        file1.close()

    avg_edges = {}
    for key in key3:
        avg_edges[key] = []

    k4 = ''
    for k1 in key1:
        for k2 in key2:
            for k3 in key3:
                for k4 in key4:
                    if len(dictionary[k1][k2][k3][k4]['e']):
                        avg_edges[k3].extend(dictionary[k1][k2][k3][k4]['e'])


    for key in key3:
        if len(avg_edges[key]) != 0:
            avg_edges[key] = sum(avg_edges[key])/len(avg_edges[key])
        else:
            avg_edges[key] = 0

    for k1 in key1:
        output[k1] = {}
        for k2 in key2:
            output[k1][k2] = {}
            for k3 in key3:
                output[k1][k2][k3] = {}
                for k4 in key4:
                    size = len(dictionary[k1][k2][k3][k4]['v'])
                    if size == 0:
                        size = 1
                    lb = 0
                    media=0
                    sum_stretch = 0
                    sum_lower_bound = 0
                    average_stretch = 0
                    average_lower_bound = 0

                    if len(dictionary[k1][k2][k3][k4]['lb']) and len(dictionary[k1][k2][k3][k4]['s']):
                        soma = 0
                        for index, value in enumerate(dictionary[k1][k2][k3][k4]['s']):
                            soma = soma + (value - dictionary[k1][k2][k3][k4]['lb'][index])
                            sum_stretch += value
                            sum_lower_bound += dictionary[k1][k2][k3][k4]['lb'][index]
                        media = soma/size
                        average_stretch = sum_stretch / size
                        average_lower_bound = sum_lower_bound / size

                    soma = 0
                    variancia = 0
                    desvio_padrao=0
                    if len(dictionary[k1][k2][k3][k4]['lb']) and len(dictionary[k1][k2][k3][k4]['s']):
                        for index, value in enumerate(dictionary[k1][k2][k3][k4]['s']):
                            soma = soma + ((value - dictionary[k1][k2][k3][k4]['lb'][index]) - media)**2
                        variancia = soma/size
                        desvio_padrao = math.sqrt(variancia)

                    output[k1][k2][k3][k4] = {
                        'v': sum(dictionary[k1][k2][k3][k4]['v'])/size,
                        'e': avg_edges[k3], 'avg_diff': media,
                        'deviation': desvio_padrao,
                        't': [],
                        'lb': lb,
                        's': [],
                        'avg_stretch': average_stretch,
                        'avg_lower': average_lower_bound
                    }

    # LATEX TABLE MODEL - EXIT

    #\begin{table}[H]
    #\begin{footnotesize}
    #\caption{\label{table:H4v2}H4v2 - ESCREVA O QUE QUISER AQUI }
    #\hspace{-2.2cm}\small\begin{tabular}{|l|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|>{\centering\arraybackslash}p{1cm}|}\hline\\diagbox{Class}{$n$ \\ $Av(m)$}
    # & 100 1.9k & 200 7.6k & 300 17.2k & 400 30.6k & 500 47.8k & 600 68.8k & 700 93.7k & 800 122.3k & 900 154.9k & 1000 191.2k\\ \hline
    #"\textrm{barabasi}& \backslashbox[1.4cm]{1.9}{0.3}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}\\ \hline"
    #\textrm{erdos}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}\\ \hline
    #\textrm{watts}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}\\ \hline
    #\textrm{bipartite}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}& \backslashbox[1.4cm]{2.0}{0.0}\\ \hline
    #\end{tabular}
    #\end{footnotesize}
    #\end{table}

    colunas = 10
    vertex_column = 'rr|' * colunas
    string = ''.join((
        '\\begin{table}[]\n',
        '\\begin{tabular}{|c|',
        vertex_column,
        '}\n\hline\n',
        'Cabecalho',
    ))

    # storing today's date in a variable
    today = date.today().isoformat()
    latex = True
    if latex:
        # Build latex table and save at file
        for k2 in key2: # Chave para os tipos de heuristicas
            string = ''.join((
                '\\begin{table}[!ht]\n',
                '\\begin{footnotesize}\n',
                '\\begin{center}\n',
                '\\scalebox{.7}{\n',
                '\\begin{tabular}{|c|',
                vertex_column,
                '}\n\hline\n',
                '\diagbox{Class}{\\begin{tabular}[c]{@{}c@{}}$|V|$ \\\ $Av(m)$\\end{tabular}}',
                #'\diagbox{Class}{$|V|$ \\ $Av(m)$}',
            ))

            for k1 in key1: # Chaves para as threads
                # string = "\\begin{table}[H]\n\\begin{footnotesize}\caption{\label{table:"
                # MyTuple = (string, k2, k1, "}", k2, " ", str(k1), " threads", "}\n")
                # string = "".join(MyTuple)
                # string1 = "\\hspace{-2.2cm}\\small\\begin{tabular}{|l|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|}\\hline\\diagbox{Class}{$n$ \\\ $Av(m)$}\n"
                # MyTuple = (string, string1)
                # string = ''.join(MyTuple)
                if int(k1) == 64:
                    for index, k3 in enumerate(key3): # Chave para quantidade de vertices
                        # k4 is the last graph name from k4 (WARNING! PAY ATTENTION IF YOU CHANGE ANYTHING)
                        # it is only reference to get average edges in any graphs
                        # they are the same in all graphs
                        value1 = str(truncate_float(output[k1][k2][k3][k4]["e"] / 1000, 1))
                        value2 = str(truncate_float(k3, 0))

                        string += '& \multicolumn{2}{c|}{\\begin{tabular}[c]{@{}c@{}}' + value2 + '\\\ ' + value1 + 'k \end{tabular}} '
                        # ''.join((string, '& \multicolumn{2}{c|}{\\begin{tabular}[c]{@{}c@{}}',
                        #         value2,
                        #         '\\\ (',
                        #         value1,
                        #         'k \end{tabular}} ',
                        #          )
                        #         )
                        #string = string + "&" + " " + str(value2) + " " + str(value) +"k"
                    #''.join((string, "\\\ \\hline\n"))
                    string += "\\\ \\hline\n"

                    for k4 in key4:
                        #string = string + "\n\\textrm{" + k4 + "}"
                        ''.join((string, '\multirow{2}{*}{', k4, '} '))
                        linha1 = [k4]
                        linha2 = [' ']
                        for k3 in key3:
                            deviation = output[k1][k2][k3][k4]["deviation"]
                            color = 'blue' if deviation == 0 else 'red'
                            deviation = str(truncate_float(deviation, 2))
                            avg_diff = str(truncate_float(output[k1][k2][k3][k4]["avg_diff"], 2))
                            avg_lb = str(truncate_float(output[k1][k2][k3][k4]["avg_lower"], 2))
                            #str_linha1 = ' & \multicolumn{1}{r|}{\\textcolor{' + color + '}{' + deviation + '}} & ' + '- '
                            str_linha1 = ' & \multicolumn{1}{r|}{\\textbf{\\textcolor{' + color + '}{' + deviation + '}}} & ' + '- '
                            str_linha2 = ' & \multicolumn{1}{r|}{'+ avg_diff + '} & ' + avg_lb + ' '
                            linha1.append(str_linha1)
                            linha2.append(str_linha2)
                            #string = string + "& \\backslashbox[1.4cm]{" + str(avg_diff) + "}" + "{" + str(deviation) + "}"
                        endsubscribe = str(colunas * 2 + 1)
                        linha1.append('\\\ \cline{2-' + endsubscribe + '} ')
                        linha2.append('\\\ \hline ')
                        sum_strings = linha1 + linha2
                        for value in sum_strings:
                            string += value
                    string += '\\end{tabular}\n'
                    string += '} % End of scalebox\n'
                    string += '\caption{Analysis of ' + k2 + ' for the presented graphs classes. In each cell, the bottom (the top) values is the average (the standard deviation) between the difference of the returned value of '+ k2 + ' and the smallest-e-cycle lower bound(bottom left) and lower bound value(bottom right).}\label{table:' + k2 + '_64}'
                    string += '\\end{center}\n'
                    string += '\\end{footnotesize}\n'
                    string += '\\end{table}\n'
                    # If folder doesn't exists, create it ##
                    output_dir = "results/"
                    #DIR = output_dir + today + "/"
                    os.makedirs(output_dir, exist_ok=True)
                    # If folder doesn't exists, create it ##
                    # if not os.path.isdir(DIR):
                    #     os.mkdir(DIR)
                    filename = output_dir + "table" + k2 + "_" + k1 + ".tex"
                    text_file = open(filename, "w")
                    text_file.write(string)
                    text_file.close()
    else:
        # Build at screen summary about graphs
        for k2 in key2:
            for k1 in key1:
                print(k2, ';', k1, ';')
                for index, k3 in enumerate(key3):
                    if index == 0:
                        print(f'{" ; ;":30}', f'{k3: >8};', end=" ")
                    else:
                        print(f'{k3: >8};', end=" ")
                print()
                for index, k3 in enumerate(key3):
                    value = output[k1][k2][k3][k4]["e"] / 1000
                    if index == 0:
                        print(f'{" ; ;":30}', f'{value: >7.2f}k;', end=" ")
                    else:
                        print(f'{value: >7.2f}k;', end=" ")
                print()
                for k4 in key4:
                    print(f'{k4:10};', f'{"Deviation":19};', end=" ")
                    for k3 in key3:
                        print(f'{output[k1][k2][k3][k4]["deviation"]: >8.2f};', end=" ")
                    print()
                    print(f'{" ;":10}', f'{"Average (S - LB)":19};', end=" ")
                    for k3 in key3:
                        print(f'{output[k1][k2][k3][k4]["avg_diff"]: >8.2f};', end=" ")
                    print()
                    print(f'{" ;":10}', f'{"Average lower bound":9};', end=" ")
                    for k3 in key3:
                        print(f'{output[k1][k2][k3][k4]["avg_lower"]: >8.2f};', end=" ")
                    print()
                print('\n')

    #
    #             for solution in keys_solutions:
    #                 print()
    #                 print(f'{solution:^100}')
    #                 print(f'{"         ":10}', f'{"n":^10}', end=" ")
    #                 for vertex in keys_vertices:
    #                     print(f'{vertex: >6}', end=" ")
    #                 print()
    #                 print(f'{"         ":10}', f'{"Av(m)":^10}', end=" ")
    #                 for vertex in keys_vertices:
    #                     print(f'{avg_graphs[solution][vertex]: >5.1f}k', end=" ")
    #                 print()
    #                 for graph in keys_graphs:
    #                     print(f'{graph:10}', f'{"Deviation":10}', end=" ")
    #                     for vertex in keys_vertices:
    #                         print(f'{graphs[graph][solution][vertex]["deviation"]: >6.2f}', end=" ")
    #                     print()
    #                     print(f'{"          ":10}', f'{"Average":10}', end=" ")
    #                     for vertex in keys_vertices:
    #                         print(f'{graphs[graph][solution][vertex]["diff"]: >6.2f}', end=" ")
    #                     print()

if __name__ == "__main__":
    main()