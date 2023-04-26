# Convert an adjacency matrix to edges' list
#
# Usage
# python3 adjacency2edges.py < [INPUT_FILE] > [OUTPUT_FILE]
#
# INPUT             OUTPUT
#   3               3
#   0 1 0    ====>  0 1
#   1 0 1           1 2
#   0 1 0


import fileinput
import re

output = ''
count_line = 0
for line in fileinput.input():
    regex = re.compile(r'[\n\r\t]')
    line = regex.sub("", line)
    reading = line.rstrip().lstrip()
    array = reading.split(' ')
    if len(array) == 1:
        if array[0] != '':
            output += array[0] + '\n'
    if len(array) > 1:
        count_column = 0
        for value in array:
            if value == '1':
                output += str(count_line) + ' ' + str(count_column) + '\n'
            count_column += 1
        count_line += 1
print(output)
