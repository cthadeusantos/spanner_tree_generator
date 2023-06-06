#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#include <initializer_list>
#include <cmath>
#include <iostream>
#include <tuple> // for tuple
#include <string>

#include "../code/graph.hpp"
#include "../code/opBasic.hpp"
#include "../code/stretch.hpp"
#include "../code/cycles.hpp"
#include "../code/centrality.hpp"

#include "library3.hpp"

#include "../Debug.h"

#include <chrono>	// contributor AZ

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

