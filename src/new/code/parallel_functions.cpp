#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy
//#include <sstream> // std::stringstream

#include <iostream>
#include "graph.hpp"
//#include "frontier.hpp"
#include "opBasic.hpp"
//#include "codigo/strech.hpp"
//#include <iostream>
//#include <ctime>
//#include <sys/time.h>

///Basic debugging controller. See Debug.h for details.
#ifdef MN_BF_SEQ_DEBUG
	#define DEBUG
#else
	#define DEBUG while(false)
#endif

#include "../Debug.h"
#include "parallel_functions.hpp"

using namespace std;
sem_t semaforo;
int total_arv = 0;
std::mutex mtx;

void find_index_parallel(Graph& g, int raiz, int start, int end, const int id)
{
    sem_wait(&semaforo); // Apenas 4 threads puderam fazer este código por vez
    //std::cout << "Thread running on Core " << sched_getcpu() << std::endl;
    //std::cout << "Maximum cores " << std::thread::hardware_concurrency() << std::endl;
    int n = g.getQtdVertices();
    int m = g.getQtdArestas();

    int prox_vizinho[n];
    int ult_colocado[n];
    int v = raiz;
    int u;
    int arv = 0; // debug
    int index_local = INF_VALUE;
    Graph tree_local;

    //int grt;

    //Frontier front;

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

    //while(index_global > grt-1 ){
    //while(g.get_stretch_index() > grt-1 ){
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

        }else{
            u = g.adjList(v)[prox_vizinho[v]];
            ++prox_vizinho[v];
            if( not tree.possui_aresta(v, u) ){
                tree.add_aresta(v, u);
                ult_colocado[v] = u;
                if(not OpBasic::is_cyclic(tree)){
                    if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                        int f = find_factor(g, tree);
                        ++arv;
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            //if(index_local == grt-1){// alteracao LF
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

    //std::cout << "thread " << id << " criou " << arv << " arvores." << std::endl;

    DEBUG std::cerr << "thread " << id << " criou " << arv << " arvores." << std::endl;

    //if(index_local < index_global) {
    //bool achou = false;
    if(index_local < g.get_stretch_index()) {
        total_arv += arv;
        g.set_stretch_index(index_local);
        //index_global = index_local;
        //tree_global = tree_local;
        g.set_best_tree(tree_local);
        //achou = true;
        //g.best_tree = tree_local;
    }
    mtx.unlock();
    //if (achou){g.set_best_tree(tree_local);}
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

void create_threads_without_trees_duplications(Graph& g)
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