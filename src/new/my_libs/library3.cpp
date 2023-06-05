#include <fstream>
#include <string>

#include "../code/graph.hpp"

void save_tree(std::string myText, std::string fileName){
  std::ofstream output("no_commit/arvores/" + fileName, std::ofstream::out | std::ofstream::app);
  output << myText;
  output.close();
}

std::string tree_to_string(Graph &tree){
    std::string value = "";
    int node1, node2;
    for (node1 = 0; node1 < tree.get_num_vertices(); node1++){
        for (auto node2: tree.adjList(node1)){
            if (node2 > node1) continue;
            value = value + "(" + std::to_string(node1) + "," +  std::to_string(node2) + ") ";
        }
    }
    value = value + "\n";
    return value;
}