#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#include <initializer_list>
#include <cmath>
#include <iostream>
#include <tuple> // for tuple
#include <string>

#include "../my_libs/library1.hpp"
#include "../my_libs/library3.hpp"
#include "graph.hpp"
#include "opBasic.hpp"
#include "stretch.hpp"
#include "parallel.hpp"
#include "centrality.hpp"

#include "../Debug.h"

#include <chrono>	// contributor AZ

//#define INDUCED_CYCLE_SYZE_START 5

extern sem_t semaforo;
extern unsigned long long int global_total_arv;
extern std::mutex mtx;
extern int num_threads;
extern int used_threads;
extern bool global_noindex;
extern int global_induced_cycle;

extern bool abort_for_timeout;
extern pthread_mutex_t mutex_signal;

std::tuple <int, int, int> define_block_chuck(int &num_threads, int &num_elements){
    //DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;
    int block_size = 1;
    int chunk_size = 0;
    int used_threads1 = num_threads;
    if (used_threads1 >= num_elements){
        used_threads1 = num_elements;
    } else if (used_threads1 < num_elements){
        if (num_elements % used_threads1)
            block_size = floor(num_elements / used_threads1) + 1;
        else
            block_size = floor(num_elements/ used_threads1);
    }
    chunk_size = num_elements - block_size * (used_threads1 - 1);

    //DEBUG std::cerr << "Threads to allocated: " << used_threads1 << std::endl;
    return std::make_tuple(block_size, chunk_size, used_threads1);
}

/**
 * @brief Calculate block size and chunk size for threads for the max degree method
 * @details block size and chunk size for threads for the max degree method
 * @author Carlos Thadeu
 * @param num_threads a integer that represents the numbers of threads proposed
 * @param num_elements a integer that represents the numbers of the edges' combinations
 * @return a tuple of integers with block size and chunk size found and the number of threads selected to be used
 */
std::tuple <int, int, int> define_block_chuck_for_max_degree(int &num_threads, int &num_elements){
    //DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;
    int block_size = 1;
    int chunk_size = 0;
    int used_threads1 = num_threads;
    if (used_threads1 >= num_elements){
        used_threads1 = num_elements;
    } else if (used_threads1 < num_elements){
        // if (num_elements % used_threads1)
        //     block_size = floor(num_elements / used_threads1) + 1;
        // else
            block_size = floor(num_elements/ used_threads1);
    }
    chunk_size = num_elements - block_size * (used_threads1 - 1);

    //DEBUG std::cerr << "Threads to allocated: " << used_threads1 << std::endl;
    return std::make_tuple(block_size, chunk_size, used_threads1);
}

/** DEPRECATED DEPRECATED - WILL BE REMOVE AT FUTURE
 * @brief Calculate stretch index using articulations
 * @details Create thread to calculate the stretch index from articulations (if exists) 
 * @author Carlos Thadeu(adapter)
 * @param g a graph instance that represents the graph
 * @param raiz
 * @param start
 * @param end
 * @param id
 */
/*void find_index_articulation(Graph &graph, Graph &subgraph, int raiz, int start, int end, const int id)
{
    DEBUG std::cerr << "CALCULANDO POR ARTICULACAO - Thread : " << id << std::endl;

    sem_wait(&semaforo); 
    int n = subgraph.getQtdVertices(); 
    int m = subgraph.getQtdArestas();

    int prox_vizinho[n];
    int ult_colocado[n];
    int v = raiz;
    int u;
    int arv = 0; // debug
    int index_local = (int)INFINITY;
    Graph tree_local;

    Graph tree(n);

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;

    int grt = OpBasic::maxLowerCicle(subgraph);
    subgraph.grt = grt;
    int lower_limit = grt - 1;

    mtx.lock();
    int stretch_index = graph.get_stretch_index();
    mtx.unlock();

    //while((stretch_index > lower_limit || stretch_index == 1) && graph.get_signal() and !(abort_for_timeout))
    while(graph.get_signal() and !(abort_for_timeout))
    {
        if(v == raiz){
            if(prox_vizinho[v] == end){
                break; // Fim do algoritmo
            }
        }

        if( prox_vizinho[v] == subgraph.grau(v) ){
            prox_vizinho[v] = 0;
            v = subgraph.ant_vertex(v);
            tree.remove_aresta(v, ult_colocado[v]);
            ult_colocado[v] = -1;
        } else {
            u = subgraph.adjList(v)[prox_vizinho[v]];
            ++prox_vizinho[v];
            if( not tree.possui_aresta(v, u) ){
                tree.add_aresta(v, u);
                ult_colocado[v] = u;
                if(not OpBasic::is_cyclic(tree)){
                    if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                        int f=1;
                        if (!global_noindex)
                            f = Stretch::find_factor(subgraph, tree);
                        ++arv;
                        subgraph.add_tree();
                        
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            if(index_local == lower_limit){// alteracao LF
                              break;// alteracao LF
                            }// alteracao LF

                        }
                    }else{
                        v = subgraph.next_vertex(v);
                        continue;
                    }
                }
                tree.remove_aresta(v, u);
            }
        }
    }

    mtx.lock();
    int arvores;
    arvores = arv;

    if (index_local == (int)INFINITY){
        index_local = 1;
        arvores = 0;
    }
    DEBUG std::cerr << "thread " << id << " criou " << arv << " arvores, e encontrou index "<< index_local << std::endl;
    set_graph_final_parameters(index_local, global_total_arv, arv, tree_local, graph);
    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
} */

//! Calculate stretch index - parallel
/*!
    \param g a graph instance that represents the graph
    \param raiz 
    \param start 
    \param end
    \param id
*/
void find_index_parallel(Graph &g, int raiz, int start, int end, const int id)
{
    // Nos meus testes, mais de 4 threads fizeram este código,
    // comigo não aconteceu o mesmo que o aconteceu com o pesquisador Daniel.
    // As threads ficaram limitadas a quantidade de arestas entregues para processamento (natural)
    sem_wait(&semaforo); // Apenas 4 threads puderam fazer este código por vez (obs. pequisador Daniel)
    int n = g.getQtdVertices(); 
    int m = g.getQtdArestas();

    int prox_vizinho[n];
    int ult_colocado[n];
    int v = raiz;
    int u;
    int local_sum_trees = 0; 
    int index_local = std::numeric_limits<int>::max();
    Graph tree_local;
    Graph tree(n);

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;
    //mtx.lock();
    int stretch_index = g.get_stretch_index();
    int lower_limit = g.grt-1 ;
    //mtx.unlock();

    //while((stretch_index > lower_limit || stretch_index == 1) && g.get_signal() && !(abort_for_timeout))
    while(g.get_signal() && !(abort_for_timeout))
    {
        if(v == raiz){
            if(prox_vizinho[v] == end){
                break; // Fim do algoritmo
            }
        }

        if( prox_vizinho[v] == g.grau(v) ){
            prox_vizinho[v] = 0;
            v = g.ant_vertex(v);
            tree.remove_aresta(v, ult_colocado[v]);
            ult_colocado[v] = -1;
        } else {
            u = g.adjList(v)[prox_vizinho[v]];
            ++prox_vizinho[v];
            if( not tree.possui_aresta(v, u) ){
                tree.add_aresta(v, u);
                ult_colocado[v] = u;
                // LINES ABOVE IMPROVE RUNNING TIME
                // THEY REPLACE if(not OpBasic::is_cyclic(tree)){
                //
                bool has_cycle_var = false;
                if (OpBasic::cyclic(tree, v)){
                    has_cycle_var = true;
                }
                if (!has_cycle_var)
                    if (OpBasic::cyclic(tree, u))
                        has_cycle_var = true;
                if(not has_cycle_var) {
                // END OF IMPROVES
                
                //if(not OpBasic::is_cyclic(tree)){
                    if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                        int f=1;
                        if (!global_noindex){
                            f =Stretch::find_factor(g, tree);
                        }
                        ++local_sum_trees;
                        //mtx.lock();
                        //g.add_tree();
                        //mtx.unlock();
                        
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            if(index_local == lower_limit){// alteracao LF
                                mtx.lock(); // Alteração CT
                                g.set_signal(); // Alteração CT
                                mtx.unlock(); // Alteração CT
                              break;// alteracao LF
                            }// alteracao LF

                        }
                    }else{
                        v = g.next_vertex(v);
                        continue;
                    }
                }
                tree.remove_aresta(v, u);
            }
        }
    }

    mtx.lock();
    int arvores;
    arvores = local_sum_trees;

    if (index_local == std::numeric_limits<int>::max()){
        index_local = 1;
        arvores = 0;
    }
    g.sum_trees(arvores);
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;
    set_graph_final_parameters(index_local, global_total_arv, local_sum_trees, tree_local, g);

    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
}

void find_index_pararell_edge(Graph& g, std::vector<int> edges, int start, const int id)
{
    //sem_wait(&semaforo);

    int n = g.getQtdVertices();
    int m = g.getQtdArestas();
    // std::vector<int> edges = OpBasic::edges(g);
    int indice[n-1];
    int j = 0;
    indice[j] = start;
    Graph tree(n);
    Graph tree_local;
    int local_sum_trees = 0;
    //int index_local = INF_VALUE;
    int index_local = std::numeric_limits<int>::max();

    Graph gTeste(n);

    // Removido pq coloquei em cria threads
    //OpBasic op;
    //int grt = op.maxLowerCicle(g);

    for(int i = start; i < edges.size(); i += 2)
    {
        gTeste.add_aresta(edges[i], edges[i+1]);
    }
    if( OpBasic::is_connected(gTeste)){        
        if((g.get_stretch_index() > g.grt-1 || g.get_stretch_index()==1)) { //Começa a busca pelas árvores geradoras. // Alterado by thadeu
            while(indice[0] < start+2  && g.get_signal() and !(abort_for_timeout)){ //Update by thadeu
                if( indice[j]/2 > m-(n-1-j) ){
                    --j;
                    tree.remove_aresta(edges[indice[j]],edges[indice[j]+1]);
                    indice[j] += 2;
                }
                else {
                    tree.add_aresta(edges[indice[j]], edges[indice[j]+1]);
                    bool has_cycle_var = false;
                    if (OpBasic::cyclic(tree, edges[indice[j]])){
                        has_cycle_var = true;
                    }
                    if (!has_cycle_var)
                        if (OpBasic::cyclic(tree, edges[indice[j]+1]))
                            has_cycle_var = true;
                    if (!has_cycle_var){
                    //if( !OpBasic::is_cyclic(tree) ){
                        if(j == n-2){ // achou uma arvore geradora
                            int f=1;
                            if (!global_noindex) 
                                f = Stretch::find_factor(g, tree);
                            ++local_sum_trees;
                            if(f < index_local){
                                index_local = f;
                                tree_local = tree;

                                if (index_local == g.grt-1) {
                                    break;
                                }
                            }
                        }
                        else{
                            int next = j+1;
                            indice[next] = indice[j] + 2;
                            j = next;
                            continue; // Simula uma chamada recursiva
                        }
                    }
                    tree.remove_aresta(edges[indice[j]], edges[indice[j]+1]);
                    indice[j] += 2;
                }
            }
        }
    }
    
    int arvores;
    arvores=local_sum_trees;
    if (index_local == (int)INFINITY){
        index_local = 1;
        arvores = 0;
    }
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;
    mtx.lock();
    g.sum_trees(arvores);
    set_graph_final_parameters(index_local, global_total_arv, local_sum_trees, tree_local, g);
    mtx.unlock();
    //sem_post(&semaforo);
}

// void find_index_parallel_edgeV2(Graph& g, std::vector<int> edges, int start, const int id)
// {
//     sem_wait(&semaforo);

//     int n = g.getQtdVertices();
//     int m = g.getQtdArestas();
//     // std::vector<int> edges = OpBasic::edges(g);
//     int indice[n-1];
//     int j = 0;
//     indice[j] = start;
    
//     Graph tree(n);
//     Graph tree_local;
//     int arv = 0;
//     //int index_local = INF_VALUE;
//     int index_local = (int)INFINITY;

//     Graph gTeste(n);

//     // Removido pq coloquei em cria threads
//     //OpBasic op;
//     //int grt = op.maxLowerCicle(g);

//     for(int i = start; i < edges.size(); i += 2)
//     {
//         gTeste.add_aresta(edges[i], edges[i+1]);
//     }
//     if( OpBasic::is_connected(gTeste)){
//     //if( g.get_signal() && !(abort_for_timeout) ){

//         if((g.get_stretch_index() > g.grt-1 || g.get_stretch_index()==1)) { //Começa a busca pelas árvores geradoras. // Alterado by thadeu
//             while(indice[0] < start+2  && g.get_signal() and !(abort_for_timeout)){ //Update by thadeu
//                 if( indice[j]/2 > m-(n-1-j) ){
//                     --j;
//                     tree.remove_aresta(edges[indice[j]],edges[indice[j]+1]);
//                     indice[j] += 2;
//                 }
//                 else {
//                     tree.add_aresta(edges[indice[j]], edges[indice[j]+1]);
//                     if( !OpBasic::is_cyclic(tree) ){
//                         if(j == n-2){ // achou uma arvore geradora
//                             int f=1;
//                             if (!global_noindex)
//                                 f = Stretch::find_factor(g, tree);
//                             ++arv;
//                             mtx.lock();
//                             g.add_tree();
//                             mtx.unlock();
//                             if(f < index_local){
//                                 index_local = f;
//                                 tree_local = tree;

//                                 if (index_local == g.grt-1) {
//                                     break;
//                                 }
//                             }
//                         }
//                         else{
//                             int next = j+1;
//                             indice[next] = indice[j] + 2;
//                             j = next;
//                             continue; // Simula uma chamada recursiva
//                         }
//                     }
//                     tree.remove_aresta(edges[indice[j]], edges[indice[j]+1]);
//                     indice[j] += 2;
//                 }
//             }
//         }
//     }
    
//     mtx.lock();
//     int arvores;
//     arvores =arv;

//     if (index_local == (int)INFINITY){
//         index_local = 1;
//         arvores = 0;
//     }
//     DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;
//     set_graph_final_parameters(index_local, global_total_arv, arv, tree_local, g);
//     mtx.unlock();
//     sem_post(&semaforo);
// }

void create_threads(Graph& graph)
{
    int raiz = graph.highest_degree_vertex();
    int qtd = graph.grau(raiz);
    int id=0;

    std::thread vetor_th[qtd];

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    //OpBasic op; // by thadeu
    //graph.grt = op.maxLowerCicle(graph); // by thadeu
    // fim calcula grt

    if(graph.possui_aresta(raiz, graph.ant_vertex(raiz) ) ){
        id = adj_id(graph, raiz, graph.ant_vertex(raiz) );
    }

    DEBUG std::cerr << "Threads to be used: " << qtd << std::endl;
    used_threads = qtd;

    for(int i=0; i < used_threads; ++i){
        vetor_th[i] = std::thread(find_index_parallel, std::ref(graph), raiz, id, id+1, i);
        id = next(id, used_threads);
    }

    for(int i=0; i < used_threads; ++i){
        vetor_th[i].join();
    }
}


//void create_threadV2_auxiliary(Graph &graph, int raiz, int start, int end, const int id, std::tuple <int, int, int> &acme){
void create_threadV2_auxiliary(Graph &graph, int raiz, int start, int end, const int id){
    for (int i = start;i < end; i++ ){
        find_index_parallel(graph, raiz, start, end, id);
    }
}

/**
 * @brief Create threads to calculate stretch index for from max degree
 * @details Create threads to calculate the stretch index using max degree
 * @author Carlos Thadeu
 * @param graph a graph that represents the graph
 */
void create_threadsV2(Graph& graph)
{
    int raiz = graph.highest_degree_vertex();
    int qty = graph.grau(raiz);
    int id=0;

    graph.reset_trees(0);
    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    //OpBasic op; // by thadeu
    //graph.grt = op.maxLowerCicle(graph); // by thadeu
    // fim calcula grt

    if(graph.possui_aresta(raiz, graph.ant_vertex(raiz) ) ){
        id = adj_id(graph, raiz, graph.ant_vertex(raiz) );
    }

    auto acme = define_block_chuck(num_threads, qty);
    int block_size = std::get<0>(acme);
    int chunk_size = std::get<1>(acme);
    used_threads = std::get<2>(acme);

    std::thread vetor_th[used_threads];
    
    for(int i = 0; i < used_threads; ++i){
        //int start = i * block_size;
        //int end = (i != used_threads - 1) * (i+1) * block_size + (i == used_threads - 1) * start + chunk_size;
        //vetor_th[i] = std::thread(create_threadV2_auxiliary, std::ref(graph), raiz, start, end, i);
        vetor_th[i] = std::thread(create_threadV2_auxiliary, std::ref(graph), raiz, id, id+1, i);
    }

    sem_wait(&semaforo);

    for(int i = 0; i < used_threads; ++i){
        vetor_th[i].join();
    }
}

void create_threads_edge_max_degree(Graph& g)
{
    int qtd_th = g.maior_grau();
    //int qtd_th = num_threads;

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    //OpBasic op; // by thadeu
    //g.grt = op.maxLowerCicle(g); // by thadeu
    // fim calcula grt

    std::vector< std::thread> vetor_th(qtd_th);

    std::vector<int> edges = OpBasic::edges_by_bigger_degree(g);
    
    DEBUG std::cerr << "Threads to be used: " << qtd_th << std::endl;
    used_threads = qtd_th;

    for(int i=0; i < qtd_th; ++i){
        vetor_th[i] = std::thread(find_index_pararell_edge, std::ref(g), edges, i*2, i); // separação dos threats
    }

    for(int i=0; i < qtd_th; ++i){
        vetor_th[i].join(); // junção das threads
    }
}

// void create_threadV2_edge_max_degree_auxiliary(Graph &graph, std::vector<int> edges, int block_size, int id_th){
//     for (int i = 0; i < block_size; i = i + 2 ){
//         int start = block_size * id_th + i;
//         find_index_parallel_edgeV2(graph, edges, start, id_th);
//     }
// }

// void create_threads_edge_max_degreeV2(Graph& graph)
// {
//     int qtd_th = graph.maior_grau();
//     //int qtd_th = num_threads;

//     // Calcula atributo grt
//     // por enquanto fica aqui, no futuro retirar 
//     // pois o método create_thread nao é para calcular nada do grafo
//     OpBasic op; // by thadeu
//     //graph.grt = op.maxLowerCicle(graph); // by thadeu
//     // fim calcula grt

//     //std::vector< std::thread> vetor_th(qtd_th);

//     graph.reset_trees(0);

//     std::vector<int> edges = OpBasic::edges_by_bigger_degree(graph);
    
//     int qty = edges.size()/2;

//     auto acme = define_block_chuck_for_max_degree(num_threads,qty);
//     int block_size = std::get<0>(acme);
//     int chunk_size = std::get<1>(acme);
//     used_threads = std::get<2>(acme);

//     std::thread vetor_th[used_threads];

//     // Adjust if block size is odd, remember, edges are pairs (u,v)
//     if ((block_size % 2)){
//         block_size++;
//         chunk_size = qty - block_size * (used_threads - 1);
//     }

//     for(int i = 0; i < used_threads; ++i){
//         vetor_th[i] = std::thread(create_threadV2_edge_max_degree_auxiliary, std::ref(graph), edges, block_size, i);
//     }

//     sem_wait(&semaforo);

//     for(int i=0; i < used_threads; ++i){
//         vetor_th[i].join(); // junção das threads
//     }
// }

/**
 * @brief Create threads to calculate stretch index from articulations
 * @details Create thread to calculate the stretch index from articulations (if exists)
 * This procedure will seek from articulations points then split the graph from
 * this articulations and calculate the stretch index
 * 
 * @author Carlos Thadeu
 * @param g a graph that represents the graph
 */
/*void create_threads_articulations(Graph& g) {
    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    //OpBasic op; // by thadeu
    g.grt = OpBasic::maxLowerCicle(g); // by thadeu
    // fim calcula grt
    std::set<int> articulations;     
    std::vector<std::pair<int,int>> bridges;
    std::tie(articulations, bridges) = seek_articulations(g); // seek for articulations and bridges
    // Building subgraphs
    std::vector<std::vector<int>> subgraph;
    g.split_in_subgraphs(articulations, subgraph, g);

    int qtd = subgraph.size();
    DEBUG std::cerr << "Articulacoes detectadas: " << qtd << std::endl;
    used_threads = qtd + 1;
    std::thread vetor_th[qtd];

    int id = 0;
    Graph xpto[qtd];

    for (std::vector<int> sb : subgraph){
        xpto[id] = g.build_subgraph(sb);
        int root = xpto[id].highest_degree_vertex();
        int neighbor = xpto[id].adjList(root)[0];
        vetor_th[id] = std::thread(find_index_articulation, std::ref(g), std::ref(xpto[id]), root, root, neighbor, id);
        id++;
    }
    for(int i = 0; i < id; ++i){
        vetor_th[i].join();
    }
}*/