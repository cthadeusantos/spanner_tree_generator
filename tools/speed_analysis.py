# Python program to
# calculate average and deviation
# for t-admissibility
# and BUILD LATEX TABLES FROM result_summary.txt in directories ct-WA


import math
import os
import re
import matplotlib.pyplot as plt
import numpy as np

def quicksort(array):
    """Sort the array by using quicksort.
    https: // stackoverflow.com / questions / 18262306 / quicksort -
    with-python"""

    less = []
    equal = []
    greater = []

    if len(array) > 1:
        pivot = array[0]
        for x in array:
            if x < pivot:
                less.append(x)
            elif x == pivot:
                equal.append(x)
            elif x > pivot:
                greater.append(x)
        # Don't forget to return something!
        return quicksort(less)+equal+quicksort(greater)  # Just use the + operator to join lists
    # Note that you want equal ^^^^^ not pivot
    else:  # You need to handle the part at the end of the recursion - when you only have one element in your array, just return the array.
        return array

# Python program to get average of a list
def average(lst):
    return sum(lst) / len(lst)


def truncate_float(num, digits):
    l = str(float(num)).split('.')
    digits = min(len(l[1]), digits)
    return l[0] + '.' + l[1][:digits]

#FINISH = 8 # data at line from instance [ 0(instance) ... 8(threads) ]

print("Please wait!")

dictionary = {}
output = {}
key1 = ['4', '8', '16', '32', '64']
key2 = ['H1v1', 'H1v2', 'H2v1', 'H2v2', 'H3v1', 'H3v2', 'H4v1', 'H4v2r1', 'H4v2r3']
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


root_dir = '../workspace'
lista_diretorio = os.listdir(root_dir)
lista_diretorio.sort()
heuristic = ''
tipo = ''

#regex1 = '-([0-9][0-9])' # select directory with minus sign and two digits

for item in lista_diretorio:
    if item.endswith('-01'):
        aux = root_dir + '/' + item
        lista_diretorio1 = os.listdir(aux)
        lista_diretorio1.sort()
        reg = re.compile(r'^grafo')
        lista = list(filter(reg.search, lista_diretorio1))

        if lista[0] == 'grafos':
            continue
        aux = aux + '/' + lista[0]
        lista_diretorio2 = os.listdir(aux)
        for directory in lista_diretorio2:
            aux2 = aux + '/' + directory
            lista_files = os.listdir(aux2)
            lista_files.sort()
            for filename in lista_files:
                if filename.endswith('.out.txt'):
                    aux3 = aux2 + '/' + filename
                    file1 = open(aux3, 'r')
                    values = {}
                    while True:

                        # Get next line from file
                        line = file1.readline()

                        # if line is empty
                        # end of file is reached
                        if not line:
                            break
                        chave = '='
                        auxiliary = line.strip().replace('\n', '').split(chave)
                        if auxiliary[0] != '':
                            auxiliary[0] = auxiliary[0].strip()
                            auxiliary[1] = auxiliary[1].strip()
                            values[auxiliary[0]] = auxiliary[1]
                    file1.close()
                    keys = ['INSTANCE', 'THREADs', 'SOLUTION_TYPE', 'NUM_VERTICES', 'NUM_EDGES', 'LOWER_BOUND', 'STRETCH_INDEX', 'SUM_TREES', 'RUNNING_TIME' ]
                    if len(values):
                        num_thread = values[keys[1]]
                        heuristic = values[keys[2]]
                        vertices = values[keys[3]]
                        tipo = ''
                        if 'gba_' in values[keys[0]]:
                            tipo = key4[0]
                        elif 'gws_' in values[keys[0]]:
                            tipo = key4[2]
                        elif 'gb_' in values[keys[0]]:
                            tipo = key4[3]
                        elif 'ger_' in values[keys[0]]:
                            tipo = key4[1]
                        else:
                            tipo = values[keys[0]]

                        edges = values[keys[4]].strip()
                        lower_bound = values[keys[5]].strip()
                        clock = values[keys[8]].strip().split('(')[0].strip()
                        stretch = values[keys[6]].strip().split('(')[0].strip()
                        dictionary[num_thread][heuristic][vertices][tipo]['v'].append(int(vertices))
                        dictionary[num_thread][heuristic][vertices][tipo]['e'].append(int(edges))
                        dictionary[num_thread][heuristic][vertices][tipo]['lb'].append(int(lower_bound))
                        dictionary[num_thread][heuristic][vertices][tipo]['t'].append(float(clock))
                        dictionary[num_thread][heuristic][vertices][tipo]['s'].append(int(stretch))

                    #dictionary[k1][k2][k3][k4] = {'v': [], 'e': [], 't': [], 'lb': [], 's': []}

print('Calculando!')
dados = []
colors = []

# grafico = {}
# for k1 in key1:             # Thread
#     for k2 in key2:         # Type
#         for k3 in key3:     # Vertices
#             for k4 in key4: # Grafo
#                 grafico[k1][k2][k3][k4]['t'] = []

set_color = {'H1v1': '#636EFA', 'H1v2': '#EF553B',
             'H2v1': '#00CC96', 'H2v2': '#4e806b',
             'H3v1': '#703201', 'H3v2': '#15006d',
             'H4v1': '#711997', 'H4v2r1': '#ff3166', 'H4v2r3': '#ff6a00',
             }
coluna = []

for k5 in key4:  # Grafo
    for k6 in key3:  # Vertices
        overlay_threads = {'4': [], '8': [], '16': [], '32': [], '64': []}
        for k1 in key1:             # Thread
            for k2 in key2:         # Type
                for k3 in key3:     # Vertices
                    for k4 in key4: # Grafo
                        if k3 == k6 and k4 == k5:
                            clock = quicksort(dictionary[k1][k2][k3][k4]['t'])
                            overlay_threads[k1].append(clock)
                            #colors.append(set_color[k2])
                            if k1 == '4':
                                if k2 not in coluna:
                                    coluna.append(k2)
        fig = plt.figure()
        fig.suptitle(k5 + " - " + k6 + " vertices")
        plt.subplots_adjust(left=0.1,
                            bottom=0.1,
                            right=0.9,
                            top=0.9,
                            wspace=0.4,
                            hspace=0.4)
        # Defining Parameters
        params = {
            'axes.titlesize': 6,
            'axes.labelsize': 6,
            'axes.titleweight': 'normal',
            'ytick.labelsize': 6,
        }
        # Updating the rcParams in Matplotlib
        plt.rcParams.update(params)

        ax1 = plt.subplot(231)
        ax1.title.set_text('4 threads')
        #ax1.title.set_size(6)
        ax2 = plt.subplot(232)
        ax2.title.set_text('8 threads')
        #ax2.title.set_size(6)
        ax3 = plt.subplot(233)
        ax3.title.set_text('16 threads')
        #ax3.title.set_size(6)
        ax4 = plt.subplot(234)
        ax4.title.set_text('32 threads')
        #ax4.title.set_size(6)
        ax5 = plt.subplot(235)
        ax5.title.set_text('64 threads')
        #ax5.title.set_size(6)
        ax1.set_yscale('log')
        ax2.set_yscale('log')
        ax3.set_yscale('log')
        ax4.set_yscale('log')
        ax5.set_yscale('log')
        ax1.boxplot(overlay_threads['4'])
        ax1.set_ylabel("time(s)")
        ax2.boxplot(overlay_threads['8'])
        ax2.set_ylabel("time(s)")

        ax3.boxplot(overlay_threads['16'])
        ax3.set_ylabel("time(s)")
        ax4.boxplot(overlay_threads['32'])
        ax4.set_ylabel("time(s)")
        ax5.boxplot(overlay_threads['64'])
        ax5.set_ylabel("time(s)")
        ax1.set_xticklabels(coluna, rotation=90, fontsize=7)
        ax2.set_xticklabels(coluna, rotation=90, fontsize=7)
        ax3.set_xticklabels(coluna, rotation=90, fontsize=7)
        ax4.set_xticklabels(coluna, rotation=90, fontsize=7)
        ax5.set_xticklabels(coluna, rotation=90, fontsize=7)
        #plt.show()
        plt.savefig('images/' + k5 + '-' + k6 + '.png', dpi=300, format='png', bbox_inches='tight')

#for k1 in key1:
#    for k2 in key2:
#        for k3 in key3:
#            for k4 in key4:
#                clock = quicksort(dictionary[k1][k2][k3][k4]['t'])
#                size = len(clock)
#                if not size:
#                    continue
#
#                if size % 2:
#                    mid = size // 2
#                    Q2 = clock[mid]
#                    range1 = []
#                    range2 = []
#                    for i in range(0, mid):
#                        range1.append(clock[i])
#                    for i in range(mid + 1, size):
#                        range2.append(clock[i])
#                    mid = len(range1) // 2
#                    Q1 = range1[mid]
#                    mid = len(range2) // 2
#                    Q3 = range2[mid]
#                else:
#                    mid2 = size // 2
#                    mid1 = mid2 - 1
#                    Q2 = (clock[mid1] + clock[mid2]) / 2
#                    range1 = []
#                    range2 = []
#                    for i in range(0, mid1):
#                        range1.append(clock[i])
#                    for i in range(mid2+1, size):
#                        range2.append(clock[i])
#                    size = len(range1)
#                    mid2 = size // 2
#                    mid1 = mid2 - 1
#                    Q1 = (range1[mid1] + range1[mid2]) / 2
#                    size = len(range2)
#                    mid2 = size // 2
#                    mid1 = mid2 - 1
#                    Q3 = (range2[mid1] + range2[mid2]) / 2
#                iqr = Q3 - Q1
#                minimum = Q1 - 1.5 * iqr
#                maximum = Q3 + 1.5 * iqr
#                print(k1, k2, k3, k4, 'Inf:', minimum, 'Sup:', maximum, 'Q1:', Q1, 'Q2:', Q2, 'Q3:', Q3)




#for item in lista_diretorio:
#    if item[:5] == 'ct-WA' or item[:5] == 'ct-ER' or item[:5] == 'ct-BI' or item[:5] == 'ct-BA':
#        item = item.split('/')
#        filename = item[0] + '/' + 'result_summary.txt'
#        print(filename)
#        file1 = open(filename, 'r')
#        while True:
#
#            # Get next line from file
#            line = file1.readline()
#
#            # if line is empty
#            # end of file is reached
#            if not line:
#                break
#            chave = 'Test description: Running heuristic '
#            if chave in line:
#                heuristic = line.split(chave)[1].strip()
#                print("{}".format(heuristic))
#
#            if line[0] == '|' and 'Instance' not in line and 'Subdir':
#                line_splitted = line.split('|')
#                if 'watts' in line_splitted[2]:
#                    tipo = 'Watts'
#                elif 'barabasi' in line_splitted[2]:
#                    tipo = 'Barabasi'
#                elif 'bi_partido' in line_splitted[2]:
#                    tipo = 'Bipartite'
#                elif 'erdos' in line_splitted[2]:
#                    tipo = 'Erdos'
#
#                num_thread = line_splitted[2].split('/')[1].split('-')
#                if len(num_thread) == 1:
#                    num_thread = '4'
#                else:
#                    auxiliar={'02': '8', '03': '16', '04': '32', '05': '64'}
#                    num_thread = auxiliar[num_thread[1]]
#
#                vertices = line_splitted[6].strip()
#                edges = line_splitted[7].strip()
#                lower_bound = line_splitted[8].strip()
#                clock = line_splitted[4].strip().split('(')[0].strip()
#                stretch = line_splitted[5].strip().split('(')[0].strip()
#                dictionary[num_thread][heuristic][vertices][tipo]['v'].append(int(vertices))
#                dictionary[num_thread][heuristic][vertices][tipo]['e'].append(int(edges))
#                dictionary[num_thread][heuristic][vertices][tipo]['lb'].append(int(lower_bound))
#                dictionary[num_thread][heuristic][vertices][tipo]['t'].append(float(clock))
#                dictionary[num_thread][heuristic][vertices][tipo]['s'].append(int(stretch))
#
#        file1.close()
#
#avg_edges = {}
#for key in key3:
#    avg_edges[key] = []
#
#k4=''
#for k1 in key1:
#    for k2 in key2:
#        for k3 in key3:
#            for k4 in key4:
#                if len(dictionary[k1][k2][k3][k4]['e']):
#                    avg_edges[k3].extend(dictionary[k1][k2][k3][k4]['e'])
#
#
#for key in key3:
#    if len(avg_edges[key]) != 0:
#        avg_edges[key] = sum(avg_edges[key])/len(avg_edges[key])
#    else:
#        avg_edges[key] = 0
#
#for k1 in key1:
#    output[k1] = {}
#    for k2 in key2:
#        output[k1][k2] = {}
#        for k3 in key3:
#            output[k1][k2][k3] = {}
#            for k4 in key4:
#                size = len(dictionary[k1][k2][k3][k4]['v'])
#                if size == 0:
#                    size = 1
#                lb = 0
#                media=0
#
#                if len(dictionary[k1][k2][k3][k4]['lb']) and len(dictionary[k1][k2][k3][k4]['s']):
#                    soma = 0
#                    for index, value in enumerate(dictionary[k1][k2][k3][k4]['s']):
#                        soma = soma + (value - dictionary[k1][k2][k3][k4]['lb'][index])
#                    media = soma/size
#                soma = 0
#                variancia = 0
#                desvio_padrao=0
#                if len(dictionary[k1][k2][k3][k4]['lb']) and len(dictionary[k1][k2][k3][k4]['s']):
#                    for index, value in enumerate(dictionary[k1][k2][k3][k4]['s']):
#                        soma = soma + ((value - dictionary[k1][k2][k3][k4]['lb'][index]) - media)**2
#                    variancia = soma/size
#                    desvio_padrao = math.sqrt(variancia)
#
#                soma_clock = 0
#                variancia_clock= 0
#                desvio_padrao_clock=0
#                if len(dictionary[k1][k2][k3][k4]['lb']) and len(dictionary[k1][k2][k3][k4]['s']):
#                    for index, value in enumerate(dictionary[k1][k2][k3][k4]['s']):
#                        soma_clock = soma_clock + ((value - dictionary[k1][k2][k3][k4]['lb'][index]) - media)**2
#                    variancia = soma/size
#                    desvio_padrao = math.sqrt(variancia)
#
#
#
#                output[k1][k2][k3][k4] = {'v': sum(dictionary[k1][k2][k3][k4]['v'])/size, 'e': avg_edges[k3], 'avg_diff': media, 'deviation': desvio_padrao, 't': [], 'lb': lb,  's': []}
#
#latex = True
#
#if latex:
#    # Build latex table and save at file
#    for k2 in key2:
#        for k1 in key1:
#            string = "\\begin{table}[H]\n\\begin{footnotesize}\caption{\label{table:"
#            MyTuple = (string, k2, k1, "}", k2, " ", str(k1), " threads", "}\n")
#            string = "".join(MyTuple)
#            string1 = "\\hspace{-2.2cm}\\small\\begin{tabular}{|l|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|}\\hline\\diagbox{Class}{$n$ \\\ $Av(m)$}\n"
#            MyTuple = (string, string1)
#            string = ''.join(MyTuple)
#            for index, k3 in enumerate(key3):
#                # k4 is the last graph name from k4 (WARNING! PAY ATTENTION IF YOU CHANGE ANYTHING)
#                # it is only reference to get average edges in any graphs
#                # they are the same in all graphs
#                value = truncate_float(output[k1][k2][k3][k4]["e"] / 1000, 1)
#                value2 = truncate_float(k3, 1)
#                string = string + "&" + " " + str(value2) + " " + str(value) +"k"
#            string = string + "\\\ \\hline\n"
#
#            for k4 in key4:
#                string = string + "\n\\textrm{" + k4 + "}"
#                for k3 in key3:
#                    deviation = truncate_float(output[k1][k2][k3][k4]["deviation"], 2)
#                    avg_diff = truncate_float(output[k1][k2][k3][k4]["avg_diff"], 2)
#                    string = string + "& \\backslashbox[1.4cm]{" + str(avg_diff) + "}" + "{" + str(deviation) + "}"
#                string = string + "\\\ \\hline\n"
#            string = string + "\\end{tabular}\n"
#            string = string + "\\end{footnotesize}\n"
#            string = string + "\\end{table}\n"
#            DIR = "../no_commit/tables/"
#            filename = DIR + "table" + k2 + "_" + k1 + ".tex"
#            text_file = open(filename, "w")
#            text_file.write(string)
#            text_file.close()
#else:
#    # Build at screen summary about graphs
#    for k2 in key2:
#        for k1 in key1:
#            print(k2, k1,)
#            for index, k3 in enumerate(key3):
#                if index == 0:
#                    print(f'{" ":20}', f'{k3: >8}', end=" ")
#                else:
#                    print(f'{k3: >8}', end=" ")
#            print()
#            for index, k3 in enumerate(key3):
#                value = output[k1][k2][k3][k4]["e"] / 1000
#                if index == 0:
#                    print(f'{" ":20}', f'{value: >7.2f}k', end=" ")
#                else:
#                    print(f'{value: >7.2f}k', end=" ")
#            print()
#            for k4 in key4:
#                print(f'{k4:10}', f'{"Deviation":9}', end=" ")
#                for k3 in key3:
#                    print(f'{output[k1][k2][k3][k4]["deviation"]: >8.2f}', end=" ")
#                print()
#                print(f'{" ":10}', f'{"Average":9}', end=" ")
#                for k3 in key3:
#                    print(f'{output[k1][k2][k3][k4]["avg_diff"]: >8.2f}', end=" ")
#                print()
#            print('\n')
#