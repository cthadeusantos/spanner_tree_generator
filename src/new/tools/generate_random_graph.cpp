/**
 * @file create_graph_generic.cpp
 * @authors { Daniel Juventude / Luis Felipe / Leandro Santiago }
 * @authors { Carlos Thadeu [CT] / Anderson Zudio[AZ](contributor)}
 * @brief Application to create generic graphs - This a modify version from original
 * 
 */

#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy
//#include <thread>  // std::thread
#include <mutex>   // std::mutex

#include "../code/initial_settings.hpp"
#include "../code/parameters.hpp"

#include "../code/graph.hpp"
#include "../code/genGraph.hpp"
#include "../code/frontier.hpp"
#include "../my_libs/library3.hpp"
#include <iostream>
#include <cstdlib>

#include <sstream> // std::stringstream

int main(int argc, char const *argv[])
{
    if (argc != 5){
        std::cerr << "Usage: " << argv[0] << " <# vertex> <probability> <quantity> <output_directory>" << std::endl;
        exit(-1);
    }
    
    int num_vertices = atoi(argv[1]);
    float probabilidade = atof(argv[2]);
    int quantity = atoi(argv[3]);
    std::string directory = corrigirDiretorio(argv[4]);
    
    srand (time(NULL));  
    Frontier f;  
    Graph newGraph(num_vertices);

    for (int i=0; i < quantity; i++){
        newGraph.clear_arestas();
        GenGraph::generate_graph2(newGraph, probabilidade);
        
        // Escreve cada grafo em seu proprio arquivo
        std::ofstream graphOut;
        //int cont = 0;
        // std::string graphFileName("./grafos/g_"
        //                         + std::to_string(num_vertices) 
        //                         + "_" 
        //                         + std::to_string(newGraph.getQtdArestas()) 
        //                         + "_" 
        //                         + std::to_string((int)(probabilidade*100))
        //                         + ".txt");
        std::string graphFileName(directory 
                                + std::to_string(num_vertices) 
                                + "_" 
                                + std::to_string(newGraph.getQtdArestas()) 
                                + "_" 
                                + std::to_string((int)(probabilidade*100))
        );
        //                        + "-1");
        graphFileName = encontrarArquivoInexistente(graphFileName, ".txt", quantity);                    

        graphOut.open(graphFileName);
        if( graphOut.is_open() ){
            f.printAdjMat_2(newGraph, "", graphOut);
            //++cont;
        } else {
            std::cerr << "ERROR ao abrir o arquivo: grafo " << graphFileName << std::endl;
            return -1;
        }
        graphOut.close();
    }

    

    std::cout << newGraph.getQtdVertices() << "," << newGraph.getQtdArestas() << std::endl;

    std::cout << "Done!" << std::endl;

    return 0;
}