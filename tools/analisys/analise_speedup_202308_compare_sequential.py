import os
import re
from statistics import mean
import matplotlib.pyplot as plt

print("Initialize. Wait!")

baseline_d = ['0_ct-bf-seq_v1-12072023-01']
base = baseline_d[0]

running_d = ['0_ct-bf-seq-08072023-01']

initial_dir = '../../'
root_dir = initial_dir + 'workspace'
images_dir = initial_dir + 'no_commit/images'
print("Read baseline.")
# LE O DIRETORIO DO BASELINE
directory1 = root_dir + "/" + baseline_d[0] + '/grafos/'
lista_diretorio = os.listdir(directory1)
lista_diretorio.sort()

folder = baseline_d[0]
baseline_dict = {folder: {}}

for item in lista_diretorio:
    subdirectory = directory1 + item
    lista_subdiretorio = os.listdir(subdirectory)
    data = {'rt': []}
    instance = ''

    for arquivo_name in lista_subdiretorio:
        if '.out.txt' in arquivo_name:
            readfile = subdirectory + "/" + arquivo_name
            fp = open(readfile, "r")

            while True:
                textfile = fp.readline()
                if not textfile:
                    break

                if "INSTANCE" in textfile:
                    instance = textfile.split("=")[1].strip('\n').strip('\'').replace('.txt', '').replace('.out', '')
                    if instance not in baseline_dict[folder]:
                        baseline_dict[folder][instance] = {}
                elif "SOLUTION_TYPE" in textfile:
                    value = textfile.split("=")[1].strip('\n').strip('\'')
                    data["solution"] = value
                elif "NUM_VERTICES" in textfile:
                    data["vertex"] = int(textfile.split("=")[1])
                elif "NUM_EDGES" in textfile:
                    data["edges"] = int(textfile.split("=")[1])
                elif "RUNNING_TIME" in textfile:
                    value = float(textfile.split("=")[1])
                    data["rt"].append(value)
                elif "STRETCH_INDEX" in textfile:
                    data["si"] = int(textfile.split("=")[1])
                elif "LOWER_BOUND" in textfile:
                    data["lb"] = int(textfile.split("=")[1])
                elif "SUM_TREES" in textfile:
                    data["ntrees"] = int(textfile.split("=")[1])
                elif "THREADS" in textfile and "NUM_THREADS" not in textfile:
                    thread = textfile.split("=")[1].strip('\n').strip('\'')
                    if thread not in baseline_dict[folder][instance]:
                        baseline_dict[folder][instance][thread] = {}
            fp.close()
        baseline_dict[folder][instance][thread] = data

# LE OS DIRETORIOS A SEREM ANALISADOS

print("Read datasets.")

main_dictionary = {}
for folder in running_d:
    if folder not in main_dictionary:
        main_dictionary[folder] = {}
    directory1 = root_dir + "/" + folder + '/grafos/'
    lista_diretorio = os.listdir(directory1)
    lista_diretorio.sort()
    running_dict = {}

    for item in lista_diretorio:
        subdirectory = directory1 + item
        lista_subdiretorio = os.listdir(subdirectory)
        data = {'rt': []}
        instance = ''
        thread = ''
        for arquivo_name in lista_subdiretorio:
            if '.out.txt' in arquivo_name:
                readfile = subdirectory + "/" + arquivo_name
                fp = open(readfile, "r")

                while True:
                    textfile = fp.readline()
                    if not textfile:
                        break
                    if "INSTANCE" in textfile:
                        instance = textfile.split("=")[1].strip('\n').strip('\'').replace('.txt', '').replace('.out','')
                        if instance not in main_dictionary[folder]:
                            main_dictionary[folder][instance] = {}
                    elif "SOLUTION_TYPE" in textfile:
                        value = textfile.split("=")[1].strip('\n').strip('\'')
                        if "CI_1" in folder:
                            data["solution"] = value + "_1"
                        elif "CI_2" in folder:
                            data["solution"] = value + "_2"
                        elif "par-v1" in folder:
                            data["solution"] = value + "_par-v1"
                        else:
                            data["solution"] = value
                    elif "NUM_VERTICES" in textfile:
                        data["vertex"] = int(textfile.split("=")[1])
                    elif "NUM_EDGES" in textfile:
                        data["edges"] = int(textfile.split("=")[1])
                    elif "RUNNING_TIME" in textfile:
                        value = float(textfile.split("=")[1])
                        data["rt"].append(value)
                    elif "STRETCH_INDEX" in textfile:
                        data["si"] = int(textfile.split("=")[1])
                    elif "LOWER_BOUND" in textfile:
                        data["lb"] = int(textfile.split("=")[1])
                    elif "SUM_TREES" in textfile:
                        data["ntrees"] = int(textfile.split("=")[1])
                    elif "TASKS" in textfile or "USED_THREADS" in textfile:
                        data["tasks"] = int(textfile.split("=")[1])
                    elif "THREADS" in textfile and ("NUM_THREADS" not in textfile or "USED_THREADS" not in textfile):
                        thread = textfile.split("=")[1].strip('\n').strip('\'')
                        if thread not in main_dictionary[folder][instance]:
                            main_dictionary[folder][instance][thread] = {}
                fp.close()
        main_dictionary[folder][instance][thread] = data

print("Compute mean.")
#for item in baseline_dict:
for instance in baseline_dict[base]:
    baseline_dict[base][instance]['1']['mean'] = mean(baseline_dict[base][instance]['1']['rt'])

print("Compute speedup.")
x_axis = set()
for folder in main_dictionary:
    for instance in main_dictionary[folder]:
        x_axis.add(instance)
        for nthread in main_dictionary[folder][instance]:
            main_dictionary[folder][instance][nthread]['mean'] = mean(main_dictionary[folder][instance][nthread]['rt'])
            main_dictionary[folder][instance][nthread]['speedup'] = baseline_dict[base][instance]['1']['mean'] / main_dictionary[folder][instance][nthread]['mean']


running = []
x_axis = list()
# solutions = list()
for folder in main_dictionary:
    # if folder not in running:
    #     running[folder] = {}
    for instance in main_dictionary[folder]:
        if instance not in x_axis:
            x_axis.append(instance)
        # if instance not in running:
        #     running[instance] = {}
        #     running[instance] = []
        # for thread in x_axis:
        #     solution = main_dictionary[folder][instance][thread]['solution']
        #     if solution not in solutions:
        #         solutions.append(solution)
        #     if solution not in running[instance]:
        #         running[instance][solution] = dict()
        #         running[instance][solution] = []
        #     speedup = main_dictionary[folder][instance][thread]['speedup']
        mean_base = mean(baseline_dict[baseline_d[0]][instance][thread]['rt'])
        mean_main = mean(main_dictionary[folder][instance][thread]['rt'])
        #x = (mean_base - mean_main)
        #speedup = (mean_base - x) / mean_base * 100
        speedup = (mean_main - mean_base)/mean_base * 100
        running.append(speedup)

# x_axis = ['4', '8', '16', '32', '64']

ax1 = ''


for instance in running:
    ax1 = plt.subplot()

    #for solution in running[folder][instance]:
    parada = running
    #ax1.plot(x_axis, parada, label=string)
    ##ax1.plot(x_axis, parada, label=instance)
    ax1.bar(x_axis, parada, width=0.2)

# Rotating X-axis labels
plt.xticks(rotation = 270)
ax1.set_ylabel("Reducao percentual")
#ax1.set_xlabel("threads")
ax1.title.set_text('Sequential v1 vs Sequential v2')
ax1.legend(loc = 'lower right', fontsize = '5', title = 'Legend', title_fontsize = '3')
#ax1.set_yscale('symlog')
#plt.show()
plt.grid(True)
plt.savefig(images_dir + '/speedup-sequential_' + baseline_d[0] + '.png', dpi=300, format='png', bbox_inches='tight')
plt.clf()
plt.close()

# for k2 in key2:
#     if k2 == 'BRUTE_FORCE':
#         continue
#     ax1 = plt.subplot()
#
#
#
#     # Updating the rcParams in Matplotlib
#     for k3 in key3:
#         vector = []
#         for k1 in key1:
#             if k1 == '1':
#                 continue
#             vector.append(overlay_threads[k2][k1][k3]['speedup'])
#
#         string = k2
#         ax1.title.set_text(string)
#         ax1.set_yscale('symlog')
#         string = str(round(overlay_threads[k2][k1][k3]['n'], 0)).replace('.0','') + ',' + str(round(overlay_threads[k2][k1][k3]['m'], 0)).replace('.0','')
#         ax1.plot(parada, vector, label=string)
#         ax1.set_ylabel("speedup")
#         ax1.set_xlabel("threads")
#         ax1.legend(loc = 'lower right', fontsize = '5', title = 'Legend', title_fontsize = '3')
#     #plt.show()
#     plt.grid(True)
#     plt.savefig(root_dir + '/images/speedup-' + k2 + '.png', dpi=300, format='png', bbox_inches='tight')
#     plt.clf()
