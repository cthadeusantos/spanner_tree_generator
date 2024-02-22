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

k4=''
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

                soma_clock = 0
                variancia_clock= 0
                desvio_padrao_clock=0
                if len(dictionary[k1][k2][k3][k4]['lb']) and len(dictionary[k1][k2][k3][k4]['s']):
                    for index, value in enumerate(dictionary[k1][k2][k3][k4]['s']):
                        soma_clock = soma_clock + ((value - dictionary[k1][k2][k3][k4]['lb'][index]) - media)**2
                    variancia = soma/size
                    desvio_padrao = math.sqrt(variancia)



                output[k1][k2][k3][k4] = {'v': sum(dictionary[k1][k2][k3][k4]['v'])/size, 'e': avg_edges[k3], 'avg_diff': media, 'deviation': desvio_padrao, 't': [], 'lb': lb,  's': []}

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
    for k2 in key2:
        for k1 in key1:
            string = "\\begin{table}[H]\n\\begin{footnotesize}\caption{\label{table:"
            MyTuple = (string, k2, k1, "}", k2, " ", str(k1), " threads", "}\n")
            string = "".join(MyTuple)
            string1 = "\\hspace{-2.2cm}\\small\\begin{tabular}{|l|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|>{\\centering\\arraybackslash}p{1cm}|}\\hline\\diagbox{Class}{$n$ \\\ $Av(m)$}\n"
            MyTuple = (string, string1)
            string = ''.join(MyTuple)
            for index, k3 in enumerate(key3):
                # k4 is the last graph name from k4 (WARNING! PAY ATTENTION IF YOU CHANGE ANYTHING)
                # it is only reference to get average edges in any graphs
                # they are the same in all graphs
                value = truncate_float(output[k1][k2][k3][k4]["e"] / 1000, 1)
                value2 = truncate_float(k3, 1)
                string = string + "&" + " " + str(value2) + " " + str(value) +"k"
            string = string + "\\\ \\hline\n"

            for k4 in key4:
                string = string + "\n\\textrm{" + k4 + "}"
                for k3 in key3:
                    deviation = truncate_float(output[k1][k2][k3][k4]["deviation"], 2)
                    avg_diff = truncate_float(output[k1][k2][k3][k4]["avg_diff"], 2)
                    string = string + "& \\backslashbox[1.4cm]{" + str(avg_diff) + "}" + "{" + str(deviation) + "}"
                string = string + "\\\ \\hline\n"
            string = string + "\\end{tabular}\n"
            string = string + "\\end{footnotesize}\n"
            string = string + "\\end{table}\n"
            DIR = "../no_commit/tables/"
            filename = DIR + "table" + k2 + "_" + k1 + ".tex"
            text_file = open(filename, "w")
            text_file.write(string)
            text_file.close()
else:
    # Build at screen summary about graphs
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
