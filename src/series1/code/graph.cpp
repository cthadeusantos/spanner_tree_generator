#include "graph.hpp"

<<<<<<< HEAD
void MyGraph::addEdge(int source, int target) {
    boost::add_edge(source, target, *this);
}

void MyGraph::printEdges() {
    std::cout << "Edges:" << std::endl;
    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(*this); ei != ei_end; ++ei) {
        std::cout << boost::source(*ei, *this) << " -- " << boost::target(*ei, *this) << std::endl;
    }
}

    bool MyGraph::hasEdge(int source, int target) {
        return boost::edge(source, target, *this).second;
    }
=======
// Constructor
Graph::Graph() = default;

//! Constructor
/*!
    Build a graph with n vertices and no edges
    \param n a integer that represents the vertex number to add
*/
Graph::Graph(int n) {
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

void Graph::set_lowerBound(unsigned int value){
    this->lower_bound = value;
};

unsigned int Graph::get_lowerBound(){
    return this->lower_bound;
};
>>>>>>> e7cc67dd368e6754eecfeb8af0629ae6939ae369