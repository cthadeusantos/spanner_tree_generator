#include "code/graph.hpp"
#include "code/frontier.hpp"
#include "code/opBasic.hpp"
//#include "codigo/strech.hpp"
#include <iostream>
#include <ctime>
#include <sys/time.h>

#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy
#include <sstream> // std::stringstream
#define TIME_s 0
#define TIME_ms 1
#define TIME_us 2
#define TIME_ns 3
#define NUM_THREAD 8

int index_global =  INF_VALUE;
int total_arv = 0;
Graph tree_global;


std::mutex mtx;
sem_t semaforo;

//static const std::string min("min");
//static const std::string max("max");

//Functions to calculate time
double get_time(int resolution);
void tic(double * time, int resolution);
void tac(double * time, int resolution);

void find_index_pararell_edge(Graph& g, std::vector<int> edges, int start, const int id);
void create_threads_edge_max_degree(Graph& g);

void find_index_pararell(Graph& g, int raiz, int start, int end, const int id);
// void create_threads(Graph& g, int raiz, int qtd_core);
void create_threads(Graph& g);
int find_factor(Graph& g, Graph& tree);
int vertice_maior_grau(Graph& g);

int adj_id(Graph& g, int v, int adj);
int next(int a, int limite);

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

    int qtd_core = NUM_THREAD;
    if(argc == 3){
        qtd_core = atoi(argv[2]);
    }
    std::cout << "# Threads=" << qtd_core << std::endl;
    
    time_t time_begin, time_end;
    double total_time = 0.0;

    sem_init(&semaforo, 0, qtd_core);

    //time(&time_begin); // inicia a contagem de tempo
    tic(&total_time, TIME_s); 
    //create_threads_edge_max_degree(g);
    create_threads(g);
    tac(&total_time, TIME_s); 
    //time(&time_end);   // encerra a contagem de tempo

    std::ofstream resultArq("result/result_par_"
                                + std::to_string(g.getQtdVertices())     
                                + "_"
                                + std::to_string(g.getQtdArestas())  
                                + "_"   
                                + std::to_string(qtd_core)
                                + ".txt", std::ios_base::app);

    if( !resultArq.is_open() ){
        std::cerr << "ERROR ao abrir o arquivo" << std::endl;
        return -1;
    }

    //f.printAdjMat(g, "Grafo original", resultArq);
    //f.print(index_global, "\tindex = ", resultArq);
    //f.print(op.girth(g)-1, "\tlimite inferior = ", resultArq);
    //f.print(op.min_diameter_tree_value(g), "\tlimite superior = ", resultArq);
    //f.printAdjMat(tree_global, "arvore encontrada: ", resultArq );

    //f.print(total_arv, "Total de arvores = ", resultArq);
    f.print(total_time, "\tTime (s): ", resultArq);
    //f.print(difftime(time_end, time_begin), "tempo em seg: ", resultArq);
    //f.print(difftime(time_end, time_begin)/60 ,"tempo em minutos: ", resultArq );
    resultArq.close();
    sem_destroy(&semaforo);

    return 0;
}
/*
void create_threads(Graph& g, int raiz, int qtd_core)
{
    //std::thread vetor_th[qtd_core];
    std::vector<std::thread> vetor_th;
    //std::queue<int> start;
    //std::queue<int> end;
    int vetor_adj = g.adjList(raiz).size();

    int resto = vetor_adj % qtd_core;
    int passo = vetor_adj / qtd_core;
    int qtd_th = 0; // percorre o 'vetor_th'
    int j = 0; // percorre o 'vetor_adj'
    int th_id = 1; // debug pessoal

    while( resto > 0){
        //vetor_th[i] = std::thread(find_index_pararell, std::ref(g), raiz, j, j+passo+1, th_id);
        vetor_th.push_back(std::thread(find_index_pararell, std::ref(g), raiz, j, j+passo+1, th_id));
        //start.push(j);
        //end.push(j+passo+1);
        ++qtd_th;
        j += passo+1;
        ++th_id;
        --resto;
    }

    while( qtd_th < qtd_core && j < vetor_adj ){
        //vetor_th[i] = std::thread(find_index_pararell, std::ref(g), raiz, j, j+passo, th_id);
        vetor_th.push_back(std::thread(find_index_pararell, std::ref(g), raiz, j, j+passo, th_id));
        //start.push(j);
        //end.push(j+passo+1);
        ++qtd_th;
        j += passo;
        ++th_id;
    }

    for(int i=0; i < qtd_core; ++i){
        vetor_th[i].join();
    }


}
*/

void find_index_pararell(Graph& g, int raiz, int start, int end, const int id)
{

    sem_wait(&semaforo); // Apenas 4 threads puderam fazer este código por vez

    int n = g.getQtdVertices();
    int m = g.getQtdArestas();

    int prox_vizinho[n];
    int ult_colocado[n];
    int v = raiz;
    int u;
    int arv = 0; // debug
    int index_local = INF_VALUE;
    Graph tree_local;

    int grt;

    Frontier front;

    Graph tree(n);

    OpBasic op;
    //int raiz;

    grt = op.maxLowerCicle(g); // alteracao DJ

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;

    while(index_global > grt-1 ){
        if(v == raiz){
            if(prox_vizinho[v] == end){
                break; // Fim do algoritmo
            }
        }

        if( prox_vizinho[v] == g.grau(v) ){
            prox_vizinho[v] = 0;
            v = g.ant_vertex(v);
            tree.remove_aresta(v, ult_colocado[v]);
            ult_colocado[v] = -1;

        }else{
            u = g.adjList(v)[prox_vizinho[v]];
            ++prox_vizinho[v];
            if( not tree.possui_aresta(v, u) ){
                tree.add_aresta(v, u);
                ult_colocado[v] = u;
                if(not OpBasic::is_cyclic(tree)){
                    if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                        int f = find_factor(g, tree);
                        ++arv;
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            if(index_local == grt-1){// alteracao LF
                              break;// alteracao LF
                            }// alteracao LF

                        }
                    }else{
                        v = g.next_vertex(v);
                        continue;
                    }
                }
                tree.remove_aresta(v, u);
            }
        }
    }
    mtx.lock();
    std::cout << "thread " << id << " criou " << arv << " arvores." << std::endl;

    if(index_local < index_global) {
      total_arv += arv;
      index_global = index_local;
      tree_global = tree_local;
    }
    mtx.unlock();

    sem_post(&semaforo); // a thread libera espaço para a proxima
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

int vertice_maior_grau(Graph& g)
{
    int raiz = -1;
    int maior_grau = -1;
    for(int i=0; i < g.getQtdVertices(); ++i){
        if(g.grau(i) > maior_grau){
            raiz = i;
            maior_grau = g.grau(i);
        }
    }
    return raiz;
}

void create_threads(Graph& g)
{
    int raiz = vertice_maior_grau(g);
    int qtd = g.grau(raiz);
    int id=0;

    std::thread vetor_th[qtd];

    if(g.possui_aresta(raiz, g.ant_vertex(raiz) ) ){
        id = adj_id(g, raiz, g.ant_vertex(raiz) );
    }

    for(int i=0; i < qtd; ++i){
        vetor_th[i] = std::thread(find_index_pararell, std::ref(g), raiz, id, id+1, i);
        id = next(id, qtd);
    }

    // std::cout << "bye";

    for(int i=0; i < qtd; ++i){
        vetor_th[i].join();
    }
}

int adj_id(Graph& g, int v, int adj)
{
    int id=0;
    for(int u : g.adjList(v) ){
        if(u == adj){
            break;
        }
        ++id;
    }
    return id;
}

int next(int a, int limite)
{
    ++a;
    return a == limite ? 0 : a;
}

/*
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

    OpBasic op;
    int grt = op.maxLowerCicle(g);

    for(int i = start; i < edges.size(); i += 2)
    {
        gTeste.add_aresta(edges[i], edges[i+1]);
    }
    if( OpBasic::is_connected(gTeste) ){
        flag = true;
    }

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
*/

void create_threads_edge_max_degree(Graph& g)
{
    int qtd_th = g.maior_grau();
    //int qtd_th = num_thread;

    std::vector< std::thread> vetor_th(qtd_th);

    std::vector<int> edges = OpBasic::edges_by_bigger_degree(g);
    
    for(int i=0; i < qtd_th; ++i){
        vetor_th[i] = std::thread(find_index_pararell_edge, std::ref(g), edges, i*2, i); // separação dos threats
    }

    for(int i=0; i < qtd_th; ++i){
        vetor_th[i].join(); // junção das threads
    }
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
