#include <iostream>

#include "../code/graph.hpp"

extern bool graph_type;
extern std::string paralel_type;
extern int min_vertices;
extern int max_vertices;
extern int num_graph;
extern float probability;

void swap(int *a, int *b);
void bubbleSort(int **array, int n);
bool in(std::vector <int> &vector1, const int &value);
std::vector<std::string> split(const std::string& s, char delimiter);
std::string get_filename();
std::string get_enviroment_var(const char *enviroment_var);
std::string get_current_dir();
void create_directory(std::string auxiliary);
Graph read_graph_file();
int create_new_graphs();
