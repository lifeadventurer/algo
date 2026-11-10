#!/usr/bin/env python3
"""Compile every template, then run deterministic and randomized C++ checks."""
import os
import argparse
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent.parent
DIRECTORIES = ("template", "data", "graph", "numeric", "string", "misc", "geometry", "dp")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sanitize", action="store_true",
                        help="Run the checks with undefined-behavior sanitization")
    args = parser.parse_args()
    if os.environ.get("CXX"):
        compiler = shlex.split(os.environ["CXX"])
    else:
        candidates = ["g++", *[f"g++-{v}" for v in range(16, 10, -1)],
                      "/opt/homebrew/opt/gcc/bin/g++-16"]
        compiler = None
        for c in candidates:
            if shutil.which(c):
                probe = subprocess.run([c, "-std=c++17", "-x", "c++", "-fsyntax-only", "-"],
                                       input="#include <bits/stdc++.h>\n#include <ext/pb_ds/assoc_container.hpp>\n",
                                       text=True, capture_output=True)
                if probe.returncode == 0:
                    compiler = [c]
                    break
        if compiler is None:
            raise SystemExit("GNU C++17 with libstdc++/PBDS is required; set CXX=g++-VERSION.")
    flags = ["-std=c++17", "-O2", "-Wall", "-Wextra", "-I", str(ROOT)]
    if args.sanitize:
        flags += ["-O1", "-g", "-fsanitize=undefined", "-fno-sanitize-recover=all"]
    deps = {"graph/two_sat.cpp": '#include "graph/scc.cpp"\n',
            "graph/kruskal.cpp": '#include "data/dsu.cpp"\n',
            "numeric/pollard_rho.cpp": '#include "numeric/miller_rabin.cpp"\n',
            "graph/virtual_tree.cpp": '#include "graph/hld.cpp"\n',
            "geometry/closest_pair.cpp": '#include "geometry/integer_geometry.cpp"\n',
            "geometry/polygon_diameter.cpp": '#include "geometry/integer_geometry.cpp"\n',
            "geometry/min_enclosing_circle.cpp": '#include "geometry/floating_geometry.cpp"\n'}
    sources = sorted(p for d in DIRECTORIES for p in (ROOT / d).rglob("*.cpp"))
    sources.append(ROOT / "debug.h")
    with tempfile.TemporaryDirectory(prefix="icpc-check-") as directory:
        tmp = Path(directory)
        for path in sources:
            relative = path.relative_to(ROOT).as_posix()
            text = path.read_text()
            if relative == "template/basic.cpp":
                text = text.replace("${0}", "")
            if relative == "numeric/mint.cpp":
                text = text.replace("${0}", "1000000007")
            text = re.sub(r"\$CURRENT_[A-Z_]+", "0", text)
            unit = tmp / "snippet.cc"
            unit.write_text("#include <bits/stdc++.h>\n#include <cassert>\nusing namespace std;\n"
                            + deps.get(relative, "") + text)
            result = subprocess.run(compiler + flags + ["-fsyntax-only", str(unit)],
                                    text=True, capture_output=True)
            if result.returncode:
                raise SystemExit(f"Compilation failed: {relative}\n{result.stderr}")
        print(f"Syntax checked all {len(sources)} sources with {' '.join(compiler)}.", flush=True)
        for name in ("algorithms", "new_topics"):
            binary = tmp / name
            subprocess.run(compiler + flags + [str(ROOT / f"codebook/tests/{name}.cc"),
                                              "-o", str(binary)], check=True)
            for seed in (20261106, 712367821, 998244353):
                subprocess.run([str(binary), str(seed)], check=True, timeout=120)


if __name__ == "__main__":
    main()
