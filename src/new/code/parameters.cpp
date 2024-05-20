#include <iostream>
/* #ifdef MN_BF_SEQ_DEBUG
	#define DEBUG
#else
	#define DEBUG while(false)
#endif */
#include "../Debug.h"
#include "version.hpp"
#include "parameters.hpp"

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
void Parameters::usage(const char *app_name){
	std::cout << "T-Admissibility version " << Version().version() << std::endl << std::endl;
	std::cout << "Usage: " << app_name << " [OPTIONS] " << "< INPUT_FILENAME >> OUTPUT_FILENAME" << std::endl << std::endl;
	std::cout << std::endl << "OPTIONS: " << std::endl;
	std::cout << "\t-h	| --help	Show this message." << std::endl;
	std::cout << "\t-t X	| --thread X	Define the numbers of threads. X is the number of threads [current " << num_threads << "]" << std::endl  << std::endl ;
	std::cout << "\t-rt X	| --running_time X	Define execution time in miliseconds until STOP! default is 0 [current " << global_running_time << "]" << std::endl  << std::endl ;
	std::cout << "\t-ci X	| --induced_cycle X	Define execution time in miliseconds until STOP! default is 0 [current " << global_induced_cycle << "]" << std::endl  << std::endl ;
	std::cout << "\t     	| --adjacency	Define which type file will be read. (adjacency matrix)[DEFAULT]" << std::endl  << std::endl;
	std::cout << "\t     	| --edges	Define which type file will be read. (edges list)" << std::endl  << std::endl ;
	std::cout << "\t-v	| --version	Which version is this app." << std::endl  << std::endl ;
	std::cout << "\t     	| --nolb	Not calculate lower bound" << std::endl  << std::endl ;
	std::cout << "\t     	| --noindex	Not calculate stretch index" << std::endl  << std::endl ;
	std::cout << "\t     	| --alg 	<< TOO SOON! >> Calculate closeness using algebraic method, only heuristics. [DEFAULT]" << std::endl  << std::endl ;
	std::cout << "\t     	| --tra 	<< TOO SOON! >> Calculate closeness using traverse method, only heuristics." << std::endl  << std::endl ;

	std::cout << "Options to display information (you MUST choose file, screen, or debug):" << std::endl;
	std::cout << "\t-f 	| --file \t\tOutput at file. [not work yet]" << std::endl;
	std::cout << "\t-s 	| --screen \t\tOutput At screen." << std::endl;
	std::cout << "\t-d 	| --debug \t\tOutput At screen only debug mode." << std::endl << std::endl;
	std::cout << "\t-b 	| --best \t\tShow the best tree found if you need. Try to use only trees with few edges!" << std::endl;
	std::cout << "You can combine screen, file, debug and best" << std::endl << std::endl;
}

/**
 * @brief Auxiliary function to parse the contents of the command line arguments.
 * @authors { Anderson Zudio[AZ] /  Carlos Thadeu [CT] (contributor)}
 * @details This is a simple auxiliary function that will parse the args from the command line. You may see the entire specification at the terminal using the -help argument.
 */
void Parameters::parseArgs(int argc, char** argv){
    if (argc < 1){
		std::cout << "You must have parameters, type -h to help!" << std::endl;
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
		else if(arg == "-v" || arg == "--version"){
			std::cout << "T-Admissibility version " << Version().version() << '\n';
			exit(0);
		}
		else if(arg == "-rt" || arg == "--running_time"){
			global_running_time = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed running time to: " << global_running_time << '\n';
		}
		else if(arg == "-ci" || arg == "--induced_cycle"){
			global_induced_cycle = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed number of induced cycle to: " << global_induced_cycle << '\n';
		}
		else if(arg == "-a" || arg == "--adjacency"){
			matrix_t = 0;
			DEBUG std::cerr << "Changed read file type to: " << matrix_t << '\n';
		}
		else if(arg == "-e" || arg == "--edges"){
			matrix_t = 1;
			DEBUG std::cerr << "Changed read file type to: " << matrix_t << '\n';
		}
		else if(arg == "-s" || arg == "--screen"){
			//global_output = global_output + 1;
			global_output = 1;
			DEBUG std::cerr << "Changed global_output type to: " << global_output << '\n';
		}
		else if(arg == "-f" || arg == "--file"){
			//global_output = global_output + 2;
			global_output = 2;
			DEBUG std::cerr << "Changed output type to: " << global_output << '\n';
		}
		else if(arg == "-d" || arg == "--debug"){
			//global_output = global_output + 64;
			global_output = 64;
			DEBUG std::cerr << "Changed output type to: " << global_output << '\n';
		}
		else if(arg == "-b" || arg == "--best"){
			best = true;
			DEBUG std::cerr << "Changed best tree visualization to: " << best << '\n';
		}
		else if(arg == "--nolb"){
			global_nolb = true;
			DEBUG std::cerr << "Disable compute the lower bound" << global_nolb << '\n';
		}
		else if(arg == "-st"){
			global_save_tree = true;
			DEBUG std::cerr << "Save trees on file (alpha version - only purposes tests - will be removed at future)" << global_save_tree << '\n';
		}
		else if(arg == "--noindex"){
			global_noindex = true;
			DEBUG std::cerr << "Disable stretch index compute." << global_noindex << '\n';
		}
		else if (arg == "--alg"){
			global_closeness = 1;
			DEBUG std::cerr << "Setting closeness to algebraic." << '\n';
		}
		else if (arg == "--tra"){
			global_closeness = 2;
			DEBUG std::cerr << "Setting closeness to transversal." << '\n';
		}	 
		else {
			std::cout << "Unknown parameter: " << arg << std::endl;
			std::cout << "Type --help for more informations!" << arg << std::endl;
			exit(1);
		};
	};
};