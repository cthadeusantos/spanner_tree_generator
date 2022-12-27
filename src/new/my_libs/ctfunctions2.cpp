#include <iostream>
#include <sys/stat.h>
//#include <errno.h>
//#include <stdio.h>
#include <iostream>
#include <sstream>
#include<unistd.h>
#include <math.h>
#include <iterator>

#include <iterator>
#include <set>

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
void swap0(int *a, int *b) {
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
                swap0(&array[0][j], &array[0][j + 1]);
                swap0(&array[1][j], &array[1][j + 1]);
            }
        }
            
                
}

/**
 * @brief Return the index of element at a vector
 * @details Return an index from an element at vector if that element exists
 * @author Carlos Thadeu
 * @param key a seek value
 * @param vector a vector that contains integers
 * @return a boolean
 */
int get_index(int key, std::vector<int> vector){
    std::vector<int>::iterator itr = std::find(vector.begin(), vector.end(), key);
    return itr - vector.begin();
}

/**
 * @details Is the value belongs the vector?
 * @param vector1 a vector that contains integers
 * @param value a seek value
 * @return a boolean
 */
bool in(std::vector <int> &vector, const int &value) {
    bool result = false;
    for(auto vertex : vector) {
        if (vertex == value) {
            result = true;
            break;
        }
    }
    return result;
}

/**
 * @details Is the value belongs the vector?
 * @param vector1 a vector that contains integers
 * @param value a seek value
 * @return a boolean
 */
bool in(const std::vector <int> &vector, const int &value) {
    bool result = false;
    for(auto vertex : vector) {
        if (vertex == value) {
            result = true;
            break;
        }
    }
    return result;
}

/**
 * @brief 
 * @details Is the value belongs the set?
 * @param set1 a set that contains integers
 * @param value a seek value
 * @return a boolean
 */
bool in(std::set <int> &set, const int &value) {
    bool result = false;
    for(auto vertex : set) {
        if (vertex == value) {
            result = true;
            break;
        }
    }
    return result;
}

/**
 * @brief Return an index from an elemento at a vector if that element exists
 * @details Return an index from an element at vector if that element exists
 * Return NULL if element not belongs a vector
 * (((  DEPRECATED  ))) will be remove at future
 * (((   REPLACED   ))) by get_index function
 * @param vector1 a vector that contains integers
 * @param value a seek element
 * @return an integer
 */
int in(const int &value, std::vector <int> &vector) {
    std::vector<int>::iterator itr = std::find(vector.begin(), vector.end(), value);
    int result = NULL; //// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< change to NULL?
    if (itr != vector.cend()) {
        result = std::distance(vector.begin(), itr);
    }
    return result;
}

/**
 * @details Return an index from an element at set if that element exists
 * Return NULL if element not belongs a set
 * @param set a set that contains integers
 * @param value a seek element
 * @return an integer
 */
int in(const int &value, std::set <int> &set) {
    std::set<int>::iterator itr = std::find(set.begin(), set.end(), value);
    int result = NULL; //// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< change to NULL?
    if (itr != set.cend()) {
        result = std::distance(set.begin(), itr);
    }
    return result;
}


/**
 * Split a string
 * @details Split a string
 * The delimiter is not optional because there isn't implemented default, you must select one.
 * 
 * Reference:
 * https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
 * Solution 1.3
 * @author Carlos Thadeu
 * @param s a string
 * @param delimiter a char that represents a delimiter of a string
 * @return a string that represents the name of file
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

/**
 * Get the filename passed in by redirection
 * @details Get the filename passed in by redirection
 * Reference: https://stackoverflow.com/questions/62697081/get-the-name-of-an-input-file-passed-in-by-redirection
 * @author Carlos Thadeu
 * @return a string that represents the name of file
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

/**
 * Seek for articulations and bridges at a graph
 * @details Locate if a graph contains articulations and bridges
 * @author Carlos Thadeu
 * @param graph a graph that represents the graph
 * @return a tuple that contains a set and a vector of pairs.
 */
std::tuple<std::set<int>, std::vector<std::pair<int,int>>> seek_articulations(Graph &graph){
    int n = graph.get_qty_vertex();
    int tme=1; //initialize time with 0
    std::vector <int> disc, low; //these are the discovery and lowest time of a particular node
    for (int i=0; i < n; i++){
        disc.push_back(0);
        low.push_back(0);
    }
    std::set<int> articu_p; 
    std::vector<std::pair<int,int>> bridge;//it represents the edges which acts as bridge
    special_dfs(0, -1, tme, disc, low, articu_p, bridge, graph);

    return std::make_tuple(articu_p, bridge);
}


/**
 * DFS to locate articulations and bridges
 * @details That's a DFS that seek and found if exists articulations and bridges at graph.
 * How the articulations and bridges are seek at same time, they are stored at two special parameters,
 * articu_p and bridge.
 * Adapter from https://github.com/sprsgupta/Articulation_points_in_graph/blob/master/Connected%20graphs
 * @author Carlos Thadeu
 * @param cur an integer that represents the current vertex
 * @param par an integer that represents the previous vertex 
 * @param tme an integer that represents the interation ("time")
 * @param disc a vector that are the discovery and lowest time of a particular node
 * @param low a vector that
 * @param articu_p a set that contains the articulations found 
 * @param bridge a vector of pair that contains the bridges found
 * @param graph a graph that represents the graph
 */

void special_dfs(int cur,int par, int &tme, std::vector <int> &disc, std::vector <int> &low,
std::set<int> &articu_p, std::vector<std::pair<int,int>> &bridge, Graph &graph){
    int child_count=0;
    disc[cur]=low[cur]=tme++;//increase the time for next call 
    //for(auto child : adj[cur]){
    for(auto child : graph.adjList(cur)){
     //if child is not visited yet
            
        if(disc[child]==0){
            //inititate a dfs call
            special_dfs(child, cur, tme, disc, low, articu_p, bridge, graph);
                //increase the child count corr to curr node
            child_count+=1;
                //update lowest time values
            low[cur]=std::min(low[cur],low[child]);
                //condition to be fulfilled for articulation point
                // take root otherwise
            if(cur !=0 && low[child]>=disc[cur]){
                articu_p.insert(cur);
            }
                //condition to be fulfilled for bridges
            if(low[child]>disc[cur]){
                bridge.push_back({child,cur});
            }
                
        }
            // now check for backedge
        else if(child !=par){
            low[cur]=std::min(low[cur],disc[child]);
        }
    }
        // now acccount for root node
    if(cur==0 && child_count>=2){
        articu_p.insert(cur);
    }

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

