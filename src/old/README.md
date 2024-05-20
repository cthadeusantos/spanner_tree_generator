# spanner_tree_generator

### executables
* main_paralelo -> Receive a graph (file .txt), and executes parallel brute-force algorithm.
* main_run_all - > Receive a graph (file .txt), and executes brute-force sequential, brute-force parallel, heuristic 1 and 2 strategies.
* main_run_all2 -> Receive a graph (file .txt), and executes brute-force sequential, heuristics 1 and 2.

### Generator of Graphs
* Setup parameters in generator.py file. Path to store the graphs must be created in advance.

### executable .sh
* exec_compare.sh -> Executes the graphs with sequential algorithm comparing it to heuristics H1, H2 and parallel algorithm. (Built by running "make build_compare")
* exec_compare_heu.sh -> Executes the graphs with heuristics strategies comparing them against two lower bounds (Built by running "make build_compare_heu")
