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


def has_key1(key1, dicionario):
    return key1 in dicionario


def has_key2(key1, key2, dicionario):
    return key2 in dicionario[key1]


def has_key3(key1, key2, key3, dicionario):
    return key3 in dicionario[key1][key2]


def has_key4(key1, key2, key3, key4, dicionario):
    return key4 in dicionario[key1][key2][key3]

#FINISH = 8 # data at line from instance [ 0(instance) ... 8(threads) ]

print("Please wait!")

dictionary = {}
output = {}
key1 = ['1', '4', '8', '16', '32', '64']
key2 = [] # Tipo
key3 = [] # Vertices
key4 = [] # classe
# for k1 in key1:
#     dictionary[k1] = {}
#     for k2 in key2:
#         dictionary[k1][k2] = {}
#         for k3 in key3:
#             dictionary[k1][k2][k3] = {}
#             for k4 in key4:
#                 dictionary[k1][k2][k3][k4] = {'v': [], 'e': [], 't': [], 'lb': [],  's': []}


root_dir = '../../workspace'
lista_diretorio = os.listdir(root_dir)
lista_diretorio.sort()
heuristic = ''
tipo = ''

#regex1 = '-([0-9][0-9])' # select directory with minus sign and two digits

lista_diretorio = quicksort(lista_diretorio)

for item in lista_diretorio:
    if item.endswith('-01'):
        aux = root_dir + '/' + item
        lista_diretorio1 = os.listdir(aux)
        lista_diretorio1.sort()
        reg = re.compile(r'^grafo')
        lista = list(filter(reg.search, lista_diretorio1))

        if lista[0] != 'grafos':
            continue
        aux = aux + '/' + lista[0]
        lista_diretorio2 = os.listdir(aux)
        lista_diretorio2 = quicksort(lista_diretorio2)
        for directory in lista_diretorio2:
            aux2 = aux + '/' + directory
            isDirectory = os.path.isdir(aux2)
            if isDirectory is False:
                continue
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
                        tipo = values[keys[0]]
                        if heuristic not in key2:
                            key2.append(heuristic)
                        if vertices not in key3:
                            key3.append(vertices)
                        if tipo not in key4:
                            key4.append(tipo)

                        edges = values[keys[4]].strip()
                        lower_bound = values[keys[5]].strip()
                        clock = values[keys[8]].strip().split('(')[0].strip()
                        stretch = values[keys[6]].strip().split('(')[0].strip()

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
                            dictionary[num_thread][heuristic][vertices][tipo] = dict_aux = {'v': [], 'e': [], 't': [],
                                                                                            'lb': [], 's': []}

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

#overlay_threads = {'4': [], '8': [], '16': [], '32': [], '64': []}
overlay_threads = {}
for k2 in key2:  # Type
    if k2 == 'BRUTE_FORCE':
        continue
    if k2 not in overlay_threads:
        overlay_threads[k2] = {}
    for k1 in key1:  # Thread
        if k1 == '1':
            continue
        if k1 not in overlay_threads[k2]:
            overlay_threads[k2][k1] = {}
        for k3 in key3:
            if k3 not in overlay_threads[k2][k1]:
                overlay_threads[k2][k1][k3] = {}
            for k4 in dictionary[k1][k2][k3]:
                T1 = sum(dictionary['1']['BRUTE_FORCE'][k3][k4]['t']) / len(dictionary['1']['BRUTE_FORCE'][k3][k4]['t'])
                TN = sum(dictionary[k1][k2][k3][k4]['t']) / len(dictionary[k1][k2][k3][k4]['t'])
                speedup = T1 / TN
                overlay_threads[k2][k1][k3] = {'speedup': round(speedup, 2),
                                               'n': average(dictionary[k1][k2][k3][k4]['v']),
                                               'm': average(dictionary[k1][k2][k3][k4]['e'])
                                               }
parada = ['4', '8', '16', '32', '64']

for k2 in key2:
    if k2 == 'BRUTE_FORCE':
        continue
    ax1 = plt.subplot()

    # Updating the rcParams in Matplotlib
    for k3 in key3:
        vector = []
        for k1 in key1:
            if k1 == '1':
                continue
            vector.append(overlay_threads[k2][k1][k3]['speedup'])

        string = k2
        ax1.title.set_text(string)
        ax1.set_yscale('symlog')
        string = str(round(overlay_threads[k2][k1][k3]['n'], 0)).replace('.0','') + ',' + str(round(overlay_threads[k2][k1][k3]['m'], 0)).replace('.0','')
        ax1.plot(parada, vector, label=string)
        ax1.set_ylabel("speedup")
        ax1.set_xlabel("threads")
        ax1.legend(loc = 'lower right', fontsize = '5', title = 'Legend', title_fontsize = '3')
    #plt.show()
    plt.grid(True)
    plt.savefig(root_dir + '/images/speedup-' + k2 + '.png', dpi=300, format='png', bbox_inches='tight')
    plt.clf()
