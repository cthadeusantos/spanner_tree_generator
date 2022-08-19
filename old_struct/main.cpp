#include "code/graph.hpp"
#include "code/frontier.hpp"
#include "code/opBasic.hpp"
#include "code/genGraph.hpp"
#include "code/heuristic.hpp"
//#include "code/strech.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <tuple>

#include <sys/stat.h>
#include <sys/types.h> // mkdir

#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy
#include <sstream> // std::stringstream
#include <algorithm> // std::find

int index_global =  INF_VALUE;
int total_arv = 0;
Graph tree_global;

std::mutex mtx;
sem_t semaforo;


void find_index_pararell_edge(Graph& g, std::vector<int> edges, int start, const int id);
void create_threads_edge(Graph& g);
void create_threads_edge_max_degree(Graph& g);
int find_factor(Graph& g, Graph& tree);

void my_quicksort(std::vector<int>& vertices, int began, int end, Graph& g);
int func_aux_h2(Graph& tree, Graph& g, int v);
void my_sort(std::vector<int>& v1, std::vector<int>& v2);

int main(int argc, char const *argv[])
{
    int min_vertices, max_vertices, qtd_grafos, qtd_core;
    double probabilidade;
    char graph_type;
    char paralel_type;
    std::string dictName;

    // ponteiro pra função que vai criar as threads
    void (*create_threads) (Graph&);

    printf("Deseja gerar grafos sem triangulos ('s' para sem triangulos, 'c' para com triangulos): ");
    std::cin >> graph_type;

    printf("Deseja usar paralelismo pelo maior grau('m') ou pela lista desconexa('l'):");
    std::cin >> paralel_type;

    if( paralel_type == 'm')
    {
        create_threads = &create_threads_edge_max_degree;
        dictName = "maxDegree_";
    } else if( paralel_type == 'l') {
        create_threads = &create_threads_edge;
        dictName = "conectList_";
    } else {
        printf("PARALELO NÃO ESPECIFICADO:\nSerá utilizado o de MAIOR GRAU");
        create_threads = &create_threads_edge_max_degree;
        dictName = "paralelDegree_";
    }

    printf("Digite a quantidade minima de vértices: ");
    scanf("%d", &min_vertices);
    printf("Digite a quantidade máxima de vértices: ");
    scanf("%d", &max_vertices);
    printf("Digite a quantidade de grafos: ");
    scanf("%d", &qtd_grafos);
    printf("Digite a probabilidade (0.XX): [para os sem triangulos, usar acima de 0.45] ");
    scanf("%lf", &probabilidade);
    printf("Digite a quantidade de threads que irão rodar ao mesmo tempo (cores): ");
    scanf("%d", &qtd_core);

    
    if( graph_type == 's')
    {
        dictName += "noTri_";
    } else {
        dictName += "grafos_";
    }

    std::string mainDict(dictName 
                            + std::to_string(min_vertices) 
                            + "_" 
                            + std::to_string(max_vertices)
                            + "_"
                            + std::to_string(qtd_grafos)
                            + "_"
                            + std::to_string((int)(probabilidade*100)));
    
    mkdir(mainDict.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);

    std::string graphDict(mainDict + "/grafos");
    mkdir(graphDict.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);

    srand (time(NULL));
    Frontier f;
    //int qtd_core = 4;
    sem_init(&semaforo, 0, qtd_core);

    std::vector<Graph> list;
    std::vector<int> respostaH1(qtd_grafos);
    std::vector<int> respostaH2(qtd_grafos);
    std::vector<int> respostaH2g(qtd_grafos);
    //std::vector<int> respostaH3(qtd_grafos);
    std::vector<int> respostaForce(qtd_grafos);

    if( graph_type == 's'){
        GenGraph::generate_list_graph_without_triangle(list, min_vertices, max_vertices, qtd_grafos, probabilidade); // gera os grafos sem triangulo
    } else {
        GenGraph::generate_list_graph(list, min_vertices, max_vertices, qtd_grafos, probabilidade); // gera os grafos aleatorios
    }

    // escreve cada grafo em seu proprio arquivo
    std::ofstream graphOut;
    int cont = 0;
    for( Graph g : list)
    {
        graphOut.open(graphDict + "/grafo" + std::to_string(cont+1) + ".txt");
        if( graphOut.is_open() ){
            f.printAdjMat_2(g, "", graphOut);
            ++cont;
        } else {
            std::cerr << "ERROR ao abrir o arquivo: " << "grafo " + std::to_string(cont+1) << std::endl;
            return -1;
        }
        graphOut.close();
    }
    
    // escreve os resultados das heuristicas no arquivo "/heuristic.txt"
    std::ofstream heuristicArq(mainDict + "/heuristic.txt");
    if( !heuristicArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo: " << mainDict + "/heuristic.txt" << std::endl;
        return -1;
    }
    cont = 0;
    
    for(Graph g : list)
    {
        heuristicArq << "\tGRAFO " + std::to_string(cont+1) + ":\n";

        Graph tree_h1  = Heuristic::heuristica_1_tree(g);
        int h1  = find_factor(g, tree_h1);
        respostaH1[cont]  = h1;
        f.print(h1, "\theuristica 1: ", heuristicArq);
        f.printAdjMat_2(tree_h1, "Arvore heurista 1: ", heuristicArq);

        Graph tree_h2  = Heuristic::heuristica_2_tree(g);
        int h2  = find_factor(g, tree_h2);
        respostaH2[cont]  = h2;
        f.print(h2, "\theuristica 2: ", heuristicArq);
        f.printAdjMat_2(tree_h2, "Arvore heurista 2: ", heuristicArq);

        Graph tree_h2g = Heuristic::heuristica_2_global(g);
        int h2g = find_factor(g, tree_h2g);
        respostaH2g[cont] = h2g;
        f.print(h2g, "\theuristica 2_global: ", heuristicArq);
        f.printAdjMat_2(tree_h2g, "Arvore heurista 2_global: ", heuristicArq);
/*
        Graph tree_h3  = Heuristic::heuristica_3_tree(g);
        int h3  = find_factor(g, tree_h3);
        respostaH3[cont]  = h3;
        f.print(h3, "\theuristica 3: ", heuristicArq);
        f.printAdjMat_2(tree_h3, "Arvore heurista 3: ", heuristicArq);
*/        
        ++cont;
    }
    heuristicArq.close();
    

    // escreve o resultado da força bruta no arquivo by_force.txt
    std::ofstream forceArq;
    time_t time_begin = 0;
    time_t time_end = 0;
    double tempo_total = 0;
    // std::vector<double> time_vector(qtd_grafos);

    cont = 0;
    for(Graph g : list){

        forceArq.open(mainDict + "/by_force.txt", std::ios_base::app);
        if( !forceArq.is_open() ){
            std::cerr << "ERROR ao abrir o arquivo: " << mainDict + "/by_force.txt" << std::endl;
            return -1;
        }
        
        std::cout << "Inicio do calculo do grafo " << cont+1 << std::endl;
        f.printAdjMat(g,"");
        time(&time_begin);
        (*create_threads)(g); // chamada do ponteiro da função
        time(&time_end);
        std::cout << "Termino do calculo do grafo " << cont+1 << std::endl;
        std::cout << std::endl;
        forceArq << "\tGRAFO " + std::to_string(cont+1) + ":\n";
        f.print(index_global, "\tindex = ", forceArq);
        f.printAdjMat_2(tree_global, "arvore encontrada:", forceArq);
        f.print(difftime(time_end, time_begin), "Tempo de execução (segundos): ", forceArq);
        forceArq << "\n";

        respostaForce[cont] = index_global;
        tempo_total += difftime(time_end, time_begin);
        
        // reseta 
        index_global = INF_VALUE;
        tree_global.clear();
        total_arv = 0;

        // vai para o próximo grafo
        ++cont;

        // fecha o arquivo para ter certeza que os dados no buffer foram escritos.
        forceArq.close();
    }
    // escrevo o tempo total.
    forceArq.open(mainDict + "/by_force.txt", std::ios_base::app);
    if( !forceArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo: " << mainDict + "/by_force.txt, Para a escritura do tempo total" << std::endl;
        return -1;
    }
    f.print(tempo_total, "Tempo total (seg) = ", forceArq);
    forceArq.close();

    // Abre o arquivo que escreve a taxa de acerto e o quanto as heuristicas erraram.
    std::ofstream taxaArq(mainDict + "/taxa.txt");
    if( !taxaArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo: " << mainDict + "/taxa.txt" << std::endl;
        return -1;
    }
    // quantidade de acertos
    double acertoH1  = 0;
    double acertoH2  = 0;
    double acertoH2g = 0;
    //double acertoH3  = 0;

    // tuple (qtd de grafos, distancia do erro)
    std::vector< std::tuple<int, int> > errosH1;
    std::vector< std::tuple<int, int> > errosH2;
    std::vector< std::tuple<int, int> > errosH2g;
    //std::vector< std::tuple<int, int> > errosH3;

    // distancia do erro;
    int distErro = 0;
    
    // flags para auxiliar a colocar um novo elemento nos vetores de erros
    bool flagH1  = true; 
    bool flagH2  = true;
    bool flagH2g = true;
    //bool flagH3  = true;
    
    // escreve a legenda
    taxaArq << "\tlegenda:\n\t\tA->acertou\n\t\tE->errou\n\n";
    for(int i=0; i < qtd_grafos; ++i){
        taxaArq << "\tGRAFO " + std::to_string(i+1) + ": ";
        if (respostaH1[i] == respostaForce[i]){
            ++acertoH1;
            taxaArq << "h_1: A;\t";
        }
        else {
            taxaArq << "h_1: E;\t";
            distErro = abs(respostaForce[i] - respostaH1[i]);
            
            // procura se já existe algum elemento com o mesmo distErro
            for( auto &tupleH1 : errosH1 ){
                if(std::get<1>(tupleH1) == distErro){
                    std::get<0>(tupleH1) += 1;
                    flagH1 = false;
                    break; // depois que achou um, não é nescessário continuar procurando 
                }
            }
            if( flagH1 ){
                errosH1.push_back(std::make_tuple(1, distErro)); // add um novo elemento no vetor
            }
            flagH1 = true; // reset
        }
            
        if (respostaH2[i] == respostaForce[i]) {
            ++acertoH2;
            taxaArq << "h_2: A;\t\t";
        }
        else {
            taxaArq << "h_2: E;\t\t";
            distErro = abs(respostaForce[i] - respostaH2[i]);

            // procura se já existe algum elemento com o mesmo distErro
            for( auto &tupleH2 : errosH2 ){
                if(std::get<1>(tupleH2) == distErro){
                    std::get<0>(tupleH2) += 1;
                    flagH2 = false;
                    break; // depois que achou um, não é nescessário continuar procurando 
                }
            }
            if( flagH2 ){
                errosH2.push_back(std::make_tuple(1, distErro)); // add um novo elemento no vetor
            }
            flagH2 = true; // reset
        }

        if (respostaH2g[i] == respostaForce[i]) {
            ++acertoH2g;
            taxaArq << "h_2g: A;\t";
        }
        else {
            taxaArq << "h_2g: E;\t";
            distErro = abs(respostaForce[i] - respostaH2g[i]);

            // procura se já existe algum elemento com o mesmo distErro
            for( auto &tupleH2g : errosH2g ){
                if(std::get<1>(tupleH2g) == distErro){
                    std::get<0>(tupleH2g) += 1;
                    flagH2g = false;
                    break; // depois que achou um, não é nescessário continuar procurando 
                }
            }
            if( flagH2g ){
                errosH2g.push_back(std::make_tuple(1, distErro)); // add um novo elemento no vetor
            }
            flagH2g = true; // reset
        }
/*
        if (respostaH3[i] == respostaForce[i]) {
            ++acertoH3;
            taxaArq << "h_3: A;\t";
        }
        else {
            taxaArq << "h_3: E;\t";
            distErro = abs(respostaForce[i] - respostaH3[i]);

            // procura se já existe algum elemento com o mesmo distErro
            for( auto &tupleH3 : errosH3 ){
                if(std::get<1>(tupleH3) == distErro){
                    std::get<0>(tupleH3) += 1;
                    flagH3 = false;
                    break; // depois que achou um, não é nescessário continuar procurando 
                }
            }
            if( flagH3 ){
                errosH3.push_back(std::make_tuple(1, distErro)); // add um novo elemento no vetor
            }
            flagH3 = true; // reset
        }
*/
        taxaArq << std::endl;
    }

    f.print(acertoH1/qtd_grafos, "Taxa de acerto da heuristica 1 é: ", taxaArq);
    f.print(acertoH2/qtd_grafos, "Taxa de acerto da heuristica 2 é: ", taxaArq);
    f.print(acertoH2g/qtd_grafos, "Taxa de acerto da heuristica 2_GLOBAL é: ", taxaArq);
    //f.print(acertoH3/qtd_grafos, "Taxa de acerto da heuristica 3 é: ", taxaArq);

    taxaArq << "\n";
    taxaArq << "\tHeuristica 1\n";
    for( auto t : errosH1){
        f.print(std::get<0>(t), "grafos: ", taxaArq);
        f.print(std::get<1>(t), "distancia do erro: ", taxaArq);
        taxaArq << "\n";
    }

    taxaArq << "\n";
    taxaArq << "\tHeuristica 2\n";
    for( auto t : errosH2){
        f.print(std::get<0>(t), "grafos: ", taxaArq);
        f.print(std::get<1>(t), "distancia do erro: ", taxaArq);
        taxaArq << "\n";
    }

    taxaArq << "\n";
    taxaArq << "\tHeuristica 2_GLOBAL\n";
    for( auto t : errosH2g){
        f.print(std::get<0>(t), "grafos: ", taxaArq);
        f.print(std::get<1>(t), "distancia do erro: ", taxaArq);
        taxaArq << "\n";
    }
/*
    taxaArq << "\n";
    taxaArq << "\tHeuristica 3\n";
    for( auto t : errosH3){
        f.print(std::get<0>(t), "grafos: ", taxaArq);
        f.print(std::get<1>(t), "distancia do erro: ", taxaArq);
        taxaArq << "\n";
    }
*/
    taxaArq.close();

    sem_destroy(&semaforo);

    return 0;
}


void find_index_pararell_edge(Graph& g, std::vector<int> edges, int start, const int id)
{
    sem_wait(&semaforo);

    int n = g.getQtdVertices();
    int m = g.getQtdArestas();
    // std::vector<int> edges = OpBasic::edges(g);
    int indice[n-1];
    int j = 0;
    indice[j] = start;
    
    Graph tree(n);
    Graph tree_local;
    int arv = 0;
    int index_local = INF_VALUE;

    bool flag = false;
    Graph gTeste(n);
    for(int i = start; i < edges.size(); i += 2)
    {
        gTeste.add_aresta(edges[i], edges[i+1]);
    }
    if( OpBasic::is_connected(gTeste) ){
        flag = true;
    }

    if(flag) { //Começa a busca pelas árvores geradoras.
        while(indice[0] < start+2){
            if( indice[j]/2 > m-(n-1-j) ){
                --j;
                tree.remove_aresta(edges[indice[j]],edges[indice[j]+1]);
                indice[j] += 2;
            }
            else {
                tree.add_aresta(edges[indice[j]], edges[indice[j]+1]);
                if( !OpBasic::is_cyclic(tree) ){
                    if(j == n-2){ // achou uma arvore geradora
                        int f = find_factor(g, tree);
                        ++arv;
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                        }
                    }
                    else{
                        int next = j+1;
                        indice[next] = indice[j] + 2;
                        j = next;
                        continue; // Simula uma chamada recursiva
                    }
                }
                tree.remove_aresta(edges[indice[j]], edges[indice[j]+1]);
                indice[j] += 2;
            }
        }
    }
    
    mtx.lock();
    if( arv == 0){
        std::cout << "thread " << id << " nao criou arvores.\n";
    }
    else {
        std::cout << "thread " << id << " criou " << arv << " arvores, e encontrou index "<< index_local << std::endl;
    }
    total_arv += arv;
    if( index_local < index_global){
        index_global = index_local;
        tree_global = tree_local;
    }
    mtx.unlock();

    sem_post(&semaforo);
}

void create_threads_edge(Graph& g)
{
    int n = g.getQtdVertices();
    int m = g.getQtdArestas();
    // int id=0;

    int qtd_th = m - n + 2;
    std::vector< std::thread> vetor_th(qtd_th);

    //std::vector<int> edges = OpBasic::edges_by_bigger_degree(g);
    std::vector<int> edges = OpBasic::edges_conected(g);

    for(int i=0; i < qtd_th; ++i){
        vetor_th[i] = std::thread(find_index_pararell_edge, std::ref(g), edges, i*2, i+1); // separação dos threats
    }

    for(int i=0; i < qtd_th; ++i){
        vetor_th[i].join(); // junção das threads
    }
}

void create_threads_edge_max_degree(Graph& g)
{
    int qtd_th = g.maior_grau();
    std::vector< std::thread> vetor_th(qtd_th);

    std::vector<int> edges = OpBasic::edges_by_bigger_degree(g);
    
    for(int i=0; i < qtd_th; ++i){
        vetor_th[i] = std::thread(find_index_pararell_edge, std::ref(g), edges, i*2, i+1); // separação dos threats
    }

    for(int i=0; i < qtd_th; ++i){
        vetor_th[i].join(); // junção das threads
    }
}

int find_factor(Graph& g, Graph& tree)
{
    std::vector<int> list = OpBasic::diference_edge(g, tree);
    std::vector<int>::iterator it;
    int factor = 1;

    it = list.begin();
    while(it != list.end()){
        int v = *it;
        int u = *(it+1);
        int d = OpBasic::distance(tree, v, u);
        if(factor < d){
            factor = d;
        }
        it = it + 2;
    }

    return factor;
}