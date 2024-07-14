#ifndef STRETCH_HPP
    #define STRETCH_HPP

#include <tuple>
#include <iostream>
#include <cmath>
//#include <thread>
//#include <mutex>

#include "parameters.hpp"
#include "opBasic.hpp"
#include "graph.hpp"

#include "../Debug.h"

//#include "graph.hpp"

//#include "genGraph.hpp"
//#include "frontier.hpp"

//#include "../my_libs/library3.hpp"





class Stretch
{
    private:
        int index;
        Graph tree;
        //std::mutex mtx;
        //int total_arv;
        
        //void find_index_pararell(Graph& g, int raiz, int start, int end); 

    // static tambem não funcionou... // static void find_index_pararell(Graph& g, int raiz, int start, int end);

        // calculo do limite inferior para o index
        int lowerBound(Graph& g);

    public:
        Stretch() { 
            index = -1;
            tree = Graph();
            //total_arv=0;
        }

        ~Stretch() { }

        void sequential(Graph& g);

        void find_index(Graph& g);
        void find_index_thread(Graph& g);

        static int find_factor(Graph& g, Graph& tree);

        int getIndex(){ return index; }
        Graph getTree(){ return tree; }
        //int getTotalTree(){ return total_arv; }

        void find_index_edge(Graph& g);

        void find_index_cycle(Graph& g, int m);

        // Não teu certo... // friend void find_index_pararell(Strech* s, Graph& g, int raiz, int start, int end); DJ

        // Provavelmente esses dois metodos ficaram desnecessários
        // depois que criei os atributos bestindex e besttree em Graph
        // Mantidos até ter certeza que não são necessários CT
        void setIndex(int val) { index = val; } // Precisei criar por força maior... DJ
        void setTree(Graph t) { tree = t; } // Precisei criar por força maior... DJ


        int find_index(Graph &original, Graph &change); //
};

#endif