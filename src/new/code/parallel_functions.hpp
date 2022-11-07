void find_index_parallel(Graph& g, int raiz, int start, int end, const int id);
void create_threads(Graph& g);
void create_threads_without_trees_duplications(Graph& g);
int adj_id(Graph& g, int v, int adj);
int next(int a, int limite);
int find_factor(Graph& g, Graph& tree);