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
        static void heuristica_1(Graph &g);
        static void heuristica_1_V2(Graph &g); // by Thadeu
        static void heuristica_2(Graph &g);
        static void heuristica_2_V2(Graph &g); // by Thadeu
        static void heuristica_3v1(Graph& g); // By thadeu
        static void heuristica_3v2(Graph& g); // By thadeu
        static void breadth_heuristic_1(Graph& g); // By thadeu
        static void breadth_heuristic_2(Graph &graph);
        
        static std::vector <int> breadth_criterion(Graph &graph, std::queue <int> &FILA,
                    std::vector <int> &visited, std::vector <int> &total_layer); // By thadeu

        static bool sortby2nd_des(const std::tuple<int, int> &a, 
                    const std::tuple<int, int> &b); // By thadeu    
        static bool sortby2nd_asc(const std::tuple<int, int> &a, 
                    const std::tuple<int, int> &b); // By Thadeu
        static void set_graph_final_parameters(int &index_local, Graph &tree_local, Graph &graph);

        // OLD FUNCTIONS
        // Developed by Daniel to return a best tree
        // and not stretch index like heuristica_X_global
        static Graph heuristica_1_tree(Graph& g);
        static Graph heuristica_2_tree(Graph& g);
        static Graph heuristica_2_tree(Graph& g, int raiz);
        static Graph heuristica_3_tree(Graph& g);
        static Graph heuristica_2_global(Graph& g);
};

#endif
