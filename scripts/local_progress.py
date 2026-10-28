"""Build a local copy of the progress page from this checkout's objdiff report.

The published page (the gh-pages branch's index.html) carries a snapshot of
`make report`'s progress.json in a <script id="pct-data"> block. This rewrites
that block from build/<version>/progress.json and writes the page with a
version.json beside it. The page polls version.json and reloads itself when the
revision changes, so a served copy refreshes whenever this is run again.

    make report && make build          # report, then the checksum-verified build
    python3 scripts/local_progress.py  # -> build/JP1/site/
    python3 -m http.server -d build/JP1/site 8000

  usage: local_progress.py [--version JP1] [--template-ref origin/gh-pages]
"""
import argparse
import hashlib
import html
import json
import re
import subprocess
from datetime import datetime, timezone
from pathlib import Path

PCT_DATA = re.compile(r'(&lt;script id=&quot;pct-data&quot; type=&quot;application/json&quot;&gt;)(.*?)(&lt;/script&gt;)', re.S)
REVISION = re.compile(r'(const revision = ")([0-9a-f]+)(";)')


def git(*args):
    return subprocess.run(['git', *args], capture_output=True, text=True, check=True).stdout


def verified(version):
    """True when every image named in the checksum file is built and matches."""
    sums = Path(f'configs/{version}/checksum.sha')
    for line in sums.read_text().splitlines():
        if not line.strip():
            continue
        digest, path = line.split(None, 1)
        p = Path(path.strip())
        if not p.exists() or hashlib.sha256(p.read_bytes()).hexdigest() != digest:
            return False
    return True


def status(fn):
    """0 exact, 1 C below 100%, 2 no C - the page's three colours."""
    pct = fn.get('fuzzy_match_percent')
    if pct is None or pct == 0:
        return 2, 0.0
    return (0 if pct >= 100 else 1), round(pct, 5)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--version', default='JP1')
    ap.add_argument('--template-ref', default='origin/gh-pages')
    args = ap.parse_args()

    report = json.loads(Path(f'build/{args.version}/progress.json').read_text())
    template = git('show', f'{args.template_ref}:index.html')
    old = json.loads(html.unescape(PCT_DATA.search(template).group(2)))

    units, counts, sizes = [], [0, 0, 0], [0, 0, 0]
    for u in report['units']:
        name = re.sub(r'^(JP1|US)/', '', u['name'])
        fns = []
        for fn in u.get('functions', []):
            st, pct = status(fn)
            size = int(fn.get('size', 0))
            fns.append([fn['name'], size, pct, st])
            counts[st] += 1
            sizes[st] += size
        units.append({'name': name, 'source': f'src/{name}.c', 'functions': fns})

    data = dict(old)  # keeps the page's port milestones and notes
    data.update({
        'commit': git('rev-parse', 'HEAD').strip(),
        'verified': verified(args.version),
        'date': datetime.now(timezone.utc).replace(microsecond=0).isoformat(),
        'measures': report['measures'],
        'counts': counts,
        'bytes': sizes,
        'units': units,
        'missingPreprocessedUnits': [],
    })
    if git('status', '--porcelain', '--', 'src', 'include', 'configs').strip():
        data['commit'] += '-dirty'

    blob = json.dumps(data, separators=(',', ':'))
    revision = hashlib.sha256(blob.encode()).hexdigest()
    page = PCT_DATA.sub(lambda m: m.group(1) + html.escape(blob, quote=True) + m.group(3), template, count=1)
    page = REVISION.sub(lambda m: m.group(1) + revision + m.group(3), page, count=1)

    out = Path(f'build/{args.version}/site')
    out.mkdir(parents=True, exist_ok=True)
    (out / 'index.html').write_text(page, encoding='utf-8')
    (out / 'version.json').write_text(json.dumps({'revision': revision, 'sourceCommit': data['commit'],
                                                  'generatedAt': data['date']}, indent=2) + '\n')
    m = report['measures']
    print(f"{out}/index.html: {m.get('matched_functions')}/{m.get('total_functions')} functions, "
          f"{float(m.get('matched_code_percent', 0)):.4f}% code, exact/fuzzy/asm = {counts}, "
          f"{'verified' if data['verified'] else 'NOT verified'}")


if __name__ == '__main__':
    main()
