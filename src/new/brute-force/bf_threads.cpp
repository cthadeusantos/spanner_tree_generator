	#ifdef BFTHREADS
		DEBUG std::cerr << "Solving with parallel brute force limited by threads - wait!\n";
		run_name = "Max_degree_edges";
		create_threads_edge_max_degree(graph);
	#endif