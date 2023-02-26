import os
import subprocess
arquivos=["graphs_vertices.txt"]
diretorios=["instances/examples/"]
#diretorio1="~/Repositories/spanner_tree_generator/instances/grafos_barabasi/"

output_file=["examples.txt"]
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
