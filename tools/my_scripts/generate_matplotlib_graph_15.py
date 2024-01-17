import sys
import os
import csv
import matplotlib.pyplot as plt


def extrair_rotulo(instancia):
    return instancia.split('-')[-1]


def plotar_grafico(x, y, labels, cores):
    plt.figure(figsize=(10, 6))

    for i in range(len(y[0])):
        plt.plot(x, [tempo[i] for tempo in y], label=labels[i], color=cores[i], marker='o')

    plt.xlabel('Instance')
    plt.ylabel('Time (s)')
    plt.title('Average Times per Instance')
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
    plt.ylabel('Time (s)')
    plt.title('Average Times per Instance')
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

    cabecalho = dados[0]
    instancias = [linha[0] for linha in dados[1:]]
    rotulos_x = [extrair_rotulo(instancia) for instancia in instancias]

    tempos = [list(map(float, linha[2:6])) for linha in dados[1:]]
    labels_y = cabecalho[2:6]
    cores = ['b', 'g', 'r', 'c']

    plotar_grafico(rotulos_x, tempos, labels_y, cores)

    nome_arquivo_png = os.path.splitext(arquivo_csv)[0] + '_grafico.png'
    salvar_grafico(rotulos_x, tempos, labels_y, cores, nome_arquivo_png)


if __name__ == "__main__":
    main()
