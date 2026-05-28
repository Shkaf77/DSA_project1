import csv
import os
import matplotlib.pyplot as plt

INPUT_FILE = "results.csv"
OUTPUT_DIR = "graphs"

def read_results(filename):
    data = {}

    with open(filename, "r", newline="") as f:
        reader = csv.reader(f)

        for row in reader:
            if len(row) != 5:
                continue

            structure = row[0].strip()
            scenario = row[1].strip()

            n = int(row[2].strip())

            operation = row[3].strip()
            microseconds = float(row[4].strip())

            key = (scenario, operation, structure)

            if key not in data:
                data[key] = []

            data[key].append((n, microseconds))

    return data

def plot_graphs(data):
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    scenarios = sorted({key[0] for key in data.keys()})
    operations = sorted({key[1] for key in data.keys()})
    structures = sorted({key[2] for key in data.keys()})

    for scenario in scenarios:
        for operation in operations:
            plt.figure(figsize=(10, 6))

            for structure in structures:
                key = (scenario, operation, structure)

                if key not in data:
                    continue

                points = sorted(data[key], key=lambda x: x[0])
                x_values = [p[0] for p in points]
                y_values = [p[1] for p in points]

                plt.plot(x_values, y_values, marker="o", label=structure)

            plt.title(f"{scenario} - {operation}")
            plt.xlabel("Number of elements (n)")
            plt.ylabel("Average time per operation (microseconds)")
            plt.legend()
            plt.grid(True)

            filename = f"{scenario.lower()}_{operation.lower()}.png"
            path = os.path.join(OUTPUT_DIR, filename)
            plt.savefig(path, dpi=300, bbox_inches="tight")
            plt.close()

def main():
    data = read_results(INPUT_FILE)

    plot_graphs(data)

    print(f"Graphs saved to folder: {OUTPUT_DIR}")

if __name__ == "__main__":
    main()