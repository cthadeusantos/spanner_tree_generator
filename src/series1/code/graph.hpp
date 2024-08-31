#pragma once
<<<<<<< HEAD

#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <vector>

// A definition of the graph type using adjacency_list
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;


class MyGraph : public Graph {
private:
    //int myPrivateVariable;
    //void myPrivateMethod();
public:
    // Method for adding an edge to the graph
    void addEdge(int source, int target);

    // Method for printing all the edges of the graph
    void printEdges();

    bool hasEdge(int source, int target);
};
=======
#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include "../boost/boost/graph/adjacency_list.hpp"
#include "../boost/boost/graph/graph_traits.hpp"
#include <iostream>

class Graph : public boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
{
private:
    unsigned long long int total_trees {0};
    unsigned int stretch_index {0};
    unsigned int lower_bound {1};
public:
    int girth {0};
    std::vector<std::tuple<int, int>> best_tree;

    // Define types using base types
    using Base = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
    using Vertex = boost::graph_traits<Base>::vertex_descriptor;
    using Edge = boost::graph_traits<Base>::edge_descriptor;

    // Constructors
    Graph();
    Graph(int n);

    // Destructor
    ~Graph() {
        // Perform any necessary cleanup (if any) here.
        // For example, free dynamically allocated resources.
    }

    // Function to add a vertex
    Graph::Vertex addVertex();

    // Function to add n vertices
    Vertex add_vertices(int n);

    // Function to get the number of vertices
    std::size_t numVertices() const;

    // Function to get the number of edges
    std::size_t numEdges() const;

    // Function to get the degree of a vertex
    std::size_t getDegree(Vertex v) const; 
    // Função para obter vértices adjacentes
    //std::vector<int> adjList(Vertex v) const;

    // Function to get the list of adjacency to a vertex
    std::vector<Graph::Vertex> getAdjacentVertices(Vertex v) const;

    // Determines if the edge exists
    bool has_edge(int v, int u);

    // Gets the list of edges
    std::vector<int> edgeList() const;

    // Gets the girth of a graph 
    unsigned int get_girth();

    // Adds n vertices to a graph
    void addVertices(int n); 
    // Graph::Vertex add_vertices(int n);

    // Function to remove an edge
    void removeEdge(Vertex u, Vertex v); 
    
    // Function to add an edge
    std::pair<Graph::Edge, bool> addEdge(Vertex u, Vertex v);

    // Sum a new tree to the trees counter
    void sum_trees();

    // Sum 'n' trees to the trees counter
    void sum_trees(unsigned long long int value);

    // Return the total trees
    unsigned long long int totalTrees();

    // Set the graph stretch index
    void set_stretch_index(unsigned int value);

    // Return the graph stretch index 
    unsigned int stretchIndex();
    
    // Set the graph lower bound
    void set_lowerBound(unsigned int value);

    //return the graph lower bound
    unsigned int get_lowerBound();
};

#endif // GRAPH_HPP_
>>>>>>> e7cc67dd368e6754eecfeb8af0629ae6939ae369
