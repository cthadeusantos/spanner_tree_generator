	#ifdef BFCYCLE
		DEBUG std::cerr << "Solving with induced cycle - PARALLEL- wait!\n";
		run_name = "Induced_cycle";
		create_threads_big_cycle(graph);
	#endif