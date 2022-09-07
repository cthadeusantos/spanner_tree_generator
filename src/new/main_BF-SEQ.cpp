/**
 * @file main_BF-SEQ.cpp
 * @author Carlos Thadeu ()
 * @brief Application to solve the T-admissibility problem with sequential brute force.
 */

#include <iostream>

#include "Debug.h"

///Basic debugging controller. See Debug.h for details.
#ifdef MN_BF_SEQ_DEBUG
#define DEBUG
#else
#define DEBUG while(false)
#endif

/**
 * @addtogroup bfseqparam
 * Available globally on the main_BF-SEQ.cpp file scope.
 * @{
 */
///The seed only exists because of external tools. The algorithm itself is deterministic.
int seed = 0;
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
		DEBUG std::cerr << "Received param: " << arg << '\n';
		if(arg == "-h" || arg == "--help"){
			usage(argv[0]);
			exit(0);
		}
		else if(arg == "-s" || arg == "--seed"){
			seed = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed seed to: " << seed << '\n';
		}
		else{
			DEBUG std::cerr << "Unknown param: " << arg << "\n";
			exit(1);
		}
	}
};

/// @brief  The main method
int main(int argc, char** argv){
    DEBUG std::cerr << "Solving with sequential brute force.\n";
	parseArgs(argc, argv); //Will setup globals

	DEBUG std::cerr << "Reading the graph\n";
	//TODO: Leitura do grafo

	DEBUG std::cerr << "Solving with brute force\n";
	//TODO: Chamar a força bruta para o grafo de entrada

	DEBUG std::cerr << "Outputing the solution\n";
	//TODO: Imprimir no std::out a saida
	std::cout << "Resposta exemplo" << std::endl;

    return 0;
}