#include "graph.hpp"

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