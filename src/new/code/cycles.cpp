#include <thread>      // std::thread
#include <mutex>       // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#include <initializer_list>
#include <cmath>
#include <iostream>
#include <tuple> // for tuple
#include <string>
#include <chrono> // contributor AZ

#include "../my_libs/library1.hpp"
#include "../my_libs/library3.hpp"
#include "graph.hpp"
#include "opBasic.hpp"
#include "stretch.hpp"
#include "cycles.hpp"
#include "centrality.hpp"

#include "../Debug.h"

extern sem_t semaforo;
extern std::mutex mtx;

extern int global_total_arv;
extern int num_threads;
extern int used_threads;
extern int global_induced_cycle;

extern bool global_noindex;
extern bool global_save_tree;
extern bool abort_for_timeout;

extern pthread_mutex_t mutex_signal;

/**
 * @brief Calculate block size and chunk size for threads for the M4 cycle method
 * @details block size and chunk size for threads for the M4 cycle method
 * @author Carlos Thadeu
 * @param num_threads a integer that represents the numbers of threads proposed
 * @param num_elements a integer that represents the numbers of the edges' combinations
 * @return a tuple of integers with block size and chunk size found and the number of threads selected to be used , the quantity of blocks and chunk
 */
std::tuple<int, int, int, int, int> define_block_chuck_for_cycleM4v2(int &num_threads, int &num_elements)
{
    int block_size = 1;
    int chunk_size = 0;
    int qty_block = 0;
    int qty_chunk = 0;
    int used_threads1 = num_threads;

    if (num_threads >= num_elements)
    {
        qty_block = num_threads;
        qty_chunk = 0;
    }
    else
    {
        int resto = num_elements % num_threads;
        int quociente = num_threads;
        if (resto < quociente && resto != 0)
        {
            qty_block = num_threads - 1;
            qty_chunk = 1;
            int divide = num_threads - 1;
            if (divide == 0)
            {
                divide = 1;
                qty_chunk = 0;
            }
            block_size = floor(num_elements / divide);
            chunk_size = num_elements - block_size;
        }
        else
        {
            if (resto != 0 and num_threads > 1)
            {
                qty_block = num_threads - 1;
                qty_chunk = 1;
                int divide = num_threads - 1;
                if (divide == 0)
                {
                    divide = 1;
                    qty_chunk = 0;
                }
                block_size = floor(num_elements / divide);
                chunk_size = num_elements - block_size;
            }
            else
            {
                qty_block = num_threads;
                qty_chunk = 0;
                block_size = floor(num_elements / num_threads);
                chunk_size = 0;
            }
        }
    }
    //std::cout << "BLOCK: " << qty_block << " " << block_size << " CHUNK" << qty_chunk << " " << chunk_size;
    return std::make_tuple(block_size, chunk_size, used_threads1, qty_block, qty_chunk);
}

/**
 * @brief Define immutable edges at induced cycle method M4 for a thread
 * @details Define immutable edges at induced cycle method M4
 * @author Carlos Thadeu
 */
std::vector<std::pair<int, int>> immovable(int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int, int>> &edges_to_be_processed)
{
    std::vector<std::pair<int, int>> immovable_edges;
    for (auto edge : edges_to_be_processed)
    {
        bool insert = true;
        for (auto idx : combinacoes[id])
        {
            if ((edge.first == edges_to_be_processed[idx].first && edge.second == edges_to_be_processed[idx].second) ||
                (edge.second == edges_to_be_processed[idx].first && edge.first == edges_to_be_processed[idx].second))
            {
                insert = false;
            }
        }
        if (insert)
        {
            immovable_edges.push_back(std::make_pair(edge.first, edge.second));
        }
    }
    return immovable_edges;
}

/**
 * @brief Calculate stretch index "method" for the M4 cycle method
 * @details Calculate stretch index "method" for the M4 cycle method
 * @details starting always at vertex 0
 * @author Carlos Thadeu
 * @param root a integer that represents the initial vertex (not are in use)
 * @param G1 a graph that represents the graph without vertices from combinations
 * @param graph a graph that represents the graph
 */
void find_index_cycleM4_2(int id, int root, std::vector<std::pair<int, int>> &immovable_edges, Graph &G1, Graph &graph)
{

    int vertex_v = 0;
    int vertex_u = 0;
    int index_local = (int)INFINITY;
    int lower_limit = (int)INFINITY;

    // G1 = graph; //For original
    std::vector<int> idx_next_neighbor(G1.get_num_vertices(), 0);
    std::vector<int> last_neighbor(G1.get_num_vertices(), -1);

    G1.reset_trees(0);

    Graph tree(G1.getQtdVertices());
    Graph tree_local(G1.getQtdVertices());

    for (auto edge : immovable_edges)
    {
        tree.add_aresta(edge.first, edge.second);
    }

    pthread_mutex_lock(&mutex_signal);
    lower_limit = graph.get_lower_limit();
    pthread_mutex_unlock(&mutex_signal);

    std::string fileName = std::to_string(id) + "saida.txt";
    std::string str_tree;

    while (graph.get_signal() && vertex_v >= 0 && !(abort_for_timeout))
    {
        if (idx_next_neighbor[vertex_v] == G1.grau(vertex_v))
        {
            idx_next_neighbor[vertex_v] = 0;
            vertex_v--;
            if (vertex_v < 0)
                break;
            if (!in(last_neighbor[vertex_v], vertex_v, immovable_edges))
            { // for modify
                tree.remove_aresta(vertex_v, last_neighbor[vertex_v]);
            } // for modify
            last_neighbor[vertex_v] = -1;
        }
        else
        {

            vertex_u = G1.adjList(vertex_v)[idx_next_neighbor[vertex_v]];
            ++idx_next_neighbor[vertex_v];

            if (not tree.possui_aresta(vertex_v, vertex_u))
            {

                tree.add_aresta(vertex_v, vertex_u);
                last_neighbor[vertex_v] = vertex_u;

                if (not OpBasic::is_cyclic(tree))
                {
                    if (tree.getQtdArestas() == tree.getQtdVertices() - 1)
                    {
                        int f = 1;
                        if (!global_noindex)
                        { // LF request - only sum tree
                            // pthread_mutex_lock (&mutex_signal);
                            f = Stretch::find_factor(graph, tree);
                            // pthread_mutex_unlock (&mutex_signal);
                        }
                        G1.add_tree();
                        if (global_save_tree)
                        {                                    // REMOVER - APENAS PARA TESTE
                            str_tree = tree_to_string(tree); // REMOVER - APENAS PARA TESTE
                            str_tree = ">" + str_tree;       // REMOVER - APENAS PARA TESTE
                            save_tree(str_tree, fileName);   // REMOVER - APENAS PARA TESTE
                        }                                    // REMOVER - APENAS PARA TESTE
                        if (f < index_local)
                        {
                            index_local = f;
                            tree_local = tree;
                            if (index_local == lower_limit)
                            {
                                pthread_mutex_lock(&mutex_signal);
                                graph.set_signal();
                                pthread_mutex_unlock(&mutex_signal);
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (vertex_v < G1.get_num_vertices() - 1)
                            vertex_v++;
                        continue;
                    }
                }
                if (!in(last_neighbor[vertex_v], vertex_v, immovable_edges))
                {
                    tree.remove_aresta(vertex_v, last_neighbor[vertex_v]);
                }  //else {
                    //if (vertex_v > 0)
                    //    vertex_v--;
                //}
            } // else { // ELSE OF IF HAS EDGE -- CHAVE 001
            if (in(vertex_u, vertex_v, immovable_edges))
            { // for modify  - CHANGE 002
                last_neighbor[vertex_v] = vertex_u;
                //int auxiliar = last_neighbor[vertex_u];
                //bool acerto = in(vertex_u, auxiliar, immovable_edges);
                //if ( vertex_v > auxiliar ) vertex_v++; // Deve considerar todos os vizinhos
                //if ( vertex_v > auxiliar || acerto ) vertex_v++; // Deve considerar todos os vizinhos
                vertex_v++;
                continue ;
            } // for modify - CHANGE 002
            //} //END OF IF HAS EDGE -- CHAVE 001
        }
    } // END OF WHILE
    //DEBUG std::cerr << "ID:" << id << "ACABOU" << index_local << std::endl;
    G1.set_stretch_index(index_local); // *********************************
    G1.set_best_tree(tree);
    // pthread_mutex_lock (&mutex_signal);
    // pthread_mutex_unlock (&mutex_signal);
}

void create_threadV4_auxiliary(int start, int end, const int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int, int>> &edges_to_be_processed, Graph &graph)
{
    // int idx = (end - start) * id;
    int acme = end - start;
    // std::chrono::time_point<std::chrono::steady_clock> start1 = std::chrono::steady_clock::now();

    // DEBUG std::cerr << "Start: " << start << "End " << end << std::endl;

    for (int i = 0; i < acme; i = i + 1)
    {
        if ((i + start) > combinacoes.size() - 1)
            break;
        find_index_induced_cycle_method_4(i + start, combinacoes, edges_to_be_processed, graph);
    }
}

/**
 * @brief Create threads to calculate stretch index for induced cycles (method 4)
 * @details Create threads to calculate the stretch index using induced cycles get from girth
 * @details Create threads from combinations (n,1) until (n,n)
 * @author Carlos Thadeu
 * @param graph a graph that represents the graph
 */
void create_threads_induced_cycle_method_4v1(Graph &graph)
{
    // int value = graph.get_stretch_index();
    // std::cout << "Valor achado: " << value << std::endl;
    int qty = 0;
    int root = -1;
    int neighbor = -1;
    int n;
    std::vector<std::vector<std::pair<int, int>>> edges_list;

    DEBUG std::cerr << "Searching for induced cycles using method 4!" << std::endl;
    edges_list = seeking_induced_cycles_edges_v4(graph);

    std::vector<std::pair<int, int>> edges_to_be_processed = make_edges_list(edges_list);
    DEBUG std::cerr << "Pre-processing edges!" << std::endl;

    edges_to_be_processed = detect_valid_edges_M4(edges_to_be_processed, graph);

    int r; // Generate the combinations
    std::vector<std::vector<int>> combinacoes;
    n = edges_to_be_processed.size();
    for (int i = n; i > 0; i--)
    {
        std::vector<std::vector<int>> edges_to_remove = combinatorics(n, i);
        for (int j = 0; j < edges_to_remove.size(); j++)
        {
            combinacoes.push_back(edges_to_remove[j]);
        }
    } // End of generate the combinations

    qty = combinacoes.size();

    DEBUG std::cerr << "Edges numbers to use!" << qty << std::endl;
    DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;

    // auto acme = define_block_chuck_for_cycleM4(num_threads, qty);
    auto acme = define_block_chuck_for_cycleM4v2(num_threads, qty);
    int block_size = std::get<0>(acme);
    int chunk_size = std::get<1>(acme);
    used_threads = std::get<2>(acme);

    int qty_block = std::get<3>(acme);
    int qty_chunk = std::get<4>(acme);

    DEBUG std::cerr << "Threads to allocated: " << used_threads << std::endl;

    // std::cout << qty << " " << num_threads << " " << block_size << " " << chunk_size << " " << used_threads << " " << std::endl;

    std::thread vetor_th[used_threads];

    std::reverse(combinacoes.begin(), combinacoes.end());
    std::vector<std::vector<int>> auxiliar = combinacoes;
    // int num_comb = combinacoes.size();
    // int jump = num_comb / combinacoes[idx_last].size();

    int idx_last = combinacoes[qty - 1].size();

    int counter = 0;
    for (int idx_size = 0; idx_size < idx_last; idx_size++)
    {
        for (int iyt = idx_size; iyt < qty; iyt = iyt + idx_last)
        {
            // int g = idx_size + iyt ;
            if (iyt < qty)
            {
                auxiliar[counter] = combinacoes[iyt];
                counter++;
            }
        }
    }

    combinacoes = auxiliar;

    int start, end;

    for (int i = 0; i < used_threads; ++i)
    {

        if (i < qty_block)
        {
            start = i * block_size;
            end = start + block_size;
        }
        else
        {
            if (i == qty_block)
            {
                start = i * block_size;
            }
            else
            {
                start = i * chunk_size;
            }
            end = start + chunk_size;
        }

        vetor_th[i] = std::thread(create_threadV4_auxiliary, start, end, i, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(graph));
    }

    sem_wait(&semaforo);

    for (int i = 0; i < used_threads; ++i)
    {
        vetor_th[i].join(); // junção das threads
    }
}

/*!
    Calculate a stretch index using an induced cycle's edge
    \param g a graph instance that represents the graph
    \param raiz an integer that represents the vertex root
    \param neighbor_start an integer that represents the vertex neighbor the root
    \param id an integer that represents a number of thread
    \param edges_list a vector that represents induced cycle's edge to be remove
    \param end
*/
void find_index_induced_cycle_method_4(int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int, int>> &edges_to_be_processed, Graph &graph)
{

    // mtx.lock();
    pthread_mutex_lock(&mutex_signal);
    //Graph G1 = graph;   // Auxiliary graph - local graph
    //Graph G1;
    Graph G1 = Graph(graph.get_num_vertices());
    OpBasic::copy(G1, graph);

    G1 = remove_edges_cycle_M2(combinacoes[id], edges_to_be_processed, graph);
    G1.reset_trees(0);
    
    pthread_mutex_unlock(&mutex_signal);
    // mtx.unlock();

    std::vector<std::pair<int, int>> immovable_edges = immovable(id, combinacoes, edges_to_be_processed);

    int num_vertices = G1.get_num_vertices();
    Graph tree(num_vertices);
    int num = combinacoes[id].size() - 1;
    int root = edges_to_be_processed[num].second;
    find_index_cycleM4_2(id, root, immovable_edges, G1, graph);

    int arvores;
    arvores = G1.get_total_tree();
    int index_local = G1.get_stretch_index();

    if (index_local == (int)INFINITY)
    {
        index_local = 1;
        arvores = 0;
        G1.reset_trees(arvores);
    }
    // G1.set_best_tree(s);
    // mtx.lock();
    pthread_mutex_lock(&mutex_signal);
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index " << index_local << std::endl;
    // graph.set_stretch_index(index_local);
    // G1.set_best_tree(tree);
    graph.sum_trees(arvores);
    global_total_arv = arvores;
    int arv = 0; // Insert to mantain compatibility with set_graph_final_parameters -- will be modified when refactoring
    DEBUG std::cerr << index_local << " " << G1.get_stretch_index() << " " << graph.get_stretch_index() << std::endl;
    set_graph_final_parameters(index_local, G1, graph);
    pthread_mutex_unlock(&mutex_signal);
    // mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
}

std::vector<std::pair<int, int>> detect_valid_edges(std::vector<std::pair<int, int>> &edges_to_be_processed, Graph &graph)
{
    std::vector<std::pair<int, int>> aux;
    // Verify for unabled edges to be removed
    int n = edges_to_be_processed.size();
    Graph G1 = graph;
    for (int i = 0; i < n; i++)
    {
        int v1 = edges_to_be_processed[i].first;
        int v2 = edges_to_be_processed[i].second;
        if (G1.possui_aresta(v1, v2))
        {
            G1.remove_aresta(v1, v2);
            if (G1.grau(v1) <= 1 && i != 0)
            {
                G1.add_aresta(v1, v2);
                if (n == (i / 2 + 1))
                {
                    DEBUG std::cerr << "Thread " << n << " with v1: " << v1 << " v2: " << v2 << " cannot be removed!" << std::endl;
                }
            }
            else
            {
                aux.push_back(std::make_pair(v1, v2));
            }
        }
    } // end of prodedure to Verify for unabled edges to be removed
    return aux;
}

/*
    Detect edges to be removed
*/
std::vector<std::pair<int, int>> detect_valid_edges_M4(std::vector<std::pair<int, int>> &edges_to_be_processed, Graph &graph)
{
    std::vector<std::pair<int, int>> aux;
    // Verify for unabled edges to be removed
    int n = edges_to_be_processed.size();
    Graph G1 = graph;
    bool flag = false;
    int connect = 0;
    for (int i = 0; i < n; i++)
    {
        int v1 = edges_to_be_processed[i].first;
        int v2 = edges_to_be_processed[i].second;

        if (G1.possui_aresta(v1, v2))
        {
            G1.remove_aresta(v1, v2);
            connect = OpBasic::distance(G1, v1, v2);
            if (connect == -1)
            {
                G1.add_aresta(v1, v2);
            }
            else
            {
                aux.push_back(std::make_pair(v1, v2));
            }
        }
    } // end of prodedure to Verify for unabled edges to be removed
    return aux;
}

/**
 * @brief Create threads to calculate stretch index for induced cycles (method 3)
 * @details Create threads to calculate the stretch index using induced cycles found(if exists)
 * @details Create threads from combinations (n,1) until (n,n)
 * @author Carlos Thadeu
 * @param g a graph that represents the graph
 */
// void create_threads_induced_cycle_method_3(Graph &g)
// {
//     int qty = -1;
//     int root = -1;
//     int neighbor = -1;
//     int n;
//     std::vector<int> edges_list = {};

//     DEBUG std::cerr << "Searching for induced cycles using method 3!" << std::endl;
//     edges_list = seeking_induced_cycles_edges_v3r1(g);

//     std::vector<std::pair<int, int>> edges_to_be_processed = make_edges_list(edges_list);
//     DEBUG std::cerr << "Pre-processing edges!" << std::endl;

//     edges_to_be_processed = detect_valid_edges(edges_to_be_processed, g);

//     int r; // Generate the combinations
//     std::vector<std::vector<int>> combinacoes;
//     n = edges_to_be_processed.size();
//     for (int i = n; i > 0; i--)
//     {
//         std::vector<std::vector<int>> edges_to_remove = combinatorics(n, i);
//         for (int j = 0; j < edges_to_remove.size(); j++)
//         {
//             combinacoes.push_back(edges_to_remove[j]);
//         }
//     } // End of generate the combinations

//     qty = combinacoes.size();

//     DEBUG std::cerr << "Edges numbers to use!" << qty << std::endl;

//     DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;
//     int threads = 0;
//     int block = 1;
//     if (num_threads >= qty)
//     {
//         num_threads = qty;
//     }
//     else if (num_threads < qty)
//     {
//         block = floor(qty / num_threads);
//     }
//     int chunk = qty - num_threads * block;
//     DEBUG std::cerr << "Threads to allocated: " << num_threads << std::endl;

//     std::thread vetor_th[num_threads];

//     for (int i = 0; i < block; ++i)
//     {
//         for (int j = 0; j < num_threads; j++)
//         {
//             int index = i * num_threads + j;
//             vetor_th[j] = std::thread(find_index_induced_cycle_method_2, index, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(g));
//         }
//         sem_wait(&semaforo);
//         for (int j = 0; j < num_threads; ++j)
//         {
//             vetor_th[j].join();
//         }
//     }
//     if (qty % num_threads)
//     {
//         for (int j = 0; j < chunk; j++)
//         {
//             int index = block * num_threads + j;
//             vetor_th[j] = std::thread(find_index_induced_cycle_method_2, index, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(g));
//         }
//         sem_wait(&semaforo);
//         for (int j = 0; j < chunk; ++j)
//         {
//             vetor_th[j].join();
//         }
//     }
// }

/**
 * @brief Create threads to calculate stretch index for induced cycles (method 3)
 * @details Create threads to calculate the stretch index using induced cycles found(if exists)
 * @details Create threads from combinations (n,1) until (n,n)
 * @author Carlos Thadeu
 * @param g a graph that represents the graph
 */
// void create_threads_induced_cycle_method_3v2(Graph &g)
// {
//     int qty = -1;
//     int root = -1;
//     int neighbor = -1;
//     int n;
//     std::vector<int> edges_list = {};
//     g.reset_trees(0);

//     DEBUG std::cerr << "Searching for induced cycles using method 3!" << std::endl;
//     edges_list = seeking_induced_cycles_edges_v3r1(g);

//     std::vector<std::pair<int, int>> edges_to_be_processed = make_edges_list(edges_list);
//     DEBUG std::cerr << "Pre-processing edges!" << std::endl;

//     edges_to_be_processed = detect_valid_edges(edges_to_be_processed, g);

//     int r; // Generate the combinations
//     std::vector<std::vector<int>> combinacoes;
//     n = edges_to_be_processed.size();
//     for (int i = n; i > 0; i--)
//     {
//         std::vector<std::vector<int>> edges_to_remove = combinatorics(n, i);
//         for (int j = 0; j < edges_to_remove.size(); j++)
//         {
//             combinacoes.push_back(edges_to_remove[j]);
//         }
//     } // End of generate the combinations

//     qty = combinacoes.size();

//     DEBUG std::cerr << "Edges numbers to use!" << qty << std::endl;

//     DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;

//     int threads = 0;
//     int block = 1;
//     int chunk;
//     if (num_threads >= qty)
//     {
//         used_threads = qty;
//     }
//     else if (num_threads < qty)
//     {
//         used_threads = num_threads;
//         block = floor(qty / num_threads);
//     }
//     chunk = qty - num_threads * block;
//     DEBUG std::cerr << "Threads to allocated: " << used_threads << std::endl;
//     std::thread vetor_th[used_threads];

//     for (int i = 0; i < block; ++i)
//     {
//         for (int j = 0; j < used_threads; j++)
//         {
//             int index = i * used_threads + j;
//             vetor_th[j] = std::thread(find_index_induced_cycle_method_2, index, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(g));
//         }
//         sem_wait(&semaforo);
//         for (int j = 0; j < used_threads; ++j)
//         {
//             vetor_th[j].join();
//         }
//     }

//     if (qty % used_threads)
//     {
//         for (int j = 0; j < chunk; j++)
//         {
//             int index = block * used_threads + j;
//             vetor_th[j] = std::thread(find_index_induced_cycle_method_2, index, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(g));
//         }
//         sem_wait(&semaforo);
//         for (int j = 0; j < chunk; ++j)
//         {
//             vetor_th[j].join();
//         }
//     }
// }

Graph remove_edges_cycle_M2(std::vector<int> combinations, std::vector<std::pair<int, int>> edges, Graph graph)
{
    for (auto i : combinations)
    {
        // if (i != -1)
        graph.remove_aresta(edges[i].first, edges[i].second);
    }
    //std::string grafo = tree_to_string(graph);
    // DEBUG std::cerr << "Este é o grafo: " << grafo << "\n";
    return graph;
}

/**
 * @brief Search for an induced cycle - PRECISION CLOSENESS - MUCH MORE SLOW
 * @details Search for an induced cycle (if exists) PRECISION CLOSENESS - MUCH MORE SLOW
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found
 */
std::vector<int> seeking_induced_cycles_edges_v2(Graph &graph)
{

    int max_cycle_size = floor(log2(num_threads + 1));
    DEBUG std::cerr << "Define induced cycle to size: " << max_cycle_size << std::endl;
    std::vector<std::vector<int>> select_cycles; //

    // Seleciona o vertice inicial
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
    std::vector<int> vertices = graph.vertices_de_maior_grau();
    std::vector<std::pair<int, float>> centrality = Centrality::closeness_centrality_list(vertices, graph);
    int root = Centrality::root_selection2(centrality);
    std::vector<int> neighbors = graph.adjList(root);                                                // Neighbors at root
    int seek = Centrality::root_selection2(Centrality::closeness_centrality_list(neighbors, graph)); // aplica a centralidade de proximidade
                                                                                                     // Fim da seleção do vértice inicial

    search_for_induced_cycles_for_M2(seek, root, max_cycle_size, select_cycles, graph);

    // TO DO
    // TO DO - Make a better choice from select_cycles
    // TO DO

    // Until make a better choice, select the first vector
    std::vector<int> edges_list;
    if (!select_cycles.empty())
        edges_list = select_cycles[0];
    return edges_list;
}

/**
 * @brief Search for an induced cycle (USING CLOSENESS CALCULATED)
 * @details Search for an induced cycle (if exists) (USING CLOSENESS CALCULATED)
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found
 */
std::vector<int> seeking_induced_cycles_edges_v3(Graph &graph)
{

    int max_cycle_size = floor(log2(num_threads + 1));
    DEBUG std::cerr << "Define induced cycle to size: " << max_cycle_size << std::endl;
    std::vector<std::vector<int>> select_cycles; //

    // Seleciona o vertice inicial
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
    std::vector<int> vertices = graph.vertices_de_maior_grau();

    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);

    DEBUG std::cerr << "Selecting root" << std::endl;
    int root = Centrality::root_selection3(vertices_closeness, vertices_leverage);

    DEBUG std::cerr << "Selected neighbor from root: " << root << std::endl;
    std::vector<int> neighbors = graph.adjList(root); // Neighbors at root
    int seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage);

    // Fim da seleção do vértice inicial

    // MANY CYCLES ARE SEARCH - UNCOMMENT BELOW LINE WHEN to do * Make a better choice from select_cycles*
    // search_for_induced_cycles_for_M2_revision1(seek, root, max_cycle_size, select_cycles, vertices_closeness, vertices_leverage, graph);

    // ONLY ONE CYCLE IS SEARCH - COMMENT BELOW LINE WHEN YOU USE search_for_induced_cycles_for_M2_revision1
    search_for_induced_cycles_for_M2_revision1_only_one(seek, root, max_cycle_size, select_cycles, vertices_closeness, vertices_leverage, graph);

    // TO DO
    // TO DO - Make a better choice from select_cycles
    // TO DO

    // Until make a better choice, select the first vector
    std::vector<int> edges_list;
    if (!select_cycles.empty())
        edges_list = select_cycles[0];
    return edges_list;
}

/**
 * @brief Search for an induced cycle (USING CLOSENESS CALCULATED)
 * @details Search for an induced cycle (if exists) (USING CLOSENESS CALCULATED)
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found
 */
std::vector<int> seeking_induced_cycles_edges_v3r1(Graph &graph)
{
    int max_cycle_size_SPECIAL;
    int n = graph.get_qty_vertex();
    // BELOW - DEFINE SIZE OF INDUCED CYCLE BASED ON THREADS NUMBERS
    // if ( num_threads < 7){
    //    max_cycle_size_SPECIAL=floor(log2(127 + 1));
    //} else {
    //    max_cycle_size_SPECIAL=floor(log2(num_threads + 1));
    //}

    // BELOW - FIXED MAX SIZE OF INDUCED CYCLE TO BE SEARCH
    max_cycle_size_SPECIAL = 7;
    DEBUG std::cerr << "Define induced cycle to size: " << max_cycle_size_SPECIAL << std::endl;
    std::vector<std::vector<int>> select_cycles; //

    // Seleciona o vertice inicial
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
    std::vector<int> vertices = graph.vertices_de_maior_grau();

    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);

    DEBUG std::cerr << "Selecting root" << std::endl;
    int root = Centrality::root_selection3(vertices_closeness, vertices_leverage);

    DEBUG std::cerr << "Selected neighbor from root: " << root << std::endl;
    std::vector<int> neighbors = graph.adjList(root); // Neighbors at root
    int seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage);

    // Fim da seleção do vértice inicial

    // MANY CYCLES ARE SEARCH - UNCOMMENT BELOW LINE WHEN to do * Make a better choice from select_cycles*
    // search_for_induced_cycles_for_M2_revision1(seek, root, max_cycle_size_SPECIAL, select_cycles, vertices_closeness, vertices_leverage, graph);

    // ONLY ONE CYCLE IS SEARCH - COMMENT BELOW LINE WHEN YOU USE search_for_induced_cycles_for_M2_revision1
    search_for_induced_cycles_for_M2_revision1_only_one(seek, root, max_cycle_size_SPECIAL, select_cycles, vertices_closeness, vertices_leverage, graph);

    // TO DO
    // TO DO - Make a better choice from select_cycles
    // TO DO - Needs improve cycle choice HERE
    // TO DO

    // Until make a better choice, select the first vector
    std::vector<int> edges_list;
    if (!select_cycles.empty())
        edges_list = select_cycles[0];
    return edges_list;
}

/**
 * @brief Search for an induced cycle (USING CLOSENESS CALCULATED)
 * @details Search for an induced cycle (if exists) (USING CLOSENESS CALCULATED)
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found
 */
std::vector<std::vector<std::pair<int, int>>> seeking_induced_cycles_edges_v4(Graph &graph)
{
    // BELOW - FIXED MAX SIZE OF INDUCED CYCLE TO BE SEARCH
    int cycle_size = graph.waist();
    graph.set_girth(cycle_size);

    DEBUG std::cerr << "Define induced cycle to size: " << cycle_size << std::endl;
    std::vector<std::vector<int>> select_cycles; //

    // Seleciona o vertice inicial
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
    std::vector<int> vertices = graph.vertices_de_maior_grau();

    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);

    DEBUG std::cerr << "Selecting root" << std::endl;
    int root = Centrality::root_selection3(vertices_closeness, vertices_leverage);

    DEBUG std::cerr << "Selected neighbor from root: " << root << std::endl;
    std::vector<int> neighbors = graph.adjList(root); // Neighbors at root
    int seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage);

    // Fim da seleção do vértice inicial

    search_for_induced_cycles_for_M4(seek, root, cycle_size, select_cycles, vertices_closeness, vertices_leverage, graph);

    return make_edges_list(select_cycles);
}

void search_for_induced_cycles_for_M2(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, Graph &graph)
{
    // int MAX_CYCLE_SIZE=floor(log2(num_threads + 1));
    int initial_max_cycle = cycle_size;

    std::vector<std::pair<int, float>> centrality;
    std::vector<int> neighbors;

    std::vector<int> processed;     // Processed vertices
    std::vector<int> vertices_list; // Vertices to be processed
    for (int i = 0; i < graph.get_qty_vertex(); i++)
    { // inicializa a lista
        vertices_list.push_back(i);
    }
    std::vector<int> cycle(1, 0); // create and initialize an auxiliary list

    int max_size_cycle = 0; // Define Max size list found  // Method 1 != Method 2

    while (true)
    {
        DEBUG std::cerr << "Searching for (" << cycle_size << ") cycle size" << std::endl;
        cycle = OpBasic::cycle(graph, cycle_size, seek, root);
        if (cycle.empty())
        {
            if (cycle_size > 3 && cycle_size > max_size_cycle)
            { // Não achei ciclo do tamanho pré-definido, procuro um menor
                cycle_size--;
                continue;
            }
            else
            { // I didn't even find a triangle, then I quit the loop
                break;
            }
        }
        else
        {
            cycle_size = initial_max_cycle;
        }
        for (int j = 0; j < cycle.size(); j++)
        { // Delete processed vertices
            int index = get_index(cycle[j], vertices_list);
            if (!vertices_list.empty() && index < vertices_list.size())
                vertices_list.erase(vertices_list.begin() + index);
            else if (vertices_list.empty())
                break;
        }
        cycle.push_back(root);
        processed.push_back(root);

        int acme = cycle.size() - 1; // auxiliary var
        if (!cycle.empty() && acme > max_size_cycle)
        {                                   // Method 1 != Method 2
            select_cycles.clear();          // Only cycles that has same size(maximum select)
            select_cycles.push_back(cycle); // will be selected
            if (cycle_size > max_size_cycle)
                max_size_cycle = acme;
        }
        else if (!cycle.empty() && acme == max_size_cycle)
        {
            select_cycles.push_back(cycle);
        }

        if (vertices_list.empty())
            break;

        bool gameover = false;
        while (!gameover)
        { // Escolhe nova raiz

            centrality = Centrality::closeness_centrality_list(vertices_list, graph);
            root = Centrality::root_selection2(centrality);
            if (!in(root, processed) || vertices_list.empty())
            {
                gameover = true;
            }
        }
        gameover = false;
        neighbors = graph.adjList(root);
        while (!gameover)
        {
            seek = Centrality::root_selection2(Centrality::closeness_centrality_list(neighbors, graph));
            if (!in(seek, processed))
            {
                gameover = true;
            }
            else
            {
                int index = get_index(seek, neighbors);
                neighbors.erase(neighbors.begin() + index);
                if (neighbors.empty())
                    break;
            }
        }
        if (neighbors.empty())
            break;
    }
}

/*
    SEARCH FOR MANY CYCLES
*/
void search_for_induced_cycles_for_M2_revision1(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph)
{
    int initial_max_cycle = cycle_size;

    std::vector<std::pair<int, float>> centrality;
    std::vector<int> neighbors;

    std::vector<int> processed;     // Processed vertices
    std::vector<int> vertices_list; // Vertices to be processed
    for (int i = 0; i < graph.get_qty_vertex(); i++)
    { // inicializa a lista
        vertices_list.push_back(i);
    }
    std::vector<int> cycle(1, 0); // create and initialize an auxiliary list

    int max_size_cycle = 0; // Define Max size list found  // Method 1 != Method 2

    while (true)
    {
        DEBUG std::cerr << "Searching for (" << cycle_size << ") cycle size" << std::endl;
        cycle = OpBasic::cycle(graph, cycle_size, seek, root);
        if (cycle.empty())
        {
            if (cycle_size > 3 && cycle_size > max_size_cycle)
            { // Não achei ciclo do tamanho pré-definido, procuro um menor
                cycle_size--;
                continue;
            }
            else
            { // I didn't even find a triangle, then I quit the loop
                break;
            }
        }
        else
        {
            cycle_size = initial_max_cycle;
        }

        for (int j = 0; j < cycle.size(); j++)
        { // Delete processed vertices
            int index = get_index(cycle[j], vertices_list);
            if (!vertices_list.empty() && index < vertices_list.size())
                vertices_list.erase(vertices_list.begin() + index);
            else if (vertices_list.empty())
                break;
        }
        cycle.push_back(root);
        processed.push_back(root);

        int acme = cycle.size() - 1; // auxiliary var
        if (!cycle.empty() && acme > max_size_cycle)
        {                                   // Method 1 != Method 2
            select_cycles.clear();          // Only cycles that has same size(maximum select)
            select_cycles.push_back(cycle); // will be selected
            if (cycle_size > max_size_cycle)
                max_size_cycle = acme;
        }
        else if (!cycle.empty() && acme == max_size_cycle)
        {
            select_cycles.push_back(cycle);
        }

        if (vertices_list.empty())
            break;

        bool gameover = false;
        while (!gameover)
        { // Escolhe nova raiz
            root = Centrality::tiebreaker(vertices_list, vertices_closeness, vertices_leverage);
            if (!in(root, processed) || vertices_list.empty())
            {
                gameover = true;
            }
        }
        gameover = false;
        neighbors = graph.adjList(root);
        while (!gameover)
        {
            seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage);
            if (!in(seek, processed))
            {
                gameover = true;
            }
            else
            {
                int index = get_index(seek, neighbors);
                neighbors.erase(neighbors.begin() + index);
                if (neighbors.empty())
                    break;
            }
        }
        if (neighbors.empty())
            break;
    }
}

/*
    SEARCH FOR ONE CYCLE - LIKE ADVISORS SUGGEST (for that time)
*/
void search_for_induced_cycles_for_M2_revision1_only_one(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph)
{
    int initial_max_cycle = cycle_size;

    std::vector<std::pair<int, float>> centrality;
    std::vector<int> neighbors;

    std::vector<int> processed;     // Processed vertices
    std::vector<int> vertices_list; // Vertices to be processed
    for (int i = 0; i < graph.get_qty_vertex(); i++)
    { // inicializa a lista
        vertices_list.push_back(i);
    }
    std::vector<int> cycle(1, 0); // create and initialize an auxiliary list

    int max_size_cycle = 0; // Define Max size list found  // Method 1 != Method 2

    while (true)
    {
        DEBUG std::cerr << "Searching for (" << cycle_size << ") cycle size" << std::endl;
        cycle = OpBasic::cycle(graph, cycle_size, seek, root);
        if (cycle.empty())
        {
            if (cycle_size > 3 && cycle_size > max_size_cycle)
            { // Não achei ciclo do tamanho pré-definido, procuro um menor
                cycle_size--;
                continue;
            }
            else
            { // I didn't even find a triangle, then I quit the loop
                break;
            }
        }
        else
        {
            select_cycles.push_back(cycle);
            break;
        }
    }
}

/* ****************************************************************
    SEARCH FOR MANY CYCLES --- CHANGE LOGIC
**************************************************************** */
void search_for_induced_cycles_for_M4v2(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph)
{
    int initial_max_cycle = cycle_size;

    std::vector<std::pair<int, float>> centrality;
    std::vector<int> neighbors;
    std::vector<int> processed;     // Processed vertices
    std::vector<int> vertices_list; // Vertices to be processed

    std::vector<int> vertices(vertices_closeness.size(), 0);
    for (int i = 0; i < vertices_closeness.size(); i++)
        vertices_closeness[i] = 0;

    for (int i = 0; i < graph.get_qty_vertex(); i++)
    { // inicializa a lista
        vertices_list.push_back(i);
    }
    std::vector<int> cycle(1, 0); // create and initialize an auxiliary list

    int max_size_cycle = 0; // Define Max size list found  // Method 1 != Method 2

    int cycles_found = 0;
    std::vector<int> neighbor_root = graph.adjList(root);
    int counter = 0;
    seek = neighbor_root[counter];

    while (true)
    {
        DEBUG std::cerr << "Searching for (" << cycle_size << ") cycle size" << std::endl;
        cycle = OpBasic::cycle(graph, cycle_size, seek, root);
        if (cycle.empty())
        {
            counter++;
            if (counter >= neighbor_root.size())
            {
                counter = 0;
                root++;
                if (root >= graph.get_num_vertices())
                {
                    root = 0;
                }
                continue;
            }
            seek = neighbor_root[counter];
            continue;
        }
        // cycle=OpBasic::cycle(graph, cycle_size);
        for (int j = 0; j < cycle.size(); j++)
        { // Delete processed vertices
            int index = get_index(cycle[j], vertices_list);
            if (!vertices_list.empty() && index < vertices_list.size())
                vertices_list.erase(vertices_list.begin() + index);
            else if (vertices_list.empty())
                break;
        }
        cycle.push_back(root);
        processed.push_back(root);

        int acme = cycle.size() - 1; // auxiliary var
        if (!cycle.empty() && acme > max_size_cycle)
        {                                   // Method 1 != Method 2
            select_cycles.clear();          // Only cycles that has same size(maximum select)
            select_cycles.push_back(cycle); // will be selected
            if (cycle_size > max_size_cycle)
                max_size_cycle = acme;
        }
        else if (!cycle.empty() && acme == max_size_cycle)
        {
            select_cycles.push_back(cycle);
        }

        if (vertices_list.empty())
            break;

        bool gameover = false;
        while (!gameover)
        { // Escolhe nova raiz
            root = Centrality::tiebreaker(vertices_list, vertices_closeness, vertices_leverage);

            if (!in(root, processed) || vertices_list.empty())
            {
                gameover = true;
            }
            else if (in(root, processed))
            {

                for (auto it = vertices_list.begin(); it != vertices_list.end();)
                {
                    if (*it == root)
                    {
                        it = vertices_list.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
        gameover = false;
        neighbors = graph.adjList(root);
        while (!gameover)
        {
            seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage);
            if (!in(seek, processed))
            {
                gameover = true;
            }
            else
            {
                int index = get_index(seek, neighbors);
                neighbors.erase(neighbors.begin() + index);
                if (neighbors.empty())
                    break;
            }
        }
        if (neighbors.empty() || cycles_found >= (global_induced_cycle - 1))
            break;
        cycles_found++;

        // For new cycle, select new root
        root++;
        if (root >= graph.get_num_vertices())
        {
            root = 0;
        }
        neighbor_root = graph.adjList(root);
        counter = 0;
        seek = neighbor_root[counter];
    }
}

/*
    SEARCH FOR MANY CYCLES
*/
void search_for_induced_cycles_for_M4(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph)
{
    int initial_max_cycle = cycle_size;

    std::vector<std::pair<int, float>> centrality;
    std::vector<int> neighbors;

    std::vector<int> processed;     // Processed vertices
    std::vector<int> vertices_list; // Vertices to be processed
    for (int i = 0; i < graph.get_qty_vertex(); i++)
    { // inicializa a lista
        vertices_list.push_back(i);
    }
    std::vector<int> cycle(1, 0); // create and initialize an auxiliary list

    int max_size_cycle = 0; // Define Max size list found  // Method 1 != Method 2

    int cycles_found = 0;
    std::vector<int> neighbor_root = graph.adjList(root);
    int counter = 0;
    seek = neighbor_root[counter];

    while (true)
    {
        DEBUG std::cerr << "Searching for (" << cycle_size << ") cycle size" << std::endl;
        cycle = OpBasic::cycle(graph, cycle_size, seek, root);
        if (cycle.empty())
        {
            counter++;
            if (counter >= neighbor_root.size())
            {
                counter = 0;
                root++;
                if (root >= graph.get_num_vertices())
                {
                    root = 0;
                }
                continue;
            }
            seek = neighbor_root[counter];
            continue;
        }
        // cycle=OpBasic::cycle(graph, cycle_size);
        for (int j = 0; j < cycle.size(); j++)
        { // Delete processed vertices
            int index = get_index(cycle[j], vertices_list);
            if (!vertices_list.empty() && index < vertices_list.size())
                vertices_list.erase(vertices_list.begin() + index);
            else if (vertices_list.empty())
                break;
        }
        cycle.push_back(root);
        processed.push_back(root);

        int acme = cycle.size() - 1; // auxiliary var
        if (!cycle.empty() && acme > max_size_cycle)
        {                                   // Method 1 != Method 2
            select_cycles.clear();          // Only cycles that has same size(maximum select)
            select_cycles.push_back(cycle); // will be selected
            if (cycle_size > max_size_cycle)
                max_size_cycle = acme;
        }
        else if (!cycle.empty() && acme == max_size_cycle)
        {
            select_cycles.push_back(cycle);
        }

        if (vertices_list.empty())
            break;

        bool gameover = false;
        while (!gameover)
        { // Escolhe nova raiz
            root = Centrality::tiebreaker(vertices_list, vertices_closeness, vertices_leverage);

            if (!in(root, processed) || vertices_list.empty())
            {
                gameover = true;
            }
            else if (in(root, processed))
            {

                for (auto it = vertices_list.begin(); it != vertices_list.end();)
                {
                    if (*it == root)
                    {
                        it = vertices_list.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
        gameover = false;
        neighbors = graph.adjList(root);
        while (!gameover)
        {
            seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage);
            if (!in(seek, processed))
            {
                gameover = true;
            }
            else
            {
                int index = get_index(seek, neighbors);
                neighbors.erase(neighbors.begin() + index);
                if (neighbors.empty())
                    break;
            }
        }
        if (neighbors.empty() || cycles_found >= (global_induced_cycle - 1))
            break;
        cycles_found++;

        // For new cycle, select new root
        root++;
        if (root >= graph.get_num_vertices())
        {
            root = 0;
        }
        neighbor_root = graph.adjList(root);
        counter = 0;
        seek = neighbor_root[counter];
    }
}

/**
 * @brief Build a edges' list
 * @details From a vertices' list that belongs a induced cycle build a list with pairs of vertices that represents edges of induced cycle.
 * Example:  input = {1,2,3,4,5,1} output = {(1,2), (2,3), (3,4), (4,5), (5,1)}
 * @author Carlos Thadeu
 * @param cycle_vertices_list a vector that contains a list of vertices that belongs a cycle.
 * @return a vector of vector of integers that represents the induced cycles found
 */
std::vector<std::pair<int, int>> make_edges_list(std::vector<int> cycle_vertices_list)
{
    int size = cycle_vertices_list.size();
    std::vector<std::pair<int, int>> edges_list;
    for (int i = 0; i < size - 1; i++)
    {
        edges_list.push_back(std::make_pair(cycle_vertices_list[i], cycle_vertices_list[i + 1]));
    }
    return edges_list;
}

/**
 * @brief Build a edges' list
 * @details From a vertices' list that belongs a induced cycle build a list with pairs of vertices that represents edges of induced cycle.
 * Example:  input = {{1,2,3,1},{4,5,1,4}} output = {(1,2), (2,3), (3,1), (4,5), (5,1), (1,4)}
 * @author Carlos Thadeu
 * @param cycle_vertices_list a vector that contains a list of vertices that belongs a cycle.
 * @return a vector of vector of integers that represents the induced cycles found
 */
std::vector<std::vector<std::pair<int, int>>> make_edges_list(std::vector<std::vector<int>> vector1)
{
    int size = vector1.size();
    std::vector<std::vector<std::pair<int, int>>> edges_list;
    std::vector<std::pair<int, int>> auxiliary;
    std::pair<int, int> edge;
    for (auto it = vector1.begin(); it != vector1.cend(); ++it)
    {
        for (auto itt = it->begin(); itt != it->cend() - 1; itt++)
        {
            edge = std::make_pair(*(itt), *(itt + 1));
            auxiliary.push_back(edge);
        }
        edges_list.push_back(auxiliary);
        auxiliary.clear();
    }
    return edges_list;
}

/**
 * @brief Build a edges' list
 * @details From a vertices' list that belongs a induced cycle build a list with pairs of vertices that represents edges of induced cycle.
 * Example:  input = {{(1,2),(3,4),(5,1)},{{(3,2),(3,5),(5,4)}} output = {(1,2),(3,4),(5,1),(3,2),(3,5),(5,4)}
 * @author Carlos Thadeu
 * @param cycle_vertices_list a vector that contains a list of vertices that belongs a cycle.
 * @return a vector of vector of integers that represents the induced cycles found
 */
std::vector<std::pair<int, int>> make_edges_list(std::vector<std::vector<std::pair<int, int>>> cycle_vertices_list)
{
    std::vector<std::pair<int, int>> result;
    for (auto it = cycle_vertices_list.begin(); it != cycle_vertices_list.cend(); ++it)
    {
        for (auto itt = it->begin(); itt != it->cend(); ++itt)
        {
            if (!(in(*itt, result)))
                result.push_back(*itt);
        }
    }
    return result;
}

/**
 * @brief Get for cycles
 * @details Get for cycles
 * @author Carlos Thadeu
 * @param edges edges
 * @param cycle_lenght size of cycle
 */
std::vector<std::vector<std::pair<int, int>>> cycle_combinations(std::vector<std::pair<int, int>> edges, int cycle_length)
{

    std::vector<std::vector<std::pair<int, int>>> result;

    std::function<void(int, std::vector<std::pair<int, int>>)> backtrack = [&](int start, std::vector<std::pair<int, int>> current_cycle)
    {
        if (current_cycle.size() == cycle_length)
        {
            result.push_back(current_cycle);
        }
        else
        {
            for (auto edge : edges)
            {
                if (edge.first == start && find(current_cycle.begin(), current_cycle.end(), edge) == current_cycle.end())
                {
                    current_cycle.push_back(edge);
                    backtrack(edge.second, current_cycle);
                    current_cycle.pop_back();
                }
            }
        }
    };

    for (int i = 0; i < edges.size(); i++)
    {
        std::vector<std::pair<int, int>> current_cycle = {edges[i]};
        backtrack(edges[i].second, current_cycle);
    }

    return result;
}