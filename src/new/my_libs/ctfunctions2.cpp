#include <iostream>
#include <sys/stat.h>
//#include <errno.h>
//#include <stdio.h>
#include <iostream>
#include <sstream>
#include<unistd.h>

#include <iterator>

#include <cstdlib>
#include <sys/types.h>

#include <string.h>
#include "../code/graph.hpp"
#include "../code/opBasic.hpp"
#include "../code/stretch.hpp"
#include "../code/frontier.hpp"
#include "../code/genGraph.hpp"

#include "../Debug.h"

#include "ctfunctions2.hpp"
//#include "externals.hpp"

bool graph_type=false;
std::string paralel_type="m";
int min_vertices = 0;
int max_vertices = 0;
int num_graph = 0;
float probability = 0.45;

/// Basic debugging controller. See Debug.h for details.
/* #ifdef MN_BF_SEQ_DEBUG
#define DEBUG
#else
#define DEBUG while (false)
#endif

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif */

/**
 * @details To swap to values at positions
 * @param *a an integers that represents a value.
 * @param *b an integers that represents a value.
 */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
 
/**
 * @details A bubblesort implementation
 * @param array an array with vertex and degree.
 * @param n an integers that represents a value.
 */
void bubbleSort(int **array, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++)
 
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++) {
            if (array[1][j] < array[1][j + 1]) {
                swap(&array[0][j], &array[0][j + 1]);
                swap(&array[1][j], &array[1][j + 1]);
            }
        }
            
                
}

bool in(std::vector <int> &vector1, const int &value) {
    bool result = false;
    for(auto vertex : vector1) {
        if (vertex == value) {
            result = true;
            break;
        }
    }
    return result;
}

/*
https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
Solution 1.3
*/
std::vector<std::string> split(const std::string& s, char delimiter) {
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter)) {
      tokens.push_back(token);
   }
   return tokens;
}

/*
Reference:
https://stackoverflow.com/questions/62697081/get-the-name-of-an-input-file-passed-in-by-redirection
*/
std::string get_filename() {
	// To get filename redirections
	char buf[512], file[512] = {0};

    snprintf(buf, sizeof buf, "/proc/self/fd/%d", fileno(stdin));
    readlink(buf, file, sizeof file - 1);

	/* 	DEBUG std::cerr << "Filename " << file;
		printf("File name: %s\n", file); */

	//std::string text = "Let me split this into words";

	std::string text = file;
	std::istringstream iss(text);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
									std::istream_iterator<std::string>());

    // At Windows and MacOS will needs another delimiter? like inverse / ?
    // I cannot test because I am using Linux
	std::vector<std::string> texto = split(text, '/');
	return texto[texto.size()-1];
}

/*
 * Funcoes que fazem a leitura/apoio do arquivo em redirections
 * Serão modificadas no futuro
*/

std::string get_enviroment_var(const char *enviroment_var)
{
    const char *tmp = getenv(enviroment_var);
    std::string env_var(tmp ? tmp : "");
    if (env_var.empty())
    {
        std::cerr << "[ERROR] No such variable DIR_TADM found!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return env_var;
};

std::string get_current_dir(){
    char* buffer;

    if( (buffer=getcwd(NULL, 0)) == NULL) {
        perror("failed to get current directory\n");
    } else {
        printf("%s \nLength: %zu\n", buffer, strlen(buffer));
        //free(buffer);
    }
    return buffer;
}

void create_directory(std::string auxiliary)
{
    if (mkdir(auxiliary.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == -1)
    {
        DEBUG std::cerr << "Error : " << strerror(errno) << std::endl;
    }
    else
    {
        DEBUG std::cerr << "Directory created!\n";
    }
}

/**
 * @details Read a graph file using redirections.
 * @param g a graph instance that represents a graph.
 * @return a integer the graph contains a cycle.
 */
Graph read_graph_file() {

    char Ch = ' ';
    std::string value_str = "";
    int dimension = 0;
    int row = 0;
    int col = 0;
    int LetterCount = 0;
    int WordCount = 0;
    Graph graph;

    while (std::cin.get(Ch)) {
        if ((Ch == '\n') || (Ch == ' ')) {
            LetterCount = 0;
            if (value_str.size()) {
                if (WordCount++ == 0) {
                    dimension = std::stoi(value_str);
                    graph.add_vertices(dimension);
                } else {
                    if (std::stoi(value_str)) {
                        if (col >= dimension || row >=dimension) {
                            printf("Ponto de parada");
                        }
                        bool exist = false;
                        for (int i=0; i < graph.adjList(row).size(); i++) {
                            if (row > col) {
                                if (std::stoi(value_str) == 1){ exist = true; }
                            }
                        }
                        if (!exist) { graph.add_aresta(row, col); }
                    }
                    col++;
                    if (col >= dimension) {
                        col = 0;
                        row++;
                        if (row >= dimension){ row = 0; }
                    }
                }
                value_str = "";
            }
        } else {
            value_str += Ch;
            ++LetterCount;
        }
    }
    return graph;
}

int create_new_graphs(){
    DEBUG std::cerr << "Creating new graphs.\n";

	std::vector<Graph> list;
    Frontier f;
    srand (time(NULL));
	
	std::string filename;
	std::string dirname;
	std::string auxiliary;
	//std::string DIR_BASE = get_enviroment_var("DIR_TADM");
	std::string DIR_BASE = get_current_dir_name();
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
}