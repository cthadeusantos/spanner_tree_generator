#ifndef CYCLES_HPP_
#define CYCLES_HPP_

std::vector<std::pair<int,int>> immovable(int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed);
void find_index_induced_cycle_method_1(Graph &graph, int raiz, int neighbor_start, const int id, std::vector<int> edges_list );
void find_index_induced_cycle_method_2( const int i, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);
void find_index_induced_cycle_method_4(int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);

void create_threads_induced_cycle_method_3(Graph &g);
void create_threads_induced_cycle_method_3v2(Graph &g);
void create_threads_induced_cycle_method_4v1(Graph& graph);
void create_threadV4_auxiliary( int start, int end, const int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);

void search_for_induced_cycles_for_M2(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, Graph &graph);
void search_for_induced_cycles_for_M2_revision1(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph);
void search_for_induced_cycles_for_M2_revision1_only_one(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph);
void search_for_induced_cycles_for_M4(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph);

std::vector<int> seeking_induced_cycles_edges_v2(Graph &graph);
std::vector<int> seeking_induced_cycles_edges_v3(Graph &graph);
std::vector<int> seeking_induced_cycles_edges_v3r1(Graph &graph);
std::vector<std::vector<std::pair<int, int>>> seeking_induced_cycles_edges_v4(Graph &graph);

std::vector<std::pair<int,int>> make_edges_list(std::vector<int> cycle_vertices_list);
Graph remove_edges_cycle_M2(std::vector<int> combinations, std::vector<std::pair<int, int>> edges, Graph graph);

std::vector<std::pair<int,int>> detect_valid_edges( std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);
std::vector<std::vector<std::pair<int, int>>> cycle_combinations(std::vector<std::pair<int, int>> edges, int cycle_length);

std::vector<std::pair<int,int>> make_edges_list(std::vector<int> cycle_vertices_list);
std::vector<std::pair<int,int>> make_edges_list(std::vector<std::vector<std::pair<int, int>>> cycle_vertices_list);
std::vector<std::vector<std::pair<int,int>>> make_edges_list(std::vector<std::vector<int>> vector);

std::vector<std::pair<int,int>> detect_valid_edges_M4( std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);

void find_index_cycleM4(int start, const std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &G1, Graph &graph);
void find_index_cycleM4_2(int id, int root, std::vector<std::pair<int,int>> &immovable_edges, Graph &G1, Graph &graph);
#endif
