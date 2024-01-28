#include "stretch.hpp"
#include "opBasic.hpp"
#include "genGraph.hpp"
//#include "frontier.hpp"
#include "graph.hpp"
#include "../my_libs/library3.hpp"

#include <tuple>
#include <iostream>
#include <cmath>

#include "../Debug.h"

extern bool global_noindex;
extern bool global_save_tree;

//extern float global_running_time;
extern bool abort_for_timeout;
//bool abort_for_timeout = false;

// class MyWatchdogTimer : public WatchdogTimer
// {
// public:
//   virtual void on_timeout()
//   {
//     // To do when your dog is bark.
//     abort_for_timeout = true;
//   }
// };

/*************************************************************************************************
Faziam parte do arquivo sequential_functions.cpp
**************************************************************************************************/
void Stretch::sequential(Graph& graph){
    find_index(graph);
}

/**
 * @details Find a stretch index from a graph.
 * Returns a integer that represents a stretch index.
 * That's the main method, the sequential t-admissibility algorithm.
 * @param g a graph instance that represents a graph.
 * @return a integer the graph contains a cycle.
 */
void Stretch::find_index(Graph& g)
{
    int prox_vizinho[g.getQtdVertices()];
    int ult_colocado[g.getQtdVertices()];
    int v = 0;
    int u;
    int arv = 0;
    int grt;
    //index = INF_VALUE;
    //index = (int)INFINITY;
    index = std::numeric_limits<int>::max();


    //OpBasic op; // alteracao LF

    Graph tree(g.getQtdVertices());
    //grt = op.maxLowerCicle(g); // alteração DJ
    grt = g.get_grt();

    for(int i=0; i < g.getQtdVertices(); ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    std::string fileName = "SEQsaida.txt";  // REMOVER - APENAS PARA TESTE
    std::string str_tree;                   // REMOVER - APENAS PARA TESTE

    while( v >= 0 and !(abort_for_timeout)){
        if( prox_vizinho[v] == g.grau(v) ){
            prox_vizinho[v] = 0;
            --v;
            if(v < 0) break; // acaba o algoritmo
            tree.remove_aresta(v, ult_colocado[v]);
            ult_colocado[v] = -1;

        }else{
            u = g.adjList(v)[prox_vizinho[v]];
            ++prox_vizinho[v];
            if( not tree.possui_aresta(v, u) ) {
                tree.add_aresta(v, u);
                ult_colocado[v] = u;
                bool has_cycle_var = false;
                if (OpBasic::cyclic(tree, v)){
                    has_cycle_var = true;
                }
                if (!has_cycle_var)
                    if (OpBasic::cyclic(tree, u))
                        has_cycle_var = true;
                if(not has_cycle_var){
                //if(not OpBasic::is_cyclic(tree)){
                    if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                        int f=1;
                        if (!global_noindex)
                            f = Stretch::find_factor(g, tree);
                        ++arv;
                        //g.sum_tree();
                        g.add_tree();

                        if (global_save_tree)                   // REMOVER - APENAS PARA TESTE             
                        {                                       // REMOVER - APENAS PARA TESTE
                            str_tree = tree_to_string(tree);    // REMOVER - APENAS PARA TESTE
                            str_tree = ">" + str_tree;          // REMOVER - APENAS PARA TESTE
                            save_tree(str_tree, fileName);      // REMOVER - APENAS PARA TESTE
                        }                                       // REMOVER - APENAS PARA TESTE

                        if(f < index){
                            index = f;
                            this->tree = tree;
                            if(index == grt-1){// alteracao LF
                              break;// alteracao LF
                            }// alteracao LF
                        }
                    }else{
                        ++v;
                        continue;
                    }
                }
                tree.remove_aresta(v, u);
            }
        }
    }
    this->total_arv = arv;
    g.set_stretch_index(index);
    g.set_best_tree(tree);
    //return index;
}

void Stretch::find_index_edge(Graph& g)
{
    //Frontier f;
    int n = g.getQtdVertices();
    int m = g.getQtdArestas();
    int indice[n-1];
    int grt;
    std::vector<int> edges = OpBasic::edges(g);

    // f.print(edges, "TESTE: ");
    int j = 0;
    indice[j] = 0;
    Graph tree(n);

    int arv = 0;
    //index = INF_VALUE;
    index = std::numeric_limits<int>::max();

    while(indice[0]/2 <= m-(n-1)){
        if( indice[j]/2 > m-(n-1-j) ){
            --j;
            tree.remove_aresta(edges[indice[j]], edges[indice[j]+1]);
            indice[j] += 2;
        }
        else {
            tree.add_aresta(edges[indice[j]], edges[indice[j]+1]);
            if( !OpBasic::is_cyclic(tree) ){
                if(j == n-2){ // achou uma arvore geradora
                    int f=1;
                    if (!global_noindex)
                        f = Stretch::find_factor(g, tree);
                    ++arv;
                    if(f < index){
                        index = f;
                        this->tree = tree;
                        if(index == grt-1){// alteracao LF
                          break;// alteracao LF
                        }// alteracao LF
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

        // f.printAdjList(tree, "ARVORE:");
    }
    this->total_arv = arv;
}

void Stretch::find_index_pararell(Graph& g, int raiz, int start, int end)
{
    int prox_vizinho[g.getQtdVertices()];
    int ult_colocado[g.getQtdVertices()];
    int v = raiz;
    int u;
    // int arv = 0; // debug
    //index = INF_VALUE;
    index = std::numeric_limits<int>::max();

    Graph tree(g.getQtdVertices());

    for(int i=0; i < g.getQtdVertices(); ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;

    while( true ){
        if(v == raiz){
            if(prox_vizinho[v] == end){
                break; // Fim do algoritmo
            }
        }

        if( prox_vizinho[v] == g.grau(v) ){
            prox_vizinho[v] = 0;
            v = g.ant_vertex(v);
            // if(v < 0) break; // acaba o algoritmo
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
                        int f=1;
                        if (!global_noindex)
                            f = Stretch::find_factor(g, tree);
                        // ++arv;
                        if(f < index){
                            //mtx.lock(); // LOCK

                            this->index = f;
                            this->tree = tree;
                            //mtx.unlock(); // NU_LOCK
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
    // std::cout << "total arv = " << arv << std::endl;
}

// CONTINUAR AQUI
void Stretch::find_index_thread(Graph& g)
{

    int raiz = -1;
    for(int i=0; i < g.getQtdVertices(); ++i){
        if(g.grau(i) > raiz){
            raiz = i;
        }
    }

    find_index_pararell(g, raiz, 0, g.adjList(raiz).size());
/*
    //std::thread vetor_th[g.adjList(raiz).size()];
    int limite = g.adjList(raiz).size()-1;
    for(int i=0; i < limite; ++i){
        //vetor_th[i] = std::thread(&Stretch::find_index_pararell, this, g, raiz, i, i+1);
    }

    for(int i=0; i < limite; ++i){
        vetor_th[i].join();
    }
*/
    /*
    for(int v=0; v < limite; ++v){
        vetor_th.push_back(std::thread (&Stretch::find_index_pararell, this, g, raiz, v, v+1) );
        // vetor_th.push_back(std::thread (g, raiz, v, v+1)  );
    }

    for(auto& h : vetor_th){
        h.join();
    }
    */
}

int Stretch::find_factor(Graph& g, Graph& tree)
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

void Stretch::find_index_cycle(Graph& g, int m)
{
    std::vector<Graph> l;
    //int index_menor = INF_VALUE;
    int index_menor = std::numeric_limits<int>::max();
    GenGraph::all_sub(g, l, m);

    int count = 1;
    for( Graph g : l){
        DEBUG std::cerr << "inicio do grafo: " << count << std::endl;
        find_index(g);
        DEBUG std::cerr << "fim do grafo: " << count << std::endl;
        if (index < index_menor){
            index_menor = index;
        }
        //index = INF_VALUE;
    }
    index = index_menor;
}

int Stretch::lowerBound(Graph& g)
{
    return OpBasic::maxLowerCicle(g) - 1;
}

/**
 * @details Find a stretch index from a graph.
 * Returns a integer that represents a stretch index.
 * That's the main method, the sequential t-admissibility algorithm.
 * @param g a graph instance that represents a graph.
 * @return a integer the graph contains a cycle.
 */
int Stretch::find_index(Graph &original, Graph &change)
{
    Graph G2=original;
    int prox_vizinho[change.getQtdVertices()];
    int ult_colocado[change.getQtdVertices()];
    int v = 0;
    int u;
    int arv = 0;
    int grt;
    //index = INF_VALUE;
    index = std::numeric_limits<int>::max();

    OpBasic op; // alteracao LF

    Graph tree(change.getQtdVertices());
    grt = op.maxLowerCicle(change); // alteração DJ

    for(int i=0; i < change.getQtdVertices(); ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    while( v >= 0 and !(abort_for_timeout)){
        if( prox_vizinho[v] == change.grau(v) ){
            prox_vizinho[v] = 0;
            --v;
            if(v < 0) break; // acaba o algoritmo
            tree.remove_aresta(v, ult_colocado[v]);
            ult_colocado[v] = -1;

        }else{
            u = change.adjList(v)[prox_vizinho[v]];
            ++prox_vizinho[v];
            if( not tree.possui_aresta(v, u) ){
                tree.add_aresta(v, u);
                ult_colocado[v] = u;
                if(not OpBasic::is_cyclic(tree)){
                    if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                        int f=1;
                        if (!global_noindex)
                            f = Stretch::find_factor(G2, tree);
                        ++arv;
                        //g.sum_tree();
                        change.add_tree();
                        if(f < index){
                            index = f;
                            this->tree = tree;
                            if(index == G2.grt - 1){// alteracao LF
                              break;// alteracao LF
                            }// alteracao LF
                        }
                    }else{
                        ++v;
                        continue;
                    }
                }
                tree.remove_aresta(v, u);
            }
        }
    }
    this->total_arv = arv;
    change.set_stretch_index(index);
    change.set_best_tree(tree);
    return index;
}