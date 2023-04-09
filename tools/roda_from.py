import os

root_dir = './batteries'
lista_diretorio = os.listdir(root_dir)
lista_diretorio.sort()
for item in lista_diretorio:
<<<<<<< HEAD
    if (item[:5] == 'ct-BI' or item[:5] == 'ct-BA') and (item.split('.')[-1]=='yaml'):
=======
    if (item[:5] == 'ct-BI' or item[:5] == 'ct-BA' or item[:5] == 'ct-WA' or item[:5] == 'ct-ER') and (item.split('.')[-1]=='yaml'):
>>>>>>> main
        valor = 'nohup python3 execution-battery.py 1 < batteries/' + item
        os.system(valor)

