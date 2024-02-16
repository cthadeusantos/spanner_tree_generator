with open("arquivo_saida.txt", "w") as file_out:
    for i in range(1, 101):  # Variando de 1 a 100
        file_out.write(f"- instance: new_grafos/graph_{i}.txt\n")
        file_out.write("  args: -t 10 -s --edges\n\n")

