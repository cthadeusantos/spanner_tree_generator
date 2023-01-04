	#ifdef BFSEQUENTIAL
		DEBUG std::cerr << "Solving with sequential brute force - wait!\n";
		run_name = "Brute_force-Sequential";
		Stretch().sequential(graph);
	#endif