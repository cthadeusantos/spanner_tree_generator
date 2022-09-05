#include "opBasic.hpp"
#include <stack>
#include <queue>

#include <iostream>

#include "frontier.hpp"

//! Returns true if the graph contains a cycle, else false.
/*!
    Seek cycles at graph start from vertex v 
    Returns true if the graph contains a cycle, else false.
    \param g a graph instance that represents a graph
    \param v an integer that represents a start vertex 
*/
bool OpBasic::cyclic(Graph& g, int v)
{
    std::stack<int> my_stack;
    bool visitados[g.getQtdVertices()];
    int prox_vizinho[g.getQtdVertices()];
    int anterior[g.getQtdVertices()];
    for(int i=0; i < g.getQtdVertices(); ++i){
        visitados[i]=false;
        prox_vizinho[i]=0;
        anterior[i]=-1;
    }
    int topo = v;
    int next;
    my_stack.push(topo);
    visitados[topo] = true;
    while(not my_stack.empty() ){
        topo = my_stack.top();
        if(prox_vizinho[topo] == g.grau(topo)){
            my_stack.pop();
        }else{
            next = g.adjList(topo)[prox_vizinho[topo]];
            ++prox_vizinho[topo];
            if(anterior[topo] != next){
                if( visitados[next] ){
                    return true; // POSSUI CICLO
                }
                visitados[next] = true;
                anterior[next] = topo;
                my_stack.push(next);
            }
        }
    }
    return false;
}

//! Check if graph has a cycle
/*!
    Check if graph has a cycle
    Returns true if the graph contains a cycle, else false.
    \param g a graph instance that represents a graph
*/
bool OpBasic::is_cyclic(Graph& g){
    
    for(int i=0; i < g.getQtdVertices(); ++i){
        if( cyclic(g, i) ){
            return true; // TEM CICLO
        }
    }
    return false;
}

//! Check if graph is a tree
/*!
    Check if graph is a tree
    Returns true if the graph is a tree, else false.
    \param g a graph instance that represents a graph
*/
bool OpBasic::is_tree(Graph& g)
{
    if( not is_cyclic(g) && g.getQtdArestas() == g.getQtdVertices()-1 ){
        return true;
    }
    return false;
}


//! Check there is edge absents between two graphs
/*!
    Compare two graphs and return a list with absent edges
    between their.
    \param g a graph instance that represents a graph
    \param t a graph instance that represents another graph
    \return a list of inytgtwith absent edges
*/
std::vector<int> OpBasic::diference_edge(Graph& g, Graph& t)
{
    //std::tuple<int, int> list;
    std::vector<int> list;
    for(int i=0; i < t.getQtdVertices(); ++i){
        for(int j=i+1; j < t.getQtdVertices(); ++j){
            if( g.possui_aresta(i, j) && not t.possui_aresta(i,j) ){
                list.push_back(i);
                list.push_back(j);
            }
        }
    }
    return list;
}

//! Calculate a distance between two vertices
/*!
    Calculate a distance between two vertices
    \param g a graph instance that represents a graph
    \param v a integer that represents a vertex
    \param u a integer that represents a vertex
    \return a integer that represents the distance between two vertices
*/
int OpBasic::distance(Graph& g, int v, int u)
{
    if(v == u){
        return 0;
    }
    std::queue<int> fila;
    int tam = g.getQtdVertices(); 
    int dist[tam];
    for(int i=0; i < tam; ++i){
        dist[i] = -1;
    }
    dist[v] = 0;
    fila.push(v);
    int first;
    while( not fila.empty() ){
        first = fila.front();
        for(int x : g.adjList(first) ){
            if( dist[x] == -1 ){
                dist[x] = dist[first] + 1;
                fila.push(x);
            }
        }
        fila.pop();
    }
    return dist[u];
}

//! Calculate the short path between two vertices
/*!
    ????????????????????????????????????????????????????????w
    \param g a graph instance that represents a graph
    \param v a integer that represents a initial vertex
    \return a vector of vector that represents the short path
*/
std::vector<std::vector<int>> OpBasic::distance(Graph& g, int v)
{
    std::vector<std::vector<int>> caminhos(g.getQtdVertices(), std::vector<int>(g.getQtdVertices(), -1));
    for(int i=0; i < g.getQtdVertices(); ++i){
        caminhos[i] = short_path(g, v, i);
    }

    return caminhos;
}

//! Get the short path between two vertices
/*!
    Calculate the lowest distance between two vertices
    \param g a graph instance that represents a graph
    \param begin a integer that represents a initial vertex
    \param end a integer that represents a finish vertex
    \return a vector that represents the short path
*/
std::vector<int> OpBasic::short_path(Graph& g, int begin, int end)
{
    std::vector<int> my_stack;
    if(begin == end){
        return my_stack;
    }
    int ordem = g.getQtdVertices();
    std::vector<int> path(ordem, -1);
    int maxGrade = 0; // TESTE
    
    // bool visitados[ordem];
    int prox_vizinho[ordem];
    int anterior[ordem];
    for(int i=0; i < ordem; ++i){
        // visitados[i]=false;
        prox_vizinho[i]=0;
        anterior[i]=-1;
    }
    int topo = begin;
    int next;
    my_stack.push_back(topo);
    // visitados[topo] = true;

    //******************************************
    // NAO ENTENDI BEM A NECESSIDADE DO SUMGRADE
    //******************************************
    while(not my_stack.empty() ){
        topo = my_stack.back();
        if(topo == end){
            if( my_stack.size() < path.size() ){
                path = my_stack;
            }
            else if( my_stack.size() == path.size() ){
                if( sumGrade(g, my_stack) > maxGrade ){
                    path = my_stack;
                    maxGrade = sumGrade(g, my_stack);
                }
            }
            my_stack.pop_back();
        }
        else
        if(prox_vizinho[topo] == g.grau(topo)){
            my_stack.pop_back();
            prox_vizinho[topo] = 0;
        }else{
            next = g.adjList(topo)[prox_vizinho[topo]];
            ++prox_vizinho[topo];
            if(anterior[topo] != next){
                if( not pertence(next, my_stack) ){
                    anterior[next] = topo;
                    my_stack.push_back(next);
                }
                /*
                if( visitados[next] ){
                    return true; // POSSUI CICLO
                }

                visitados[next] = true;
                anterior[next] = topo;
                my_stack.push_back(next);
                */
            }
        }
    }
    //path.erase(path.begin());
    return path;
}

//! Get the girth of graph
/*!
    Calculate the girth of graph
    \param g a graph instance that represents a graph
    \return a integer that represents the girth
*/
int OpBasic::girth(Graph& g)
{
    int grt = INF_VALUE;
    int ordem = g.getQtdVertices();
    int parent[ordem];
    int distance[ordem];
    std::vector<int> pilha;
    std::queue<int> fila;
    for(int i=0; i < ordem; ++i){
        parent[i]=-1;
        distance[i]=0;
    }

    for(int v=0; v < ordem; ++v){
        pilha.clear();
        fila.push(v);
        parent[v] = -1;
        distance[v] = 0;
        while( not fila.empty() ){
            int atual = fila.front();
            fila.pop();
            pilha.push_back(atual);
            for(int u : g.adjList(atual)){
                if(u != parent[atual]){
                    if( not pertence(u, pilha) ){
                        parent[u] = atual;
                        distance[u] = distance[atual] + 1;
                        fila.push(u);
                    }else{
                        int small = distance[atual] + distance[u] + 1;
                        if(small < grt){
                            grt = small;
                        }
                    }
                }
            }
        }
    }
    return grt;
}

//! Clean the queue
/*!
    Clean the queue
    \param g a graph instance that represents a graph
*/
void OpBasic::clear(std::queue<int>& q)
{
    while(not q.empty()){
        q.pop();
    }
}

//! Clean the stack
/*!
    Clean the stack
    \param s a stack instance that represents a stack
*/
void OpBasic::clear(std::stack<int>& s)
{
    while(not s.empty()){
        s.pop();
    }
}

//! Check if element is in a vector
/*!
    Given an element verify that exist in a vector that represents e.g. a stack, a queue, etc
    \param x a integer that represents a value searched
    \param v a vector 
    \return a boolean that represent is element exist or not
*/
bool OpBasic::pertence(int x, std::vector<int>& v)
{
    for(int i : v){
        if(i == x){
            return true;
        }
    }
    return false;
}

//! Calculate the center of the graph
/*!
    Get the vertices of the center of the graph
    \param g a graph instance that represents the graph
    \return a vector with vertices that represents the center of the graph
*/
std::vector<int> OpBasic::centers(Graph& g)
{
    int ordem = g.getQtdVertices();
    std::vector<int> matrix(ordem*ordem, 0);
    for(int v=0; v < ordem; ++v){
        for(int u=v+1; u < ordem; ++u){
            matrix[ordem*v+u] = distance(g, v, u);
            matrix[ordem*u+v] = distance(g, v, u);
        }
    }

    int max[ordem];
    for(int v = 0; v < ordem; ++v){
        int maior;
        maior = 0;
        for(int u = 0; u < ordem; ++u){
            if(matrix[ordem*v+u] > maior){
                maior = matrix[ordem*v+u];
            }
        }
        max[v] = maior;
    }
    
    int min = max[0];
    for(int i=1; i < ordem; ++i){
        if(max[i] < min){
            min = max[i];
        }
    }

    std::vector<int> center;
    for(int i=0; i < ordem; ++i){
        if(max[i] == min){
            center.push_back(i);
        }
    }

    return center;
}

//! Calculate the diameter of the graph
/*!
    The diameter of graph is the maximum distance between the pair of vertices.
    It can also be defined as the maximal distance between the pair of vertices.
    Way to solve it is to find all the paths and then find the maximum of all. 
    \param g a graph instance that represents the graph
    \return a integer that represents the diameter of the graph
*/
int OpBasic::diameter(Graph& g)
{
    int maior = 0;
    for(int v=0; v < g.getQtdVertices(); ++v){
        for(int u=v+1;u < g.getQtdVertices(); ++u){
            int d = distance(g, v, u);
            if(d > maior){
                maior = d;
            }
        }
    }
    return maior;
}


/// @brief ?????????????????????????????????
/// @param g 
/// @return ???????????????????????????????
int OpBasic::min_diameter_tree_value(Graph& g)
{
    std::vector<int> center = centers(g);
/*
    std::cout << "Centers: ";
    for( int c : center){
        std::cout << c << " ";
    }
    std::cout << std::endl;
    Frontier f;
*/
    Graph tree(g.getQtdVertices() );
    int min_diameter = INF_VALUE;
    int diameter;
    std::vector<int> path;
    std::vector<std::vector<int>> caminhos(g.getQtdVertices(), std::vector<int>());
    for(int c : center){
        caminhos = distance(g, c);
        for(int v=0; v < g.getQtdVertices(); ++v){
            if(c != v){
                for(int i=0; i < caminhos[v].size()-1; ++i){
                    int x = caminhos[v][i];
                    int y = caminhos[v][i+1];
                    if(not tree.possui_aresta(x, y) ){
                        tree.add_aresta(x, y);
                    }
                }
            }
        }

        diameter = OpBasic::diameter(tree);
/*   
        std::cout << "diameter: " << diameter << std::endl;
        f.printAdjList(tree, "\ttree:");
*/       
        if(diameter < min_diameter){
            min_diameter = diameter;
        }

        tree.clear_arestas();
        path.clear();
        for(int i=0; i < g.getQtdVertices(); ++i){
            caminhos[i].clear();
        }
        
    }
//    std::cout << "-------------------------\n";

    return min_diameter;
}

//! Sum vertices degrees from a vertice list
/*!
    The degree sum formula says that if you add up the degree
    of all the vertices in a (finite) graph,
    the result is twice the number of the edges in the graph
    \param g a graph instance that represents the graph
    \param v a vector that represents the vertices list
    \return a integer that represents the sum of degree
*/
int OpBasic::sumGrade(Graph& g, std::vector<int> v)
{
    auto it = v.begin();
    int sum = 0;
    while(it != v.end()){
        sum += g.grau(*it);
        ++it;
    }

    return sum;
}

//! Check if the graph is valid
/*!
    checks if the graph is connected, is not a tree, not has a universal vertex and not has a leaf
    \param g a graph instance that represents the graph
    \return a boolean that represents true is valid, false is invalid
*/
bool OpBasic::is_valid(Graph& g)
{
    return is_connected(g) && !is_tree(g) && !has_universal_vertex(g) && !has_leaf(g);
}

//! Check if is a connected or disconnected graph
/*!
    Check if is a connect graph or a forest
    \param g a graph instance that represents the graph
    \return a boolean that represents true is connected, false disconnected
*/
bool OpBasic::is_connected(Graph& g)
{
    //std::vector<int>* pilha = new std::vector<int>();
    //std::vector<int>* visitados = new std::vector<int>();
    std::vector<int> pilha;
    std::vector<int> visitados;

    pilha.push_back(0);
    visitados.push_back(0);

    //OpBasic::teste(visitados, "FIRST visitados");
    //OpBasic::teste(pilha, "FIRST pilha");

    while ( !pilha.empty() )
    {
        int atual = pilha.back();
        pilha.pop_back();
        //std::cout << "Atual = " << atual << std::endl;
        for( int v : g.adjList(atual) )
        {
            std::vector<int>::iterator it;
            it = std::find(visitados.begin(), visitados.end(), v);
            //std::cout << "\tv = " << v << std::endl;
            if( it == visitados.end() )
            {
                //int a = *atual;
                visitados.push_back(v);
                pilha.push_back(v);

                //OpBasic::teste(visitados, "visitados");
                //OpBasic::teste(pilha, "pilha");
            }
        }
    }
    bool aswer = visitados.size() == g.getQtdVertices();

    //delete[] pilha;
    //delete[] visitados;

    return aswer;
}

void OpBasic::teste(std::vector<int> v, std::string msg){
    std::cout << msg << ": ";
    for(int x : v){
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

//! Check if graph has a universal vertex
/*!
    Check if graph has a vertex that is adjacent to all
    others vertices of the graph (a dominanting vertex)
    \param g a graph instance that represents the graph
    \return a boolean that represents true(has) or false(not)
*/
bool OpBasic::has_universal_vertex(Graph& g)
{
    int max_grade = g.maior_grau();

    return max_grade == (g.getQtdVertices()-1);
}

//! Check if two graphs are the same
/*!
    Check if two graph are equal
    \param g1 a graph instance that represents the graph
    \param g2 a graph instance that represents the graph
    \return a boolean that represents true(yes) or false(not)
*/
bool OpBasic::equal(Graph& g1, Graph& g2)
{
    if(g1.getQtdVertices() != g2.getQtdVertices() || g1.getQtdArestas() != g2.getQtdArestas()){
        return false;
    }

    for(int i=0; i < g1.getQtdVertices(); ++i)
    {
        if( g1.grau(i) != g2.grau(i)){
            return false;
        }
        for(int j=0; j < g1.grau(i); ++j){
            if(g1.getGraph()[i][j] != g2.getGraph()[i][j]){
                return false;
            }
        }
    }
    return true;
}

//! Check if the graph has leaf
/*!
    Check if the graph has leaf
    \param g a graph instance that represents the graph
    \return a boolean that represents true(yes) or false(not)
*/
bool OpBasic::has_leaf(Graph& g)
{
    for(int i=0; i < g.getQtdVertices(); ++i){
        if(g.grau(i) == 1){
            return true;
        }
    }

    return false;
}

//! Xor function
/*!
    Custom Xor function
    \param a a boolean
    \param b a boolean
    \return a boolean that represents my custom Xor
*/
bool OpBasic::myXor(bool a, bool b)
{
    bool c = a && !b;
    bool d = !a && b;

    return (c || d);
}

//! Xnor function
/*!
    Custom Xnor function
    \param a a boolean
    \param b a boolean
    \return a boolean that represents my custom Xnor
*/
bool OpBasic::myXnor(bool a, bool b)
{
    return !myXor(a,b);
}

//! Rename ???????????????????????????????
/*!
    Rename ???????????????????????????????
    \param g a graph instance that represents the graph
    \param v a integer that represents a vertex
    \param u a integer that represents a vertex
*/
void OpBasic::rename(Graph& g, int v, int u)
{
    Frontier f;
    int n = g.getQtdVertices();
    Graph gAux(n);

    for(int i=0; i<n; ++i){
        if(i != v && i != u){
            if (myXnor( g.possui_aresta(i,v), g.possui_aresta(i,u))){

                for(int j : g.adjList(i) ){
                    if( !gAux.possui_aresta(i,j) ){
                        gAux.add_aresta(i,j);
                    }
                }
                
            }else{

                for(int j : g.adjList(i) ){
                
                    if( j == v ){
                        if( !gAux.possui_aresta(i,u) ) gAux.add_aresta(i, u);
                    }else
                    if( j == u){
                        if( !gAux.possui_aresta(i,v) ) gAux.add_aresta(i, v);
                    }else{
                        if( !gAux.possui_aresta(i,j) ) gAux.add_aresta(i, j);
                    }
                    
                }

            }
        }
    }

    if(g.possui_aresta(v,u)){
        gAux.add_aresta(v,u);
    }

    copy(g, gAux);
}

//! Duplicate a graph
/*!
    Duplicate a graph
    \param g1 a graph instance that represents the old graph
    \param g2 a graph instance that represents the new graph
*/
void OpBasic::copy(Graph& g1, Graph& g2)
{
    g1.clear();
    for(int i = 0; i < g2.getQtdVertices(); ++i){
        g1.add_vertex();
    }

    for(int i = 0; i < g2.getQtdVertices(); ++i){
        for(int j : g2.adjList(i) ){
            if(j > i){
                g1.add_aresta(i, j);
            }
        }
    }
}

//! Get edges' list
/*!
    Build a list with edges from graph
    \param g a graph instance that represents the graph
    \return a vector with edges' list
*/
std::vector<int> OpBasic::edges(Graph& g)
{
    std::vector<int> myEdges(g.getQtdArestas()*2);
    int k = 0;
    for(int i=0; i < g.getQtdVertices(); ++i){
        for(int j : g.adjList(i) ){
            if( j > i){
                myEdges[k] = i;
                myEdges[k+1] = j;
                k += 2;
            }
        }
    }
    return myEdges;
}

//! ????????????????
/*!
    ????????????????
    \param g a graph instance that represents the graph
    \return a vector with edges' list
*/
std::vector<int> OpBasic::edges_by_bigger_degree(Graph& g)
{
    std::vector<int> myEdges;

    myEdges = OpBasic::edges(g);
    int vertex = g.vertice_maior_grau();
    int lastMoved = 0;

    for( int i = 0; i < myEdges.size(); i += 2)
    {
        if( myEdges[i] == vertex || myEdges[i+1] == vertex){
            int aux_u = myEdges[lastMoved];
            int aux_v = myEdges[lastMoved+1];

            myEdges[lastMoved] = myEdges[i];
            myEdges[lastMoved+1] = myEdges[i+1];

            myEdges[i] = aux_u;
            myEdges[i+1] = aux_v;
            lastMoved += 2;
        }
    }

    return myEdges;
}

//! ????????????????
/*!
    Retorna um vetor de arestas que se retirar as arestas a partir do inicio, o grafo nunca sera desconexo.
    \param g a graph instance that represents the graph
    \return a vector with edges' list
*/
std::vector<int> OpBasic::edges_conected(Graph& g)
{
    int n = g.getQtdVertices();
    Graph gAux(n);
    
    gAux.add_aresta( OpBasic::edges(g) );
    std::vector<int> myEdges;
    std::vector<int> cycle;

    while( gAux.getQtdArestas() > n-1){
        cycle.clear(); 

        cycle = OpBasic::biggestCycle(gAux); // Encontra um ciclo sem corda

        gAux.remove_aresta(*cycle.begin(), *(cycle.begin()+1) ); // remove uma aresta que faz parte do ciclo

        myEdges.push_back(*cycle.begin());
        myEdges.push_back( *(cycle.begin()+1) );
    }

    // pega e adciona as arestas que faltam no myEdges
    std::vector<int> e = OpBasic::edges(gAux); 
    for(auto it = e.begin(); it != e.end(); it += 2){
        myEdges.push_back(*it);
        myEdges.push_back(*(it+1));
    }

    return myEdges;
}

//! Detect if a specific vertex belongs a cycle
/*!
    Detect if the vertex n belongs to a cycle
    \param g a graph instance that represents the graph
    \param n an integer representing a vertex that belongs to the cycle 
    \return a vector that represents a vertices list belongs a cycle
*/
std::vector<int> OpBasic::cycle(Graph& g, int n)
{
    std::vector<int> v;
    bool visitado[g.getQtdVertices()];
    bool achou = false;

    for(int i=0; i < g.getQtdVertices(); ++i){
        visitado[i] = false;
    }
    
    for(int i=0; i < g.getQtdVertices(); ++i){
        // std::cout << "\ti = " << i << std::endl;
        find_cycle(g, i, n-1, i, v, visitado, &achou);

        if (achou ) break;
    }
    return v;
}

//! Detect a cycle
/*!
    Seek if vertex belongs a cycle from a specific vertex
    \param g a graph instance that represents the graph
    \param n an integer that represents the numbers of vertices
    \param u an integer that represents a seek vertex
    \param v an integer that represents a start vertex
    \return a vector with vertices list that belongs a cycle
*/
std::vector<int> OpBasic::cycle(Graph& g, int n, int u, int v)
{
    // o vértice v já se encontra no ciclo.
    std::vector<int> ciclo(1, v);
    bool visitado[g.getQtdVertices()];
    bool achou = false;

    for(int i=0; i < g.getQtdVertices(); ++i){
        visitado[i] = false;
    }
    visitado[v] = true; // o vértice 'v' já se encontra no ciclo

    // começa a busca pelo vértice 'u', tendo como "incio" o vértice 'v'
    find_cycle(g, u, n-2, v, ciclo, visitado, &achou);

    if (!achou) {
        ciclo.pop_back(); // remove o vertice 'v'
    }

    return ciclo;
}

int OpBasic::biggestCicleWithEdge(Graph& g, int u, int v)
{
    Frontier f;
    std::vector<int> ciclo;
    std::vector<int> cicloAux;
    int n = girth(g);

    f.print(n, "limite minimo = ");
    cicloAux = cycle(g, n, u, v);

    while( !cicloAux.empty() ){
        ciclo = cicloAux;
        ++n;
        cicloAux = cycle(g, n, u, v);
        f.print(n, "n = ");
        f.print(ciclo, "ciclo = ");
        f.print(cicloAux, "cicloAux = ");
    }
    return ciclo.size();
}

int OpBasic::lowestCicleWithEdge(Graph& g, int u, int v)
{
    Frontier f;
    std::vector<int> ciclo;
    int n = 3; // Menor ciclo possível em um grafo

    while (ciclo.empty() && n <= g.getQtdVertices())
    {
        ciclo = cycle(g, n, u, v);
        ++n;
    }
    return ciclo.size();
}

int OpBasic::maxLowerCicle(Graph& g)
{
    std::vector<int> edgeList = g.edgeList();
    int max = 0;
    int lowerCicle;
    for (int i = 0; i < edgeList.size(); i=i+2)
    {
        lowerCicle = lowestCicleWithEdge(g, edgeList.at(i), edgeList.at(i+1));
        if (lowerCicle > max) {
            max = lowerCicle;
        }
    }
    return max;
}

//! Detect a cycle
/*!
    PARECE SER UM DFS FAZENDO BACKTRAKING PARA DETECTAR UM CICLO
    \param g a graph instance that represents the graph
    \param i a integer that represents node
    \param n a integer ?????????????????????????
    \param start a constant integer ?????????????????
    \param v a vector ?????????????????
    \param visitado a boolean ????????????????????
    \param achou a boolean ??????????????????????
    \return a vector with edges' list
*/
void OpBasic::find_cycle(Graph& g, int i, int n, const int start, std::vector<int>& v, bool* visitado, bool* achou)
{
     
    v.push_back(i);
    visitado[i] = true;
/*
    std::cout << "i , n  = " << i << ", " << n << std::endl;
    for( int j : v){
        std::cout << j << ", ";
    }
    std::cout << std::endl << std::endl;
*/
    if( n == 0 ){
        if( g.possui_aresta(i, start) ){
            if ( !possuiCorda(g, v) ){
                *achou = true; // ciclo encontrado
                return;
            } 
        }
        //v.pop_back();
        //visitado[i] = false;
    }
    else {
        for(int u : g.adjList(i) ){
            if(!visitado[u]){
                find_cycle(g, u, n-1, start, v, visitado, achou); // chamada recursiva
            }
            if( *achou ) return; // ciclo encontrado;
        }
        //v.pop_back();
        //visitado[i] = false;
    }
    v.pop_back();
    visitado[i] = false;
}

//! Detect the biggest cycle
/*!
    Detect the biggest cycle
    \param g a graph instance that represents the graph
    \return a vector with a vertices list that represents the biggest cycle
*/
std::vector<int> OpBasic::biggestCycle(Graph& g)
{
    Frontier f;
    //int min = girth(g);
    //int max = g.getQtdVertices() - 1;
    //int n = (min+max) / 2;
    int n = girth(g);
    std::vector<int> ciclo;
    std::vector<int> cicloAux;

    cicloAux = cycle(g, n);

    while( !cicloAux.empty() ){
        // f.print(n, "Buscando ciclo de tamanho: ");
        ciclo = cicloAux;
        ++n;
        cicloAux = cycle(g, n);
        //f.print(n, "n = ");
        //f.print(ciclo, "ciclo = ");
        //f.print(cicloAux, "cicloAux = ");
    }
    return ciclo;
}

std::vector<int> OpBasic::biggestCycleUpDown(Graph& g)
{
    Frontier f;
    int n = g.getQtdVertices()-1;
    int gi = OpBasic::girth(g);
    std::vector<int> ciclo;
    while( ciclo.empty() ){
        if( n < gi) break;
        f.print(n, "Procurando ciclo de tamanho ");

        ciclo = cycle(g, n);
        --n;
        
    }
    return ciclo;
}


//! Check if cycle has a chord
/*!
    Check if cycle has a chord
    \param g a graph instance that represents the graph
    \param ciclo a vector of integers that represents a list of vertices of a cycle 
    \return a boolean (true, has a chord , false, not has a chord)
*/
bool OpBasic::possuiCorda(Graph& g, std::vector<int> ciclo)
{
    /*
    if(ciclo.empty()){
        std::cout << "VETOR FAZIO DENTRO DA FUNÇÃO \"possuiCorda\".";
        return false;
    }
    */
    std::vector<int>::iterator it;
    std::vector<int>::iterator that;

    it = ciclo.begin();

    // primeira iteracao
    that = it+2;
    while(that != ciclo.end()-1){
        //std::cout << "it, that = " << *it << ", " << *that << std::endl;
        if ( g.possui_aresta(*it, *that)){
            return true;
        }
        ++that;
    }
    ++it;

    // proximas iteracoes
    while( it != ciclo.end()-2){
        that = it + 2;
        while(that != ciclo.end()){
            //std::cout << "it, that = " << *it << ", " << *that << std::endl;
            if ( g.possui_aresta(*it, *that)){
                return true;
            }
            ++that;
        }
        ++it;
    }
    return false;
}

int OpBasic::quantidadeDeTriangulos(Graph& g){
    int qtdTriangulo;
    int ordem = g.getQtdVertices();
    int* a = new int[ordem * ordem];
    initMatrix(a, ordem);
    int* a_2 = new int[ordem * ordem];
    initMatrix(a_2, ordem);
    int* a_3 = new int[ordem * ordem];
    initMatrix(a_3, ordem);

    graphToMatrix(g, a, ordem);

    //print(a, ordem);
    multiplicacaoMatricial(a, a, a_2, ordem);
    //print(a_2, ordem);
    multiplicacaoMatricial(a, a_2, a_3, ordem);
    //print(a_3, ordem);
    int t = traco(a_3, ordem);
    //std::cout << "traco = " << t << std::endl;
    qtdTriangulo = t / 6;

    free(a);
    free(a_2);
    free(a_3);
    return qtdTriangulo;
}

void OpBasic::graphToMatrix(Graph& g, int* m, int ordem)
{
    for(int i = 0; i < ordem; ++i){
        for(int j = i+1; j < ordem; ++j){
            if( g.possui_aresta(i, j) ){
                m[i*ordem + j] = 1;
                m[j*ordem + i] = 1;
            }
        }
        
    }
}



void OpBasic::multiplicacaoMatricial(int* a, int* b, int* c, int ordem)
{
    int sum;
    for(int i = 0; i < ordem; ++i){
        for(int j = 0; j < ordem; ++j){
            sum = 0;
            for(int k = 0; k < ordem; ++k){
                sum += a[i*ordem + k] * b[k*ordem + j];
            }
            c[i*ordem + j] = sum;
        }
        
    }
}

int OpBasic::traco(int* m, int ordem)
{
    int sum = 0;
    for(int i = 0; i < ordem; ++i){
       sum += m[i*ordem + i];
    }

    return sum;
}

void OpBasic::initMatrix(int* m, int ordem, int val)
{
    for(int i = 0; i < ordem; ++i){
        for(int j = 0; j < ordem; ++j){
            m[i*ordem + j] = val;
        }
    }
}
