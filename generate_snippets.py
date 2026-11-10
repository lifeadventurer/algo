# Generates cpp.json with snippets for VSCode

import json
import sys
from pathlib import Path

snippets = {}

root = Path(__file__).resolve().parent
for directory in ('template', 'data', 'graph', 'numeric', 'string', 'misc', 'geometry', 'dp'):
  for path in sorted((root / directory).rglob('*.cpp')):
    name = path.stem

    if name in snippets:
      print(f'error: duplicate snippet {name}', file=sys.stderr)
      sys.exit(1)

    with open(path, 'r') as f:
      snippet = {
        "prefix": name,
        "body": [line.rstrip() for line in f.readlines()],
        "description": name
      }
      snippets[name] = snippet

    print(f'generated snippet {name}', file=sys.stderr)

with open(root / 'cpp.json', 'w') as f:
  f.write(json.dumps(snippets, indent=2))

print('done', file=sys.stderr)
