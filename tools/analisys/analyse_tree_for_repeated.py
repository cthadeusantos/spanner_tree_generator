# Python program to
# demonstrate readline()

#L = ["Geeks\n", "for\n", "Geeks\n"]

# Writing to a file
#file1 = open('myfile.txt', 'w')
#file1.writelines((L))
#file1.close()

# Using readline()
#file1 = open('myfile.txt', 'r')
count = 0

import sys

arvores = []
# Lê todas as linhas do arquivo redirecionado
for line in sys.stdin:
    # Processa cada linha do arquivo
    arvores.append(line.strip())

count = 0
for i in range(0, len(arvores) - 1):
    for j in range(i + 1, len(arvores)):
        if (arvores[i]==arvores[j]):
            count += 1
            print("Repetida linhas ", i+1, j+1)
            print(arvores[i], arvores[j])

print("Repetidas ", count)