#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#include <initializer_list>

#include <iostream>
#include "graph.hpp"
#include "opBasic.hpp"
#include "stretch.hpp"

#include "../Debug.h"
#include "parallel.hpp"
#include "centrality.hpp"

#define INDUCED_CYCLE_SYZE_START 8

sem_t semaforo;
int total_arv = 0;
std::mutex mtx;
int num_threads;
int max_induced_cycles;
//extern std::atomic<bool> processando;

/**
 * @brief Calculate stretch index using articulations
 * @details Create thread to calculate the stretch index from articulations (if exists) 
 * @author Daniel Juventude (original)
 * @author Carlos Thadeu(adapter)
 * @param g a graph instance that represents the graph
 * @param raiz
 * @param start
 * @param end
 * @param id
 */
void find_index_articulation(Graph &graph, Graph &subgraph, int raiz, int start, int end, const int id)
{
    DEBUG std::cerr << "CALCULANDO POR ARTICULACAO - Thread : " << id << std::endl;

    sem_wait(&semaforo); 
    int n = subgraph.getQtdVertices(); 
    int m = subgraph.getQtdArestas();

    int prox_vizinho[n];
    int ult_colocado[n];
    int v = raiz;
    int u;
    int arv = 0; // debug
    int index_local = (int)INFINITY;
    Graph tree_local;

    Graph tree(n);

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;

    //while(g.get_stretch_index() > g.grt-1 ){
    int grt = OpBasic::maxLowerCicle(subgraph);
    subgraph.grt = grt;
    int lower_limit = grt - 1;

    mtx.lock();
    int stretch_index = graph.get_stretch_index();
    mtx.unlock();

    while(stretch_index > lower_limit && graph.get_signal()){
    //while(stretch_index > lower_limit ){
        if(v == raiz){
            if(prox_vizinho[v] == end){
                break; // Fim do algoritmo
            }
        }

        if( prox_vizinho[v] == subgraph.grau(v) ){
            prox_vizinho[v] = 0;
            v = subgraph.ant_vertex(v);
            tree.remove_aresta(v, ult_colocado[v]);
            ult_colocado[v] = -1;
        } else {
            u = subgraph.adjList(v)[prox_vizinho[v]];
            ++prox_vizinho[v];
            if( not tree.possui_aresta(v, u) ){
                tree.add_aresta(v, u);
                ult_colocado[v] = u;
                if(not OpBasic::is_cyclic(tree)){
                    if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                        int f = find_factor(subgraph, tree);
                        ++arv;
                        subgraph.add_tree();
                        
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            if(index_local == lower_limit){// alteracao LF
                              break;// alteracao LF
                            }// alteracao LF

                        }
                    }else{
                        v = subgraph.next_vertex(v);
                        continue;
                    }
                }
                tree.remove_aresta(v, u);
            }
        }
    }

    mtx.lock();

    DEBUG std::cerr << "thread " << id << " criou " << arv << " arvores, e encontrou index "<< index_local << std::endl;
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, graph);
    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
}

//! Calculate stretch index - parallel
/*!
    \param g a graph instance that represents the graph
    \param raiz 
    \param start 
    \param end
    \param id
*/
void find_index_parallel(Graph &g, int raiz, int start, int end, const int id)
{
    // Nos meus testes, mais de 4 threads fizeram este código,
    // comigo não aconteceu o mesmo que o aconteceu com o pesquisador Daniel.
    // As threads ficaram limitadas a quantidade de arestas entregues para processamento (natural)
    sem_wait(&semaforo); // Apenas 4 threads puderam fazer este código por vez (obs. pequisador Daniel)
    int n = g.getQtdVertices(); 
    int m = g.getQtdArestas();

    int prox_vizinho[n];
    int ult_colocado[n];
    int v = raiz;
    int u;
    int arv = 0; // debug
    //int index_local = INF_VALUE;
    int index_local = (int)INFINITY;
    Graph tree_local;
    Graph tree(n);

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;
    mtx.lock();
    int stretch_index = g.get_stretch_index();
    int lower_limit = g.grt-1 ;
    mtx.unlock();

    while(stretch_index > lower_limit && g.get_signal() ){
   //while(stretch_index > lower_limit){
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
        } else {
            u = g.adjList(v)[prox_vizinho[v]];
            ++prox_vizinho[v];
            if( not tree.possui_aresta(v, u) ){
                tree.add_aresta(v, u);
                ult_colocado[v] = u;
                if(not OpBasic::is_cyclic(tree)){
                    if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                        int f = find_factor(g, tree);
                        ++arv;
                        mtx.lock();
                        g.add_tree();
                        mtx.unlock();
                        
                        if(f < index_local){
                            index_local = f;
                            tree_local = tree;
                            if(index_local == lower_limit){// alteracao LF
                                mtx.lock(); // Alteração CT
                                g.set_signal(); // Alteração CT
                                mtx.unlock(); // Alteração CT
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

    DEBUG std::cerr << "thread " << id << " criou " << arv << " arvores, e encontrou index "<< index_local << std::endl;
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, g);

    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
}

/*!
    Calculate a stretch index using an induced cycle's edge
    \param g a graph instance that represents the graph
    \param raiz an integer that represents the vertex root
    \param neighbor_start an integer that represents the vertex neighbor the root
    \param id an integer that represents a number of thread
    \param edges_list a vector that represents induced cycle's edge to be remove
    \param end
*/
void find_index_induced_cycle(Graph &graph, int raiz, int neighbor_start, const int id, std::vector<int> edges_list )
{
    bool execute = true;
    sem_wait(&semaforo);
    //sem_wait(&semaforo);
    //sem_wait(&semaforo);
	mtx.lock();
    int lower_limit = graph.grt-1 ;
    Graph G1 = graph;   // Auxiliary graph - local graph
    mtx.unlock();

    // remove the root edges from a cycle of previous threads
    // Edges removed will processed another threads
    for (int i=0; i < id * 2 ; i=i+2){
        int v1=edges_list[i];
        int v2=edges_list[i+1];
        G1.remove_aresta(v1, v2);
        if (G1.grau(v1)<=1 && i != 0){
            G1.add_aresta(v1, v2);
            if (id == (i / 2 + 1)){
                DEBUG std::cerr << "Thread " << id << " with v1: " << v1 << " v2: " << v2 << " cannot be removed!"  << std::endl;
                execute=false;
            }

        }
    } // end of prodedure to remove the root edges
    //mtx.unlock();   // add by Thadeu

    bool disconnected = false;  // Can a tree be built?
    int start = G1.neighbor_index(raiz, neighbor_start); // Convert start vertex to start index from adjacency list
    int end = start + 1;            // Condition to stop algorithm 
    int n = G1.getQtdVertices();    // num vertices
    int m = G1.getQtdArestas();     // num edges
    int prox_vizinho[n] = {};       // next neighbor (index)
    int ult_colocado[n] = {};       // last add vertex at tree(index)
    int v = raiz;
    int u = -1;
    int index_local = (int)INFINITY;

    Graph tree_local(n);
    Graph tree(n);

    for(int i=0; i < n; ++i){
        prox_vizinho[i] = 0;
        ult_colocado[i] = -1;
    }

    prox_vizinho[v] = start;

    //if (G1.grau(v) == 0) disconnected = true; // There's no way to build a tree
    if (execute){
        while(G1.get_stretch_index() > graph.grt - 1 && graph.get_signal()) {
        //while(G1.get_stretch_index() > graph.grt - 1 && processando) {
        // while(G1.get_stretch_index() > G1.grt - 1 && G1.grau(v) > 0 && !(disconnected)) {
            if(v == raiz){
                if (prox_vizinho[v] == end){
                    break; // Fim do algoritmo
                }
            }
            if ( prox_vizinho[v] == G1.grau(v) ){
                prox_vizinho[v] = 0;
                v = G1.ant_vertex(v);
                tree.remove_aresta(v, ult_colocado[v]);
                ult_colocado[v] = -1;
            } else {
                u = G1.adjList(v)[prox_vizinho[v]];
                ++prox_vizinho[v];
                if( not tree.possui_aresta(v, u) ){
                    tree.add_aresta(v, u);
                    ult_colocado[v] = u;

                    if(not OpBasic::is_cyclic(tree)){

                        if(tree.getQtdArestas() == tree.getQtdVertices()-1){
                            mtx.lock();
                            int f = find_factor(graph, tree);
                            mtx.unlock();
                            G1.add_tree();

                            if(f < index_local){
                                index_local = f;
                                tree_local = tree;
                                if(index_local == lower_limit){// alteracao LF
                                    break; // alteracao LF
                                }// alteracao LF
                                /* if(index_local == G1.grt - 1){
                                    break;
                                } */
                            }
                        } else {
                            v = G1.next_vertex(v);
                            continue;
                        }
                    }
                    tree.remove_aresta(v, u);
                }
            }
        }
    }

    int arvores = G1.get_total_tree();
    DEBUG std::cerr << "thread " << id << " criou " << arvores << " arvores, e encontrou index "<< index_local << std::endl;
    
    mtx.lock();
    graph.sum_trees(G1.get_total_tree());
    int arv = 0; // Insert by compatibility with set_graph_final_parameters
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, graph);
    mtx.unlock();
    sem_post(&semaforo); // a thread libera espaço para a proxima
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
    //int index_local = INF_VALUE;
    int index_local = (int)INFINITY;

    Graph gTeste(n);

    // Removido pq coloquei em cria threads
/*     OpBasic op;
    int grt = op.maxLowerCicle(g); */

    for(int i = start; i < edges.size(); i += 2)
    {
        gTeste.add_aresta(edges[i], edges[i+1]);
    }
    if( OpBasic::is_connected(gTeste)){
        if(g.get_stretch_index() > g.grt-1) { //Começa a busca pelas árvores geradoras. // Alterado by thadeu
            while(indice[0] < start+2  && g.get_signal()){ //Update by thadeu
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
                            mtx.lock();
                            g.add_tree();
                            mtx.unlock();
                            if(f < index_local){
                                index_local = f;
                                tree_local = tree;

                                if (index_local == g.grt-1) {
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
    DEBUG std::cerr << "thread " << id << " criou " << arv << " arvores, e encontrou index "<< index_local << std::endl;
    set_graph_final_parameters(index_local, total_arv, arv, tree_local, g);
    mtx.unlock();
    sem_post(&semaforo);
}

void create_threads(Graph& g)
{
    int raiz = vertice_maior_grau(g);
    int qtd = g.grau(raiz);
    int id=0;

    std::thread vetor_th[qtd];

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    OpBasic op; // by thadeu
    g.grt = op.maxLowerCicle(g); // by thadeu
    // fim calcula grt

    if(g.possui_aresta(raiz, g.ant_vertex(raiz) ) ){
        id = adj_id(g, raiz, g.ant_vertex(raiz) );
    }

    for(int i=0; i < qtd; ++i){
        vetor_th[i] = std::thread(find_index_parallel, std::ref(g), raiz, id, id+1, i);
        id = next(id, qtd);
    }

    // std::cout << "bye";

    for(int i=0; i < qtd; ++i){
        vetor_th[i].join();
    }
}

void create_threads_edge_max_degree(Graph& g)
{
    //int qtd_th = g.maior_grau();
    int qtd_th = num_threads;

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    OpBasic op; // by thadeu
    g.grt = op.maxLowerCicle(g); // by thadeu
    // fim calcula grt

    std::vector< std::thread> vetor_th(qtd_th);

    std::vector<int> edges = OpBasic::edges_by_bigger_degree(g);
    
    qtd_th = std::max({num_threads, g.maior_grau(), int(edges.size()/2)});
/*     if (num_threads > g.maior_grau()){
        qtd_th = g.maior_grau();
    } else if (num_threads > edges.size()/2){
        qtd_th = int(edges.size()/2);
    } */
    if (qtd_th > num_threads){
        qtd_th = num_threads;
    }

    for(int i=0; i < qtd_th; ++i){
        vetor_th[i] = std::thread(find_index_pararell_edge, std::ref(g), edges, i*2, i); // separação dos threats
    }

    for(int i=0; i < qtd_th; ++i){
        vetor_th[i].join(); // junção das threads
    }
}

/**
 * @brief Create threads to calculate stretch index induced cycles
 * @details Create thread to calculate the stretch index induced cycles (if exists)
 * This procedure will seek for induced cycles and calculate the stretch index.
 * @author Carlos Thadeu
 * @param g a graph that represents the graph
 */
void create_threads_big_cycle(Graph& g) {
    int qty = -1;
    int root = -1;
    int neighbor = -1;
    std::vector<int> edges_list = {};

    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    OpBasic op; // by thadeu
    g.grt = op.maxLowerCicle(g); // by thadeu
    // fim calcula grt

    std::vector<std::vector<int>> induced_cycles = seeking_induced_cycles(g);

    // build edge list to be deleted from original graph inside find_index_induced_cycle
    // Thread 1 remove 1st edge (inside find_index_induced_cycle)
    // Thread 2 remove 1st and 2nd edges (inside find_index_induced_cycle)
    // Thread 3 remove 1st, 2nd and 3rd edges (inside find_index_induced_cycle)
    // and go on deleting edges (inside find_index_induced_cycle)

    for (int j=0; j < induced_cycles.size(); j++){
        for(int i = 0; i < induced_cycles[j].size()-1; ++i){
            root = induced_cycles[j][i];
            neighbor=induced_cycles[j][i+1];
            edges_list.push_back(root);
            edges_list.push_back(neighbor);
        }
    }
    
    qty = int(edges_list.size() / 2);

    if (qty > num_threads)
        qty = num_threads;
            
    DEBUG std::cerr << "Threads usadas: " << qty << std::endl;
    std::thread vetor_th[qty];

    for(int i = 0; i < qty; ++i){
        root = edges_list[i*2];
        neighbor = edges_list[i*2+1];
        // Here send edges' list to be deleted
        // Delete the edges ensures that there are no duplicate trees
        //vetor_th[i] = std::thread(find_index_induced_cycle, std::ref(g), root, neighbor, i, std::ref(edges_list));
        vetor_th[i] = std::thread(find_index_induced_cycle, std::ref(g), root, neighbor, i, std::ref(edges_list));
    }
    sem_wait(&semaforo);
    for(int i = 0; i < qty; ++i){
        vetor_th[i].join();
    }
    
}

/**
 * @brief Create threads to calculate stretch index from articulations
 * @details Create thread to calculate the stretch index from articulations (if exists)
 * This procedure will seek from articulations points then split the graph from
 * this articulations and calculate the stretch index
 * 
 * @author Carlos Thadeu
 * @param g a graph that represents the graph
 */
void create_threads_articulations(Graph& g) {
    // Calcula atributo grt
    // por enquanto fica aqui, no futuro retirar 
    // pois o método create_thread nao é para calcular nada do grafo
    //OpBasic op; // by thadeu
    g.grt = OpBasic::maxLowerCicle(g); // by thadeu
    // fim calcula grt


    std::set<int> articulations;     
    std::vector<std::pair<int,int>> bridges;
    std::tie(articulations, bridges) = seek_articulations(g); // seek for articulations and bridges
    //auto vertex_importance = Centrality::closeness_centrality_list(articulations, g);

    //root = Centrality::root_selection2(vertex_importance);
    
    // Building subgraphs
    std::vector<std::vector<int>> subgraph;
    //g.delete_vertex(root);
    g.split_in_subgraphs(articulations, subgraph, g);

    int qtd = subgraph.size();
    DEBUG std::cerr << "Articulacoes detectadas: " << qtd << std::endl;
    
    std::thread vetor_th[qtd];

    int id = 0;
    Graph xpto[qtd];

    for (std::vector<int> sb : subgraph){
        xpto[id] = g.build_subgraph(sb);
        int root = vertice_maior_grau(xpto[id]);
        int neighbor = xpto[id].adjList(root)[0];
        vetor_th[id] = std::thread(find_index_articulation, std::ref(g), std::ref(xpto[id]), root, root, neighbor, id);
        id++;
    }
    for(int i = 0; i < id; ++i){
        vetor_th[i].join();
    }
}


//! Get index from adjacent vertex
/*!
    \param g a graph instance that represents the graph
    \param v a integer that represents a vertex
    \param adj a integer that represents a vertex adjacent at v
    \return a integer that represents a index
*/
int adj_id(Graph& g, int v, int adj)
{
    int index = 0;
    for(int u : g.adjList(v) ){
        if(u == adj){
            break;
        }
        ++index;
    }
    return index;
}

int next(int a, int limite)
{
    ++a;
    return a == limite ? 0 : a;
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

void set_graph_final_parameters(int &index_local, int &total_arv, int &arv, Graph &tree_local, Graph &graph){
    if(index_local < graph.get_stretch_index() && index_local != (int)INFINITY) {
        total_arv += arv;
        graph.set_stretch_index(index_local);
        graph.set_best_tree(tree_local);
        if (index_local==graph.grt - 1){
            graph.set_signal();
            //processando.store(false,std::memory_order_release);
        }
    }
}


/**
 * @brief Search for induced cycles
 * @details Search for induced cycles (if exists)
 * @author Carlos Thadeu
 * @param g a graph instance that represents the graph
 * @return a vector of vector of integers that represents the induced cycles found 
 */
std::vector<std::vector<int>> seeking_induced_cycles(Graph &graph){
    int cycle_size=INDUCED_CYCLE_SYZE_START;    // define the maximum number at edges at induced cycle
	std::vector<std::vector<int>> select_cycles;

	// Seleciona o vertice inicial
    // Seleciona o vértice com maior grau, se houver empate usa a
    // centralidade de proximidade para desempate
    // caso o empate persista, pega o primeiro vértice da lista
	std::vector <int>vertices = graph.vertices_de_maior_grau();
	std::vector<std::pair<int,float>> centrality = Centrality::closeness_centrality_list(vertices, graph);
	int root = Centrality::root_selection2(centrality);
	
    std::vector<int> neighbors = graph.adjList(root);   // Neighbors at root
	int seek = Centrality::root_selection2(Centrality::closeness_centrality_list(neighbors, graph)); // aplica a centralidade de proximidade
	// Fim da seleção do vértice inicial

	std::vector<int> processed; // Processed vertices
	std::vector<int> list_one;  //Vertices to be processed

	//inicializa a lista
	for (int i=0; i<graph.get_qty_vertex(); i++){
		list_one.push_back(i);
	}

    std::vector<int> cycle;
	while (true) {
		DEBUG std::cerr << "(" << cycle_size<< ") ";
		cycle=OpBasic::cycle(graph, cycle_size, seek, root);
		if (cycle.empty()){
            // Não achei ciclo do tamanho pré-definido, procuro um menor
			if (cycle_size > 3){ 
				cycle_size--;
				continue;
			}
			else {  // Não achei nem um triângulo
				break;
			}
		} else {
			cycle_size=INDUCED_CYCLE_SYZE_START;
		}
		join_vectors(processed, cycle);

		// Apaga vertices ja processados
		for (int j=0; j<cycle.size(); j++){
			int index=get_index(cycle[j], list_one);
			if (!list_one.empty() && index < list_one.size())
				list_one.erase (list_one.begin() + index);
			else
                if (list_one.empty() ) 
				    break;
		}
		cycle.push_back(root);
		processed.push_back(root);
		select_cycles.push_back(cycle);

		if (list_one.empty()) break;

		bool gameover = false;
		while (!gameover){
			// Escolhe nova raiz
			centrality = Centrality::closeness_centrality_list(list_one, graph);
			root = Centrality::root_selection2(centrality);
			if (!in(root, processed) || list_one.empty()){
				gameover=true;
			}
		}
		gameover = false;
		neighbors = graph.adjList(root);
		while (!gameover){
			seek = Centrality::root_selection2(Centrality::closeness_centrality_list(neighbors, graph));
			if (!in(seek, processed)){
				gameover=true;
			} else {
				int index=get_index(seek, neighbors);
				neighbors.erase (neighbors.begin() + index);
				if (neighbors.empty()) break;
			}
		}
		if (neighbors.empty()) break;
	}
    return select_cycles;
}