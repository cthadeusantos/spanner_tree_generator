import os

def verificar_matriz_valida(matriz, num_vertices):
    # Verificar se a matriz é uma lista de listas
    if not isinstance(matriz, list) or not all(isinstance(row, list) for row in matriz):
        return False

    # Verificar se o número de linhas da matriz é igual ao número de vértices
    if len(matriz) != num_vertices:
        return False

    # Verificar se cada linha da matriz tem o número correto de elementos e contém apenas 0 ou 1
    for row in matriz:
        if len(row) != num_vertices or not all(element in {0, 1} for element in row):
            return False

    return True

def calcular_distribuicao_graus(arquivo):
    with open(arquivo, 'r') as file:
        # Ler as linhas do arquivo e remover espaços em branco
        linhas = [line.strip() for line in file.readlines()]

        # Remover linhas em branco
        linhas = [line for line in linhas if line]

        # Obter o número de vértices
        num_vertices = int(linhas[0])

        # Inicializar a matriz de adjacência
        matriz_adjacencia = [list(map(int, line.split())) for line in linhas[1:]]

        # Verificar se a matriz é válida
        if not verificar_matriz_valida(matriz_adjacencia, num_vertices):
            raise ValueError("A matriz de adjacência no arquivo não é válida.")

    # Inicializar lista de graus com zeros
    graus = [sum(row) for row in matriz_adjacencia]

    # Ordenar a lista de graus em ordem não-decrescente
    graus.sort(reverse=True)

    return graus

def main():
    # Solicitar o diretório onde os arquivos estão localizados
    diretorio_base = input("Digite o diretório onde os arquivos estão localizados (ex: ../instances/grafo_15/): ")

    # Adicionar barra final ao diretório se não estiver presente
    if not diretorio_base.endswith('/'):
        diretorio_base += '/'

    # Criar o diretório 'output' dentro do diretório base
    diretorio_saida = os.path.join(diretorio_base, 'output')
    os.makedirs(diretorio_saida, exist_ok=True)

    # Solicitar o nome do arquivo sem a tag "-X"
    nome_base = input(f"Digite o nome do arquivo (sem a tag '-X') no diretório {diretorio_base}: ")

    # Solicitar se deseja sobrescrever o arquivo de saída, se existir
    sobrescrever = input("Deseja sobrescrever o arquivo de saída, se já existir? (S/N): ").lower() == 's'

    # Processar os arquivos e calcular as distribuições de graus
    distribuicoes = []
    i = 1
    while True:
        nome_arquivo = os.path.join(diretorio_base, f"{nome_base}-{i}.txt")

        # Verificar se o arquivo existe
        if not os.path.exists(nome_arquivo):
            break

        try:
            distribuicao_graus = calcular_distribuicao_graus(nome_arquivo)

            # Adicionar a distribuição à lista auxiliar
            distribuicoes.append((nome_arquivo, distribuicao_graus))

        except ValueError as e:
            print(f"Erro ao processar {nome_arquivo}: {e}")

        i += 1

    # Ordenar as distribuições apenas pelos graus
    distribuicoes.sort(key=lambda x: x[1], reverse=True)

    # Salvar as distribuições ordenadas no arquivo
    output_file_path = os.path.join(diretorio_saida, f"{nome_base}-distribuicao.out")

    # Determinar o modo de abertura do arquivo
    modo = 'w' if sobrescrever else 'a'

    # Adicionar o cabeçalho (apenas na primeira vez)
    if sobrescrever or not os.path.exists(output_file_path):
        with open(output_file_path, 'w') as output_file:
            output_file.write("Nome do Arquivo\tDistribuição de Graus\n")

    # Salvar as distribuições no arquivo
    with open(output_file_path, modo) as output_file:
        for nome_arquivo, distribuicao_graus in distribuicoes:
            nome_arquivo_simples = os.path.splitext(os.path.basename(nome_arquivo))[0]
            output_file.write(f"{nome_arquivo_simples}\t{' '.join(map(str, distribuicao_graus))}\n")

    print(f"\nArquivo de distribuição gravado em: {output_file_path}")

if __name__ == "__main__":
    main()

