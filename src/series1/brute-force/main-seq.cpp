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



/// @brief  The main method
int main(int argc, char** argv){
	Graph graph;

	WatchdogTimer timer;

	std::string filename {get_filename_v2()};
	std::string run_name {"BRUTE_FORCE"};

	unsigned int lower_limit {1};

	DEBUG(std::cerr << "****************************************" << std::endl);
    DEBUG(std::cerr << "* " << global_parameters.global_max_threads_supported << " concurrent threads are supported *" << std::endl);
	DEBUG(std::cerr << "****************************************" << std::endl);

	// Check argv parameters, Input Redirected and data avaliable in redirected
	validateInputBeforeExecution(argc, argv);

	global_parameters.parseArgs(argc, argv); //Will setup globals

	DEBUG(std::cerr << filename << std::endl);
	DEBUG(std::cerr << "Wait! Reading the graph." << std::endl);

	//TODO: Leitura do grafo
	if (global_parameters.global_matrix_t==1)
		graph = read_graph_file_edges_list();
	else
		graph = read_graph_file();

	DEBUG(std::cerr << "Vertices number => " << graph.numVertices() << std::endl);
	DEBUG(std::cerr << "Edges number    => " << graph.numEdges() << std::endl);

	// Calculate the lower bound
	if (!global_parameters.global_nolb){
		graph.girth = OpBasic::maxLowerCicle(graph);
		lower_limit = graph.girth - 1;
		graph.set_lowerBound(lower_limit);
	}

	DEBUG(std::cerr << "Lower bound computed: " << lower_limit << std::endl);
	DEBUG(std::cerr << "Calculate stretch index."<< std::endl);
	DEBUG(std::cerr << "Solving with sequential "<< run_name << " - wait!" << std::endl);

	// Start time counting
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

	// Time limited ?
    if (global_parameters.global_running_time > 0){ // Time limited
        timer.kick(global_parameters.global_running_time);
        Stretch().sequential(graph);
        timer.stop();
    } else { // time unlimited
        Stretch().sequential(graph);
    }

	// End time counting
	std::chrono::time_point<std::chrono::steady_clock>	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration(end - start);
	double lastExecutionTime = execution_duration.count();

	output_data(run_name, filename, lastExecutionTime, graph);
    return 0;
};