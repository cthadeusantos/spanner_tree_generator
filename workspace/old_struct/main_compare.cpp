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

#include <sstream>     // std::stringstream
#include <algorithm>   // std::find
#include <thread>      // std::thread
#include <mutex>       // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#define NUM_THREAD 8
#define TIME_s 0
#define TIME_ms 1
#define TIME_us 2
#define TIME_ns 3

int index_global = INF_VALUE;
int total_arv = 0;
Graph tree_global;
std::mutex mtx;
sem_t semaforo;

void find_index_pararell_edge(Graph &g, std::vector<int> edges, int start, const int id);
void create_threads_edge_max_degree(Graph &g, int num_thread);
int find_factor(Graph &g, Graph &tree);

//Functions to calculate time
double get_time(int resolution);
void tic(double *time, int resolution);
void tac(double *time, int resolution);

int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <path/nome_directory/> <nome_basico> <quantidade_de_grafos>" << std::endl;
        exit(-1);
    }

    std::string path(argv[1]);
    std::string nameBase(argv[2]);
    const int qtdGrafos = atoi(argv[3]);

    Frontier f;
    Strech s;
    Graph grafos[qtdGrafos];
    std::string grafos_nomes[qtdGrafos];
    OpBasic op;

    // Lê os grafos
    for (int i = 0; i < qtdGrafos; i++)
    {
        grafos_nomes[i] = path + nameBase + std::to_string(i);
        f.open_in(grafos_nomes[i] + ".txt");
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

    int pararel_answers[qtdGrafos];
    Graph pararel_trees[qtdGrafos];
    double pararel_times[qtdGrafos];

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

        pararel_answers[i] = -1;
        pararel_times[i] = 0.0;
    }

    sem_init(&semaforo, 0, NUM_THREAD);
    std::ofstream resultArqParcial;

    for (int i = 0; i < qtdGrafos; i++)
    {
        resultArqParcial.open(grafos_nomes[i] + "_result.txt");
        if (!resultArqParcial.is_open())
        {
            std::cerr << "ERROR ao abrir o arquivo" << grafos_nomes[i] + "_result.txt" << std::endl;
            return -1;
        }

        new_limite_inferior[i] = op.maxLowerCicle(grafos[i]) - 1;
        old_limite_inferior[i] = op.girth(grafos[i]) - 1;

        f.print(i, "\t\tGRAFO: ", resultArqParcial);
        f.print(new_limite_inferior[i], "\tNovo limite inferior (MAX(min(e))-1): ", resultArqParcial);
        f.print(old_limite_inferior[i], "\tantigo limite inferiod (girth-1): ", resultArqParcial);

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

        // Sequencial
        tic(&sequencial_times[i], TIME_s);
        s.find_index(grafos[i]);
        tac(&sequencial_times[i], TIME_s);
        sequencial_trees[i] = s.getTree();
        sequencial_answers[i] = find_factor(grafos[i], sequencial_trees[i]);

        std::cout << "Resposta Sequencial = " << sequencial_answers[i] << "; tempo: " << sequencial_times[i] << std::endl;

        // paralelo
        tic(&pararel_times[i], TIME_s);
        create_threads_edge_max_degree(grafos[i], NUM_THREAD);
        tac(&pararel_times[i], TIME_s);
        pararel_trees[i] = tree_global;
        pararel_answers[i] = find_factor(grafos[i], pararel_trees[i]);

        // Reseta os valores para o proximo valor paralelo
        index_global = INF_VALUE;
        tree_global.clear();
        total_arv = 0;

        std::cout << "Resposta Paralelo = " << pararel_answers[i] << "; tempo: " << pararel_times[i] << std::endl;
        std::cout << std::endl;

        f.print(sequencial_answers[i], "\tResposta Sequencial = ", resultArqParcial);
        f.print(sequencial_times[i], "\tTempo sequencial em segundos = ", resultArqParcial);
        f.print(pararel_answers[i], "\tResposta Paralela = ", resultArqParcial);
        f.print(pararel_times[i], "\tTempo Paralela em segundos = ", resultArqParcial);
        f.print(h1_answers[i], "\tResposta H1 = ", resultArqParcial);
        f.print(h1_times[i], "\tTempo H1 micro seg= ", resultArqParcial);        
        f.print(h2_answers[i], "\tResposta H2 = ", resultArqParcial);
        f.print(h2_times[i], "\tTempo H2 micro seg= ", resultArqParcial);
        //f.printAdjMat_2(sequencial_trees[i], "\tArvore Sequencial", resultArqParcial);
        //f.printAdjMat_2(pararel_trees[i], "\tArvore Paralela", resultArqParcial);
        //f.printAdjMat_2(h1_trees[i], "\tArvore H1", resultArqParcial);
        //f.printAdjMat_2(h2_trees[i], "\tArvore H2", resultArqParcial);

        resultArqParcial.close();
    }

    int h1_acertos = 0;
    double h1_taxa;
    int h2_acertos = 0;
    double h2_taxa;
    int paralel_acertos = 0;
    double paralel_taxa;
    for (int i = 0; i < qtdGrafos; i++)
    {
        if (h1_answers[i] == sequencial_answers[i])
        {
            h1_acertos++;
        }

        if (h2_answers[i] == sequencial_answers[i])
        {
            h2_acertos++;
        }

        if (pararel_answers[i] == sequencial_answers[i])
        {
            paralel_acertos++;
        }
    }

    h1_taxa = (double)h1_acertos / qtdGrafos;
    h2_taxa = (double)h2_acertos / qtdGrafos;
    paralel_taxa = (double)paralel_acertos / qtdGrafos;

    std::ofstream resultArq(path + "result.txt");
    if (!resultArq.is_open())
    {
        std::cerr << "ERROR ao abrir o arquivo" << std::endl;
        return -1;
    }

    f.print(h1_taxa, "TAXA ACERTO H1: ", resultArq);
    f.print(h2_taxa, "TAXA ACERTO H2: ", resultArq);
    f.print(paralel_taxa, "TACA ACERTO PARALELO: ", resultArq);

    resultArq.close();

    return 0;
}

int find_factor(Graph &g, Graph &tree)
{
    std::vector<int> list = OpBasic::diference_edge(g, tree);
    std::vector<int>::iterator it;
    int factor = 1;

    it = list.begin();
    while (it != list.end())
    {
        int v = *it;
        int u = *(it + 1);
        int d = OpBasic::distance(tree, v, u);
        if (factor < d)
        {
            factor = d;
        }
        it = it + 2;
    }

    return factor;
}

void create_threads_edge_max_degree(Graph &g, int num_thread)
{
    // int qtd_th = g.maior_grau();
    int qtd_th = num_thread;

    std::vector<std::thread> vetor_th(qtd_th);

    std::vector<int> edges = OpBasic::edges_by_bigger_degree(g);
    Frontier f;
    f.print(edges, "TESTE: ");
    for (int i = 0; i < qtd_th; ++i)
    {
        vetor_th[i] = std::thread(find_index_pararell_edge, std::ref(g), edges, i * 2, i + 1); // separação dos threats
    }

    for (int i = 0; i < qtd_th; ++i)
    {
        vetor_th[i].join(); // junção das threads
    }
}

void find_index_pararell_edge(Graph &g, std::vector<int> edges, int start, const int id)
{
    sem_wait(&semaforo);

    int n = g.getQtdVertices();
    int m = g.getQtdArestas();
    // std::vector<int> edges = OpBasic::edges(g);
    int indice[n - 1];
    int j = 0;
    indice[j] = start;

    Graph tree(n);
    Graph tree_local;
    int arv = 0;
    int index_local = INF_VALUE;

    OpBasic op;
    int grt = op.maxLowerCicle(g);

    Graph gTeste(n);

    for (int i = start; i < edges.size(); i += 2)
    {
        gTeste.add_aresta(edges[i], edges[i + 1]);
    }
    if (OpBasic::is_connected(gTeste))
    {
        //Começa a busca pelas árvores geradoras.
        while (indice[0] < start + 2 && index_global > grt - 1)
        {
            if (indice[j] / 2 > m - (n - 1 - j))
            {
                --j;
                tree.remove_aresta(edges[indice[j]], edges[indice[j] + 1]);
                indice[j] += 2;
            }
            else
            {
                tree.add_aresta(edges[indice[j]], edges[indice[j] + 1]);
                if (!OpBasic::is_cyclic(tree))
                {
                    if (j == n - 2)
                    { // achou uma arvore geradora
                        int f = find_factor(g, tree);
                        ++arv;
                        if (f < index_local)
                        {
                            index_local = f;
                            tree_local = tree;
                            if (index_local == grt - 1)
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        int next = j + 1;
                        indice[next] = indice[j] + 2;
                        j = next;
                        continue; // Simula uma chamada recursiva
                    }
                }
                tree.remove_aresta(edges[indice[j]], edges[indice[j] + 1]);
                indice[j] += 2;
            }
        }
    }

    mtx.lock();
    if (arv == 0)
    {
        std::cout << "thread " << id << " nao criou arvores.\n";
    }
    else
    {
        std::cout << "thread " << id << " criou " << arv << " arvores, e encontrou index " << index_local << std::endl;
    }
    total_arv += arv;
    if (index_local < index_global)
    {
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

void tic(double *time, int resolution)
{
    *time = get_time(resolution);
}

void tac(double *time, int resolution)
{
    *time = get_time(resolution) - *time;
}
