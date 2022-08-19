#include "code/graph.hpp"
#include "code/frontier.hpp"
#include "code/opBasic.hpp"
#include "code/genGraph.hpp"
#include "code/heuristic.hpp"
#include "code/strech.hpp"
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <string>
#include <fstream>
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
    if(argc < 3){
        std::cerr << "Usage: " << argv[0] << " <graph_dir> <file_with_graph_pathes>" << std::endl;
        exit(-1);
    }

    std::string path(argv[1]);
    std::string graphsFilename(argv[2]);
    std::string nameBase = graphsFilename.substr(0, graphsFilename.rfind("."));

    std::ifstream graphsFile;    
    graphsFile.open(path + graphsFilename);

    if (!graphsFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    
    std::vector<std::string> graphNames;
    std::string graphName;
    while (graphsFile >> graphName) {
        graphNames.push_back(path + graphName.substr(graphName.rfind("/") + 1, graphName.length() - 1));
    }
    graphsFile.close();
    
    int qtdGrafos = graphNames.size();    
    Frontier f;
    Strech s;
    Graph grafos[qtdGrafos];
    std::string grafos_nomes[qtdGrafos];
    OpBasic op;

    // Lê os grafos
    for (int i = 0; i < qtdGrafos; i++) {        
        f.open_in(graphNames[i]);
        f.read(grafos[i]);
        f.close_in();
    }    

    int new_limite_inferior[qtdGrafos];
    int old_limite_inferior[qtdGrafos];

    int h1_answers[qtdGrafos];
    Graph h1_trees[qtdGrafos];
    double h1_times[qtdGrafos];
    
    int h2_answers[qtdGrafos];
    Graph h2_trees[qtdGrafos];
    double h2_times[qtdGrafos];

    int sequencial_answers[qtdGrafos];
    Graph sequencial_trees[qtdGrafos];
    double sequencial_times[qtdGrafos];

    double maxCircleTime[qtdGrafos];
    //double girthTime;

    // zerar os valores
    for (int i = 0; i < qtdGrafos; i++)
    {
        new_limite_inferior[i] = 0;
        old_limite_inferior[i] = 0;

        h1_answers[i] = -1;
        h1_times[i] = 0.0;
        
        h2_answers[i] = -1;
        h2_times[i] = 0.0;

        sequencial_answers[i] = -1;
        sequencial_times[i] = 0.0;

    }

    for (int i = 0; i < qtdGrafos; i++)
    {
        std::cout << "GRAFO " << i << ": " << grafos[i].getQtdVertices() << ", " << grafos[i].getQtdArestas() << std::endl;
        tic(&maxCircleTime[i], TIME_us);
        new_limite_inferior[i] = op.maxLowerCicle(grafos[i]) - 1;
        tac(&maxCircleTime[i], TIME_us);

        //std::cout << "Time maxLowerCicle = " << maxCircleTime << std::endl;

        /*tic(&girthTime, TIME_us);
        old_limite_inferior[i] = op.girth(grafos[i]) - 1;
        tac(&girthTime, TIME_us);
        std::cout << "Time girth = " << girthTime << std::endl;
        */


        std::cout << "novo limite inferior (MAX(c(e))-1) = " << new_limite_inferior[i] << std::endl;
        //std::cout << "antigo limite inferior (girth-1) = " << old_limite_inferior[i] << std::endl;

        // Heuristica 1
        tic(&h1_times[i], TIME_us);
        h1_trees[i] = Heuristic::heuristica_1_tree(grafos[i]);
        tac(&h1_times[i], TIME_us);
        h1_answers[i] = find_factor(grafos[i], h1_trees[i]);

        std::cout << "Resposta H1 = " << h1_answers[i] << std::endl;
        
        // Heuristica 2
        tic(&h2_times[i], TIME_us);
        h2_trees[i] = Heuristic::heuristica_2_tree(grafos[i]);
        tac(&h2_times[i], TIME_us);
        h2_answers[i] = find_factor(grafos[i], h2_trees[i]);

        std::cout << "Resposta H2 = " << h2_answers[i] << std::endl;
    }

    int h1_acertos_new = 0;
    double h1_taxa_new;
    //int h1_acertos_old = 0;
    //double h1_taxa_old;
    int h2_acertos_new = 0;
    double h2_taxa_new;
    //int h2_acertos_old = 0;
    //double h2_taxa_old;
    for (int i = 0; i < qtdGrafos; i++)
    {
        if (h1_answers[i] == new_limite_inferior[i]) {
            h1_acertos_new++;
        }
        /*if (h1_answers[i] == old_limite_inferior[i]) {
            h1_acertos_old++;
        }*/

        if (h2_answers[i] == new_limite_inferior[i]) {
            h2_acertos_new++;
        }
        /*if (h2_answers[i] == old_limite_inferior[i]) {
            h2_acertos_old++;
        }*/ 
    }

    h1_taxa_new = (double)h1_acertos_new / qtdGrafos;
    //h1_taxa_old = (double)h1_acertos_old / qtdGrafos;
    h2_taxa_new = (double)h2_acertos_new / qtdGrafos;
    //h2_taxa_old = (double)h2_acertos_old / qtdGrafos;

    std::ofstream resultArq(path + "heur_lim_" + nameBase + "_results.txt");
    if( !resultArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo" << std::endl;
        return -1;
    }

    resultArq << "novo limite: para cada e in G, calcula o menor ciclo que contenha e (C(e)), o limite é MAX(C(e)) - 1" << std::endl;
    resultArq << "antigo limite: girth(G) - 1" << std::endl;
    resultArq << std::endl;
    f.print(h1_taxa_new, "TAXA ACERTO H1 Sobre o novo limite: ", resultArq);
    //f.print(h1_taxa_old, "TAXA ACERTO H1 Sobre o antigo limite: ", resultArq);
    f.print(h2_taxa_new, "TAXA ACERTO H2 Sobre o novo limite: ", resultArq);
    //f.print(h2_taxa_old, "TAXA ACERTO H2 Sobre o antigo limite: ", resultArq);
    resultArq << std::endl;

    for (int i = 0; i < qtdGrafos; i++)
    {
        resultArq << "\nGRAFO " << i << ": " << grafos[i].getQtdVertices() << ", " << grafos[i].getQtdArestas() << std::endl;
        f.print(new_limite_inferior[i], "Novo limite inferior = ", resultArq);
        //f.print(old_limite_inferior[i], "Antigo limite inferior = ", resultArq);        
        f.print(maxCircleTime[i], "Tempo maxCircleTime micro seg= ", resultArq);        
        f.print(h1_answers[i], "Resposta H1 = ", resultArq);
        f.print(h1_times[i], "Tempo H1 micro seg= ", resultArq);        
        f.print(h2_answers[i], "Resposta H2 = ", resultArq);
        f.print(h2_times[i], "Tempo H2 micro seg= ", resultArq);        
        //f.printAdjMat_2(h1_trees[i], "\tArvore H1", resultArq);
        //f.printAdjMat_2(h2_trees[i], "\tArvore H2", resultArq);
    }

    resultArq.close();  

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
