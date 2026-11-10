# ICPC codebook

The full book contains every C++ template in this repository and `debug.h`,
plus contest notes, formulas, and reductions. Listings use the current sources;
the original buffered IO templates are preserved unchanged. Notes above each
listing explain placeholders and usage assumptions. Added
templates use C++17, 0-based indexing, and half-open ranges
unless their notes specify otherwise. Paste selected snippets into the starter,
not the entire book into one translation unit.

From the repository root:

```sh
python3 codebook/build.py
```

Outputs:

- `output/pdf/icpc-codebook.pdf` - A4, two columns, 8.4 pt code, numbered pages,
  with subtle blue keywords, red italic comments, and green strings/characters;
  clickable page headers list the snippets printed on each page
- `output/pdf/icpc-codebook-source.zip` - portable LaTeX and all printed sources

Requires Python 3 and `pdflatex` (TeX Live or MacTeX). Standard packages:
geometry, listings, lmodern, multicol, xcolor, fancyhdr, titlesec, hyperref,
amsmath, amssymb, refcount, microtype, and textcomp. No shell escape or minted needed.
Run `python3 codebook/build.py --generate-only` if LaTeX is not installed.
The script works from any working directory.

The quick reference includes a minimal Vim setup, editing commands, and compile/run
commands. `codebook/contest.vimrc` uses two-space indentation without plugins,
and leaves namespace contents unindented to match the templates.
`set number` is the same as `set nu`. `scrolloff=3` keeps context around the
cursor, and `wildmenu` shows command-line completion choices.
Try it with `vim -u codebook/contest.vimrc main.cpp`, or copy the settings into
`~/.vimrc` yourself. Building the book does not change your editor configuration.

For Overleaf, upload the source ZIP and set the main document to
`codebook/icpc-codebook.tex`, using pdfLaTeX. The archive has the same directory
layout as the repository, so listing paths continue to work.
Page headers resolve snippet page ranges from LaTeX labels, including in
`--generate-only` exports. Overleaf updates them automatically after layout
changes. For manual builds, rerun pdfLaTeX until reference warnings disappear.

Customize `icpc-codebook.tex` for margins, fonts, A4/Letter, and the
`codeblue`/`codered`/`codegreen` syntax-color palette.
Each listing has a description, usage example, time complexity, source line
count, and the first 12 hexadecimal characters of its SHA-256 fingerprint.
The fingerprint covers the exact source bytes, including whitespace and comments.
To compare a local source against the printed version:

```sh
python3 codebook/build.py --fingerprint graph/hungarian.cpp
```

Edit titles, complexity, preconditions, and usage examples in `catalog.json`
(four strings per entry, in that order). Editing a C++ file
changes the next PDF directly. New `.cpp` files in template/data/graph/string/
numeric/geometry/dp/misc are included automatically, with a reminder to add
catalog metadata. No page cap is enforced.
Rerun the Python generator to refresh source fingerprints, line counts, or the
catalog after changing C++ sources; page navigation needs only LaTeX recompilation.

The added templates follow the existing tourist-style layout: two-space
indentation, attached braces, spaces around operators, explicit loop bodies,
postfix loop increments, and separate statements. `.clang-format` records the
spacing and brace rules. To format one new template, use
`clang-format -i path/to/template.cpp`.

## Validation

```sh
python3 codebook/verify.py
python3 codebook/verify.py --sanitize
# If needed, choose a GNU compiler (PBDS requires libstdc++):
CXX=g++-16 python3 codebook/verify.py
```

The verifier compiles every snippet separately, replacing the two existing
editor placeholders only in temporary compilation inputs. It runs seeded
randomized comparisons against brute-force oracles using three fixed seeds,
plus deterministic edge cases and large-arithmetic boundaries for the new
algorithms, including the lazy segment tree's wide update and query
intermediates. The original buffered IO receives syntax checks only; runtime
tests cover the added algorithms. `--sanitize` enables undefined-behavior
checks for the algorithm suite.
The separate new-topic suite checks assignment against exhaustive matchings,
recurrences against direct generation, binary systems against every assignment,
string rotations against enumeration, virtual trees against pairwise LCAs,
and geometry against exhaustive point-pair and candidate-circle oracles.
This is a practical local check, not a proof or an online-judge acceptance
claim. The standard iostream starter is the default.

To check the portable LaTeX navigation workflow (requires pdfLaTeX and Poppler):

```sh
python3 codebook/tests/navigation.py
```

This builds a fresh `--generate-only` export using only pdfLaTeX, then changes
the layout from A4 to Letter and verifies every snippet header against the
rendered document's page references. It also adds a temporary template without
catalog metadata and verifies that the navigation check includes it.

## References

The additions were written here from standard algorithms. Explanations and
cross-check references include [Dinic](https://cp-algorithms.com/graph/dinic.html),
[Miller-Rabin](https://cp-algorithms.com/algebra/primality_tests.html),
[NTT](https://cp-algorithms.com/algebra/fft.html), and
[KACTL](https://github.com/kth-competitive-programming/kactl) for broader coverage.
The [MIT NULL 2019 book](https://github.com/ecnerwala/icpc-book/blob/master/kactl.pdf)
inspired the usage metadata, source fingerprints, and per-page navigation.
Additional cross-checks include
[Hungarian assignment](https://cp-algorithms.com/graph/hungarian-algorithm.html) and
[minimum enclosing circles](https://cp-algorithms.com/geometry/enclosing-circle.html).
No third-party implementation files are vendored.
