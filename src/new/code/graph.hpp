#include <limits>
#ifndef GRAPH_HPP_
#define GRAPH_HPP_
#define INF_VALUE std::numeric_limits<int>::max()

#include <vector>
#include <tuple>

class Graph
{
private:
    std::vector< std::vector<int> > graph;
    int qtdVertices;
    int qtdArestas;
    int stretch_index = INF_VALUE; // by Thadeu
    int total_tree = 0; // by Thadeu
    
public:
    Graph(int n);
    Graph();
    ~Graph();
    
    void create(int dimension);
    void add_vertex();
    void add_aresta(int v, int u);
    void add_aresta(std::vector<int> arestas);
    void remove_aresta(int v, int u);
    void clear_arestas();
    void clear();
    void add_all_vertice(int n); // add de 0 até n-1
    bool possui_aresta(int v, int u);
    int grau(int v);
    int maior_grau();
    int vertice_maior_grau();
    std::vector<int> vertices_de_maior_grau();

    // Assing stretch index calculated to stretch_index attribute
    void set_stretch_index(int value);
    // Get stretch index attribute
    int get_stretch_index();

    // retorna o próximo vertice do grafo. se v for o maior, retorna o 0;
    int next_vertex(int v);
    // retorna o vertice anterior do grafo. se v for 0, retorna o n-1
    int ant_vertex(int v);
    // retorna a quantidade de vertices que possui um dado grau
    int qtd_vertex_grau(int grau=0);

    const std::vector<int> adjList(int v);
    const std::vector<int> edgeList();
    int getQtdVertices(){ return qtdVertices; }
    int getQtdArestas(){ return qtdArestas; }
    std::vector<std::vector<int> > getGraph(){ return graph; }

    // By THADEU
    int grt = 0; // by Thadeu
    std::vector<std::tuple<int, int> > best_tree;
    int get_num_edges();
    void set_best_tree(Graph graph);
    std::vector<std::tuple<int, int> > get_best_tree();
    void add_vertices(int n);
    void show_best_tree();
    std::vector<int> induced_cycle(Graph &g, int n);
    bool has_chord(Graph &g, std::vector<int> x);
    void add_tree();
    void sum_trees(int value);
    int get_total_tree();
    int get_qty_vertex();
    int neighbor_index(int vertex, int neighbor);
    
};


#endif