	#ifdef BFMAXDEGREE
		DEBUG std::cerr << "Solving brute force with Maximum degree - PARALLEL- wait!\n";
		run_name = "Maximum_degree-parallel";
		create_threads(graph);
	#endif