/**
 * @file main_BF-PAR.cpp
 * @author Carlos Thadeu ()
 * @brief Application to solve the T-admissibility problem with parallelism of threads.
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
#include "code/parallel_functions.hpp"

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
	std::cout << "-s | --seed\t\t\tSetup this application seed. [current " << seed << "]" << std::endl;
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
		else if(arg == "-s" || arg == "--seed"){
			seed = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed seed to: " << seed << '\n';
		}
/* 		else if(std::regex_match(arg, thd)){
			std::string value = regex_replace(arg, regexp, "");
			num_threads = std::stoi(value);
			DEBUG std::cerr << "Changed number of threads to: " << num_threads << '\n';
		} */
		else if(arg == "-t" || arg == "--thread"){
			num_threads = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed number of threads to: " << num_threads << '\n';
		}
		else if(arg == "-d" || arg == "--max_degree"){
			type_running = 0;
			DEBUG std::cerr << "Changed type running to: " << type_running << '\n';
		}
		else if(arg == "-z" || arg == "--no_dup"){
			type_running = 1;
			DEBUG std::cerr << "Changed type running to: " << type_running << '\n';
		}
		else {
			DEBUG std::cerr << "Unknown param: " << arg << "\n";
			exit(1);
		};
	};
};


/// @brief  The main method
int main(int argc, char** argv){
    DEBUG std::cerr << "Calculate stretch index using paralellism.\n";
	parseArgs(argc, argv); //Will setup globals

	DEBUG std::cerr << "Reading the graph\n";
	//TODO: Leitura do grafo
	Graph graph;
    graph = read_graph_file();
//	std::cout << "Quantidade de vertices => " << graph.getQtdVertices() << std::endl;
	DEBUG std::cerr << "Quantidade de vertices => " << graph.getQtdVertices() << std::endl;

	DEBUG std::cerr << "Solving with paralellism - wait!\n";
	//TODO: Chamar a força bruta para o grafo de entrada

	//Parallelism acme; // Lonney Tunes rocks!
	
	time_t time_begin = 0;
    time_t time_end = 0;
    double tempo_total = 0;
	
	sem_init(&semaforo, 0, num_threads);

	time(&time_begin);
	//acme.create_threads(graph);
	if (type_running == 0){
		DEBUG std::cerr << "Maximum degree\n";
		create_threads(graph);
	} else if (type_running == 1){
		DEBUG std::cerr << "Maximum degree without duplications trees\n";
		create_threads_without_trees_duplications(graph);
	}
	time(&time_end);

    DEBUG std::cerr << "Stretch index calculated: " << graph.get_stretch_index() << "\n";
	DEBUG std::cerr << "Running time (in seconds): " << difftime(time_end, time_begin) << "\n";

	graph.show_best_tree();

	std::vector<int> x;
	x = graph.induced_cycle(graph, 5);
	OpBasic acme;
	x = acme.biggestCycle(graph);
	//bool y;
	//y = graph.has_chord(graph, x);

	DEBUG std::cerr << "Outputing the solution\n";
	//TODO: Imprimir no std::out a saida
	std::cout << "Resposta exemplo" << std::endl;

    return 0;
};

