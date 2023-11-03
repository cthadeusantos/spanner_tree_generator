#ifndef HEURISTIC_HPP
    #define HEURISTIC_HPP

#include "graph.hpp"
#include "stretch.hpp"
#include "opBasic.hpp"

class Heuristic
{
    private:
        
        static int func_aux_h2(Graph& tree, Graph& g, int v);
        static int func_aux_h3(Graph& tree, Graph& g, int v);
        static void my_sort(std::vector<int>& v1, std::vector<int>& v2);

    public:
        static void my_quicksort(std::vector<int>& vertices, int began, int end, Graph& g);
        static void heuristica_1v1(Graph &g);
        static void heuristica_1v2(Graph &g); // by Thadeu
        static void heuristica_1v3(Graph &g); // by Eriky
        static void heuristica_1v4(Graph &g); // by Eriky
        static void heuristic_2v1(Graph &g);
        static void heuristica_2v2(Graph &g); // by Thadeu
        static void heuristica_3v1(Graph& g); // By thadeu
        static void heuristica_3v2(Graph& g); // By thadeu
        static void Heuristica_4v1(Graph& g); // By thadeu
        static void Heuristica_4v2r1(Graph &graph);
        // Heuristic 4v2-2 Create only purpose compare with H4v2
        static void Heuristica_4v2r2(Graph &graph);
        static void Heuristica_4v2r3(Graph &graph);
        static void Heuristica_4v2(Graph &graph);

        static std::vector <int> breadth_criterion(Graph &graph, std::queue <int> &FILA,
                    std::vector <int> &visited, std::vector <int> &total_layer); // By thadeu

        static bool sortby2nd_des(const std::tuple<int, int> &a, 
                    const std::tuple<int, int> &b); // By thadeu    
        static bool sortby2nd_asc(const std::tuple<int, int> &a, 
                    const std::tuple<int, int> &b); // By Thadeu
        //static void set_graph_final_parameters(int &index_local, Graph &tree_local, Graph &graph);

};

#endif
