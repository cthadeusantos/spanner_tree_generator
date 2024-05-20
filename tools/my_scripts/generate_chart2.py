import sys
import os
import csv
import matplotlib.pyplot as plt


def extrair_rotulo(instancia):
    return instancia.split('-')[-1]


def calcular_arvores_por_segundo(dados):
    total_arvores_por_segundo = []

    for linha in dados[1:]:
        total_arvores = float(linha[-2])
        tempos = list(map(float, linha[2:6]))
        arvores_por_segundo = [total_arvores / tempo for tempo in tempos]
        total_arvores_por_segundo.append(arvores_por_segundo)

    return total_arvores_por_segundo


def plotar_grafico(x, y, labels, cores):
    plt.figure(figsize=(10, 6))

    for i in range(len(y[0])):
        plt.plot(x, [tempo[i] for tempo in y], label=labels[i], color=cores[i], marker='o')

    plt.xlabel('Instance')
    plt.ylabel('Trees per second')
    plt.title('Trees per second per instance')
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    plt.show()


def salvar_grafico(x, y, labels, cores, nome_arquivo):
    plt.figure(figsize=(10, 6))

    for i in range(len(y[0])):
        plt.plot(x, [tempo[i] for tempo in y], label=labels[i], color=cores[i], marker='o')

    plt.xlabel('Instance')
    plt.ylabel('Trees per second')
    plt.title('Trees per second per instance')
    plt.xticks(rotation=45)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    plt.savefig(nome_arquivo)
    print(f"Chart saved as {nome_arquivo}")


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 my_app.py <arquivo_csv>")
        sys.exit(1)

    arquivo_csv = sys.argv[1]

    with open(arquivo_csv, 'r') as arquivo:
        leitor_csv = csv.reader(arquivo)
        dados = list(leitor_csv)

    total_arvores_por_segundo = calcular_arvores_por_segundo(dados)

    cabecalho = dados[0]
    instancias = [linha[0] for linha in dados[1:]]
    rotulos_x = [extrair_rotulo(instancia) for instancia in instancias]

    labels_y = cabecalho[2:6]
    cores = ['b', 'g', 'r', 'c']

    plotar_grafico(rotulos_x, total_arvores_por_segundo, labels_y, cores)

    nome_arquivo_saida = os.path.splitext(arquivo_csv)[0] + '_saida.png'
    salvar_grafico(rotulos_x, total_arvores_por_segundo, labels_y, cores, nome_arquivo_saida)
    print(f"Processo concluído. Resultados salvos em {nome_arquivo_saida}.")

if __name__ == "__main__":
    main()



