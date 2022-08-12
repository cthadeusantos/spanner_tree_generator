#include "code/graph.hpp"
#include "code/frontier.hpp"
#include "code/opBasic.hpp"
#include "code/genGraph.hpp"
#include "code/heuristic.hpp"
#include "code/strech.hpp"
#include <iostream>
#include <cstdlib>
#include <sys/time.h>

#include <sstream> // std::stringstream
#include <algorithm> // std::find

#define TIME_s 0
#define TIME_ms 1
#define TIME_us 2
#define TIME_ns 3

int find_factor(Graph& g, Graph& tree);
//Functions to calculate time
double get_time(int resolution);
void tic(double * time, int resolution);
void tac(double * time, int resolution);

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
    //int respostaForce;    
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
    
    /*int qtd_core = NUM_THREAD;
    
    sem_init(&semaforo, 0, qtd_core);
    tic(&brute_time, TIME_s);     
    create_threads_edge_max_degree(g, qtd_core);
    tac(&brute_time, TIME_s);     
    respostaForce = index_global;*/

     // escreve os resultados das heuristicas no arquivo "/heuristic.txt"
    /*std::ofstream resultArq("result/result_seq_heur_"
                                + std::to_string(g.getQtdVertices())     
                                + "_"
                                + std::to_string(g.getQtdArestas())     
                                + ".txt");
    */

    std::ofstream seqResultArq("result/result_seq_"
                                + std::to_string(g.getQtdVertices())     
                                + "_"
                                + std::to_string(g.getQtdArestas())     
                                + ".txt", std::ios_base::app);
    std::ofstream h1ResultArq("result/result_h1_"
                                + std::to_string(g.getQtdVertices())     
                                + "_"
                                + std::to_string(g.getQtdArestas())     
                                + ".txt", std::ios_base::app);
    std::ofstream h2ResultArq("result/result_h2_"
                                + std::to_string(g.getQtdVertices())     
                                + "_"
                                + std::to_string(g.getQtdArestas())     
                                + ".txt", std::ios_base::app);

    /*if( !resultArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo" << std::endl;
        return -1;
    }*/

    if( !seqResultArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo" << std::endl;
        return -1;
    }

    if( !h1ResultArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo" << std::endl;
        return -1;
    }

    if( !h2ResultArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo" << std::endl;
        return -1;
    }

    f.print(seq_time, "\tTime (s): ", seqResultArq);
    f.print(h1_time, "\tTime (s): ", h1ResultArq);
    f.print(h2_time, "\tTime (s): ", h2ResultArq);

    //resultArq << "\tGRAFO n=" + std::to_string(g.getQtdVertices()) + ", m=" + std::to_string(g.getQtdArestas()) + ":\n";

    /*f.print(respostaH1, "\theuristica 1: ", resultArq);
    f.print(h1_time, "\tTime (us): ", resultArq);
    f.printAdjMat_2(tree_h1, "Arvore heurista 1: ", resultArq);

    f.print(respostaH2, "\theuristica 2: ", resultArq);
    f.print(h2_time, "\tTime (us): ", resultArq);
    f.printAdjMat_2(tree_h2, "Arvore heurista 2: ", resultArq);

    f.print(respostaSequential, "\tSequential Force: ", resultArq);
    f.print(seq_time, "\tTime (s): ", resultArq);
    f.printAdjMat_2(s.getTree(), "Arvore Brute Force: ", resultArq);*/

    //resultArq.close();
    seqResultArq.close();
    h1ResultArq.close();
    h2ResultArq.close();  

    return 0;
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
