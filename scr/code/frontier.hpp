#ifndef FRONTIER_HPP_
#define FRONTIER_HPP_

#include "graph.hpp"
#include "strech.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

#define INF_VALUE std::numeric_limits<int>::max()
#define INF "inf"

class Frontier
{
private:
    std::ifstream in;
    std::ofstream out;

    bool is_inf(int value);
public:
    Frontier(const char* name_in, const char* name_out);
    Frontier(std::string name_in, std::string name_out);
    Frontier(){}; // motivo de força maior
    ~Frontier();

    void open_in(std::string name_in){  if(not in.is_open()) this->in.open(name_in);
                                        else std::cerr << "shitzn"; }
    void open_in(const char* name_in){ this->in.open(name_in); }

    void open_out(std::string name_out){ this->out.open(name_out); }
    void open_out(const char* name_out){ this->out.open(name_out); }

    void close();
    void close_in(){ if(in.is_open()) this->in.close(); }
    void close_out(){ this->out.close(); }
    std::ostream& getOut() { return this->out; }

    void printAdjList(Graph g,std::string msg, std::ostream& out=std::cout);
    void printAdjMat(Graph g,std::string msg, std::ostream& out=std::cout);
    void printAdjMat_2(Graph g,std::string msg, std::ostream& out=std::cout);
    void print(Strech& s);
    void print(std::vector<int> vetor, std::string str="",std::ostream& out=std::cout);
    void print(int value, std::string str="", std::ostream& out=std::cout);
    void print(double value, std::string str="", std::ostream& out=std::cout);
    
    void write(int value, std::string msg, std::string end, std::ostream& out=std::cout);
    void write(double value, std::string msg, std::string end, std::ostream& out=std::cout);
    
    void read(Graph& g);
    void read(Graph& g, std::ifstream& my_in);

    void msg_error(const std::string& msg);
};

#endif