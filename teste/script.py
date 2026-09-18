import re
import csv
from pathlib import Path

INPUT_FILES = [
    "result_summary_grafos.txt",
    "result_summary_examples.txt",
    "result_summary_new_grafos.txt",
    "result_summary_jupiter.txt",
]

OUTPUT_FILE = "all_instances_stretch_lowerbound.csv"


def clean_instance_name(name):
    return re.sub(r"^[!+#*]+", "", name.strip())


def extract_row(line):
    """
    Extrai:
      instance_name
      stretch_index
      lower_bound

    de uma linha da tabela.
    """

    if not line.strip().startswith("|"):
        return None

    # separa pelas barras da tabela
    parts = [p.strip() for p in line.split("|")]

    # remove vazios
    parts = [p for p in parts if p]

    if len(parts) < 9:
        return None

    instance_name = clean_instance_name(parts[0])

    #
    # Localiza o campo stretch:
    # exemplo: "3 (3/3) [0]"
    #
    stretch_index = None

    for field in parts:
        m = re.fullmatch(
            r"([0-9]+(?:\.[0-9]+)?)\s*\([^)]*\)\s*\[[^]]*\]",
            field
        )

        if m:
            stretch_index = m.group(1)

    #
    # Lower Bound é sempre o penúltimo valor numérico
    # antes de "Total trees"
    #
    numeric_fields = []

    for field in parts:
        if re.fullmatch(r"-?\d+", field):
            numeric_fields.append(field)

    if len(numeric_fields) < 2:
        return None

    lower_bound = numeric_fields[-2]

    return {
        "instance_name": instance_name,
        "stretch_index": stretch_index,
        "lower_bound": lower_bound
    }


rows = []

for filename in INPUT_FILES:
    print(f"Try reading: {filename}")
    path = Path(filename)

    if not path.exists():
        print(f"Arquivo não encontrado: {filename}")
        continue
    print(f"Arquivo lido: {filename}")
    with open(path, "r", encoding="utf-8", errors="ignore") as f:

        for line in f:

            # ignora cabeçalho
            if "Instance Name" in line:
                continue

            if "stretch index" in line.lower():
                continue

            row = extract_row(line)

            if row:
                rows.append(row)

# remove duplicados
seen = set()
unique_rows = []

for row in rows:

    key = (
        row["instance_name"],
        row["stretch_index"],
        row["lower_bound"]
    )

    if key not in seen:
        seen.add(key)
        unique_rows.append(row)

with open(
    OUTPUT_FILE,
    "w",
    newline="",
    encoding="utf-8"
) as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[
            "instance_name",
            "stretch_index",
            "lower_bound",
        ]
    )

    writer.writeheader()
    writer.writerows(unique_rows)

print(f"Registros extraídos: {len(unique_rows)}")
print(f"Arquivo salvo em: {OUTPUT_FILE}")