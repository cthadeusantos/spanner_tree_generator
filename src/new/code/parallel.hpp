#ifndef PARALLEL_HPP_
    #define PARALLEL_HPP_
#endif


void find_index_parallel(Graph &g, int raiz, int start, int end, const int id);
void find_index_induced_cycle_method_1(Graph &graph, int raiz, int neighbor_start, const int id, std::vector<int> edges_list );
void find_index_induced_cycle_method_2( const int i, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);
//void find_index_induced_cycle_method_3( const int i, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);

void create_threads(Graph& g);
//void create_threads_induced_cycle_method_1(Graph &g);
//void create_threads_induced_cycle_method_2(Graph &g);
void create_threads_induced_cycle_method_3(Graph &g);
void create_threads_induced_cycle_method_3v2(Graph &g);
void create_threads_articulations(Graph &g);
void create_threads_edge_max_degree(Graph &g);

int adj_id(Graph& g, int v, int adj);
int next(int a, int limite);
int find_factor(Graph& g, Graph& tree);
int vertice_maior_grau(Graph& g);

void find_index_pararell_edge(Graph& g, std::vector<int> edges, int start, const int id);
void find_index_articulation(Graph &graph, Graph &subgraph, int raiz, int start, int end, const int id);

void set_graph_final_parameters(int &index_local, int &total_arv, int &arv, Graph &tree_local, Graph &graph);
//std::vector<std::vector<int>> seeking_induced_cycles_edges_v1(Graph &graph);
//std::vector<int> seeking_induced_cycles_edges_v1(Graph &graph);
std::vector<int> seeking_induced_cycles_edges_v2(Graph &graph);
std::vector<int> seeking_induced_cycles_edges_v3(Graph &graph);
std::vector<int> seeking_induced_cycles_edges_v3r1(Graph &graph);

void search_for_induced_cycles_for_M2(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, Graph &graph);
void search_for_induced_cycles_for_M2_revision1(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph);
void search_for_induced_cycles_for_M2_revision1_only_one(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph);

std::vector<std::pair<int,int>> make_edges_list(std::vector<int> cycle_vertices_list);
Graph remove_edges_cycle_M2(std::vector<int> combinations, std::vector<std::pair<int, int>> edges, Graph graph);

std::vector<std::pair<int,int>> detect_valid_edges( std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);