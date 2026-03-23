#!/bin/bash

# LINE Solver Post-Release Script
# Usage: ./post-release.sh <version> [pypi-token]
# Example: ./post-release.sh 3.0.50 pypi-xxxxxxxxxxxxx
#
# This script:
# 1. Optionally publishes the Python package to PyPI using poetry
# 2. Runs diagnostics to verify the release succeeded

set -e

VERSION=$1
PYPI_TOKEN=$2

if [ -z "$VERSION" ]; then
    echo "Usage: $0 <version> [pypi-token]"
    echo "Example: $0 3.0.50 pypi-xxxxxxxxxxxxx"
    echo "If pypi-token is omitted, PyPI publishing is skipped."
    exit 1
fi

echo "================================================"
echo "LINE Solver Post-Release - Version $VERSION"
echo "================================================"
echo ""

if [ -n "$PYPI_TOKEN" ]; then
    # Change to python native directory (PyPI package, no JAR dependency)
    cd python

    # Check if version matches pyproject.toml (python/ uses 4-digit: X.Y.Z.0)
    PY_VERSION="${VERSION}.0"
    TOML_VERSION=$(grep '^version = ' pyproject.toml | cut -d'"' -f2)
    if [ "$TOML_VERSION" != "$PY_VERSION" ]; then
        echo "Warning: Version mismatch!"
        echo "  Requested version: $PY_VERSION"
        echo "  pyproject.toml version: $TOML_VERSION"
        echo ""
        read -p "Update pyproject.toml to version $PY_VERSION? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            sed -i "s/^version = \".*\"/version = \"$PY_VERSION\"/" pyproject.toml
            echo "Updated pyproject.toml to version $PY_VERSION"
            git add pyproject.toml
            git commit -m "p chore: bump version to $PY_VERSION"
            echo "Committed version update"
        else
            echo "Please update pyproject.toml manually and re-run this script"
            exit 1
        fi
    fi

    # echo ""
    # echo "Step 1: Configuring PyPI token..."
    # poetry config pypi-token.pypi "$PYPI_TOKEN"
    # echo "PyPI token configured (stored in poetry config)"

    # echo ""
    # echo "Step 2: Building Python wheel with LDES native binary..."
    # ./build-wheel.sh
    # echo "Build complete: wheel with LDES binary created in dist/"

    # echo ""
    # echo "Step 3: Publishing to PyPI..."
    # read -p "Publish to PyPI? This cannot be undone. (y/N): " -n 1 -r
    # echo
    # if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    #     echo "Publication cancelled."
    #     echo "Build artifacts remain in python/dist/"
    #     exit 0
    # fi

    # poetry publish --no-build
    # echo "Published to PyPI successfully!"

    # echo ""
    # echo "================================================"
    # echo "PyPI Release Complete!"
    # echo "================================================"
    # echo ""
    # echo "Package available at:"
    # echo "  https://pypi.org/project/line-solver/$VERSION/"
    # echo ""
    # echo "Installation command:"
    # echo "  pip install line-solver==$VERSION"
    # echo ""
else
    # Bump version to next minor release
    MAJOR=$(echo "$VERSION" | cut -d. -f1)
    MINOR=$(echo "$VERSION" | cut -d. -f2)
    PATCH=$(echo "$VERSION" | cut -d. -f3)
    NEXT_PATCH=$((PATCH + 1))
    NEXT_VERSION="${MAJOR}.${MINOR}.${NEXT_PATCH}"

    echo "Bumping version: $VERSION -> $NEXT_VERSION"
    echo ""

    # JAR GlobalConstants.java
    GC_FILE="jar/src/main/kotlin/jline/GlobalConstants.java"
    if [ -f "$GC_FILE" ]; then
        sed -i "s/Version = \"$VERSION\"/Version = \"$NEXT_VERSION\"/" "$GC_FILE"
        echo "  Updated $GC_FILE to $NEXT_VERSION"
    fi

    # MATLAB lineStart.m
    LS_FILE="matlab/lineStart.m"
    if [ -f "$LS_FILE" ]; then
        sed -i "s/LINE_VERSION     = '$VERSION'/LINE_VERSION     = '$NEXT_VERSION'/" "$LS_FILE"
        echo "  Updated $LS_FILE to $NEXT_VERSION"
    fi

    # Sphinx conf.py
    SPHINX_FILE="doc/config.sphinx/conf.py"
    if [ -f "$SPHINX_FILE" ]; then
        sed -i "s/release = '$VERSION'/release = '$NEXT_VERSION'/" "$SPHINX_FILE"
        echo "  Updated $SPHINX_FILE to $NEXT_VERSION"
    fi

    # python-wrapper/pyproject.toml (3-digit: X.Y.Z)
    if [ -f "python-wrapper/pyproject.toml" ]; then
        sed -i "s/^version = \"$VERSION\"/version = \"$NEXT_VERSION\"/" "python-wrapper/pyproject.toml"
        echo "  Updated python-wrapper/pyproject.toml to $NEXT_VERSION"
    fi

    # python/pyproject.toml (4-digit: X.Y.Z.0)
    if [ -f "python/pyproject.toml" ]; then
        sed -i "s/^version = \"${VERSION}\.[0-9]*\"/version = \"${NEXT_VERSION}.0\"/" "python/pyproject.toml"
        echo "  Updated python/pyproject.toml to ${NEXT_VERSION}.0"
    fi

    echo ""
    echo "================================================"
    echo "Version bumped to $NEXT_VERSION"
    echo "================================================"
    echo ""
fi
