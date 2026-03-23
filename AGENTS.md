# Repository Guidelines

## Project Structure & Module Organization
This repository is split by runtime. `jar/` contains the main Java/Kotlin solver (`src/main/kotlin`) and JUnit tests (`src/test/java`). `python/` is the native Python package (`line_solver/`) with pytest suites in `tests/` and solver-focused tests in `line_solver/tests/`. `python-wrapper/` mirrors that layout for the JPype-based wrapper. `rest-api/` contains the SparkJava server in `src/main/java/jline/rest` and tests in `src/test/java`. `matlab/` holds the MATLAB source in `src/`, examples in `examples/`, and bundled third-party libraries in `lib/`. Built JVM artifacts are shared through `common/`.

## Build, Test, and Development Commands
Use the command that matches the module you are touching:

- `mvn clean package` from the repo root builds the bundled JAR by delegating to `jar/`.
- `cd jar && mvn test -DskipTests=false` runs JVM tests; add `-DexcludedGroups=` to include slow tests.
- `cd python && poetry install && ./run_tests.sh --quick` runs native Python tests quickly.
- `cd python-wrapper && poetry install && ./run_tests_wrapper.sh -k gettingstarted` runs wrapper tests selectively.
- `cd rest-api && mvn clean package` builds the REST server; `common/jline.jar` must exist first.
- `python line-cli.py info` checks the standalone CLI locally.

## Coding Style & Naming Conventions
Use 4-space indentation across Python, Java/Kotlin, and MATLAB. Follow PEP 8 in Python: `snake_case` for modules and functions, `PascalCase` for classes. In Java/Kotlin, keep package names lowercase, classes `PascalCase`, and tests named `*Test.java`. For public JVM APIs, add Javadoc. Follow [`jar/STYLE_GUIDE.md`](jar/STYLE_GUIDE.md): use JUnit 5, avoid `System.out`, and do not change `jar/pom.xml` or `Matrix.java` without maintainer approval.

## Testing Guidelines
Add or update tests with every behavior change. JVM changes belong in `jar/src/test/java`; REST changes in `rest-api/src/test/java`; Python changes in `python/tests/`, `python-wrapper/tests/`, or module-local solver tests. Use `test_*.py` for pytest files and descriptive test names that reflect the model or solver under test. No enforced coverage threshold is declared, so contributors should add focused regression coverage for touched code paths.

## Commit & Pull Request Guidelines
Recent history uses short conventional subjects such as `fix: ...`, `feat: ...`, and `chore: ...`, sometimes prefixed by area tags like `j`, `p`, or `m`. Keep commit titles imperative and module-scoped when possible. Pull requests should summarize affected modules, list the exact commands run, link related issues or manual-gap notes, and include sample output or screenshots for CLI, REST, or documentation-visible changes.
