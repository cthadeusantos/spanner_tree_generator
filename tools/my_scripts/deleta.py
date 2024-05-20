import os

def deletar_arquivos(diretorio, parametro):
    # Obtém a lista de arquivos no diretório
    arquivos = os.listdir(diretorio)

    # Itera sobre os arquivos no diretório
    for arquivo in arquivos:
        caminho_completo = os.path.join(diretorio, arquivo)

        # Verifica se é um arquivo (não é um diretório)
        if os.path.isfile(caminho_completo):
            # Verifica se o nome do arquivo não corresponde ao parâmetro
            if parametro not in arquivo:
                # Se não corresponder, exclui o arquivo
                os.remove(caminho_completo)
                print(f"Arquivo excluído: {caminho_completo}")

if __name__ == "__main__":
    # Obtém o diretório do usuário
    diretorio = input("Digite o caminho do diretório: ")

    # Obtém o parâmetro do usuário
    parametro = input("Digite o nome do arquivo a ser mantido: ")

    # Verifica se o diretório existe
    if os.path.exists(diretorio):
        deletar_arquivos(diretorio, parametro)
        print("Processo concluído.")
    else:
        print("Diretório não encontrado.")

