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

#print ('Number of arguments:', len(sys.argv), 'arguments.')
#print ('Argument List:', str(sys.argv))
input1 = sys.argv[1]
input2 = sys.argv[2]

arvores1 = []
arvores2 = []

file1 = open(input1, 'r')
file2 = open(input2, 'r')

while True:
    line = file1.readline()
    if not line:
        break
    arvores1.append(line.strip())
file1.close()

while True:
    line = file2.readline()
    if not line:
        break
    arvores2.append(line.strip())
file2.close()

count = 0
for i in range(0, len(arvores1)):
    notin = False
    for j in range(0, len(arvores2)):
        if arvores1[i] == arvores2[j]:
            notin = True
            break
    if not(notin):
        print(arvores1[i])
