/**
 * @file main_create.cpp
 * @authors { Carlos Thadeu [CT] / Anderson Zudio[AZ](contributor)}
 * @brief Application to create graphs
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

#include "Debug.h"

//#include "my_libs/ctfunctions.cpp"

#include "code/parameters.hpp"
#include "my_libs/ctfunctions2.hpp"
//#include "my_libs/externals.hpp"
#include "code/parallel.hpp"
/* #include "code/sequential_functions.hpp" */

#include "code/frontier.hpp"
#include "code/genGraph.hpp"
#include "code/graph.hpp"
#include "code/heuristic.hpp"
#include "code/opBasic.hpp"
#include "code/stretch.hpp"
#include "code/centrality.hpp"

///Basic debugging controller. See Debug.h for details.
/* #ifdef MN_BF_SEQ_DEBUG
	#define DEBUG
#else
	#define DEBUG while(false)
#endif */

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


int output = 0;
bool best = false;
int matrix_t=0;

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
	std::cout << "Usage: " << app_name << " [OPTIONS] " << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\t-h | --help \t\tShow this message." << std::endl;

	std::cout << "If running is 0 (create tree) - parameters must be necessary" << std::endl;
	std::cout << "\t\t\t\t -tri | --triangle\tCreate with triangles (default)" << std::endl;
	std::cout << "\t\t\t\t -notri | --notriangle\tCreate without triangles (-tri is default)" << std::endl;
	std::cout << "\t\t\t\t -min X\t\t\tMinimum number of vertices in the graph (default=5)" << std::endl;
	std::cout << "\t\t\t\t -max X\t\t\tMaximum number of vertices in the graph (default=10)" << std::endl;
	std::cout << "\t\t\t\t -qty X\t\t\tNumber of graphs will be create (default=1)" << std::endl;
	std::cout << "\t\t\t\t -prob X\t\tProbability of vertex has edge (Between 0.45 and 1.00) (default=0.65)" << std::endl;
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
	// default values
	graph_type = true;
	min_vertices = 5;
	max_vertices = 10;
	num_graph = 1;
	probability = .65;

	for(int i = 1; i < argc; ++i){
		std::string arg(argv[i]);

		DEBUG std::cerr << "Received param: " << arg << '\n';
		if(arg == "-h" || arg == "--help"){
			usage(argv[0]);
			exit(0);
		}
		else if(arg == "-tri" || arg == "--triangle"){
			graph_type = true;
			DEBUG std::cerr << "Changed triangle to: " << graph_type << '\n';
		}
		else if(arg == "-notri" || arg == "--notriangle"){
			graph_type = false;
			DEBUG std::cerr << "Changed triangle to: " << graph_type << '\n';
		}
		else if(arg == "-min"){
			min_vertices = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed minimum to: " << min_vertices << '\n';
		}
		else if(arg == "-max"){
			max_vertices = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed maximum to: " << max_vertices << '\n';
		}
		else if(arg == "-qty"){
			num_graph = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed number of graphs to: " << num_graph << '\n';
		}
		else if(arg == "-prob"){
			probability = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed probability to: " << probability << '\n';
		}
		else {
			DEBUG std::cerr << "Unknown param: " << arg << "\n";
			exit(1);
		};
	};
}

/// @brief  The main method
int main(int argc, char** argv){
	num_threads = 1;
	//max_induced_cycles = 1;
	if(argc < 2){
		usage("--help");
		exit(0);
	}

	parseArgs(argc, argv); //Will setup globals

	DEBUG std::cerr << "Creating graphs - WAIT!\n";
	create_new_graphs();

    return 0;
};

