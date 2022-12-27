#ifndef PARALLEL_HPP_
    #define PARALLEL_HPP_
#endif

void find_index_parallel(Graph &g, int raiz, int start, int end, const int id);
void find_index_induced_cycle(Graph &graph, int raiz, int start, const int id, std::vector<int> edges_list );
void create_threads(Graph& g);
void create_threads_big_cycle(Graph& g);
void create_threads_articulations(Graph& g);
int adj_id(Graph& g, int v, int adj);
int next(int a, int limite);
int find_factor(Graph& g, Graph& tree);
int vertice_maior_grau(Graph& g);

void create_threads_edge_max_degree(Graph& g);
void find_index_pararell_edge(Graph& g, std::vector<int> edges, int start, const int id);
void find_index_articulation(Graph &graph, Graph &subgraph, int raiz, int start, int end, const int id);