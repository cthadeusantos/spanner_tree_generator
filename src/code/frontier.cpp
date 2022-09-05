#include "frontier.hpp"

//! Constructor
/*!
    Open input and output files
    \param name_in input file name
    \param name_out output file name
*/
Frontier::Frontier(const char* name_in, const char* name_out)
{
    in.open(name_in);
    if( not in.is_open() ){
        std::cerr << "Não foi possivel abrir o arqivo de entrada \'" << name_in << '\'' << std::endl;
        exit(-1);
    }
    out.open(name_out);
    if( not out.is_open() ){
        std::cerr << "Não foi possivel abrir o arqivo de saída \'" << name_in << '\'' << std::endl;
        exit(-1);
    }
}

//! Constructor
/*!
    Open input and output files
    \param name_in input file name
    \param name_out output file name
*/
Frontier::Frontier(std::string name_in, std::string name_out)
{
    in.open(name_in);
    if( not in.is_open() ){
        std::cerr << "Não foi possivel abrir o arqivo de entrada \'" << name_in << '\'' << std::endl;
        exit(-1);
    }
    out.open(name_out);
    if( not out.is_open() ){
        std::cerr << "Não foi possivel abrir o arqivo de saída \'" << name_in << '\'' << std::endl;
        exit(-1);
    }
}

//! Destructor
/*!
    Close input & output files
*/
Frontier::~Frontier()
{
    if(in.is_open()){
        in.close();
    }
    if( out.is_open() ){
        out.close();
    }
    
}

//! Destructor
/*!
    Close input & output files
*/
void Frontier::close()
{
    if(in.is_open()){
        in.close();
    }
    if( out.is_open() ){
        out.close();
    }
}

//! Print the adjacency list
/*!
    Print the adjacency list
    \param g a graph instance
    \param msg a string
    \param out an output stream
*/
void Frontier::printAdjList(Graph g, std::string msg, std::ostream& out)
{
    out << msg << std::endl;
    out << "|V| = " << g.getQtdVertices() << std::endl;
    out << "|E| = " << g.getQtdArestas() << std::endl;

    for(int i=0; i < g.getQtdVertices(); ++i){
        out << "\t" << i << " -> ";
        for(int v : g.adjList(i)){
            out << v << " ";
        }
        out << std::endl;
    }
    out << std::endl;
}

//! Print the adjacency matrix - first method
/*!
    Print the adjacency matrix plot vertices, edges and matrix
    \param g a graph instance
    \param msg a string
    \param out an output stream
*/
void Frontier::printAdjMat(Graph g,std::string msg, std::ostream& out)
{
    out << msg << std::endl;
    out << "|V| = " << g.getQtdVertices() << std::endl;
    out << "|E| = " << g.getQtdArestas() << std::endl;

    int ordem = g.getQtdVertices();
    out << ordem << std::endl;
    for(int i=0; i < ordem; ++i){
        for(int j=0; j < ordem; ++j){
            if(g.possui_aresta(i,j)){
                out << "1";
            }else{
                out << "0";
            }
            out << " ";
        }
        out << std::endl;
    }
}

/*
    REVIEW printAdjMat & printAdjMat2 when
    optimization the code
*/
//! Print the adjacency matrix - second method
/*!
    Print the adjacency matrix plot matrix
    \param g a graph instance
    \param msg a string
    \param out an output stream
*/
void Frontier::printAdjMat_2(Graph g,std::string msg, std::ostream& out)
{
    out << msg << std::endl;
    int ordem = g.getQtdVertices();
    out << ordem << std::endl;
    for(int i=0; i < ordem; ++i){
        for(int j=0; j < ordem; ++j){
            if(g.possui_aresta(i,j)){
                out << "1";
            }else{
                out << "0";
            }
            out << " ";
        }
        out << std::endl;
    }
    out << std::endl;
}

//! Read data to the graph
/*!
    User define adjacency matrix order then add or not edges
    if edge(i,j) has value 1, there is edge,
    any other value , there is not edge.

    \param g a graph instance
*/
void Frontier::read(Graph& g)
{
    int ordem;
    int teste;
    in >> ordem;
    g.add_all_vertice(ordem);
    int matriz[ordem][ordem];

    for(int i=0; i < ordem; ++i){
        for(int j=0; j < ordem; ++j){
            in >> matriz[i][j];
        }    
    }
    for(int i=0; i < ordem; ++i){
        for(int j=i+1; j < ordem; ++j){
            if(matriz[i][j] == 1){
                g.add_aresta(i, j);
            }
        }    
    }
}

//! ?
/*!
    ?
    \param g a graph instance
    \param my_in ?
*/
void Frontier::read(Graph& g, std::ifstream& my_in)
{
    int ordem;
    int teste;
    my_in >> ordem;
    g.add_all_vertice(ordem);
    int matriz[ordem][ordem];

    for(int i=0; i < ordem; ++i){
        for(int j=0; j < ordem; ++j){
            my_in >> matriz[i][j];
        }    
    }
    for(int i=0; i < ordem; ++i){
        for(int j=i+1; j < ordem; ++j){
            if(matriz[i][j] == 1){
                g.add_aresta(i, j);
            }
        }    
    }
}

//! Return an error message and exit
/*!
    Return an error message and exit
    \param msg a string
*/
void Frontier::msg_error(const std::string& msg){
    std::cerr << msg << std::endl;
    exit(-1);
}

//! Print stretch index and tree at screen
/*!
    Print stretch index and tree at screen
    \param s a strech instance
*/
void Frontier::print(Strech& s)
{
    out << "index = " << s.getIndex() << std::endl;
    out << "Arvore: " << std::endl;
    printAdjMat(s.getTree(),"Matriz de Adjacência: ", this->out);
    printAdjList(s.getTree(),"Lista de Adjacência: ", this->out);

}

//! Print ??
/*!
    Print ??
    \param value an integer
    \param str a string
    \param out an output stream
*/
void Frontier::print(int value, std::string str,  std::ostream& out)
{
    if ( is_inf(value) ){
        out << str << INF << std::endl;
    }else{
        out << str << value << std::endl;
    }
}

//! Print ??
/*!
    Print ??
    \param value a double
    \param str a string
    \param out an output stream
*/
void Frontier::print(double value, std::string str, std::ostream& out)
{
    if ( is_inf(value) ){
       out << str << INF << std::endl;
    }else{
        out << str << value << std::endl;
    }
}

//! Check if infinity
/*!
    Check if infinity
    \param value an integer
    \return a boolean
*/
bool Frontier::is_inf(int value)
{
    return value == INF_VALUE ? true : false;
}

//! Print ??
/*!
    Print ?
    \param vetor a vector
    \param str a string
    \param out an output stream
*/
void Frontier::print(std::vector<int> vetor, std::string str,std::ostream& out)
{
    if( vetor.empty() ){
        out << "Vetor vazio\n";
    }
    else {
        out << str;
        for(int i : vetor){
            out << i << " ";
        }
        out << std::endl;
    }
    
}

//! Write at file
/*!
    Write at file
    \param value an integer
    \param msg a string
    \param end a string
    \param out an output stream
*/
void Frontier::write(int value, std::string msg, std::string end, std::ostream& out)
{
    out << msg;

    if ( is_inf(value) ){
        out << INF;
    }else{
        out << value;
    }
    out << end;
}

//! Write at file
/*!
    Write at file
    \param value a double
    \param msg a string
    \param end a string
    \param out an output stream
*/
void Frontier::write(double value, std::string msg, std::string end, std::ostream& out)
{
    out << msg;

    if ( is_inf(value) ){
        out << INF;
    }else{
        out << value;
    }
    out << end;
}