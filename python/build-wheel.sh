#!/bin/bash

# Build line-solver wheel with LDES native binary bundled.
#
# Usage: ./build-wheel.sh [--publish]
#   --publish: Also upload the wheel to PyPI after building
#
# The resulting wheel includes:
#   - ldes native binary (for fast LDES solver startup, no JVM needed)
#   - ldes.jar is NOT bundled — auto-downloaded at runtime if native binary unavailable
#
# Prerequisites:
#   - LDES native binary must be built first (run upload-ldes.sh in line-dev.git)
#   - poetry must be installed

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"
BIN_DIR="$SCRIPT_DIR/line_solver/bin"
LDES_DEV_DIR="$REPO_ROOT/../ldes-dev.git"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Ensure bin/ directory exists
mkdir -p "$BIN_DIR"

# Find native binary
LDES_NATIVE=""
for candidate in \
    "$LDES_DEV_DIR/target/ldes" \
    "$REPO_ROOT/common/ldes"; do
    if [ -f "$candidate" ] && [ -x "$candidate" ]; then
        LDES_NATIVE="$candidate"
        break
    fi
done

if [ -z "$LDES_NATIVE" ]; then
    echo -e "${RED}Error: LDES native binary not found. Build it first:${NC}"
    echo "  cd $REPO_ROOT && ./upload-ldes.sh"
    exit 1
fi

cp "$LDES_NATIVE" "$BIN_DIR/ldes"
chmod 755 "$BIN_DIR/ldes"
echo -e "${GREEN}Bundled:${NC} ldes native binary ($(du -h "$BIN_DIR/ldes" | cut -f1))"

# Build wheel
echo
echo -e "${GREEN}Building wheel...${NC}"
cd "$SCRIPT_DIR"
poetry build -f wheel

echo
echo -e "${GREEN}Wheel built:${NC}"
ls -lh dist/*.whl 2>/dev/null

# Verify binary is in the wheel
VERSION=$(grep '^version' pyproject.toml | head -1 | sed 's/.*"\(.*\)"/\1/')
WHEEL="dist/line_solver-${VERSION}-py3-none-any.whl"
if [ -f "$WHEEL" ]; then
    if python3 -m zipfile -l "$WHEEL" | grep -q "bin/ldes"; then
        echo -e "${GREEN}Verified: ldes binary present in wheel${NC}"
    else
        echo -e "${RED}Error: ldes binary NOT found in wheel!${NC}"
        rm -f "$BIN_DIR/ldes"
        exit 1
    fi
fi

# Clean up bundled binary (it lives in ldes-dev.git, not here)
rm -f "$BIN_DIR/ldes"
echo -e "${GREEN}Cleaned up bundled binary from source tree${NC}"

# Publish if requested
if [ "$1" = "--publish" ]; then
    echo
    echo -e "${YELLOW}Publishing to PyPI...${NC}"
    poetry publish
fi
