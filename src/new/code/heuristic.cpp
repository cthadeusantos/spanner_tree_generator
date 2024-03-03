#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
//#include <bits/stdc++.h>
#include <limits.h>
#include "../Debug.h"
#include "heuristic.hpp"
#include "graph.hpp"
#include "centrality.hpp"
#include <iostream>
using namespace std;
#include "opBasic.hpp"
#include "../my_libs/library1.hpp"
#include "../my_libs/ctfunctions2.hpp"

extern int global_closeness;
/*
* Struct to return a tree and your factor
*/
struct treeWFactor
{
    Graph tree;
    int factor;
};

//Global variable to control the number of trees created
int trees_created = 0;

/**
 * @brief T-admissibility heuristic 1.
 * @details List vertices in decreasing order.
 * Add to the tree the first vertex of the list is not in the tree yet and all neighbors
 * Process the list until there isn't a vertex in the tree.
 * @author Daniel Juventude
 * @param g a graph instance that represents the graph.
 */
void Heuristic::heuristica_1v1(Graph& g)
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
    int factor = stretch.find_factor(g, tree); //By thadeu
    g.sum_trees(1); //By thadeu
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
void Heuristic::heuristica_1v2(Graph& graph)
{
    int n = graph.getQtdVertices();
    int root = 0;
    std::vector<int> vertex_list(n);

    DEBUG std::cerr << "calculating vertices importance" << std::endl;

    for( int i = 0; i < n; ++i)
    {
        vertex_list[i] = i;
        //importance[i] = Centrality::vertex_importance(i, g);
    }

    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness;
    if (global_closeness == 2) {
        vertices_closeness = Centrality::closeness_centrality_thread(graph);
    } else {
        vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    }
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);
    
    //DEBUG std::cerr << "Selecting root" << std::endl;
    //root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    //DEBUG std::cerr << "Selected root: " << root << std::endl;

    // SELECAO PELO METODO CLOSENESS CENTRALITY COM PRECISAO, POREM DEMORADO PARA GRAFOS GRANDES
    //std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(g);
    //// Root must be select before sorting
    //DEBUG std::cerr << "selecting root H1V2" << std::endl;
    //root= Centrality::root_selection2(vertices_closeness);

    DEBUG std::cerr << "start my_insertion_sort" << std::endl;
    // Ordena os vértices já considerando a importancia dos vertices como criterio de desempate
    //Centrality::my_insertionSort(vertex_list, vertices_closeness, graph); 
    Centrality::my_insertionSort(vertex_list, vertices_closeness, vertices_leverage, graph); // Sort by vertices by degree and vertex importance 

    root = vertex_list[0];

    Graph tree(n);

    for( int v : graph.adjList(root) )  // Start build tree , insert select vertex and all neighbors
    {
        tree.add_aresta(root, v);
    }

    int i = 1;
    while( i < n && !OpBasic::is_tree(tree))
    {
        for( int v : graph.adjList(vertex_list[i]))
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
    int factor = Stretch::find_factor(graph, tree); //By thadeu
    graph.sum_trees(1); //By thadeu
    set_graph_final_parameters(factor, tree, graph); //By thadeu
}

/**
 * @brief Auxiliary function to generate all possible trees in case of a tie
 * @details This function return the tree with the lower factor
 * @author Eriky Marciano
 */
treeWFactor verify_possibilities(Graph& g, std::vector<int>& vertex_list, int actual_position){
    Stretch stretch;
    int aux;
    treeWFactor result, result2, result_aux;
    //result2.factor = INT_MAX;
    result2.factor = std::numeric_limits<int>::max();
    int n = g.getQtdVertices();
    int root = 0;
    //int limit = INT_MAX;
    int limit = std::numeric_limits<int>::max();

    if(trees_created > limit) return result2;
    //loop que verifica se ha empate
    for( int i = actual_position; i < n-1; ++i)
    {
        if(trees_created > limit) break;
        if(g.grau(vertex_list[i]) == g.grau(vertex_list[i+1])){
            std::vector<int> copy_vertex_list;
            copy_vertex_list = vertex_list;
            aux = copy_vertex_list[i];
            copy_vertex_list[i] = copy_vertex_list[i+1];
            copy_vertex_list[i+1] = aux;
            trees_created = trees_created+1;
            result_aux = verify_possibilities(g, copy_vertex_list, i+1);
            if(result_aux.factor < result2.factor) result2 = result_aux;
        }
        ++i;
    }
    Graph tree(n);
    result.tree = tree;
    root = vertex_list[0];

    for( int v : g.adjList(root) )
    {
        tree.add_aresta(root, v);
    }

    int j = 1;
    while( j < n && !OpBasic::is_tree(tree))
    {
        for( int v : g.adjList(vertex_list[j]))
        {
            if( !tree.possui_aresta(vertex_list[j], v))
            {
                tree.add_aresta(vertex_list[j], v);
                if(OpBasic::is_cyclic(tree))
                {
                    tree.remove_aresta(vertex_list[j], v);
                }
            }
        }
        ++j;
    }
    result.factor = stretch.find_factor(g, tree); //By thadeu
    if(result.factor > result2.factor){
        return result2;
    } else {
        return result;
    }
}

/**
 * @brief T-admissibility heuristic 1 modified.
 * @details The main diference about heuristic 1 is the select root. Now considering degree and vertex importance to select root.
 * @author Daniel Juventude(original)
 * @author Eriky Marciano (modified)
 * @date 2023/09/20
 * @param g a graph instance that represents the graph.
 */

void Heuristic::heuristica_1v3(Graph& g)
{
    Stretch stretch;
    int n = g.getQtdVertices();
    int root = 0;
    std::vector<int> vertex_list(n);
    for( int i = 0; i < n; ++i)
    {
        vertex_list[i] = i;
    }
    my_quicksort(vertex_list, 0, n, g);
    treeWFactor result = verify_possibilities(g, vertex_list, 0);
    g.sum_trees(1); //By thadeu
    set_graph_final_parameters(result.factor, result.tree, g); //By thadeu
    g.set_best_tree(result.tree);
}

void sort_edges(std::map<string,int> edges_occurrence, vector<pair<int,int>> &edge_list, int num_edges, int atual){
    int i, j, pivo;
    pair<int,int> aux;
	i = atual;
	j = num_edges-1;
    if(edge_list[(atual + num_edges) / 2].first > edge_list[(atual + num_edges) / 2].second){
        pivo = edges_occurrence[to_string(edge_list[(atual + num_edges) / 2].first)+"_"+to_string(edge_list[(atual + num_edges) / 2].second)];
    } else {
        pivo = edges_occurrence[to_string(edge_list[(atual + num_edges) / 2].second)+"_"+to_string(edge_list[(atual + num_edges) / 2].first)];
    }
	while(i <= j)
	{
        if(edge_list[i].first > edge_list[i].second){
            while(edges_occurrence[to_string(edge_list[i].first)+"_"+to_string(edge_list[i].second)] > pivo && i < num_edges)
            {
                ++i;
            }
            while(edges_occurrence[to_string(edge_list[j].first)+"_"+to_string(edge_list[j].second)] < pivo && j > atual)
            {
                --j;
            }
            if(i <= j)
            {
                aux = edge_list[i];
                edge_list[i] = edge_list[j];
                edge_list[j] = aux;
                ++i;
                --j;
            }
        }
		
	}
	if(j > atual)
		sort_edges(edges_occurrence, edge_list, j+1, atual);
	if(i < num_edges)
		sort_edges(edges_occurrence, edge_list, num_edges, i);

}

std::map<string,int> count_edges(Graph& g, std::vector<int>& vertex_list, int actual_position){
    Stretch stretch;
    int aux;
    treeWFactor result;
    int n = g.getQtdVertices();
    int root = 0;
    int limit = 2;
    std::map<string, int> edges_occurrence;
    
    //loop que verifica se ha empate
    for( int i = actual_position; i < n-1; ++i)
    {
        if(trees_created > limit) break;
        if(g.grau(vertex_list[i]) == g.grau(vertex_list[i+1])){
            std::vector<int> copy_vertex_list;
            copy_vertex_list = vertex_list;
            aux = copy_vertex_list[i];
            copy_vertex_list[i] = copy_vertex_list[i+1];
            copy_vertex_list[i+1] = aux;
            trees_created = trees_created+1;            
            edges_occurrence = count_edges(g, copy_vertex_list, i+1);
            if(trees_created > limit) return edges_occurrence;
        }
        ++i;
    }

    Graph tree(n);
    result.tree = tree;
    root = vertex_list[0];
    string edge_key;

    for( int v : g.adjList(root) )
    {
        tree.add_aresta(root, v);
        
        if(root>v){
            edge_key = to_string(root)+"_"+to_string(v);
        } else edge_key = to_string(v)+"_"+to_string(root);
        
        if (edges_occurrence.find(edge_key) == edges_occurrence.end()) {
        // not found
            edges_occurrence.insert({edge_key, 1});
        } else {
        // found
            edges_occurrence[edge_key] = edges_occurrence[edge_key]+1;
        }
    }

    int j = 1;
    while( j < n && !OpBasic::is_tree(tree))
    {
        for( int v : g.adjList(vertex_list[j]))
        {
            if( !tree.possui_aresta(vertex_list[j], v))
            {
                tree.add_aresta(vertex_list[j], v);
                if(OpBasic::is_cyclic(tree))
                {
                    tree.remove_aresta(vertex_list[j], v);
                } else {
                    if(root>v){
                        edge_key = to_string(root)+"_"+to_string(v);
                    } else edge_key = to_string(v)+"_"+to_string(root);
                    
                    if (edges_occurrence.find(edge_key) == edges_occurrence.end()) {
                    // not found
                        edges_occurrence.insert({edge_key, 1});
                    } else {
                    // found
                        edges_occurrence[edge_key] = edges_occurrence[edge_key]+1;
                    }
                }
            }
        }
        ++j;
    }
    return edges_occurrence;
}

void Heuristic::heuristica_1v4(Graph& g)
{    
    Stretch stretch;
    int n = g.getQtdVertices();
    int root = 0;
    std::vector<int> vertex_list(n);
    Graph result_tree(n);
    for( int i = 0; i < n; ++i){
        vertex_list[i] = i;
    }
    my_quicksort(vertex_list, 0, n, g);
    int number_edges = g.getQtdArestas();
    std::vector<std::pair<int, int>> sorted_edge_list = g.get_edges_set();
    sort_edges(count_edges(g, vertex_list, 0), sorted_edge_list, number_edges, 0);
    int k = 0;
    while( k < number_edges && !OpBasic::is_tree(result_tree))
    {
        if( !result_tree.possui_aresta(sorted_edge_list[k].first, sorted_edge_list[k].second))
        {
            result_tree.add_aresta(sorted_edge_list[k].first, sorted_edge_list[k].second);
            if(OpBasic::is_cyclic(result_tree))
            {
                result_tree.remove_aresta(sorted_edge_list[k].first, sorted_edge_list[k].second);
            }
        }
        ++k;
    }
    int factor = stretch.find_factor(g, result_tree); //By thadeu
    g.sum_trees(1); //By thadeu
    set_graph_final_parameters(factor, result_tree, g); //By thadeu
}

/**
 * @brief T-admissibility heuristic 2.
 * @details Heuristic 2
 * @author Daniel Juventude
 * @param g a graph instance that represents the graph.
 */
void Heuristic::heuristic_2v1(Graph& graph)
{
    Stretch stretch;
    Graph tree(graph.getQtdVertices());
    int raiz = graph.highest_degree_vertex();
    int source;

    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    for( int v : graph.adjList(raiz))
    {
        lista.push_back(v);
        tree.add_aresta(raiz, v);
    }

    while( tree.qtd_vertex_grau() > 0)
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(graph.grau(v) - func_aux_h2(tree, graph, v));
            lista_relativa_vertice.push_back(v);
        }

        //my_sort(lista_relativa_valor, lista_relativa_vertice);
        Centrality::my_insertionSort(lista_relativa_valor, lista_relativa_vertice, 'a');

        source = lista_relativa_vertice.back();
        std::vector<int>::iterator it = std::find(lista.begin(), lista.end(), source);
        lista.erase(it);

        for( int v : graph.adjList(source))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(source, v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
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
void Heuristic::heuristica_2v2(Graph& graph)
{
    Stretch stretch;
    int root = 0; // by thadeu
    int n = graph.getQtdVertices(); // by thadeu
    int source;
    Graph tree(n);
    std::vector<int> lista;
    std::vector<int> vertex_list(n, 0); // by thadeu
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    //std::vector<float> importance(n, 0); // by thadeu
    std::vector<int> vertex;    // by thadeu
    //std::vector<float> closeness; // by thadeu

    for( int i = 0; i < n; ++i) // Listing vertices using closeness centrality
    {
        vertex_list[i] = i;
        //importance[i] = Centrality::vertex_importance(i, graph);    // calculate the vertex importance
    }
    
    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness;
    if (global_closeness == 2) {
        vertices_closeness = Centrality::closeness_centrality_thread(graph);
    } else {
        vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    }
    //std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);
    //DEBUG std::cerr << "Selecting root" << std::endl;
    //root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    //DEBUG std::cerr << "Selected root: " << root << std::endl;

    // SELECAO PELO METODO CLOSENESS CENTRALITY COM PRECISAO, POREM DEMORADO PARA GRAFOS GRANDES
    //std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(graph);
    // Root must be select before sorting
    //DEBUG std::cerr << "selecting root H1V2" << std::endl;
    //root= Centrality::root_selection2(vertices_closeness);

    //Centrality::my_insertionSort(vertex_list, vertices_closeness, graph);
    DEBUG std::cerr << "Sorting: " << std::endl;
    Centrality::my_insertionSort(vertex_list, vertices_closeness, vertices_leverage, graph); // Sort by vertices by degree and vertex importance 

    root = vertex_list[0];
    
    // After my_insertSort Probably root selection2 is not necessary
    // because the choice is sorted in vertex list, then vertex_list[0]
    // is the root
    // To be analized
    //root = Centrality::root_selection2(graph);

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

        //my_sort(lista_relativa_valor, lista_relativa_vertice);
        Centrality::my_insertionSort(lista_relativa_valor, lista_relativa_vertice, 'a');

        //int u = lista_relativa_vertice.back();

        // CRITERIO DE DESEMPATE SE OS VERTICES TIVEREM MESMO MAX DEGREE

        int max_degree = lista_relativa_valor.back();

        for (int i=lista_relativa_valor.size(); i >= 0; i--){   // Seleciona vertices com mesmo grau
            if (lista_relativa_valor[i]==max_degree){
                vertex.push_back(lista_relativa_vertice[i]);
                //closeness.push_back(vertices_closeness[i]);
            }
            ////if (lista_relativa_valor[i] < max_degree) break;
        }

    //ISSO AQUI TRAZ MELHORES RESULTADOS QUE tiebreaker(vertex, vertices_closeness, vertices_leverage)
    //mas so leva em consideração closeness calculado
    // O ideal seria testar em grafos grandes para validar sua qualidade
/*         float max_closeness = *max_element(closeness.begin(), closeness.end()); // Valor da maior importancia de vértice

        for (int i=0; i < vertex.size(); i++){ // Seleciona vertice com a maior importancia de vertice
            if (closeness[i]==max_closeness) { // se houver empate, o escolhido é o último achado no loop
                u = vertex[i];
            }
        } */

        //source=Centrality::tiebreaker(vertex, vertices_closeness, vertices_leverage);
        source=Centrality::tiebreaker(vertex, vertex_list, vertices_closeness, vertices_leverage);
        
        // ^^^^^^^^^^^^ ALTERAÇÃO DEVE TERMINAR AQUI  ^^^^^^^^^^^^

        std::vector<int>::iterator it = std::find(lista.begin(), lista.end(), source); // by DJ
        lista.erase(it); // by DJ

        for( int v : graph.adjList(source))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(source, v);
                vertex_list.push_back(v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
        vertex.clear();
        //closeness.clear();
    }

    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
}

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
    int source = graph.highest_degree_vertex();
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;

    for(int i = 0; i < graph.getQtdVertices(); ++i) // coloca todos os vertices do grafo na lista
        lista.push_back(i);

    for( int v : graph.adjList(source))
        tree.add_aresta(source, v); // coloca o vertice de maior grau e os seus vizinhos na arvore

    int index = get_index(source, lista);
    lista.erase(lista.begin()+index);   // Remove o raiz da lista ,

    while(!lista.empty())
    {
        // Subtrai dos graus dos vertices da lista dos grau dos vertices que já estão na arvore
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

        //my_sort(lista_relativa_valor, lista_relativa_vertice); // Ordena os vértices na ascendente pelos graus
        // Ordena os vértices da lista pelo maior grau (ascendente)
        Graph::my_insertionSort_graph(lista_relativa_valor, lista_relativa_vertice, 'a');

        // escolhe o vertice de mais alto grau da lista que nao esta na árvore
        //int max=0;
        //for (int xpto=0; xpto < lista_relativa_vertice.size(); xpto++){
        //    if (!(in(lista_relativa_vertice[xpto], tree.my_connected_vertices()))){
        //        if (lista_relativa_valor[xpto] > max){
        //            max = lista_relativa_valor[xpto];
        //            source = lista_relativa_vertice[xpto];
        //        }
        //    }
        //}
        source = lista_relativa_vertice[lista_relativa_vertice.size()-1];

       bool connect = false;    // if source vertex connect to leaf, it cannot connect a leaf again
        for( int vertex : graph.adjList(source))
        {
            // If vertex to be added not create a cycle
            // add edge (source, vertex)
            if (tree.grau(vertex)==0)
            {
                if (tree.possui_aresta(source, vertex)==false)
                    tree.add_aresta(source, vertex);
            } //else if (tree.grau(vertex) > 0 && connect == false && tree.possui_aresta(source, vertex)==false){
            //     tree.add_aresta(source, vertex);
            //     connect = true;
            // }
            else {
                if (tree.grau(vertex) > 0 && connect == false && tree.possui_aresta(source, vertex)==false){
                    tree.add_aresta(source, vertex);
                    bool has_cycle_var = false;
                    if (OpBasic::cyclic(tree, source)){
                        has_cycle_var = true;
                    }
                    if (!has_cycle_var)
                        if (OpBasic::cyclic(tree, vertex))
                            has_cycle_var = true;
                    if (has_cycle_var){
                        tree.remove_aresta(source, vertex);
                    }
                    connect = true;
                }
            }
            if (tree.get_num_edges() == tree.get_num_vertices() - 1){
                break;
            }
        }
        if (tree.get_num_edges() == tree.get_num_vertices() - 1){
            break;
        }
        if (!lista.empty()){
            int index = get_index(source, lista);
            lista.erase(lista.begin()+index);
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
        if (tree.get_num_edges() + 1 == graph.get_qty_vertex())
            lista.clear();
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
    //Stretch stretch;
    Graph tree(graph.getQtdVertices());
    std::vector<int> lista_vertices_candidatos = graph.vertices_de_maior_grau();

    int source = graph.highest_degree_vertex();
    std::vector<int> lista;
    std::vector<float> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;

    //raiz = Centrality::root_selection2(graph);

  /*   std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(graph);

    source = Centrality::tiebreaker(lista_vertices_candidatos, vertices_closeness); */
    
    // Select which type closeness will be computed (ALGEBRAIC or TRAVERSE) 
    // from arguments
    std::vector<float> vertices_closeness;
    if (global_closeness == 2) {
        vertices_closeness = Centrality::closeness_centrality_thread(graph);
    } else {
        vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    }

    //std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);

    DEBUG std::cerr << "Selecting root" << std::endl;
    //root = Centrality::root_selection2(vertices_closeness);
    
    //source = Centrality::root_selection3(vertices_closeness, vertices_leverage); // ORIGINAL


    //raiz =Centrality::root_selection2(vertices_closeness);
    //DEBUG std::cerr << "Root:" << source << std::endl;
    
    int n = graph.getQtdVertices(); // by thadeu
    std::vector<int> vertex_list(n, 0); // by thadeu
    
    for( int i = 0; i < n; ++i) // Listing vertices using closeness centrality
    {
        vertex_list[i] = i;
        //importance[i] = Centrality::vertex_importance(i, graph);    // calculate the vertex importance
    }

    DEBUG std::cerr << "Sorting: " << std::endl;
    Centrality::my_insertionSort(vertex_list, vertices_closeness, vertices_leverage, graph); // Sort by vertices by degree and vertex importance
    source = vertex_list[0];
    DEBUG std::cerr << "Root:" << source << std::endl;

    for(int i = 0; i < graph.getQtdVertices(); ++i) // coloca todos os vertices do grafo na lista
        lista.push_back(i);

    for( int v : graph.adjList(source))   // coloca o vertice de maior importancia e os seus vizinhos na arvore
        tree.add_aresta(source, v);

    int index = get_index(source, lista);
    lista.erase(lista.begin()+index);

    while(!lista.empty())
    {
        // Subtrai do grauos vertices dos vizinhos dos vertices que já estão na arvore
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

        //my_sort(lista_relativa_valor, lista_relativa_vertice); // Ordena os vértices na ascendente pelos graus
        // Ordena os vértices da lista pelo maior grau (ascendente)
        Graph::my_insertionSort_graph(lista_relativa_valor, lista_relativa_vertice, 'a');

        // escolhe o vertice de mais alto grau da lista que nao esta na árvore
        //int max=0;
        //for (int xpto=0; xpto < lista_relativa_vertice.size(); xpto++){
        //    if (!(in(lista_relativa_vertice[xpto], tree.my_connected_vertices()))){
        //        if (lista_relativa_valor[xpto] > max){
        //            max = lista_relativa_valor[xpto];
        //            source = lista_relativa_vertice[xpto];
        //        }
        //    }
        //}
        int max_degree = lista_relativa_valor[lista_relativa_valor.size()-1];
        lista_vertices_candidatos.clear();
        for (int i=lista_relativa_valor.size()-1; i > 0; i--){
            if (lista_relativa_valor[i]==max_degree){
                lista_vertices_candidatos.push_back(lista_relativa_vertice[i]);
            }
            else{
                break;
            }
        }
        if (lista_vertices_candidatos.empty()){
            break;
        }
        //source = Centrality::tiebreaker(lista_vertices_candidatos, vertices_closeness, vertices_leverage);
        source=Centrality::tiebreaker(lista_vertices_candidatos, vertex_list, vertices_closeness, vertices_leverage);
        //source = lista_relativa_vertice[lista_relativa_vertice.size()-1];

        bool connect = false;    // if source vertex connect to leaf, it cannot connect a leaf again
        for( int vertex : graph.adjList(source))
        {
            // If vertex to be added not create a cycle
            // add edge (source, vertex)
            if (tree.grau(vertex)==0)
            {
                if (tree.possui_aresta(source, vertex)==false)
                    tree.add_aresta(source, vertex);
            } //else if (tree.grau(vertex) > 0 && connect == false && tree.possui_aresta(source, vertex)==false){
                //tree.add_aresta(source, vertex);
                //connect = true;
            //}
            else {
                if (tree.grau(vertex) > 0 && connect == false && tree.possui_aresta(source, vertex)==false){
                    tree.add_aresta(source, vertex);
                    bool has_cycle_var = false;
                    if (OpBasic::cyclic(tree, source)){
                        has_cycle_var = true;
                    }
                    if (!has_cycle_var)
                        if (OpBasic::cyclic(tree, vertex))
                            has_cycle_var = true;
                    if (has_cycle_var){
                        tree.remove_aresta(source, vertex);
                    }
                    connect = true;
                }
            }
            if (tree.get_num_edges() == tree.get_num_vertices() - 1){
                break;
            }
        }
        if (tree.get_num_edges() == tree.get_num_vertices() - 1){
            break;
        }
        if (!lista.empty()){
            int index = get_index(source, lista);
            lista.erase(lista.begin()+index);
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
        if (tree.get_num_edges() + 1 == graph.get_qty_vertex())
            lista.clear();
    }
    int factor = Stretch::find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
}

/**
 * @brief T-admissibility breadth heuristic - Heuristic 4.v1
 * @details The breadth heuristic - heuristic 4v1
 * @author Carlos Thadeu
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::Heuristica_4v1(Graph &graph)
{
    Stretch stretch;
    //int counter = 0;
    int root = 0;
    int n = graph.getQtdVertices();
    Graph tree(n);

    std::queue <int> QUEUE1;
    std::vector <int> neighbor_list; // newline
    std::vector <int> degree_list; // newline
    //std::vector <int> enqueued; // processed vertices that was enqueued anytime
    std::vector <bool> visited(n, false); // processed vertices that was enqueued anytime

    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);

    std::vector<int> lista_vertices_candidatos = graph.vertices_de_maior_grau();
    DEBUG std::cerr << "Selecting root" << std::endl;
    //root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    root = Centrality::tiebreaker(lista_vertices_candidatos, vertices_closeness, vertices_leverage);
    DEBUG std::cerr << "Selected root: " << root << std::endl;

    ////std::vector <float> xxx = Centrality::closeness_centrality(graph);
    ////root = Centrality::root_selection2(graph);
    //std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(graph);
    //root=Centrality::root_selection2(vertices_closeness);
    //DEBUG std::cerr << "Selected root: " << root << std::endl;

    QUEUE1.push(root);
    //enqueued.push_back(root);
    visited[root]=true;

    //Centrality::my_insertionSort(degree_list, neighbor_list, 'd');

    // the heuristic main loop
    while (!(QUEUE1.empty())){
        root = QUEUE1.front();
        QUEUE1.pop();

        //int num_neighbor = graph.adjList(root).size();
        //for (int i = 0; i < num_neighbor; ++i){
        //    int neighbor = graph.adjList(root)[i];
        //    if (tree.grau(neighbor) == 0) {
        //        tree.add_aresta(root, neighbor);
        //        neighbor_list.push_back(neighbor); // newline
        //        degree_list.push_back(graph.grau(neighbor));
        //    }
        //}
        for (auto vertex: graph.adjList(root)){
            if (tree.grau(vertex) == 0) {
                tree.add_aresta(root, vertex);
                neighbor_list.push_back(vertex); // newline
                degree_list.push_back(graph.grau(vertex));
            }
        }

        Centrality::my_insertionSort(degree_list, neighbor_list, 'd');

        //for (int i = 0; i < neighbor_list.size(); ++i){ // Fixed core dumped BUG - change num_neighbor for neighbor_list.size()
        //    if (!in( neighbor_list[i], enqueued)){
        //        QUEUE1.push(neighbor_list[i]);
        //        enqueued.push_back(neighbor_list[i]);
        //    }
        //}
        for (auto vertex : neighbor_list){
            if (!visited[vertex]){
                QUEUE1.push(vertex);
                visited[vertex]=true;
            }
        }
        neighbor_list.clear();
        degree_list.clear();
    }

    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
}

/**
 * @brief T-admissibility breadth heuristic - Heuristic 4v2.
 * @details The breadth heuristic - heuristic 4v2
 * @author Carlos Thadeu
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::Heuristica_4v2r1(Graph &graph)
{
    Stretch stretch;
    //int counter = 0;
    int root = 0;
    int n = graph.getQtdVertices();
    Graph tree(n);

    // ************ REMOVER WHEN JOIN HEURISTICS ****
    // ***** TEMPORALLY ****
    global_closeness = 2;
    // ***********************

    std::queue <int> QUEUE1;
    std::vector <int> neighbor_list; // newline
    std::vector <float> closeness_list; // newline
    //std::vector <int> enqueued; // processed vertices that was enqueued anytime
    std::vector <bool> visited(n, false); // processed vertices that was enqueued anytime

    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);
    DEBUG std::cerr << "Selecting root" << std::endl;
    root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    DEBUG std::cerr << "Selected root: " << root << std::endl;

    QUEUE1.push(root);
    //enqueued.push_back(root);
    visited[root]=true;

    DEBUG std::cerr << "Starting Heuristica_4v2r1 - Closeness centrality - better precision" << root << std::endl;

    //Centrality::my_insertionSort(centrality_list, neighbor_list, 'd');
    // the heuristic main loop
    while (!(QUEUE1.empty())){
        root = QUEUE1.front();
        QUEUE1.pop();

        //int num_neighbor = graph.adjList(root).size();
        //for (int i = 0; i < num_neighbor; ++i){
        //    int neighbor = graph.adjList(root)[i];
        //    if (tree.grau(neighbor) == 0) {
        //        tree.add_aresta(root, neighbor);
        //        neighbor_list.push_back(neighbor); // newline
        //        centrality_list.push_back(graph.grau(neighbor));
        //    }
        //}
        for (auto vertex: graph.adjList(root)){
            if (tree.grau(vertex) == 0) {
                tree.add_aresta(root, vertex);
                neighbor_list.push_back(vertex); // newline
                //degree_list.push_back(graph.grau(vertex));
                closeness_list.push_back(vertices_closeness[vertex]);
            }
        }

        Centrality::my_insertionSort(closeness_list, neighbor_list, 'd');

        //for (int i = 0; i < neighbor_list.size(); ++i){ // Fixed core dumped BUG - change num_neighbor for neighbor_list.size()
        //    if (!in( neighbor_list[i], enqueued)){
        //        QUEUE1.push(neighbor_list[i]);
        //        enqueued.push_back(neighbor_list[i]);
        //    }
        //}
        for (auto vertex : neighbor_list){
            if (!visited[vertex]){
                QUEUE1.push(vertex);
                visited[vertex]=true;
            }
        }
        neighbor_list.clear();
        closeness_list.clear();
    }

    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
}

/**
 * @brief T-admissibility breadth heuristic - Heuristic 4v2_2.
 * @details The breadth heuristic - heuristic 4v2_2
 * @author Carlos Thadeu
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::Heuristica_4v2r2(Graph &graph)
{
    Stretch stretch;
    //int counter = 0;
    int root = 0;
    int n = graph.getQtdVertices();
    Graph tree(n);

    std::queue <int> QUEUE1;
    std::vector <int> neighbor_list; // newline
    std::vector <float> closeness_list; // newline
    //std::vector <int> enqueued; // processed vertices that was enqueued anytime
    std::vector <bool> visited(n, false); // processed vertices that was enqueued anytime
    //std::vector<std::pair<int,float>> vertices_closeness = Centrality::closeness_centrality_list(graph);
    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    //std::vector<float> vertices_closeness = Centrality::closeness_centrality_vector(graph);

    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    //std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);

    DEBUG std::cerr << "Selecting root" << std::endl;
    root = Centrality::root_selection2(vertices_closeness);
    //root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    DEBUG std::cerr << "Selected root: " << root << std::endl;
    QUEUE1.push(root);
    //enqueued.push_back(root);
    visited[root]=true;

    DEBUG std::cerr << "Starting Heuristica_4v2r2 - - Closeness centrality - calculated" << root << std::endl;

    //Centrality::my_insertionSort(centrality_list, neighbor_list, 'd');
    // the heuristic main loop
    while (!(QUEUE1.empty())){
        root = QUEUE1.front();
        QUEUE1.pop();

        //int num_neighbor = graph.adjList(root).size();
        //for (int i = 0; i < num_neighbor; ++i){
        //    int neighbor = graph.adjList(root)[i];
        //    if (tree.grau(neighbor) == 0) {
        //        tree.add_aresta(root, neighbor);
        //        neighbor_list.push_back(neighbor); // newline
        //        centrality_list.push_back(graph.grau(neighbor));
        //    }
        //}
        for (auto vertex: graph.adjList(root)){
            if (tree.grau(vertex) == 0) {
                tree.add_aresta(root, vertex);
                neighbor_list.push_back(vertex); // newline
                //degree_list.push_back(graph.grau(vertex));
                closeness_list.push_back(vertices_closeness[vertex]);
            }
        }

        Centrality::my_insertionSort(closeness_list, neighbor_list, 'd');

        //for (int i = 0; i < neighbor_list.size(); ++i){ // Fixed core dumped BUG - change num_neighbor for neighbor_list.size()
        //    if (!in( neighbor_list[i], enqueued)){
        //        QUEUE1.push(neighbor_list[i]);
        //        enqueued.push_back(neighbor_list[i]);
        //    }
        //}
        for (auto vertex : neighbor_list){
            if (!visited[vertex]){
                QUEUE1.push(vertex);
                visited[vertex]=true;
            }
        }
        neighbor_list.clear();
        closeness_list.clear();
    }

    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
}

/**
 * @brief T-admissibility breadth heuristic - Heuristic 4v2r3.
 * @details The breadth heuristic - heuristic 4v2r3
 * @author Carlos Thadeu
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::Heuristica_4v2r3(Graph &graph)
{
    Stretch stretch;
    //int counter = 0;
    int root = 0;
    int n = graph.getQtdVertices();
    Graph tree(n);

    std::queue <int> QUEUE1;
    std::vector <int> neighbor_list; // newline
    std::vector <float> closeness_list; // newline
    std::vector <float> leverage_list; // newline
    //std::vector <int> enqueued; // processed vertices that was enqueued anytime
    std::vector<bool> visited(n, 0);
    //std::vector<std::pair<int,float>> vertices_closeness = Centrality::closeness_centrality_list(graph);
    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    //std::vector<float> vertices_closeness = Centrality::closeness_centrality_vector(graph);

    std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);

    DEBUG std::cerr << "Selecting root" << std::endl;
    //root = Centrality::root_selection2(vertices_closeness);
    root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    DEBUG std::cerr << "Selected root: " << root << std::endl;
    QUEUE1.push(root);
    //enqueued.push_back(root);
    visited[root]=true;

    DEBUG std::cerr << "Starting Heuristica_4v2r3 - - Closeness centrality using leverage centrality - calculated" << root << std::endl;

    //Centrality::my_insertionSort(centrality_list, neighbor_list, 'd');
    // the heuristic main loop
    while (!(QUEUE1.empty())){
        root = QUEUE1.front();
        QUEUE1.pop();

        //int num_neighbor = graph.adjList(root).size();
        //for (int i = 0; i < num_neighbor; ++i){
        //    int neighbor = graph.adjList(root)[i];
        //    if (tree.grau(neighbor) == 0) {
        //        tree.add_aresta(root, neighbor);
        //        neighbor_list.push_back(neighbor); // newline
        //        centrality_list.push_back(graph.grau(neighbor));
        //    }
        //}
        for (auto vertex: graph.adjList(root)){
            if (tree.grau(vertex) == 0) {
                tree.add_aresta(root, vertex);
                neighbor_list.push_back(vertex); // newline
                closeness_list.push_back(vertices_closeness[vertex]);
                leverage_list.push_back(vertices_leverage[vertex]);
            }
        }

        //Centrality::my_insertionSort(centrality_list, neighbor_list, 'd');
        Centrality::my_insertionSort(neighbor_list, closeness_list, leverage_list, graph);

        //for (int i = 0; i < neighbor_list.size(); ++i){ // Fixed core dumped BUG - change num_neighbor for neighbor_list.size()
        //    if (!in( neighbor_list[i], enqueued)){
        //        QUEUE1.push(neighbor_list[i]);
        //        enqueued.push_back(neighbor_list[i]);
        //    }
        //}
        for (auto vertex : neighbor_list){
            if (!visited[vertex]){
                QUEUE1.push(vertex);
                visited[vertex]=true;
            }
        }
        neighbor_list.clear();
        closeness_list.clear();
        leverage_list.clear();
    }

    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
}

/**
 * @brief T-admissibility breadth heuristic - Heuristic 4v2.
 * @details The breadth heuristic - heuristic 4v2
 * @author Carlos Thadeu
 * @param graph a graph instance that represents the graph.
 */
void Heuristic::Heuristica_4v2(Graph &graph)
{
    Stretch stretch;
    //int counter = 0;
    int root = 0;
    int n = graph.getQtdVertices();
    Graph tree(n);
    
    std::queue <int> QUEUE1;
    std::vector <int> neighbor_list; // newline
    std::vector <float> closeness_list; // newline
    //std::vector <int> enqueued; // processed vertices that was enqueued anytime
    std::vector <bool> visited(n, false); // processed vertices that was enqueued anytime

    DEBUG std::cerr << "Calculating vertex importance!" << std::endl;
    //std::vector<float> vertices_closeness = Centrality::closeness_centrality_thread(graph);
    std::vector<float> vertices_closeness;
    if (global_closeness == 2) {
        vertices_closeness = Centrality::closeness_centrality_thread(graph);
    } else {
        vertices_closeness = Centrality::closeness_centrality_thread_V2(graph);
    }
    std::vector<float> vertices_leverage = Centrality::leverage_centrality_thread(graph);
    DEBUG std::cerr << "Selecting root" << std::endl;
    root = Centrality::root_selection3(vertices_closeness, vertices_leverage);
    DEBUG std::cerr << "Selected root: " << root << std::endl;

    QUEUE1.push(root);
    //enqueued.push_back(root);
    visited[root]=true;

    DEBUG std::cerr << "Starting Heuristica_4v2 "<< std::endl;

    //Centrality::my_insertionSort(centrality_list, neighbor_list, 'd');
    // the heuristic main loop 
    while (!(QUEUE1.empty())){
        root = QUEUE1.front();
        QUEUE1.pop();

        //int num_neighbor = graph.adjList(root).size();
        //for (int i = 0; i < num_neighbor; ++i){
        //    int neighbor = graph.adjList(root)[i];
        //    if (tree.grau(neighbor) == 0) {
        //        tree.add_aresta(root, neighbor);
        //        neighbor_list.push_back(neighbor); // newline
        //        centrality_list.push_back(graph.grau(neighbor));
        //    }
        //}
        for (auto vertex: graph.adjList(root)){
            if (tree.grau(vertex) == 0) {
                tree.add_aresta(root, vertex);
                neighbor_list.push_back(vertex); // newline
                //degree_list.push_back(graph.grau(vertex));
                closeness_list.push_back(vertices_closeness[vertex]);
            }
        }

        Centrality::my_insertionSort(closeness_list, neighbor_list, 'd');

        //for (int i = 0; i < neighbor_list.size(); ++i){ // Fixed core dumped BUG - change num_neighbor for neighbor_list.size()
        //    if (!in( neighbor_list[i], enqueued)){
        //        QUEUE1.push(neighbor_list[i]);
        //        enqueued.push_back(neighbor_list[i]);
        //    }
        //}
        for (auto vertex : neighbor_list){
            if (!visited[vertex]){
                QUEUE1.push(vertex);
                visited[vertex]=true;
            }
        }   
        neighbor_list.clear();
        closeness_list.clear();
    }

    int factor = stretch.find_factor(graph, tree);
    graph.sum_trees(1);
    set_graph_final_parameters(factor, tree, graph);
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
    //int a=graph.grau(vertex) - tree.grau(vertex);
    //if (a==0)
    //    a=graph.grau(vertex);
    //return a;
    int count = 0;
    for(int u : graph.adjList(vertex) ){
        if( tree.grau(u) != 0){
            ++count;
        }
    }
    return count;


}


// Existe um bug na my_sort original, o primeiro vértice não é ordenado
// TO DO: Proceder com este reparo
// Nas heuristicas 1 e 2 v1 que utilizam este método
// ele foi substituido temporariamente por my_insertSort
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
// void Heuristic::set_graph_final_parameters(int &index_local, Graph &tree_local, Graph &graph){
//     if(index_local < graph.get_stretch_index() && index_local != 1) {
//         graph.set_stretch_index(index_local);
//         graph.set_best_tree(tree_local);
//         if (index_local==graph.grt - 1){
//             graph.set_signal();
//             //processando.store(false,std::memory_order_release);
//         }
//     }
// }

/**
 * @brief Auxiliary function to count all occurrences of the edges of the trees created
 * @details Return a list of edges and their occurrences numbers
 * @author Eriky Marciano
 */

