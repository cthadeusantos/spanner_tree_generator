#ifndef STRECH_HPP_
#define STRECH_HPP_

#include "graph.hpp"
#include <thread>
#include <mutex>

class Strech
{
private:
    int index;
    Graph tree;
    std::mutex mtx;
    int total_arv;
    
    void find_index_pararell(Graph& g, int raiz, int start, int end); 

   // static tambem não funcionou... // static void find_index_pararell(Graph& g, int raiz, int start, int end);

    // calculo do limite inferior para o index
    int lowerBound(Graph& g);

public:
    Strech() { index = -1; tree = Graph(); total_arv=0; }
    ~Strech() { }

    void find_index(Graph& g);
    void find_index_thread(Graph& g);

    int find_factor(Graph& g, Graph& tree);

    int getIndex(){ return index; }
    Graph getTree(){ return tree; }
    int getTotalTree(){ return total_arv; }

    
    void find_index_edge(Graph& g);

    void find_index_cycle(Graph& g, int m);


    // Não teu certo... // friend void find_index_pararell(Strech* s, Graph& g, int raiz, int start, int end);

    void setIndex(int val) { index = val; } // Precisei criar por força maior...
    void setTree(Graph t) { tree = t; } // Precisei criar por força maior...
};

#endif