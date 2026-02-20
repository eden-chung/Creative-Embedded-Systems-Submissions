import csv

CSV_IN = "hn_word_counts.csv"
OUT_H = "words.h"

MAX_WORDS = 300 
MIN_COUNT = 4

def main():
    rows = []
    with open(CSV_IN, newline="", encoding="utf-8") as f:
        r = csv.DictReader(f)
        for row in r:
            w = row["word"].strip()
            c = int(row["count"])
            if c < MIN_COUNT:
                continue
            if w:
                rows.append((w, c))

    rows.sort(key=lambda x: -x[1])
    rows = rows[:MAX_WORDS]

    with open(OUT_H, "w", encoding="utf-8") as f:
        f.write("// Auto-generated from hn_word_counts.csv\n")
        f.write("#pragma once\n")
        f.write("#include <Arduino.h>\n\n")

        f.write(f"static const uint16_t WORD_COUNT = {len(rows)};\n\n")

        for i, (w, c) in enumerate(rows):
            safe = w.replace("\\", "\\\\").replace('"', '\\"')
            f.write(f'static const char w_{i}[] PROGMEM = "{safe}";\n')
        f.write("\n")

        f.write("static const char* const WORDS[] PROGMEM = {\n")
        for i in range(len(rows)):
            f.write(f"  w_{i},\n")
        f.write("};\n\n")

        f.write("static const uint16_t WEIGHTS[] PROGMEM = {\n")
        for (w, c) in rows:
            f.write(f"  {c},\n")
        f.write("};\n")

    print(f"Wrote {OUT_H} with {len(rows)} words.")

if __name__ == "__main__":
    main()
