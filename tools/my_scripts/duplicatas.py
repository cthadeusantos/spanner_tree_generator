import hashlib
import os

def calcular_hash(matriz):
    matriz_str = str(matriz)
    return hashlib.md5(matriz_str.encode()).hexdigest()

def verificar_duplicatas(pasta, apagar_duplicatas=False):
    hash_tabelas = {}
    duplicatas = []

    for arquivo in os.listdir(pasta):
        if arquivo.endswith(".txt"):
            caminho_arquivo = os.path.join(pasta, arquivo)

            with open(caminho_arquivo, "r") as file:
                # Encontrar N na primeira linha não vazia ou em branco
                n = None
                for linha in file:
                    linha = linha.strip()
                    if linha:
                        n = int(linha)
                        break

                if n is not None:
                    # Ler matriz
                    matriz = []
                    for _ in range(n):
                        linha_matriz = list(map(int, file.readline().split()))
                        matriz.append(linha_matriz)

                    # Calcular hash da matriz
                    hash_matriz = calcular_hash(matriz)

                    # Verificar duplicatas
                    if hash_matriz in hash_tabelas:
                        duplicatas.append((hash_tabelas[hash_matriz], arquivo))
                        if apagar_duplicatas:
                            os.remove(caminho_arquivo)
                    else:
                        hash_tabelas[hash_matriz] = arquivo

    # Escrever duplicatas no arquivo de saída
    with open("duplicidade.output", "w") as output_file:
        for duplicata in duplicatas:
            output_file.write(f"{duplicata[0]} - {duplicata[1]}\n")

if __name__ == "__main__":
    # Modifique o caminho da pasta conforme necessário
    #pasta_de_arquivos = "/caminho/para/seus/arquivos"
    pasta_de_arquivos = "teste/G33/"


    # Modifique esta variável para True se quiser apagar duplicatas
    apagar_duplicatas = False

    verificar_duplicatas(pasta_de_arquivos, apagar_duplicatas)
