# Python program to
# calculate average and deviation
# for t-admissibility
# sum_edges not work
import math

# Python program to get average of a list
def average(lst):
    return sum(lst) / len(lst)

FINISH = 8 # data at line from instance [ 0(instance) ... 8(threads) ]


directory='results/'
#directory=''
files = ['examples.txt'] # Arquivos que contem os dados
#files = ['barabasi.txt']
graphs = {}

for open_file in files: # Inicia leitura de arquivos de dados
    xpto = directory + open_file
    _file = open(xpto, 'r') # Opening file
    
    dictionary = {}
    counter = 0
    #keys = []
    values = []
    
    for line in _file:
        if line == '\n':
            continue
        chunk = line.split("=")
        #key = chunk[0]
        value = chunk[1].replace('\n', '')

        #keys.append(key)
        values.append(value)

        if counter == FINISH:
            # values[1] # solution type
            value2 = int(values[2]) # num vertices
            value3 = int(values[3]) # num edges
            value4 = int(values[4]) # lower bound
            value5 = int(values[5]) # stretch index
            value7 = float(values[7]) # running time
            #if value2 not in dictionary:
            if values[1] not in dictionary:
                #dictionary[value2] = {values[1]: {"LB": [value4], 'stretch': [value5], 'edges': [value3], 'time': [value7]}}
                dictionary[values[1]] = {
                    value2: {"LB": [value4], 'stretch': [value5], 'edges': [value3], 'time': [value7]}}
            #elif values[1] not in dictionary[value2]:
            elif value2 not in dictionary[values[1]]:
                #dictionary[value2][values[1]] = {"LB": [value4], 'stretch': [value5], 'edges': [value3], 'time': [value7]}
                dictionary[values[1]][value2] = {"LB": [value4], 'stretch': [value5], 'edges': [value3], 'time': [value7]}
            else:
                #lb = dictionary[value2][values[1]]['LB']
                lb = dictionary[values[1]][value2]['LB']
                lb.append(value4)
                #stretch = dictionary[value2][values[1]]['stretch']
                stretch = dictionary[values[1]][value2]['stretch']
                stretch.append(value5)
                #edges = dictionary[value2][values[1]]['edges']
                edges = dictionary[values[1]][value2]['edges']
                edges.append(value3)
                #time = dictionary[value2][values[1]]['time']
                time = dictionary[values[1]][value2]['time']
                time.append(value7)
                #dictionary[value2][values[1]] = {"LB": lb, 'stretch': stretch, 'edges': edges, 'time': time}
                dictionary[values[1]][value2] = {"LB": lb, 'stretch': stretch, 'edges': edges, 'time': time}
            counter = 0
            keys = []
            values = []
        else:
            counter += 1
    _file.close() # Closing file
    name = open_file.split('.')[0]
    graphs[name]=dictionary

sum_edges = {}
for graph in graphs:
    for solution in graphs[graph]:

        for vertex in graphs[graph][solution]:
            lower_avg = average(graphs[graph][solution][vertex]['LB'])
            stretch_avg = average(graphs[graph][solution][vertex]['stretch'])
            edges_avg = average(graphs[graph][solution][vertex]['edges'])
            if solution not in sum_edges:
                sum_edges[solution] = {vertex: edges_avg}
            elif vertex not in sum_edges[solution]:
                sum_edges[solution][vertex]=edges_avg
            else:
                sum_edges[solution][vertex] = sum_edges[solution][vertex] + edges_avg

            diff_stretch_lower = stretch_avg - lower_avg
            soma = 0
            for value in graphs[graph][solution][vertex]['stretch']:
                soma = soma + (value - stretch_avg) * (value - stretch_avg)
            variation = soma / len(graphs[graph][solution][vertex]['stretch'])
            deviation = math.sqrt(variation)

            # average = sum(dictionary[vertices][solution]['stretch']) / dictionary[vertices][solution]['counter']
            # diff_H_lower = average - dictionary[vertices][solution]['LB']
            # stretch = dictionary[vertices][solution]['stretch']
            # soma = 0
            # average_table.append(diff_H_lower)
            # for value in stretch:
            #     soma = soma + (value - average) * (value - average)
            # variation = soma / dictionary[vertices][solution]['counter']
            # variation_table.append(variation)

            graphs[graph][solution][vertex].update({'deviation': deviation})
            graphs[graph][solution][vertex].update({'diff': diff_stretch_lower})

avg_graphs={}

for solution in sum_edges:
    for vertex in sum_edges[solution]:
        if solution not in avg_graphs:
            avg_graphs[solution]={vertex: sum_edges[solution][vertex]/len(files)/ 1000}
        else:
            avg_graphs[solution][vertex]=(sum_edges[solution][vertex]/len(files)) / 1000

keys_graphs=list(graphs.keys())
keys_solutions=list(graphs[keys_graphs[0]].keys())
keys_vertices=list(graphs[keys_graphs[0]][keys_solutions[0]].keys())

for solution in keys_solutions:
    print()
    print(f'{solution:^100}')
    print(f'{"         ":10}', f'{"n":^10}',  end=" ")
    for vertex in keys_vertices:
        print(f'{vertex: >6}', end=" ")
    print()
    print(f'{"         ":10}', f'{"Av(m)":^10}' , end=" ")
    for vertex in keys_vertices:
        print(f'{avg_graphs[solution][vertex]: >5.1f}k', end=" ")
    print()
    for graph in keys_graphs:
        print(f'{graph:10}', f'{"Deviation":10}', end=" ")
        for vertex in keys_vertices:
            print(f'{graphs[graph][solution][vertex]["deviation"]: >6.2f}', end=" ")
        print()
        print(f'{"          ":10}', f'{"Average":10}', end=" ")
        for vertex in keys_vertices:
            print(f'{graphs[graph][solution][vertex]["diff"]: >6.2f}', end=" ")
        print()
