import datetime
import json
import re
import urllib.request
import csv

DAYS_BACK = 30
MAX_STORIES_TOTAL = 2000 

HEADLINES_OUT = "hn_week.txt"
COUNTS_OUT = "hn_word_counts.csv"

BASE = "https://hacker-news.firebaseio.com/v0"
TOP_URL = f"{BASE}/topstories.json"
NEW_URL = f"{BASE}/newstories.json"
ITEM_URL = f"{BASE}/item/{{}}.json"

# Practical stopwords (edit freely)
STOPWORDS = {
    "a","an","and","are","as","at","be","been","but","by",
    "can","could","did","do","does","doing","done",
    "for","from","had","has","have","having",
    "he","her","hers","him","his",
    "how","i","if","in","into","is","it","its","just",
    "may","me","more","most","my","no","not","of","on","or","our","out","over",
    "she","so","some","than","that","the","their","them","then","there","these","they","this",
    "to","too","under","up","us","was","we","were","what","when","where","which","who","why",
    "will","with","you","your","yours", "hn", "show", "ask", "tell"
}

def fetch_json(url):
    req = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
    with urllib.request.urlopen(req, timeout=25) as resp:
        return json.loads(resp.read().decode("utf-8", errors="replace"))


def tokenize(title):
    title = title.lower()
    title = re.sub(r"[^a-z0-9]+", " ", title)

    words = []
    for raw in title.split():
        if raw in STOPWORDS:
            continue
        if len(raw) < 2:
            continue

        # drop pure numbers
        all_digits = True
        for ch in raw:
            if ch < "0" or ch > "9":
                all_digits = False
                break
        if all_digits:
            continue

        words.append(raw)

    return words


def main():
    now = datetime.datetime.now().astimezone()
    cutoff_ts = int((now - datetime.timedelta(days=DAYS_BACK)).timestamp())

    top_ids = fetch_json(TOP_URL)[:MAX_STORIES_TOTAL]
    new_ids = fetch_json(NEW_URL)[:MAX_STORIES_TOTAL]

    ids = []
    seen = set()
    for x in top_ids + new_ids:
        if x not in seen:
            seen.add(x)
            ids.append(x)

    headlines = []
    counts = {}
    
    print("Fetching ")

    for sid in ids:
        item = fetch_json(ITEM_URL.format(sid))
        if not item:
            continue
        if item.get("type") != "story":
            continue
        if item.get("time", 0) < cutoff_ts:
            continue

        title = item.get("title", "")
        if not title:
            continue

        title_clean = " ".join(title.split())
        headlines.append(title_clean)

        for w in tokenize(title_clean):
            counts[w] = counts.get(w, 0) + 1
            
    print("Fetched headlines")

    headlines = list(dict.fromkeys(headlines))

    with open(HEADLINES_OUT, "w", encoding="utf-8") as f:
        for t in headlines:
            f.write(t + "\n")

    items = list(counts.items())
    items.sort(key=lambda x: (-x[1], x[0]))

    with open(COUNTS_OUT, "w", encoding="utf-8", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["word", "count"])
        for w, c in items:
            writer.writerow([w, c])

    print(f"Wrote {len(headlines)} headlines to {HEADLINES_OUT}")
    print(f"Wrote {len(items)} unique words to {COUNTS_OUT}")


if __name__ == "__main__":
    main()
