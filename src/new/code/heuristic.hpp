#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include "graph.hpp"
#include "stretch.hpp"
#include "opBasic.hpp"

class Heuristic
{
private:
    static void my_quicksort(std::vector<int>& vertices, int began, int end, Graph& g);
    static int func_aux_h2(Graph& tree, Graph& g, int v);
    static void my_sort(std::vector<int>& v1, std::vector<int>& v2);
    static std::vector <int> breadth_criterion( Graph &graph, std::queue <int> &FILA,
            std::vector <int> &VISITED, std::vector <int> &SOMATOTAL); // By Thadeu
public:
    static int heuristica_1(Graph& g);
    static int heuristica_2(Graph& g);
    static int breadth_heuristic(Graph& g); // By thadeu

    static Graph heuristica_1_tree(Graph& g);
    static Graph heuristica_2_tree(Graph& g);
    static Graph heuristica_2_tree(Graph& g, int raiz);
    static Graph heuristica_3_tree(Graph& g);

    static Graph heuristica_2_global(Graph& g);

    static bool sortby2nd_des(const std::tuple<int, int> &a, 
                const std::tuple<int, int> &b); // By thadeu    
    static bool sortby2nd_asc(const std::tuple<int, int> &a, 
                const std::tuple<int, int> &b); // By Thadeu

    static int root_selection(Graph &g); // By Thadeu
};

#endif