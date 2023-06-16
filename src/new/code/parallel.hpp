#ifndef PARALLEL_HPP_
#define PARALLEL_HPP_

void find_index_parallel(Graph &g, int raiz, int start, int end, const int id);
void find_index_pararell_edge(Graph& g, std::vector<int> edges, int start, const int id);
void find_index_parallel_edgeV2(Graph& g, std::vector<int> edges, int start, const int id);
//void find_index_articulation(Graph &graph, Graph &subgraph, int raiz, int start, int end, const int id);

void create_threads(Graph& g);
void create_threadsV2(Graph& graph);
//void create_threads_articulations(Graph &g);
void create_threads_edge_max_degree(Graph &g);
void create_threads_edge_max_degreeV2(Graph &g);
void create_threadV2_edge_max_degree_auxiliary(Graph &graph, std::vector<int> edges, int block_size, int id_th);

#endif
