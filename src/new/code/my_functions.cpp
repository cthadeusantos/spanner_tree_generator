#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#include <iostream>
#include "graph.hpp"
#include "opBasic.hpp"
#include "stretch.hpp"

///Basic debugging controller. See Debug.h for details.
#ifdef MN_BF_SEQ_DEBUG
	#define DEBUG
#else
	#define DEBUG while(false)
#endif

#include "../Debug.h"
#include "my_functions.hpp"
#include "../ctfunctions2.cpp"

using namespace std;
sem_t semaforo;
int total_arv = 0;
std::mutex mtx;


/*************************************************************************************************
Faziam parte do arquivo sequential_functions.cpp
**************************************************************************************************/
void sequential(Graph& graph){
	Stretch acme; // Lonney Tunes rocks!
	acme.find_index(graph);
}


/*************************************************************************************************
Faziam parte do arquivo parallel_functions.cpp
**************************************************************************************************/


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
    sem_wait(&semaforo); // Apenas 4 threads puderam fazer este código por vez
    int n = g.getQtdVertices();
    int m = g.getQtdArestas();

    int prox_vizinho[n];
    int ult_colocado[n];
    int v = raiz;
    int u;
    int arv = 0; // debug
    int index_local = INF_VALUE;
    Graph tree_local;

    Graph tree(n);

    // PASSEI O GRT para atributo do grafo
    //OpBasic op;
    ////int raiz;
    //grt = op.maxLowerCicle(g); // alteracao DJ

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;

    while(g.get_stretch_index() > g.grt-1 ){
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
                        int f = find_factor(g, tree);
                        ++arv;
                        g.add_tree();
                        
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            if(index_local == g.grt-1){// alteracao LF
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

    DEBUG std::cerr << "thread " << id << " criou " << arv << " arvores." << std::endl;

    if(index_local < g.get_stretch_index()) {
        total_arv += arv;
        g.set_stretch_index(index_local);
        g.set_best_tree(tree_local);
    }
    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
}

//! Calculate stretch index - cycle
/*!
    Calculate a stretch index using an induced cycle's edge
    \param g a graph instance that represents the graph
    \param raiz an integer that represents the vertex root
    \param neighbor_start an integer that represents the vertex neighbor the root
    \param id an integer that represents a number of thread
    \param edges_list a vector that represents induced cycle's edge to be remove
    \param end
*/
void find_index_induced_cycle(Graph &graph, int raiz, int neighbor_start, const int id, std::vector<int> edges_list )
{
    
    sem_wait(&semaforo);
	mtx.lock();
    Graph G1 = graph;   // Auxiliary graph - local graph
    mtx.unlock();

    // remove the root edges from a cycle of previous threads
    // Edges removed will processed another threads
    vector<int> remove = {};
    remove = edges_list;

    mtx.lock();     // add by Thadeu - Avoid message -> malloc(): corrupted top size Aborted (core dumped)
    for (int i=0; i < id * 2 ; i=i+2){
        G1.remove_aresta(remove[i], remove[i+1]);
    } // end of prodedure to remove the root edges
    mtx.unlock();   // add by Thadeu

    bool disconnected = false;  // Can a tree be built?
    int start = G1.neighbor_index(raiz, neighbor_start); // Convert start vertex to start index from adjacency list
    int end = start + 1;            // Condition to stop algorithm 
    int n = G1.getQtdVertices();    // num vertices
    int m = G1.getQtdArestas();     // num edges
    int prox_vizinho[n] = {};       // next neighbor (index)
    int ult_colocado[n] = {};       // last add vertex at tree(index)
    int v = raiz;
    int u = -1;
    int index_local = INF_VALUE;

    Graph tree_local(n);
    Graph tree(n);

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;

    if (G1.grau(v) == 0) disconnected = true; // There's no way to build a tree
    
    while(G1.get_stretch_index() > G1.grt - 1 && G1.grau(v) > 0 && !(disconnected)) {
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
                                int f = find_factor(graph, tree);
                                mtx.unlock();
                        G1.add_tree();

                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            if(index_local == G1.grt -1){
                              break;
                            }
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
    DEBUG std::cerr << "thread " << id << " criou " << G1.get_total_tree() << " arvores." << std::endl;
    mtx.lock();
        graph.sum_trees(G1.get_total_tree());
        if (index_local < graph.get_stretch_index()) {
            graph.set_stretch_index(index_local);
            graph.set_best_tree(tree_local);
        }
    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
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

int vertice_maior_grau(Graph& g)
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

void create_threads(Graph& g)
{
    int raiz = vertice_maior_grau(g);
    int qtd = g.grau(raiz);
    int id=0;

    std::thread vetor_th[qtd];

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    OpBasic op; // by thadeu
    g.grt = op.maxLowerCicle(g); // by thadeu
    // fim calcula grt

    if(g.possui_aresta(raiz, g.ant_vertex(raiz) ) ){
        id = adj_id(g, raiz, g.ant_vertex(raiz) );
    }

    for(int i=0; i < qtd; ++i){
        vetor_th[i] = std::thread(find_index_parallel, std::ref(g), raiz, id, id+1, i);
        id = next(id, qtd);
    }

    // std::cout << "bye";

    for(int i=0; i < qtd; ++i){
        vetor_th[i].join();
    }
}

void create_threads_big_cycle(Graph& g) {
    int qty = -1;
    int root = -1;
    int neighbor = -1;
    std::vector<int> edges_list = {};
    std::vector<int> neighbors;
    //int pos = -1;

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    OpBasic op; // by thadeu
    g.grt = op.maxLowerCicle(g); // by thadeu
    // fim calcula grt
    
    int n = g.get_qty_vertex();
    root = vertice_maior_grau(g);
    int max_degree = g.grau(root);
    int cycle_size;
 
  /*   if (type == 1){
        DEBUG std::cerr << "Configurando para max degree: " << std::endl;
        qty = max_degree;
        neighbors = g.adjList(root);
    } else { */
        std::vector<int> big_cycle;
        // find the largest induced cycle
        
        for (int i = n; i > 2; i--){
            DEBUG std::cerr << "Procurando ciclo induzido de tamanho : " << i << std::endl;
            big_cycle = op.cycle(g, i);
            cycle_size = big_cycle.size();
            if (cycle_size != 0){
                DEBUG std::cerr << "Achei ciclo induzido de tamanho :" << cycle_size << std::endl;
                break;
            }
        }
        qty = cycle_size;
        neighbors = big_cycle;
/*     } */
           
    std::thread vetor_th[qty];
    for(int i = 0; i < qty; ++i){
/*         if (type == 0){
            neighbor = g.adjList(root)[i];
        } else { */
            root = neighbors[i];
            neighbor = (i+1 == qty) ? neighbors[0] : neighbors[i+1];
/*         } */

        vetor_th[i] = std::thread(find_index_induced_cycle, std::ref(g), root, neighbor, i, std::ref(edges_list));
        mtx.lock();
        edges_list.push_back(root);
        edges_list.push_back(neighbor);
        mtx.unlock();
    }


    for(int i = 0; i < qty; ++i){
        vetor_th[i].join();
    }
    
}

/* void create_threads_without_trees_duplications(Graph& g)
{
    int qty = -1;
    int root = -1;
    int neighbor = -1;

    std::vector<int> edges_list = {};
    int pos = -1;

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    OpBasic op; // by thadeu
    g.grt = op.maxLowerCicle(g); // by thadeu
    // fim calcula grt


    
    int n = g.get_qty_vertex();
    root = vertice_maior_grau(g);
    int max_degree = g.grau(root);
    int cycle_size;

    std::vector<int> big_cycle;
    // find the largest induced cycle
    cerr << "Procurando ciclo induzido de tamanho :\n";
    for (int i = n; i > 2; i--){
        big_cycle = op.cycle(g, i);
        cycle_size = big_cycle.size();
        if (cycle_size != 0){
            cerr << "Achei ciclo induzido de tamanho :" << cycle_size ;
            break;
        }
    }
    
    std::vector<int> neighbors;
    int type = 0;
    if (max_degree > cycle_size){
        qty = max_degree;
        neighbors = g.adjList(root);
        type = 0;   // Define type building to max_degree template
    } else{
        qty = cycle_size;
        neighbors = big_cycle;
        type = 1;   // Define type building to biggest induced cycle template
    }
        
    std::thread vetor_th[qty];
    for(int i = 0; i < qty; ++i){
        if (type == 0){
            neighbor = g.adjList(root)[i];
        } else {
            root = neighbors[i];
            neighbor = (i+1 == qty) ? neighbors[0] : neighbors[i+1];
        }

        //vetor_th[i] = std::thread(find_index_induced_cycle, std::ref(g), root, index, pos, i);
        vetor_th[i] = std::thread(find_index_induced_cycle, std::ref(g), root, neighbor, i, std::ref(edges_list));
        edges_list.push_back(root);
        edges_list.push_back(neighbor);
        //edges_list.push_back(g.adjList(root)[index]);
        //index = next(index, qty);
        pos++;
    }

    for(int i = 0; i < qty; ++i){
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