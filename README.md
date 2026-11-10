# algo

Personal competitive programming toolkit with C++ templates, data structures,
numeric utilities, and VS Code snippets.

## Overview

This repo collects the reusable pieces I keep around for programming contests:

- a base C++ template for fast setup
- common data structures and helpers
- numeric utilities for modular arithmetic and exponentiation
- IO/debug utilities
- a script to generate VS Code snippets from the `.cpp` files

The code is organized as small copy-pasteable building blocks rather than a
single library.

## Structure

- `template/` - contest templates
- `data/` - data structures such as DSU, Fenwick tree, segment tree, PBDS, and
  hashing helpers
- `graph/` - graph algorithms such as BFS, Dijkstra, and LCA
- `numeric/` - modular arithmetic, fast power, extended gcd
- `string/` - string algorithms such as Z-function
- `misc/` - fast IO, pragma helpers, RNG, and other utilities
- `geometry/` - exact integer and floating-point geometry
- `dp/` - LIS, Li Chao tree, and subset transforms
- `codebook/` - printable LaTeX ICPC reference, usage notes, and validation
- `debug.h` - local debugging helpers
- `generate_snippets.py` - builds a `cpp.json` snippet file for VS Code

## Snippet Workflow

Each `.cpp` file is treated as a snippet source. To generate a VS Code snippet file:

```bash
python3 generate_snippets.py
```

This writes `cpp.json` in the repo root, with one snippet per `.cpp` file.

## Notes

- Most files are intended to be pasted into contest solutions as needed.
- The template expects `debug.h` when compiling locally with `LOCAL` defined.
- Some snippets include placeholders meant to be filled in before use.

## ICPC Codebook

Build the complete two-column A4 reference with every template, usage notes,
formulas, and contest reminders:

```bash
python3 codebook/build.py
```

The PDF and an Overleaf-ready LaTeX source ZIP are written to `output/pdf/`.
Requires Python 3 and pdfLaTeX. See [codebook/README.md](codebook/README.md) for
customization, dependencies, and algorithm verification.
