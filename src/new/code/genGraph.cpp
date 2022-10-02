#include "genGraph.hpp"
#include "opBasic.hpp"
#include "frontier.hpp"

#include<iostream>

//! Build a line graph
/*!
    Build a line graph with N vertices
    \param g a graph instance
    \param n a integer
*/
void GenGraph::line(Graph& g, int n)
{
    for(int i=0; i<n-1; ++i){
        g.add_aresta(i, i+1);
    }
}

//! Build a cycle graph
/*!
    Build a cycle graph with N vertices
    \param g a graph instance that represents a graph   
    \param n a integer that represents a number of vertices
*/
void GenGraph::cycle(Graph& g, int n)
{
    line(g, n);
    g.add_aresta(0, n-1);
}

//! Build a potent cycle
/*!
    Build a cycle graph with N vertices
    \param g a graph instance
    \param n an integer
    \param p an integer
*/
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

//! Generate an random number
/*!
    Generate an random number between B and  (B - A + 1)
    \param a an integer
    \param b an integer
    \return an integer
*/
int GenGraph::random(int a, int b)
{
    srand ( time(NULL) ); //to give a different seed
    return (int)rand()%(b-a + 1) + a;
}

//! Generate an floating-point random number
/*!
    Generate an floating-point random number between 0(inclusive) and 1 (exclusive)
    \return a double
*/
double GenGraph::random_01()
{
    srand ( time(NULL) ); //to give a different seed
    return ((double) rand() / (RAND_MAX));
}

//! Generate a graph
/*!
    Generate a graph randomically, the number of vertices was defined previously and 
    the user define a probalility index 
    \param g a graph instance
    \param p a double
*/
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

//! Generate a defined number of graphs with triangles
/*!
    Generate a defined list of graphs randomically generate with vertices number defined randomically
    between two values entered by the user and probability too. 
    \param list a graph instance
    \param vMin an integer
    \param vMax an integer
    \param qtd a integer
    \param p a double
*/
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

//! Generate a defined number of graphs without triangles
/*!
    Generate a defined list of graphs randomically without triangles generate with vertices number defined randomically
    between two values entered by the user and probability too. 
    \param list a vector of vertices a from graph
    \param vMin an integer
    \param vMax an integer
    \param qtd a integer
    \param p a double
*/
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

//! ?
/*!
    ? 
    \param list a vertices list
    \param g a graph instance
*/
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

//! ?
/*!
    ? 
    \param list a vertices list
    \param g a graph instance
    \param ciclo a vector 
*/
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