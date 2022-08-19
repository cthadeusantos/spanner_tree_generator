#include "graph.hpp"

Graph::Graph(int n)
{
    // graph = new std::vector<std::vector<int> >(n, std::vector<int>());
    qtdVertices = n;
    qtdArestas = 0;
    for(int i = 0; i < n; ++i){
        graph.push_back(std::vector<int>());
    }
}

Graph::Graph()
{
    // graph = new std::vector<std::vector<int> >(n, std::vector<int>());
    qtdVertices = 0;
    qtdArestas = 0;
}


Graph::~Graph()
{
    // delete graph;
}

void Graph::add_vertex()
{
    ++qtdVertices;
    graph.push_back(std::vector<int>());
}

void Graph::add_aresta(int v, int u)
{
    std::vector<int>::iterator it;

    it = graph[v].begin();
    while(it != graph[v].end() && *it < u){
        ++it;
    }
    graph[v].insert(it, u);

    it = graph[u].begin();
    while(it != graph[u].end() && *it < v){
        ++it;
    }
    graph[u].insert(it, v);

    //graph[v].push_back(u);
    //graph[u].push_back(v);
    ++qtdArestas;
}

void Graph::add_aresta(std::vector<int> arestas)
{
    for (std::vector<int>::iterator it = arestas.begin(); it != arestas.end(); it += 2){
        add_aresta(*it, *(it+1));
    }
}

void Graph::remove_aresta(int v, int u)
{
    std::vector<int>::iterator it;

    it = graph[v].begin();
    while(it != graph[v].end() && *it != u){
        ++it;
    }
    graph[v].erase(it);

    it = graph[u].begin();
    while(it != graph[u].end() && *it != v){
        ++it;
    }
    graph[u].erase(it);

    --qtdArestas;
}

void Graph::add_all_vertice(int n)
{
    for(int i=0; i < n; ++i){
        graph.push_back(std::vector<int>());
    }
    qtdVertices = n;
}

const std::vector<int> Graph::adjList(int v)
{
    return graph[v];
}

bool Graph::possui_aresta(int v, int u)
{  
    for(int x : adjList(v)){
        if(x == u){
            for(int y : adjList(u)){
                if(y == v){
                    return true;
                }
            }
        }
    }
    return false;
}

int Graph::grau(int v)
{
    return graph[v].size();
}

void Graph::clear_arestas()
{
    for(int i=0; i< getQtdVertices(); ++i){
        graph[i].clear();
    }
    qtdArestas = 0;
}

void Graph::clear()
{
    clear_arestas();
    this->graph.clear();
    qtdVertices = 0;
}

int Graph::next_vertex(int v)
{
    ++v;
    return v == getQtdVertices() ? 0 : v;
}

int Graph::ant_vertex(int v)
{
    --v;
    return v < 0 ? getQtdVertices()-1 : v;
}

const std::vector<int> Graph::edgeList()
{
    std::vector<int> aux;
    for(int v=0; v < getQtdVertices(); ++v){
        for(int u : adjList(v)){
            if(u > v){
                aux.push_back(v);
                aux.push_back(u);
            }
        }
    }
    return aux;
}

int Graph::maior_grau()
{
    int maior = grau(0);
    for(int i = 1; i < getQtdVertices(); ++i){
        int teste = grau(i);
        if(teste > maior){
            maior = teste;
        }
    }
    return maior;
}

int Graph::vertice_maior_grau()
{
    int maior = grau(0);
    int v = 0;
    for(int i = 1; i < getQtdVertices(); ++i){
        int teste = grau(i);
        if(teste > maior){
            maior = teste;
            v = i;
        }
    }
    return v;
}

std::vector<int> Graph::vertices_de_maior_grau()
{
    std::vector<int> list_vertices;
    int maior_grau = this->maior_grau();

    for(int i = 0; i < getQtdVertices(); ++i)
    {
        if(grau(i) == maior_grau){
            list_vertices.push_back(i);
        }
    }

    return list_vertices;
}

int Graph::qtd_vertex_grau(int grau)
{
    int count = 0;
    for(int i = 0; i < getQtdVertices(); ++i)
    {
        if( this->grau(i) == grau)
        {
            ++count;
        }
    }
    return count;
}