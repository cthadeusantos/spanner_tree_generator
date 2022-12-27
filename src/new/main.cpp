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

//#include "my_libs/ctfunctions.cpp"

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
	std::cout << "Usage: " << app_name << " <option(s)> " << "< INPUT_FILENAME >>> OUTPUT_FILENAME" << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\t-h | --help \t\tShow this message." << std::endl;
	std::cout << "\t-t X | --thread X\tDefine the numbers of threads. X is the number of threads [current " << num_threads << "]" << std::endl  << std::endl ;
	
	std::cout << "Show info:" << std::endl;
	std::cout << "\t-s | --summary \t\tAt file. [current " << output << "]" << std::endl;
	std::cout << "\t-e | --expo \t\tAt screen. [current " << output << "]" << std::endl;
	std::cout << "\t-d | --debug \t\tAt screen only debug mode. [current " << output << "]" << std::endl;
	std::cout << "\t-b | --best \t\tShow the best tree found." << std::endl;
	std::cout << "You can combine summary, expo, debug and show" << std::endl << std::endl;

	std::cout << "Define which type application will be run. [current " << type_running << "]" << std::endl;
	std::cout << "\t-r X | --running X" << std::endl;
	std::cout << "\t\t\t\t 0  - Create trees" << std::endl;
	std::cout << "\t\t\t\t 1  - Brute Force (sequential)" << std::endl;
	std::cout << "\t\t\t\t 2  - Brute Force (parallel) limited by threads" << std::endl;
	std::cout << "\t\t\t\t 3  - Max Degree (parallel)" << std::endl;
	std::cout << "\t\t\t\t 4  - Induced cycle (parallel)" << std::endl;
	std::cout << "\t\t\t\t 5  - Heuristic 1" << std::endl;
	std::cout << "\t\t\t\t 6  - Heuristic 1 vertex importance" << std::endl;
	std::cout << "\t\t\t\t 7  - Heuristic 2" << std::endl;
	std::cout << "\t\t\t\t 8  - Heuristic 2 vertex importance" << std::endl;
	std::cout << "\t\t\t\t 9  - Breadth heuristic" << std::endl << std::endl;
	std::cout << "\t\t\t\t 10 - Articulations (parallel)" << std::endl << std::endl;

	std::cout << "If running is 0 (create tree) - parameters must be necessary" << std::endl;
	std::cout << "\t\t\t\t -tri | --triangle\tCreate with triangles" << std::endl;
	std::cout << "\t\t\t\t -notri | --notriangle\tCreate without triangles" << std::endl;
	std::cout << "\t\t\t\t -min X\t\t\tMinimum number of vertices in the graph" << std::endl;
	std::cout << "\t\t\t\t -max X\t\t\tMaximum number of vertices in the graph" << std::endl;
	std::cout << "\t\t\t\t -qty X\t\t\tNumber of graphs will be create" << std::endl;
	std::cout << "\t\t\t\t -prob X\t\tProbability of vertex has edge (Between 0.45 and 1.00)" << std::endl;
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
		else if(arg == "-t" || arg == "--thread"){
			num_threads = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed number of threads to: " << num_threads << '\n';
		}
		else if(arg == "-r" || arg == "--running"){
			type_running = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed type running to: " << type_running << '\n';
		}
		else if(arg == "-e" || arg == "--expo"){
			output = output + 1;
			DEBUG std::cerr << "Changed output type to: " << type_running << '\n';
		}
		else if(arg == "-s" || arg == "--summary"){
			output = output + 2;
			DEBUG std::cerr << "Changed output type to: " << type_running << '\n';
		}
		else if(arg == "-d" || arg == "--debug"){
			output = output + 64;
			DEBUG std::cerr << "Changed output type to: " << type_running << '\n';
		}
		else if(arg == "-tri" || arg == "--triangle"){
			graph_type = true;
			DEBUG std::cerr << "Changed triangle to: " << graph_type << '\n';
		}
		else if(arg == "-notri" || arg == "--notriangle"){
			graph_type = false;
			DEBUG std::cerr << "Changed triangle to: " << graph_type << '\n';
		}
/* 		else if(arg == "--degree"){
			paralel_type = "m";
			DEBUG std::cerr << "Changed paralelism to: " << paralel_type << '\n';
		}
		else if(arg == "--list"){
			paralel_type = "l";
			DEBUG std::cerr << "Changed paralelism to: " << paralel_type << '\n'; 
		} */
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

	if(argc < 2){
		usage("--help");
		exit(0);
	}

	std::string filename = get_filename();
	std::string run_name  = "";

	std::cout << filename << std::endl;

	DEBUG std::cerr << "Calculate stretch index.\n";
	parseArgs(argc, argv); //Will setup globals

	DEBUG std::cerr << "Reading the graph\n";

	//TODO: Leitura do grafo
	Graph graph;
    graph = read_graph_file();
	DEBUG std::cerr << "Quantidade de vertices => " << graph.getQtdVertices() << std::endl;

	time_t time_begin = 0;
    time_t time_end = 0;
    double tempo_total = 0;

	OpBasic op; 
    int lower_limit = op.maxLowerCicle(graph) - 1;

	sem_init(&semaforo, 0, num_threads);
	time(&time_begin);

	if (type_running == 0){
		DEBUG std::cerr << "Create graph - wait!\n";
		create_new_graphs();
	} else if (type_running == 1){
		DEBUG std::cerr << "Solving with sequential brute force - wait!\n";
		run_name = "Brute force - Sequential";
		Stretch().sequential(graph);
	} else if (type_running == 2){
		DEBUG std::cerr << "Solving with parallel brute force limited by threads - wait!\n";
		run_name = "Maximum degree - parallel";
		create_threads_edge_max_degree(graph);
	} else if (type_running == 3){
		DEBUG std::cerr << "Solving brute force with Maximum degree - PARALLEL- wait!\n";
		run_name = "Maximum degree - parallel";
		create_threads(graph);
	} else if (type_running == 4){
		DEBUG std::cerr << "Solving with induced cycle - PARALLEL- wait!\n";
		run_name = "Induced cycle";
		create_threads_big_cycle(graph);
	} else if (type_running == 5){
		DEBUG std::cerr << "Solving with heuristic 1 - wait!\n";
		run_name = "Heuristic 1v1";
		Heuristic::heuristica_1(graph);
	} else if (type_running == 6){
		DEBUG std::cerr << "Solving with heuristic 1 vertex importance- wait!\n";
		run_name = "Heuristic 1v2";
		Heuristic::heuristica_1_modified(graph);
	} else if (type_running == 7){
		DEBUG std::cerr << "Solving with heuristic 2 - wait!\n";
		run_name = "Heuristic 2v1";
		Heuristic::heuristica_2(graph);
	} else if (type_running == 8){
		DEBUG std::cerr << "Solving with heuristic 2 vertex importance- wait!\n";
		run_name = "Heuristic 2v2";
		Heuristic::heuristica_2_modified(graph);
	} else if (type_running == 9){
		DEBUG std::cerr << "Solving with breadth heuristic - wait!\n";
		run_name = "Breadth heuristic";
		Heuristic::breadth_heuristic(graph);
	} else if (type_running == 10){
		DEBUG std::cerr << "Solving with articulations - wait!\n";
		run_name = "Articulations method";
		create_threads_articulations(graph);
	}
	time(&time_end);

	// OUTPUT - nothing - screen - file - debug
	if ((output & 1)==1){
		std::cout << "Outputing the solution for " << run_name << std::endl;
		std::cout << "Input filename: " << filename << std::endl;
		std::cout << "Limite inferior: " << lower_limit << std::endl;
		std::cout << "Stretch index calculated: " << graph.get_stretch_index() <<  std::endl;
		std::cout << "Total de árvores calculadas: " << graph.get_total_tree() <<  std::endl;
		std::cout << "Running time (in seconds): " << difftime(time_end, time_begin) <<  std::endl;
		if (best) graph.show_best_tree();
	}
	if ((output & 2)==2){
		std::cout << filename << run_name << " " << lower_limit << graph.get_stretch_index() << " " << graph.get_total_tree() << " " << difftime(time_end, time_begin) << std::endl;
		if (best) graph.show_best_tree();
	}
	if ((output & 64)==64){
		DEBUG std::cerr << "Outputing the solution for " << run_name << std::endl;
		DEBUG std::cerr << "Input filename: " << filename << std::endl;
		DEBUG std::cerr << "Limite inferior: " << lower_limit << std::endl;
	    DEBUG std::cerr << "Stretch index calculated: " << graph.get_stretch_index() <<  std::endl;
		DEBUG std::cerr << "Total de árvores calculadas: " << graph.get_total_tree() <<  std::endl;
		DEBUG std::cerr << "Running time (in seconds): " << difftime(time_end, time_begin) <<  std::endl;
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

