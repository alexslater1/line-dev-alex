#!/bin/bash
# Build Sphinx documentation for LINE Solver Python
#
# This script builds the HTML documentation from the Sphinx source files.
#
# Usage:
#   ./build-sphinx.sh

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SPHINX_DIR="$SCRIPT_DIR/config.sphinx"
BUILD_DIR="$SPHINX_DIR/_build"

echo "Building LINE Solver Python documentation..."
echo "Source: $SPHINX_DIR"
echo "Output: $BUILD_DIR"
echo ""

# Clean previous build
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning previous build..."
    rm -rf "$BUILD_DIR"
fi

# Build HTML documentation
echo "Building HTML documentation..."
cd "$SPHINX_DIR"
sphinx-build -b html . _build

echo ""
echo "Documentation build complete!"
echo "Open $BUILD_DIR/index.html in your browser to view the documentation."
