#ifndef CTFUNCTIONS_HPP_
#define CTFUNCTIONS_HPP_

#include <iostream>
//#include<bits/stdc++.h>
#include <set>
#include <iomanip>      // std::setw

#include "../code/graph.hpp"

extern bool graph_type;
extern std::string paralel_type;
extern int min_vertices;
extern int max_vertices;
extern int num_graph;
extern float probability;
extern bool global_yed;

void swap0(int *a, int *b);
void bubbleSort(int **array, int n);

// Distance functions
int get_index(int key, std::vector<int> v);

// Search functions
bool in(const int &value, std::vector <int> &vector);
bool in(const int &value, const std::vector <int> &vector);
bool in(const int &value, std::set <int> &set );
bool in( const std::pair<int, int> &edge, const std::vector<std::pair<int, int>> &vector);
bool in(const int &u, const int &v, const std::vector<std::pair<int,int>> &edges_to_be_processed);
//bool in(const int &v, const std::vector<std::pair<int,int>> &edges_to_be_processed);
bool in(const int &value, const char position, const std::vector<std::pair<int,int>> &edges_to_be_processed);

//String functions
std::vector<std::string> split(const std::string& s, char delimiter);
std::vector<std::string> split_filename_v2(const std::string &s, char delim);
//
std::vector<int> join_vectors(std::vector<int> &vector1, std::vector<int> &vector2);
void output_data(std::string &run_name, std::string &filename, int &output, bool &best, double &lastExecutionTime, int &lower_limit, Graph &graph);
std::string current_date();

std::vector<std::vector<int>> combinatorics(int n, int r);

bool isDataAvailable();
bool isInputRedirected();

bool validateInputBeforeExecution(int argc, char** argv);

// Read and create graphs
std::string get_filename(); // DEPRECATED replaced by get_filename_v2()
std::string get_filename_v2();
//std::string get_filename_v3();
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

bool validarEntradaArestas(std::string entrada);
bool validarEntradaAdjacencia(std::string entrada);
std::vector<double> extrairNumeros(std::string entrada);

Graph readTreeFromFile(const std::string& caminhoArquivo);

int save_yed_file(Graph &graph, const std::string &filename);
std::string yed_file(Graph &graph);
std::string generate_string_edges(int nedge, int u, int v, bool red_edge);
std::string generate_string_nodes(int i);
std::string ensure_graphml_extension(const std::string& filename);


#endif //CTFUNCTIONS_HPP_