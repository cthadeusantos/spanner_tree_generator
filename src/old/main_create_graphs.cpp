/**
 * @file main_BF-SEQ.cpp
 * @author Carlos Thadeu ()
 * @brief Application to solve the T-admissibility problem with sequential brute force.
 */

#include <iostream>
#include <regex>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>

#include "Debug.h"

#include "../my_libs/ctfunctions2.hpp"


#include "code/frontier.hpp"
#include "code/genGraph.hpp"
#include "code/graph.hpp"
#include "code/heuristic.hpp"
#include "code/opBasic.hpp"

///Basic debugging controller. See Debug.h for details.
// #ifdef MN_BF_SEQ_DEBUG
// 	#define DEBUG
// #else
// 	#define DEBUG while(false)
// #endif

/**
 * @addtogroup bfseqparam
 * Available globally on the main_BF-SEQ.cpp file scope.
 * @{
 */
///The seed only exists because of external tools. The algorithm itself is deterministic.
/* int seed = 0;
bool graph_type=false;
std::string paralel_type="m";
int min_vertices = 0;
int max_vertices = 0;
int num_graph = 0;
float probability = 0.45;
int num_threads = 1; */

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
	for(int i = 1; i < argc; ++i){
		std::string arg(argv[i]);

		std::regex regexp("(.*)(=)");	// Select all character before equals sign AND equals sign
		std::regex min("(-min=)(.*)");
		std::regex max("(-max=)(.*)");
		std::regex qty("(-qty=)(.*)");
		std::regex prob("(-prob=)(.*)");
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
		else if(arg == "-tri" || arg == "--triangle"){
			//seed = std::atoi(argv[++i]);
			graph_type = true;
			DEBUG std::cerr << "Changed triangle to: " << graph_type << '\n';
		}
		else if(arg == "-notri" || arg == "--notriangle"){
			graph_type = false;
			DEBUG std::cerr << "Changed triangle to: " << graph_type << '\n';
		}
		else if(arg == "-md" || arg == "--degree"){
			paralel_type = "m";
			DEBUG std::cerr << "Changed paralelism to: " << paralel_type << '\n';
		}
		else if(arg == "-ld" || arg == "--list"){
			paralel_type = "l";
			DEBUG std::cerr << "Changed paralelism to: " << paralel_type << '\n';
		}
		else if(std::regex_match(arg, min)){
			std::string value = regex_replace(arg, regexp, "");
			min_vertices = std::stoi(value);
			DEBUG std::cerr << "Changed minimum to: " << min_vertices << '\n';
		}
		else if(std::regex_match(arg, max)){
			std::string value = regex_replace(arg, regexp, "");
			max_vertices = std::stoi(value);
			DEBUG std::cerr << "Changed maximum to: " << max_vertices << '\n';
		}
		else if(std::regex_match(arg, qty)){
			std::string value = regex_replace(arg, regexp, "");
			num_graph = std::stoi(value);
			DEBUG std::cerr << "Changed number of graphs to: " << num_graph << '\n';
		}
		else if(std::regex_match(arg, prob)){
			std::string value = regex_replace(arg, regexp, "");
			probability = std::stof(value);
			DEBUG std::cerr << "Changed probability to: " << probability << '\n';
		}
		else if(std::regex_match(arg, thd)){
			std::string value = regex_replace(arg, regexp, "");
			num_threads = std::stoi(value);
			DEBUG std::cerr << "Changed number of threads to: " << num_threads << '\n';
		}
		else {
			DEBUG std::cerr << "Unknown param: " << arg << "\n";
			exit(1);
		};
	};
};

/// @brief  The main method
int main(int argc, char** argv){
    DEBUG std::cerr << "Creating new graphs.\n";
	parseArgs(argc, argv); //Will setup globals

	std::vector<Graph> list;
    Frontier f;
    srand (time(NULL));
	
	std::string filename;
	std::string dirname;
	std::string auxiliary;
	//std::string DIR_BASE = get_enviroment_var("DIR_TADM");
	std::string DIR_BASE = get_current_dir();
	//std::string DIR_BASE = argv[0];
	std::string DIR_INSTANCES = DIR_BASE + "instances/";
	std::string DIR_RESULTS = DIR_BASE + "results/"; 

	dirname = "paralelDegree_";
	if (paralel_type == "m") {
		dirname = "maxDegree_";
	} else if (paralel_type == "l") {
		dirname = "conectList_";
	}
	auxiliary = "noTri_";
	if (graph_type){
		auxiliary = "grafos_";
	} 
	dirname +=  auxiliary + std::to_string(min_vertices) + "_"
						+ std::to_string(max_vertices) + "_"
						+ std::to_string(num_graph) + "_"
						+ std::to_string((int)(probability*100))
						+ "/";

	auxiliary = DIR_INSTANCES + dirname;
	create_directory(auxiliary);
	auxiliary += "grafos/";
	create_directory(auxiliary);

    if(!graph_type ){
        GenGraph::generate_list_graph_without_triangle(list, min_vertices, max_vertices, num_graph, probability); // gera os grafos sem triangulo
    } else {
        GenGraph::generate_list_graph(list, min_vertices, max_vertices, num_graph, probability); // gera os grafos aleatorios
    }

	DEBUG std::cerr << "Save generated graphs!\n";
    // escreve cada grafo em seu proprio arquivo
    std::ofstream graphOut;
    int cont = 0;
    for( Graph g : list)
    {
		graphOut.open(auxiliary + "grafo" + std::to_string(cont+1) + ".txt");
        if( graphOut.is_open() ){
            f.printAdjMat_2(g, "", graphOut);
            ++cont;
        } else {
            std::cerr << "ERROR ao abrir o arquivo: " << "grafo " + std::to_string(cont+1) << std::endl;
            return -1;
        }
        graphOut.close();
    }
    return 0;
};