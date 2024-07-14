#include "parameters.hpp"

Parameters::Parameters()
    : global_num_threads(1),
      global_threads_supported(0),
      global_used_threads(0),
      global_output(0),
      global_matrix_t(0),
      global_best(false),
      global_nolb(false),
      global_noindex(false),
      global_induced_cycle(0),
      global_induced_cycle_used(0),
      global_save_tree(false),
      global_seed(0),
      global_total_arv(0), // Probably deprecated
      global_running_time(0.0f),
      global_abort_for_timeout(false),
      global_closeness(0) {}

void Parameters::usage(const std::string &app_name) const {
    std::cout << "T-Admissibility version " << Version().version() << std::endl << std::endl;
    std::cout << "Usage: " << app_name << " [OPTIONS] " << "< INPUT_FILENAME >> OUTPUT_FILENAME" << std::endl << std::endl;
    std::cout << std::endl << "OPTIONS: " << std::endl;
    std::cout << "\t-h\t| --help\tShow this message." << std::endl;
    std::cout << "\t-t X\t| --thread X\tDefine the numbers of threads. X is the number of threads [current " << this->global_num_threads << "]" << std::endl << std::endl;
    std::cout << "\t-rt X\t| --running_time X\tDefine execution time in milliseconds until STOP! default is 0 [current " << this->global_running_time << "]" << std::endl << std::endl;
    std::cout << "\t-ci X\t| --induced_cycle X\tDefine execution time in milliseconds until STOP! default is 0 [current " << this->global_induced_cycle << "]" << std::endl << std::endl;
    std::cout << "\t\t| --adjacency\tDefine which type file will be read. (adjacency matrix)[DEFAULT]" << std::endl << std::endl;
    std::cout << "\t\t| --edges\tDefine which type file will be read. (edges list)" << std::endl << std::endl;
    std::cout << "\t-v\t| --version\tWhich version is this app." << std::endl << std::endl;
    std::cout << "\t\t| --nolb\tNot calculate lower bound" << std::endl << std::endl;
    std::cout << "\t\t| --noindex\tNot calculate stretch index" << std::endl << std::endl;
    std::cout << "\t\t| --alg\t<< TOO SOON! >> Calculate closeness using algebraic method, only heuristics. [DEFAULT]" << std::endl << std::endl;
    std::cout << "\t\t| --tra\t<< TOO SOON! >> Calculate closeness using traverse method, only heuristics." << std::endl << std::endl;

    std::cout << "Options to display information (you MUST choose file, screen, or debug):" << std::endl;
    std::cout << "\t-f\t| --file \t\tOutput at file. [not work yet]" << std::endl;
    std::cout << "\t-s\t| --screen \t\tOutput At screen." << std::endl;
    std::cout << "\t-d\t| --debug \t\tOutput At screen only debug mode." << std::endl << std::endl;
    std::cout << "\t-b\t| --best \t\tShow the best tree found if you need. Try to use only trees with few edges!" << std::endl;
    std::cout << "You can combine screen, file, debug and best" << std::endl << std::endl;
}

void Parameters::parseArgs(int argc, char** argv) {
    if (argc < 1){
        std::cout << "You must have parameters, type -h to help!" << std::endl;
        exit(-1);
    }
    for(int i = 1; i < argc; ++i){
        std::string arg(argv[i]);

        if(arg == "-h" || arg == "--help"){
            usage(argv[0]);
            exit(0);
        }        
        else if(arg == "-t" || arg == "--thread"){
            global_num_threads = std::stoi(argv[++i]);
        }
        else if(arg == "-v" || arg == "--version"){
            std::cout << "T-Admissibility version " << Version().version() << '\n';
            exit(0);
        }
        else if(arg == "-rt" || arg == "--running_time"){
            global_running_time = std::stof(argv[++i]);
        }
        else if(arg == "-ci" || arg == "--induced_cycle"){
            global_induced_cycle = std::stoi(argv[++i]);
        }
        else if(arg == "-a" || arg == "--adjacency"){
            global_matrix_t = 0;
        }
        else if(arg == "-e" || arg == "--edges"){
            global_matrix_t = 1;
        }
        else if(arg == "-s" || arg == "--screen"){
            global_output = 1;
        }
        else if(arg == "-f" || arg == "--file"){
            global_output = 2;
        }
        else if(arg == "-d" || arg == "--debug"){
            global_output = 64;
        }
        else if(arg == "-b" || arg == "--best"){
            global_best = true;
        }
        else if(arg == "--nolb"){
            global_nolb = true;
        }
        else if(arg == "-st"){
            global_save_tree = true;
        }
        else if(arg == "--noindex"){
            global_noindex = true;
        }
        else if (arg == "--alg"){
            global_closeness = 1;
        }
        else if (arg == "--tra"){
            global_closeness = 2;
        }     
        else {
            std::cout << "Unknown parameter: " << arg << std::endl;
            std::cout << "Type --help for more informations!" << arg << std::endl;
            exit(1);
        }
    }
}

// int Parameters::get_threads_supported() {
//     // Retorna um valor padrão se o optional estiver vazio
//     return global_threads_supported.value_or(std::thread::hardware_concurrency());
// }