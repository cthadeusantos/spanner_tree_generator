#ifndef OP_BASIC_HPP_
#define OP_BASIC_HPP_

#include <stack>
#include <queue>
#include <limits>
#include <iostream>
#include <vector>
#include <limits>
//#include <stack> // pilha
//#include <queue> // fila
//#include <algorithm> // std::find
//#include <string>

#include <boost/graph/adjacency_list.hpp>

#include "graph.hpp"

#define INF_VALUE std::numeric_limits<int>::max()
#define INF "inf"

class OpBasic
{
private:
    // static void clear(std::queue<int>& q);
    // static void clear(std::stack<int>& s);
    // static bool pertence(int x, std::vector<int>& v);
    // static std::vector<int> short_path(Graph& g, int v, int u);
    

    // static void teste(std::vector<int> v, std::string msg = "");
    // static void graphToMatrix(Graph& g, int* m, int ordem);
    // static void multiplicacaoMatricial(int* a, int* b, int* c, int ordem);
    // static int traco(int* m, int ordem);
    // static void initMatrix(int* m, int ordem, int val=0);
public:
    // Define types using base types
    using Base = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
    using Vertex = boost::graph_traits<Base>::vertex_descriptor;
    using Edge = boost::graph_traits<Base>::edge_descriptor;

    OpBasic() { }
    ~OpBasic() { }

    // MAX number of lowest cicles with edge of g
    static int maxLowerCicle(Graph &g);

    // retorna o tamanho do menor ciclo sem corda que possua a aresta(u,v) de g
    static int lowestCicleWithEdge(Graph &g, int u, int v);

    // Retorna uma lista de vertices que formam um ciclo sem corda de tamanho 'n'
    static std::vector<int> cycle(Graph &g, int n);
    static std::vector<int> cycle(Graph &g, int n, int u, int v);
    static std::vector<int> cycle2(Graph &g, int n, Vertex u, Vertex v);
    
    static void find_cycle(Graph &g, int i, int n, const int start,std::vector<int>& v, bool* visitado, bool* achou);
    static void find_cycle2(Graph &g, int size_cycle, Vertex i,  const Vertex start, std::vector<int> &cycle_vertices, bool *visited, bool *found);
    static void find_cycle2(Graph &g, int size_cycle, Vertex i, const Vertex start, std::vector<int>& verticesOFcycle, std::vector<bool>& visited, bool& found);
    static int shortest_cycle(const boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>& g);

    // Grafo possui corda ?
    static bool has_chord(Graph& g, std::vector<int> ciclo);
    //static bool has_chord(Graph& g, const std::vector<int>& ciclo);// ESSE AQUI FOI GERADO QUANDO FOI REFATORADO, TESTAR

    //static bool cyclic(Graph& g, int v);
    static bool cyclic(Graph& g, int v);

    static bool is_cyclic(Graph& g);

    // static bool is_tree(Graph& g);
    static std::vector<int> diference_edge(Graph &g, Graph &t);

    static int distance(Graph& g, int v, int u);
    
    // static int girth(Graph& g);
    // static std::vector<int> centers(Graph& g);
    // static int diameter(Graph& g);
    // static std::vector<std::vector<int> > distance(Graph& g, int v);
    // static int min_diameter_tree_value(Graph& g);
    // static bool is_valid(Graph& g);
    // static bool is_connected(Graph& g);
    // static bool has_universal_vertex(Graph& g);
    // static bool has_leaf(Graph& g);
    // static bool equal(Graph& g1, Graph& g2);

    // static void rename(Graph& g, int v, int u);
    // static void copy(Graph& g1, Graph& g2); // g1 = g2;

    // static bool myXor(bool a, bool b);
    // static bool myXnor(bool a, bool b);

    // // Retorna um vetor de arestas do grafo. a cada dois int conscutivo é uma aresta.
    // static std::vector<int> edges(Graph& g);
    
    // // Retorna um vetor de arestas de g, mas ordenado pelo vertice de maior grau
    // static std::vector<int> edges_by_bigger_degree(Graph& g);

    // // Retorna um vetor de arestas que se retirar as arestas a partir do inicio, o grafo nunca sera desconexo.
    // static std::vector<int> edges_conected(Graph& g);



    // // Retorna uma lista de vertices que formam um ciclo sem corda de tamanho 'n'
    // // que possua a aresta (u,v)
    // static std::vector<int> cycle(Graph& g, int n, int u, int v);
    
    // static std::vector<int> biggestCycle(Graph& g);
    // static std::vector<int> biggestCycleUpDown(Graph& g);
    // static int quantidadeDeTriangulos(Graph& g);

    // // retorna o tamanho do maior ciclo sem corda que possua a aresta(u,v) de g
    // static int biggestCicleWithEdge(Graph& g, int u, int v);




    // // This method was private, I change it to public because I need this at heuristic
    // static int sumGrade(Graph& g, std::vector<int> v);


    // // Get the eccentricity of a vertex
    // static int eccentricity(Graph &graph, int vertex);

    // static bool has_cycle(Graph &graph, int vertex);

    // static void dfs(Graph &graph, int vertex, std::vector<bool>& visited);
    // static bool canReachAllVertices(Graph &graph, int startVertex);

};

#endif