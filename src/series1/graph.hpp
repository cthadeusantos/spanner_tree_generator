#pragma once

#include "../boost/boost/graph/adjacency_list.hpp"
#include <iostream>

class Graph : public boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> {
private:
    int myPrivateVariable;
    void myPrivateMethod();
    
public:
    void meuMetodo();
};
