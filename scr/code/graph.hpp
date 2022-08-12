#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <vector>


class Graph
{
private:
    std::vector<std::vector<int> > graph;
    int qtdVertices;
    int qtdArestas;
public:
    Graph(int n);
    Graph();
    ~Graph();

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
};


#endif