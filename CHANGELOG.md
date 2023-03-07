# changes

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