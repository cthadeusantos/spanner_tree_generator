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
#include "../code/initial_settings.hpp"

/// @brief  The main method
int main(int argc, char** argv){
	//num_threads = 1;
	//max_induced_cycles = 10;
	if(argc < 1){
		Parameters::usage("--help");
		exit(0);
	}

	std::string filename = get_filename_v2();
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


	// CALCULATE LOWER LIMIT
	// NOT COMPUTE IN TIME IN THIS SCRIPT
    int lower_limit = OpBasic::maxLowerCicle(graph) - 1;
	DEBUG std::cerr << "Lower bound: " << lower_limit << std::endl;
	
	sem_init(&semaforo, 0, num_threads);
	
	Graph graph_copy=graph;

	// MAIN PROCEDURE
	
	// Heuristic 1 V1
	// Start time counting
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	DEBUG std::cerr << "Solving with heuristic 1 - wait!\n";
	run_name = "H1v1";
	Heuristic::heuristica_1v1(graph);
	// End time counting
	std::chrono::time_point<std::chrono::steady_clock>	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration0(end - start);
	double lastExecutionTime = execution_duration0.count();
	output_data(run_name, filename, global_output,best, lastExecutionTime, lower_limit, graph);

	// Heuristic 1 V2
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with heuristic 1 vertex importance- wait!\n";
	graph=graph_copy;
	run_name = "H1v2";
	Heuristic::heuristica_1v2(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration1(end - start);
	lastExecutionTime = execution_duration1.count();
	output_data(run_name, filename, global_output,best, lastExecutionTime, lower_limit, graph);

	// Heuristic 2 V1
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with heuristic 2v1- wait!\n";
	graph=graph_copy;
	run_name = "H2v1";
	Heuristic::heuristic_2v1(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration2(end - start);
	lastExecutionTime = execution_duration2.count();
	output_data(run_name, filename, global_output,best, lastExecutionTime, lower_limit, graph);

	// Heuristic 2 V2
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with heuristic H2v2 - wait!\n";
	graph=graph_copy;
	run_name = "H2v2";
	Heuristic::heuristica_2v2(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration3(end - start);
	lastExecutionTime = execution_duration3.count();
	output_data(run_name, filename, global_output,best, lastExecutionTime, lower_limit, graph);

	// Heuristic 3v1
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with heuristic 3v1 - wait!\n";
	graph=graph_copy;
	run_name = "H3v1";
	Heuristic::heuristica_3v1(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration4(end - start);
	lastExecutionTime = execution_duration4.count();
	output_data(run_name, filename, global_output,best, lastExecutionTime, lower_limit, graph);

	// Heuristic 3v2
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with heuristic 3v2 - wait!\n";
	graph=graph_copy;
	run_name = "H3v2";
	Heuristic::heuristica_3v2(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration5(end - start);
	lastExecutionTime = execution_duration5.count();
	output_data(run_name, filename, global_output,best, lastExecutionTime, lower_limit, graph);

	// Breadth heuristic V1
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with breadth heuristic version 1- vertex importance- wait!\n";
	graph=graph_copy;
	run_name = "H4v1";
	Heuristic::Heuristica_4v1(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration6(end - start);
	lastExecutionTime = execution_duration6.count();
	output_data(run_name, filename, global_output,best, lastExecutionTime, lower_limit, graph);

	// Breadth heuristic V2
	// Start time counting
	start = std::chrono::steady_clock::now();
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with Heuristic 4 version 2 revisao 1 wait!\n";
	graph=graph_copy;
	run_name = "H4v2";
	Heuristic::Heuristica_4v2r1(graph);
	// End time counting
	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration7(end - start);
	lastExecutionTime = execution_duration7.count();
	output_data(run_name, filename, global_output,best, lastExecutionTime, lower_limit, graph);

	sem_destroy(&semaforo);

    return 0;
};

