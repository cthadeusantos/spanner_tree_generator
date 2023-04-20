import os

root_dir = 'batteries'
lista_diretorio = os.listdir(root_dir)
lista_diretorio.sort()
for item in lista_diretorio:
    #if (item[:5] == '03-ct' or item[:5] == '03-ct' or item[:5] == '03-ct' or item[:5] == '03-ct'):
    if (item[:10] == '03-ct-1000' and item[12:15] == 'SEQ'):
        #valor = 'python3 execution-battery-analyzer.py ../workspace/zudio-bf-par-exa' + item
        valor = 'nohup python3 execution-battery.py 1 < ' + root_dir + '/' + item
        os.system(valor)