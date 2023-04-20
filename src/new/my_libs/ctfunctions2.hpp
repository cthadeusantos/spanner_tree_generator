#include <iostream>
#include<bits/stdc++.h>
#include <set>

#include "../code/graph.hpp"

extern bool graph_type;
extern std::string paralel_type;
extern int min_vertices;
extern int max_vertices;
extern int num_graph;
extern float probability;

void swap0(int *a, int *b);
void bubbleSort(int **array, int n);

// Distance functions
int get_index(int key, std::vector<int> v);

// Search functions
bool in(const int &value, std::vector <int> &vector);
bool in(const int &value, const std::vector <int> &vector);
bool in(const int &value, std::set <int> &set );
bool in( const std::pair<int, int> &edge, const std::vector<std::pair<int, int>> &vector);

//String functions
std::vector<std::string> split(const std::string& s, char delimiter);

//
std::vector<int> join_vectors(std::vector<int> &vector1, std::vector<int> &vector2);
void output_data(std::string &run_name, std::string &filename, int &output, bool &best, double &lastExecutionTime, int &lower_limit, Graph &graph);

std::vector<std::vector<int>> combinatorics(int n, int r);

// Read and create graphs
std::string get_filename();
std::string get_enviroment_var(const char *enviroment_var);
std::string get_current_dir();
void create_directory(std::string auxiliary);
Graph read_graph_file();
Graph read_graph_file_edges_list();
int create_new_graphs();

// Special search
std::tuple<std::set<int>, std::vector<std::pair<int,int>> > seek_articulations(Graph &graph);
void special_dfs(int cur,int par, int &tme,
        std::vector <int> &disc, std::vector <int> &low, std::set<int> &articu_p,
        std::vector<std::pair<int,int>> &bridge, Graph &graph);

float logBn(float n, float b);