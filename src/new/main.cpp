/**
 * @file main.cpp
 * @author Carlos Thadeu ()
 * @brief Application to solve the T-admissibility problem with parallelism of threads or sequential.
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

#include "Debug.h"

#include "ctfunctions.cpp"
#include "code/my_functions.hpp"
/* #include "code/sequential_functions.hpp" */

#include "code/frontier.hpp"
#include "code/genGraph.hpp"
#include "code/graph.hpp"
#include "code/heuristic.hpp"
#include "code/opBasic.hpp"
#include "code/stretch.hpp"

///Basic debugging controller. See Debug.h for details.
#ifdef MN_BF_SEQ_DEBUG
	#define DEBUG
#else
	#define DEBUG while(false)
#endif

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
int num_threads = 1;
int type_running = 0;
bool display = false;

/**
 * }@
 */

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
	std::cout << "Usage: " << app_name << " <option(s)> " << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "-h | --help\t\t\tShow this message." << std::endl;
	std::cout << "-s | --show\t\t\tShow infos at screen. [current " << display << "]" << std::endl;
	std::cout << "-t X| --thread\t\t\tDefine the numbers of threads. X is the number of threads [current " << num_threads << "]" << std::endl;
	std::cout << "-r X| --running\t\t\tDefine how the application run. X is 0-Sequentially 1-Parallel Max Degree 2-No dup. [current " << type_running << "]" << std::endl;
	std::cout << std::endl;
}

/**
 * @brief Auxiliary function to parse the contents of the command line arguments. 
 * @details This is a simple auxiliary function that will parse the args from the command line. You may see the entire specification at the terminal using the -help argument.
 */
void parseArgs(int argc, char** argv){
    if (argc <= 1){
        printf("You must have parameters, type -h to help!\n");
        exit(-1);
    }
	for(int i = 1; i < argc; ++i){
		std::string arg(argv[i]);
		std::regex regexp("(.*)(=)");	// Select all character before equals sign AND equals sign
		std::regex thd("(-thd=)(.*)");


		DEBUG std::cerr << "Received param: " << arg << '\n';
		if(arg == "-h" || arg == "--help"){
			usage(argv[0]);
			exit(0);
		}
/* 		else if(arg == "-s" || arg == "--seed"){
			seed = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed seed to: " << seed << '\n';
		} */
/* 		else if(std::regex_match(arg, thd)){
			std::string value = regex_replace(arg, regexp, "");
			num_threads = std::stoi(value);
			DEBUG std::cerr << "Changed number of threads to: " << num_threads << '\n';
		} */
		else if(arg == "-t" || arg == "--thread"){
			num_threads = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed number of threads to: " << num_threads << '\n';
		}
		else if(arg == "-r" || arg == "--running"){
			type_running = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed type running to: " << type_running << '\n';
		}
		else if(arg == "-s" || arg == "--show"){
			display = true;
			DEBUG std::cerr << "Changed type running to: " << type_running << '\n';
		}
		else if(arg == "-tri" || arg == "--triangle"){
			graph_type = true;
			DEBUG std::cerr << "Changed triangle to: " << graph_type << '\n';
		}
		else if(arg == "-notri" || arg == "--notriangle"){
			graph_type = false;
			DEBUG std::cerr << "Changed triangle to: " << graph_type << '\n';
		}
		else if(arg == "--degree"){
			paralel_type = "m";
			DEBUG std::cerr << "Changed paralelism to: " << paralel_type << '\n';
		}
		else if(arg == "--list"){
			paralel_type = "l";
			DEBUG std::cerr << "Changed paralelism to: " << paralel_type << '\n';
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
};


/// @brief  The main method
int main(int argc, char** argv){
    DEBUG std::cerr << "Calculate stretch index.\n";
	parseArgs(argc, argv); //Will setup globals

	DEBUG std::cerr << "Reading the graph\n";

	//TODO: Leitura do grafo
	Graph graph;
    graph = read_graph_file();
	DEBUG std::cerr << "Quantidade de vertices => " << graph.getQtdVertices() << std::endl;

	//DEBUG std::cerr << "Solving with paralellism - wait!\n";
	//TODO: Chamar a força bruta para o grafo de entrada

	//Parallelism acme; // Lonney Tunes rocks!
	
	time_t time_begin = 0;
    time_t time_end = 0;
    double tempo_total = 0;
	
	sem_init(&semaforo, 0, num_threads);
	time(&time_begin);

	if (type_running == 0){
		DEBUG std::cerr << "Create graph - wait!\n";
		create_new_graphs();
	} else if (type_running == 1){
		DEBUG std::cerr << "Solving with sequential brute force - wait!\n";
		create_threads(graph);
	} else if (type_running == 2){
		DEBUG std::cerr << "Solving with Maximum degree - PARALLEL- wait!\n";
		create_threads(graph);
	} else if (type_running == 3){
		DEBUG std::cerr << "Solving with induced cycle - PARALLEL- wait!\n";
		create_threads_big_cycle(graph);
	} else if (type_running == 4){
		DEBUG std::cerr << "Solving with heuristic 1 - wait!\n";
		Heuristic acme = Heuristic();	// Lonney Tunes rocks!
		acme.heuristica_1(graph);
	} 

	time(&time_end);

	//TODO: Imprimir no std::out a saida
	if (display){
		std::cout << "Outputing the solution" << std::endl;
		std::cout << "Stretch index calculated: " << graph.get_stretch_index() <<  std::endl;
		std::cout << "Total de árvores calculadas: " << graph.get_total_tree() <<  std::endl;
		std::cout << "Running time (in seconds): " << difftime(time_end, time_begin) <<  std::endl;	
		graph.show_best_tree();
	} else {
		DEBUG std::cerr << "Outputing the solution\n";
	    DEBUG std::cerr << "Stretch index calculated: " << graph.get_stretch_index() <<  std::endl;
		DEBUG std::cerr << "Total de árvores calculadas: " << graph.get_total_tree() <<  std::endl;
		DEBUG std::cerr << "Running time (in seconds): " << difftime(time_end, time_begin) <<  std::endl;
		int node1 = 0, node2 = 0;
    	for (auto&& tuple: graph.best_tree){
        	std::tie(node1, node2) = tuple;
        	std::cerr << "(" << node1 << " , " << node2 << ") ";
    	}
	}
    return 0;
};

