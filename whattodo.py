#!/usr/bin/python3


from pathlib import Path
from concurrent.futures import ThreadPoolExecutor


def chop_line(line: str, at: int):
    if at >= len(line):
        return line, ''

    split_pos = line.rfind(' ', 0, at)
    if split_pos == -1:
        split_pos = at

    remaining = line[:split_pos].rstrip()
    chopped = line[split_pos:].lstrip()

    return remaining, chopped


def search_file(path):
    with open(path, "r", encoding="utf-8", errors="ignore") as file:
        lines = list(map(str.strip, file.readlines()))
        lines_size = len(lines)

        index = 0

        while index < lines_size:
            line: str = lines[index]
            lineno = index + 1

            if not line.startswith("//"):
                index = index + 1
                continue

            line = line[2::].strip()

            if not line.startswith("TODO:"):
                index = index + 1
                continue

            body = [line[5::].strip()]

            index = index + 1

            while index < lines_size:
                line = lines[index]

                if not line.startswith("//"):
                    break

                line = line[2::].strip()

                if line:
                    body.append(line)

                index = index + 1

            indent = f"{path}:{lineno}: "

            i = 0

            while i < len(body):
                body[i], chopped = chop_line(body[i], 80 - len(indent))

                if chopped:
                    body.insert(i + 1, chopped)

                i = i + 1

            formatted_body = ("\n" + " " * len(indent)).join(body)

            print(f"\033[96m{indent}\033[0m{formatted_body}")
            print()


def main():
    root_dir = Path("src")
    files = [f for f in root_dir.rglob("*") if f.is_file()]

    with ThreadPoolExecutor() as executor:
        executor.map(search_file, files)


if __name__ == "__main__":
    main()

