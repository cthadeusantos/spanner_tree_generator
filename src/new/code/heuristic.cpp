#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm> 

#include "heuristic.hpp"
#include "graph.hpp"
#include "../ctfunctions2.hpp"

#include <bits/stdc++.h>


/**
 * @brief T-admissibility heuristic circular.
 * @details The breadth heuristic
 * That heuristic create a tree from a highest degree vertex, than insert your neighbors, after insert the neighbors of neighbors
 * @author Carlos Thadeu
 * @param graph a graph instance that represents the graph.
 */
int Heuristic::breadth_heuristic(Graph &graph)
{
    Stretch stretch;
    int counter = 0;
    int root = 0;
    int n = graph.getQtdVertices();
    std::vector <int> vertices;
    
    Graph tree(n);

    std::queue<int> QUEUE1;
    int LIST2[2][n];

    root = root_selection(graph);

    std::cerr << "Vértice raiz da árvore: " << root << std::endl;

    QUEUE1.push(root);
    

    while (!(QUEUE1.empty())){
        //counter = 0;
        root = QUEUE1.front();
        QUEUE1.pop();
        for (int j =0; j < n; ++j){
            LIST2[0][j] = -1;
            LIST2[1][j] = -1;
        }
        int num_neighbor = graph.adjList(root).size();
        for (int i = 0; i < num_neighbor; ++i){
            int neighbor = graph.adjList(root)[i];
            if (tree.grau(neighbor) == 0) {
                tree.add_aresta(root, neighbor);
                LIST2[0][i] = neighbor;
                //LIST2[0][counter] = neighbor;
                LIST2[1][i] = graph.grau(neighbor);
                //LIST2[1][counter] = graph.grau(neighbor);
                //counter++;
            }
        }

        // BUBLESORT TOSCÃO
        //for (int i=0; i < counter - 1; ++i){
        for (int i=0; i < num_neighbor; ++i){
            //for (int j = 1; j < counter; ++j){
            for (int j = 0; j < num_neighbor; ++j){
                if (LIST2[1][i] < LIST2[1][j]){
                    int temp1 = LIST2[0][j];
                    LIST2[0][j] = LIST2[0][i];
                    LIST2[0][i] = temp1;
                    int temp2 = LIST2[1][j];
                    LIST2[1][j] = LIST2[1][i];
                    LIST2[1][i] = temp2;
                }
            }
        }

        //for (int i = 0; i < counter; ++i){
        for (int i = 0; i < num_neighbor; ++i){
            if (LIST2[0][i] != -1) QUEUE1.push(LIST2[0][i]);
        }
    }
    int factor = stretch.find_factor(graph, tree);
    //std::cout << "******ACHEI FATOR --->>>> :" << factor << std::endl;
    graph.set_stretch_index(factor);
    graph.set_best_tree(tree);
    return 0;
}

/**
 * @brief T-admissibility heuristic 1.
 * @details List vertices in decreasing order.
 * Add to the tree the first vertex of the list is not in the tree yet and all neighbors
 * Process the list until there isn't a vertex in the tree.
 * @param g a graph instance that represents the graph.
 */
int Heuristic::heuristica_1(Graph& g)
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

    //root = root_selection(g);
    root = vertex_list[0];

    std::cerr << "RAIZ ESOLHIDA: " << root << std::endl;
/*     for( int v : g.adjList(vertex_list[0]) )
    {
        tree.add_aresta(vertex_list[0], v);
    } */

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
/*
    f.printAdjMat(tree, "arvore h1:");
    for( int v : vertex_list){
        f.write(v, "", " ");
    }
*/
    return stretch.find_factor(g, tree);
}

int Heuristic::heuristica_2(Graph& g)
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

    return stretch.find_factor(g, tree);
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


int Heuristic::func_aux_h2(Graph& tree, Graph& g, int v)
{
    int count = 0;
    for(int u : g.adjList(v) ){
        if( tree.grau(u) != 0){
            ++count;
        }
    }
    return count;
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

        //////////////
        /* int num_neighbor = graph.adjList(vertex).size();
        int LIST2[2][num_neighbor];

        for (int i = 0; i < num_neighbor; ++i){
                LIST2[0][i] = graph.adjList(vertex)[i];
                LIST2[1][i] = graph.grau(LIST2[0][i]);
        }

        // BUBLESORT TOSCÃO
        //for (int i=0; i < counter - 1; ++i){
        for (int i=0; i < num_neighbor; ++i){
            //for (int j = 1; j < counter; ++j){
            for (int j = 0; j < num_neighbor; ++j){
                if (LIST2[1][i] > LIST2[1][j]){
                    int temp1 = LIST2[0][j];
                    LIST2[0][j] = LIST2[0][i];
                    LIST2[0][i] = temp1;
                    int temp2 = LIST2[1][j];
                    LIST2[1][j] = LIST2[1][i];
                    LIST2[1][i] = temp2;
                }
            }
        }

        
        for (int i=0; i < num_neighbor; i++){
            int v = LIST2[0][i];
            if (!in(FILA2, v)){
                FILA2.push_back(v);
            }
        } */

        //////////////


        for (auto v : graph.adjList(vertex)){
            if (!in(FILA2, v)){
                FILA2.push_back(v);
            }
        }
    }
    while (i < FILA2.size()){
        vertex = FILA2[i];
        if (!in(visited, vertex)){
            FILA.push(vertex);
        }
        i++;
    }
 
    if (soma != 0){
        total_layer.push_back(soma);
        breadth_criterion(graph, FILA, visited, total_layer);
    }
    return total_layer; 
}

int Heuristic::root_selection(Graph &g){    // By thadeu
    int max = 1;
    int n = g.get_qty_vertex();
    std::vector<int> vertex_list(n);
    std::queue <int>FILA;   // contains the vertices with the highest degree (same degree)
    std::vector <std::vector<int>> sum_layer;    // contains the sum of vertices degree of layer

    for( int i = 0; i < n; ++i) vertex_list[i] = i;
    my_quicksort(vertex_list, 0, n, g);

    for (auto vertex: g.vertices_de_maior_grau()){
        FILA.push(vertex);
    }
    
    while (!FILA.empty()){
        std::vector <int>visited;   // Visited vertices
        std::vector <int>total_layer;   // Sum total of the layer
        std::queue <int>FILA3;
        FILA3.push(FILA.front());
        FILA.pop();
        sum_layer.push_back(breadth_criterion(g, FILA3, visited, total_layer));
        FILA3.pop();
    }
    
    for (int index=0; index < sum_layer[0].size(); index++){
        for (int j=0; j < sum_layer.size() - 1; j++){
            if (sum_layer[j][index] > sum_layer[j+1][index]){
                max = index;
            } else if (sum_layer[j+1][index] > sum_layer[j][index]){
                max = index;
            }
        }
        if (max != 0) {
            index = sum_layer.size() + 1;
        }
    }
    return vertex_list[max-1];
}