def read_graphs(file_name):
    with open(file_name, 'r') as file:
        data = file.read().split('\n\n')

    graphs = []
    for graph_data in data:
        if not graph_data.strip():
            continue
        lines = graph_data.strip().replace(' edges','').split('\n')
        graph_num, num_edges = map(int, lines[0].split('-'))
        edges = [tuple(map(int, edge.split())) for edge in lines[1].split(';')]
        graphs.append((graph_num, num_edges, edges))

    return graphs


def write_graph(graph_num, vertices, edges):
    file_name = f"graph_{graph_num}.txt"
    with open(file_name, 'w') as file:
        file.write(f"{vertices}\n")
        for edge in edges:
            file.write(f"{edge[0]},{edge[1]}\n")


def main(file_name):
    graphs = read_graphs(file_name)
    for graph_num, num_edges, edges in graphs:
        vertices = 12  # Assume sempre 12 vértices
        write_graph(graph_num, vertices, edges)


if __name__ == "__main__":
    main("admissiveis.txt")  # Substitua "dados.txt" pelo nome do seu arquivo de dados
