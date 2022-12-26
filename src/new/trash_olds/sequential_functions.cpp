#include "graph.hpp"
#include "opBasic.hpp"
#include "stretch.hpp"

#include <ctime>
#include <cstdlib>
#include <iostream>

///Basic debugging controller. See Debug.h for details.
#ifdef MN_BF_SEQ_DEBUG
	#define DEBUG
#else
	#define DEBUG while(false)
#endif

#include "../Debug.h"
/* #include "sequential_functions.hpp"

void sequential(Graph& graph){
	Stretch acme; // Lonney Tunes rocks!
	acme.find_index(graph);
} */