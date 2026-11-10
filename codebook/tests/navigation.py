#!/usr/bin/env python3
"""Check page headers after clean and incremental LaTeX-only builds."""
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import xml.etree.ElementTree as ET
import zipfile

ROOT = Path(__file__).resolve().parents[2]
SOURCE_DIRECTORIES = ("template", "data", "graph", "string", "numeric",
                      "geometry", "dp", "misc")


def compile_book(root):
    previous = None
    for _ in range(6):
        result = subprocess.run(
            ["pdflatex", "-interaction=nonstopmode", "-halt-on-error",
             "codebook/icpc-codebook.tex"], cwd=root, text=True,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        if result.returncode:
            raise AssertionError(result.stdout[-6000:])
        state = tuple((root / f"icpc-codebook.{suffix}").read_bytes()
                      for suffix in ("aux", "toc", "out"))
        if state == previous:
            return
        previous = state
    raise AssertionError("LaTeX references did not stabilize")


def check_headers(root):
    records = re.findall(r"\\RegisterSnippet\{(\d+)\}\{([^}]+)\}",
                         (root / "codebook/navigation.tex").read_text())
    sources = [path for directory in SOURCE_DIRECTORIES
               for path in (root / directory).rglob("*.cpp")]
    sources.append(root / "debug.h")
    assert len(records) == len(sources), "Navigation must include every source file"
    aux = (root / "icpc-codebook.aux").read_text()
    labels = {(kind, index): int(page) for kind, index, page in re.findall(
        r"\\newlabel\{snippet-(start|end)-(\d+)\}\{\{[^}]*\}\{(\d+)\}", aux)}
    expected = {}
    for index, name in records:
        first, last = labels[("start", index)], labels[("end", index)]
        assert 1 <= first <= last
        for page in range(first, last + 1):
            expected.setdefault(page, []).append(name.replace(r"\_", "_"))

    # Inspect rendered header text, independently of the LaTeX navigation code.
    xml = subprocess.check_output(["pdftotext", "-bbox", "icpc-codebook.pdf", "-"],
                                  cwd=root, text=True)
    # Poppler can encode body math delimiters as XML-invalid control characters.
    xml = re.sub(r"[\x00-\x08\x0b\x0c\x0e-\x1f]", "", xml)
    document = ET.fromstring(xml)
    namespace = {"pdf": "http://www.w3.org/1999/xhtml"}
    pages = document.findall(".//pdf:page", namespace)
    for number, page in enumerate(pages, 1):
        if number not in expected:
            continue
        width = float(page.attrib["width"])
        words = [word.text or "" for word in page.findall("pdf:word", namespace)
                 if float(word.attrib["yMax"]) < 35
                 and float(word.attrib["xMin"]) > 0.28 * width]
        actual = "".join(words)
        wanted = ",".join(expected[number])
        assert actual == wanted, f"Page {number}: header {actual!r}, expected {wanted!r}"
    assert max(expected) <= len(pages)
    print(f"Verified all snippet headers on {len(pages)} pages.")
    return expected


def main():
    for tool in ("pdflatex", "pdftotext"):
        if not shutil.which(tool):
            raise SystemExit(f"{tool} is required for navigation checks")
    with tempfile.TemporaryDirectory(prefix="icpc-navigation-") as temporary:
        root = Path(temporary)
        source = root / "source"
        source.mkdir()
        for directory in (*SOURCE_DIRECTORIES, "codebook"):
            shutil.copytree(ROOT / directory, source / directory,
                            ignore=shutil.ignore_patterns("contents.tex", "navigation.tex",
                                                          "__pycache__"))
        for name in ("debug.h", ".clang-format"):
            shutil.copy2(ROOT / name, source / name)
        subprocess.run([sys.executable, "codebook/build.py", "--generate-only"],
                       cwd=source, check=True)
        overleaf = root / "overleaf"
        with zipfile.ZipFile(source / "output/pdf/icpc-codebook-source.zip") as bundle:
            bundle.extractall(overleaf)
        compile_book(overleaf)
        a4 = check_headers(overleaf)

        tex = overleaf / "codebook/icpc-codebook.tex"
        tex.write_text(tex.read_text().replace("a4paper", "letterpaper"))
        compile_book(overleaf)
        letter = check_headers(overleaf)
        assert a4 != letter, "The layout change must exercise different page ranges"

        # The generator includes new sources even before catalog metadata exists.
        with tempfile.NamedTemporaryFile(mode="w", prefix="nav_extra_", suffix=".cpp",
                                         dir=overleaf / "misc", delete=False) as extra:
            extra.write("int navigation_regression_value = 0;\n")
        subprocess.run([sys.executable, "codebook/build.py", "--generate-only"],
                       cwd=overleaf, check=True)
        compile_book(overleaf)
        check_headers(overleaf)
        print("Clean export, A4-to-Letter, and uncatalogued-source navigation checks passed.")


if __name__ == "__main__":
    main()
