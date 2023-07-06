#include "graph.hpp"
#include "opBasic.hpp"
#include <iostream>
#include <tuple>
#include <bits/stdc++.h>
#include <iterator>

#include "../Debug.h"
#include "../my_libs/ctfunctions2.hpp"

//! Constructor
/*!
    Build a graph with n vertices and no edges
    \param n a integer that represents the vertex number to add
*/
Graph::Graph(int n)
{
    // graph = new std::vector<std::vector<int> >(n, std::vector<int>());
    qtdVertices = n;
    qtdArestas = 0;
    for (int i = 0; i < n; ++i)
    {
        graph.push_back(std::vector<int>());
    }
}

//! Constructor
/*!
    Build a graph without vertices or edges
*/
Graph::Graph()
{
    // graph = new std::vector<std::vector<int> >(n, std::vector<int>());
    qtdVertices = 0;
    qtdArestas = 0;
}

//! Destructor
/*!
    Write a detailed description
*/
Graph::~Graph()
{
    // delete graph;
}

//! Add vertex
/*!
    Add new vertex to graph
*/
void Graph::add_vertex()
{
    ++qtdVertices;
    graph.push_back(std::vector<int>());
}

//! Add new edge
/*!
    Add new edge to graph
    \param v a integer that represents the vertex
    \param u a integer that represents the neighborhood
*/
void Graph::add_aresta(int v, int u)
{
    std::vector<int>::iterator it;

    it = graph[v].begin();
    while (it != graph[v].end() && *it < u)
    {
        ++it;
    }
    graph[v].insert(it, u);

    it = graph[u].begin();
    while (it != graph[u].end() && *it < v)
    {
        ++it;
    }
    graph[u].insert(it, v);

    // graph[v].push_back(u);
    // graph[u].push_back(v);
    ++qtdArestas;
}

//! Add new edge(s)
/*!
    Add new edge(s) to graph from a vector
    \param arestas a vector
*/
void Graph::add_aresta(std::vector<int> arestas)
{
    for (std::vector<int>::iterator it = arestas.begin(); it != arestas.end(); it += 2)
    {
        add_aresta(*it, *(it + 1));
    }
}

/**
 * Delete a vertex from graph METHOD SPECIFIC FOR ARTICULATIONS - NOT REMOVE THE VERTEX FROM THE DATA STRUCTURE)
 * @details Delete a vertex from graph and your edges
 * @author Carlos Thadeu
 * @param vertex an integer that represents the vertex to be deleted
 */
/*void Graph::delete_vertex(int vertex)
{
    std::vector<int>::iterator it;
    std::vector<int> neighbors;
    neighbors = adjList(vertex);

    // otimizar usando iterator
    for (int i = 0; i < neighbors.size(); i++)
    {
        remove_aresta(vertex, neighbors[i]);
    }
}*/

//! Delete an edge
/*!
    Delete an edge from graph
    \param v a integer that represents the vertex
    \param u a integer that represents the neighbor
*/
void Graph::remove_aresta(int v, int u)
{
    std::vector<int>::iterator it;

    it = graph[v].begin();
    while (it != graph[v].end() && *it != u)
    {
        ++it;
    }
    graph[v].erase(it);

    it = graph[u].begin();
    while (it != graph[u].end() && *it != v)
    {
        ++it;
    }
    graph[u].erase(it);

    --qtdArestas;
}

//! Add all new vertices
/*!
    Add new vertices
    \param n a integer that represents the numbers of vertices of graph
*/
void Graph::add_all_vertice(int n)
{
    for (int i = 0; i < n; ++i)
    {
        graph.push_back(std::vector<int>());
    }
    qtdVertices = n;
}

//! Return the adjacency list from a vertex
/*!
    Return the adjacency list from a vertex
    \param v a integer that represents the vertex number
    \return a vector of integer that represents an adjacency list vector
*/
const std::vector<int> Graph::adjList(int v)
{
    return graph[v];
}

//! Check if edge exist
/*! DEPRECATED WILL BE REMOVED AT FUTURE
    REPLACED BY METHOD has_edge(int v, int u)
    Seek at vertex's adjacency list if edge exist
    \param v a integer that represents the vertex
    \param u a integer that represents the neighborhood
    \return a boolean
*/
bool Graph::possui_aresta(int v, int u)
{
    for (int x : adjList(v))
    {
        if (x == u)
        {
            for (int y : adjList(u))
            {
                if (y == v)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

//! Check if edge exist
/*!
    Seek at vertex's adjacency list if edge exist
    \param v a integer that represents the vertex
    \param u a integer that represents the neighborhood
    \return a boolean
*/
bool Graph::has_edge(int v, int u)
{
    for (int x : adjList(v))
    {
        if (x == u)
        {
            for (int y : adjList(u))
            {
                if (y == v)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

//! Return a vertex degree
/*!
    Return the degree's vertex
    \param v a integer that represents the vertex
    \return a integer
*/
int Graph::grau(int v)
{
    return graph[v].size();
}

//! Delete all edges
/*!
    Destroy the list of adjacencies of all vertices
*/
void Graph::clear_arestas()
{
    for (int i = 0; i < getQtdVertices(); ++i)
    {
        graph[i].clear();
    }
    qtdArestas = 0;
}

//! Clear edges
/*!
    Delete edges of all vertices
*/
void Graph::clear()
{
    clear_arestas();
    this->graph.clear();
    qtdVertices = 0;
}

//! Go to next vertex
/*!
    Go to next vertex
    \param v a integer that represent a actual vertex
    \return a integer
*/
int Graph::next_vertex(int v)
{
    ++v;
    return v == getQtdVertices() ? 0 : v;
}

//! Go to previous vertex
/*!
    Go to previous vertex
    \param v a integer that represent a actual vertex
    \return a integer
*/
int Graph::ant_vertex(int v)
{
    --v;
    return v < 0 ? getQtdVertices() - 1 : v;
}

//! An edge list
/*!
    Return an edge list
    \return a vector of integer
*/
const std::vector<int> Graph::edgeList()
{
    std::vector<int> aux;
    for (int v = 0; v < getQtdVertices(); ++v)
    {
        for (int u : adjList(v))
        {
            if (u > v)
            {
                aux.push_back(v);
                aux.push_back(u);
            }
        }
    }
    return aux;
}

//! Get max degree
/*!
    Seek for max degree
    \return a integer
*/
int Graph::maior_grau()
{
    int maior = grau(0);
    for (int i = 1; i < getQtdVertices(); ++i)
    {
        int teste = grau(i);
        if (teste > maior)
        {
            maior = teste;
        }
    }
    return maior;
}

//! Get vertex with max degree
/*!
    Seek for vertex with max degree
    \return a integer
*/
int Graph::highest_degree_vertex()
{
    int maior = grau(0);
    int v = 0;
    for (int i = 1; i < getQtdVertices(); ++i)
    {
        int teste = grau(i);
        if (teste > maior)
        {
            maior = teste;
            v = i;
        }
    }
    return v;
}

//! List all vertices with max degree
/*!
    Seek for all vertices with max degree
    \return a vector
*/
std::vector<int> Graph::vertices_de_maior_grau()
{
    std::vector<int> list_vertices;
    int maior_grau = this->maior_grau();

    for (int i = 0; i < getQtdVertices(); ++i)
    {
        if (grau(i) == maior_grau)
        {
            list_vertices.push_back(i);
        }
    }

    return list_vertices;
}

//! Count the number of vertices that has a specific degree
/*!
    Count the number of vertices that has a specific degree
    \param grau is an integer that represents the degree seek
    \return an integer that represents the quantity of vertices that has a specific degree
*/
int Graph::qtd_vertex_grau(int grau)
{
    int count = 0;
    for (int i = 0; i < getQtdVertices(); ++i)
    {
        if (this->grau(i) == grau)
        {
            ++count;
        }
    }
    return count;
}

/*
    Methods created by Carlos Thadeu
*/

int Graph::get_num_edges()
{
    return qtdArestas;
};

//! Set a best tree found
/*!
    After found a best tree , assing it to graph
    \param n a graph that represents a local tree from a thread
*/
void Graph::set_best_tree(Graph graph)
{
    this->best_tree = {};
    for (int i = 0; i < graph.edgeList().size(); i = i + 2) {
        this->best_tree.push_back(std::make_tuple(graph.edgeList()[i], graph.edgeList()[i + 1]));
    }
};

//! Get a best tree found
/*!
    Return a best tree found
    \return a vector of tuples that represents edges from best tree
*/
std::vector<std::tuple<int, int>> Graph::get_best_tree()
{
    return this->best_tree;
}

//! Show at screen a best tree found
/*!
    Show at screen a best tree found
*/
void Graph::show_best_tree()
{
    int node1 = 0, node2 = 0;
    // std::cerr << "Stretch index : " << this->get_stretch_index() << "\n" << std::endl;
    for (auto &&tuple : this->best_tree)
    {
        std::tie(node1, node2) = tuple;
        std::cout << "(" << node1 << " , " << node2 << ") ";
    }
    std::cout << std::endl;
};


//! Assign value to stretch index
/*!
    Assign value to stretch index
    \param n a integer that represents the stretch index
*/
void Graph::set_stretch_index(int value)
{
    this->stretch_index = value;
}

//! Get value to stretch index
/*!
    Get value to stretch index
    \return an integer that represents stretch index
*/
int Graph::get_stretch_index()
{
    return this->stretch_index;
}

//! Add new vertices to graph create without vertices Graph()
/*!
    Add new vertices to graph create without vertices Graph()
    \param n a integer that represents the number of vertices of graph
*/
void Graph::add_vertices(int n)
{
    for (int i = 0; i < n; ++i)
    {
        this->add_vertex();
    };
};

std::vector<int> Graph::induced_cycle(Graph &g, int n)
{
    OpBasic a = OpBasic();
    return a.cycle(g, n);
}

bool Graph::has_chord(Graph &g, std::vector<int> x)
{
    OpBasic a = OpBasic();
    return a.possuiCorda(g, x);
}

/// @brief DEPRECATED WILL BE REMOVED AT FUTURE - REPLACED BY sum_tree()
void Graph::add_tree()
{
    this->total_tree++;
}


void Graph::sum_trees()
{
    this->total_tree++;
}

void Graph::sum_trees(int value)
{
    this->total_tree = this->total_tree + value;
}

int Graph::get_total_tree()
{
    return this->total_tree;
}

void Graph::reset_trees()
{
    this->total_tree = 0;
}

void Graph::reset_trees(int value)
{
    this->total_tree = value;
}

/// @brief DEPRECATED WILL BE REMOVE AT FUTURE - REPLACE BY get_num_vertices
int Graph::get_qty_vertex()
{
    return this->qtdVertices;
}

int Graph::get_num_vertices()
{
    return this->qtdVertices;
}

// Get the index from vertex
int Graph::neighbor_index(int vertex, int neighbor)
{
    int index = -1;
    int size = this->adjList(vertex).size();
    for (int i = 0; i < size; ++i)
    {
        if (this->adjList(vertex)[i] == neighbor)
        {
            index = i;
            break;
        }
    }
    return index;
}

int Graph::eccentricity(Graph &graph, int vertex)
{
    return OpBasic().eccentricity(graph, vertex);
}

/**
 * @brief Deep first search
 * @details Deep first search using stack
 * Returns all reachable verttices from initial vertex
 * adapter from https://www.geeksforgeeks.org/iterative-depth-first-traversal/
 * @author Carlos Thadeu
 * @param vertex an integer that represents a vertex
 * @return a vector with all vertices reachable
 */
std::vector<int> Graph::DFS(int vertex)
{
    // Initially mark all vertices as not visited
    // std::vector<bool> visited(V, false);
    std::vector<bool> visited(get_qty_vertex(), false);
    std::vector<int> vertices_dfs;

    // Create a stack for DFS
    std::stack<int> stack;

    // Push the current source node.
    stack.push(vertex);

    while (!stack.empty())
    {
        // Pop a vertex from stack and print it
        int vertex = stack.top();
        stack.pop();

        // Stack may contain same vertex twice. So
        // we need to print the popped item only
        // if it is not visited.
        if (!visited[vertex])
        {
            vertices_dfs.push_back(vertex);
            visited[vertex] = true;
        }

        // Get all adjacent vertices of the popped vertex s
        // If a adjacent has not been visited, then push it
        // to the stack.
        // for (auto i = adj[s].begin(); i != adj[s].end(); ++i)
        // for (auto i = adjList(s).begin(); i != adjList(s).end(); ++i)
        for (int i = 0; i < adjList(vertex).size(); i++)
        {
            if (!visited[adjList(vertex)[i]])
                stack.push(adjList(vertex)[i]);
        }
    }
    return vertices_dfs;
}

/**
 * @brief Find subgraphs from an articulations at the original graph
 * @details Seek subgraphs from an articulations the original graph
 * @author Carlos Thadeu
 * @param articulations a set that represents the articulations of graph
 * @param subgraph a vector of vectors that represents the subgraphs
 * @param g a graph that represents the original graph
 */
/*void Graph::split_in_subgraphs(std::set<int> articulations, std::vector<std::vector<int>> &subgraph, Graph &g)
{
    Graph graph;
    // transform set at vector
    graph = g;
    std::vector<int> list_articulations(articulations.begin(), articulations.end());
    // std::vector<std::vector<int>> neighbors;
    std::set<int> visited;
    for (auto root : list_articulations)
    {
        // neighbors.push_back(this->adjList(root));
        graph.delete_vertex(root);
    }
    for (auto root : list_articulations)
    {

        // for (int i=0; i < neighbors.size(); i++){
        for (auto neighbor : g.adjList(root))
        {
            // if (!in(visited, neighbors[i])){
            if (!in(neighbor, visited) && (!in(neighbor, list_articulations)))
            {
                // subgraph.push_back(this->DFS(neighbors[i])); //add vertices
                subgraph.push_back(graph.DFS(neighbor)); // add vertices
                // subgraph[subgraph.size()-1].push_back(root); // add root vertex
                for (const auto &t : subgraph.back())
                {                      // reference avoids copying element
                    visited.insert(t); // element can not be changed
                }
                // if (subgraph[subgraph.size()-1].size() < 3){ // Usa isto Se usar subgraph[subgraph.size()-1].push_back(root);
                if (subgraph[subgraph.size() - 1].size() < 2)
                {
                    for (auto i : subgraph[subgraph.size() - 1])
                    {
                        visited.erase(i);
                    }
                    subgraph.erase(subgraph.begin() + subgraph.size() - 1);
                }
            }
        }
    }
    int count = 0;
    for (auto sb : subgraph)
    {
        for (auto v : sb)
        {
            for (auto neighbor : g.adjList(v))
            {
                if (in(neighbor, list_articulations) && (!in(neighbor, subgraph[count])))
                {
                    subgraph[count].push_back(neighbor);
                }
            }
        }
        count++;
    }
}*/

/*Graph Graph::build_subgraph(std::vector<int> &subgraph)
{
    Graph new_graph(subgraph.size());

    std::vector<int> adjacentes;
    int index = 0;
    for (auto v1 : subgraph)
    {
        int e1 = get_index(v1, subgraph);
        int e2;
        adjacentes = this->adjList(v1);
        for (auto v2 : adjacentes)
        {
            if (in(v2, subgraph) && v2 != v1)
            {
                e2 = get_index(v2, subgraph);
                if ((!in(e2, new_graph.adjList(e1))) && (!in(e1, new_graph.adjList(e2))))
                    new_graph.add_aresta(e1, e2);
            }
        }
    }
    return new_graph;
}*/

/**
 * @brief Return signal if lower limit was reached
 * @details Return signal if lower limit was reached when the original graph was splitted
 * Lower limit not reached, signal is true.
 * Lower limit reached, signal is false.
 * @author Carlos Thadeu
 */
bool Graph::get_signal()
{
    return this->signal;
}

/**
 * @brief Set signal to false
 * @details If the lower limit was reached, the signal is setting to false
 * The signal at original graph is useful when the original graph was split in many parts
 * and there is a need to know if lower limit was reached
 * Not reached, signal is true. Reached, signal is false.
 * @author Carlos Thadeu
 */
void Graph::set_signal()
{
    this->signal = false;
}

/**
 * @brief Modify insert sort
 * @details Details decription
 * That's a modify code to sort two vectors at ascendind or descending.
 * The sorting will be controlled by the first vector (it will be mandatory)
 * The second vector will be sort following the first vector positions
 * Use 'a' for ascending or 'd' for descending
 * Example: v1={4,3,6,2} v2={1,2,3,4} ascending
 * Result: v1={2,3,4,6} v2={4,2,1,3}
 * @date 2022/12/17
 * @author Original code rathbhupendra
 * @author Modify code cthadeusantos
 * @param vector1   That's an integer vector that will be sort
 * @param vector2   That's an integer vector that will be sort following vector1 order
 * @param order (optional) a - ascending d - descending
 */
void Graph::my_insertionSort_graph(std::vector<int> &vector1, std::vector<int> &vector2, char order = 'a')
{
    int n = vector1.size();
    int i, key1, key2, j;

    for (i = 1; i < n; i++)
    {
        key1 = vector1[i];
        key2 = vector2[i];
        j = i - 1;

        // Move elements of arr[0..i-1],
        // that are greater than key, to one
        // position ahead of their
        // current position
        if (order == 'a')
        {
            while (j >= 0 && key1 < vector1[j])
            {
                vector1[j + 1] = vector1[j];
                vector2[j + 1] = vector2[j];
                j = j - 1;
            }
        }
        else if (order == 'd')
        {
            while (j >= 0 && key1 > vector1[j])
            {
                vector1[j + 1] = vector1[j];
                vector2[j + 1] = vector2[j];
                j = j - 1;
            }
        }

        vector1[j + 1] = key1;
        vector2[j + 1] = key2;
    }
}

/**
 * @brief Modify insert sort
 * @details Details decription
 * That's a modify code to sort two vectors at ascendind or descending.
 * The sorting will be controlled by the first vector (it will be mandatory)
 * The second vector will be sort following the first vector positions
 * Use 'a' for ascending or 'd' for descending
 * Example: v1={4,3,6,2} v2={1,2,3,4} ascending
 * Result: v1={2,3,4,6} v2={4,2,1,3}
 * @date 2022/12/17
 * @author Original code rathbhupendra
 * @author Modify code cthadeusantos
 * @param vector1   That's an integer vector that will be sort
 * @param vector2   That's an integer vector that will be sort following vector1 order
 * @param order (optional) a - ascending d - descending
 */
void Graph::my_insertionSort_graph(std::vector<float> &vector1, std::vector<int> &vector2, char order = 'a')
{
    int n = vector1.size();
    int i, key2, j;
    float key1;

    for (i = 1; i < n; i++)
    {
        key1 = vector1[i];
        key2 = vector2[i];
        j = i - 1;

        // Move elements of arr[0..i-1],
        // that are greater than key, to one
        // position ahead of their
        // current position
        if (order == 'a')
        {
            while (j >= 0 && key1 < vector1[j])
            {
                vector1[j + 1] = vector1[j];
                vector2[j + 1] = vector2[j];
                j = j - 1;
            }
        }
        else if (order == 'd')
        {
            while (j >= 0 && key1 > vector1[j])
            {
                vector1[j + 1] = vector1[j];
                vector2[j + 1] = vector2[j];
                j = j - 1;
            }
        }

        vector1[j + 1] = key1;
        vector2[j + 1] = key2;
    }
}

std::vector<int> Graph::select_max_degree_vertices_at_list(int max, std::vector<int> &vector1, std::vector<int> &vector2)
{
    std::vector<int> output;
    for (int i = 0; i < vector1.size(); i++)
    {
        if (vector2[i] == max)
            output.push_back(vector1[i]);
    }
    return output;
}

std::vector<int> Graph::my_connected_vertices()
{
    std::vector<int> auxiliary;
    for (int i = 0; i < this->graph.size(); i++)
    {
        if (this->grau(i) > 0)
            auxiliary.push_back(i);
    }
    return auxiliary;
}

// BFS algorithm
//https://www.programiz.com/dsa/graph-bfs
std::vector<int> Graph::BFS(int vertex) {
    // Initially mark all vertices as not visited
    // std::vector<bool> visited(V, false);
    std::vector<bool> visited(get_qty_vertex(), false);
    std::vector<int> vertices_bfs;
  
    // Create a queue for BFS
    std::queue<int> queue;

    queue.push(vertex);

    while (!queue.empty())
    {
        // Pop a vertex from stack and print it
        int vertex = queue.front();
        queue.pop();

        // Queue may contain same vertex twice. So
        // we need to print the popped item only
        // if it is not visited.
        if (!visited[vertex])
        {
            vertices_bfs.push_back(vertex);
            visited[vertex] = true;
        }

        // Get all adjacent vertices of the popped vertex s
        // If a adjacent has not been visited, then push it
        // to the queue.
        // for (auto i = adj[s].begin(); i != adj[s].end(); ++i)
        // for (auto i = adjList(s).begin(); i != adjList(s).end(); ++i)
        for (int i = 0; i < adjList(vertex).size(); i++)
        {
            if (!visited[adjList(vertex)[i]])
                queue.push(adjList(vertex)[i]);
        }
    }
    return vertices_bfs;
}




void Graph::set_girth(int value){
    this->girth = value;
}

int Graph::get_girth(){
    return this->girth;
}

std::vector<std::pair<int, int>> Graph::get_edges_set(){
    std::vector<std::pair<int, int>> edges_list;
    std::vector<std::vector<int>> vector = this->graph;
    int vertex = 0;
    int *ptr = &vertex;
    std::pair<int,int> edge;
    //I am using iterators , if you read the code and not understand
    for (auto it = vector.begin(); it != vector.cend(); ++it){
        for (auto itt = it->begin(); itt != it->cend(); ++itt){
            if (*itt > *ptr){
                edge = std::make_pair(*itt, *ptr);
                edges_list.push_back(edge);
                //DEBUG std::cerr << "("<< *ptr << ", " << *itt << ")" ;
            }
        }
        *ptr = *ptr + 1;
    }
    return edges_list;
}

std::vector<int> Graph::get_neighbors(int vertex){
    return this->graph[vertex];
}

/**
 * @brief Calculate the girth of graph
 * @details Calculate the girth of graph
 * That's an alternative to compute the girth developed by DJ at OpBasic class
 * @date 2023/04/17
 * @author cthadeusantos
 * @param adjacency That's a vector of vector (an adjacencies list)
 * @return min_waist That's an integer
 */
/*int Graph::waist(std::vector<std::vector<int>> adjacency) {
    int n = adjacency.size();
    int min_waist = n;
    for (int i = 0; i < n; i++) {
        std::vector<int> distances(n, -1);
        distances[i] = 0;
        std::queue<int> q;
        q.push(i);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adjacency[u]) {
                if (distances[v] == -1) {
                    distances[v] = distances[u] + 1;
                    q.push(v);
                } else if (v != i && v != u) {
                    min_waist = std::min(min_waist, distances[u] + distances[v] + 1);
                }
            }
        }
    }
    return min_waist;
}*/

/**
 * @brief Calculate the girth of graph
 * @details Calculate the girth of graph
 * That's an alternative to compute the girth developed by DJ at OpBasic class
 * @date 2023/04/17
 * @author cthadeusantos
 * @return min_waist That's an integer
 */
/*int Graph::waist() {
    std::vector<std::vector<int>> adjacency = this->get_neighbors();
    int n = adjacency.size();
    int min_waist = n;
    for (int i = 0; i < n; i++) {
        std::vector<int> distances(n, -1);
        distances[i] = 0;
        std::queue<int> q;
        q.push(i);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adjacency[u]) {
                if (distances[v] == -1) {
                    distances[v] = distances[u] + 1;
                    q.push(v);
                } else if (v != i && v != u) {
                    min_waist = std::min(min_waist, distances[u] + distances[v] + 1);
                }
            }
        }
    }
    return min_waist;
} */

std::vector<std::vector<int>> Graph::get_neighbors(){
    return this->graph;
}

void Graph::show_edges()
{
    // std::cerr << "Stretch index : " << this->get_stretch_index() << "\n" << std::endl;
    for (int i=0; i < this->get_qty_vertex(); i++){
        for (auto neighbor : this->get_neighbors(i)){
            DEBUG std::cerr << "(" << i << " , " << neighbor << ") ";
        }
    }
};

void Graph::check_integrity(){
    //Check integrity of graph 
    for (int i=0; i < this->getQtdVertices();i++){
        if (this->get_neighbors(i).size() < 1){
            DEBUG std::cerr << "INTEGRIDADE COMPROMETIDA" << std::endl;
        }
    }
}

void Graph::set_grt(Graph graph){
    this->grt = OpBasic::maxLowerCicle(graph);
}

int Graph::get_grt(){
    return this->grt;
}

void Graph::set_lower_limit(int value){
    if (value <=0) value = 1;
    this->lower_limit = value;
}

void Graph::set_lower_limit(Graph graph){
    this->lower_limit = OpBasic::maxLowerCicle(graph) - 1;
    if (this->lower_limit <=0) this->lower_limit = 1;
}

int Graph::get_lower_limit(){
    return this->lower_limit;
}