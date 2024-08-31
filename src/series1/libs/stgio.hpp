#pragma once
#ifndef STGIO_HPP_
#define STGIO_HPP_

#include <string>
#include <iostream>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <math.h>
#include <iterator>
#include <chrono>

#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <set>
#include <regex>
#include <filesystem>
#include <vector>
#include <cstdlib>
#include <sys/types.h>

#include "../code/graph.hpp"
#include "../code/parameters.hpp"
#include "../Debug.h"

#include "stgstring.hpp"
#include "stglimits.hpp"


std::string get_filename_v2();
Graph read_graph_file();
Graph read_graph_file_edges_list();

bool validateEdgeInput(std::string entrada);
bool validateAdjacencyInput(std::string entrada);
bool validateInputBeforeExecution(int argc, char** argv);

#endif //CTFUNCTIONS_HPP_