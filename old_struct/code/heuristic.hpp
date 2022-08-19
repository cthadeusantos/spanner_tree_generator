#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include "graph.hpp"
#include "strech.hpp"
#include "opBasic.hpp"

class Heuristic
{
private:
    static void my_quicksort(std::vector<int>& vertices, int began, int end, Graph& g);
    static int func_aux_h2(Graph& tree, Graph& g, int v);
    static void my_sort(std::vector<int>& v1, std::vector<int>& v2);
public:
    static int heuristica_1(Graph& g);
    static int heuristica_2(Graph& g);

    static Graph heuristica_1_tree(Graph& g);
    static Graph heuristica_2_tree(Graph& g);
    static Graph heuristica_2_tree(Graph& g, int raiz);
    static Graph heuristica_3_tree(Graph& g);

    static Graph heuristica_2_global(Graph& g);
};



#endif