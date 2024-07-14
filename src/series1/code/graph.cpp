#include "graph.hpp"

// Constructor
Graph::Graph() = default;

//! Constructor
/*!
    Build a graph with n vertices and no edges
    \param n a integer that represents the vertex number to add
*/
Graph::Graph(int n) {
    // graph = new std::vector<std::vector<int> >(n, std::vector<int>());
    //qtdVertices = n;
    //qtdArestas = 0;
    // for (int i = 0; i < n; ++i)
    // {
    //     //graph.push_back(std::vector<int>());
    //     graph.addVertex();
    // }
    this->addVertices(n);
}

// Function to add a vertex
Graph::Vertex Graph::addVertex() {
    return boost::add_vertex(*this);
}

// Function to add n vertices
void Graph::addVertices(int n) {
    for (int i = 0; i < n; ++i) {
        this->addVertex();
    }
}

// Function to add n vertices
// Graph::Vertex Graph::add_vertices(int n) {
//     Vertex lastVertex;
//     for (int i = 0; i < n; ++i) {
//         lastVertex = boost::add_vertex(*this);
//     }
//     return lastVertex; // Retorna o último vértice adicionado
// }

// Function to add an edge
std::pair<Graph::Edge, bool> Graph::addEdge(Vertex u, Vertex v) {
    return boost::add_edge(u, v, *this);
}

// Function to get the number of vertices
std::size_t Graph::numVertices() const {
    return boost::num_vertices(*this);
}

// Function to get the number of edges
std::size_t Graph::numEdges() const {
    return boost::num_edges(*this);
}

// Function to get the degree of a vertex
std::size_t Graph::getDegree(Vertex v) const {
    return boost::degree(v, *this);
}

//! Function to get the list of adjacency to a vertex
/*!
    Function to get the list of adjacency to a vertex
    \param v a integer that represents the vertex number
    \return a vector that represents an adjacency list vector
*/
std::vector<Graph::Vertex> Graph::getAdjacentVertices(Vertex v) const {
    std::vector<Vertex> adjacentVertices;
    auto [begin, end] = boost::adjacent_vertices(v, *this);
    for (auto it = begin; it != end; ++it) {
        adjacentVertices.push_back(*it);
    }
    return adjacentVertices;
}

// Determines if the edge exists
bool Graph::has_edge(int v, int u){
    return boost::edge(u, v, *this).second;
}

//! Gets the list of edges
/*!
    Gets the list of edges
    \return a vector of integer
*/
std::vector<int> Graph::edgeList() const{
    std::vector<int> aux;
    for (int v = 0; v < this->numVertices(); ++v)
    {
        for (int u : this->getAdjacentVertices(v))
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

// Gets the girth of a graph
unsigned int Graph::get_girth(){
    return this->girth;
}

// Function to remove an edge
void Graph::removeEdge(Vertex u, Vertex v) {
    boost::remove_edge(u, v, *this);
}

void Graph::sum_trees()
{
    this->total_trees++;
}

void Graph::sum_trees(unsigned long long int value)
{
    this->total_trees = this->total_trees + value;
}

void Graph::set_stretch_index(unsigned int value){
    this->stretch_index = value;
}

unsigned int Graph::stretchIndex(){
    return this->stretch_index;
}

unsigned long long int Graph::totalTrees(){
    return this->total_trees;
}