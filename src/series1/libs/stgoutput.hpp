#ifndef STGOUTPUT_HPP_
#define STGOUTPUT_HPP_

#include <iostream>
//#include<bits/stdc++.h>
#include <set>
#include <iomanip>      // std::setw

#include "../code/graph.hpp"
#include "../code/version.hpp"
#include "../code/parameters.hpp"
#include "../Debug.h"

void output_data(std::string &run_name, std::string &filename, double &lastExecutionTime, int &lower_limit, Graph &graph);

#endif //STGOUTPUT_HPP_