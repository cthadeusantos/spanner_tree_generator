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
	MyWatchdogTimer wdt;

	//num_threads = 1;
	//max_induced_cycles = 1;
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
    graph = read_graph_file();
	DEBUG std::cerr << "Quantidade de vertices => " << graph.getQtdVertices() << std::endl;
	DEBUG std::cerr << "Quantidade de arestas => " << graph.get_num_edges() << std::endl;

	// Start time counting
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	int lower_limit = 1;
	if (!nolb)
		lower_limit = OpBasic::maxLowerCicle(graph) - 1;
	DEBUG std::cerr << "Lower bound: " << lower_limit << std::endl;
	
	sem_init(&semaforo, 0, num_threads);

	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with heuristic 3 version 2 - wait!\n";
	run_name = "H3v2";
		
	if (running_time > 0){
        wdt.kick(running_time);
        Heuristic::heuristica_3v2(graph);
        wdt.stop();
    } else {
        Heuristic::heuristica_3v2(graph);
    }
	

	// End time counting
	std::chrono::time_point<std::chrono::steady_clock>	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration(end - start);
	double lastExecutionTime = execution_duration.count();
	output_data(run_name, filename, output,best, lastExecutionTime, lower_limit, graph);

	sem_destroy(&semaforo);
    return 0;
};

