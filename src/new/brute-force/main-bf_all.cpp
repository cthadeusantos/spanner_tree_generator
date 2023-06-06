/**
 * @file main-bf.cpp
 * @authors { Carlos Thadeu [CT] / Anderson Zudio[AZ](contributor)}
 * @brief Application to solve the T-admissibility problem using brute force.
 */

#include <iostream>
#include <regex>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>

#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy
#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <ctime>

#include <chrono>	// contributor AZ

#include "../Debug.h"
#include "../code/initial_settings.hpp"

#include "../code/parameters.hpp"
#include "../my_libs/ctfunctions2.hpp"
#include "../code/parallel.hpp"
#include "../code/frontier.hpp"
#include "../code/genGraph.hpp"
#include "../code/graph.hpp"
#include "../code/heuristic.hpp"
#include "../code/opBasic.hpp"
#include "../code/stretch.hpp"
#include "../code/centrality.hpp"
#include "../code/watchdog.hpp"

extern int max_induced_cycles;

/// @brief  The main method
int main(int argc, char** argv){
	//num_threads = 1;
	//max_induced_cycles = 6;
	if(argc < 2){
		Parameters::usage("--help");
		exit(0);
	}

	std::string filename = get_filename();
	std::string run_name  = "";

	DEBUG std::cerr << filename << std::endl;

	DEBUG std::cerr << "Calculate stretch index.\n";
	Parameters::parseArgs(argc, argv); //Will setup globals

	DEBUG std::cerr << "Reading the graph\n";

	//TODO: Leitura do grafo
	Graph graph;
	if (matrix_t==1)
		graph = read_graph_file_edges_list();
	else
		graph = read_graph_file();

	DEBUG std::cerr << "Quantidade de vertices => " << graph.getQtdVertices() << std::endl;
	DEBUG std::cerr << "Quantidade de arestas => " << graph.get_num_edges() << std::endl;

    int lower_limit = OpBasic::maxLowerCicle(graph) - 1;
	DEBUG std::cerr << "Lower bound: " << lower_limit << std::endl;
	
	sem_init(&semaforo, 0, num_threads);
	
	Graph graph_copy=graph;
	// MAX DEGREE PARALLELISM
	// Start time counting
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with max degree - wait!\n";
	
	run_name = "MAX_DEGREE";
	create_threads_edge_max_degree(graph);
	// End time counting
	std::chrono::time_point<std::chrono::steady_clock>	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration0(end - start);
	double lastExecutionTime = execution_duration0.count();
	output_data(run_name, filename, output,best, lastExecutionTime, lower_limit, graph);

	// EDGE_PARALLELISM
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with edge parallelism - wait!\n";
	graph=graph_copy;
	run_name = "EDGE_PARALLELISM";
	create_threads(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration1(end - start);
	lastExecutionTime = execution_duration1.count();
	output_data(run_name, filename, output,best, lastExecutionTime, lower_limit, graph);

	// INDUCED_CYCLE
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with induced cycle - wait!\n";
	graph=graph_copy;
	run_name = "INDUCED_CYCLE";
	create_threads_induced_cycle_method_3(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration2(end - start);
	lastExecutionTime = execution_duration2.count();
	output_data(run_name, filename, output,best, lastExecutionTime, lower_limit, graph);

	// BRUTE FORCE
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with brute force - wait!\n";
	graph=graph_copy;
	run_name = "BRUTE_FORCE";
	Stretch().sequential(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration4(end - start);
	lastExecutionTime = execution_duration4.count();
	output_data(run_name, filename, output,best, lastExecutionTime, lower_limit, graph);

	sem_destroy(&semaforo);

    return 0;
};

