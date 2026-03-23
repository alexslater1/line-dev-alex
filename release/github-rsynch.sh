#!/bin/bash
#
set -e

# Require a version argument
if [ -z "$1" ]; then
    echo "Usage: $0 <version>"
         exit 1
fi

VERSION="$1"

SRC=$(cd "$(dirname "$0")/../../line-dev.git" && pwd)
DST=$(cd "$(dirname "$0")/../../line-solver.git" && pwd)

HASH=$(git -C "$SRC" rev-parse --short HEAD)

# Sync all files (existing + new), excluding dev-only directories
rsync -av --delete \
    --exclude='.git/' \
    --exclude='.claude/' \
    --exclude='release/' \
    --exclude='parity/' \
    --exclude='.pytest_cache/' \
    --exclude='__pycache__/' \
    --exclude='CLAUDE.md' \
    --exclude='common/JMT.jar' \
    --exclude='common/ldes.jar' \
    --exclude='common/ldes' \
    --exclude='common/line-viewer.jar' \
    "$SRC/" "$DST/"

cd "$DST"
git add -A
git commit -m "Minor release $VERSION (from dev @ $HASH)"
