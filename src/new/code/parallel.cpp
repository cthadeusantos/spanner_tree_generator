#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#include <initializer_list>
#include <cmath>
#include <iostream>
#include <tuple> // for tuple
#include <string>

#include "graph.hpp"
#include "opBasic.hpp"
#include "stretch.hpp"
#include "parallel.hpp"
#include "centrality.hpp"

#include "../my_libs/library3.hpp"

#include "../Debug.h"

#include <chrono>	// contributor AZ

//#define INDUCED_CYCLE_SYZE_START 5

sem_t semaforo;
int total_arv = 0;
std::mutex mtx;
extern int num_threads;
extern int used_threads;
extern bool noindex;
extern int global_induced_cycle;

extern bool abort_for_timeout;
extern pthread_mutex_t mutex_signal;


/**
 * @brief Define immutable edges at induced cycle method M4 for a thread
 * @details Define immutable edges at induced cycle method M4
 * @author Carlos Thadeu
 */
std::vector<std::pair<int,int>> immovable(int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed){
    std::vector<std::pair<int,int>> immovable_edges;
    for (auto edge: edges_to_be_processed){
        bool insert = true;
        for (auto idx : combinacoes[id]){
                if ((edge.first == edges_to_be_processed[idx].first && edge.second == edges_to_be_processed[idx].second) ||
                    (edge.second == edges_to_be_processed[idx].first && edge.first == edges_to_be_processed[idx].second)){
                        insert = false;
                    }
        }
        if (insert){
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
void find_index_cycleM4_2(int id, int root, std::vector<std::pair<int,int>> &immovable_edges, Graph &G1, Graph &graph){
    
    int vertex_v = 0;
    int vertex_u = 0;
    int index_local = (int)INFINITY;
    int lower_limit = (int)INFINITY;

    //G1 = graph; //For original
    std::vector<int> idx_next_neighbor(G1.get_num_vertices(), 0);
    std::vector<int> last_neighbor(G1.get_num_vertices(), -1);

    G1.reset_trees(0);

    Graph tree(G1.getQtdVertices());
    Graph tree_local(G1.getQtdVertices());

    //DEBUG std::cerr << "id:" << id << " edges: " << G1.get_num_edges() << std::endl;

     for (auto edge: immovable_edges){ // for modify
        tree.add_aresta(edge.first, edge.second);// for modify
        //DEBUG std::cerr << std::endl << id << "EDGES " << edge.first << " -- " << edge.second << std::endl;
        
     }// for modify

    pthread_mutex_lock (&mutex_signal);
    lower_limit = graph.get_lower_limit();
    pthread_mutex_unlock (&mutex_signal);
    //int raiz = 0;
    //int end = G1.get_num_vertices();
    //idx_next_neighbor[vertex_v] = raiz;

    DEBUG std::cerr << id << "ORIGINAL "<< graph.getQtdArestas() << " QTE ARESTAS: " << G1.getQtdArestas() << std::endl;
    
    std::string fileName = std::to_string(id) + "saida.txt";
    std::string str_tree;

    while( graph.get_signal() && vertex_v >= 0 && !(abort_for_timeout)) {
        // if(vertex_v == raiz){
        //     if(idx_next_neighbor[vertex_v] == end){
        //         break; // Fim do algoritmo
        //     }
        // }
        if( idx_next_neighbor[vertex_v] == G1.grau(vertex_v) ) {
            idx_next_neighbor[vertex_v] = 0;
            //vertex_v = G1.ant_vertex(vertex_v);
            vertex_v--;
            if (vertex_v < 0) break;
            if (!in(last_neighbor[vertex_v], vertex_v, immovable_edges)){ // for modify
                tree.remove_aresta(vertex_v, last_neighbor[vertex_v]);
            } // for modify
            last_neighbor[vertex_v] = -1;
        } else {
            vertex_u = G1.adjList(vertex_v)[idx_next_neighbor[vertex_v]];
            ++idx_next_neighbor[vertex_v];
            if( not tree.possui_aresta(vertex_v, vertex_u) ){
                tree.add_aresta(vertex_v, vertex_u);
                last_neighbor[vertex_v] = vertex_u;
                if (not OpBasic::is_cyclic(tree)) {
                    if (tree.getQtdArestas() == tree.getQtdVertices()-1) {
                        int f=1;
                        if (!noindex){ // LF request - only sum tree
                            //pthread_mutex_lock (&mutex_signal);
                            f = find_factor(graph, tree);
                            //pthread_mutex_unlock (&mutex_signal);
                        }
                        G1.add_tree();
                        str_tree = tree_to_string(tree);
                        save_tree(str_tree, fileName);
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            if(index_local == lower_limit){
                                pthread_mutex_lock (&mutex_signal);
                                graph.set_signal();
                                pthread_mutex_unlock (&mutex_signal);
                              break;
                            }
                        }
                    } else {
                        //vertex_v = G1.next_vertex(vertex_v);
                        if (vertex_v < G1.get_num_vertices() - 1) vertex_v++;
                        continue;
                    }
                }
                if (!in(last_neighbor[vertex_v], vertex_v, immovable_edges)){
                    //tree.remove_aresta(vertex_v, last_neighbor[vertex_v]);
                    tree.remove_aresta(vertex_v, last_neighbor[vertex_v]);
                }
            } else { // ELSE OF IF HAS EDGE -- CHAVE 001
                // if (in(vertex_u, vertex_v, immovable_edges)) {
                //     last_neighbor[vertex_v] = vertex_u;
                // }
                // if (in(last_neighbor[vertex_v], vertex_v, immovable_edges)) { // for modify  - CHANGE 002
                if (in(vertex_u, vertex_v, immovable_edges)) { // for modify  - CHANGE 002
                    //DEBUG std::cerr << std::endl << id << "Inside " << vertex_v << " -- " << last_neighbor[vertex_v] << std::endl;
                    last_neighbor[vertex_v] = vertex_u;
                    if (not OpBasic::is_cyclic(tree)) {
                        if (tree.getQtdArestas() == tree.getQtdVertices()-1) {
                            //DEBUG std::cerr << "id:" << id << " TREE edges: " << tree.get_num_edges() << "  vertx" << tree.get_num_vertices()<< std::endl;
                            int f=1;
                            if (!noindex){ // LF request - only sum tree
                                //pthread_mutex_lock (&mutex_signal);
                                f = find_factor(graph, tree);
                                //pthread_mutex_unlock (&mutex_signal);
                            }
                            G1.add_tree();
                            str_tree = tree_to_string(tree);
                            save_tree(str_tree, fileName);
                            if(f < index_local){
                                index_local = f;
                                tree_local = tree;
                                if(index_local == lower_limit){
                                    pthread_mutex_lock (&mutex_signal);
                                    graph.set_signal();
                                    pthread_mutex_unlock (&mutex_signal);
                                break;
                                }
                            }
                        }
                    } //else {
                       //     ++idx_next_neighbor[vertex_v];
                         //   if (idx_next_neighbor[vertex_v] > G1.grau(vertex_v) ) --idx_next_neighbor[vertex_v];
                            //vertex_v = G1.next_vertex(vertex_v);
                    if (vertex_v < G1.get_num_vertices() - 1) vertex_v++;
                            //continue;
                    //}            
                } // for modify - CHANGE 002
            } //END OF IF HAS EDGE -- CHAVE 001
        } 
    } // END OF WHILE
    DEBUG std::cerr << "ID:"<< id << "ACABOU" << index_local << std::endl;
    G1.set_stretch_index(index_local); // *********************************
    G1.set_best_tree(tree);
    //pthread_mutex_lock (&mutex_signal);
    //pthread_mutex_unlock (&mutex_signal);
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
void find_index_cycleM4_1(int id, int root, std::vector<std::pair<int,int>> &immovable_edges, Graph &G1, Graph &graph){
    int vertex_v = 0;
    int vertex_u = 0;
    int index_local = (int)INFINITY;
    int lower_limit = (int)INFINITY;

    std::vector<int> idx_next_neighbor(G1.get_num_vertices(), 0);
    std::vector<int> last_neighbor(G1.get_num_vertices(), -1);
    G1.reset_trees(0);

    Graph tree(G1.getQtdVertices());
    Graph tree_local(G1.getQtdVertices());

    for (auto edge: immovable_edges){
        tree.add_aresta(edge.first, edge.second);
    }
    pthread_mutex_lock (&mutex_signal);
    lower_limit = graph.get_lower_limit();
    pthread_mutex_unlock (&mutex_signal);

    while( graph.get_signal() && vertex_v >= 0 && !(abort_for_timeout)) {
        if ( idx_next_neighbor[vertex_v] == G1.grau(vertex_v) ){
            idx_next_neighbor[vertex_v] = 0;
            --vertex_v;

            if ( vertex_v < 0){
                break;
            }

            if (!in(last_neighbor[vertex_v], vertex_v, immovable_edges)){
                tree.remove_aresta(vertex_v, last_neighbor[vertex_v]);
            }

            last_neighbor[vertex_v] = -1;

        } else {

            vertex_u = G1.adjList(vertex_v)[idx_next_neighbor[vertex_v]];
            ++idx_next_neighbor[vertex_v];

            if( not tree.possui_aresta(vertex_v, vertex_u) ){
                tree.add_aresta(vertex_v, vertex_u);
                last_neighbor[vertex_v] = vertex_u;
                if( not OpBasic::is_cyclic(tree)){

                    if (tree.get_num_edges() == (tree.get_num_vertices() - 1)){
                        int f = 1;

                        if (!noindex){ // LF request - only sum tree
                            pthread_mutex_lock (&mutex_signal);
                            f = find_factor(graph, tree);
                            pthread_mutex_unlock (&mutex_signal);
                        }

                        G1.add_tree();

                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            G1.set_stretch_index(index_local);

                            if( index_local == lower_limit ){
                                pthread_mutex_lock (&mutex_signal);
                                graph.set_signal();
                                pthread_mutex_unlock (&mutex_signal);
                                break;
                            }

                        }

                    } else {

                        if (vertex_v < (G1.get_num_vertices() - 1)){
                            ++vertex_v;
                        }
                        continue;

                    } 

                }

                if (!in(last_neighbor[vertex_v], vertex_v, immovable_edges)){
                    tree.remove_aresta(vertex_v, last_neighbor[vertex_v]);
                }


            }

        }

    }
    DEBUG std::cerr << "ID:"<< id << "ACABOU" << std::endl;
    G1.set_stretch_index(index_local); // *********************************
    G1.set_best_tree(tree);
    //pthread_mutex_lock (&mutex_signal);
    //pthread_mutex_unlock (&mutex_signal);
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
void find_index_cycleM4(int root, const std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &G1, Graph &graph){
    //int start = root;
    //int end = (root - 1) * (root!=0) + (G1.get_num_vertices() - 1) * (root==0);
    //DEBUG std::cerr << "(" << start << "," << end << ")";
    // ROOT vertex not used yet, start from vertex 0
    int vertex_v = 0;
    int vertex_u = 0;
    int grt = (int)INFINITY;
    int index_local = (int)INFINITY;
    int lower_limit = (int)INFINITY;

    std::vector<int> idx_next_neighbor(G1.get_num_vertices(), 0);
    std::vector<int> idx_last_neighbor(G1.get_num_vertices(), -1);
    G1.reset_trees(0);

    Graph tree(G1.getQtdVertices());
    Graph tree_local(G1.getQtdVertices());

    //pthread_mutex_lock (&mutex_signal);
    grt = graph.get_grt();
    //graph.set_lower_limit(grt-1);
    lower_limit = graph.get_lower_limit();
    //Graph G2 = graph;
    //pthread_mutex_unlock (&mutex_signal);
    //std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    while( graph.get_signal() && vertex_v >= 0 && !(abort_for_timeout)){
        if( idx_next_neighbor[vertex_v] == G1.grau(vertex_v) ){
            idx_next_neighbor[vertex_v] = 0;
            --vertex_v;
            if(vertex_v < 0){
                break; // stop the algorithm
            } 
            if (!in(idx_last_neighbor[vertex_v], vertex_v, edges_to_be_processed)){
                tree.remove_aresta(vertex_v, idx_last_neighbor[vertex_v]);
            }
            idx_last_neighbor[vertex_v] = -1;

        } else {
            vertex_u = G1.adjList(vertex_v)[idx_next_neighbor[vertex_v]];
            ++idx_next_neighbor[vertex_v];
            if( not tree.possui_aresta(vertex_v, vertex_u) ){
                tree.add_aresta(vertex_v, vertex_u);
                idx_last_neighbor[vertex_v] = vertex_u;
                
                if(not OpBasic::is_cyclic(tree)){
                    if(tree.get_num_edges() == (tree.get_num_vertices() - 1)){
                        int f = 1;
                        if (!noindex){ // LF request
                            //pthread_mutex_lock (&mutex_signal);
                            //f = find_factor(G2, tree);
                            //std::chrono::time_point<std::chrono::steady_clock> start1 = std::chrono::steady_clock::now();
                            f = find_factor(graph, tree);
                            //std::chrono::time_point<std::chrono::steady_clock>	end1 = std::chrono::steady_clock::now();	
	                        //std::chrono::duration<double> execution_duration1(end1 - start1);
                            //double lastExecutionTime1 = execution_duration1.count();
                            //std::cout << "[RUNNING_TIME]=" << lastExecutionTime1 <<  std::endl;
                            //pthread_mutex_unlock (&mutex_signal);
                        }
                        G1.add_tree();
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            G1.set_stretch_index(index_local);
                            DEBUG std::cerr << std::endl;
                            tree_local.show_best_tree();
                            if (tree.get_num_edges() < 19){
                                DEBUG std::cerr << "Menor que 19";
                            }
                            DEBUG std::cerr << "Edges: " << tree_local.get_num_edges() ;
                            DEBUG std::cerr << "Indices: " << index_local ;
                            DEBUG std::cerr << std::endl;
                            if( index_local == lower_limit ){
                                pthread_mutex_lock (&mutex_signal);
                                DEBUG std::cerr << "ATIVOU SIGNAL";
                                graph.set_signal();
                                pthread_mutex_unlock (&mutex_signal);
                                break;
                            }
                        }
                    } else {
                        if (vertex_v < (G1.get_num_vertices() - 1)){
                            ++vertex_v;
                        }
                        continue;
                    }
                } // End IF of check cycle
                if (!in(idx_last_neighbor[vertex_v], vertex_v, edges_to_be_processed)){
                    tree.remove_aresta(vertex_v, idx_last_neighbor[vertex_v]);
                }
            }
        }
    }
    //total_arv = arv;
    //std::cout << "ARVORES: " << G1.get_total_tree() << std::endl;
    G1.set_stretch_index(index_local); // *********************************
    G1.set_best_tree(tree);
    pthread_mutex_lock (&mutex_signal);
    graph.sum_trees(G1.get_total_tree());
    pthread_mutex_unlock (&mutex_signal);
    //return index;
}

void create_threadV4_auxiliary( int start, int end, const int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph){
    //int idx = (end - start) * id;
    int acme = end - start;
    //std::chrono::time_point<std::chrono::steady_clock> start1 = std::chrono::steady_clock::now();

    DEBUG std::cerr << "Start: " << start << "End " << end << std::endl;
    
    for (int i = 0; i < acme; i=i+1 ){
        //std::cout << std::endl<< "i" << i << "Start:" << start << "  End: " << end << " ACme:: "<< acme << " start+i:" << i+ start << std::endl;
        if ((i + start) > combinacoes.size() - 1) break;
        find_index_induced_cycle_method_4(i + start , combinacoes, edges_to_be_processed, graph);
    }
    //std::chrono::time_point<std::chrono::steady_clock>	end1 = std::chrono::steady_clock::now();	
    //std::chrono::duration<double> execution_duration1(end1 - start1);
    //double lastExecutionTime1 = execution_duration1.count();
    //std::cout << "Thread " << id << "Combinacoes: " << "Factor"<< graph.get_stretch_index() << combinacoes.size() << "[RUNNING_TIME]=" << lastExecutionTime1 <<  std::endl;
        //for (auto i: combinacoes[id]){
        //if (i != -1)
        //    std::cout << "Thread "<< id << "(" << edges_to_be_processed[i].first << "," << edges_to_be_processed[i].second << ")" << std::endl;
    //}
}

/**
 * @brief Create threads to calculate stretch index for induced cycles (method 4)
 * @details Create threads to calculate the stretch index using induced cycles get from girth
 * @details Create threads from combinations (n,1) until (n,n)
 * @author Carlos Thadeu
 * @param graph a graph that represents the graph
 */
void create_threads_induced_cycle_method_4v1(Graph &graph) {
	//int value = graph.get_stretch_index();
	//std::cout << "Valor achado: " << value << std::endl;
    int qty = 0;
    int root = -1;
    int neighbor = -1;
    int n;
    std::vector<std::vector<std::pair<int, int>>> edges_list ;

    DEBUG std::cerr << "Searching for induced cycles using method 4!"  << std::endl;
    edges_list = seeking_induced_cycles_edges_v4(graph);

    std::vector<std::pair<int,int>> edges_to_be_processed = make_edges_list(edges_list);
    DEBUG std::cerr << "Pre-processing edges!"  << std::endl;
    
    edges_to_be_processed=detect_valid_edges_M4(edges_to_be_processed, graph);

    int r;   // Generate the combinations
    std::vector<std::vector<int>> combinacoes;
    n=edges_to_be_processed.size();
    for (int i=n; i > 0; i--){
        std::vector<std::vector<int>> edges_to_remove = combinatorics(n, i);
        for (int j=0; j < edges_to_remove.size(); j++){
            combinacoes.push_back(edges_to_remove[j]);
        }
    }   // End of generate the combinations
    // ULTIMA COMBINACAO É O GRAFO SEM NENHUMA ARESTA REMOVIDA
    //std::vector<int> aux(1,-1);
    //combinacoes.push_back(aux);
    // FIM ULTIMA COMBINACAO É O GRAFO SEM NENHUMA ARESTA REMOVIDA
    qty = combinacoes.size();

    DEBUG std::cerr << "Edges numbers to use!" << qty << std::endl;
    DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;

    //auto acme = define_block_chuck_for_cycleM4(num_threads, qty);
    auto acme = define_block_chuck_for_cycleM4v2(num_threads, qty);
    int block_size = std::get<0>(acme);
    int chunk_size = std::get<1>(acme);
    used_threads = std::get<2>(acme);

    int qty_block = std::get<3>(acme);
    int qty_chunk = std::get<4>(acme);

    DEBUG std::cerr << "Threads to allocated: " << used_threads << std::endl;

    //std::cout << qty << " " << num_threads << " " << block_size << " " << chunk_size << " " << used_threads << " " << std::endl;

    std::thread vetor_th[used_threads];

    std::reverse(combinacoes.begin(), combinacoes.end());
    std::vector<std::vector<int>> auxiliar = combinacoes;
    // int num_comb = combinacoes.size();
    // int jump = num_comb / combinacoes[idx_last].size();

    int idx_last = combinacoes[qty - 1].size();
    
    int counter = 0;
    for (int idx_size = 0; idx_size < idx_last; idx_size++){
        for (int iyt = idx_size; iyt < qty; iyt = iyt + idx_last){
            //int g = idx_size + iyt ;
            if ( iyt < qty){
                auxiliar[counter]= combinacoes[iyt];
                counter++;
            }
        }
    }

    combinacoes = auxiliar;

    int start, end;
    
    for(int i = 0; i < used_threads; ++i){
                    // int start = i * block_size;
                    // int end = start + ((i != used_threads - 1) * block_size) + ((i == used_threads - 1) * chunk_size );
                    // if (end == start) end++;

        if (i < qty_block){
            start = i * block_size;
            end = start + block_size;
        } else {
            if (i==qty_block){
                start = i * block_size;
            } else {
                start = i * chunk_size;
            }
            end = start + chunk_size;
        }

        //if ((start==end) && (end==0)) end++;
        vetor_th[i] = std::thread(create_threadV4_auxiliary, start, end, i, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(graph));
    }


    // for(int i = 0; i < combinacoes.size(); ++i){
    //     vetor_th[i] = std::thread(find_index_induced_cycle_method_4, i , std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(graph));
    // }

    sem_wait(&semaforo);

    for(int i=0; i < used_threads; ++i){
        vetor_th[i].join(); // junção das threads
    }
}




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
 * @brief ** DEPRECATED ** Calculate block size and chunk size for threads for the M4 cycle method ** DEPRECATED **
 * @details ** DEPRECATED ** block size and chunk size for threads for the M4 cycle method ** DEPRECATED **
 * @details REPLACED BY define_block_chuck_for_cycleM4v2
 * @author Carlos Thadeu
 * @param num_threads a integer that represents the numbers of threads proposed
 * @param num_elements a integer that represents the numbers of the edges' combinations
 * @return a tuple of integers with block size and chunk size found and the number of threads selected to be used 
 */
std::tuple <int, int, int> define_block_chuck_for_cycleM4(int &num_threads, int &num_elements){
    //DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;
    int block_size = 1;
    int chunk_size = 0;
    int used_threads1 = num_threads;
    if (used_threads1 >= num_elements){
        used_threads1 = num_elements;
    } else if (used_threads1 < num_elements){
        if (!(num_elements % used_threads1))
            block_size = floor(num_elements / used_threads1);
        else{
            block_size = floor(num_elements/ used_threads1);
            chunk_size = num_elements - block_size * (used_threads1 - 1);
        }

        // block_size = floor(num_elements / used_threads1);
        // chunk_size = num_elements - (used_threads1 - 1) * block_size;
    }
    //DEBUG std::cerr << "Threads to allocated: " << used_threads1 << std::endl;
    return std::make_tuple(block_size, chunk_size, used_threads1);
}

/**
 * @brief Calculate block size and chunk size for threads for the M4 cycle method
 * @details block size and chunk size for threads for the M4 cycle method
 * @author Carlos Thadeu
 * @param num_threads a integer that represents the numbers of threads proposed
 * @param num_elements a integer that represents the numbers of the edges' combinations
 * @return a tuple of integers with block size and chunk size found and the number of threads selected to be used , the quantity of blocks and chunk
 */
std::tuple <int, int, int, int, int> define_block_chuck_for_cycleM4v2(int &num_threads, int &num_elements){
    //DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;
    int block_size = 1;
    int chunk_size = 0;
    int qty_block = 0;
    int qty_chunk = 0;
    int used_threads1 = num_threads;

    if (num_threads >= num_elements){
        qty_block = num_threads;
        qty_chunk = 0;
    } else {
        int resto = num_elements % num_threads;
        int quociente = num_threads;
        if (resto < quociente && resto != 0){
            qty_block = num_threads - 1;
            qty_chunk = 1;
            int divide = num_threads - 1;
            if (divide == 0) {
                divide = 1;
                qty_chunk = 0;
                }
            block_size = floor(num_elements / divide);
            chunk_size = num_elements - block_size;
        } else {
            if (resto != 0 and num_threads > 1){
                qty_block = num_threads -1;
                qty_chunk = 1;
                int divide = num_threads - 1;
                if (divide == 0) {
                    divide = 1;
                    qty_chunk = 0;
                }
                block_size = floor(num_elements / divide);
                chunk_size = num_elements - block_size;
            } else {
                qty_block = num_threads;
                qty_chunk = 0;
                block_size = floor(num_elements / num_threads );
                chunk_size = 0;
            }
        }
    }

    // if (used_threads1 >= num_elements){ // There are more threads than elements, threads will be same elements
    //     used_threads1 = num_elements;
    //     qty_block = used_threads1;
    //     qty_chunk = num_elements - qty_block;
    // } else if (used_threads1 < num_elements){ // There are less threads than elements
    //     if (!(num_elements % used_threads1)){
    //         block_size = floor(num_elements / used_threads1);
    //         if (block_size==0) { block_size++ ; };
    //     } else {
    //         block_size = floor(num_elements/ used_threads1) + 1;
    //         chunk_size = floor(num_elements/ used_threads1);
    //     }
    //     qty_block = num_elements % used_threads1 ;
    //     qty_chunk = num_elements - qty_block;
    //}

    //DEBUG std::cerr << "Threads to allocated: " << used_threads1 << std::endl;
    return std::make_tuple(block_size, chunk_size, used_threads1, qty_block, qty_chunk);
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
void find_index_articulation(Graph &graph, Graph &subgraph, int raiz, int start, int end, const int id)
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
                        if (!noindex)
                            f = find_factor(subgraph, tree);
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
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, graph);
    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
}

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
    int arv = 0; // debug
    int index_local = (int)INFINITY;
    Graph tree_local;
    Graph tree(n);

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;
    mtx.lock();
    int stretch_index = g.get_stretch_index();
    int lower_limit = g.grt-1 ;
    mtx.unlock();

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
                if(not OpBasic::is_cyclic(tree)){
                    if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                        int f=1;
                        if (!noindex)
                            f = find_factor(g, tree);
                        ++arv;
                        mtx.lock();
                        g.add_tree();
                        mtx.unlock();
                        
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
    arvores = arv;

    if (index_local == (int)INFINITY){
        index_local = 1;
        arvores = 0;
    }
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, g);

    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
}


/**
 * @brief Calculate a stretch index using an induced cycle (method 1)
 * @details Create thread to calculate the stretch index induced cycles (if exists)
 * This procedure will seek for induced cycles and calculate the stretch index.
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @param raiz an integer that represents the vertex root
 * @param neighbor_start an integer that represents the vertex neighbor the root
 * @param id an integer that represents a number of thread
 * @param edges_list a vector that represents induced cycle's edge to be remove
 * @param end
 */
void find_index_induced_cycle_method_1(Graph &graph, int raiz, int neighbor_start, const int id, std::vector<int> edges_list )
{
    bool execute = true;
	
    mtx.lock();
    int lower_limit = graph.grt-1 ;
    Graph G1 = graph;   // Auxiliary graph - local graph
    mtx.unlock();

    // remove the root edges from a cycle of previous threads
    // Edges removed will processed another threads
    int i=id;
    int idx = i * 2;
    int v1=edges_list[idx];
    int v2=edges_list[idx+1];
    G1.remove_aresta(v1, v2);

    int start=0;
    int end=G1.adjList(raiz).size()-1;
    DEBUG std::cerr << "Removing: " << v1 << "," << v2 << " Start: " << start << " End: " << end << std::endl;
    int n = G1.getQtdVertices();    // num vertices
    int m = G1.getQtdArestas();     // num edges
    int prox_vizinho[n] = {};       // next neighbor (index)
    int ult_colocado[n] = {};       // last add vertex at tree(index)
    int v = raiz;
    int u = -1;
    int index_local = (int)INFINITY;

    Graph tree_local(n);
    Graph tree(n);

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;

    if (execute){ // THE MAIN LOOP
        while((G1.get_stretch_index() > graph.grt - 1 || G1.get_stretch_index() ==1) && graph.get_signal() and !(abort_for_timeout)) {
            if(v == raiz){
                if (prox_vizinho[v] == end){
                    break; // Fim do algoritmo
                }
            }
            if ( prox_vizinho[v] == G1.grau(v) ){
                prox_vizinho[v] = 0;
                v = G1.ant_vertex(v);
                tree.remove_aresta(v, ult_colocado[v]);
                ult_colocado[v] = -1;
            } else {
                u = G1.adjList(v)[prox_vizinho[v]];
                ++prox_vizinho[v];
                if( not tree.possui_aresta(v, u) ){
                    tree.add_aresta(v, u);
                    ult_colocado[v] = u;

                    if(not OpBasic::is_cyclic(tree)){

                        if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                            mtx.lock();
                            int f=1;
                            if (!noindex)
                                f = find_factor(graph, tree);
                            mtx.unlock();
                            G1.add_tree();

                            if(f < index_local){
                                index_local = f;
                                tree_local = tree;
                                if(index_local == lower_limit){// alteracao LF
                                    break; // alteracao LF
                                }// alteracao LF
                                /* if(index_local == G1.grt - 1){
                                    break;
                                } */
                            }
                        } else {
                            v = G1.next_vertex(v);
                            continue;
                        }
                    }
                    tree.remove_aresta(v, u);
                }
            }
        }
    }

    int arvores;
    arvores = G1.get_total_tree();

    if (index_local == (int)INFINITY){
        index_local = 1;
        arvores = 0;
    }
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;
    
    mtx.lock();
    graph.sum_trees(G1.get_total_tree());
    int arv = 0; // Insert by compatibility with set_graph_final_parameters
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, graph);
    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
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
void find_index_induced_cycle_method_2(const int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph)
{
    bool execute = true; 

    mtx.lock();
    int lower_limit = graph.grt-1 ; // calculate lower limit
    Graph G1 = graph;   // Auxiliary graph - local graph
    G1.reset_trees();
    G1 = remove_edges_cycle_M2(combinacoes[id], edges_to_be_processed, graph);
    mtx.unlock();

    int tamanho=combinacoes[id].size();
    int raiz=edges_to_be_processed[id%tamanho].first;
    int start=0;    // Pick up the first source vertex (index 0)
    int end=G1.adjList(raiz).size(); // Pick up the last source vertex (index defined by neighbors size)
    int n = G1.getQtdVertices();    // num vertices
    int m = G1.getQtdArestas();     // num edges
    std::vector<int> prox_vizinho(n, 0);       // next neighbor (index)
    std::vector<int> ult_colocado(n, -1);       // last add vertex at tree(index)
    int v = raiz;
    int u = -1;
    int index_local = (int)INFINITY;

    Graph tree_local(n);
    Graph tree(n);

    prox_vizinho[v] = start;

    if (execute){
        //while((G1.get_stretch_index() > graph.grt - 1 || G1.get_stretch_index() ==1) && graph.get_signal()  and !(abort_for_timeout)) {
        while(graph.get_signal()  and !(abort_for_timeout)) {
            if(v == raiz){
                if (prox_vizinho[v] == end){
                    break; // Fim do algoritmo
                }
            }
            if ( prox_vizinho[v] == G1.grau(v) ){
                prox_vizinho[v] = 0;
                v = G1.ant_vertex(v);
                tree.remove_aresta(v, ult_colocado[v]);
                ult_colocado[v] = -1;
            } else {
                u = G1.adjList(v)[prox_vizinho[v]];
                ++prox_vizinho[v];
                if( not tree.possui_aresta(v, u) ){
                    tree.add_aresta(v, u);
                    ult_colocado[v] = u;

                    if(not OpBasic::is_cyclic(tree)){

                        if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                            mtx.lock();
                            int f=1;
                            if (!noindex)
                                f = find_factor(graph, tree);
                            G1.add_tree();
                            if(f < index_local){
                                index_local = f;
                                tree_local = tree;
                                if(index_local == lower_limit){// alteracao LF
                                    graph.set_signal();
                                    break; // alteracao LF
                                }// alteracao LF
                                /* if(index_local == G1.grt - 1){
                                    break;
                                } */
                            }
                            mtx.unlock();
                        } else {
                            v = G1.next_vertex(v);
                            continue;
                        }
                    }
                    tree.remove_aresta(v, u);
                }
            }
        }
    }

    int arvores;
    arvores = G1.get_total_tree();

    if (index_local == (int)INFINITY){
        index_local = 1;
        arvores = 0;
        G1.reset_trees(arvores);
    }
 
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;

    mtx.lock();
    graph.sum_trees(arvores);
    int arv = 0; // Insert to mantain compatibility with set_graph_final_parameters -- will be modified when refactoring
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, graph);
    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
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
void find_index_induced_cycle_method_4(int id, std::vector<std::vector<int>> &combinacoes, std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph)
{

    //mtx.lock();
    //pthread_mutex_lock (&mutex_signal);
    //Graph G1 = graph;   // Auxiliary graph - local graph
    
    Graph G1 = Graph();
    OpBasic::copy(G1, graph);
    //Graph G2 = graph;   // Auxiliary graph - local graph
    //pthread_mutex_unlock (&mutex_signal);
    //mtx.unlock();

    G1.reset_trees(0);
    G1 = remove_edges_cycle_M2(combinacoes[id], edges_to_be_processed, graph);
    
    std::vector<std::pair<int,int>> immovable_edges = immovable(id, combinacoes, edges_to_be_processed); 

    int num_vertices = G1.get_num_vertices();
    Graph tree(num_vertices);
    int num = combinacoes[id].size() - 1;
    int root = edges_to_be_processed[num].second;
    //find_index_cycleM4(root, edges_to_be_processed, G1, graph);
    find_index_cycleM4_2(id, root, immovable_edges, G1, graph);

    int arvores;
    arvores = G1.get_total_tree();
    int index_local = G1.get_stretch_index();

    if (index_local == (int)INFINITY){
        index_local = 1;
        arvores = 0;
        G1.reset_trees(arvores);
    }
    //G1.set_best_tree(s);
    //mtx.lock();
    pthread_mutex_lock (&mutex_signal);
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;
    //graph.set_stretch_index(index_local);
    //G1.set_best_tree(tree);
    graph.sum_trees(arvores);
    total_arv = arvores;
    int arv = 0; // Insert to mantain compatibility with set_graph_final_parameters -- will be modified when refactoring
    set_graph_final_parameters(index_local, total_arv, arv, G1, graph);
    pthread_mutex_unlock (&mutex_signal);
    //mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
}

void find_index_pararell_edge(Graph& g, std::vector<int> edges, int start, const int id)
{
    sem_wait(&semaforo);

    int n = g.getQtdVertices();
    int m = g.getQtdArestas();
    // std::vector<int> edges = OpBasic::edges(g);
    int indice[n-1];
    int j = 0;
    indice[j] = start;
    
    Graph tree(n);
    Graph tree_local;
    int arv = 0;
    //int index_local = INF_VALUE;
    int index_local = (int)INFINITY;

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
                    if( !OpBasic::is_cyclic(tree) ){
                        if(j == n-2){ // achou uma arvore geradora
                            int f=1;
                            if (!noindex) 
                                f = find_factor(g, tree);
                            ++arv;
                            mtx.lock();
                            g.add_tree();
                            mtx.unlock();
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
    
    mtx.lock();
    int arvores;
    arvores =arv;

    if (index_local == (int)INFINITY){
        index_local = 1;
        arvores = 0;
    }
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, g);
    mtx.unlock();
    sem_post(&semaforo);
}

void find_index_parallel_edgeV2(Graph& g, std::vector<int> edges, int start, const int id)
{
    sem_wait(&semaforo);

    int n = g.getQtdVertices();
    int m = g.getQtdArestas();
    // std::vector<int> edges = OpBasic::edges(g);
    int indice[n-1];
    int j = 0;
    indice[j] = start;
    
    Graph tree(n);
    Graph tree_local;
    int arv = 0;
    //int index_local = INF_VALUE;
    int index_local = (int)INFINITY;

    Graph gTeste(n);

    // Removido pq coloquei em cria threads
    //OpBasic op;
    //int grt = op.maxLowerCicle(g);

    for(int i = start; i < edges.size(); i += 2)
    {
        gTeste.add_aresta(edges[i], edges[i+1]);
    }
    if( OpBasic::is_connected(gTeste)){
    //if( g.get_signal() && !(abort_for_timeout) ){

        if((g.get_stretch_index() > g.grt-1 || g.get_stretch_index()==1)) { //Começa a busca pelas árvores geradoras. // Alterado by thadeu
            while(indice[0] < start+2  && g.get_signal() and !(abort_for_timeout)){ //Update by thadeu
                if( indice[j]/2 > m-(n-1-j) ){
                    --j;
                    tree.remove_aresta(edges[indice[j]],edges[indice[j]+1]);
                    indice[j] += 2;
                }
                else {
                    tree.add_aresta(edges[indice[j]], edges[indice[j]+1]);
                    if( !OpBasic::is_cyclic(tree) ){
                        if(j == n-2){ // achou uma arvore geradora
                            int f=1;
                            if (!noindex)
                                f = find_factor(g, tree);
                            ++arv;
                            mtx.lock();
                            g.add_tree();
                            mtx.unlock();
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
    
    mtx.lock();
    int arvores;
    arvores =arv;

    if (index_local == (int)INFINITY){
        index_local = 1;
        arvores = 0;
    }
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, g);
    mtx.unlock();
    sem_post(&semaforo);
}

void create_threads(Graph& graph)
{
    int raiz = highest_degree_vertex(graph);
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
    int raiz = highest_degree_vertex(graph);
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

    for(int i=0; i < used_threads; ++i){
        vetor_th[i] = std::thread(find_index_pararell_edge, std::ref(g), edges, i*2, i); // separação dos threats
    }

    for(int i=0; i < qtd_th; ++i){
        vetor_th[i].join(); // junção das threads
    }
}

void create_threadV2_edge_max_degree_auxiliary(Graph &graph, std::vector<int> edges, int block_size, int id_th){
    for (int i = 0; i < block_size; i = i + 2 ){
        int start = block_size * id_th + i;
        find_index_parallel_edgeV2(graph, edges, start, id_th);
    }
}

// Function to slice a given vector
// from range X to Y
std::vector<int> slicing(std::vector<int>& arr,
                    int X, int Y)
{
 
    // Starting and Ending iterators
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y + 1;
 
    // To store the sliced vector
    std::vector<int> result(Y - X + 1);
 
    // Copy vector using copy function()
    copy(start, end, result.begin());
 
    // Return the final sliced vector
    return result;
}

void create_threads_edge_max_degreeV2(Graph& graph)
{
    int qtd_th = graph.maior_grau();
    //int qtd_th = num_threads;

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    OpBasic op; // by thadeu
    //graph.grt = op.maxLowerCicle(graph); // by thadeu
    // fim calcula grt

    //std::vector< std::thread> vetor_th(qtd_th);

    graph.reset_trees(0);

    std::vector<int> edges = OpBasic::edges_by_bigger_degree(graph);
    
    int qty = edges.size()/2;

    auto acme = define_block_chuck_for_max_degree(num_threads,qty);
    int block_size = std::get<0>(acme);
    int chunk_size = std::get<1>(acme);
    used_threads = std::get<2>(acme);

    std::thread vetor_th[used_threads];

    // Adjust if block size is odd, remember, edges are pairs (u,v)
    if ((block_size % 2)){
        block_size++;
        chunk_size = qty - block_size * (used_threads - 1);
    }

    for(int i = 0; i < used_threads; ++i){
        vetor_th[i] = std::thread(create_threadV2_edge_max_degree_auxiliary, std::ref(graph), edges, block_size, i);
    }

    sem_wait(&semaforo);

    for(int i=0; i < used_threads; ++i){
        vetor_th[i].join(); // junção das threads
    }
}

/**
 * @brief Create threads to calculate stretch index for induced cycles (method 1) (DEPRECATED) will be remove at future
 * @details Create threads to calculate the stretch index using induced cycles (if exists)
 * Remove an edge of the graph, send new graph to thread, back with the edge to the original graph, remove the next edge and go on
 * (DEPRECATED) will be remove at future
 * @author Carlos Thadeu
 * @param g a graph that represents the graph
 */
/* void create_threads_induced_cycle_method_1(Graph &g) {
    int qty = -1;
    int root = -1;
    int neighbor = -1;
    std::vector<int> edges_list = {};

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    OpBasic op; // by thadeu
    g.grt = op.maxLowerCicle(g); // by thadeu
    // fim calcula grt

    // build edge list to be deleted from original graph inside find_index_induced_cycle
    edges_list = seeking_induced_cycles_edges_v1(g);


    // remove the root edges from a cycle of previous threads
    // Edges removed will processed another threads
    std::vector<int> aux = {};

    for (int i=0; i < edges_list.size(); i=i+2){
        int v1=edges_list[i];
        int v2=edges_list[i+1];
        g.remove_aresta(v1, v2);
        if (g.grau(v1) <= 1 && i!=0) { // Disconnected the graph? If Yes, cancel edge removed
            g.add_aresta(v1, v2);
        } else {
            aux.push_back(v1);
            aux.push_back(v2);
            g.add_aresta(v1, v2);
        }
    }

    edges_list = aux;
    qty = int(edges_list.size() / 2);
    if (qty > num_threads)
        qty = num_threads;

    DEBUG std::cerr << "Threads usadas: " << qty << std::endl;
    std::thread vetor_th[qty];
    sem_wait(&semaforo);

    // Thread 0 will remove the 1st edge
    // Thread 1 will remove the 2nd edge
    // Thread 2 will remove the 3rd edge
    // ...
    // Thread n will remove the (n+1)rd edge
    for(int i = 0; i < qty; ++i){
        DEBUG std::cerr << "Thread a ser processada: " << i << std::endl;
        root = edges_list[i*2];
        neighbor = edges_list[i*2+1];
        // Here send edges' list to be deleted
        // Delete the edges ensures that there are no duplicate trees
        //vetor_th[i] = std::thread(find_index_induced_cycle, std::ref(g), root, neighbor, i, std::ref(edges_list));
        vetor_th[i] = std::thread(find_index_induced_cycle_method_1, std::ref(g), root, neighbor, i, std::ref(edges_list));
    }
    sem_wait(&semaforo);
    for(int i = 0; i < qty; ++i){
        vetor_th[i].join();
    }
} */

std::vector<std::pair<int,int>> detect_valid_edges( std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph){
    std::vector<std::pair<int,int>> aux;
    // Verify for unabled edges to be removed
    int n=edges_to_be_processed.size();
    Graph G1=graph;
    for (int i=0; i < n; i++){
        int v1=edges_to_be_processed[i].first;
        int v2=edges_to_be_processed[i].second;
        if (G1.possui_aresta(v1, v2)){
            G1.remove_aresta(v1, v2);
            if (G1.grau(v1)<=1 && i != 0){
                G1.add_aresta(v1, v2);
                if (n == (i / 2 + 1)){
                    DEBUG std::cerr << "Thread " << n << " with v1: " << v1 << " v2: " << v2 << " cannot be removed!"  << std::endl;
                }
            } else {
                aux.push_back(std::make_pair(v1,v2));
            }
        }
    } // end of prodedure to Verify for unabled edges to be removed
    return aux;
}


/*
    Detect edges to be removed
*/
std::vector<std::pair<int,int>> detect_valid_edges_M4( std::vector<std::pair<int,int>> &edges_to_be_processed, Graph &graph){
    std::vector<std::pair<int,int>> aux;
    // Verify for unabled edges to be removed
    int n=edges_to_be_processed.size();
    Graph G1=graph;
    bool flag = false;
    int connect = 0;
    for (int i=0; i < n; i++){
        int v1=edges_to_be_processed[i].first;
        int v2=edges_to_be_processed[i].second;
        
        if (G1.possui_aresta(v1, v2)){
            G1.remove_aresta(v1, v2);
            connect = OpBasic::distance(G1, v1, v2);
            if (connect == -1){
                G1.add_aresta(v1, v2);
            } else {
                aux.push_back(std::make_pair(v1,v2));
            }

            // if ((G1.grau(v1) < 1 || G1.grau(v2) < 1) || ((G1.grau(v1) == 1 && flag == true) || G1.grau(v2) < 1) ){
            //     G1.add_aresta(v1, v2);
            //     flag = true;
            //     if (n == (i / 2 + 1)){
            //         DEBUG std::cerr << "Thread " << n << " with v1: " << v1 << " v2: " << v2 << " cannot be removed!"  << std::endl;
            //     }
            // } else {
            //     aux.push_back(std::make_pair(v1,v2));
            // }
            // if (G1.grau(v2) > 1)
            //     flag = false;
        }
    } // end of prodedure to Verify for unabled edges to be removed
    return aux;
}

/** DEPRECATED WILL BE REMOVE IN NEXT RELEASES
 * @brief Create threads to calculate stretch index for induced cycles (method 2)
 * @details Create threads to calculate the stretch index using induced cycles found(if exists)
 * @details Create threads from combinations (n,1) until (n,n)
 * @author Carlos Thadeu
 * @param g a graph that represents the graph
 */
/* void create_threads_induced_cycle_method_2(Graph& g) {
    int qty = -1;
    int root = -1;
    int neighbor = -1;
    int n;
    std::vector<int> edges_list = {};

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    OpBasic op; // by thadeu
    g.grt = op.maxLowerCicle(g); // by thadeu
    // fim calcula grt

    DEBUG std::cerr << "Searching for induced cycles using method 2!"  << std::endl;
    edges_list = seeking_induced_cycles_edges_v3(g);

    std::vector<std::pair<int,int>> edges_to_be_processed = make_edges_list(edges_list);
    DEBUG std::cerr << "Pre-processing edges!"  << std::endl;

    edges_to_be_processed=detect_valid_edges(edges_to_be_processed, g);
    

    int r;   // Generate the combinations
    std::vector<std::vector<int>> combinacoes;
    n=edges_to_be_processed.size();
    for (int i=n; i > 0; i--){
        std::vector<std::vector<int>> edges_to_remove = combinatorics(n, i);
        for (int j=0; j < edges_to_remove.size(); j++){
            combinacoes.push_back(edges_to_remove[j]);
        }
    }   // End of generate the combinations

    qty = combinacoes.size();

    DEBUG std::cerr << "Edges numbers to use!" << qty << std::endl;

    if (qty > num_threads)
        qty = num_threads;
            
    DEBUG std::cerr << "Threads to be used: " << qty << std::endl;
    std::thread vetor_th[qty];

    for(int i = 0; i < qty; ++i){
        vetor_th[i] = std::thread(find_index_induced_cycle_method_2, i, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(g));
    }
    sem_wait(&semaforo);
    for(int i = 0; i < qty; ++i){
        vetor_th[i].join();
    }
} */

/**
 * @brief Create threads to calculate stretch index for induced cycles (method 3)
 * @details Create threads to calculate the stretch index using induced cycles found(if exists)
 * @details Create threads from combinations (n,1) until (n,n)
 * @author Carlos Thadeu
 * @param g a graph that represents the graph
 */
void create_threads_induced_cycle_method_3(Graph& g) {
    int qty = -1;
    int root = -1;
    int neighbor = -1;
    int n;
    std::vector<int> edges_list = {};

    DEBUG std::cerr << "Searching for induced cycles using method 3!"  << std::endl;
    edges_list = seeking_induced_cycles_edges_v3r1(g);

    std::vector<std::pair<int,int>> edges_to_be_processed = make_edges_list(edges_list);
    DEBUG std::cerr << "Pre-processing edges!"  << std::endl;
    
    edges_to_be_processed=detect_valid_edges(edges_to_be_processed, g);

    int r;   // Generate the combinations
    std::vector<std::vector<int>> combinacoes;
    n=edges_to_be_processed.size();
    for (int i=n; i > 0; i--){
        std::vector<std::vector<int>> edges_to_remove = combinatorics(n, i);
        for (int j=0; j < edges_to_remove.size(); j++){
            combinacoes.push_back(edges_to_remove[j]);
        }
    }   // End of generate the combinations

    qty = combinacoes.size();

    DEBUG std::cerr << "Edges numbers to use!" << qty << std::endl;
            
    DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;
    int threads=0;
    int block=1;
    if (num_threads >= qty){
        num_threads=qty;
    } else if (num_threads < qty){
        block=floor(qty/num_threads);
    }
    int chunk = qty - num_threads * block;
    DEBUG std::cerr << "Threads to allocated: " << num_threads << std::endl;

    std::thread vetor_th[num_threads];

    for(int i = 0; i < block; ++i){
        for (int j = 0; j < num_threads; j++){
            int index = i * num_threads + j;
            vetor_th[j] = std::thread(find_index_induced_cycle_method_2, index, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(g));
        }
        sem_wait(&semaforo);
        for(int j = 0; j < num_threads; ++j){
            vetor_th[j].join();
        }
    }
    if (qty%num_threads){
        for (int j = 0; j < chunk; j++){
            int index = block * num_threads + j;
            vetor_th[j] = std::thread(find_index_induced_cycle_method_2, index, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(g));
        }
        sem_wait(&semaforo);
        for(int j = 0; j < chunk; ++j){
            vetor_th[j].join();
        }
    }
}

/**
 * @brief Create threads to calculate stretch index for induced cycles (method 3)
 * @details Create threads to calculate the stretch index using induced cycles found(if exists)
 * @details Create threads from combinations (n,1) until (n,n)
 * @author Carlos Thadeu
 * @param g a graph that represents the graph
 */
void create_threads_induced_cycle_method_3v2(Graph& g) {
    int qty = -1;
    int root = -1;
    int neighbor = -1;
    int n;
    std::vector<int> edges_list = {};
    g.reset_trees(0);

    DEBUG std::cerr << "Searching for induced cycles using method 3!"  << std::endl;
    edges_list = seeking_induced_cycles_edges_v3r1(g);

    std::vector<std::pair<int,int>> edges_to_be_processed = make_edges_list(edges_list);
    DEBUG std::cerr << "Pre-processing edges!"  << std::endl;
    
    edges_to_be_processed=detect_valid_edges(edges_to_be_processed, g);

    int r;   // Generate the combinations
    std::vector<std::vector<int>> combinacoes;
    n=edges_to_be_processed.size();
    for (int i=n; i > 0; i--){
        std::vector<std::vector<int>> edges_to_remove = combinatorics(n, i);
        for (int j=0; j < edges_to_remove.size(); j++){
            combinacoes.push_back(edges_to_remove[j]);
        }
    }   // End of generate the combinations

    qty = combinacoes.size();

    DEBUG std::cerr << "Edges numbers to use!" << qty << std::endl;
            
    DEBUG std::cerr << "Threads proposed: " << num_threads << std::endl;

    int threads=0;
    int block=1;
    int chunk;
    if (num_threads >= qty){
        used_threads = qty;
    } else if (num_threads < qty){
        used_threads = num_threads;
        block=floor(qty / num_threads);
    }
    chunk = qty - num_threads * block;
    DEBUG std::cerr << "Threads to allocated: " << used_threads << std::endl;
    std::thread vetor_th[used_threads];

    for(int i = 0; i < block; ++i){
        for (int j = 0; j < used_threads; j++){
            int index = i * used_threads + j;
            vetor_th[j] = std::thread(find_index_induced_cycle_method_2, index, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(g));
        }
        sem_wait(&semaforo);
        for(int j = 0; j < used_threads; ++j){
            vetor_th[j].join();
        }
    }

        if (qty % used_threads){
        for (int j = 0; j < chunk; j++){
            int index = block * used_threads + j;
            vetor_th[j] = std::thread(find_index_induced_cycle_method_2, index, std::ref(combinacoes), std::ref(edges_to_be_processed), std::ref(g));
        }
        sem_wait(&semaforo);
        for(int j = 0; j < chunk; ++j){
            vetor_th[j].join();
        }
    }
}

Graph remove_edges_cycle_M2(std::vector<int> combinations, std::vector<std::pair<int, int>> edges, Graph graph){
    for (auto i: combinations){
        //if (i != -1)
            graph.remove_aresta(edges[i].first, edges[i].second);
    }
    std::string grafo = tree_to_string(graph);
    DEBUG std::cerr << "Este é o grafo: " << grafo << "\n";
    return graph;
}

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
        int root = highest_degree_vertex(xpto[id]);
        int neighbor = xpto[id].adjList(root)[0];
        vetor_th[id] = std::thread(find_index_articulation, std::ref(g), std::ref(xpto[id]), root, root, neighbor, id);
        id++;
    }
    for(int i = 0; i < id; ++i){
        vetor_th[i].join();
    }
} */

//! Get index from adjacent vertex
/*!
    \param g a graph instance that represents the graph
    \param v a integer that represents a vertex
    \param adj a integer that represents a vertex adjacent at v
    \return a integer that represents a index
*/
int adj_id(Graph& g, int v, int adj)
{
    int index = 0;
    for(int u : g.adjList(v) ){
        if(u == adj){
            break;
        }
        ++index;
    }
    return index;
}

int next(int a, int limite)
{
    ++a;
    return a == limite ? 0 : a;
}

int find_factor(Graph& g, Graph& tree)
{
    std::vector<int> list = OpBasic::diference_edge(g, tree);
    std::vector<int>::iterator it;
    int factor = 1;

    it = list.begin();
    while(it != list.end()){
        int v = *it;
        int u = *(it+1);
        int d = OpBasic::distance(tree, v, u);
        if(factor < d){
            factor = d;
        }
        it = it + 2;
    }

    return factor;
}

int highest_degree_vertex(Graph& g)
{
    int raiz = -1;
    int maior_grau = -1;
    for(int i=0; i < g.getQtdVertices(); ++i){
        if(g.grau(i) > maior_grau){
            raiz = i;
            maior_grau = g.grau(i);
        }
    }
    return raiz;
}

/**
 * @brief Auxiliary function to setting all parameters after stretch index found
 * @details Auxiliary function to setting all parameters after stretch index found
 * @author Carlos Thadeu
 */
void set_graph_final_parameters(int &index_local, int &total_arv, int &arv, Graph &tree_local, Graph &graph){
    if((index_local < graph.get_stretch_index() && index_local != 1) || graph.get_stretch_index()==1) {
        total_arv += arv;
        graph.set_stretch_index(index_local);
        graph.set_best_tree(tree_local);
        if (index_local==graph.grt - 1){
            graph.set_signal();
            //processando.store(false,std::memory_order_release);
        }
    }
}

/**
 * @brief Search for induced cycles (DEPRECATED) will be remove at future
 * @details Search for induced cycles (if exists)
 * (DEPRECATED) will be remove at future
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found 
 */
/* std::vector<int> seeking_induced_cycles_edges_v1(Graph &graph){
    int cycle_size=INDUCED_CYCLE_SYZE_START;        // define the max edges at induced cycle to be search
	std::vector<std::vector<int>> select_cycles;    // 
	std::vector<int> processed;                     // Processed vertices
	std::vector<int> vertices_list;                 // Vertices to be processed

	// Seleciona o vertice inicial
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
	std::vector <int>vertices = graph.vertices_de_maior_grau();
	std::vector<std::pair<int,float>> centrality = Centrality::closeness_centrality_list(vertices, graph);
	int root = Centrality::root_selection2(centrality);
    std::vector<int> neighbors = graph.adjList(root);   // Neighbors at root
	int seek = Centrality::root_selection2(Centrality::closeness_centrality_list(neighbors, graph)); // aplica a centralidade de proximidade
	// Fim da seleção do vértice inicial

	for (int i=0; i<graph.get_qty_vertex(); i++){   //inicializa a lista
		vertices_list.push_back(i);
	}

    std::vector<int> cycle;
	while (true) {
		DEBUG std::cerr << "Searching for (" << cycle_size<< ") " << std::endl;
		cycle=OpBasic::cycle(graph, cycle_size, seek, root);
		if (cycle.empty()){
            // Não achei ciclo do tamanho pré-definido, procuro um menor
			if (cycle_size > 3){ 
				cycle_size--;
				continue;
			}
			else {  // Não achei nem um triângulo
				break;
			}
		} else {
			cycle_size=INDUCED_CYCLE_SYZE_START;
		}
		join_vectors(processed, cycle);

		// Apaga vertices ja processados
		for (int j=0; j<cycle.size(); j++){
			int index=get_index(cycle[j], vertices_list);
			if (!vertices_list.empty() && index < vertices_list.size())
				vertices_list.erase (vertices_list.begin() + index);
			else
                if (vertices_list.empty() ) 
				    break;
		}
		cycle.push_back(root);
		processed.push_back(root);
		select_cycles.push_back(cycle);

		if (vertices_list.empty()) break;

		bool gameover = false;
		while (!gameover){
			// Escolhe nova raiz
			centrality = Centrality::closeness_centrality_list(vertices_list, graph);
			root = Centrality::root_selection2(centrality);
			if (!in(root, processed) || vertices_list.empty()){
				gameover=true;
			}
		}
		gameover = false;
		neighbors = graph.adjList(root);
		while (!gameover){
			seek = Centrality::root_selection2(Centrality::closeness_centrality_list(neighbors, graph));
			if (!in(seek, processed)){
				gameover=true;
			} else {
				int index=get_index(seek, neighbors);
				neighbors.erase (neighbors.begin() + index);
				if (neighbors.empty()) break;
			}
		}
		if (neighbors.empty()) break;
	}

    int neighbor;
    bool accept = false;
    std::vector<int> edges_list;
    for (int j=0; j < select_cycles.size(); j++){
        for(int i = 0; i < select_cycles[j].size()-1; ++i){
            root = select_cycles[j][i];
            neighbor=select_cycles[j][i+1];
            if (edges_list.size() > 0){
                for (int k=0; k < edges_list.size(); k=k+2){
                    if ((edges_list[k]==root && edges_list[k+1]==neighbor) || (edges_list[k]==neighbor && edges_list[k+1]==root) ){
                        accept=true;
                    }
                }
            }

            edges_list.push_back(root);
            edges_list.push_back(neighbor);
            accept=false;
        }
    }

    //return select_cycles;
    return edges_list;
} */

/**
 * @brief Search for an induced cycle - PRECISION CLOSENESS - MUCH MORE SLOW
 * @details Search for an induced cycle (if exists) PRECISION CLOSENESS - MUCH MORE SLOW
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found 
 */
std::vector<int> seeking_induced_cycles_edges_v2(Graph &graph){
  
    int max_cycle_size=floor(log2(num_threads + 1));
    DEBUG std::cerr << "Define induced cycle to size: " << max_cycle_size << std::endl;
    std::vector<std::vector<int>> select_cycles;    // 

	// Seleciona o vertice inicial
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
	std::vector <int>vertices = graph.vertices_de_maior_grau();
	std::vector<std::pair<int,float>> centrality = Centrality::closeness_centrality_list(vertices, graph);
	int root = Centrality::root_selection2(centrality);
    std::vector<int> neighbors = graph.adjList(root);   // Neighbors at root
	int seek = Centrality::root_selection2(Centrality::closeness_centrality_list(neighbors, graph)); // aplica a centralidade de proximidade
	// Fim da seleção do vértice inicial

    search_for_induced_cycles_for_M2(seek, root, max_cycle_size, select_cycles, graph);

    // TO DO
    // TO DO - Make a better choice from select_cycles
    // TO DO

    // Until make a better choice, select the first vector
    std::vector<int> edges_list;
    if (!select_cycles.empty())
        edges_list=select_cycles[0];
    return edges_list;
}

/**
 * @brief Search for an induced cycle (USING CLOSENESS CALCULATED)
 * @details Search for an induced cycle (if exists) (USING CLOSENESS CALCULATED)
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found 
 */
std::vector<int> seeking_induced_cycles_edges_v3(Graph &graph){

    int max_cycle_size=floor(log2(num_threads + 1));
    DEBUG std::cerr << "Define induced cycle to size: " << max_cycle_size << std::endl;
    std::vector<std::vector<int>> select_cycles;    // 

	// Seleciona o vertice inicial 
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
	std::vector <int>vertices = graph.vertices_de_maior_grau();
    
    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);
    
    DEBUG std::cerr << "Selecting root" << std::endl;
    int root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    
    DEBUG std::cerr << "Selected neighbor from root: " << root << std::endl;
    std::vector<int> neighbors = graph.adjList(root);   // Neighbors at root
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
        edges_list=select_cycles[0];
    return edges_list;
}

/**
 * @brief Search for an induced cycle (USING CLOSENESS CALCULATED)
 * @details Search for an induced cycle (if exists) (USING CLOSENESS CALCULATED)
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found 
 */
std::vector<int> seeking_induced_cycles_edges_v3r1(Graph &graph){
    int max_cycle_size_SPECIAL;
    int n = graph.get_qty_vertex();
    // BELOW - DEFINE SIZE OF INDUCED CYCLE BASED ON THREADS NUMBERS
    //if ( num_threads < 7){
    //    max_cycle_size_SPECIAL=floor(log2(127 + 1));
    //} else {
    //    max_cycle_size_SPECIAL=floor(log2(num_threads + 1));
    //}

    // BELOW - FIXED MAX SIZE OF INDUCED CYCLE TO BE SEARCH
    max_cycle_size_SPECIAL=7;
    DEBUG std::cerr << "Define induced cycle to size: " << max_cycle_size_SPECIAL << std::endl;
    std::vector<std::vector<int>> select_cycles;    // 

	// Seleciona o vertice inicial 
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
	std::vector <int>vertices = graph.vertices_de_maior_grau();
    
    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);
    
    DEBUG std::cerr << "Selecting root" << std::endl;
    int root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    
    DEBUG std::cerr << "Selected neighbor from root: " << root << std::endl;
    std::vector<int> neighbors = graph.adjList(root);   // Neighbors at root
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
        edges_list=select_cycles[0];
    return edges_list;
}


/**
 * @brief Search for an induced cycle (USING CLOSENESS CALCULATED)
 * @details Search for an induced cycle (if exists) (USING CLOSENESS CALCULATED)
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found 
 */
std::vector<std::vector<std::pair<int, int>>> seeking_induced_cycles_edges_v4(Graph &graph){
    // BELOW - FIXED MAX SIZE OF INDUCED CYCLE TO BE SEARCH
    //int cycle_size = graph.waist();
    int cycle_size = OpBasic::girth(graph);
    graph.set_girth(cycle_size);

    DEBUG std::cerr << "Define induced cycle to size: " << cycle_size << std::endl;
    std::vector<std::vector<int>> select_cycles;    // 

	// Seleciona o vertice inicial 
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
	std::vector <int>vertices = graph.vertices_de_maior_grau();
    
    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);
    
    DEBUG std::cerr << "Selecting root" << std::endl;
    int root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    
    DEBUG std::cerr << "Selected neighbor from root: " << root << std::endl;
    std::vector<int> neighbors = graph.adjList(root);   // Neighbors at root
	int seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage); 
    
    // Fim da seleção do vértice inicial

    search_for_induced_cycles_for_M4(seek, root, cycle_size, select_cycles, vertices_closeness, vertices_leverage, graph);

    return make_edges_list(select_cycles);

}

/**
 * @brief Search for an induced cycle (USING CLOSENESS CALCULATED)
 * @details Search for induced cycles (if exists) (USING CLOSENESS CALCULATED)
 * The max size cycle is defined by girth
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found 
 */
// std::vector<int> seeking_induced_cycles_edges_v4(Graph &graph){
//     //int n = graph.get_qty_vertex();

//     std::vector<std::vector<int>> select_cycles;    // 

// 	// Seleciona o vertice inicial 
//     // Seleciona o vértice com maior grau, se houver empate usa a
//     // centralidade de proximidade para desempate
//     // caso o empate persista, pega o primeiro vértice da lista
// 	std::vector <int>vertices = graph.vertices_de_maior_grau();
    
//     DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
//     std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
//     std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);
    
//     DEBUG std::cerr << "Selecting root" << std::endl;
//     int root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    
//     DEBUG std::cerr << "Selected neighbor from root: " << root << std::endl;
//     std::vector<int> neighbors = graph.adjList(root);   // Neighbors at root
// 	int seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage); 
    
//     // Fim da seleção do vértice inicial

//     // MANY CYCLES ARE SEARCH - UNCOMMENT BELOW LINE WHEN to do * Make a better choice from select_cycles*
//     search_for_induced_cycles_for_M4(seek, root, graph.get_girth(), select_cycles, vertices_closeness, vertices_leverage, graph);
    
//     // ONLY ONE CYCLE IS SEARCH - COMMENT BELOW LINE WHEN YOU USE search_for_induced_cycles_for_M2_revision1
//     //search_for_induced_cycles_for_M2_revision1_only_one(seek, root, max_cycle_size_SPECIAL, select_cycles, vertices_closeness, vertices_leverage, graph);

//     // TO DO
//     // TO DO - Make a better choice from select_cycles
//     // TO DO - Needs improve cycle choice HERE
//     // TO DO

//     // Until make a better choice, select the first vector
//     std::vector<std::pair<int, int>> edges_aux;
//     for (auto i : select_cycles){
//         std::pair<int, int> edge;
//         int limit = i.size() - 1;
//         for (int j = 0; j < limit; j++) {
//             edge = std::make_pair(i[j], i[j+1]);
//             if (!in(edge, edges_aux))
//                 edges_aux.push_back(edge);
//         }
//     }
//     // if (!select_cycles.empty())
//     //     edges_list=select_cycles[0];
//     std::vector<int>edges_list;
//     edges_list.push_back(1);
//     return edges_list;
// }


void search_for_induced_cycles_for_M2(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, Graph &graph){
    //int MAX_CYCLE_SIZE=floor(log2(num_threads + 1));
    int initial_max_cycle=cycle_size;

    std::vector<std::pair<int,float>> centrality;
    std::vector<int> neighbors;

    std::vector<int> processed;                     // Processed vertices
	std::vector<int> vertices_list;                 // Vertices to be processed
	for (int i=0; i<graph.get_qty_vertex(); i++){   //inicializa a lista
		vertices_list.push_back(i);
	}
    std::vector<int> cycle(1,0); // create and initialize an auxiliary list

    int max_size_cycle=0;   // Define Max size list found  // Method 1 != Method 2

	while (true) {
		DEBUG std::cerr << "Searching for (" << cycle_size<< ") cycle size" << std::endl;
		cycle=OpBasic::cycle(graph, cycle_size, seek, root);
		if (cycle.empty()){
			if (cycle_size > 3 && cycle_size > max_size_cycle){ // Não achei ciclo do tamanho pré-definido, procuro um menor
				cycle_size--;
				continue;
			}
			else {  // I didn't even find a triangle, then I quit the loop
				break;
			}
		} else {
			cycle_size=initial_max_cycle;
		}
		for (int j=0; j < cycle.size(); j++){ // Delete processed vertices
			int index=get_index(cycle[j], vertices_list);
			if (!vertices_list.empty() && index < vertices_list.size())
				vertices_list.erase (vertices_list.begin() + index);
			else
                if (vertices_list.empty() ) 
				    break;
		}
		cycle.push_back(root);
		processed.push_back(root);

        int acme=cycle.size()-1;    //auxiliary var
        if (!cycle.empty() && acme > max_size_cycle){   // Method 1 != Method 2
    		select_cycles.clear();                              // Only cycles that has same size(maximum select)
            select_cycles.push_back(cycle);                     // will be selected
            if (cycle_size>max_size_cycle)
                max_size_cycle=acme;
        } else if (!cycle.empty() && acme == max_size_cycle){   
            select_cycles.push_back(cycle);
        }

		if (vertices_list.empty()) break;

		bool gameover = false;
		while (!gameover){  // Escolhe nova raiz
			
			centrality = Centrality::closeness_centrality_list(vertices_list, graph);
			root = Centrality::root_selection2(centrality);
			if (!in(root, processed) || vertices_list.empty()){
				gameover=true;
			}
		}
		gameover = false;
		neighbors = graph.adjList(root);
		while (!gameover){
			seek = Centrality::root_selection2(Centrality::closeness_centrality_list(neighbors, graph));
			if (!in(seek, processed)){
				gameover=true;
			} else {
				int index=get_index(seek, neighbors);
				neighbors.erase (neighbors.begin() + index);
				if (neighbors.empty()) break;
			}
		}
		if (neighbors.empty()) break;
	}
}

/*
    SEARCH FOR MANY CYCLES
*/
void search_for_induced_cycles_for_M2_revision1(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph){
    int initial_max_cycle=cycle_size;

    std::vector<std::pair<int,float>> centrality;
    std::vector<int> neighbors;

    std::vector<int> processed;                     // Processed vertices
	std::vector<int> vertices_list;                 // Vertices to be processed
	for (int i=0; i<graph.get_qty_vertex(); i++){   //inicializa a lista
		vertices_list.push_back(i);
	}
    std::vector<int> cycle(1,0); // create and initialize an auxiliary list

    int max_size_cycle=0;   // Define Max size list found  // Method 1 != Method 2

	while (true) {
		DEBUG std::cerr << "Searching for (" << cycle_size<< ") cycle size" << std::endl;
		cycle=OpBasic::cycle(graph, cycle_size, seek, root);
		if (cycle.empty()){
			if (cycle_size > 3 && cycle_size > max_size_cycle){ // Não achei ciclo do tamanho pré-definido, procuro um menor
				cycle_size--;
				continue;
			}
			else {  // I didn't even find a triangle, then I quit the loop
				break;
			}
		} else {
			cycle_size=initial_max_cycle;
		}

		for (int j=0; j < cycle.size(); j++){ // Delete processed vertices
			int index=get_index(cycle[j], vertices_list);
			if (!vertices_list.empty() && index < vertices_list.size())
				vertices_list.erase (vertices_list.begin() + index);
			else
                if (vertices_list.empty() ) 
				    break;
		}
		cycle.push_back(root);
		processed.push_back(root);

        int acme=cycle.size()-1;    //auxiliary var
        if (!cycle.empty() && acme > max_size_cycle){   // Method 1 != Method 2
    		select_cycles.clear();                              // Only cycles that has same size(maximum select)
            select_cycles.push_back(cycle);                     // will be selected
            if (cycle_size > max_size_cycle)
                max_size_cycle=acme;
        } else if (!cycle.empty() && acme == max_size_cycle){   
            select_cycles.push_back(cycle);
        }

		if (vertices_list.empty()) break;

		bool gameover = false;
		while (!gameover){  // Escolhe nova raiz
            root = Centrality::tiebreaker(vertices_list, vertices_closeness, vertices_leverage);
			if (!in(root, processed) || vertices_list.empty()){
				gameover=true;
			}
		}
		gameover = false;
		neighbors = graph.adjList(root);
		while (!gameover){
            seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage);
			if (!in(seek, processed)){
				gameover=true;
			} else {
				int index=get_index(seek, neighbors);
				neighbors.erase (neighbors.begin() + index);
				if (neighbors.empty()) break;
			}
		}
		if (neighbors.empty()) break;
	}
}


/*
    SEARCH FOR ONE CYCLE - LIKE ADVISORS SUGGEST (for that time)
*/
void search_for_induced_cycles_for_M2_revision1_only_one(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph){
    int initial_max_cycle=cycle_size;

    std::vector<std::pair<int,float>> centrality;
    std::vector<int> neighbors;

    std::vector<int> processed;                     // Processed vertices
	std::vector<int> vertices_list;                 // Vertices to be processed
	for (int i=0; i<graph.get_qty_vertex(); i++){   //inicializa a lista
		vertices_list.push_back(i);
	}
    std::vector<int> cycle(1,0); // create and initialize an auxiliary list

    int max_size_cycle=0;   // Define Max size list found  // Method 1 != Method 2

	while (true) {
		DEBUG std::cerr << "Searching for (" << cycle_size<< ") cycle size" << std::endl;
		cycle=OpBasic::cycle(graph, cycle_size, seek, root);
		if (cycle.empty()){
			if (cycle_size > 3 && cycle_size > max_size_cycle){ // Não achei ciclo do tamanho pré-definido, procuro um menor
				cycle_size--;
				continue;
			}
			else {  // I didn't even find a triangle, then I quit the loop
				break;
			}
		} else {
			select_cycles.push_back(cycle);
            break; 
		}
	}
}


/* ****************************************************************
    SEARCH FOR MANY CYCLES --- CHANGE LOGIC 
**************************************************************** */
void search_for_induced_cycles_for_M4v2(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph){
    int initial_max_cycle=cycle_size;

    std::vector<std::pair<int,float>> centrality;
    std::vector<int> neighbors;
    std::vector<int> processed;                     // Processed vertices
	std::vector<int> vertices_list;                 // Vertices to be processed


    std::vector<int> vertices(vertices_closeness.size(), 0);
    for (int i = 0; i < vertices_closeness.size(); i++) vertices_closeness[i] = 0;



	for (int i=0; i<graph.get_qty_vertex(); i++){   //inicializa a lista
		vertices_list.push_back(i);
	}
    std::vector<int> cycle(1,0); // create and initialize an auxiliary list

    int max_size_cycle=0;   // Define Max size list found  // Method 1 != Method 2

    int cycles_found = 0;
    std::vector<int> neighbor_root=graph.adjList(root);
    int counter=0;
    seek=neighbor_root[counter];


	while (true) {
		DEBUG std::cerr << "Searching for (" << cycle_size<< ") cycle size" << std::endl;
		cycle=OpBasic::cycle(graph, cycle_size, seek, root);
        if (cycle.empty()){
            counter++;
            if (counter >= neighbor_root.size()){
                counter = 0;
                root++;
                if (root>=graph.get_num_vertices()){
                    root = 0;
                }
                continue;
            }
            seek=neighbor_root[counter];
            continue;
        } 
        //cycle=OpBasic::cycle(graph, cycle_size);
		for (int j=0; j < cycle.size(); j++){ // Delete processed vertices
			int index=get_index(cycle[j], vertices_list);
			if (!vertices_list.empty() && index < vertices_list.size())
				vertices_list.erase (vertices_list.begin() + index);
			else
                if (vertices_list.empty() ) 
				    break;
		}
		cycle.push_back(root);
		processed.push_back(root);

        int acme=cycle.size()-1;    //auxiliary var
        if (!cycle.empty() && acme > max_size_cycle){   // Method 1 != Method 2
    		select_cycles.clear();                              // Only cycles that has same size(maximum select)
            select_cycles.push_back(cycle);                     // will be selected
            if (cycle_size > max_size_cycle)
                max_size_cycle=acme;
        } else if (!cycle.empty() && acme == max_size_cycle){   
            select_cycles.push_back(cycle);
        }

		if (vertices_list.empty()) break;

		bool gameover = false;
		while (!gameover){  // Escolhe nova raiz
            root = Centrality::tiebreaker(vertices_list, vertices_closeness, vertices_leverage);
        
			if (!in(root, processed) || vertices_list.empty()){
				gameover=true;
			} else if (in(root, processed)){

                for (auto it = vertices_list.begin(); it != vertices_list.end();) {
                    if (*it == root) {
                        it = vertices_list.erase(it);
                    } else {
                        ++it;
                    }
                }

            }
		}
		gameover = false;
		neighbors = graph.adjList(root);
		while (!gameover){
            seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage);
			if (!in(seek, processed)){
				gameover=true;
			} else {
				int index=get_index(seek, neighbors);
				neighbors.erase (neighbors.begin() + index);
				if (neighbors.empty()) break;
			}
		}
		if (neighbors.empty() || cycles_found >= (global_induced_cycle - 1)) break;
        cycles_found++;

        // For new cycle, select new root
        root++;
        if (root>=graph.get_num_vertices()){
            root = 0;
        }
        neighbor_root=graph.adjList(root);
        counter=0;
        seek=neighbor_root[counter];
	}
}


/*
    SEARCH FOR MANY CYCLES
*/
void search_for_induced_cycles_for_M4(int seek, int root, int cycle_size, std::vector<std::vector<int>> &select_cycles, std::vector<float> &vertices_closeness, std::vector<float> &vertices_leverage, Graph &graph){
    int initial_max_cycle=cycle_size;

    std::vector<std::pair<int,float>> centrality;
    std::vector<int> neighbors;

    std::vector<int> processed;                     // Processed vertices
	std::vector<int> vertices_list;                 // Vertices to be processed
	for (int i=0; i<graph.get_qty_vertex(); i++){   //inicializa a lista
		vertices_list.push_back(i);
	}
    std::vector<int> cycle(1,0); // create and initialize an auxiliary list

    int max_size_cycle=0;   // Define Max size list found  // Method 1 != Method 2

    int cycles_found = 0;
    std::vector<int> neighbor_root=graph.adjList(root);
    int counter=0;
    seek=neighbor_root[counter];


	while (true) {
		DEBUG std::cerr << "Searching for (" << cycle_size<< ") cycle size" << std::endl;
		cycle=OpBasic::cycle(graph, cycle_size, seek, root);
        if (cycle.empty()){
            counter++;
            if (counter >= neighbor_root.size()){
                counter = 0;
                root++;
                if (root>=graph.get_num_vertices()){
                    root = 0;
                }
                continue;
            }
            seek=neighbor_root[counter];
            continue;
        } 
        //cycle=OpBasic::cycle(graph, cycle_size);
		for (int j=0; j < cycle.size(); j++){ // Delete processed vertices
			int index=get_index(cycle[j], vertices_list);
			if (!vertices_list.empty() && index < vertices_list.size())
				vertices_list.erase (vertices_list.begin() + index);
			else
                if (vertices_list.empty() ) 
				    break;
		}
		cycle.push_back(root);
		processed.push_back(root);

        int acme=cycle.size()-1;    //auxiliary var
        if (!cycle.empty() && acme > max_size_cycle){   // Method 1 != Method 2
    		select_cycles.clear();                              // Only cycles that has same size(maximum select)
            select_cycles.push_back(cycle);                     // will be selected
            if (cycle_size > max_size_cycle)
                max_size_cycle=acme;
        } else if (!cycle.empty() && acme == max_size_cycle){   
            select_cycles.push_back(cycle);
        }

		if (vertices_list.empty()) break;

		bool gameover = false;
		while (!gameover){  // Escolhe nova raiz
            root = Centrality::tiebreaker(vertices_list, vertices_closeness, vertices_leverage);
        
			if (!in(root, processed) || vertices_list.empty()){
				gameover=true;
			} else if (in(root, processed)){

                for (auto it = vertices_list.begin(); it != vertices_list.end();) {
                    if (*it == root) {
                        it = vertices_list.erase(it);
                    } else {
                        ++it;
                    }
                }

            }
		}
		gameover = false;
		neighbors = graph.adjList(root);
		while (!gameover){
            seek = Centrality::tiebreaker(neighbors, vertices_closeness, vertices_leverage);
			if (!in(seek, processed)){
				gameover=true;
			} else {
				int index=get_index(seek, neighbors);
				neighbors.erase (neighbors.begin() + index);
				if (neighbors.empty()) break;
			}
		}
		if (neighbors.empty() || cycles_found >= (global_induced_cycle - 1)) break;
        cycles_found++;

        // For new cycle, select new root
        root++;
        if (root>=graph.get_num_vertices()){
            root = 0;
        }
        neighbor_root=graph.adjList(root);
        counter=0;
        seek=neighbor_root[counter];
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
std::vector<std::pair<int,int>> make_edges_list(std::vector<int> cycle_vertices_list){
    int size=cycle_vertices_list.size();
    std::vector<std::pair<int,int>> edges_list;
    for (int i=0; i < size - 1; i++){
        edges_list.push_back(std::make_pair(cycle_vertices_list[i], cycle_vertices_list[i+1]));
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
std::vector<std::vector<std::pair<int,int>>> make_edges_list(std::vector<std::vector<int>> vector1){
    int size=vector1.size();
    std::vector<std::vector<std::pair<int,int>>> edges_list;
    std::vector<std::pair<int,int>> auxiliary;
    std::pair<int,int> edge;
    for (auto it = vector1.begin(); it != vector1.cend(); ++it){
        for (auto itt = it->begin(); itt != it->cend() - 1; itt++){
            edge = std::make_pair(*(itt), *(itt+1));
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
std::vector<std::pair<int,int>> make_edges_list(std::vector<std::vector<std::pair<int, int>>> cycle_vertices_list){
    std::vector<std::pair<int,int>> result;
    for (auto it = cycle_vertices_list.begin(); it != cycle_vertices_list.cend(); ++it){
        for (auto itt = it->begin(); itt != it->cend(); ++itt){
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
std::vector<std::vector<std::pair<int, int>>> cycle_combinations(std::vector<std::pair<int, int>> edges, int cycle_length) {
    
    std::vector<std::vector<std::pair<int, int>>> result;

    std::function<void(int, std::vector<std::pair<int, int>>)> backtrack = [&](int start, std::vector<std::pair<int, int>> current_cycle) {
        if (current_cycle.size() == cycle_length) {
            result.push_back(current_cycle);
        } else {
            for (auto edge : edges) {
                if (edge.first == start && find(current_cycle.begin(), current_cycle.end(), edge) == current_cycle.end()) {
                    current_cycle.push_back(edge);
                    backtrack(edge.second, current_cycle);
                    current_cycle.pop_back();
                }
            }
        }
    };

    for (int i = 0; i < edges.size(); i++) {
        std::vector<std::pair<int, int>> current_cycle = {edges[i]};
        backtrack(edges[i].second, current_cycle);
    }

    return result;
}