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

#include "../code/watchdog.hpp"
#include "../code/graph.hpp"
#include "../code/opBasic.hpp"
#include "../code/stretch.hpp"

#include "../libs/stgio.hpp"
#include "../libs/stgoutput.hpp"

// #include "../code/parallel.hpp"
// #include "../code/frontier.hpp"
// #include "../code/genGraph.hpp"
// #include "../code/heuristic.hpp"
// #include "../code/centrality.hpp"


/// @brief  The main method
int main(int argc, char** argv){
	MyWatchdogTimer wdt;
	//global_parameters.global_threads_supported = std::thread::hardware_concurrency();
	//global_parameters.global_closeness = 0;

	DEBUG std::cerr << "********************************************" << std::endl;
    DEBUG std::cerr << "* " << global_parameters.global_max_threads_supported << " concurrent threads are supported *" << std::endl;
	DEBUG std::cerr << "********************************************" << std::endl ;

	// Check argv parameters, Input Redirected and data avaliable in redirected
	validateInputBeforeExecution(argc, argv);

	std::string filename = get_filename_v2();
	std::string run_name  = "";

	DEBUG std::cerr << filename << std::endl;

	global_parameters.parseArgs(argc, argv); //Will setup globals

	DEBUG std::cerr << "Reading the graph - wait\n";

	//TODO: Leitura do grafo
	Graph graph;
	if (global_parameters.global_matrix_t==1)
		graph = read_graph_file_edges_list();
	else
		graph = read_graph_file();

	DEBUG std::cerr << "Vertices number => " << graph.numVertices() << std::endl;
	DEBUG std::cerr << "Edges number    => " << graph.numEdges() << std::endl;

	int lower_limit = 1;
	if (!global_parameters.global_nolb){
		graph.girth = OpBasic::maxLowerCicle(graph);
		lower_limit = graph.girth - 1;
	}
	DEBUG std::cerr << "Lower bound computed: " << lower_limit << std::endl;

	// Start time counting
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

	run_name = "BRUTE_FORCE";
	DEBUG std::cerr << "Calculate stretch index.\n";
	DEBUG std::cerr << "Solving with sequential "<< run_name << " - wait!\n";

	//Stretch acme; // Lonney Tunes rocks!
    if (global_parameters.global_running_time > 0){
        wdt.kick(global_parameters.global_running_time);
        Stretch().sequential(graph);
        wdt.stop();
    } else {
        Stretch().sequential(graph);
    }

	// End time counting
	std::chrono::time_point<std::chrono::steady_clock>	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration(end - start);
	double lastExecutionTime = execution_duration.count();

	// OUTPUT - nothing - screen - file - debug
	output_data(run_name, filename, lastExecutionTime, lower_limit, graph);
    return 0;
};

