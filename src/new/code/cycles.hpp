#ifndef CYCLES_HPP_
#define CYCLES_HPP_

struct Valid_Edges_r {
    //std::vector<std::vector<std::pair<int, int>>> edges_list;
    std::vector<std::vector<std::pair<int, int>>> disposable_edges_list;
    std::vector<std::vector<std::pair<int, int>>> immutable_edges_list;
} typedef Valid_Edges_r;

struct Blocks_r {
    int block_size;
    int chunk_size;
    int qty_block;
    int qty_chunk;
    int num_threads;
} typedef Blocks_r;

void create_threads_induced_cycle_method_4_NEW_APPROACH_V2_USANDO_FILA(Graph &graph);
void create_threadV4_auxiliary_NEW_APPROACHzzzzz2(int id, std::queue<Valid_Edges_r> &QUEUE_VECTOR, Graph &graph);

std::vector<std::vector<int>> generator_combinations(int nelements);
void create_threads_induced_cycle_method_4_NEW_APPROACH(Graph &graph);
//std::vector<std::vector<std::pair<int, int>>> detect_valid_edges_M4_NEW_APPROACH(std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);
Valid_Edges_r detect_valid_edges_M4_NEW_APPROACH(std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);
std::vector<std::pair<int, int>> make_edges_list(Graph graph);
Blocks_r define_block_chuck_for_cycleM4v2_NEW_APPROACH(int &num_threads, int &num_elements);
void create_threadV4_auxiliary_NEW_APPROACH(const int id, int start, int end, Valid_Edges_r &edges_to_threads, Graph &graph);
void create_threadV4_auxiliary_NEW_APPROACHxxxxx2(int id, int start, int end, std::vector<Valid_Edges_r> pilha, Graph &graph);
void find_index_induced_cycle_method_4_NEW_APPROACH(int id,  Valid_Edges_r &edges_to_threads , Graph &graph, int idx);
void find_index_cycleM4_NEW_APPROACH(int id, int root, Valid_Edges_r &edges_to_threads, Graph &G1, Graph &graph, int idx);

std::vector<std::pair<int,int>> immovable(int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed);
//void find_index_induced_cycle_method_1(Graph &graph, int raiz, int neighbor_start, const int id, std::vector<int> edges_list );
//void find_index_induced_cycle_method_2( const int i, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);
void find_index_induced_cycle_method_4(int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph, std::vector<std::pair<int, int>> &remains_edges);

//void create_threads_induced_cycle_method_3(Graph &g);
//void create_threads_induced_cycle_method_3v2(Graph &g);
void create_threads_induced_cycle_method_4v1(Graph& graph);
void create_threadV4_auxiliary( int start, int end, const int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph, std::vector<std::pair<int, int>> &remains_edges);

void search_for_induced_cycles_for_M2(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, Graph &graph);
void search_for_induced_cycles_for_M2_revision1(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph);
void search_for_induced_cycles_for_M2_revision1_only_one(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph);
void search_for_induced_cycles_for_M4(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph);

std::vector<int> seeking_induced_cycles_edges_v2(Graph &graph);
std::vector<int> seeking_induced_cycles_edges_v3(Graph &graph);
std::vector<int> seeking_induced_cycles_edges_v3r1(Graph &graph);
std::vector<std::vector<std::pair<int, int>>> seeking_induced_cycles_edges_v4(Graph &graph);

Graph remove_edges_cycle_M2(std::vector<int> combinations, std::vector<std::pair<int, int>> edges, Graph graph);

std::vector<std::pair<int,int>> detect_valid_edges( std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);
std::vector<std::vector<std::pair<int, int>>> cycle_combinations(std::vector<std::pair<int, int>> edges, int cycle_length);

//std::vector<std::pair<int,int>> make_edges_list(std::vector<int> cycle_vertices_list);
std::vector<std::pair<int,int>> make_edges_list(std::vector<int> cycle_vertices_list);
std::vector<std::pair<int,int>> make_edges_list(std::vector<std::vector<std::pair<int, int>>> cycle_vertices_list);
std::vector<std::vector<std::pair<int,int>>> make_edges_list(std::vector<std::vector<int>> vector);

std::vector<std::pair<int,int>> detect_valid_edges_M4( std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph);

//void find_index_cycleM4(int start, const std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &G1, Graph &graph);
void find_index_cycleM4_2(int id, int root, std::vector<std::pair<int,int>> &immovable_edges, Graph &G1, Graph &graph, std::vector<std::pair<int, int>> &remains_edges);


    std::tuple<int, int, int, int, int> define_block_chuck_for_cycleM4v2(int &num_threads, int &num_elements);
    void find_index_cycleM4_NEW(int id, int root, std::vector<std::pair<int, int>> &immovable_edges, Graph &G1, Graph &graph, std::vector<std::pair<int, int>> &remains_edges);
    void search_for_induced_cycles_for_M4v2(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph);

#endif
