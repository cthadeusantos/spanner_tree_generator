#include <iostream>
#include <sys/stat.h>
//#include <errno.h>
//#include <stdio.h>
#include <iostream>
#include <sstream>
#include<unistd.h>
#include <math.h>
#include <iterator>
#include <chrono>

//#include <iostream>
#include <vector>
//#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdlib>

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <cerrno>

#include <iterator>
#include <set>
#include <regex>

#include <cstdlib>
#include <sys/types.h>

#include <iostream>
#include <string>
#include <random>
#include <sstream>

//#include <string.h>
#include "../code/opBasic.hpp"
#include "../code/stretch.hpp"
#include "../code/frontier.hpp"
#include "../code/genGraph.hpp"
#include "../code/version.hpp"
#include "../code/parameters.hpp"

#include "../Debug.h"

#include "ctfunctions2.hpp"
#include "my_limits.hpp"
//#include "externals.hpp"


bool graph_type=false;
std::string paralel_type="m";
int min_vertices = 0;
int max_vertices = 0;
int num_graph = 0;
float probability = 0.45;
extern int num_threads;
extern int used_threads;
extern int global_induced_cycle;
extern int global_induced_cycle_used;
extern int global_closeness;
extern int global_threads_supported;
extern bool global_nolb;
extern bool global_noindex;

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

std::string get_closeness_type(){
    if (global_closeness==1) {
        return "ALGEBRAIC";
    } else if (global_closeness==2){
        return "TRAVERSE";
    } else {
        return "NO";
    }
}

std::string get_noindex_type(){
    if (global_noindex){
        return "NO";
    } else {
        return "YES";
    }
}

std::string get_nolb_type(){
    if (global_nolb){
        return "NO";
    } else {
        return "YES";
    }
}

std::string current_date()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char buf[100] = {0};
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", std::localtime(&now));
    return buf;
}




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
 * @details Is the value belongs the vector?
 * @param vector a vector that contains integers
 * @param pair a pair searched
 * @return a boolean
 */
/*
bool in(const int &v, const std::vector<std::pair<int,int>> &edges_to_be_processed) {
    bool result = false;
    for(auto edge : edges_to_be_processed) {
        if ((edge.first == v ||  edge.second == v)) {
            result = true;
            break;
        }
    }
    return result;
}
*/

/**
 * @details Is the value belongs the vector?
 * @param vector a vector that contains integers
 * @param pair a pair searched
 * @return a boolean
 */
bool in(const int &value, const char position, const std::vector<std::pair<int,int>> &edges_to_be_processed) {
    bool result = false;
    for(auto edge : edges_to_be_processed) {
        if (((edge.first == value ||  edge.second == value) && position == 'b') || (edge.first == value && position == 'f') || (edge.second == value && position == 's')) {
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
 * DEPRECATED ** Replaced by get_filename_v2() for use in Linux and MacOS
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
    ssize_t var = readlink(buf, file, sizeof file - 1);

	std::string text = file;
	std::istringstream iss(text);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
									std::istream_iterator<std::string>());

    // At Windows and MacOS will needs another delimiter? like inverse / ?
    // I cannot test because I am using Linux
	std::vector<std::string> texto = split(text, '/');
	return texto[texto.size()-1];
}


// std::vector<std::string> split_filename_v2(const std::string &s, char delim) {
//     std::vector<std::string> result;
//     std::istringstream iss(s);
//     std::string item;
//     while (std::getline(iss, item, delim)) {
//         result.push_back(item);
//     }
//     return result;
// }


// FUNCTION BELOW , works in Linux , but don't work at MacOS
// /**
//  * Get the filename passed in by redirection
//  * @details Get the filename passed in by redirection
//  * @author Carlos Thadeu
//  * @return a string that represents the name of file
//  */
// std::string get_filename_v2() {
//     // To get filename redirections
//     char buf[512];
    
//     // Get file descriptor associated with stdin
//     int fd = fileno(stdin);

//     // Use readlink to get the path of the file
//     ssize_t len = readlink(("/proc/self/fd/" + std::to_string(fd)).c_str(), buf, sizeof(buf) - 1);

//     std::cout << "-->" << len << std::endl ;

//     if (len != -1) {
//         buf[len] = '\0';
//         std::string text = buf;

//         // Use platform-independent path separator
//         char pathSeparator = '/';
//         std::vector<std::string> texto = split_filename_v2(text, pathSeparator);
//         return texto.back();
//     } else {
//         // Handle error
//         return "Error";
//     }
// }

std::vector<std::string> split_filename_v2(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

#include <iostream>
#include <unistd.h>
std::string get_filename_v2() {
#if defined(__linux__)
    //DEBUG std::cerr << "Running on Linux" << std::endl;
    if (!isatty(fileno(stdin))) {
        int fileDescriptor = fileno(stdin);
        if (fileDescriptor != -1) {
            char path[PATH_MAX];
            ssize_t len = readlink(("/proc/self/fd/" + std::to_string(fileDescriptor)).c_str(), path, sizeof(path) - 1);
            if (len != -1) {
                path[len] = '\0';
                //return std::string(path);
                // Use platform-independent path separator
                char pathSeparator = '/';
                std::vector<std::string> texto = split_filename_v2(std::string(path), pathSeparator);
                return texto.back();
            }
        }
    }
#elif defined(__APPLE__) || defined(__MACH__)
    //DEBUG std::cerr << "Running on macOS" << std::endl;
    // Get file descriptor associated with stdin
    int fd = fileno(stdin);

    // Use fstat to get information about the file
    struct stat file_info;
    if (fstat(fd, &file_info) == 0) {
        // Use fcntl to get the file path from the file descriptor
        char path[PATH_MAX];
        if (fcntl(fd, F_GETPATH, path) != -1) {
            std::string text = path;

            // Use platform-independent path separator
            char pathSeparator = '/';
            std::vector<std::string> texto = split_filename_v2(text, pathSeparator);
            return texto.back();
        }
    }
#elif defined(_WIN32) || defined(_WIN64)
    std::cout << "Running on Windows" << std::endl;
    std::cout << "I cannot read the input file on this operating system. Please contact the developers." << std::endl;
    exit(1);
#elif defined(__FreeBSD__)
    std::cout << "Running on FreeBSD" << std::endl;
    std::cout << "I cannot read the input file on this operating system. Please contact the developers." << std::endl;
    exit(1);
#elif defined(__unix__) || defined(__unix)
    std::cout << "Running on a Unix-like system" << std::endl;
    std::cout << "I cannot read the input file on this operating system. Please contact the developers." << std::endl;
    exit(1);
#else
    std::cout << "Running on an unknown or unsupported operating system" << std::endl;
    std::cout << "I cannot read the input file on this operating system. Please contact the developers." << std::endl;
    exit(1);
#endif


    // Default implementation or unsupported platform
    return "Error";
}



// //#include <iostream>
// //#include <unistd.h>
// std::string get_filename_v3() {
// #ifdef __linux__ // Linux-specific implementation
//     if (!isatty(fileno(stdin))) {
//         int fileDescriptor = fileno(stdin);
//         if (fileDescriptor != -1) {
//             char path[PATH_MAX];
//             ssize_t len = readlink(("/proc/self/fd/" + std::to_string(fileDescriptor)).c_str(), path, sizeof(path) - 1);
//             if (len != -1) {
//                 path[len] = '\0';
//                 //return std::string(path);
//                 // Use platform-independent path separator
//                 char pathSeparator = '/';
//                 std::vector<std::string> texto = split_filename_v2(std::string(path), pathSeparator);
//                 return texto.back();
//             }
//         }
//     }
// #elif __APPLE__ // macOS-specific implementation
//     if (!isatty(fileno(stdin))) {
//         int fileDescriptor = fileno(stdin);
//         if (fileDescriptor != -1) {
//             char path[PATH_MAX];
//             ssize_t len = readlink(("/dev/fd/" + std::to_string(fileDescriptor)).c_str(), path, sizeof(path) - 1);
//             if (len != -1) {
//                 path[len] = '\0';
//                 //return std::string(path);
//                 // Use platform-independent path separator
//                 char pathSeparator = '/';
//                 std::vector<std::string> texto = split_filename_v2(std::string(path), pathSeparator);
//                 return texto.back();
//             }
//         }
//     }
// #endif

//     // Default implementation or unsupported platform
//     return "";
// }


// std::string get_filename_v2() {
//     // Get file descriptor associated with stdin
//     int fd = fileno(stdin);

//     // Use fstat to get information about the file
//     struct stat file_info;
//     if (fstat(fd, &file_info) == 0) {
//         // Use realpath to get the canonicalized absolute pathname
//         char resolved_path[PATH_MAX];

//         // Construa a string diretamente sem usar c_str()
//         std::string path_string = "/proc/self/fd/" + std::to_string(fd);
        
//         if (realpath(path_string.c_str(), resolved_path) != nullptr) {
//             std::string text = resolved_path;

//             // Use platform-independent path separator
//             char pathSeparator = '/';
//             std::vector<std::string> texto = split_filename_v2(text, pathSeparator);
//             return texto.back();
//         }
//     }

//     // Handle error
//     return "Error";
// }

// std::string get_filename_v2() {
//     // Get file descriptor associated with stdin
//     int fd = fileno(stdin);

//     // Use fstat to get information about the file
//     struct stat file_info;
//     if (fstat(fd, &file_info) == 0) {
//         // Use fcntl to get the file path from the file descriptor
//         char path[PATH_MAX];
//         if (fcntl(fd, F_GETPATH, path) != -1) {
//             std::string text = path;

//             // Use platform-independent path separator
//             char pathSeparator = '/';
//             std::vector<std::string> texto = split_filename_v2(text, pathSeparator);
//             return texto.back();
//         }
//     }

//     // Handle error
//     return "Error";
// }

bool isDataAvailable() {
    // Verificar se há dados disponíveis no redirecionamento (stdin)
    return std::cin.peek() != EOF;
}

// Checa se existe algum redirecionamento
bool isInputRedirected() {
    return !isatty(fileno(stdin));
}

bool validateInputBeforeExecution(int argc, char** argv){
    if(argc < 1){
		Parameters::usage(argv[0]);
		exit(1);
	}
	if (!isInputRedirected()) {
		std::cout << "No input redirection detected." << std::endl;
        Parameters::usage(argv[0]);
		exit(1);
	}
	if (!isDataAvailable()) {
		std::cout << "No data provided via redirection." << std::endl;
        Parameters::usage(argv[0]);
		exit(1);
	}
    return true;
}

// AS FUNCOES ABAIXO FUNCIONAM EM C++20
// std::vector<std::string> split_filename_v2(const std::string &s, char delim) {
//     std::vector<std::string> result;
//     std::istringstream iss(s);
//     std::string item;
//     while (std::getline(iss, item, delim)) {
//         result.push_back(item);
//     }
//     return result;
// }

// /**
//  * Get the filename passed in by redirection
//  * @details Get the filename passed in by redirection
//  * @author Carlos Thadeu
//  * @return a string that represents the name of file
//  */
// std::string get_filename_v2() {
//     // To get filename redirections
//     char buf[512], file[512] = {0};

//     // Get file descriptor associated with stdin
//     int fd = fileno(stdin);

//     // Get file status using fstat
//     struct stat st;
//     if (fstat(fd, &st) == -1) {
//         // Handle error
//         return "Error";
//     }

//     // Get the path of the file using fcntl
//     if (fcntl(fd, F_GETPATH, file) == -1) {
//         // Handle error
//         return "Error";
//     }

//     std::string text = file;

//     // Use platform-independent path separator
//     char pathSeparator = '/';
//     std::vector<std::string> texto = split_filename_v2(text, pathSeparator);
//     return texto.back();
// }

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
}

// END OF FILE & REDIRECTIONS FUNCTIONS

/**
 * Output data function 
 * @details Show data at screen or file or debug mode
 * @author Carlos Thadeu
 */
void output_data(std::string &run_name, std::string &filename, int &output, bool &best, double &lastExecutionTime, int &lower_limit, Graph &graph){
    // OUTPUT - nothing - screen - file - debug

    // if (lower_limit == (int)INFINITY)
    //     lower_limit = 1;
    if (lower_limit == std::numeric_limits<int>::infinity()) {
        lower_limit = 1;
    }

    int stretch_index = graph.get_stretch_index();

    std::string notes;
    notes = "[NOTE 1]: IF lower bound, stretch factor or stretch factor are equal to 1, it means that they were not evaluated;\n"
    "[NOTE 2]: IF total trees, icycles proposed or icycles selected are equal to 0, it means that they were not evaluated;\n"
    "[NOTE 3]: Running time measured in seconds;";

    if ((output & 1)==1){	// TO SCREEN
		std::cout << "INSTANCE ............. = " << filename << std::endl;
		std::cout << "SOLUTION_TYPE......... = " << run_name << std::endl;
		std::cout << "NUM_VERTICES.......... = " << graph.get_qty_vertex() << std::endl;
		std::cout << "NUM_EDGES............. = " << graph.get_num_edges() << std::endl;
		std::cout << "LOWER_BOUND........... = " << lower_limit << std::endl;
		std::cout << "STRETCH_INDEX......... = " << graph.get_stretch_index() <<  std::endl;
		std::cout << "STRETCH_FACTOR........ = " << graph.get_factor() <<  std::endl;
        std::cout << "TOTAL_TREES........... = " << graph.get_total_tree() <<  std::endl;
		std::cout << "RUNNING_TIME.......... = " << lastExecutionTime << std::endl;
        std::cout << "THREADs............... = " << num_threads <<  std::endl;
        std::cout << "MAX_THREADS_SUPPORTED. = " << global_threads_supported << std::endl;
        std::cout << "TASKs................. = " << used_threads <<  std::endl;
        std::cout << "DATE.................. = " << current_date() <<  std::endl;
        std::cout << "APP_RELEASE........... = " << Version().version() <<  std::endl;
        std::cout << "CLOSENESS_(HEUR)...... = " << get_closeness_type() << std::endl;
        std::cout << "COMPUTE_INDEX......... = " << get_noindex_type() << std::endl;
        std::cout << "COMPUTE_LOWER_BOUND... = " << get_nolb_type() << std::endl;
        std::cout << "ICYCLES_PROPOSED...... = " << global_induced_cycle <<  std::endl;
        std::cout << "ICYCLES_SELECTED...... = " << global_induced_cycle_used <<  std::endl;
        std::cout << notes << std::endl;

		if (best) {
            std::cout << "[BEST TREE]" <<  std::endl;
            graph.show_best_tree();
        }
        std::cout << std::endl;
	}
	if ((output & 2)==2){	// TO FILE
        std::cout << "INSTANCE=" << filename << std::endl;
		std::cout << "SOLUTION_TYPE=" << run_name << std::endl;
		std::cout << "NUM_VERTICES=" << graph.get_qty_vertex() << std::endl;
		std::cout << "NUM_EDGES=" << graph.get_num_edges() << std::endl;
		std::cout << "LOWER_BOUND=" << lower_limit << std::endl;
		std::cout << "STRETCH_INDEX=" << graph.get_stretch_index() <<  std::endl;
        std::cout << "STRETCH_FACTOR=" << graph.get_factor() <<  std::endl;
        std::cout << "TOTAL_TREES=" << graph.get_total_tree() <<  std::endl;
		std::cout << "RUNNING_TIME=" << lastExecutionTime << std::endl;
        std::cout << "THREADS=" << num_threads <<  std::endl;
        std::cout << "MAX_THREADS_SUPPORTED=" << global_threads_supported << std::endl;
        std::cout << "TASKS=" << used_threads <<  std::endl;
        std::cout << "DATE=" << current_date() <<  std::endl;
        std::cout << "APP_RELEASE=" << Version().version() <<  std::endl;
        std::cout << "CLOSENESS_(HEUR)=" << get_closeness_type() << std::endl;
        std::cout << "COMPUTE_INDEX=" << get_noindex_type() << std::endl;
        std::cout << "COMPUTE_LOWER_BOUND=" << get_nolb_type() << std::endl;
        std::cout << "ICYCLES_PROPOSED=" << global_induced_cycle <<  std::endl;
        std::cout << "ICYCLES_SELECTED=" << global_induced_cycle_used <<  std::endl;
        std::cout << notes << std::endl;
		if (best) graph.show_best_tree();
        std::cout << std::endl;
	}
	if ((output & 64)==64){	// TO SCREEN AT DEBUGGER
		std::cerr << "[INSTANCE]=" << filename << std::endl;
		std::cerr << "[SOLUTION_TYPE]=" << run_name << std::endl;
		std::cerr << "[NUM_VERTICES]=" << graph.get_qty_vertex() << std::endl;
		std::cerr << "[NUM_EDGES]=" << graph.get_num_edges() << std::endl;
		std::cerr << "[LOWER_BOUND]=" << lower_limit << std::endl;
		std::cerr << "[STRETCH_INDEX]=" << graph.get_stretch_index() <<  std::endl;
        std::cerr << "[STRETCH_FACTOR]=" << graph.get_factor() <<  std::endl;
        std::cerr << "[TOTAL_TREES]=" << graph.get_total_tree() <<  std::endl;
		std::cerr << "[RUNNING_TIME]=" << lastExecutionTime << std::endl;
        std::cerr << "[THREADS]=" << num_threads <<  std::endl;
        std::cerr << "[MAX_THREADS_SUPPORTED]=" << global_threads_supported << std::endl;
        std::cerr << "[TASKS]=" << used_threads <<  std::endl;
        std::cerr << "[DATE]=" << current_date() <<  std::endl;
        std::cerr << "[APP_RELEASE]=" << Version().version() <<  std::endl;
        std::cerr << "[CLOSENESS_(HEUR)]=" << get_closeness_type() << std::endl;
        std::cerr << "[COMPUTE_INDEX]=" << get_noindex_type() << std::endl;
        std::cerr << "[COMPUTE_LOWER_BOUND]=" << get_nolb_type() << std::endl;
        std::cerr << "[ICYCLES_PROPOSED]=" << global_induced_cycle <<  std::endl;
        std::cerr << "[ICYCLES_SELECTED]=" << global_induced_cycle_used <<  std::endl;
        std::cerr << notes << std::endl;


		if (best){
            std::cerr << "[BEST TREE]" <<  std::endl;
			int node1 = 0, node2 = 0;
			for (auto&& tuple: graph.best_tree){
				std::tie(node1, node2) = tuple;
				DEBUG std::cerr << "(" << node1 << " , " << node2 << ") ";
			}
		}
        //std::cerr << std::endl;
	}
    if (global_yed){
        if (!save_yed_file(graph, filename)){
            std::string yed_filename = ensure_graphml_extension(filename);
            std::cout << "Arquivo salvo com sucesso como " << yed_filename << std::endl;
        } else {        
            std::cout << "Erro ao abrir o arquivo para escrita!" << std::endl;        
        };
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

Graph readTreeFromFile(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    Graph tree_of_graph;
    
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << std::endl;
        return tree_of_graph;
    }
    
    std::string linha;
    int numVertices = 0;
    
    // Lê o número de vértices (primeira linha)
    // if (getline(arquivo, linha)) {
    //     try {
    //         numVertices = stoi(linha);
    //         tree_of_graph.add_vertices(numVertices);
    //     } catch (...) {
    //         std::cerr << "Formato inválido. A primeira linha deve conter o número de vértices." << std::endl;
    //         return tree_of_graph;
    //     }
    // } else {
    //     std::cerr << "Arquivo vazio." << std::endl;
    //     return tree_of_graph;
    // }

    // Lê o número de vértices (primeira linha) evitando o try/catch
    // para não ter que remover -fno-exceptions do makefile
    // já que não sei o impacto no tempo de execução
    if (getline(arquivo, linha)) {
        char* endptr;
        errno = 0; // Resetar errno antes da conversão
        long numVerticesLong = std::strtol(linha.c_str(), &endptr, 10);

        // Verifica se a conversão foi bem-sucedida
        if (errno != 0 || *endptr != '\0' || numVerticesLong <= 0 || numVerticesLong > INT_MAX) {
            std::cerr << "Formato inválido. A primeira linha deve conter um número inteiro válido de vértices." << std::endl;
            exit(1);
            //return tree_of_graph;
        }

        numVertices = static_cast<int>(numVerticesLong);
        tree_of_graph.add_vertices(numVertices);
    } else {
        std::cerr << "Arquivo vazio." << std::endl;
        exit(1);
        //return tree_of_graph;
    }

    
    // Verifica o formato do arquivo
    bool formatoMatriz = false;
    bool formatoListaArestas = false;
    
    if (getline(arquivo, linha)) {
        // Verifica se a linha contém vírgulas (formato lista de arestas)
        if (linha.find(',') != std::string::npos) {
            formatoListaArestas = true;
        } 
        // Verifica se a linha contém apenas números e espaços (formato matriz)
        else {
            bool apenasNumerosEspacos = all_of(linha.begin(), linha.end(), [](char c) {
                return isdigit(c) || isspace(c);
            });
            
            if (apenasNumerosEspacos) {
                formatoMatriz = true;
            }
        }
        
        // Volta para a linha anterior para processar novamente
        arquivo.seekg(0);
        getline(arquivo, linha); // Pula a primeira linha novamente
    }
    
    if (!formatoMatriz && !formatoListaArestas) {
        std::cerr << "Formato de arquivo não reconhecido." << std::endl;
        exit(1);
        //return tree_of_graph;
    }
    
    // Processa o arquivo de acordo com o formato identificado
    if (formatoListaArestas) {
        // Processa formato lista de arestas (x,x)
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;
            
            replace(linha.begin(), linha.end(), ',', ' ');
            std::istringstream iss(linha);
            int u, v;
            
            if (iss >> u >> v) {
                if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
                    tree_of_graph.add_aresta(u, v);
                } else {
                    std::cerr << "Vértice inválido na aresta: " << u << "," << v << std::endl;
                }
            }
        }
    } else if (formatoMatriz) {
        // Processa formato matriz de adjacência
        int linhaAtual = 0;
        while (getline(arquivo, linha) && linhaAtual < numVertices) {
            if (linha.empty()) continue;
            
            std::istringstream iss(linha);
            int valor;
            int colunaAtual = 0;
            
            while (iss >> valor && colunaAtual < numVertices) {
                if (valor == 1) {
                    tree_of_graph.add_aresta(linhaAtual, colunaAtual);
                }
                colunaAtual++;
            }
            
            if (colunaAtual != numVertices) {
                std::cerr << "Número incorreto de colunas na matriz." << std::endl;
            }
            
            linhaAtual++;
        }
        
        if (linhaAtual != numVertices) {
            std::cerr << "Número incorreto de linhas na matriz." << std::endl;
        }
    }
    
    arquivo.close();
    return tree_of_graph;
}

std::string generate_string_nodes(int i) {
    // Gerador de números aleatórios
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> x_dist(0.0, 1000.0);
    static std::uniform_real_distribution<> y_dist(0.0, 1000.0);

    // Gerar valores aleatórios para x e y
    double x = x_dist(gen);
    double y = y_dist(gen);

    // Criar a string do nó
    std::ostringstream oss;
    oss << R"(    <node id="n)" << i << R"(">
      <data key="d6">
        <y:ShapeNode>
          <y:Geometry height="30.0" width="30.0" x=")" << x << R"(" y=")" << y << R"("/>
          <y:Fill color="#FFCC00" transparent="false"/>
          <y:BorderStyle color="#000000" raised="false" type="line" width="1.0"/>
          <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" height="18.1328125" horizontalTextPosition="center" iconTextGap="4" modelName="custom" textColor="#000000" verticalTextPosition="bottom" visible="true" width="46.0703125" x="-8.03515625" xml:space="preserve" y="5.93359375">)" << i << R"(<y:LabelModel><y:SmartNodeLabelModel distance="4.0"/></y:LabelModel><y:ModelParameter><y:SmartNodeLabelModelParameter labelRatioX="0.0" labelRatioY="0.0" nodeRatioX="0.0" nodeRatioY="0.0" offsetX="0.0" offsetY="0.0" upX="0.0" upY="-1.0"/></y:ModelParameter></y:NodeLabel>
          <y:Shape type="ellipse"/>
        </y:ShapeNode>
      </data>
    </node>)";

    return oss.str();
          //<y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" height="18.1328125" horizontalTextPosition="center" iconTextGap="4" modelName="custom" textColor="#000000" verticalTextPosition="bottom" visible="true" width="46.0703125" x="-8.03515625" xml:space="preserve" y="5.93359375">LABEL1<y:LabelModel><y:SmartNodeLabelModel distance="4.0"/></y:LabelModel><y:ModelParameter><y:SmartNodeLabelModelParameter labelRatioX="0.0" labelRatioY="0.0" nodeRatioX="0.0" nodeRatioY="0.0" offsetX="0.0" offsetY="0.0" upX="0.0" upY="-1.0"/></y:ModelParameter></y:NodeLabel>


}

std::string generate_string_edges(int nedge, int u, int v, bool red_edge) {

    // Criar a string do nó
    std::ostringstream oss;
    if (red_edge){
        oss << R"( <edge id="e)" << nedge << R"(" source="n)" << u << R"(" target="n)" << v << R"(">  
        <data key="d10">
            <y:PolyLineEdge>
            <y:Path sx="0.0" sy="0.0" tx="0.0" ty="0.0"/>
            <y:LineStyle color="#FF0000" type="line" width="6.0"/>
            <y:Arrows source="none" target="none"/>
            <y:BendStyle smoothed="false"/>
            </y:PolyLineEdge>
        </data>
        </edge> )";
    } else {
        oss << R"( <edge id="e)" << nedge << R"(" source="n)" << u << R"(" target="n)" << v << R"(">  
        <data key="d10">
            <y:PolyLineEdge>
            <y:Path sx="0.0" sy="0.0" tx="0.0" ty="0.0"/>
            <y:LineStyle color="#000000" type="line" width="1.0"/>
            <y:Arrows source="none" target="none"/>
            <y:BendStyle smoothed="false"/>
            </y:PolyLineEdge>
        </data>
        </edge> )";
    }


    return oss.str();
}

std::string yed_file(Graph &graph){
    std::string str0 =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
        "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" xmlns:java=\"http://www.yworks.com/xml/yfiles-common/1.0/java\" xmlns:sys=\"http://www.yworks.com/xml/yfiles-common/markup/primitives/2.0\" xmlns:x=\"http://www.yworks.com/xml/yfiles-common/markup/2.0\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:y=\"http://www.yworks.com/xml/graphml\" xmlns:yed=\"http://www.yworks.com/xml/yed/3\" xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://www.yworks.com/xml/schema/graphml/1.1/ygraphml.xsd\">"
        "<!--Created by yEd 3.25.1-->"
        "<key attr.name=\"Description\" attr.type=\"string\" for=\"graph\" id=\"d0\"/>"
        "<key for=\"port\" id=\"d1\" yfiles.type=\"portgraphics\"/>"
        "<key for=\"port\" id=\"d2\" yfiles.type=\"portgeometry\"/>"
        "<key for=\"port\" id=\"d3\" yfiles.type=\"portuserdata\"/>"
        "<key attr.name=\"url\" attr.type=\"string\" for=\"node\" id=\"d4\"/>"
        "<key attr.name=\"description\" attr.type=\"string\" for=\"node\" id=\"d5\"/>"
        "<key for=\"node\" id=\"d6\" yfiles.type=\"nodegraphics\"/>"
        "<key for=\"graphml\" id=\"d7\" yfiles.type=\"resources\"/>"
        "<key attr.name=\"url\" attr.type=\"string\" for=\"edge\" id=\"d8\"/>"
        "<key attr.name=\"description\" attr.type=\"string\" for=\"edge\" id=\"d9\"/>"
        "<key for=\"edge\" id=\"d10\" yfiles.type=\"edgegraphics\"/>"
        "<graph edgedefault=\"directed\" id=\"G\">"
        "<data key=\"d0\" xml:space=\"preserve\"/>";
    
    std::string strN =
        "  </graph>"
        "  <data key=\"d7\">"
        "    <y:Resources/>"
        "  </data>"
        "</graphml>";
    
    std::string allNodes;
    std::string allEdges;
    int nedges = 0;
    int nodes = graph.get_num_vertices();

    for (int i = 0; i < nodes; ++i) {
        std::string node = generate_string_nodes(i);
        allNodes += node + "\n";
    }

    for (int u=0; u < nodes; u++){
        for (int v: graph.adjList(u)){
            
            // Check if edge is in the best tree
            int node1 = 0, node2 = 0;
            bool red_edge =false;
            for (auto&& tuple: graph.best_tree){
               std::tie(node1, node2) = tuple;
               if ((node1==u && node2==v) || (node2==u && node1==v)){
                   red_edge = true;
                   break;
               }
            }

            if (v > u){
                std::string edge = generate_string_edges(nedges, u, v, red_edge);
                allEdges += edge + "\n";
                nedges++;
            }
        }
    }

    return str0 + allNodes + allEdges + strN ;
}

int save_yed_file(Graph &graph, const std::string &filename){
        // Gerar o conteúdo XML
    std::string xmlContent = yed_file(graph);
    
    std::string yed_filename = ensure_graphml_extension(filename);

    // Gravar em um arquivo
    std::ofstream outFile(yed_filename);
    if (outFile.is_open()) {
        outFile << xmlContent;
        outFile.close();
        //std::cout << "Arquivo salvo com sucesso como " << filename << std::endl;
        return 0;
    } //else {
        //std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
        return 1;
    //}
    //return 0;
}

std::string ensure_graphml_extension(const std::string& filename) {
    // Regex para verificar se tem extensão (captura nome e extensão separadamente)
    std::regex ext_regex(R"((.*?)(\.[^.]*|)$)");
    std::smatch matches;
    
    if (std::regex_match(filename, matches, ext_regex)) {
        // matches[1] = nome base (sem extensão)
        // matches[2] = extensão existente (ou vazio)
        return matches[1].str() + ".graphml";
    }
    
    // Fallback (caso raro onde regex falha)
    return filename + ".graphml";
}