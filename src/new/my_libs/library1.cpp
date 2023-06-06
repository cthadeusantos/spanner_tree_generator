#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#include <initializer_list>
#include <cmath>
#include <iostream>
#include <tuple> // for tuple
#include <string>
#include <chrono>	// contributor AZ

#include "../code/graph.hpp"
#include "../code/opBasic.hpp"
#include "../code/stretch.hpp"
#include "../code/cycles.hpp"
#include "../code/centrality.hpp"

#include "library1.hpp"
#include "library3.hpp"

#include "../Debug.h"

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

// int find_factor(Graph& g, Graph& tree)
// {
//     std::vector<int> list = OpBasic::diference_edge(g, tree);
//     std::vector<int>::iterator it;
//     int factor = 1;

//     it = list.begin();
//     while(it != list.end()){
//         int v = *it;
//         int u = *(it+1);
//         int d = OpBasic::distance(tree, v, u);
//         if(factor < d){
//             factor = d;
//         }
//         it = it + 2;
//     }

//     return factor;
// }

// int highest_degree_vertex(Graph& g)
// {
//     int raiz = -1;
//     int maior_grau = -1;
//     for(int i=0; i < g.getQtdVertices(); ++i){
//         if(g.grau(i) > maior_grau){
//             raiz = i;
//             maior_grau = g.grau(i);
//         }
//     }
//     return raiz;
// }

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
 * @brief Auxiliary function to setting all parameters after stretch index found
 * @details Auxiliary function to setting all parameters after stretch index found
 * @author Carlos Thadeu
 */
void set_graph_final_parameters(int &index_local, Graph &tree_local, Graph &graph){
    if(index_local < graph.get_stretch_index() && index_local != 1) {
        graph.set_stretch_index(index_local);
        graph.set_best_tree(tree_local);
        if (index_local==graph.grt - 1){
            graph.set_signal();
            //processando.store(false,std::memory_order_release);
        }
    }
}

