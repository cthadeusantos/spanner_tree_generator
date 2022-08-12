#ifndef GENGRAPH_HPP_
#define GENGRAPH_HPP_

#include "graph.hpp"

class GenGraph
{

public:
    static void line(Graph& g, int n);
    static void cycle(Graph& g, int n);
    static void potentCycle(Graph& g, int n, int p);

    static int random(int a, int b);
    static double random_01();
    static void generate_graph(Graph& g, double p);
    static void generate_list_graph(std::vector<Graph>& list, int vMin, int vMax, int qtd, double p);
    static void generate_list_graph_without_triangle(std::vector<Graph>& list, int vMin, int vMax, int qtd, double p);
    static void sub_graph_cycle(std::vector<Graph>& list, Graph& g);
    static void sub_graph_cycle(std::vector<Graph>& list, Graph& g, std::vector<int> ciclo);
    static std::vector<Graph> all_sub(Graph& g);
    static void all_sub(Graph& g, std::vector<Graph>& list, int m);

    static void remove_triangles(Graph& g);

};

#endif