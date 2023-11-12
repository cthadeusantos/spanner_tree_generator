[Back homepage](https://github.com/cthadeusantos/spanner_tree_generator)
# TO DO

* Soon, heuristic 4 will be the only one. Degree, closeness, and leverage will be chosen through parameters. 
* Validate the number of vertices in the input against the adjacency matrix or edge list
* Improve closeness centrality at heuristic 4 (split closeness vector for each thread against pass-by-reference for all vector)
* Implement sum of trees in 64-bit unsigned integer: unsigned long long