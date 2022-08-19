#include "code/graph.hpp"
#include "code/frontier.hpp"
#include "code/opBasic.hpp"
#include "code/genGraph.hpp"
#include "code/heuristic.hpp"
#include "code/strech.hpp"
#include <iostream>
#include <cstdlib>
#include <sys/time.h>

#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy
#include <sstream> // std::stringstream
#include <algorithm> // std::find

#define TIME_s 0
#define TIME_ms 1
#define TIME_us 2
#define TIME_ns 3
#define NUM_THREAD 8

//Functions to calculate time
double get_time(int resolution);
void tic(double * time, int resolution);
void tac(double * time, int resolution);

int index_global =  INF_VALUE;
int total_arv = 0;
Graph tree_global;
std::mutex mtx;
sem_t semaforo;

void find_index_pararell_edge(Graph& g, std::vector<int> edges, int start, const int id);
void create_threads_edge_max_degree(Graph& g, int num_thread);
int find_factor(Graph& g, Graph& tree);

int main(int argc, char const *argv[])
{
    if(argc < 2){
        std::cerr << "Usage: " << argv[0] << " <path/nome_arq_in.txt>" << std::endl;
        exit(-1);
    }
    Frontier f;
    Strech s;
    f.open_in(argv[1]);
    Graph g;
    OpBasic op;

    f.read(g);
    f.close_in();
    
    int respostaH1;
    int respostaH2;
    int respostaForce;    
    int respostaSequential;    
    double h1_time = 0.0, h2_time = 0.0, brute_time = 0.0, seq_time = 0.0;
       

    tic(&h1_time, TIME_us);    
    Graph tree_h1 = Heuristic::heuristica_1_tree(g);    
    tac(&h1_time, TIME_us);
    respostaH1 = find_factor(g, tree_h1);
    
    tic(&h2_time, TIME_us);
    Graph tree_h2  = Heuristic::heuristica_2_tree(g);
    tac(&h2_time, TIME_us);

    respostaH2 = find_factor(g, tree_h2);
    
    //Sequential
    tic(&seq_time, TIME_s);
    s.find_index(g);
    tac(&seq_time, TIME_s);
    respostaSequential = s.getIndex();

    //Parallel Brute Force
    int qtd_core = NUM_THREAD;
    
    sem_init(&semaforo, 0, qtd_core);
    
    tic(&brute_time, TIME_s);     
    create_threads_edge_max_degree(g, qtd_core);
    tac(&brute_time, TIME_s);     
    respostaForce = index_global;

     // escreve os resultados das heuristicas no arquivo "/heuristic.txt"
    std::ofstream resultArq("result/result_"
                                + std::to_string(g.getQtdVertices())     
                                + "_"
                                + std::to_string(g.getQtdArestas())     
                                + ".txt");

    if( !resultArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo" << std::endl;
        return -1;
    }

    resultArq << "\tGRAFO n=" + std::to_string(g.getQtdVertices()) + ", m=" + std::to_string(g.getQtdArestas()) + ":\n";
    f.print(respostaH1, "\theuristica 1: ", resultArq);
    f.print(h1_time, "\tTime (us): ", resultArq);
    f.printAdjMat_2(tree_h1, "Arvore heurista 1: ", resultArq);

    f.print(respostaH2, "\theuristica 2: ", resultArq);
    f.print(h2_time, "\tTime (us): ", resultArq);
    f.printAdjMat_2(tree_h2, "Arvore heurista 2: ", resultArq);

    f.print(respostaSequential, "\tSequential Force: ", resultArq);
    f.print(seq_time, "\tTime (s): ", resultArq);
    f.printAdjMat_2(s.getTree(), "Arvore Brute Force: ", resultArq);


    f.print(respostaForce, "\tParallel Brute Force: ", resultArq);
    f.print(brute_time, "\tTime (s): ", resultArq);
    f.printAdjMat_2(tree_global, "Arvore Brute Force: ", resultArq);

    resultArq.close();  

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

    Graph gTeste(n);

    OpBasic op;
    int grt = op.maxLowerCicle(g);

    for(int i = start; i < edges.size(); i += 2)
    {
        gTeste.add_aresta(edges[i], edges[i+1]);
    }
    if( OpBasic::is_connected(gTeste) ){
        if(index_global > grt-1) { //Começa a busca pelas árvores geradoras.
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
                                if (index_local == grt-1) {
                                    break;
                                }
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

void create_threads_edge_max_degree(Graph& g, int num_thread)
{
    int qtd_th = g.maior_grau();
    //int qtd_th = num_thread;

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

double get_time(int resolution)
/******************************************************************************
 returns the time in the desired resolution
 
 TIME_s - seconds
 TIME_ms - miliseconds
 TIME_us - microseconds
 TIME_ns - nanoseconds
 ******************************************************************************/
{
	struct timespec ts;
	double ret;
    
	clock_gettime(CLOCK_REALTIME, &ts);
	
	switch (resolution)
	{
		case TIME_s:
			ret = (((double)ts.tv_sec) + (((double)ts.tv_nsec) / 1000000000));
			//printf("s %.20lf\n", ret);
			return ret;
		case TIME_ms:
			ret = ((((double)ts.tv_sec) * 1000) + (((double)ts.tv_nsec) / 1000000));
			//printf("ms %.20lf\n", ret);
			return ret;
		case TIME_us:
			ret = ((((double)ts.tv_sec)) * 1000000 + (((double)ts.tv_nsec) / 1000));
			//printf("us %.20lf\n", ret);
			return ret;
		case TIME_ns:
			ret = ((((double)ts.tv_sec)) * 1000000000 + ((double)ts.tv_nsec));
			//printf("ns %.20lf\n", ret);
	 		return ret;
		default:
			fprintf(stderr, "treb_get_time() - resolution not supported\n");
			exit(1);
	}		
}

void tic(double * time, int resolution)
{
    *time = get_time(resolution);
}

void tac(double * time, int resolution)
{
    *time = get_time(resolution) - *time;
}
