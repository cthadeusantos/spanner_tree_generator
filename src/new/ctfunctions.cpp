#include <iostream>
#include <sys/stat.h>
//#include <errno.h>
//#include <stdio.h>
#include <string.h>
#include "code/graph.hpp"

#include "Debug.h"

///Basic debugging controller. See Debug.h for details.
#ifdef MN_BF_SEQ_DEBUG
	#define DEBUG
#else
	#define DEBUG while(false)
#endif

#ifdef WINDOWS
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
	#include <unistd.h>
	#define GetCurrentDir getcwd
#endif


/* std::string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir(buff, FILENAME_MAX );
   std::string current_working_dir(buff);
   return current_working_dir;
}; */

std::string get_enviroment_var(const char *enviroment_var) {
	const char *tmp = getenv(enviroment_var);
	std::string env_var(tmp ? tmp : "");
	if (env_var.empty()) {
		std::cerr << "[ERROR] No such variable DIR_TADM found!" << std::endl;
		exit(EXIT_FAILURE);
	}
	return env_var;
};

void create_directory(std::string auxiliary){
	if (mkdir(auxiliary.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == -1){
		std::cerr << "Error : " << strerror(errno) << std::endl;
	} else {
		DEBUG std::cerr << "Directory created!\n";
	}
}

Graph read_graph_file(){
    
	char Ch = ' ';
    std::string value_str = "";
    int dimension = 0;
    int row = 0;
    int col = 0;
    int LetterCount = 0;
    int WordCount = 0;
    Graph graph;

    while (std::cin.get(Ch))
    {
        if ((Ch == '\n') || (Ch == ' '))
        {
            LetterCount = 0;
            if (value_str.size()){
                if (WordCount++ == 0){
                    dimension = std::stoi(value_str);
                    graph.create(dimension);
                } else {
                    if (std::stoi(value_str)){
                        graph.add_aresta(row, col);
                    }
                    if (col++ >= dimension){
                        col = 0;
                        if (row++ >= dimension){
                            row = dimension - 1;
                        }
                    }
                }
                value_str = "";
            }
        }
        else {
            value_str += Ch;
            ++LetterCount;
        }
    }
    return graph;
}
