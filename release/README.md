# LINE Solver Release Scripts

## Scripts

| Script | Purpose |
|--------|---------|
| `pre-release.sh` | Prepares release: builds JAR, creates archives, generates changelog |
| `release.sh` | Uploads to SourceForge |
| `post-release.sh` | Publishes to PyPI, bumps version |

## Workflow

```bash
./pre-release.sh 3.0.50              # Build, create archives
./release.sh 3.0.50                  # Upload to SourceForge
./post-release.sh 3.0.50 <token>     # Publish to PyPI
```

## Release Archives

Each release produces four zip files:

| Archive | Contents | Audience |
|---------|----------|----------|
| `line-<ver>.zip` | Everything (common, doc, jar, python, matlab) | Full distribution |
| `line-<ver>-jar.zip` | `jar/`, `common/jline.jar`, Java/Kotlin docs | Java/Kotlin users |
| `line-<ver>-python.zip` | `python/`, Python docs | Native Python users |
| `line-<ver>-matlab.zip` | `matlab/`, `common/jline.jar`, MATLAB docs | MATLAB users |

All archives include: LICENSE, README, NOTICE, AUTHORS, CONTRIBUTING, and the LINE cheatsheet PDF.

## Details

### pre-release.sh

```bash
./pre-release.sh <version> [from-hash]
```

1. Updates version numbers across all codebases
2. Builds JAR package with bundled dependencies
3. Deploys to Maven repository
4. Builds LDES JAR + native binary (if ldes-dev.git available)
5. Stages a clean copy of the repository
6. Creates four release archives (uber + jar + python + matlab)
7. Generates changelog and release highlights from git commits

### release.sh

```bash
./release.sh <version>
```

1. Uploads all release archives to SourceForge
2. Uploads PDF documentation
3. Syncs to line-solver.git (GitHub mirror)

### post-release.sh

```bash
./post-release.sh <version> <pypi-token>
```

1. Verifies version in `pyproject.toml`
2. Publishes to PyPI (if token provided)
3. Bumps version to next patch release (if no token)
