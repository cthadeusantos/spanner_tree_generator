#include "heuristic.hpp"

int Heuristic::heuristica_1(Graph& g)
{
    //Frontier f;
    Strech strech;
    int n = g.getQtdVertices();
    std::vector<int> vertex_list(n);
    for( int i = 0; i < n; ++i)
    {
        vertex_list[i] = i;
    }

    my_quicksort(vertex_list, 0, n, g);

    Graph tree(n);

    for( int v : g.adjList(vertex_list[0]) )
    {
        tree.add_aresta(vertex_list[0], v);
    }

    int i = 1;
    while( i < n && !OpBasic::is_tree(tree))
    {
        for( int v : g.adjList(vertex_list[i]))
        {
            if( !tree.possui_aresta(vertex_list[i], v))
            {
                tree.add_aresta(vertex_list[i], v);
                if(OpBasic::is_cyclic(tree))
                {
                    tree.remove_aresta(vertex_list[i], v);
                }
            }
        }
        ++i;
    }
/*
    f.printAdjMat(tree, "arvore h1:");
    for( int v : vertex_list){
        f.write(v, "", " ");
    }
*/
    return strech.find_factor(g, tree);
}

int Heuristic::heuristica_2(Graph& g)
{
    Strech strech;
    Graph tree(g.getQtdVertices());
    int raiz = g.vertice_maior_grau();
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    for( int v : g.adjList(raiz))
    {
        lista.push_back(v);
        tree.add_aresta(raiz, v);
    }
    while( tree.qtd_vertex_grau() > 0)
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(g.grau(v) - func_aux_h2(tree, g, v));
            lista_relativa_vertice.push_back(v);
        }

        my_sort(lista_relativa_valor, lista_relativa_vertice);

        int u = lista_relativa_vertice.back();
        std::vector<int>::iterator it = std::find(lista.begin(), lista.end(), u);
        lista.erase(it);

        for( int v : g.adjList(u))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(u, v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    return strech.find_factor(g, tree);
}

Graph Heuristic::heuristica_2_global(Graph& g)
{
    std::vector<int> vertices_maiores_grau = g.vertices_de_maior_grau();
    Graph tree;
    Graph right_tree;
    Strech strech;
    int index = INF_VALUE;
    int new_index = 0;
    for(int vertice : vertices_maiores_grau) {
        tree = heuristica_2_tree(g, vertice);
        new_index = strech.find_factor(g, tree);

        if(new_index < index) {
            index = new_index;
            right_tree = tree;
        }
    }

    return right_tree;
}

Graph Heuristic::heuristica_2_tree(Graph& g)
{
    // Strech strech;
    Graph tree(g.getQtdVertices());
    int raiz = g.vertice_maior_grau();
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    for( int v : g.adjList(raiz))
    {
        lista.push_back(v);
        tree.add_aresta(raiz, v);
    }
    while( tree.qtd_vertex_grau() > 0)
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(g.grau(v) - func_aux_h2(tree, g, v));
            lista_relativa_vertice.push_back(v);
        }

        my_sort(lista_relativa_valor, lista_relativa_vertice);

        int u = lista_relativa_vertice.back();
        std::vector<int>::iterator it = std::find(lista.begin(), lista.end(), u);
        lista.erase(it);

        for( int v : g.adjList(u))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(u, v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    return tree;
}

Graph Heuristic::heuristica_3_tree(Graph& g)
{
    Graph tree(g.getQtdVertices());
    int raiz = g.vertice_maior_grau();
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    
    // coloca o vertice de maior grau e os seus vizinhos na arvore
    for( int v : g.adjList(raiz))
    {
        tree.add_aresta(raiz, v);
    }

    // coloca todos os vertices do grafo na lista
    for(int i = 0; i < g.getQtdVertices(); ++i)
    {
        lista.push_back(i);
    }

    while( tree.qtd_vertex_grau() > 0 )
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(g.grau(v) - func_aux_h2(tree, g, v));
            lista_relativa_vertice.push_back(v);
        }

        my_sort(lista_relativa_valor, lista_relativa_vertice);

        int u = lista_relativa_vertice.back();

        for( int v : g.adjList(u))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(u, v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    if( !OpBasic::is_tree(tree) )
    {
        std::vector<int> list = OpBasic::diference_edge(g, tree);
        int i = 0;
        while( !OpBasic::is_tree(tree) )
        {
            tree.add_aresta(i, i+1);
            if( OpBasic::is_cyclic(tree) )
            {
                tree.remove_aresta(i, i+1);
            }
            i += 2;
        }
    }

    return tree;
}

Graph Heuristic::heuristica_2_tree(Graph& g, int raiz)
{
    // Strech strech;
    Graph tree(g.getQtdVertices());
    // int raiz = g.vertice_maior_grau();
    std::vector<int> lista;
    std::vector<int> lista_relativa_valor;
    std::vector<int> lista_relativa_vertice;
    for( int v : g.adjList(raiz))
    {
        lista.push_back(v);
        tree.add_aresta(raiz, v);
    }
    while( tree.qtd_vertex_grau() > 0)
    {
        for(int v : lista)
        {
            lista_relativa_valor.push_back(g.grau(v) - func_aux_h2(tree, g, v));
            lista_relativa_vertice.push_back(v);
        }

        my_sort(lista_relativa_valor, lista_relativa_vertice);

        int u = lista_relativa_vertice.back();
        std::vector<int>::iterator it = std::find(lista.begin(), lista.end(), u);
        lista.erase(it);

        for( int v : g.adjList(u))
        {
            if( tree.grau(v) == 0)
            {
                tree.add_aresta(u, v);
                lista.push_back(v);
            }
        }

        lista_relativa_valor.clear();
        lista_relativa_vertice.clear();
    }

    return tree;
}

Graph Heuristic::heuristica_1_tree(Graph& g)
{
    //Frontier f;
    Strech strech;
    int n = g.getQtdVertices();
    std::vector<int> vertex_list(n);
    for( int i = 0; i < n; ++i)
    {
        vertex_list[i] = i;
    }
    
    my_quicksort(vertex_list, 0, n, g);

    Graph tree(n);
    
    for( int v : g.adjList(vertex_list[0]) )
    {
        tree.add_aresta(vertex_list[0], v);
    }
    
    int i = 1;
    while( i < n && !OpBasic::is_tree(tree))
    {
        for( int v : g.adjList(vertex_list[i]))
        {
            if( !tree.possui_aresta(vertex_list[i], v))
            {
                tree.add_aresta(vertex_list[i], v);
                if(OpBasic::is_cyclic(tree))
                {
                    tree.remove_aresta(vertex_list[i], v);
                }
            }
        }
        
        ++i;
    }

    return tree;
}

void Heuristic::my_quicksort(std::vector<int>& vertices, int began, int end, Graph& g)
{
	int i, j, pivo, aux;
	i = began;
	j = end-1;
	pivo = g.grau(vertices[(began + end) / 2]);
	while(i <= j)
	{
		while( g.grau(vertices[i]) > pivo && i < end)
		{
			++i;
		}
		while( g.grau(vertices[j]) < pivo && j > began)
		{
			--j;
		}
		if(i <= j)
		{
			aux = vertices[i];
			vertices[i] = vertices[j];
			vertices[j] = aux;
			++i;
			--j;
		}
	}
	if(j > began)
		my_quicksort(vertices, began, j+1, g);
	if(i < end)
		my_quicksort(vertices, i, end, g);
}


int Heuristic::func_aux_h2(Graph& tree, Graph& g, int v)
{
    int count = 0;
    for(int u : g.adjList(v) ){
        if( tree.grau(u) != 0){
            ++count;
        }
    }
    return count;
}

void Heuristic::my_sort(std::vector<int>& v1, std::vector<int>& v2)
{
    int n = v1.size();
    for(int i = 0; i < n-1; ++i)
    {
        int j = i;
        while( j > 0 && v1[j] > v1[j+1])
        {
            int aux = v1[j];
            v1[j] = v1[j+1];
            v1[j+1] = aux;

            aux = v2[j];
            v2[j] = v2[j+1];
            v2[j+1] = aux;

            --j;
        }
    }
}