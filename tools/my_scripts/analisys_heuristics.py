import os
import re
from colorama import Fore, Back, Style  # For colored output (install with pip install colorama)

BASE_DIR = "../../workspace/grafos15-bf-seq-09012024-02/"
LIST_DIR = ["../workspace/grafos15-hr-H1v1-16012024-01/",
            "../workspace/grafos15-hr-H1v2-16012024-01/",
            "../workspace/grafos15-hr-H1v3-16012024-01/",
            "../workspace/grafos15-hr-H1v4-16012024-01/",
            "../workspace/grafos15-hr-H2v1-16012024-01/",
            "../workspace/grafos15-hr-H2v2-16012024-01/",
            "../workspace/grafos15-hr-H3v1-16012024-01/",
            "../workspace/grafos15-hr-H3v2-16012024-01/",
            "../workspace/grafos15-hr-H4v1-16012024-01/",
            "../workspace/grafos15-hr-h4v2r1-15012024-01/",
            "../workspace/grafos15-hr-h4v2r3-15012024-01/"]

# Function to extract instance name from the given line
def extract_instance_name(line):
    match = re.search(r'\/([^\/]+)\.txt', line)
    if match:
        return match.group(1)
    return None

# Function to extract stretch index from the given line
def extract_stretch_index(line):
    match = re.search(r'(\d+) \(\d+/\d+\) \[\d+\.\d+\]', line)
    if match:
        return int(match.group(1))
    return None

# Initialize the LaTeX table header
latex_table = "\\begin{table}[!ht]\n" \
              "    \\begin{footnotesize}\n" \
              "        \\begin{center}\\scalebox{0.80}\n" \
              "            {\n" \
              "                \\begin{tabular}{|l|"
for dir_name in LIST_DIR:
    latex_table += ">{\\centering\\arraybackslash}p{1cm}|"
latex_table += "}\n" \
               "\\hline\\diagbox{|V|}{$ $ \\ $Type$}\n"

# Extract header information from the base directory
base_file_path = os.path.join(BASE_DIR, "result_summary.txt")
with open(base_file_path, 'r') as base_file:
    header_line = base_file.readline()
    while "Avg stretch index (MIN/MAX) [std_dev]" not in header_line:
        header_line = base_file.readline()
    stretch_index_col = header_line.split("|").index(" Avg stretch index (MIN/MAX) [std_dev] ")

# Extract stretch index from the base directory
with open(base_file_path, 'r') as base_file:
    for line in base_file:
        if "Instance Name" in line:
            instance_name_line = base_file.readline()
            base_stretch_index = extract_stretch_index(instance_name_line)
            break

# Add headers to the LaTeX table
latex_table += "|"
for dir_name in LIST_DIR:
    result_file_path = os.path.join(dir_name, "result_summary.txt")
    with open(result_file_path, 'r') as result_file:
        for result_line in result_file:
            if "Test description: Running" in result_line:
                #column_name = re.search(r'Running ([^\s]+)', result_line).group(1)
                column_name = result_line.split()[-1]
                # Encontrar a última palavra na linha contendo "Test description: Running"
                latex_table += f" {{{column_name}}} &"
                break
latex_table = latex_table.rstrip('&')
latex_table += " \\\\ \\hline\n"

# Populate the LaTeX table with differences and colors
with open(base_file_path, 'r') as base_file:
    for line in base_file:
        if "Instance Name" not in line:
            instance_name = extract_instance_name(line)
            base_stretch_index = extract_stretch_index(line)

            if instance_name is not None:  # Check if instance_name is not None
                # Adicionamos cifrões ($) e barras invertidas (\) ao redor do nome da instância
                instance_name_latex = instance_name.replace("_", "\\_")
                latex_table += f"\\textrm{{${instance_name_latex}$}}&"

                for dir_name in LIST_DIR:
                    result_file_path = os.path.join(dir_name, "result_summary.txt")
                    with open(result_file_path, 'r') as result_file:
                        for result_line in result_file:
                            if instance_name in result_line:
                                result_stretch_index = extract_stretch_index(result_line)
                                diff = result_stretch_index - base_stretch_index
                                if diff == 0:
                                    latex_table += f" {{\\textbf{{\\color{{blue}}{diff}}}}}&"
                                else:
                                    latex_table += f" {{\\textbf{{\\color{{red}}{diff}}}}}&"
                                break
                latex_table = latex_table.rstrip('&')
                latex_table += " \\\\ \\hline\n"

latex_table += "                \\end{tabular}\n" \
               "            }\\caption{The values in each cell represent the difference between the true value found by sequential  brute-force algorithm and the value found by heuristics. [0 means a tie, in any other case, the brute force result is better].}\\label{table:graphs_10_20}\n" \
               "        \\end{center}\n" \
               "        \\hspace{-2.2cm}\\small\\small\n" \
               "    \\end{footnotesize}\n" \
               "\\end{table}"

# Print or save the LaTeX table
print(latex_table)

# Optionally, save the LaTeX table to a file
# with open("output_table.tex", 'w') as output_file:
#     output_file.write(latex_table)

