#This is a very simple script that will output a list of files in a given directory. The list is printed at the console.
#The input is the folder that will be printed passed as command line argument.

#You may pass a file extension as an optional argument to filter some of the files.

#Examples:
#python3 list_files.py ../instances/
#Should print every file inside the ../instances/ folder

#python3 list_files.py ../instances/ .tsp.json
#Should print every file with the extension ".tsp.json" in the ../instances/ folder


import sys
import os

extension = ""
directory = sys.argv[1]
if len(sys.argv) > 2:
    extension = sys.argv[2]

for subdir, dirs, files in os.walk(directory):
    for file in files:
        filepath = os.path.join(subdir, file)
        if file.endswith(extension) or extension == "":
            print(filepath.lstrip(directory))