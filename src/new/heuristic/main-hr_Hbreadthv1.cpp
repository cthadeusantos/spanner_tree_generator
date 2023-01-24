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
	std::cout << "\t-t X | --thread X\tDefine the numbers of threads. X is the number of threads [current " << num_threads << "]" << std::endl  << std::endl ;

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
		else if(arg == "-t" || arg == "--thread"){
			num_threads = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed number of threads to: " << num_threads << '\n';
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
	DEBUG std::cerr << "Solving with breadth heuristic - vertex importance- wait!\n";
	run_name = "Breadth_heuristic";
	Heuristic::breadth_heuristic_1(graph);

	// End time counting
	std::chrono::time_point<std::chrono::steady_clock>	end = std::chrono::steady_clock::now();	
	std::chrono::duration<double> execution_duration(end - start);
	double lastExecutionTime = execution_duration.count();
	output_data(run_name, filename, output,best, lastExecutionTime, lower_limit, graph);

	sem_destroy(&semaforo);
    return 0;
};

