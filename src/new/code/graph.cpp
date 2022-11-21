#include "graph.hpp"
#include "opBasic.hpp"
#include <iostream>
#include <tuple>


//! Constructor
/*!
    Build a graph with n vertices and no edges
    \param n a integer that represents the vertex number to add
*/
Graph::Graph(int n)
{
    // graph = new std::vector<std::vector<int> >(n, std::vector<int>());
    qtdVertices = n;
    qtdArestas = 0;
    for (int i = 0; i < n; ++i)
    {
        graph.push_back(std::vector<int>());
    }
}

//! Constructor
/*!
    Build a graph without vertices or edges
*/
Graph::Graph()
{
    // graph = new std::vector<std::vector<int> >(n, std::vector<int>());
    qtdVertices = 0;
    qtdArestas = 0;
}

//! Destructor
/*!
    Write a detailed description
*/
Graph::~Graph()
{
    // delete graph;
}

//! Add vertex
/*!
    Add new vertex to graph
*/
void Graph::add_vertex()
{
    ++qtdVertices;
    graph.push_back(std::vector<int>());
}

//! Add new edge
/*!
    Add new edge to graph
    \param v a integer that represents the vertex
    \param u a integer that represents the neighborhood
*/
void Graph::add_aresta(int v, int u)
{
    std::vector<int>::iterator it;

    it = graph[v].begin();
    while (it != graph[v].end() && *it < u)
    {
        ++it;
    }
    graph[v].insert(it, u);

    it = graph[u].begin();
    while (it != graph[u].end() && *it < v)
    {
        ++it;
    }
    graph[u].insert(it, v);

    // graph[v].push_back(u);
    // graph[u].push_back(v);
    ++qtdArestas;
}

//! Add new edge(s)
/*!
    Add new edge(s) to graph from a vector
    \param arestas a vector
*/
void Graph::add_aresta(std::vector<int> arestas)
{
    for (std::vector<int>::iterator it = arestas.begin(); it != arestas.end(); it += 2)
    {
        add_aresta(*it, *(it + 1));
    }
}

//! Delete an edge
/*!
    Delete an edge from graph
    \param v a integer that represents the vertex
    \param u a integer that represents the neighbor
*/
void Graph::remove_aresta(int v, int u)
{
    std::vector<int>::iterator it;

    it = graph[v].begin();
    while (it != graph[v].end() && *it != u)
    {
        ++it;
    }
    graph[v].erase(it);

    it = graph[u].begin();
    while (it != graph[u].end() && *it != v)
    {
        ++it;
    }
    graph[u].erase(it);

    --qtdArestas;
}

//! Add all new vertices
/*!
    Add new vertices
    \param n a integer that represents the numbers of vertices of graph
*/
void Graph::add_all_vertice(int n)
{
    for (int i = 0; i < n; ++i)
    {
        graph.push_back(std::vector<int>());
    }
    qtdVertices = n;
}

//! Return the adjacency list from a vertex
/*!
    Return the adjacency list from a vertex
    \param v a integer that represents the vertex number
    \return a vector of integer that represents an adjacency list vector
*/
const std::vector<int> Graph::adjList(int v)
{
    return graph[v];
}

//! Check if edge exist
/*!
    Seek at vertex's adjacency list if edge exist
    \param v a integer that represents the vertex
    \param u a integer that represents the neighborhood
    \return a boolean
*/
bool Graph::possui_aresta(int v, int u)
{
    for (int x : adjList(v))
    {
        if (x == u)
        {
            for (int y : adjList(u))
            {
                if (y == v)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

//! Return a vertex degree
/*!
    Return the degree's vertex
    \param v a integer that represents the vertex
    \return a integer
*/
int Graph::grau(int v)
{
    return graph[v].size();
}

//! Delete all edges
/*!
    Destroy the list of adjacencies of all vertices
*/
void Graph::clear_arestas()
{
    for (int i = 0; i < getQtdVertices(); ++i)
    {
        graph[i].clear();
    }
    qtdArestas = 0;
}

//! Clear edges
/*!
    Delete edges of all vertices
*/
void Graph::clear()
{
    clear_arestas();
    this->graph.clear();
    qtdVertices = 0;
}

//! Go to next vertex
/*!
    Go to next vertex
    \param v a integer that represent a actual vertex
    \return a integer
*/
int Graph::next_vertex(int v)
{
    ++v;
    return v == getQtdVertices() ? 0 : v;
}

//! Go to previous vertex
/*!
    Go to previous vertex
    \param v a integer that represent a actual vertex
    \return a integer
*/
int Graph::ant_vertex(int v)
{
    --v;
    return v < 0 ? getQtdVertices() - 1 : v;
}

//! An edge list
/*!
    Return an edge list
    \return a vector of integer
*/
const std::vector<int> Graph::edgeList()
{
    std::vector<int> aux;
    for (int v = 0; v < getQtdVertices(); ++v)
    {
        for (int u : adjList(v))
        {
            if (u > v)
            {
                aux.push_back(v);
                aux.push_back(u);
            }
        }
    }
    return aux;
}

//! Get max degree
/*!
    Seek for max degree
    \return a integer
*/
int Graph::maior_grau()
{
    int maior = grau(0);
    for (int i = 1; i < getQtdVertices(); ++i)
    {
        int teste = grau(i);
        if (teste > maior)
        {
            maior = teste;
        }
    }
    return maior;
}

//! Get vertex with max degree
/*!
    Seek for vertex with max degree
    \return a integer
*/
int Graph::vertice_maior_grau()
{
    int maior = grau(0);
    int v = 0;
    for (int i = 1; i < getQtdVertices(); ++i)
    {
        int teste = grau(i);
        if (teste > maior)
        {
            maior = teste;
            v = i;
        }
    }
    return v;
}

//! List all vertices with max degree
/*!
    Seek for all vertices with max degree
    \return a vector
*/
std::vector<int> Graph::vertices_de_maior_grau()
{
    std::vector<int> list_vertices;
    int maior_grau = this->maior_grau();

    for (int i = 0; i < getQtdVertices(); ++i)
    {
        if (grau(i) == maior_grau)
        {
            list_vertices.push_back(i);
        }
    }

    return list_vertices;
}

//! Get quantity of vertices with max degree
/*!
    Count the number of vertices with highest degree
    \param grau is an integer
    \return an integer
*/
int Graph::qtd_vertex_grau(int grau)
{
    int count = 0;
    for (int i = 0; i < getQtdVertices(); ++i)
    {
        if (this->grau(i) == grau)
        {
            ++count;
        }
    }
    return count;
}

/*
    Methods created by Carlos Thadeu
*/

int Graph::get_num_edges()
{
    return qtdArestas;
};

//! Set a best tree found
/*!
    After found a best tree , assing it to graph
    \param n a graph that represents a local tree from a thread
*/
void Graph::set_best_tree(Graph graph)
{
    this->best_tree = {};
    for (int i = 0; i < graph.edgeList().size(); i = i + 2)
    {
        this->best_tree.push_back(std::make_tuple(graph.edgeList()[i], graph.edgeList()[i+1]));
    }
};

//! Get a best tree found
/*!
    Return a best tree found
    \return a vector of tuples that represents edges from best tree
*/
std::vector<std::tuple<int, int>> Graph::get_best_tree(){
    return this->best_tree;
}

//! Show at screen a best tree found
/*!
    Show at screen a best tree found
*/
void Graph::show_best_tree()
{
    int node1 = 0, node2 = 0;
    std::cerr << "Stretch index : " << this->get_stretch_index() << "\n" << std::endl;
    for (auto&& tuple: this->best_tree){
        std::tie(node1, node2) = tuple;
        std::cerr << "(" << node1 << " , " << node2 << ") ";
    }
    std::cerr << "\n" ;
};

//! Assign value to stretch index
/*!
    Assign value to stretch index
    \param n a integer that represents the stretch index
*/
void Graph::set_stretch_index(int value)
{
    this->stretch_index = value;
}

//! Get value to stretch index
/*!
    Get value to stretch index
    \return an integer that represents stretch index
*/
int Graph::get_stretch_index()
{
    return this->stretch_index;
}

//! Add new vertices to graph create without vertices Graph() 
/*!
    Add new vertices to graph create without vertices Graph()
    \param n a integer that represents the number of vertices of graph
*/
void Graph::add_vertices(int n)
{
    for (int i = 0; i < n; ++i)
    {
        this->add_vertex();
    };
};

std::vector<int> Graph::induced_cycle(Graph &g, int n){
    OpBasic a = OpBasic();
    return a.cycle(g, n);
}

bool Graph::has_chord(Graph &g, std::vector<int> x){
    OpBasic a = OpBasic();
    return a.possuiCorda(g, x);
}

void Graph::add_tree(){
    this->total_tree++;
}

void Graph::sum_trees(int value){
    this->total_tree = this->total_tree + value;
}

int Graph::get_total_tree(){
    return this->total_tree;
}

int Graph::get_qty_vertex(){
    return this->qtdVertices;
}

int Graph::neighbor_index(int vertex, int neighbor){
    int index = -1;
    int size = this->adjList(vertex).size();
    for (int i = 0; i < size; ++i)
        {
            if (this->adjList(vertex)[i] == neighbor){
                index = i;
                break;
            }
        }
    return index;
}