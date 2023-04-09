# Python program to
# calculate average and deviation
# for t-admissibility
# and BUILD LATEX TABLES FROM result_summary.txt in directories ct-WA


import math
import os


# Python program to get average of a list
def average(lst):
    return sum(lst) / len(lst)


def truncate_float(num, digits):
    l = str(float(num)).split('.')
    digits = min(len(l[1]), digits)
    return l[0] + '.' + l[1][:digits]


def has_key1(key1, dicionario):
    return key1 in dicionario


def has_key2(key1, key2, dicionario):
    return key2 in dicionario[key1]


def has_key3(key1, key2, key3, dicionario):
    return key3 in dicionario[key1][key2]


def has_key4(key1, key2, key3, key4, dicionario):
    return key4 in dicionario[key1][key2][key3]

dictionary = {}
output = {}
key1 = ['1', '4', '8', '16', '32', '64']
key2 = []
key3 = []
key4 = []

dict_aux = {'v': [], 'e': [], 't': [], 'lb': [],  's': []}

root_dir = '../workspace/'
lista_diretorio = os.listdir(root_dir)
lista_diretorio.sort()
heuristic = ''
tipo = ''
for item in lista_diretorio:
    if item[:5] == 'ct-hr' or item[:5] == 'ct-bf':
        item = item.split('/')
        filename = root_dir + item[0] + '/' + 'result_summary.txt'

        isDirectory = os.path.isdir(filename)
        if isDirectory is False:
            continue
        print(filename)
        file1 = open(filename, 'r')
        while True:

            # Get next line from file
            line = file1.readline()

            # if line is empty
            # end of file is reached
            if not line:
                break
            chave = 'Test description: Running '
            if chave in line:
                if 'max' in line and 'degree' in line:
                    heuristic = 'degree'
                elif 'edges mode' in line:
                    heuristic = 'edges'
                elif 'induced cycle' in line:
                    heuristic = 'cycle'
                elif 'sequential' in line:
                    heuristic = 'seq'
                else:
                    heuristic = line.split(' ')[-1].strip()
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
                else:
                    tipo = line_splitted[2].split('/')[1]
                    tipo = tipo.split('-')[0].strip()
                    # if tipo not in key4:
                    #     key4.append(tipo)

                num_thread = line_splitted[2].split('/')[1].split('-')
                if len(num_thread) == 1:
                    if heuristic == 'seq':
                        num_thread = '1'
                    else:
                        num_thread = '4'
                else:
                    auxiliar={'02': '8', '03': '16', '04': '32', '05': '64'}
                    num_thread = auxiliar[num_thread[1]]

                vertices = line_splitted[6].strip()
                edges = line_splitted[7].strip()
                lower_bound = line_splitted[8].strip()
                clock = line_splitted[4].strip().split('(')[0].strip()
                stretch = line_splitted[5].strip().split('(')[0].strip()

                if not has_key1(num_thread, dictionary):
                    dictionary[num_thread] = {}
                if not has_key2(num_thread, heuristic, dictionary):
                    if vertices not in dictionary[num_thread] and heuristic not in key2:
                        key2.append(heuristic)
                    dictionary[num_thread][heuristic] = {}
                if not has_key3(num_thread, heuristic, vertices, dictionary):
                    if vertices not in dictionary[num_thread][heuristic] and vertices not in key3:
                        key3.append(vertices)
                    dictionary[num_thread][heuristic][vertices] = {}

                if not has_key4(num_thread, heuristic, vertices, tipo, dictionary):
                    if tipo not in dictionary[num_thread][heuristic][vertices] and tipo not in key4:
                        key4.append(tipo)
                    dictionary[num_thread][heuristic][vertices][tipo] = dict_aux = {'v': [], 'e': [], 't': [], 'lb': [],  's': []}

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
                if k1 != '1' and k2 != 'seq':
                    if has_key4(k1, k2, k3, k4, dictionary):
                        if len(dictionary[k1][k2][k3][k4]['e']):
                            avg_edges[k3].extend(dictionary[k1][k2][k3][k4]['e'])
                else:
                    if has_key4('1', 'seq', k3, k4, dictionary) and k1 == '1' and k2 == 'seq':
                        avg_edges[k3].extend(dictionary[k1][k2][k3][k4]['e'])

for key in key3:
    if len(avg_edges[key]) != 0:
        avg_edges[key] = sum(avg_edges[key])/len(avg_edges[key])
    else:
        avg_edges[key] = 0

output['64'] = {}
for k3 in key3:
    for k2 in key2:
        if k2 == 'seq' or k2 == 'degree' or k2 == 'edges' or k2 == 'cycle':
            continue
        # if k2 not in output['64']:
        #     output['64'][k2] = {}
        # if k3 not in output['64'][k2]:
        #     output['64'][k2][k3] = {}
        for k4 in dictionary['64'][k2][k3]:
            diff = dictionary['64'][k2][k3][k4]['s'][0] - dictionary['1']['seq'][k3][k4]['s'][0]
            error = round(diff / dictionary['64'][k2][k3][k4]['s'][0] * 100, 1)
            if k2 not in output['64']:
                output['64'][k2] = {}
            if k2 in output['64'] and k3 not in output['64']:
                output['64'][k2][k3] = {}
            output['64'][k2][k3][k4] = {'err': error, 'diff': diff}

latex = True


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

if latex:
    # Build latex table and save at file
    k1 = '64'
    string = "\\begin{table}[H]\n\\begin{footnotesize}\caption{\label{table:"
    MyTuple = (string, k1, k1, "}", 'Error rate', "}\n")
    string = "".join(MyTuple)
    string1 = "\\hspace{-2.2cm}\\small\\begin{tabular}{|l|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|}\\hline\\diagbox{Vertices}{$ $ \\\ $Type$}\n"
    MyTuple = (string, string1)
    string = ''.join(MyTuple)
    for index, k2 in enumerate(key2):
        # k4 is the last graph name from k4 (WARNING! PAY ATTENTION IF YOU CHANGE ANYTHING)
        # it is only reference to get average edges in any graphs
        # they are the same in all graphs
        if k2 == 'seq' or k2 == 'degree' or k2 == 'edges' or k2 == 'cycle':
            continue
        value = k2
        value2 = 2
        string = string + "&" + " " + str(value)
    string = string + "\\\ \\hline\n"

    for k3 in key3:
        for k4 in output[k1][k2][k3]:
            string = string + "\n\\textrm{" + k3 + "}"
            for k2 in key2:
                if k2 == 'seq' or k2 == 'degree' or k2 == 'edges' or k2 == 'cycle':
                    continue
                deviation = truncate_float(output[k1][k2][k3][k4]["err"], 2)
                avg_diff = truncate_float(output[k1][k2][k3][k4]["diff"], 2)
                if avg_diff != '0.0':
                    colore1 = "{\\textbf{\\color{red}" + str(avg_diff) + '}}'
                else:
                    colore1 = str(avg_diff)
                if deviation != '0.0':
                    colore2 = "{\\textbf{\\color{red}" + str(deviation) + '}}'
                else:
                    colore2 = str(deviation)
                string = string + "& \\backslashbox[1.4cm]{" + colore1 + "}" + "{" + colore2 + "}"
            string = string + "\\\ \\hline\n"
    string = string + "\\end{tabular}\n"
    string = string + "\\end{footnotesize}\n"
    string = string + "\\end{table}\n"
    DIR = "../no_commit/tables/"
    filename = DIR + "erro_grafos_10a20" + ".tex"
    text_file = open(filename, "w")
    text_file.write(string)
    text_file.close()
