#include <thread>      // std::thread
#include <mutex>       // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#include <initializer_list>
#include <cmath>
#include <iostream>
#include <tuple> // for tuple
#include <string>
#include <chrono> // contributor AZ
#include <stack>

#include "../my_libs/library1.hpp"
#include "../my_libs/library3.hpp"
#include "graph.hpp"
#include "opBasic.hpp"
#include "stretch.hpp"
#include "cycles.hpp"
#include "centrality.hpp"

#include "../Debug.h"

//extern sem_t semaforo;
extern std::mutex mtx;

extern int global_total_arv;
extern int num_threads;
extern int used_threads;
extern int global_induced_cycle;
extern int global_induced_cycle_used;

extern bool global_noindex;
extern bool global_save_tree;
extern bool abort_for_timeout;

extern std::queue<std::vector<Valid_Edges_r>> QUEUE_VECTOR;

extern pthread_mutex_t mutex_signal;
extern sem_t semaforo;

/**
 * @brief Create threads to calculate stretch index for induced cycles (method 4)
 * @details Create threads to calculate the stretch index using induced cycles get from girth
 * @details Create threads from combinations (n,1) until (n,n)
 * @author Carlos Thadeu
 * @param graph a graph that represents the graph
 */
void create_threads_induced_cycle_V2(Graph &graph)
{
    int qty = 0;
    int root = -1;

    std::vector<std::vector<std::pair<int, int>>> edges_list_cycles;
    std::vector<std::pair<int, int>> edges_to_be_processed;
    std::vector<std::vector<int>> combinacoes;

    Valid_Edges_r edges_to_threads;

    DEBUG std::cerr << "Searching for induced cycles using method 4!" << std::endl;
    edges_list_cycles = seeking_induced_cycles_edges_v4(graph);
    global_induced_cycle_used = edges_list_cycles.size();
    edges_to_be_processed = make_edges_list(edges_list_cycles);
    combinacoes = generator_combinations(edges_to_be_processed.size());

    DEBUG std::cerr << "Pre-processing edges' list!" << std::endl;
    edges_to_threads = detect_valid_edges_M4_NEW_APPROACH(combinacoes, edges_to_be_processed, graph);

    qty = edges_to_threads.immutable_edges_list.size();

    DEBUG std::cerr << "*Edges numbers to use!" << qty << std::endl;
    DEBUG std::cerr << "*Threads proposed: " << num_threads << std::endl;

    // AGRUPA ARESTAS EM UM VETOR DE VETORES (NOMEADO PILHA , a ser mudado)
    int tasks = edges_to_threads.immutable_edges_list.size();
    used_threads = tasks;
    DEBUG std::cerr << "*Tasks to allocated: " << used_threads << std::endl;

    std::queue<Valid_Edges_r> edges_queue;
    
    for (int i =0 ; i < tasks; i++)
    {
        Valid_Edges_r aux;
        aux.disposable_edges_list.push_back(edges_to_threads.disposable_edges_list[i]);
        aux.immutable_edges_list.push_back(edges_to_threads.immutable_edges_list[i]);
        edges_queue.push(aux);
    } // FIM DO AGRUPA ARESTAS
    
    std::thread vetor_th[num_threads];

    for (int i = 0; i < num_threads; ++i)
    {
        std::vector<Valid_Edges_r> EDGES_AUX;
        vetor_th[i] = std::thread(create_threadV4_auxiliary_V2, i, std::ref(edges_queue), std::ref(graph));
    
    }

    for (int i = 0; i < num_threads; ++i)
    {
        vetor_th[i].join(); // junção das threads
    }
}

void create_threadV4_auxiliary_V2(int id, std::queue<Valid_Edges_r> &QUEUE_VECTOR, Graph &graph)
{
    int task = 0;
    Valid_Edges_r edges_to_thread;
    bool gameover = false;
    while (!gameover){
        mtx.lock();
        if (QUEUE_VECTOR.empty())
        {
            gameover = true;
            mtx.unlock();
        } else
        {
            //mtx.lock();
            edges_to_thread = QUEUE_VECTOR.front();
            QUEUE_VECTOR.pop();
            mtx.unlock();
            find_index_induced_cycle_V2( id , edges_to_thread, graph, task);
            task++;
        }
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

void find_index_induced_cycle_V2(int id,  Valid_Edges_r &edges_to_threads , Graph &graph, int task)
{

    int root = 0;
    Graph G1 = graph;   // Auxiliary graph - local graph

    for (auto edge: edges_to_threads.disposable_edges_list[0]){
        G1.remove_aresta(edge.first, edge.second);
    }
    G1.reset_trees(0);
    
    int num_vertices = G1.get_num_vertices();
    Graph tree(num_vertices);
       
    find_index_cycle_V2(id, root, edges_to_threads, G1, graph, task);

    int arvores;
    arvores = G1.get_total_tree();
    int index_local = G1.get_stretch_index();

    if (index_local == (int)INFINITY)
    {
        index_local = 1;
        arvores = 0;
        G1.reset_trees(arvores);
    }
    mtx.lock();
    DEBUG std::cerr << "THREAD " << id << "NA  TASK " << task << " criou " << arvores << " arvores, e encontrou index " << index_local << std::endl;
    graph.sum_trees(arvores);
    global_total_arv = arvores;
    int arv = 0; // Insert to mantain compatibility with set_graph_final_parameters -- will be modified when refactoring
    set_graph_final_parameters(index_local, G1, graph);
    mtx.unlock();
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

void find_index_cycle_V2(int id, int root, Valid_Edges_r &edges_to_threads, Graph &G1, Graph &graph, int task)
{
    
    int vertex_v = 0;
    int vertex_u = 0;
    int index_local = (int)INFINITY;
    int lower_limit = (int)INFINITY;

    std::vector<int> idx_next_neighbor(G1.get_num_vertices(), 0);
    std::vector<int> last_neighbor(G1.get_num_vertices(), -1);

    G1.reset_trees(0);

    Graph tree(G1.getQtdVertices());
    Graph tree_local(G1.getQtdVertices());

    lower_limit = graph.grt - 1;

    //std::string fileName = std::to_string(id) + "saida.txt";
    //std::string str_tree;

    int num_fixed_edges = edges_to_threads.immutable_edges_list[0].size();

    for (auto edge : edges_to_threads.immutable_edges_list[0])
    {
        G1.remove_aresta(edge.first, edge.second);
    }

    int nvertices = G1.get_num_vertices();
    int marestas = G1.get_num_edges();

    int start = 0;
    std::vector<int> indice(nvertices, 0);
    int j = 0;

    // Make edge's list

    std::vector<std::pair<int, int>> edges_list ;
    for (int a = 0; a < G1.get_num_vertices(); a++)
    {
        for (auto b : G1.adjList(a))
            if (b > a){
                edges_list.push_back(std::make_pair(a, b));
            }
    }

    int limit_of_zero_index = marestas - (nvertices - num_fixed_edges - 1 - 1);

    // Which are the immutable vertices?
    // This set is used to seek cycles to start from the specific vertex
    // 
    std::set<int> immutable_vertices_set;
    for (auto edge : edges_to_threads.immutable_edges_list[0])
    {
        immutable_vertices_set.insert(edge.first);
        immutable_vertices_set.insert(edge.second);
    }
    
    while(indice[0] < start + limit_of_zero_index && graph.get_signal() and !(abort_for_timeout))
    {
        if ( indice[j] > limit_of_zero_index - 1 + j )
        {
            --j;
            tree.remove_aresta(edges_list[indice[j]].first, edges_list[indice[j]].second);
            indice[j]++;
        }
        else
        {
            tree.add_aresta(edges_list[indice[j]].first, edges_list[indice[j]].second);
            bool has_cycle_var = false;
            if (OpBasic::cyclic(tree, edges_list[indice[j]].first)){
                has_cycle_var = true;
            }
            if (!has_cycle_var)
                if (OpBasic::cyclic(tree, edges_list[indice[j]].first))
                    has_cycle_var = true;
            if (!has_cycle_var)
            {
                if (j == nvertices - 1 - num_fixed_edges - 1 ) // Find spanner tree
                {
                        for (auto edge : edges_to_threads.immutable_edges_list[0])
                        {
                           tree.add_aresta(edge.first, edge.second);
                        }
                        bool has_cycle_var = false;
                        for (auto vertex : immutable_vertices_set)
                        {
                            if (OpBasic::cyclic(tree, vertex)){
                                has_cycle_var = true;
                                break;
                            }
                        }
                        if ( !has_cycle_var)
                        {
                            int f = 1;

                            if (!global_noindex)
                            { // LF request - only sum tree
                                f = Stretch::find_factor(graph, tree);
                            }
                            G1.add_tree();  // sum total trees

                            // if (global_save_tree)                   // REMOVER - APENAS PARA TESTE
                            // {                                       // REMOVER - APENAS PARA TESTE
                            //     str_tree = tree_to_string(tree);    // REMOVER - APENAS PARA TESTE
                            //     str_tree = ">" + str_tree;          // REMOVER - APENAS PARA TESTE
                            //     save_tree(str_tree, fileName);      // REMOVER - APENAS PARA TESTE
                            // }                                       // REMOVER - APENAS PARA TESTE

                            if (f < index_local)    // Compute stretch factor & stretch index
                            {
                                index_local = f;
                                tree_local = tree;
                                if (index_local == lower_limit)
                                {
                                    mtx.lock();
                                    graph.set_signal();
                                    mtx.unlock();
                                    break;
                                }
                            }
                        }
                        
                        for (auto edge : edges_to_threads.immutable_edges_list[0])
                        {
                           tree.remove_aresta(edge.first, edge.second);
                        }
                }
                else
                {
                    int next = j + 1;
                    indice[next] = indice[j] + 1;
                    j = next;
                    continue;
                }
            }
            tree.remove_aresta(edges_list[indice[j]].first, edges_list[indice[j]].second);
            indice[j]++;
        }
    }

    for (auto edge : edges_to_threads.immutable_edges_list[0])
    {
        tree.add_aresta(edge.first, edge.second);
    }

    DEBUG std::cerr << "THREAD: " << id << " JOB " << task << "  ACHOU " << index_local << std::endl;
    G1.set_stretch_index(index_local); // *********************************
    G1.set_best_tree(tree);
}

std::vector<std::vector<int>> generator_combinations(int nelements){
    std::vector<std::vector<int>> combinacoes;
    for (int i = nelements - 1; i >= 0; i--)
    {
        std::vector<std::vector<int>> edges_to_remove = combinatorics(nelements, i);
        for (int j = 0; j < edges_to_remove.size(); j++)
        {
            combinacoes.push_back(edges_to_remove[j]);
        }
    } // End of generate the combinations
    return combinacoes;
}

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
 * @brief Calculate block size and chunk size for threads for the M4 cycle method
 * @details block size and chunk size for threads for the M4 cycle method
 * @author Carlos Thadeu
 * @param num_threads a integer that represents the numbers of threads proposed
 * @param num_elements a integer that represents the numbers of the edges' combinations
 * @return a tuple of integers with block size and chunk size found and the number of threads selected to be used , the quantity of blocks and chunk
 */
Blocks_r define_block_chuck_for_cycleM4v2_NEW_APPROACH(int &num_threads, int &num_elements)
{
    Blocks_r ret;
    int block_size = 0;
    int chunk_size = 0;
    int qty_block = 0;
    int qty_chunk = 0;
    int used_threads1 = num_threads;

    if (num_threads >= num_elements)
    {
        used_threads1 = num_elements;
        qty_block = num_elements;
        block_size = 1;
        qty_chunk = 0;
        chunk_size = 0;
    }
    else if (num_elements % num_threads == 0)
    {
        used_threads1 = num_threads;
        qty_block = num_threads;
        block_size = num_elements / num_threads;
        qty_chunk = 0;
        chunk_size = 0;
    }
    else
    {
        used_threads1 = num_threads;

        int dividendo = num_elements;
        int divisor = num_threads;
        int quociente = floor(dividendo / divisor);
        int resto = dividendo % divisor;
        if (resto !=0 ) quociente++;
        
        while (quociente * (qty_block+1) < num_elements){
            qty_block++;
        }
        qty_chunk = num_threads - qty_block;
        block_size = quociente;
        chunk_size = num_elements - block_size * qty_block;
    }
    ret.block_size = block_size;
    ret.chunk_size = chunk_size;
    ret.num_threads = used_threads1;
    ret.qty_block = qty_block;
    ret.qty_chunk = qty_chunk;

    return ret;
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

// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED
// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED
// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED
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

std::vector<int> complement(const std::vector<int> &setA, const std::vector<int> &setB) {
    std::vector<int> complementSet;

    // Sort set B for efficient searching
    std::vector<int> sortedSetB(setB);
    std::sort(sortedSetB.begin(), sortedSetB.end());

    // Iterate over set A and check if each element is present in set B
    for (const int &element : setA) {
        if (!std::binary_search(sortedSetB.begin(), sortedSetB.end(), element)) {
            complementSet.push_back(element);
        }
    }

    return complementSet;
}



/*
    Detect edges to be removed
*/
Valid_Edges_r detect_valid_edges_M4_NEW_APPROACH(std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int, int>> &edges_to_be_processed, Graph &graph)
{
    Valid_Edges_r ret;
    //std::vector<std::vector<std::pair<int, int>>> edges_list;
    std::vector<std::vector<std::pair<int, int>>> disposable_edges_list;
    std::vector<std::vector<std::pair<int, int>>> immutable_edges_list;

    Graph G1 = graph;
    std::vector<std::pair<int, int>> disposable_edges_auxiliary;
    std::vector<std::pair<int, int>> immutable_edge_auxiliary;

    std::vector<int> vector1(edges_to_be_processed.size(),0);
    for (int i=0; i < edges_to_be_processed.size(); i++)
        vector1[i]=i;
    std::vector<int> complementSet;

   for (auto combinacao : combinacoes){
        bool not_conet = false;

        complementSet = complement(vector1, combinacao);
        disposable_edges_auxiliary.clear();
        for (auto index: complementSet){
            int v1 = edges_to_be_processed[index].first;
            int v2 = edges_to_be_processed[index].second;
            G1.remove_aresta(v1, v2);
            disposable_edges_auxiliary.push_back(std::make_pair(v1, v2));
            if (!OpBasic::is_connected(G1)) {
                not_conet = true;
                G1 = graph;
                break;            
            }
        }
        if (not_conet) continue;
        for ( auto index : combinacao){
            int v1 = edges_to_be_processed[index].first;
            int v2 = edges_to_be_processed[index].second;
            immutable_edge_auxiliary.push_back(std::make_pair(v1, v2));
        }

        disposable_edges_list.push_back(disposable_edges_auxiliary);
        immutable_edges_list.push_back(immutable_edge_auxiliary);
        disposable_edges_auxiliary.clear();
        immutable_edge_auxiliary.clear();

        G1 = graph;

    }
    ret.immutable_edges_list = immutable_edges_list;
    ret.disposable_edges_list = disposable_edges_list;

    return ret;
}

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
    //int cycle_size = graph.waist();
    int cycle_size = OpBasic::girth(graph);
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

    //search_for_induced_cycles_for_M4(seek, root, cycle_size, select_cycles, vertices_closeness, vertices_leverage, graph);
    search_for_induced_cycles_for_M4_VERSAO2(root, cycle_size, select_cycles, vertices_closeness, vertices_leverage, graph);
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
 * @brief Build a edges' list from a graph
 * @details Return a edge's list from a graph
 * Example:  input = a Graph output = {(1,2), (2,3), (3,4), (4,5), (5,1)}
 * @author Carlos Thadeu
 * @param graph a graph
 * @return a vector of vector of integers that represents edges' list
 */
std::vector<std::pair<int, int>> make_edges_list(Graph graph)
{
    std::vector<std::pair<int, int>> edges_list;
    for (int vertex = 0; vertex < graph.get_num_vertices(); vertex++){
        for (auto neighbor : graph.adjList(vertex))
        {
            if (neighbor > vertex)
                edges_list.push_back(std::make_pair(vertex, neighbor));
        }
    }
    return edges_list;
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

/*
    SEARCH FOR MANY CYCLES
*/
void search_for_induced_cycles_for_M4_VERSAO2(int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph)
{
    // 
    // Seleciona os vizinhos do raiz
    
    
    std::vector<std::pair<int, float>> centrality(Centrality::leverage_centrality_list(vertices_leverage));
    Centrality::my_insertionSort(centrality);

    int seek(graph.adjList(root)[0]);

    // Remove root vertex
    // for (int i = 0; i < centrality.size(); ++i) {
    //     if (centrality[i].first == root){
    //         std::swap(centrality[i], centrality[centrality.size()]);
    //         break;
    //     };
    // };
    centrality.erase(centrality.begin());

    std::vector<int> cycle(1, 0); // create and initialize an auxiliary list with cycles found

    bool gameover = false;
    while (!gameover)
    {
        cycle = OpBasic::cycle(graph, cycle_size, seek, root);
        if (cycle.empty()){
            if (centrality.empty())
                break;
            root = centrality[0].first;
            seek = graph.adjList(root)[0];
            centrality.erase(centrality.begin());
            continue;
        } else {
            bool repeated = false;
            for (auto vectors: select_cycles){ // Checa se o ciclo já existe 
                int match = 0;
                int index = 0;
                for (auto vertex: vectors){
                    for (std::vector<int>::iterator itr = cycle.begin(); itr != cycle.end(); itr++) {
                        if (*itr == vertex)
                            match++;
                    }
                }
                if (match == vectors.size()){   // OPS! ciclo repetido
                //if (match != vectors.size()){   // OPS! ciclo repetido
                    if (centrality.empty())
                        break;
                    root = centrality[0].first;
                    seek = graph.adjList(root)[0];
                    centrality.erase(centrality.begin());
                    repeated = true;
                    break;
                }
            }
            if (repeated) continue;
            // Não repetiu o ciclo , segue o jogo
            cycle.push_back(cycle[0]);
            select_cycles.push_back(cycle);
            if (select_cycles.size() >= global_induced_cycle){
                gameover = true;
            } else {
                if (centrality.empty())
                        break;
                for (auto vertex: cycle){
                    for (std::vector<std::pair<int, float>>::iterator itr = centrality.begin(); itr != centrality.end();) {
                        if ((*itr).first == vertex) {
                            itr = centrality.erase(itr);
                        }
                        else
                        {
                            ++itr;
                        }
                    }
                }
            }
        }
    }
}