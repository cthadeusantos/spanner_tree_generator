#include "code/graph.hpp"
#include "code/frontier.hpp"
#include "code/opBasic.hpp"

int main(int argc, char const *argv[])
{
    if(argc < 2){
        std::cerr << "Usage: " << argv[0] << " <path/nome_arq_in.txt>" << std::endl;
        exit(-1);
    }
    Frontier f;
    f.open_in(argv[1]);
    Graph g;
    OpBasic op;

    f.read(g);
    f.close_in();
    f.printAdjList(g, "grafo de entrada", std::cout);

    int tam = OpBasic::maxLowerCicle(g);

    // std::vector<int> tam = OpBasic::cycle(g, 4, 0, 11);

    f.print(tam, "tamanho do maior ciclo: ");

    return 0;
}