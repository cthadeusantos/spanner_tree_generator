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
void Parameters::usage(const char* app_name){
	std::cout << "Usage: " << app_name << " [OPTIONS] " << "< INPUT_FILENAME >> OUTPUT_FILENAME" << std::endl;
	std::cout << std::endl << "OPTIONS: " << std::endl;
	std::cout << "\t-h | --help \t\tShow this message." << std::endl;
	std::cout << "\t-t X | --thread X\tDefine the numbers of threads. X is the number of threads [current " << num_threads << "]" << std::endl  << std::endl ;
	std::cout << "\t-rt X| --running_time X\tDefine execution time in miliseconds until STOP! default is 0 [current " << running_time << "]" << std::endl  << std::endl ;
	std::cout << "\t     | --adjacency \t\tDefine which type file will be read. (adjacency list)[default]" << std::endl  << std::endl;
	std::cout << "\t-v	 | --version\tWhich version is this app." << std::endl  << std::endl ;
	std::cout << "\t     | --edges   \t\tDefine which type file will be read. (edges list)" << std::endl  << std::endl ;
	std::cout << "\t     | --nolb   \t\tNot calculate lower bound (only heuristics)" << std::endl  << std::endl ;
	std::cout << "\t     | --noindex\t\tNot calculate stretch index" << std::endl  << std::endl ;

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
void Parameters::parseArgs(int argc, char** argv){
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
		else if(arg == "-v" || arg == "--version"){
			std::cout << "T-Admissibility version " << Version().version() << '\n';
			exit(0);
		}
		else if(arg == "-rt" || arg == "--running_time"){
			running_time = std::atoi(argv[++i]);
			DEBUG std::cerr << "Changed running time to: " << running_time << '\n';
		}
		else if(arg == "--adjacency"){
			matrix_t = 0;
			DEBUG std::cerr << "Changed read file type to: " << matrix_t << '\n';
		}
		else if(arg == "--edges"){
			matrix_t = 1;
			DEBUG std::cerr << "Changed read file type to: " << matrix_t << '\n';
		}
		else if(arg == "-s" || arg == "--screen"){
			output = output + 1;
			DEBUG std::cerr << "Changed output type to: " << output << '\n';
		}
		else if(arg == "-f" || arg == "--file"){
			output = output + 2;
			DEBUG std::cerr << "Changed output type to: " << output << '\n';
		}
		else if(arg == "-d" || arg == "--debug"){
			output = output + 64;
			DEBUG std::cerr << "Changed output type to: " << output << '\n';
		}
		else if(arg == "-b" || arg == "--best"){
			best = true;
			DEBUG std::cerr << "Changed best tree visualization to: " << best << '\n';
		}
		else if(arg == "--nolb"){
			nolb = true;
			DEBUG std::cerr << "Disable lower bound compute." << nolb << '\n';
		}
		else if(arg == "--noindex"){
			noindex = true;
			DEBUG std::cerr << "Disable stretch index compute." << nolb << '\n';
		}
		else {
			DEBUG std::cerr << "Unknown param: " << arg << "\n";
			exit(1);
		};
	};
};