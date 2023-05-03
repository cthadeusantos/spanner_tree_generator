import os

root_dir = '../workspace'
lista_diretorio = os.listdir(root_dir)
lista_diretorio.sort()
for item in lista_diretorio:
    if (item[:5] == 'ct-BI' or item[:5] == 'ct-BA' or item[:5] == 'ct-ER' or item[:5] == 'ct-WA'):
        #valor = 'python3 execution-battery-analyzer.py ../workspace/zudio-bf-par-exa' + item
        valor = 'python3 execution-battery-analyzer.py ' + root_dir + '/' + item
        os.system(valor)

