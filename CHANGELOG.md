# changes

## 0.1.7 (2023/05/03)
* Fix the error that caused slowdown in cycle M4 method

## 0.1.6 (2023/05/02)
* Reorganize tools directories
* Return to classical settings MAX and EDGES parallelism
* Bug fixes at reorganize combinations(cycle M4, create thread)

## 0.1.5 (2023/04/29)
* Induced cycle M4 method implemented
* Add sum trees cycle M4
* New parameters add (--version , --noindex)
* Parameter nolb now working with brute-force
* Paralell max degree was modified, now there is a V2
* App max_degree and edge paralellism now are limited by thread numbers
* New define block size and chunk size for threads implemented
* Remove compute grt inside create_thread (parallel)
* Replace name new_find_index to find_index_cycleM4 (parallel.cpp)
* Many minor fixes, new tools, new batteries
* Add tools

## 0.1.4b (2023/04/26)
* Add tools
* App max_degree and edge paralellism now are limited by thread numbers
* App induced cycle M4 not working yet

## 0.1.4a (2023/04/19)
* New parameters add (--version , --noindex)
* Parameter nolb now working with brute-force
* Paralell max degree was modified, now there is a V2
* New method for induced cycle in progress (not working yet)
* Many minor fixes, new tools, new batteries

## 0.1.4 (2023/04/11)
* Fix wrong dates at CHANGELOG.md
* Implemented new method to induced cycles, now seek for induced cycles using girth with reference

## 0.1.3.7a (2023/04/09)
* Fix bugs at SUM_TREES in INDUCED CYCLE that return invalid value in specific cases with few threads that was missing and not commit in 0.1.3.7
* All updates for 0.1.3.7

## 0.1.3.7 (2023/04/08)
* Added parameter (-rt | --running_time) to stop execution after X miliseconds (all applications)
* Class Watchdog added
* Fix bugs at SUM_TREES in INDUCED CYCLE that return invalid value in specific cases with few threads 

## 0.1.3.6a (2023/04/07)

* All 0.1.3.6 updates
* Update my analisys tools (not for users)
* Update sequential yaml to use only 1 thread
* Analysis tools added, working but waiting for refactoring
* Old struct zipped, directory removed
* Add analysis tools for graphs with 10 until 20 vertices
* Update makefile to compile linux and macOS(not testing)

## 0.1.3.6 (2023/03/20)

* Fixed not defined type var for node1 and node2
at read_graph_file_edges_list() (forgot after last update)
* Var max_cycle_size_SPECIAL is defined to be
8 (max induced cicle to be searched)
* We comment procedure to define size of
induced cycle(max_cycle_size_SPECIAL) based
on threads number thread to be used at future

## 0.1.3.5 (2023/03/20)
* Fixed not defined type var for node1 and node2 at read_graph_file_edges_list() (forgot after last update)
* Var max_cycle_size_SPECIAL is defined to be 8 (max induced cicle to be searched)
* We comment procedure to define size of induced cycle(max_cycle_size_SPECIAL) based on threads number thread to be used at future

## 0.1.3.4 (2023/03/11)
* Add parameter --nolb to avoid compute lower bound using heuristics with big graphs
* Heuristics updated 
* H4v1 updated to select root using degree centrality , if are tie, applied closeness and leverage centrality to select root
* Replaced std::vector <int> enqueued to std::vector <bool> visited(n, false) in heuristics 4, now complexity to check if vertex was visited is O(1)
* Batteries H4v2r2 not will run anymore

## 0.1.3.3 (2023/03/10)
* Fixed wrong parameters passed to method tiebreaker(std::vector<int> &vertex_list, std::vector<float> &closeness, std::vector<float> &leverage);
* Fix tiebreaker(std::vector<int> &vertex_list, std::vector<float> &closeness, std::vector<float> &leverage) parameters

## 0.1.3.2(2023/03/07)
* Induced cycle M3 version implementation
* Change Induced cycle M2 to M3 (M2 was discarted)
* Move batteries induced cycle M2 to M3
* Remove comments
* Comment code unused
* Remove induced cycle M2 from makefile
* Minor fixes
* Makefile updated

## 0.1.3.1(2023/03/01)
* Leverage centrality implemented
* Closeness centrality calculated implemented
* Heuristics 1, 2, 3 & 4 now are using closeness and leverage calculated
* H2v1 & v2 using my_insertSort, old mySort has a bug!
* H2v2, new tiebreaker implemented
* H4v1, using closeness and leverage calculated
* H4v2r1, using closeness with precision, much more slow at bigger graphs
* H4v2r2, using only closeness calculated
* H4v2r3 implemented using closeness and leverage centrality calculated
* Method the logarithm of n to the base b implemented (logBn)
* New my_insertSort using 3 vectors
* New root selection added (root_selection3)
* Makefile has changed
* Minor fixes

## 0.1.3 (2023/02/26)
* Change init measure point with chronos before lower bound calc
* Fixed wrong split threads at  closeness_centrality_thread
* Add thread_importanceV2 and vertex_importanceV2
Now we estimate vertex_importance
* Add new heuristic4v2 (now R1 & R2)
R1 accurately calculates the importance of vertice
R2 R1 calculates through estimation the importance of vertice
* Batteries added
* Class Parameters added

## 0.1.2.2-alpha (2023/02/19)
* Rebuild heuristic 3 v1&v2
* New reading format added
* Tiebreaker function added
* Recovery barabasi.txt (it was damage at 0.1.2.1-alpha)

## 0.1.2.1-alpha (2023/02/09)
* Adjust files and directories (CT)
* Add tools for running with examples (AZ)
* Add tools for analyse with examples (AZ)

## 0.1.2-alpha (2023/02/08)
* FIXED a serious bug that caused core dumped (wrong index) at heuristic 4 (v1 & v2) in some graphs (G14_centrallity)
* add new graph (G13_turan_graph) at examples instances
* Replace tag name Breadth Heuristic to H4v1 for compatibility reasons
* Update my python tools at my_python_tools(these are just for my purpose), not for general public


## 0.1.1-alpha (2023/02/05)
* Python tools added (only my purposes)
* Add new graphs (examples)
* Analysis file example (real heuristics data) added

## 0.1.0-alpha (2023/01/31)
* First release! Now we have an usable code.