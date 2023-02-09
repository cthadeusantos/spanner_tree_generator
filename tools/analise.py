# DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED
# DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED
# DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED
# DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED

# Python program to
# calculate average and deviation
# for t-admissibility
# sum_edges not work

# DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED
# DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED

import math

FINISH = 8 # data from instance [0(instance) - 8(threads)]


directory='results/'
#directory=''
files = ['barabasi.txt', 'bipartite.txt', 'erdos.txt', 'watts.txt'] # Arquivos que contem os dados
#files = ['barabasi.txt']
graphs = {}

# sum_edges not work
for open_file in files: # Inicia leitura de arquivos de dados
    xpto = directory + open_file
    _file = open(xpto, 'r') # Opening file
    counter = 0
    dictionary = {}
    keys = []
    values = []

    for line in _file:
        if line == '\n':
            continue
        chunk = line.split("=")
        key = chunk[0]
        value = chunk[1].replace('\n', '')

        keys.append(key)
        values.append(value)

        if counter == FINISH:
            value2 = int(values[2]) # num vertices
            value3 = int(values[3]) # num edges
            value4 = int(values[4]) # lower bound
            value5 = int(values[5]) # stretch index
            if value2 not in dictionary:
                dictionary[value2] = {values[1]: {"LB": value4, "counter": 1, 'stretch': [value5], 'sum_edges': value3, 'avg_edges': 0}}
            elif values[1] not in dictionary[value2]:
                dictionary[value2][values[1]] = {"LB": value4, "counter": 1, 'stretch': [value5], 'sum_edges': value3, 'avg_edges': 0}
            else:
                counter = dictionary[value2][values[1]]['counter'] + 1
                stretch = dictionary[value2][values[1]]['stretch']
                stretch.append(value5)
                # sum_edges not work
                sum_edges = dictionary[value2][values[1]]['sum_edges'] + value3
                dictionary[value2][values[1]] = {"LB": value4, "counter": counter, 'stretch': stretch, 'sum_edges': sum_edges, 'avg_edges': sum_edges/counter}
            counter = 0
            keys = []
            values = []
        else:
            counter += 1

    solution_type = ['H1v1', 'H1v2', 'H2v1', 'H2v2', 'H3v1', 'H3v2', 'H4v1', 'H4v2']

    table = {}
    for solution in solution_type:
        average_table = []
        variation_table = []
        for vertices in dictionary:
            average = sum(dictionary[vertices][solution]['stretch']) / dictionary[vertices][solution]['counter']
            diff_H_lower = average - dictionary[vertices][solution]['LB']
            stretch = dictionary[vertices][solution]['stretch']
            soma = 0
            average_table.append(diff_H_lower)
            for value in stretch:
                soma = soma + (value - average) * (value - average)
            variation = soma / dictionary[vertices][solution]['counter']
            variation_table.append(variation)
        table[solution] = {'a': average_table, 'b': variation_table}

    name = open_file.split('.')[0]
    graphs[name] = table
    _file.close()

output = {}

for graph in graphs:
    solutions = graphs[graph]
    for solution in solutions:
        if output:
            if solution in output:
                output[solution][graph]= {'variation': graphs[graph][solution]['b'], 'average': graphs[graph][solution]['a']}
            else:
                output[solution] = {
                    graph: {'variation': graphs[graph][solution]['b'], 'average': graphs[graph][solution]['a']}}
        else:
            output[solution] = {
                graph: {'variation': graphs[graph][solution]['b'], 'average': graphs[graph][solution]['a']}}

# Organiza apresentação pelas classes de grafos
graphs = ['barabasi', 'erdos',  'watts', 'bipartite']
for solution in output:
    print()
    print(solution)
    print(f'{"Classe":10}', f'{"          "}', end=" ")
    for i in range(10):
        print(f'{i*100+100:4}', end=" ")
        #vertices=i*100+100
        #value=dictionary[vertices][solution]['avg_edges']/100
        #print(f'{i*100+100:4}', end=" ")
        #print(f'{value:3.1f}', end=" ")
    print()
    for graph in graphs:
        print(f'{graph:10}', f'{"Deviation":10}', end=" ")
        for i in range(10):
            x = math.sqrt(output[solution][graph]['variation'][i])
            print(f'{x:2.2f}', end=" ")
        print()
        print(f'{"          "}', f'{"Average":10}', end=" ")
        for i in range(10):
            x = output[solution][graph]['average'][i]
            print(f'{x:2.2f}', end=" ")
        print()
        print()
    print()