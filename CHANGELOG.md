# Changes

## 0.2.10 (2024/02/18)
* The variable total_trees in Graph class has been changed from signed int 32 bits to unsigned long long int 64 bits, fixing the issue of the final sum of total trees being negative in some cases.

## 0.2.9 (2024/02/16)
* Added help message after the warning 'No input redirection detected.'.
* Comment line DEBUG std::cerr << "Running on Linux" << std::endl; in ctfunctions2.cpp which could insert the message into the output file in debug mode.
* Comment line DEBUG std::cerr << "Running on macOS" << std::endl; in ctfunctions2.cpp which could insert the message into the output file in debug mode.
* Added warning "I cannot read the input file on this operating system. Please contact the developers." if unsupported operating system detected.

## 0.2.8 (2024/02/16)
* Fixed the issue that caused the insertion of the message 'Running on Linux/Mac/Win' into the output file of the application, causing an error that halted the execution of the battery files.

## 0.2.7 (2024/01/28)
* Now the code compile and running at MacOS
* Added Makefile for OSX.
* Fixed segmentation fault, trace trap when running at MacOS.
* Update split_filename_v2() and get_filename_v2() again for compatibility reasons between Linux and MacOS.
* Replace (int)INFINITY to std::numeric_limits<int>::max() for compatibility reasons between Linux and MacOS.
* Added the get_filename_V2() function to fix a segmentation fault issue on MacOS when reading an input file. (ctfunction2.cpp).
* Added the split_filename_v2(). (ctfunction2.cpp)
* All main files for brute force and heuristics have been adjusted to use get_filename_v2().
* Replaced the INT_MAX constant in heuristics.cpp with std::numeric_limits<int>::max(), using #include <limits>.
* Removed the non-standard header file stdc++.h for improved compatibility between Linux and MacOS (heuristics.cpp , graph.cpp, centrality.cpp, ctfunctions2.cpp).
* Added header file include <functional>. (cycles.cpp).
* Replaced get_current_dir_name() with get_current_dir() in main_create_graphs.cpp, correcting an error in the function name associated with DIR_BASE.
* Added 3 new functions to validate parameters passed through input: isDataAvailable(), isInputRedirected(), and validateInputBeforeExecution().
* Makefile_OSX updated.
* README.md updated.
* Minor fixes.
* Minor adjusts.

## 0.2.6 (2024/01/17)
* Fix bug in heuristic 3 that caused incorrect stretch factor for some graphs.

## 0.2.5 (2023/11/12)
* Added info "NO" the CLOSENESS_(HEUR) output includes the additional information 'NO' for heuristics H1v1, H1v3, H1v4, H2v1, H3v1.
* Added new paper PDF
* Added the PowerPoint presentation for the Brazilian Operational Research Symposium 2023. 

## 0.2.4
* New version system implemented
* Minor fixes

## 0.2.3a (2023/11/03)
* Fixed the bug that was causing 'free(): invalid pointer' in heuristics H3v2.
* Added the output information MAX_THREAD_SUPPORTED, which indicates the maximum number of threads supported by the processor.

## 0.2.3 (2023/10/26)
* Heuristics H1v3 & H1v4 provided by Eriky Marciano

## 0.2.2 (2023/10/26)
* Fixed bug that could select inappropriate vertex in the list of candidate vertices in the H2v2 & H3v2 heuristics
* Fixed bug in heuristic 4 where the closeness list for candidate vertices was defined as a list of integers, changed to a list of floats
* Added new parameters --alg & --tra, now you can select the closeness method to be used (ALG, uses the algebraic method to calculate the closeness of the vertices and TRA, the method traverses the vertices of the graph to define the closeness value)
* Heurísticas H1v2, H2v2, H3v2, and all H4 now benefit from the implemented --alg and --tra parameters
* Added a new overload to the Centrality::my_insertionSort(std::vector <float> &, std::vector <int> &, char) method
* Added a new overload to the Centrality::tiebreaker(std::vector<int> &, std::vector<int> &, std::vector<float> &, std::vector<float> &);
* Added new information to the output report. Date, lower bound calculation requested or not, stretch index calculation requested or not
* Many minor bugs fixed
* Many minor methods or functions implemented


## 0.2.1.1 (2023/09/20)
* FIX! Remove ' s' characters at field running time in the final report
* Change attribute at Class Version, number -> release
* Update README.md
* Added TODO.md
* Added a new dataset (dataset3)

## 0.2.1 (2023/07/xx)
* FIX! Heuristic 3v2 now reads edges' list files 
* Update README.md
* Added TODO.md
* Added 3 new graphs

## 0.2.0 (2023/07/06)
Better news! Release 0.2.0 is out!
* Induced cycle method implemented
* Improvements implemented in the sequential method
* Improvements implemented in the adjacencies list method
* Improvements implemented in the edges list method
* Update output data function
* New functions added at centrallity
* Fixes at Graph class

## 0.2.0-RC Release Candidate (2023/07/04)
* **Induced cycle is working**
* Update & fix edge paralellism that cause a huge running time  
* Minor fixes
* Minor updates

## 0.1.7e (2023/06/15)
* Fixed bug in stretch::find that was causing an issue in passing the value of the extension index in CYCLE_M4
* Disable articulations method (will be remove at future) *comment*
* Disable vertex method (graph.cpp) specific for articulation disable
* Disable CYCLE_M3 (remove from makefile)
* Add 3 new graphs at examples

## 0.1.7d (2023/06/23)

## 0.1.7a (2023/05/03)
* Fix parameters file bug that not show "warning" if unknown parameters are passed
* Update README.md

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