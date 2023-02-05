import os
import subprocess
arquivos=["100_vertices.txt", "200_vertices.txt", "300_vertices.txt", "400_vertices.txt", "500_vertices.txt", "600_vertices.txt", "700_vertices.txt", "800_vertices.txt", "900_vertices.txt", "1000_vertices.txt"]
diretorios=["instances/grafos_barabasi/", "instances/grafos_bi_partido/","instances/grafos_erdos/","instances/grafos_watts/"]
#diretorio1="~/Repositories/spanner_tree_generator/instances/grafos_barabasi/"

output_file=["barabasi.txt", "bipartite.txt", "erdos.txt", "watts.txt"]
counter=0
for diretorio in diretorios:
    for num_vertex in arquivos:
        meuArquivo = open(diretorio+num_vertex)
        for line in meuArquivo:
            print(line)
            archive=line.split("/")
            diretorio=diretorio.replace('\n','')
            arquivo=archive[-1].replace('\n','')
            executable = ('build/release/app_HR-ALL -t 32 --file < ', diretorio, arquivo, ' >> ', 'results/', output_file[counter])
            exec = ''.join(executable)
            os.system(exec)
    counter=counter+1
