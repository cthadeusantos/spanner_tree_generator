#pragma once

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
