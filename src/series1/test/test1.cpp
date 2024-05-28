#define BOOST_TEST_MODULE GraphTest
#include <boost/test/included/unit_test.hpp> 
#include <boost/test/framework.hpp>
#include <series1/code/graph.hpp>
#define BOOST_TEST_MODULE GraphTest

BOOST_AUTO_TEST_CASE(first_test) {
    MyGraph graph;

    // Teste para adicionar uma aresta e verificar se ela existe
    graph.addEdge(0, 1);
    BOOST_CHECK(graph.hasEdge(0, 1));

    // Teste para adicionar uma aresta e verificar se ela não existe
    BOOST_CHECK(!graph.hasEdge(1, 0));
}