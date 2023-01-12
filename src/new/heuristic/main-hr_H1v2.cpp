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

#include "../my_libs/ctfunctions2.hpp"
#include "../code/parallel.hpp"
#include "../code/frontier.hpp"
#include "../code/genGraph.hpp"
#include "../code/graph.hpp"
#include "../code/heuristic.hpp"
#include "../code/opBasic.hpp"
#include "../code/stretch.hpp"
#include "../code/centrality.hpp"

///Basic debugging controller. See Debug.h for details.
#ifdef MN_BF_SEQ_DEBUG
	#define DEBUG
#else
	#define DEBUG while(false)
#endif

//#include <atomic>
//std::atomic<bool> processando(true);

extern sem_t semaforo;
extern int index_global;
//extern Graph tree_global;

/**
 * @addtogroup bfseqparam
 * Available globally on the main_BF-PAR.cpp file scope.
 * @{
 */
///The seed only exists because of external tools. The algorithm itself is deterministic.
int seed = 0;
extern int num_threads;
extern int max_induced_cycles;
int type_running = 0;
int output = 0;
bool best = false;

/**
 * @brief Auxiliary function to show application usage instruction at terminal.
 * @details The instance file should be passed to standard input following the /doc/input_format.md specification.
 * The following parameters can be passed through command line:
 * `-h` or `--help` will some usage instructions.
 * The remaining parameters are optional and can be passed followed by a value:
 * PARAM		    					/ DESCRIPTION                 								/ VALUE TYPE
 * `-s` or `--seed` 					/ Setup this application seed 								/ integer
 * @param app_name The name of the application as called in the command line.
 */
void usage(const char* app_name){
	std::cout << "Usage: " << app_name << " [OPTIONS] " << "< INPUT_FILENAME >>> OUTPUT_FILENAME" << std::endl;
	std::cout << std::endl << "OPTIONS: " << std::endl;
	std::cout << "\t-h | --help \t\tShow this message." << std::endl;

	std::cout << "Show info:" << std::endl;
	std::cout << "\t-f | --file \t\tAt file. [current " << output << "]" << std::endl;
	std::cout << "\t-s | --screen \t\tAt screen. [current " << output << "]" << std::endl;
	std::cout << "\t-d | --debug \t\tAt screen only debug mode. [current " << output << "]" << std::endl;
	std::cout << "\t-b | --best \t\tShow the best tree found." << std::endl;
	std::cout << "You can combine summary, expo, debug and show" << std::endl << std::endl;
}

/**
 * @brief Auxiliary function to parse the contents of the command line arguments.
 * @authors { Anderson Zudio[AZ] /  Carlos Thadeu [CT] (contributor)}
 * @details This is a simple auxiliary function that will parse the args from the command line. You may see the entire specification at the terminal using the -help argument.
 */
void parseArgs(int argc, char** argv){
    if (argc <= 1){
        printf("You must have parameters, type -h to help!\n");
        exit(-1);
    }
	for(int i = 1; i < argc; ++i){
		std::string arg(argv[i]);

		DEBUG std::cerr << "Received param: " << arg << '\n';
		if(arg == "-h" || arg == "--help"){
			usage(argv[0]);
			exit(0);
		}
		else if(arg == "-s" || arg == "--screen"){
			output = output + 1;
			DEBUG std::cerr << "Changed output type to: " << type_running << '\n';
		}
		else if(arg == "-f" || arg == "--file"){
			output = output + 2;
			DEBUG std::cerr << "Changed output type to: " << type_running << '\n';
		}
		else if(arg == "-d" || arg == "--debug"){
			output = output + 64;
			DEBUG std::cerr << "Changed output type to: " << type_running << '\n';
		}
		else if(arg == "-b" || arg == "--best"){
			best = true;
			DEBUG std::cerr << "Changed best tree visualization to: " << best << '\n';
		}
		else {
			DEBUG std::cerr << "Unknown param: " << arg << "\n";
			exit(1);
		};
	};
};


/// @brief  The main method
int main(int argc, char** argv){
	num_threads = 1;
	max_induced_cycles = 1;
	if(argc < 2){
		usage("--help");
		exit(0);
	}

	std::string filename = get_filename();
	std::string run_name  = "";

	DEBUG std::cerr << filename << std::endl;

	DEBUG std::cerr << "Calculate stretch index.\n";
	parseArgs(argc, argv); //Will setup globals

	DEBUG std::cerr << "Reading the graph\n";

	//TODO: Leitura do grafo
	Graph graph;
    graph = read_graph_file();
	DEBUG std::cerr << "Quantidade de vertices => " << graph.getQtdVertices() << std::endl;

    int lower_limit = OpBasic::maxLowerCicle(graph) - 1;
	
	sem_init(&semaforo, 0, num_threads);
	
	// Start time counting
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

	// MAIN PROCEDURE
	DEBUG std::cerr << "Solving with heuristic 1 vertex importance- wait!\n";
	run_name = "Heuristic_1v2";
	Heuristic::heuristica_1_modified(graph);

	// End time counting
	std::chrono::time_point<std::chrono::steady_clock>	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration(end - start);
	double lastExecutionTime = execution_duration.count();

	// OUTPUT - nothing - screen - file - debug
	if ((output & 1)==1){	// TO SCREEN
		std::cout << "Input filename: " << filename << std::endl;
		std::cout << "Outputing the solution for " << run_name << std::endl;
		std::cout << "Vertices: " << graph.get_qty_vertex() << std::endl;
		std::cout << "Edges: " << graph.get_num_edges() << std::endl;
		std::cout << "Limite inferior: " << lower_limit << std::endl;
		std::cout << "Stretch index calculated: " << graph.get_stretch_index() <<  std::endl;
		std::cout << "Total de árvores calculadas: " << graph.get_total_tree() <<  std::endl;
		//std::cout << "Running time (in seconds): " << difftime(time_end, time_begin) <<  std::endl;
		std::cout << "Running time (in seconds): " << lastExecutionTime <<  std::endl;
		if (best) graph.show_best_tree();
	}
	if ((output & 2)==2){	// TO FILE
		//std::cout << filename << " " << run_name << " " << lower_limit << " " << graph.get_stretch_index() << " " << graph.get_total_tree() << " " << difftime(time_end, time_begin) << std::endl;
		std::cout <<	filename << " " <<
						run_name << " " <<
						graph.get_qty_vertex() << " " <<
						graph.get_num_edges() << " " <<
						lower_limit << " " <<
						graph.get_stretch_index() << " " <<
						graph.get_total_tree() << " " <<
						lastExecutionTime << std::endl;
		if (best) graph.show_best_tree();
	}
	if ((output & 64)==64){	// TO SCREEN AT DEBUGGER
		DEBUG std::cerr << "Input filename: " << filename << std::endl;
		DEBUG std::cerr << "Outputing the solution for " << run_name << std::endl;
		DEBUG std::cerr << "Vertices: " << graph.get_qty_vertex() << std::endl;
		DEBUG std::cerr << "Edges: " << graph.get_num_edges() << std::endl;
		DEBUG std::cerr << "Limite inferior: " << lower_limit << std::endl;
	    DEBUG std::cerr << "Stretch index calculated: " << graph.get_stretch_index() <<  std::endl;
		DEBUG std::cerr << "Total de árvores calculadas: " << graph.get_total_tree() <<  std::endl;
		//DEBUG std::cerr << "Running time (in seconds): " << difftime(time_end, time_begin) <<  std::endl;
		DEBUG std::cerr << "Running time (in seconds): " << lastExecutionTime <<  std::endl;

		if (best){
			int node1 = 0, node2 = 0;
			for (auto&& tuple: graph.best_tree){
				std::tie(node1, node2) = tuple;
				DEBUG std::cerr << "(" << node1 << " , " << node2 << ") ";
			}
		}
	}
    return 0;
};

