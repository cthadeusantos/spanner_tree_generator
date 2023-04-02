# Python program to
# calculate average and deviation
# for t-admissibility
# sum_edges not work
import math
import os

# Python program to get average of a list
def average(lst):
    return sum(lst) / len(lst)

FINISH = 8 # data at line from instance [ 0(instance) ... 8(threads) ]

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


root_dir = '.'
lista_diretorio = os.listdir(root_dir)
lista_diretorio.sort()
heuristic = ''
tipo = ''
for item in lista_diretorio:
    if item[:5] == 'ct-WA' or item[:5] == 'ct-ER' or item[:5] == 'ct-BI' or item[:5] == 'ct-BA':
        item = item.split('/')
        filename = item[0] + '/' + 'result_summary.txt'
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
                    num_thread = '4'
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

                if len(dictionary[k1][k2][k3][k4]['lb']) and len(dictionary[k1][k2][k3][k4]['s']):
                    soma = 0
                    for index, value in enumerate(dictionary[k1][k2][k3][k4]['s']):
                        soma = soma + (value - dictionary[k1][k2][k3][k4]['lb'][index])
                    media = soma/size
                soma = 0
                variancia = 0
                desvio_padrao=0
                if len(dictionary[k1][k2][k3][k4]['lb']) and len(dictionary[k1][k2][k3][k4]['s']):
                    for index, value in enumerate(dictionary[k1][k2][k3][k4]['s']):
                        soma = soma + ((value - dictionary[k1][k2][k3][k4]['lb'][index]) - media)**2
                    variancia = soma/size
                    desvio_padrao = math.sqrt(variancia)

                output[k1][k2][k3][k4] = {'v': sum(dictionary[k1][k2][k3][k4]['v'])/size, 'e': avg_edges[k3], 'avg_diff': media, 'deviation': desvio_padrao, 't': [], 'lb': lb,  's': []}

latex= False

if latex:
    pass
else:
    for k2 in key2:
        for k1 in key1:
            print(k2, k1,)
            for index, k3 in enumerate(key3):
                if index == 0:
                    print(f'{" ":20}', f'{k3: >8}', end=" ")
                else:
                    print(f'{k3: >8}', end=" ")
            print()
            for index, k3 in enumerate(key3):
                value = output[k1][k2][k3][k4]["e"] / 1000
                if index == 0:
                    print(f'{" ":20}', f'{value: >7.2f}k', end=" ")
                else:
                    print(f'{value: >7.2f}k', end=" ")
            print()
            for k4 in key4:
                print(f'{k4:10}', f'{"Deviation":9}', end=" ")
                for k3 in key3:
                    print(f'{output[k1][k2][k3][k4]["deviation"]: >8.2f}', end=" ")
                print()
                print(f'{" ":10}', f'{"Average":9}', end=" ")
                for k3 in key3:
                    print(f'{output[k1][k2][k3][k4]["avg_diff"]: >8.2f}', end=" ")
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
