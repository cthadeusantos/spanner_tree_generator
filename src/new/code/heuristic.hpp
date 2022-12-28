#ifndef HEURISTIC_HPP
    #define HEURISTIC_HPP

#include "graph.hpp"
#include "stretch.hpp"
#include "opBasic.hpp"

class Heuristic
{
    private:
        
        static int func_aux_h2(Graph& tree, Graph& g, int v);
        static void my_sort(std::vector<int>& v1, std::vector<int>& v2);

    public:
        static void my_quicksort(std::vector<int>& vertices, int began, int end, Graph& g);
        static int heuristica_1(Graph &g);
        static void heuristica_1_modified(Graph &g); // by Thadeu
        static int heuristica_2(Graph &g);
        static void heuristica_2_modified(Graph &g); // by Thadeu
        static int breadth_heuristic(Graph& g); // By thadeu
        static std::vector <int> breadth_criterion(Graph &graph, std::queue <int> &FILA,
                    std::vector <int> &visited, std::vector <int> &total_layer);

        static Graph heuristica_1_tree(Graph& g);
        static Graph heuristica_2_tree(Graph& g);
        static Graph heuristica_2_tree(Graph& g, int raiz);
        static Graph heuristica_3_tree(Graph& g);

        static Graph heuristica_2_global(Graph& g);

        static bool sortby2nd_des(const std::tuple<int, int> &a, 
                    const std::tuple<int, int> &b); // By thadeu    
        static bool sortby2nd_asc(const std::tuple<int, int> &a, 
                    const std::tuple<int, int> &b); // By Thadeu

};

#endif
