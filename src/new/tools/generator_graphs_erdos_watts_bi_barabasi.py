import networkx as nx
import numpy as np

# func create, write and close the file
def writeFile(fileName, order, mat):
    f = open(fileName, "w")
    f.write(str(order) + "\n")
    for row in mat:
        linha = ""
        for value in row:
            linha += str(value) + " "
        f.write(linha + "\n")
    f.close()

# --- Dados de Entrada -------------
# Dados para o grafo bi partido
#list_vertices = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
list_vertices = [70]

for vertice in list_vertices:
    BPG_qtd = 10
    BPG_n = vertice>>1 
    BPG_m = vertice>>1
    BPG_p = 30 # xx%
    BPG_name = "gb_" + str(BPG_n) + "_" + str(BPG_m) + "_" + str(BPG_p) + "_"
    BPG_path = "HeuristicsAND_ExactParallel2/grafos_bi_partido/"

    # Dados para o grafo barabasi albert
    BAG_qtd = 10
    BAG_n = vertice
    BAG_m = int(vertice*0.4) # Number of edges to attach from a new node to existing nodes 
    BAG_name = "gba_" + str(BAG_n) + "_" + str(BAG_m) + "_"
    BAG_path = "HeuristicsAND_ExactParallel2/grafos_barabasi/"

    # Dados para o grafo Watts Strogatz
    WSG_qtd = 10
    WSG_n = vertice
    WSG_k =  int(vertice*0.4) # Each node is connected to k nearest neighbors in ring topology 
    WSG_p = 30 # The probability of rewiring each edge

    WSG_name = "gws_" + str(WSG_n) + "_" + str(WSG_k) + "_" + str(WSG_p) + "_"
    WSG_path = "HeuristicsAND_ExactParallel2/grafos_watts/"

    # Dados para o grafo Erdos Reny
    ERG_qtd = 10
    ERG_n = vertice
    ERG_p = 50
    ERG_name = "ger_" + str(ERG_n) + "_" + str(ERG_p) + "_"
    ERG_path = "HeuristicsAND_ExactParallel2/grafos_erdos/"

    # --- Construção dos grafos Bi Partido -------------------------
    with open(BPG_path + str(vertice) + "_vertices.txt", "w") as fd:
        for i in range(BPG_qtd):
            g = nx.bipartite.random_graph(BPG_n, BPG_m, BPG_p / 100)
            while not nx.is_connected(g):
                g = nx.bipartite.random_graph(BPG_n, BPG_m, BPG_p / 100)

            matg = nx.adjacency_matrix(g).todense()
            filename = BPG_path + BPG_name + "_" + str(g.number_of_nodes()) + "_" + str(g.number_of_edges()) +  "_" + str(i) + ".txt" 
            writeFile(filename, g.order(), matg.A)
            fd.write(filename + "\n")

    # --- Construção dos grafos Barabasi -------------------------
    with open(BAG_path + str(vertice) + "_vertices.txt", "w") as fd:
        for i in range(BAG_qtd):
            g = nx.generators.random_graphs.barabasi_albert_graph(BAG_n, BAG_m)
            while not nx.is_connected(g):
                g = nx.generators.random_graphs.barabasi_albert_graph(BAG_n, BAG_m)

            matg = nx.adjacency_matrix(g).todense()
            filename = BAG_path + BAG_name + "_" + str(g.number_of_nodes()) + "_" + str(g.number_of_edges())  +  "_" + str(i) + ".txt" 
            writeFile(filename, g.order(), matg.A)
            fd.write(filename + "\n")

    # --- Construção dos grafos Watts -------------------------
    with open(WSG_path + str(vertice) + "_vertices.txt", "w") as fd:
        for i in range(WSG_qtd):
            g = nx.generators.random_graphs.connected_watts_strogatz_graph(WSG_n, WSG_k, WSG_p / 100)
            while not nx.is_connected(g):
                g = nx.generators.random_graphs.connected_watts_strogatz_graph(WSG_n, WSG_k, WSG_p / 100)

            matg = nx.adjacency_matrix(g).todense()
            filename = WSG_path + WSG_name + "_" + str(g.number_of_nodes()) + "_" + str(g.number_of_edges())  +  "_" + str(i) + ".txt"
            writeFile(filename, g.order(), matg.A)
            fd.write(filename + "\n")

    # --- Construção dos grafos erdos -------------------------
    with open(ERG_path + str(vertice) + "_vertices.txt", "w") as fd:
        for i in range(ERG_qtd):
            g = nx.gnp_random_graph(ERG_n, ERG_p / 100)
            while not nx.is_connected(g):
                g = nx.gnp_random_graph(ERG_n, ERG_p / 100)

            matg = nx.adjacency_matrix(g).todense()
            
            filename = ERG_path + ERG_name + "_" + str(g.number_of_nodes()) + "_" + str(g.number_of_edges())  +  "_" + str(i) + ".txt"
            writeFile(filename, g.order(), matg.A)
            fd.write(filename + "\n")
