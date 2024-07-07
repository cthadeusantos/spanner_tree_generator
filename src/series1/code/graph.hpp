#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include "../boost/boost/graph/adjacency_list.hpp"
#include <iostream>

class Graph : public boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
{
public:
    // Defina os tipos utilizando os tipos base
    using Base = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
    using Vertex = boost::graph_traits<Base>::vertex_descriptor;
    using Edge = boost::graph_traits<Base>::edge_descriptor;

    // Construtor
    Graph() = default;

    // Função para adicionar um vértice
    Vertex addVertex() {
        return boost::add_vertex(*this);
    }

    // Função para adicionar uma aresta
    std::pair<Edge, bool> addEdge(Vertex u, Vertex v) {
        return boost::add_edge(u, v, *this);
    }

    // Seu método específico
    void meuMetodo();
};

#endif //GRAPH_HPP_