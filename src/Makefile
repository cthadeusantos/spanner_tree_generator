CODE_DIR=code

all:
	clear
	g++ -std=c++11 -pthread -g -O3 -w ${CODE_DIR}/*.cpp main.cpp -o exec.out

build_gen_graph:
	clear
	g++ -std=c++11 -g -O3 -w ${CODE_DIR}/*.cpp main_generate_graph.cpp -o gen_graph.out
	
build_run_all:
	clear
	g++ -std=c++11 -pthread -g -O3 -w -lrt ${CODE_DIR}/*.cpp main_run_all.cpp -o run_all.out

build_run_all2:
	clear
	g++ -std=c++11 -g -O3 -w -lrt ${CODE_DIR}/*.cpp main_run_all2.cpp -o run_all2.out

build_run_par:
	clear
	g++ -std=c++11 -pthread -g -O3 -lrt ${CODE_DIR}/*.cpp main_paralelo.cpp -o run_par.out

build_compare:
	clear
	g++ -std=c++11 -pthread -g -O3 -lrt ${CODE_DIR}/*.cpp main_compare.cpp -o compare.out

build_compare_heu:
	clear
	g++ -std=c++11 -g -O3 -lrt ${CODE_DIR}/*.cpp main_compare_heu.cpp -o compare_heu.out

build_teste:
	clear
	g++ -std=c++11 -g -O3 -lrt ${CODE_DIR}/*.cpp main_teste.cpp -o teste.out