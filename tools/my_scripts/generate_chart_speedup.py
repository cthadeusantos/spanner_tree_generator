import sys
import csv
import os
import matplotlib.pyplot as plt


def read_csv(file_path):
    instances = []
    data = []

    with open(file_path, 'r') as file:
        reader = csv.reader(file)
        header = next(reader)

        for row in reader:
            instances.append(row[0])
            data.append(list(map(float, row[6:9])))

    return instances, data


def plot_grouped_bar_chart(instances, data):
    #plt.figure(figsize=(16, 8))     # Widescreen

    labels = [int(instance.split('-')[-1]) for instance in instances]

    adjacency = [speedup[0] for speedup in data]
    edges = [speedup[1] for speedup in data]
    cycle = [speedup[2] for speedup in data]

    bar_width = 0.25
    index = range(len(labels))

    plt.bar(index, adjacency, color='tab:blue', width=bar_width, label='Adjacency')
    plt.bar([i + bar_width for i in index], edges, color='tab:orange', width=bar_width, label='Edges')
    plt.bar([i + 2 * bar_width for i in index], cycle, color='tab:purple', width=bar_width, label='Cycle')

    plt.xlabel('Instance')
    plt.ylabel('Speedup')
    plt.title('Speedup for parallel approaches - n=15 , m=33')
    plt.xticks([i + bar_width for i in index], labels)
    plt.legend()


def save_and_show_plot(file_path):
    plt.savefig(file_path)
    plt.show()


def main():
    #file_path = input("Enter the file path (including directory if needed): ")
    if len(sys.argv) != 2:
        print("Usage: python script.py <file_path>")
        sys.exit(1)

    file_path = sys.argv[1]
    # Handling different directory notations
    file_path = os.path.normpath(file_path)

    # Read CSV
    instances, data = read_csv(file_path)

    # Plot Grouped Bar Chart
    plot_grouped_bar_chart(instances, data)

    # Save and Show Plot
    output_file = os.path.splitext(os.path.basename(file_path))[0] + "_speedup.png"
    save_and_show_plot(output_file)


if __name__ == "__main__":
    main()
