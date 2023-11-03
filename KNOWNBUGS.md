[Back homepage](https://github.com/cthadeusantos/spanner_tree_generator)
# KNOWN BUGS

* Sometimes, the induced cycle method doesn't work properly for -ci > 1, Resulting in zero summed trees.
* Sometimes, with large graphs, the TOTAL_TREES field in the final report may take on negative values because it's a signed int32 variable.