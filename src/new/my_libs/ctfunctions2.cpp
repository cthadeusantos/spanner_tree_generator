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
#include <regex>

#include <cstdlib>
#include <sys/types.h>

#include <string.h>
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
extern int num_threads;
extern int used_threads;

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
bool in(const int &value, std::vector <int> &vector) {
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
bool in( const int &value, const std::vector <int> &vector) {
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
bool in(const int &value, std::set <int> &set) {
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
 * @details Is the value belongs the vector?
 * @param vector a vector that contains integers
 * @param pair a pair searched
 * @return a boolean
 */
bool in( const std::pair<int, int> &pair, const std::vector<std::pair<int, int>> &vector) {
    bool result = false;
    for(auto edge : vector) {
        if ((edge.first == pair.first &&  edge.second == pair.second)
        || (edge.first == pair.second &&  edge.second == pair.first)) {
            result = true;
            break;
        }
    }
    return result;
}

/**
 * @details Is the value belongs the vector?
 * @param vector a vector that contains integers
 * @param pair a pair searched
 * @return a boolean
 */
bool in(const int &u, const int &v, const std::vector<std::pair<int,int>> &edges_to_be_processed) {
    bool result = false;
    for(auto edge : edges_to_be_processed) {
        if ((edge.first == u &&  edge.second == v) || (edge.first == v &&  edge.second == u)) {
            result = true;
            break;
        }
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

/**
 * Join two vectors 
 * @details Join vector 1 and vector 2
 * @author Carlos Thadeu
 * @param vector1 a vector
 * @param vector2 a vector 
 * @return a vector (vector 1 + vector 2)
 */
std::vector<int> join_vectors(std::vector<int> &vector1, std::vector<int> &vector2){
    for (int i=0; i < vector2.size(); i++)
        vector1.push_back(vector2[i]);
    return vector1;
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
                    if (!validarEntradaAdjacencia(value_str)) {
                        std::cout << "WARNING! INVALID FILE! Please check if your file is an adjacency matrix!" << std::endl;
                        DEBUG std::cerr << "WARNING! INVALID FILE! Please check if your file is an adjacency matrix!" << std::endl;
                        exit(1);
                    }
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

/**
 * @details Read a graph file using redirections.
 * @param g a graph instance that represents a graph.
 * @return a integer the graph contains a cycle.
 */
Graph read_graph_file_edges_list() {

    char Ch = ' ';
    std::string value_str = "";
    int dimension = 0;
    int row = 0;
    int col = 0;
    int LetterCount = 0;
    int WordCount = 0;
    int node1 = 0;
    int node2 = 0;
    Graph graph;

    while (std::cin.get(Ch)) {
        if ((Ch == '\n') || (Ch == ' ')) {
            LetterCount = 0;
            if (value_str.size()) {
                if (WordCount++ == 0) {
                    dimension = std::stoi(value_str);
                    graph.add_vertices(dimension);
                } else {
                    if (!validarEntradaArestas(value_str)) {
                        std::cout << "WARNING! INVALID FILE! Please check if your file is a list edges!" << std::endl;
                        DEBUG std::cerr << "WARNING! INVALID FILE! Please check if your file is a list edges!" << std::endl;
                        exit(1);
                    }
                    std::vector<std::string> nodes = split(value_str, ',');
                    int node1 = stoi(nodes[0]);
                    int node2 = stoi(nodes[1]);
                    if (!(graph.possui_aresta(node1, node2) && graph.possui_aresta(node2, node1)))
                        graph.add_aresta(node1, node2);
                }
                value_str = "";
            }
        } else {
            value_str += Ch;
            ++LetterCount;
        }
    }
    // Fix bug on read edge list file
    // If file not end with endline or space
    // last edge not added at adjacency matrix
    // TO DO AT FUTURE: A function because this code is duplicate  less than IF clause
    if (!((Ch == '\n') || (Ch == ' '))) {
            LetterCount = 0;
            if (value_str.size()) {
                if (WordCount++ == 0) {
                    dimension = std::stoi(value_str);
                    graph.add_vertices(dimension);
                } else {
                    if (!validarEntradaArestas(value_str)) {
                        std::cout << "WARNING! INVALID FILE! Please check if your file is a list edges!" << std::endl;
                        DEBUG std::cerr << "WARNING! INVALID FILE! Please check if your file is a list edges!" << std::endl;
                        exit(1);
                    }
                    std::vector<std::string> nodes = split(value_str, ',');
                    node1 = stoi(nodes[0]);
                    node2 = stoi(nodes[1]);
                    if (!(graph.possui_aresta(node1, node2) && graph.possui_aresta(node2, node1)))
                        graph.add_aresta(node1, node2);
                }
                value_str = "";
            }
    }
    if (dimension != graph.get_num_vertices()){
        std::cout << "Invalid file!" << std::endl;
        exit(1);
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

// END OF FILE & REDIRECTIONS FUNCTIONS

/**
 * Output data function 
 * @details Show data at screen or file or debug mode
 * @author Carlos Thadeu
 */
void output_data(std::string &run_name, std::string &filename, int &output, bool &best, double &lastExecutionTime, int &lower_limit, Graph &graph){
    	// OUTPUT - nothing - screen - file - debug

    if (lower_limit == (int)INFINITY)
        lower_limit = 1;

    int stretch_index = graph.get_stretch_index();

    if ((output & 1)==1){	// TO SCREEN
		std::cout << "INSTANCE = " << filename << std::endl;
		std::cout << "SOLUTION_TYPE = " << run_name << std::endl;
		std::cout << "NUM_VERTICES = " << graph.get_qty_vertex() << std::endl;
		std::cout << "NUM_EDGES = " << graph.get_num_edges() << std::endl;
		std::cout << "LOWER_BOUND = " << lower_limit << std::endl;
		std::cout << "STRETCH_INDEX = " << stretch_index <<  std::endl;
		std::cout << "SUM_TREES = " << graph.get_total_tree() <<  std::endl;
		std::cout << "RUNNING_TIME = " << lastExecutionTime <<  std::endl;
        std::cout << "THREADs = " << num_threads <<  std::endl;
        std::cout << "USED_THREADs = " << used_threads <<  std::endl;
		if (best) {
            std::cout << "[BEST TREE]" <<  std::endl;
            graph.show_best_tree();
        }
        std::cout << std::endl << std::endl;
	}
	if ((output & 2)==2){	// TO FILE
        std::cout << "[INSTANCE]=" << filename << std::endl;
		std::cout << "[SOLUTION_TYPE]=" << run_name << std::endl;
		std::cout << "[NUM_VERTICES]=" << graph.get_qty_vertex() << std::endl;
		std::cout << "[NUM_EDGES]=" << graph.get_num_edges() << std::endl;
		std::cout << "[LOWER_BOUND]=" << lower_limit << std::endl;
		std::cout << "[STRETCH_INDEX]=" << stretch_index <<  std::endl;
		std::cout << "[SUM_TREES]=" << graph.get_total_tree() <<  std::endl;
		std::cout << "[RUNNING_TIME]=" << lastExecutionTime <<  std::endl;
        std::cout << "[THREADS]=" << num_threads <<  std::endl;
        std::cout << "[USED_THREADS]=" << used_threads <<  std::endl;
		if (best) graph.show_best_tree();
        std::cout << std::endl;
	}
	if ((output & 64)==64){	// TO SCREEN AT DEBUGGER
		std::cerr << "[INSTANCE]=" << filename << std::endl;
		std::cerr << "[SOLUTION_TYPE]=" << run_name << std::endl;
		std::cerr << "[NUM_VERTICES]=" << graph.get_qty_vertex() << std::endl;
		std::cerr << "[NUM_EDGES]=" << graph.get_num_edges() << std::endl;
		std::cerr << "[LOWER_BOUND]=" << lower_limit << std::endl;
		std::cerr << "[STRETCH_INDEX]=" << stretch_index <<  std::endl;
		std::cerr << "[SUM_TREES]=" << graph.get_total_tree() <<  std::endl;
		std::cerr << "[RUNNING_TIME]=" << lastExecutionTime <<  std::endl;
        std::cerr << "[THREADS]=" << num_threads <<  std::endl;
        std::cerr << "[USED_THREADS]=" << used_threads <<  std::endl;
		if (best){
            std::cerr << "[BEST TREE]" <<  std::endl;
			int node1 = 0, node2 = 0;
			for (auto&& tuple: graph.best_tree){
				std::tie(node1, node2) = tuple;
				DEBUG std::cerr << "(" << node1 << " , " << node2 << ") ";
			}
		}
        std::cerr << std::endl << std::endl;
	}
}

/*
Generate combinations
struct combinations needs to combinatorics(int n, int r)
*/
struct combinations
{
    typedef std::vector<int> combination_t;

    // initialize status
   combinations(int N, int R) :
       completed(N < 1 || R > N),
       generated(0),
       N(N), R(R)
   {
       for (int c = 1; c <= R; ++c)
           curr.push_back(c);
   }

   // true while there are more solutions
   bool completed;

   // count how many generated
   int generated;

   // get current and compute next combination
   combination_t next()
   {
       combination_t ret = curr;

       // find what to increment
       completed = true;
       for (int i = R - 1; i >= 0; --i)
           if (curr[i] < N - R + i + 1)
           {
               int j = curr[i] + 1;
               while (i <= R-1)
                   curr[i++] = j++;
               completed = false;
               ++generated;
               break;
           }

       return ret;
   }

private:

   int N, R;
   combination_t curr;
};

/* int combinatorics(int n, int r){

    combinations cs(n, r);
    while (!cs.completed)
    {
        combinations::combination_t c = cs.next();
        copy(c.begin(), c.end(), std::ostream_iterator<int>(std::cout, ","));
        std::cout << std::endl;
    }
    return cs.generated;
} */



/**
 * Generate a subsets r-element subsets in an n-element set  
 * @details C(n, r) is read as "n" choose "r". It determines the number of combinations of
 * n objects, taken r at a time (without replacement).
 * Example : C(3,2), returns {(0,1), (0,2), (1,2)}
 * Adapter from https://stackoverflow.com/questions/9430568/generating-combinations-in-c/9432150#9432150
 * @author Carlos Thadeu
 * @param n an integer that represents the initial set size
 * @param r an integer that r-element subset
 * @return a vector of vector of integers that represents the combinations
 */

std::vector<std::vector<int>> combinatorics(int n, int r){
    std::vector<std::vector<int>> combinacoes;
    combinations cs(n, r);
    while (!cs.completed)
    {
        combinations::combination_t c = cs.next();
        std::vector<int> prognosticos;
        for (auto i = c.begin(); i != c.end(); ++i)
        {
            prognosticos.push_back(*i-1);
        }

        combinacoes.push_back(prognosticos);
    }
    return combinacoes;
}

float logBn(float n, float b){
    return log(n) / log(b);
}

bool validarEntradaArestas(std::string entrada) {
    std::regex padrao("\\s*-?\\d+(\\.\\d+)?\\s*,\\s*-?\\d+(\\.\\d+)?\\s*");
    return regex_match(entrada, padrao);
}

bool validarEntradaAdjacencia(std::string entrada) {
    std::regex padrao("\\s*-?\\d+(\\.\\d+)?(\\s+-?\\d+(\\.\\d+)?)*\\s*");
    return regex_match(entrada, padrao);
}

std::vector<double> extrairNumeros(std::string entrada) {
    std::vector<double> numeros;
    std::istringstream iss(entrada);
    std::string numero;

    while (iss >> numero) {
        numeros.push_back(std::stod(numero));
    }

    return numeros;
}