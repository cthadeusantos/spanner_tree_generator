#include "centrality.hpp"
#include "heuristic.hpp"

/**
 * Select a vertex to be root
 * @details Selects a vertex to be root using closeness centrality from all graph's vertices
 * @author Carlos Thadeu
 * @param g a graph
 */
int Centrality::root_selection2(Graph &g){    // By thadeu
    //std::queue <int>FILA;   // contains the vertices with the highest degree (same degree)

    float importance = 0;
    int choice = -1;
    for (auto vertex: g.vertices_de_maior_grau()){
        float value = vertex_importance( vertex, g );
        if (value > importance) {
            importance = value;
            choice = vertex;
        }
    }
    return choice;
}

/**
 * Select a vertex to be root
 * @details Select a vertex to be root using closeness centrality 
 * @author Carlos Thadeu
 * @param vector1 a vector of pair <int, float> that represents a vertex and your centrality
 * @return selected an integer that represents the vertex with max centrality 
 */
int Centrality::root_selection2(std::vector<std::pair<int,float>> vector1){    // By thadeu

    float max = 0;
    float centrality=0;
    int vertex = 0;
    int selected = 0;

    for (int i=0; i < vector1.size(); i++){
        std::tie(vertex, centrality) = vector1[i];
        if (centrality > max) {
            max = centrality;
            selected = vertex;
        }
    }
    return selected;
}

/**
 * Select a vertex to be root
 * @details Select a vertex to be root using closeness centrality.
 * @details That's a first version ... DEPRECATED
 * @author Carlos Thadeu
 * @param vector1 a vector of pair <int, float> that represents a vertex and your centrality
 * @return selected an integer that represents the vertex with max centrality 
 */
int Centrality::root_selection(Graph &g){    // By thadeu
    int index = 0;
    int n = g.get_qty_vertex();
    std::vector<int> vertex_list(n);
    std::queue <int>FILA;   // contains the vertices with the highest degree (same degree)
    std::vector <std::vector<int>> sum_layer;    // contains the sum of vertices degree of layer

    for( int i = 0; i < n; ++i) vertex_list[i] = i;
    Heuristic::my_quicksort(vertex_list, 0, n, g);

    for (auto vertex: g.vertices_de_maior_grau()){
        FILA.push(vertex);
    }
    
    int min_diameter = INF_VALUE;
    std::vector <int> nominees;

    while (!FILA.empty()){
        std::vector <int>visited;   // Visited vertices
        std::vector <int>total_layer;   // Sum total of the layer
        std::queue <int>FILA3;
        int y = FILA.front();
        FILA3.push(y);
        FILA.pop();
        nominees.push_back(y);

        sum_layer.push_back(Heuristic::breadth_criterion(g, FILA3, visited, total_layer));
        FILA3.pop();

    }
    
    for (int col=0; col < sum_layer[0].size(); col++){
        int max =0;
        for (int row=0; row < sum_layer.size() - 1; row++){
            //if (sum_layer[j][index] > sum_layer[j+1][index] and diameters[j*2+1] <= min_diameter){
            if (sum_layer[row][col] > sum_layer[row+1][col] and sum_layer[row][col] > max){
                index = row;
                max = sum_layer[row][col];
            //} else if (sum_layer[j+1][index] > sum_layer[j][index] and diameters[j*2+1] <= min_diameter){
            } else if (sum_layer[row+1][col] > sum_layer[row][col] and sum_layer[row+1][col] > max){
                index = row;
                max = sum_layer[row+1][col];
            }
        }
        if (max != 0) {
            col = sum_layer[0].size() + 1;
        }
    }
    //return vertex_list[max-1];
    return nominees[index];
}

/**
 * Closeness centrality 
 * @details Calculate a closeness centrality for all vertices at graph
 * @author Carlos Thadeu
 * @param g a graph
 * @return a vector of float that represents closeness centrality
 */
std::vector <float> Centrality::closeness_centrality(Graph &graph){
    int n = graph.get_qty_vertex();
    std::vector <float> closeness;
    for (int i=0; i < n; i++){
        closeness.push_back(vertex_importance(i, graph));
    }
    return closeness;
}

/**
 * Closeness centrality 
 * @details Calculate a closeness centrality for specific vertices
 * @author Carlos Thadeu
 * @param vertices a set of vertices
 * @param graph a graph 
 * @return a vector of pair (int, float) that represents vertex and closeness centrality
 */
std::vector<std::pair<int,float>> Centrality::closeness_centrality_list(std::set <int> &vertices,Graph &graph){
    std::vector<std::pair<int,float>> closeness;
    for (auto i : vertices){
        closeness.push_back({i, vertex_importance(i, graph)});
    }
    return closeness;
}

/**
 * Closeness centrality 
 * @details Calculate a closeness centrality for specific vertices
 * @author Carlos Thadeu
 * @param vertices a vector of vertices
 * @param graph a graph 
 * @return a vector of pair (int, float) that represents vertex and closeness centrality
 */
std::vector<std::pair<int,float>> Centrality::closeness_centrality_list(std::vector <int> &vertices,Graph &graph){
    std::vector<std::pair<int,float>> closeness;
    for (auto i : vertices){
        closeness.push_back({i, vertex_importance(i, graph)});
    }
    return closeness;
}

float Centrality::vertex_importance(int root,  Graph &graph){
    std::set <int> NEIGHBORS;
    std::queue <int> QUEUE1;
    std::vector <int> visited;
    
    int vertex = root;
    float sum = 0;
    int level = 1;

    QUEUE1.push(vertex);
    sum += level;
    level++;

    while (!QUEUE1.empty()){

        while (!QUEUE1.empty()){
            vertex = QUEUE1.front();
            QUEUE1.pop();
            visited.push_back(vertex);

            for (int neighbor : graph.adjList(vertex)){
                if (!in(neighbor, visited )) NEIGHBORS.insert(neighbor);
            }

        }

        std::set<int>:: iterator it;
        for( it = NEIGHBORS.begin(); it!=NEIGHBORS.end(); ++it){
            int value = *it;
            if (!in(value, visited)){
                QUEUE1.push(value);
                sum = sum + level; 
            } 
            
        }
        NEIGHBORS.clear();
        level++;
    }
    return (1 / sum);
}

/**
 * @brief Insert sort
 * @details https://www.geeksforgeeks.org/insertion-sort/
 * @author Original code rathbhupendra
 * @param arr an array
 * @param n array items number
 */
void Centrality::insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
 
        // Move elements of arr[0..i-1], 
        // that are greater than key, to one
        // position ahead of their
        // current position
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

/**
 * @brief Modify insert sort
 * @details Details description
 * That's an insert sort method modify to sort by degree and vertex importance by descending.
 * It's only purpose for heuristic's codes because it's use vertex degree
 * @date 2022/12/15
 * @author Original code rathbhupendra
 * @author Modify code cthadeusantos
 * @param vertex - That's a vector that represents a vertices list
 * @param closeness - That's a vector that represents a closeness complexity (vertex importance)
 * @param graph - That's an instance that represents a graph
 */
void Centrality::my_insertionSort(std::vector <int> &vertex, std::vector <float> &closeness, Graph &graph)
{
    int n = vertex.size();
    int i, key1, key1a, j;
    float key2;
    for (i = 1; i < n; i++)
    {
        key1a = vertex[i];
        key1 = graph.grau(vertex[i]);
        key2 = closeness[i];
        j = i - 1;
 
        // Move elements of arr[0..i-1], 
        // that are greater than key, to one
        // position ahead of their
        // current position
        while (j >= 0 && (graph.grau(vertex[j]) < key1 || (graph.grau(vertex[j]) == key1 && closeness[j] < key2)))
        {
            vertex[j + 1] = vertex[j];
	        closeness[j + 1] = closeness[j];
            j = j - 1;
        }
        vertex[j + 1] = key1a;
	closeness[j + 1] = key2;
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
void Centrality::my_insertionSort(std::vector <int> &vector1, std::vector <int> &vector2, char order='a')
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
        if (order == 'a'){
            while (j >= 0 && key1 < vector1[j])
            {
                vector1[j + 1] = vector1[j];
                vector2[j + 1] = vector2[j];
                j = j - 1;
            }
        } else if (order == 'd'){
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