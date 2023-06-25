/**
 * @file main-bf_par_cycles_m4.cpp
 * @authors { Carlos Thadeu [CT] / Anderson Zudio[AZ](contributor)}
 * @brief Application to solve the T-admissibility problem using parallelism - induced cycle from girth
 */

#include <iostream>
#include <regex>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>

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
#include "../code/cycles.hpp"

/// @brief  The main method
int main(int argc, char** argv){
	MyWatchdogTimer wdt;

	pthread_mutex_init(&mutex_signal, NULL);

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
	if (matrix_t==1)
		graph = read_graph_file_edges_list();
	else
		graph = read_graph_file();
		
	DEBUG std::cerr << "Quantidade de vertices => " << graph.getQtdVertices() << std::endl;
	DEBUG std::cerr << "Quantidade de arestas => " << graph.get_num_edges() << std::endl;

	// Start time counting
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	//Stretch().find_index(graph);

	int lower_limit = 1;
	if (!global_nolb){
		//graph.grt = OpBasic::maxLowerCicle(graph);
		graph.set_grt(graph);
		lower_limit = graph.get_grt() - 1;
		graph.set_lower_limit(lower_limit);
	}
	DEBUG std::cerr << "Lower bound: " << lower_limit << std::endl;

	sem_init(&semaforo, 0, num_threads);
	
	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with induced cycle Method 4 - PARALLEL- induced cycle from girth wait!\n";
	run_name = "INDUCED_CYCLE-M4";
	if (global_running_time > 0){
        wdt.kick(global_running_time);
        //create_threads_induced_cycle_method_4v1(graph);
		create_threads_induced_cycle_method_4_NEW_APPROACH(graph);
        wdt.stop();
    } else {
        //create_threads_induced_cycle_method_4v1(graph);
		create_threads_induced_cycle_method_4_NEW_APPROACH(graph);
    }
	
	// End time counting
	std::chrono::time_point<std::chrono::steady_clock>	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration(end - start);
	double lastExecutionTime = execution_duration.count();

	// OUTPUT - nothing - screen - file - debug
	output_data(run_name, filename, global_output,best, lastExecutionTime, lower_limit, graph);
	sem_destroy(&semaforo);
    return 0;
};

