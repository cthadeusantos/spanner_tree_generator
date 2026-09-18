import csv
import subprocess
import re
from decimal import Decimal
INPUT_CSV = "../teste/all_instances_stretch_lowerbound.csv"
OUTPUT_CSV = "inconsistencias.csv"

CPP_PROGRAM = "../build/release/app_SOLVER"

#
# Escolha:
# "--edges"
# "--adjacency"
#


def detect_graph_format(filename):

    with open(filename, "r", encoding="utf-8") as f:

        lines = []

        for line in f:
            line = line.strip()

            if line:
                lines.append(line)

            if len(lines) >= 10:
                break

    #
    # ignora primeira linha (número de vértices)
    #
    sample = lines[1:]

    comma_count = 0
    matrix_count = 0

    for line in sample:

        if "," in line:
            comma_count += 1
            continue

        tokens = line.split()

        if len(tokens) > 3:
            matrix_count += 1

    if comma_count > 0:
        return "--edges"

    if matrix_count > 0:
        return "--adjacencies"

    raise RuntimeError(
        f"Formato desconhecido para {filename}"
    )

def run_cpp(instance_file):

    try:

        cpp_option = detect_graph_format(instance_file)

        with open(instance_file, "r") as fin:

            result = subprocess.run(
                [CPP_PROGRAM, cpp_option],
                stdin=fin,
                capture_output=True,
                text=True,
                timeout=3600
            )

        if result.returncode != 0:
            return None, result.stderr

        output = result.stdout.strip()

        #
        # Ajuste ao formato do programa C++
        #
        # Ex:
        # "3"
        # "stretch = 3"
        # "Best stretch index: 3"
        #
        #m = re.search(r"(\d+(?:\.\d+)?)", output)
        m = re.search(
            r"STRETCH_FACTOR.*?=\s*(\d+(?:\.\d+)?)",
            output,
            re.MULTILINE
        )

        if not m:
            return None, f"Não consegui extrair valor de: {output}"

        return float(m.group(1)), None

    except Exception as e:
        return None, str(e)


errors = []

total = 0
ok = 0

with open(INPUT_CSV, encoding="utf-8") as f:

    reader = csv.DictReader(f)

    for row in reader:

        total += 1

        instance = "../instances/" + row["instance_name"]
        
        expected = float(row["stretch_index"])
        lower_bound = row["lower_bound"]

        calculated, err = run_cpp(instance)

        if err:

            errors.append({
                "instance": instance,
                "lower_bound": lower_bound,
                "expected": expected,
                "calculated": "",
                "difference": "",
                "status": err
            })

            continue

        if calculated is not None and expected is not None:
            diff = calculated - expected
        else:
            diff = Decimal('Infinity')

        if abs(diff) > 1e-9:

            errors.append({
                "instance": instance,
                "lower_bound": lower_bound,
                "expected": expected,
                "calculated": calculated,
                "difference": diff,
                "status": "MISMATCH"
            })

        else:
            ok += 1

with open(
        OUTPUT_CSV,
        "w",
        newline="",
        encoding="utf-8") as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[
            "instance",
            "lower_bound",
            "expected",
            "calculated",
            "difference",
            "status"
        ]
    )

    writer.writeheader()
    writer.writerows(errors)

print(f"Instâncias analisadas : {total}")
print(f"Corretas             : {ok}")
print(f"Inconsistências      : {len(errors)}")
print(f"Relatório            : {OUTPUT_CSV}")