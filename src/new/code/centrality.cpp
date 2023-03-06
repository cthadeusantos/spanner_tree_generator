#include <math.h>
#include <thread>  // std::thread
#include <mutex>   // std::mutex
#include <semaphore.h> // sem_t, sem_init, sem_wait, sem_post, sem_destroy

#include "centrality.hpp"
#include "heuristic.hpp"
///Basic debugging controller. See Debug.h for details.
/* #ifdef MN_BF_SEQ_DEBUG
	#define DEBUG
#else
	#define DEBUG while(false)
#endif */

extern int num_threads;
extern sem_t semaforo;
extern std::mutex mtx;

/**
 * Calculate leverate centrality for all vertices (using one thread, SEQUENTIAL MODE)
 * @details This measure combines the degree Ki of a node and the degree Kj of each of its neighbor's
 * average over all neighbors vertex Ni.
 * @author Carlos Thadeu
 * @param graph a graph
 * @return a vector of float
 */
std::vector<float> Centrality::leverage(Graph &graph){
    int size = graph.get_qty_vertex();
    std::vector<float> leverage_centrality(size, 0.0);
    for (int Ni=0; Ni < size; Ni++){
        leverage_centrality[Ni] = vertex_leverage(Ni,  graph); 
    }
    return leverage_centrality;
}

/**
 * Split vertices to threads to calculate leverage centrality
 * @details Split vertices to threads to calculate leverage centrality
 * @author Carlos Thadeu
 * @param start an integer that represents the first vertex to be processed
 * @param end an integer that represents last vertex
 * @param leverage a float vector with leverage centrality
 * @param graph a graph 
 * @return a vector of pair (int, float) that represents vertex and closeness centrality
 */
void Centrality::thread_leverage(int start, int end, std::vector <float> &leverage_centrality, Graph &graph){
    //DEBUG std::cerr << "START: " << start << " END: " << end << std::endl;
    for (int i=start; i < end; i++){
        float auxiliary = vertex_leverage(i, graph);
        mtx.lock();
        leverage_centrality[i]=auxiliary;
        mtx.unlock();
        //DEBUG std::cerr << i << " importance: " << std::setprecision(15) << auxiliary << std::endl;
    }
    sem_post(&semaforo);
}

/**
 * Leverade centrality 
 * @details Calculate a leverade centrality for all vertices at graph
 * @author Carlos Thadeu
 * @param graph a graph
 * @return a vector of float that represents leverade centrality
 */
std::vector <float> Centrality::leverage_centrality_thread(Graph &graph){
    int start, end;
    int n = graph.get_qty_vertex();
    int chunk;
    std::thread vetor_th[num_threads];
    std::vector <float> leverage(n, 0.0);

    int execute_thread = num_threads;
    chunk = floor(n / execute_thread);

    if (execute_thread > n){
        execute_thread = n;
        chunk = ceil(n / num_threads)+1;
    }

    DEBUG std::cerr << "execute_thread: " << execute_thread << " , chunk: " << chunk << std::endl ;
    for(int j=0; j < execute_thread; j++){
        if (j == num_threads-1){
            start = j * chunk ;
            end = n - 1;
        } else {
            start = j * chunk;
            end = (j+1) * chunk - 1;
        }
        //DEBUG std::cerr << "Thread: " << j << " start: " << start << " end: " << end << std::endl ;
        vetor_th[j] = std::thread(thread_leverage, start, end + 1, std::ref(leverage), std::ref(graph));
    }
    for(int i=0; i < execute_thread; ++i){
        vetor_th[i].join();
    }
    return leverage;
}

/**
 * Calculate leverate centrality for a vertex
 * @details This measure combines the degree Ki of a node and the degree Kj of each of its neighbor's
 * average over all neighbors vertex Ni.
 * @author Carlos Thadeu
 * @param graph a graph
 * @return a float
 */
float Centrality::vertex_leverage(int Ni,  Graph &graph){
    float leverage = 0.0;
    int Ki, Kj;
    Ki = graph.grau(Ni);
    std::vector<int> neighbors = graph.adjList(Ni);
    float sum_leverage = 0.0;
    for (auto j: neighbors) {
        Kj = graph.grau(j);
        sum_leverage = sum_leverage + (((float)Ki - (float)Kj) / ((float)Ki + (float)Kj));
    }
    return 1.0 / (float)Ki * sum_leverage;
}


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

int Centrality::root_selection2(std::vector<float> vector1){    // By thadeu

    float max = 0;
    float centrality=0;
    int vertex = 0;
    int selected = 0;

    for (int i=0; i < vector1.size(); i++){
        vertex=i;
        centrality = vector1[i];
        if (centrality > max) {
            max = centrality;
            selected = vertex;
        }
    }
    return selected;
}

int Centrality::root_selection3(std::vector<float> vertices_closeness, std::vector<float> vertices_leverage){    // By thadeu

    float max = 0;
    float centrality=0;
    int vertex = 0;
    //int selected = 0;

    std::vector<int> selected;

    for (int i=0; i < vertices_closeness.size(); i++){
        vertex=i;
        centrality = vertices_closeness[i];
        if (centrality > max) {
            selected.clear();
            max = centrality;
            selected.push_back(vertex);
        } else if (centrality == max){
            selected.push_back(vertex);
        }
    }
    float min = vertices_leverage[selected[0]];
    vertex = selected[0];
    for (auto i: selected){
        centrality = vertices_leverage[i];
        if (centrality < min) {
            vertex = i;
        }
    }
    return vertex;
}

/**
 * Select a vertex to be root (DEPRECATED)
 * @details Select a vertex to be root using closeness centrality. (DEPRECATED)
 * @details That's a first version ... (DEPRECATED)
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
 * @details Calculate a closeness centrality for all vertices at graph
 * @author Carlos Thadeu
 * @param g a graph
 * @return a vector of float that represents closeness centrality
 */
std::vector <float> Centrality::closeness_centrality_thread(Graph &graph){
    int start, end;
    int n = graph.get_qty_vertex();
    int chunk;
    std::thread vetor_th[num_threads];
    std::vector <float> closeness(n, 0.0);

    int execute_thread = num_threads;
    chunk = floor(n / execute_thread);

    if (execute_thread > n){
        execute_thread = n;
        chunk = ceil(n / num_threads)+1;
    }

    DEBUG std::cerr << "execute_thread: " << execute_thread << " , chunk: " << chunk << std::endl ;
    for(int j=0; j < execute_thread; j++){
        if (j == num_threads-1){
            start = j * chunk ;
            end = n - 1;
        } else {
            start = j * chunk;
            end = (j+1) * chunk - 1;
        }
        //DEBUG std::cerr << "Thread: " << j << " start: " << start << " end: " << end << std::endl ;
        vetor_th[j] = std::thread(thread_importance, start, end + 1, std::ref(closeness), std::ref(graph));
    }
    for(int i=0; i < execute_thread; ++i){
        vetor_th[i].join();
    }
    return closeness;
}

/**
 * Closeness centrality 
 * @details Calculate a closeness centrality for all vertices at graph
 * @author Carlos Thadeu
 * @param g a graph
 * @return a vector of float that represents closeness centrality
 */
std::vector <float> Centrality::closeness_centrality_thread_V2(Graph &graph){
    int start, end;
    int n = graph.get_qty_vertex();
    int chunk;
    std::thread vetor_th[num_threads];
    std::vector <float> closeness(n, 0.0);

    int execute_thread = num_threads;
    chunk = floor(n / execute_thread);

    if (execute_thread > n){
        execute_thread = n;
        chunk = ceil(n / num_threads)+1;
    }

    DEBUG std::cerr << "execute_thread: " << execute_thread << " , chunk: " << chunk << std::endl ;
    for(int j=0; j < execute_thread; j++){
        if (j == num_threads-1){
            start = j * chunk ;
            end = n - 1;
        } else {
            start = j * chunk;
            end = (j+1) * chunk - 1;
        }
        //DEBUG std::cerr << "Thread: " << j << " start: " << start << " end: " << end << std::endl ;
        vetor_th[j] = std::thread(thread_importanceV2, start, end + 1, std::ref(closeness), std::ref(graph));
    }
    for(int i=0; i < execute_thread; ++i){
        vetor_th[i].join();
    }
    return closeness;
}

/**
 * Split vertices to threads 
 * @details split vertices to threads
 * @author Carlos Thadeu
 * @param start an integer that represents the first vertex to be processed
 * @param end an integer that represents last vertex
 * @param closeness a float vector with importance vertex
 * @param graph a graph 
 * @return a vector of pair (int, float) that represents vertex and closeness centrality
 */
// USING VERTEX IMPORTANCE V2 
// IF YOU WOULD LIKE USE VERTEX IMPORTANCE V1
// UNCOMMENT float auxiliary = vertex_importance(i, graph);
// AND COMMENT
// float auxiliary = vertex_importance_v2(i, graph);
// VERTEX IMPORTANCE 1 IS MUCH MORE PRECISE BUT MORE SLOW TO CALCULATE
void Centrality::thread_importance(int start, int end, std::vector <float> &closeness, Graph &graph){
    //DEBUG std::cerr << "START: " << start << " END: " << end << std::endl;
    for (int i=start; i < end; i++){
        float auxiliary = vertex_importance(i, graph);
        //float auxiliary = vertex_importance_v2(i, graph);
        mtx.lock();
        closeness[i]=auxiliary;
        mtx.unlock();
        //DEBUG std::cerr << i << " importance: " << std::setprecision(15) << auxiliary << std::endl;
    }
    sem_post(&semaforo);
}

/**
 * Split vertices to threads 
 * @details split vertices to threads
 * @author Carlos Thadeu
 * @param start an integer that represents the first vertex to be processed
 * @param end an integer that represents last vertex
 * @param closeness a float vector with importance vertex
 * @param graph a graph 
 * @return a vector of pair (int, float) that represents vertex and closeness centrality
 */
// USING VERTEX IMPORTANCE V2 
// IF YOU WOULD LIKE USE VERTEX IMPORTANCE V1
// UNCOMMENT float auxiliary = vertex_importance(i, graph);
// AND COMMENT
// float auxiliary = vertex_importance_v2(i, graph);
// VERTEX IMPORTANCE 1 IS MUCH MORE PRECISE BUT MORE SLOW TO CALCULATE
void Centrality::thread_importanceV2(int start, int end, std::vector <float> &closeness, Graph &graph){
    for (int i=start; i < end; i++){
        //float auxiliary = vertex_importance(i, graph);
        float auxiliary = vertex_importance_v2(i, graph);
        mtx.lock();
        closeness[i]=auxiliary;
        mtx.unlock();
        //DEBUG std::cerr << i << " importance: " << std::setprecision(15) << auxiliary << std::endl;
    }
    sem_post(&semaforo);
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
        vertex_importance_v2(i, graph);
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
        vertex_importance_v2(i, graph);
    }
    return closeness;
}

/**
 * Closeness centrality 
 * @details Calculate a closeness centrality for all vertices of the graph
 * @author Carlos Thadeu
 * @param vertices a set of vertices
 * @param graph a graph 
 * @return a vector of pair (int, float) that represents vertex and closeness centrality
 */
std::vector<std::pair<int,float>> Centrality::closeness_centrality_list(Graph &graph){
    std::vector<std::pair<int,float>> closeness;
    int vertices=graph.get_qty_vertex();
    for (int i=0; i < vertices; i++){
        closeness.push_back({i, vertex_importance(i, graph)});
        vertex_importance_v2(i, graph);
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
std::vector<std::pair<int,float>> Centrality::closeness_centrality_list_V2(std::vector <int> &vertices,Graph &graph){
    std::vector<std::pair<int,float>> closeness;
    for (auto i : vertices){
        float aux = vertex_importance_v2(i, graph);
        closeness.push_back({i, aux});
    }
    return closeness;
}

/**
 * Closeness centrality 
 * @details Calculate a closeness centrality for all vertices of the graph
 * @author Carlos Thadeu
 * @param vertices a set of vertices
 * @param graph a graph 
 * @return a vector of pair (int, float) that represents vertex and closeness centrality
 */
std::vector<float> Centrality::closeness_centrality_vector(Graph &graph){
    std::vector<float> closeness(graph.getQtdVertices(), 0.00);
    int vertices=graph.get_qty_vertex();
    for (int i=0; i < vertices; i++){
        closeness[i]=vertex_importance(i, graph);
        vertex_importance_v2(i, graph);
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

float Centrality::vertex_importance_v2(int root,  Graph &graph){
    float Z = 8;
    int sum_degree=0;
    for (auto i: graph.adjList(root)){
        float beta = ( (1.0 / (Z - 1.0)) + (log(Z -1.0) / log(Z)) + ((1 / log(Z)) * log(graph.get_qty_vertex())));
        float index = (-1 / log(Z)) * log (graph.grau(i)) + beta;       
        sum_degree = sum_degree + index; 
    }
    //float average = (graph.get_num_edges() * 2 - (graph.grau(root) + sum_degree_leaves)) / graph.get_qty_vertex();
    //float z_bar =  log(graph.get_num_edges() * 2 - (graph.grau(root) + sum_degree_leaves)) ;
    float beta = ( (1.0 / (Z - 1.0)) + (log(Z -1.0) / log(Z)) + ((1 / log(Z)) * log(graph.get_qty_vertex())));
    float index = (-1 / log(Z)) * log (graph.grau(root)) + beta;
    float centra = index - sum_degree;
    //float index = (-1 / log(Z)) * log (graph.grau(root)) + beta;
    //DEBUG std::cerr << root << "---"<< std::setprecision(15) << 1/index << std::endl; 
    return 1 / index;
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
    int i, key1, key0, j;
    float key2;
    for (i = 1; i < n; i++)
    {
        key0 = vertex[i];
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
        vertex[j + 1] = key0;
	closeness[j + 1] = key2;
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
void Centrality::my_insertionSort(std::vector <int> &vertex, std::vector <float> &closeness, std::vector <float> &leverage, Graph &graph)
{
    int n = vertex.size();
    int i, key1, key0, j;
    float key2;
    float key3;
    for (i = 1; i < n; i++)
    {
        key0 = vertex[i];
        key1 = graph.grau(vertex[i]);
        key2 = closeness[i];
        key3 = leverage[i];
        j = i - 1;
 
        // Move elements of arr[0..i-1], 
        // that are greater than key, to one
        // position ahead of their
        // current position
        while (
            j >= 0 && (graph.grau(vertex[j]) < key1
                        || (graph.grau(vertex[j]) == key1 && closeness[j] < key2)
                        || (graph.grau(vertex[j]) == key1 && closeness[j] == key2 && leverage[j] == key3)
                        
                        )
        
        ){
            vertex[j + 1] = vertex[j];
	        closeness[j + 1] = closeness[j];
            leverage[j + 1] = leverage[j];
            j = j - 1;
        }
        vertex[j + 1] = key0;
	closeness[j + 1] = key2;
    leverage[j + 1] = key3;
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
void Centrality::my_insertionSort(std::vector <int> &vector1, std::vector <int> &vector2, char order)
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

int Centrality::tiebreaker(std::vector<int> &vertex_list, std::vector<float> &closeness){
    float max = 0;
    int best_vertex=vertex_list[0];
    for (auto vertex: vertex_list){
        if (closeness[vertex] > max){
            max = closeness[vertex];
            best_vertex = vertex;
        }
    }
    return best_vertex;
}

int Centrality::tiebreaker(std::vector<int> &vertex_list, std::vector<float> closeness, std::vector<float> leverage){
    if (vertex_list.size() == 1)
        return vertex_list[0];
    int best_vertex=vertex_list[0];
    float max = closeness[best_vertex];
    float min = leverage[best_vertex];
    for (auto vertex: vertex_list){
        if (closeness[vertex]==max && leverage[vertex]==min){
            best_vertex=vertex;
        }
        else if (closeness[vertex]==max && leverage[vertex] < min){
            min=leverage[vertex];
            best_vertex=vertex;
        } else if (closeness[vertex] > max ){
            max=closeness[vertex];
            best_vertex=vertex;
        }
    }
    return best_vertex;
}