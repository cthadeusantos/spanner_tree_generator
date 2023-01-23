#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <bits/stdc++.h>

#include "../Debug.h"

#include "heuristic.hpp"
#include "graph.hpp"
#include "centrality.hpp"

#include "../my_libs/ctfunctions2.hpp"

/**
 * @brief T-admissibility Heuristic 3 version 1.
 * @details The heuristic 3 version 1, that's a mix between heuristic 1 & heuristic 2
 * @author Daniel Juventude (original version with bug)
 * @author Carlos Thadeu (modified version)
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::heuristica_3v1(Graph &graph)
{
    Stretch stretch;
    Graph tree(graph.getQtdVertices());
    int raiz = graph.vertice_maior_grau();
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    
    for( int v : graph.adjList(raiz))   // coloca o vertice de maior grau e os seus vizinhos na arvore
        tree.add_aresta(raiz, v);
    
    for(int i = 0; i < graph.getQtdVertices(); ++i) // coloca todos os vertices do grafo na lista
        lista.push_back(i);

    int index = get_index(raiz, lista);
    lista.erase(lista.begin()+index);

    int last;
    bool disconnected=true;
    int source;
    while(disconnected && !lista.empty())
    {
        // Ordena os vértices da lista pelo maior grau (ascendente)
        disconnected=true;
        for(int vertex : lista) 
        {
            int a=graph.grau(vertex);
            lista_relativa_vertice.push_back(vertex);
            if ((in(vertex, tree.my_connected_vertices()))){
                int b=func_aux_h3(tree, graph, vertex);
                lista_relativa_valor.push_back(a - b);
            } else {
                lista_relativa_valor.push_back(a);
            }

        }
        if (tree.my_connected_vertices().size()==graph.get_qty_vertex())
            disconnected=false;
        //my_sort(lista_relativa_valor, lista_relativa_vertice); // Ordena os vértices na ascendente pelos graus
        
        Graph::my_insertionSort_graph(lista_relativa_valor, lista_relativa_vertice, 'a');
        
        // escolhe o vertice de mais alto grau da lista que nao esta na árvore
        int max=0;
        last=source;
        source=-1;
        for (int xpto=0; xpto < lista_relativa_vertice.size(); xpto++){
            if (!(in(lista_relativa_vertice[xpto], tree.my_connected_vertices()))){
                if (lista_relativa_valor[xpto] > max){
                    max = lista_relativa_valor[xpto];
                    source = lista_relativa_vertice[xpto];
                    last=source;
                }
            }
        }

       if (!disconnected){
            source=last;
       }

        for( int vertex : graph.adjList(source))
        {
            if( tree.grau(vertex) == 0)
            {
                tree.add_aresta(source, vertex);
                if (!(in(vertex, lista)))
                    lista.push_back(vertex);
            } else if ( tree.grau(vertex) != 0 && disconnected){
                if (!tree.possui_aresta(source, vertex)){
                    tree.add_aresta(source, vertex);
                    break;
                }
            }
        }
         if (!lista.empty() && disconnected){
            int index = get_index(source, lista);
            lista.erase(lista.begin()+index);
         }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
}

/**
 * @brief T-admissibility Heuristic 3 version 2.
 * @details The heuristic 3 version 2, that's a mix between heuristic 1 & heuristic 2 using vertex importance
 * @author Carlos Thadeu
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::heuristica_3v2(Graph &graph)
{
    Stretch stretch;
    Graph tree(graph.getQtdVertices());
    int raiz = graph.vertice_maior_grau();
    std::vector<int> lista;
    std::vector<float> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    
    //raiz = Centrality::root_selection2(graph);
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(graph);
    raiz =Centrality::root_selection2(vertices_closeness);

    for( int v : graph.adjList(raiz))   // coloca o vertice de maior importancia e os seus vizinhos na arvore
        tree.add_aresta(raiz, v);
    
    for(int i = 0; i < graph.getQtdVertices(); ++i) // coloca todos os vertices do grafo na lista
        lista.push_back(i);

    int index = get_index(raiz, lista);
    lista.erase(lista.begin()+index);

    int last;
    bool disconnected=true;
    int source;
    while(disconnected && !lista.empty())
    {
        // Ordena os vértices da lista pelo maior grau (ascendente)
        disconnected=true;
        for(int vertex : lista) 
        {
            if (!(in(vertex, tree.my_connected_vertices()))){
                lista_relativa_valor.push_back(vertices_closeness[vertex]);
                lista_relativa_vertice.push_back(vertex);
            }

        }
        if (tree.my_connected_vertices().size()==graph.get_qty_vertex())
            disconnected=false;
        //my_sort(lista_relativa_valor, lista_relativa_vertice); // Ordena os vértices na ascendente pelos graus
        
        Graph::my_insertionSort_graph(lista_relativa_valor, lista_relativa_vertice, 'a');
        
        // escolhe o vertice de mais alto grau da lista que nao esta na árvore
        int max=0;
        last=source;
        source=-1;
        for (int xpto=0; xpto < lista_relativa_vertice.size(); xpto++){
            if (!(in(lista_relativa_vertice[xpto], tree.my_connected_vertices()))){
                if (lista_relativa_valor[xpto] > max){
                    max = lista_relativa_valor[xpto];
                    source = lista_relativa_vertice[xpto];
                    last=source;
                }
            }
        }

       if (!disconnected){
            source=last;
       }

        for( int vertex : graph.adjList(source))
        {
            if( tree.grau(vertex) == 0)
            {
                tree.add_aresta(source, vertex);
                if (!(in(vertex, lista)))
                    lista.push_back(vertex);
            } else if ( tree.grau(vertex) != 0 && disconnected){
                if (!tree.possui_aresta(source, vertex)){
                    tree.add_aresta(source, vertex);
                    break;
                }
            }
        }
         if (!lista.empty() && disconnected){
            int index = get_index(source, lista);
            lista.erase(lista.begin()+index);
         }
        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
}

/**
 * @brief T-admissibility breadth heuristic - Heuristic 4.v1
 * @details The breadth heuristic - heuristic 4v1
 * @author Carlos Thadeu
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::breadth_heuristic_1(Graph &graph)
{
    Stretch stretch;
    int counter = 0;
    int root = 0;
    int n = graph.getQtdVertices();
    Graph tree(n);
    
    std::queue <int> QUEUE1;
    std::vector <int> neighbor_list; // newline
    std::vector <int> degree_list; // newline
    std::vector <int> enqueued; // processed vertices that was enqueued anytime


    //std::vector <float> xxx = Centrality::closeness_centrality(graph);

    //root = Centrality::root_selection2(graph);
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(graph);
    root =Centrality::root_selection2(vertices_closeness);

    DEBUG std::cerr << "Selected root: " << root << std::endl;

    QUEUE1.push(root);
    enqueued.push_back(root);

    // the heuristic main loop 
    while (!(QUEUE1.empty())){
        root = QUEUE1.front();
        QUEUE1.pop();

        int num_neighbor = graph.adjList(root).size();
        for (int i = 0; i < num_neighbor; ++i){
            int neighbor = graph.adjList(root)[i];
            if (tree.grau(neighbor) == 0) {
                tree.add_aresta(root, neighbor);
                neighbor_list.push_back(neighbor); // newline
                degree_list.push_back(graph.grau(neighbor));
            }
        }

        Centrality::my_insertionSort(degree_list, neighbor_list, 'd');

        for (int i = 0; i < num_neighbor; ++i){
            if (!in( neighbor_list[i], enqueued)){
                QUEUE1.push(neighbor_list[i]);
                enqueued.push_back(neighbor_list[i]);
            }
        }
        neighbor_list.clear();
        degree_list.clear();
    }
/*     int factor = stretch.find_factor(graph, tree);
    graph.set_stretch_index(factor);
    graph.set_best_tree(tree); */
    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
/*     graph.set_stretch_index(factor);
    graph.set_best_tree(tree); */
    set_graph_final_parameters(factor, tree, graph);
}

/**
 * @brief T-admissibility breadth heuristic - Heuristic 4v2.
 * @details The breadth heuristic - heuristic 4v2
 * @author Carlos Thadeu
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::breadth_heuristic_2(Graph &graph)
{
    Stretch stretch;
    int counter = 0;
    int root = 0;
    int n = graph.getQtdVertices();
    Graph tree(n);
    
    std::queue <int> QUEUE1;
    std::vector <int> neighbor_list; // newline
    std::vector <int> centrality_list; // newline
    std::vector <int> enqueued; // processed vertices that was enqueued anytime
    //std::vector<std::pair<int,float>> vertices_closeness = Centrality::closeness_centrality_list(graph);
    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    //std::vector<float> vertices_closeness = Centrality::closeness_centrality_vector(graph);
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(graph);
    
    DEBUG std::cerr << "Selecting root" << std::endl;
    root = Centrality::root_selection2(vertices_closeness);
    DEBUG std::cerr << "Selected root: " << root << std::endl;

    QUEUE1.push(root);
    enqueued.push_back(root);

    DEBUG std::cerr << "Starting breath heuristic 2" << root << std::endl;
    // the heuristic main loop 
    while (!(QUEUE1.empty())){
        root = QUEUE1.front();
        QUEUE1.pop();

        int num_neighbor = graph.adjList(root).size();
        for (int i = 0; i < num_neighbor; ++i){
            int neighbor = graph.adjList(root)[i];
            if (tree.grau(neighbor) == 0) {
                tree.add_aresta(root, neighbor);
                neighbor_list.push_back(neighbor); // newline
                centrality_list.push_back(graph.grau(neighbor));
            }
        }

        Centrality::my_insertionSort(centrality_list, neighbor_list, 'd');

        for (int i = 0; i < num_neighbor; ++i){
            if (!in( neighbor_list[i], enqueued)){
                QUEUE1.push(neighbor_list[i]);
                enqueued.push_back(neighbor_list[i]);
            }
        }
        neighbor_list.clear();
        centrality_list.clear();
    }
/*     int factor = stretch.find_factor(graph, tree);
    graph.set_stretch_index(factor);
    graph.set_best_tree(tree); */
    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
/*     graph.set_stretch_index(factor);
    graph.set_best_tree(tree); */
    set_graph_final_parameters(factor, tree, graph);
}

/**
 * @brief T-admissibility heuristic 1.
 * @details List vertices in decreasing order.
 * Add to the tree the first vertex of the list is not in the tree yet and all neighbors
 * Process the list until there isn't a vertex in the tree.
 * @author Daniel Juventude
 * @param g a graph instance that represents the graph.
 */
void Heuristic::heuristica_1(Graph& g)
{
    //Frontie'r' f;
    Stretch stretch;
    int n = g.getQtdVertices();
    int root = 0;
    std::vector<int> vertex_list(n);
    for( int i = 0; i < n; ++i)
    {
        vertex_list[i] = i;
    }
    my_quicksort(vertex_list, 0, n, g);
    Graph tree(n);
    root = vertex_list[0];
    //DEBUG std::cerr << "Selected root: " << root << std::endl;
    for( int v : g.adjList(root) )
    {
        tree.add_aresta(root, v);
    }

    int i = 1;
    while( i < n && !OpBasic::is_tree(tree))
    {
        for( int v : g.adjList(vertex_list[i]))
        {
            if( !tree.possui_aresta(vertex_list[i], v))
            {
                tree.add_aresta(vertex_list[i], v);
                if(OpBasic::is_cyclic(tree))
                {
                    tree.remove_aresta(vertex_list[i], v);
                }
            }
        }
        ++i;
    }
    //return stretch.find_factor(g, tree);
    int factor = stretch.find_factor(g, tree); //By thadeu
    g.sum_trees(1); //By thadeu
/*     g.set_stretch_index(factor);
    g.set_best_tree(tree); */
    set_graph_final_parameters(factor, tree, g); //By thadeu
}

/**
 * @brief T-admissibility heuristic 1 modified.
 * @details List vertices in decreasing order by degree and vertex importance.
 * Add to the tree the first vertex of the list is not in the tree yet and all neighbors
 * Process the list until there isn't a vertex in the tree.
 * Only diference about heuristic 1 is the select root. Now considering degree and vertex importance to select root.
 * @author Daniel Juventude(original)
 * @author Carlos Thadeu (modified)
 * @date 2022/12/14
 * @param g a graph instance that represents the graph.
 */
void Heuristic::heuristica_1_modified(Graph& g)
{
    Stretch stretch;
    int n = g.getQtdVertices();
    int root = 0;
    std::vector<int> vertex_list(n);
    //std::vector<float> importance(n); // by thadeu

    DEBUG std::cerr << "calculating vertices importance" << std::endl;
/*     for( int i = 0; i < n; ++i)
    {
        vertex_list[i] = i;
        importance[i] = Centrality::vertex_importance(i, g);
    } */

    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(g);
    DEBUG std::cerr << "start my_insertion_sort" << std::endl;
    Centrality::my_insertionSort(vertex_list, vertices_closeness, g); // Sort by vertices by degree and vertex importance 

    Graph tree(n);

    //root = Centrality::root_selection2(g);
    DEBUG std::cerr << "selecting root" << std::endl;
    root= Centrality::root_selection2(vertices_closeness);
/*     for (int i=0; i < n; i++){
        DEBUG std::cerr << i << "-" << Centrality::vertex_importance(i, g) << std::endl; 
    } */
    
    //DEBUG std::cerr << "Selected root: " << root << std::endl;

    for( int v : g.adjList(root) )  // Start build tree , insert select vertex and all neighbors
    {
        tree.add_aresta(root, v);
    }

    int i = 1;
    while( i < n && !OpBasic::is_tree(tree))
    {
        for( int v : g.adjList(vertex_list[i]))
        {
            if( !tree.possui_aresta(vertex_list[i], v))
            {
                tree.add_aresta(vertex_list[i], v);
                if(OpBasic::is_cyclic(tree))
                {
                    tree.remove_aresta(vertex_list[i], v);
                }
            }
        }
        ++i;
    }
    int factor = stretch.find_factor(g, tree); //By thadeu
/*     g.set_stretch_index(factor);
    g.set_best_tree(tree);
        int factor = stretch.find_factor(graph, tree); */
    g.sum_trees(1); //By thadeu
/*     graph.set_stretch_index(factor);
    graph.set_best_tree(tree); */
    set_graph_final_parameters(factor, tree, g); //By thadeu
}

/**
 * @brief T-admissibility heuristic 2.
 * @details Heuristic 2
 * @author Daniel Juventude
 * @param g a graph instance that represents the graph.
 */
void Heuristic::heuristica_2(Graph& g)
{
    Stretch stretch;
    Graph tree(g.getQtdVertices());
    int raiz = g.vertice_maior_grau();
    
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    for( int v : g.adjList(raiz))
    {
        lista.push_back(v);
        tree.add_aresta(raiz, v);
    }

    while( tree.qtd_vertex_grau() > 0)
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(g.grau(v) - func_aux_h2(tree, g, v));
            lista_relativa_vertice.push_back(v);
        }

        my_sort(lista_relativa_valor, lista_relativa_vertice);

        int u = lista_relativa_vertice.back();
        std::vector<int>::iterator it = std::find(lista.begin(), lista.end(), u);
        lista.erase(it);

        for( int v : g.adjList(u))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(u, v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    //return stretch.find_factor(g, tree);
    int factor = stretch.find_factor(g, tree);
    g.sum_trees(1);
/*     graph.set_stretch_index(factor);
    graph.set_best_tree(tree); */
    set_graph_final_parameters(factor, tree, g);
}

/**
 * @brief T-admissibility heuristic 2 modified.
 * @details List vertices in decreasing order by degree and vertex importance.
 * Add to the tree the vertex with max degree of the leaves's list and all neighbors
 * Process the list until there isn't a vertex in the tree.
 * Differences of heuristic 2 are:
 * tiebrake among vertices at the leaves list is select which vertex has max vertex importance(max centrallity)
 * @author Daniel Juventude(original)
 * @author Carlos Thadeu (modified)
 * @date 2022/12/14
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::heuristica_2_modified(Graph& graph)
{
    Stretch stretch;
    int root = 0; // by thadeu
    int n = graph.getQtdVertices(); // by thadeu

    Graph tree(n);
    std::vector<int> lista;
    std::vector<int> vertex_list(n, 0); // by thadeu
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    //std::vector<float> importance(n, 0); // by thadeu
    std::vector<int> vertex;    // by thadeu
    std::vector<float> closeness; // by thadeu

/*     for( int i = 0; i < n; ++i) // Listing vertices using closeness centrality
    {
        vertex_list[i] = i;
        importance[i] = Centrality::vertex_importance(i, graph);    // calculate the vertex importance
    } */
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(graph);

    Centrality::my_insertionSort(vertex_list, vertices_closeness, graph);

    // After my_insertSort Probably root selection2 is not necessary
    // because the choice is sorted in vertex list, then vertex_list[0]
    // is the root
    // To be analized
    root = Centrality::root_selection2(graph);

    for( int v : graph.adjList(root))
    {
        lista.push_back(v);
        tree.add_aresta(root, v);
    }
    while( tree.qtd_vertex_grau() > 0)
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(graph.grau(v) - func_aux_h2(tree, graph, v));
            lista_relativa_vertice.push_back(v);
        }

        my_sort(lista_relativa_valor, lista_relativa_vertice);

        int u = lista_relativa_vertice.back();

        // CRITERIO DE DESEMPATE SE OS VERTICES TIVEREM MESMO MAX DEGREE
        
        int max_degree = lista_relativa_valor.back();

        for (int i=lista_relativa_valor.size(); i >= 0; i--){   // Seleciona vertices com mesmo grau
            if (lista_relativa_valor[i]==max_degree){
                vertex.push_back(lista_relativa_vertice[i]);
                closeness.push_back(vertices_closeness[i]);
            }
            //if (lista_relativa_valor[i] < max_degree) break;
        }
        float max_closeness = *max_element(closeness.begin(), closeness.end()); // Valor da maior importancia de vértice

        for (int i=0; i < vertex.size(); i++){ // Seleciona vertice com a maior importancia de vertice
            if (closeness[i]==max_closeness) { // se houver empate, o escolhido é o último achado no loop
                u = vertex[i];
            }
        }
        
        // ^^^^^^^^^^^^ ALTERAÇÃO DEVE TERMINAR AQUI  ^^^^^^^^^^^^
        
        std::vector<int>::iterator it = std::find(lista.begin(), lista.end(), u); // by DJ
        lista.erase(it); // by DJ

        for( int v : graph.adjList(u))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(u, v);
                vertex_list.push_back(v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
        vertex.clear();
        closeness.clear();
    }

/*     int factor = stretch.find_factor(graph, tree);
    graph.set_stretch_index(factor);
    graph.set_best_tree(tree); */
    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
/*     graph.set_stretch_index(factor);
    graph.set_best_tree(tree); */
    set_graph_final_parameters(factor, tree, graph);
}

Graph Heuristic::heuristica_2_global(Graph& g)
{
    std::vector<int> vertices_maiores_grau = g.vertices_de_maior_grau();
    Graph tree;
    Graph right_tree;
    Stretch stretch;
    int index = INF_VALUE;
    int new_index = 0;
    for(int vertice : vertices_maiores_grau) {
        tree = heuristica_2_tree(g, vertice);
        new_index = stretch.find_factor(g, tree);

        if(new_index < index) {
            index = new_index;
            right_tree = tree;
        }
    }

    return right_tree;
}

void Heuristic::my_quicksort(std::vector<int>& vertices, int began, int end, Graph& g)
{
	int i, j, pivo, aux;
	i = began;
	j = end-1;
	pivo = g.grau(vertices[(began + end) / 2]);
	while(i <= j)
	{
		while( g.grau(vertices[i]) > pivo && i < end)
		{
			++i;
		}
		while( g.grau(vertices[j]) < pivo && j > began)
		{
			--j;
		}
		if(i <= j)
		{
			aux = vertices[i];
			vertices[i] = vertices[j];
			vertices[j] = aux;
			++i;
			--j;
		}
	}
	if(j > began)
		my_quicksort(vertices, began, j+1, g);
	if(i < end)
		my_quicksort(vertices, i, end, g);
}

/**
 * @brief Auxiliary function to count the degrees of the vertex 
 * @details counts the degrees of the vertex that are in the tree and graph 
 * @author Daniel Juventude
 * @param graph a graph instance that represents the graph.
 * @param tree a graph instance that represents the tree.
 * @param vertex an integer that represents a vertex
 * @return an integer 
 */
int Heuristic::func_aux_h2(Graph &tree, Graph &graph, int vertex)
{
    int count = 0;
    for(int u : graph.adjList(vertex) ){
        if( tree.grau(u) != 0){
            ++count;
        }
    }
    return count;
}

/**
 * @brief Auxiliary function to count the degrees of the vertex 
 * @details counts the degrees of the vertex that are in the tree and graph 
 * @author Daniel Juventude
 * @param graph a graph instance that represents the graph.
 * @param tree a graph instance that represents the tree.
 * @param vertex an integer that represents a vertex
 * @return an integer 
 */
int Heuristic::func_aux_h3(Graph &tree, Graph &graph, int vertex)
{
    int a=graph.grau(vertex) - tree.grau(vertex);
    if (a==0)
        a=graph.grau(vertex);
    return a;
}

void Heuristic::my_sort(std::vector<int>& v1, std::vector<int>& v2)
{
    int n = v1.size();
    for(int i = 0; i < n-1; ++i)
    {
        int j = i;
        while( j > 0 && v1[j] > v1[j+1])
        {
            int aux = v1[j];
            v1[j] = v1[j+1];
            v1[j+1] = aux;

            aux = v2[j];
            v2[j] = v2[j+1];
            v2[j+1] = aux;

            --j;
        }
    }
}

bool Heuristic::sortby2nd_asc(const std::tuple<int, int> &a, 
               const std::tuple<int, int> &b)   // By thadeu
{
    return (std::get<1>(a) < std::get<1>(b));
}

bool Heuristic::sortby2nd_des(const std::tuple<int, int> &a, 
               const std::tuple<int, int> &b)   // By thadeu
{
    return (std::get<1>(a) > std::get<1>(b));
}

std::vector <int> Heuristic::breadth_criterion(Graph &graph, std::queue <int> &FILA,
                std::vector <int> &visited, std::vector <int> &total_layer){    // By thadeu
    int soma = 0;
    int vertex = -1;
    int i = 0;
    std::vector <int>FILA2;
    
    while (!FILA.empty()){
        vertex = FILA.front();
        FILA.pop();
        int vgrau = graph.grau(vertex);
        soma = soma + vgrau;
        visited.push_back(vertex);

        for (auto v : graph.adjList(vertex)){
            if (!in(v, FILA2)){
                FILA2.push_back(v);
                
            }
        }
    }
    int parent = vertex;
    while (i < FILA2.size()){
        vertex = FILA2[i];
        if (!in(vertex, visited)){
            FILA.push(vertex);
            //tree.add_aresta(parent, vertex);
        }
        i++;
    }
 
    if (soma != 0){
        total_layer.push_back(soma);
        breadth_criterion(graph, FILA, visited, total_layer);
    }
    return total_layer; 
}

/**
 * @brief Auxiliary function to setting all parameters after stretch index found
 * @details Auxiliary function to setting all parameters after stretch index found
 * @author Carlos Thadeu
 */
void Heuristic::set_graph_final_parameters(int &index_local, Graph &tree_local, Graph &graph){
    if(index_local < graph.get_stretch_index() && index_local != (int)INFINITY) {
        graph.set_stretch_index(index_local);
        graph.set_best_tree(tree_local);
        if (index_local==graph.grt - 1){
            graph.set_signal();
            //processando.store(false,std::memory_order_release);
        }
    }
}
//**********************************************************************************************
//
// ESTES CÓDIGOS SÃO IGUAIS AS HEURÍSTICAS ORIGINAIS, APENAS RETORNAM A ÁRVORE AO INVÉS DO FATOR.
// MANTIDOS CASO O CÓDIGO ANTIGO FAÇA USO DELES EM ALGUM MOMENTO E EM ALGUM PONTO
// A SER REVISADO PARA REMOÇÃO FUTURA
//
// Com as alterações feitas (thadeu) o grafo passou a ter os atributos best_tree and stretch index
// provavelmente tornando desnecessário estes metodos
// Mantidos para efeitos de compatibilidade até serem depreciados por completos
//**********************************************************************************************


Graph Heuristic::heuristica_1_tree(Graph& g)
{
    //Frontier f;
    Stretch stretch;
    int n = g.getQtdVertices();
    std::vector<int> vertex_list(n);
    for( int i = 0; i < n; ++i)
    {
        vertex_list[i] = i;
    }
    
    my_quicksort(vertex_list, 0, n, g);

    Graph tree(n);
    
    for( int v : g.adjList(vertex_list[0]) )
    {
        tree.add_aresta(vertex_list[0], v);
    }
    
    int i = 1;
    while( i < n && !OpBasic::is_tree(tree))
    {
        for( int v : g.adjList(vertex_list[i]))
        {
            if( !tree.possui_aresta(vertex_list[i], v))
            {
                tree.add_aresta(vertex_list[i], v);
                if(OpBasic::is_cyclic(tree))
                {
                    tree.remove_aresta(vertex_list[i], v);
                }
            }
        }
        
        ++i;
    }

    return tree;
}

Graph Heuristic::heuristica_2_tree(Graph& g)
{
    // Strech strech;
    Graph tree(g.getQtdVertices());
    int raiz = g.vertice_maior_grau();
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    for( int v : g.adjList(raiz))
    {
        lista.push_back(v);
        tree.add_aresta(raiz, v);
    }
    while( tree.qtd_vertex_grau() > 0)
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(g.grau(v) - func_aux_h2(tree, g, v));
            lista_relativa_vertice.push_back(v);
        }

        my_sort(lista_relativa_valor, lista_relativa_vertice);

        int u = lista_relativa_vertice.back();
        std::vector<int>::iterator it = std::find(lista.begin(), lista.end(), u);
        lista.erase(it);

        for( int v : g.adjList(u))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(u, v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    return tree;
}

Graph Heuristic::heuristica_2_tree(Graph& g, int raiz)
{
    // Strech strech;
    Graph tree(g.getQtdVertices());
    // int raiz = g.vertice_maior_grau();
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    for( int v : g.adjList(raiz))
    {
        lista.push_back(v);
        tree.add_aresta(raiz, v);
    }
    while( tree.qtd_vertex_grau() > 0)
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(g.grau(v) - func_aux_h2(tree, g, v));
            lista_relativa_vertice.push_back(v);
        }

        my_sort(lista_relativa_valor, lista_relativa_vertice);

        int u = lista_relativa_vertice.back();
        std::vector<int>::iterator it = std::find(lista.begin(), lista.end(), u);
        lista.erase(it);

        for( int v : g.adjList(u))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(u, v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    return tree;
}

Graph Heuristic::heuristica_3_tree(Graph& g)
{
    Graph tree(g.getQtdVertices());
    int raiz = g.vertice_maior_grau();
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    
    // coloca o vertice de maior grau e os seus vizinhos na arvore
    for( int v : g.adjList(raiz))
    {
        tree.add_aresta(raiz, v);
    }

    // coloca todos os vertices do grafo na lista
    for(int i = 0; i < g.getQtdVertices(); ++i)
    {
        lista.push_back(i);
    }

    while( tree.qtd_vertex_grau() > 0 )
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(g.grau(v) - func_aux_h2(tree, g, v));
            lista_relativa_vertice.push_back(v);
        }

        my_sort(lista_relativa_valor, lista_relativa_vertice);

        int u = lista_relativa_vertice.back();

        for( int v : g.adjList(u))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(u, v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    if( !OpBasic::is_tree(tree) )
    {
        std::vector<int> list = OpBasic::diference_edge(g, tree);
        int i = 0;
        while( !OpBasic::is_tree(tree) )
        {
            tree.add_aresta(i, i+1);
            if( OpBasic::is_cyclic(tree) )
            {
                tree.remove_aresta(i, i+1);
            }
            i += 2;
        }
    }

    return tree;
}