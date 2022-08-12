#include "code/graph.hpp"
#include "code/genGraph.hpp"
#include "code/frontier.hpp"
#include <iostream>
#include <cstdlib>

#include <sstream> // std::stringstream

int main(int argc, char const *argv[])
{
    if (argc != 3){
        std::cerr << "Usage: " << argv[0] << " <# vertex> <probability> " << std::endl;
        exit(-1);
    }
    
    int num_vertices = atoi(argv[1]);
    double probabilidade = atof(argv[2]);
    
    srand (time(NULL));  
    Frontier f;  
    Graph newGraph(num_vertices);

    GenGraph::generate_graph(newGraph, probabilidade);
    
    // Escreve cada grafo em seu proprio arquivo
    std::ofstream graphOut;
    int cont = 0;
    std::string graphFileName("./grafos/g_"
                            + std::to_string(num_vertices) 
                            + "_" 
                            + std::to_string(newGraph.getQtdArestas()) 
                            + "_" 
                            + std::to_string((int)(probabilidade*100))
                            + ".txt");

    graphOut.open(graphFileName);
    if( graphOut.is_open() ){
        f.printAdjMat_2(newGraph, "", graphOut);
        ++cont;
    } else {
        std::cerr << "ERROR ao abrir o arquivo: grafo " << std::endl;
        return -1;
    }
    graphOut.close();
    

    std::cout << newGraph.getQtdVertices() << "," << newGraph.getQtdArestas() << std::endl;

    std::cout << "Done!" << std::endl;

    return 0;
}
