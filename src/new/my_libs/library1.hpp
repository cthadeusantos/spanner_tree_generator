
#include "../code/graph.hpp"
#include <tuple>

//std::tuple <int, int, int> define_block_chuck(int &num_threads, int &qty);
//std::tuple <int, int, int> define_block_chuck_for_max_degree(int &num_threads, int &num_elements);
//std::tuple <int, int, int> define_block_chuck_for_cycleM4(int &num_threads, int &num_elements);
//std::tuple <int, int, int, int, int> define_block_chuck_for_cycleM4v2(int &num_threads, int &num_elements);
std::vector<int> slicing(std::vector<int>& arr,int X, int Y);
int adj_id(Graph& g, int v, int adj);
int next(int a, int limite);
//int find_factor(Graph& g, Graph& tree);
void set_graph_final_parameters(int &index_local, int &total_arv, int &arv, Graph &tree_local, Graph &graph);
void set_graph_final_parameters(int &index_local, Graph &tree_local, Graph &graph);
//int highest_degree_vertex(Graph& g);
