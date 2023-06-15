#include <limits>
#ifndef GRAPH_HPP_
#define GRAPH_HPP_
#define INF_VALUE std::numeric_limits<int>::max()

#include <vector>
#include <tuple>
#include <set>
#include <cmath>

class Graph
{
private:
    std::vector< std::vector<int> > graph;
    int qtdVertices;
    int qtdArestas;
    int stretch_index = 1; // by Thadeu
    int total_tree = 0; // by Thadeu
    bool signal = true;
    int girth = 0;
    int lower_limit = 1;

public:
    Graph(int n);
    Graph();
    ~Graph();
    
    void set_grt(Graph graph);
    int get_grt();
    void set_lower_limit(int value);
    void set_lower_limit(Graph graph);
    int get_lower_limit();
    //void create(int dimension);
    void add_vertex();
    void add_aresta(int v, int u);
    void add_aresta(std::vector<int> arestas);
    //void delete_vertex(int vertex); // by thadeu
    void remove_aresta(int v, int u);
    void clear_arestas();
    void clear();
    void add_all_vertice(int n); // add de 0 até n-1
    bool possui_aresta(int v, int u);
    bool has_edge(int v, int u);
    int grau(int v);
    int maior_grau();
    int highest_degree_vertex();
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
    int grt = 1; // by Thadeu
    std::vector<std::tuple<int, int> > best_tree;
    int get_num_edges();
    int get_num_vertices();
    void set_best_tree(Graph graph);
    std::vector<std::tuple<int, int> > get_best_tree();
    void add_vertices(int n);
    void show_best_tree();
    std::vector<int> induced_cycle(Graph &g, int n);
    bool has_chord(Graph &g, std::vector<int> x);

    void add_tree();
    
    void sum_trees();
    void sum_trees(int value);
    void reset_trees();
    void reset_trees(int value);
    int get_total_tree();

    void set_girth(int value);
    int get_girth();

    int get_qty_vertex();
    int neighbor_index(int vertex, int neighbor);
    int eccentricity(Graph &graph, int vertex);

    std::vector<int> DFS(int s);
    std::vector<int> BFS(int s);
    
    //void split_in_subgraphs(std::set<int> articulations, std::vector<std::vector<int>> &subgraph, Graph &g);
    //Graph build_subgraph(std::vector<int> &split_in_subgraphs);
    bool get_signal();
    void set_signal();

    std::vector<int> my_connected_vertices();

    static void my_insertionSort_graph(std::vector <int> &vector1, std::vector <int> &vector2, char order);
    static void my_insertionSort_graph(std::vector <float> &vector1, std::vector <int> &vector2, char order);
    std::vector<int> select_max_degree_vertices_at_list(int max, std::vector <int> &vector1, std::vector <int> &vector2);

    std::vector<std::pair<int, int>> get_edges_set();
    std::vector<int> get_neighbors(int vertex);
    std::vector<std::vector<int>> get_neighbors();

    //int waist(std::vector<std::vector<int>> graph);
    //int waist();

    void show_edges();
    void check_integrity();



};

#endif