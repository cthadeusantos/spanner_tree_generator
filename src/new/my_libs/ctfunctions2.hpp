#include "../code/graph.hpp"

void swap(int *a, int *b);
void bubbleSort(int **array, int n);
bool in(std::vector <int> &vector1, const int &value);
std::vector<std::string> split(const std::string& s, char delimiter);
std::string get_filename();
std::string get_enviroment_var(const char *enviroment_var);
std::string get_current_dir();
void create_directory(std::string auxiliary);
Graph read_graph_file();
/* int create_new_graphs();
 */