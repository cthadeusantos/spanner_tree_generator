#include "genGraph.hpp"
#include "opBasic.hpp"
#include "frontier.hpp"

#include<iostream>

void GenGraph::line(Graph& g, int n)
{
    for(int i=0; i<n-1; ++i){
        g.add_aresta(i, i+1);
    }
}

void GenGraph::cycle(Graph& g, int n)
{
    line(g, n);
    g.add_aresta(0, n-1);
}

void GenGraph::potentCycle(Graph& g, int n, int p)
{
    Graph gAux(n);

    cycle(g, n);
    cycle(gAux, n);

    for(int i=0; i < n; ++i){
        for(int j=i+1; j < n; ++j){
            if( !g.possui_aresta(i,j) && OpBasic::distance(gAux, i, j) <= p){
                g.add_aresta(i, j);
            }
        }
    }
}


int GenGraph::random(int a, int b)
{
    return (int)rand()%(b-a + 1) + a;
}

double GenGraph::random_01()
{
    return ((double) rand() / (RAND_MAX));
}

void GenGraph::generate_graph(Graph& g, double p)
{
    //Frontier f;
    while( !OpBasic::is_valid(g) )
    {
        
        g.clear_arestas();
        
        for( int i=0; i < g.getQtdVertices(); ++i)
        {
            for( int j = i+1; j < g.getQtdVertices(); ++j)
            {
                double flag = random_01();
                if(p > flag)
                {
                    g.add_aresta(i, j);
                }
            }
        }
    }
    //f.printAdjList(g, "teste");
}

void GenGraph::generate_list_graph(std::vector<Graph>& list, int vMin, int vMax, int qtd, double p)
{
    bool flag_igual = false; // evita a criação de grafos indenticos
    for(int i=0; i < qtd; ++i)
    {
        int n = random(vMin, vMax);

        Graph g(n);
        generate_graph(g, p);

        for( Graph g2 : list){
            if( OpBasic::equal(g, g2)){
                flag_igual = true;
            }
        }
        if( flag_igual ){
            flag_igual = false;
        } else {
            list.push_back(g);
        }
    }
}

void GenGraph::generate_list_graph_without_triangle(std::vector<Graph>& list, int vMin, int vMax, int qtd, double p)
{
    Frontier f;
    generate_list_graph(list, vMin, vMax, qtd, p);
    //int count = 1;
    for( Graph& g : list){
        //f.printAdjList(g, "grafo para retirar triangulos");
        remove_triangles(g);
        //std::cout << "grafo " + std::to_string(count) + " criado\n";
        //++count;
    }
}

void GenGraph::sub_graph_cycle(std::vector<Graph>& list, Graph& g)
{
    std::vector<int> ciclo;
    Graph newGraph;

    ciclo = OpBasic::biggestCycle(g);
    
    int qtd = ciclo.size();

    for(int i=0; i < qtd-1; ++i){
        OpBasic::copy(newGraph, g);
        newGraph.remove_aresta(ciclo[i], ciclo[i+1]);
        list.push_back(newGraph);
    }

    OpBasic::copy(newGraph, g);
    newGraph.remove_aresta(ciclo.front(), ciclo.back());
    list.push_back(newGraph);
}

void GenGraph::sub_graph_cycle(std::vector<Graph>& list, Graph& g, std::vector<int> ciclo)
{
    Graph newGraph;    
    int qtd = ciclo.size();

    for(int i=0; i < qtd-1; ++i){
        OpBasic::copy(newGraph, g);
        newGraph.remove_aresta(ciclo[i], ciclo[i+1]);
        list.push_back(newGraph);
    }

    OpBasic::copy(newGraph, g);
    newGraph.remove_aresta(ciclo.front(), ciclo.back());
    list.push_back(newGraph);
}

std::vector<Graph> GenGraph::all_sub(Graph& g)
{
    std::vector<Graph> list;
    std::vector<Graph> listAux;
    std::vector<Graph> listAux2;

    GenGraph::sub_graph_cycle(listAux, g);
    for( Graph gAux : listAux){
        listAux2 = all_sub(gAux);
        list.insert(list.end(), listAux2.begin(), listAux2.end());
        listAux2.clear();
    }
    return list;
}

void GenGraph::all_sub(Graph& g, std::vector<Graph>& list, int m)
{
    //std::vector<Graph> list;
    std::vector<Graph> listAux;

    if (m == 0 || g.getQtdArestas() == g.getQtdVertices() ){
        //std::cout << "*";
        list.push_back(g);
        return;
    }

    GenGraph::sub_graph_cycle(listAux, g);
    for( Graph gAux : listAux){
        all_sub(gAux, list, m-1);
    }
}

void GenGraph::remove_triangles(Graph& g)
{
    // if (OpBasic::quantidadeDeTriangulos(g) == 0) return;
    std::vector<int> ciclo;

    while ( !(ciclo = OpBasic::cycle(g, 3)).empty() ){
        
        int i = 0;
        while( i < ciclo.size()){
            if(i == ciclo.size()-1) { // ultima aresta do ciclo
                if( g.grau(ciclo.front()) > 2 && g.grau(ciclo.back()) > 2){
                    g.remove_aresta(ciclo.front(), ciclo.back());
                    i = ciclo.size(); // sair do loop;
                }
                else {
                    ++i;
                }
            }
            else{
                 if( g.grau(ciclo[i]) > 2 && g.grau(ciclo[i+1]) > 2){
                    g.remove_aresta(ciclo[i], ciclo[i+1]);
                    i = ciclo.size(); // sair do loop;
                }
                else {
                    ++i;
                }
            }
           
        }
        
    }
}