#!/bin/bash

# LINE Solver Release Preparation Script
# Usage: ./pre-release.sh <version> [from-hash]
# Example: ./pre-release.sh 3.0.50
# Example: ./pre-release.sh 3.0.50 abc123

set -e

VERSION=$1
FROM_HASH=$2

# Get repository root directory (works regardless of where script is run from)
REPO_ROOT=$(git rev-parse --show-toplevel)

# Check VERSION parameter first
if [ -z "$VERSION" ]; then
    echo "Usage: $0 <version> [from-hash]"
    echo "Example: $0 3.0.50"
    echo "Example: $0 3.0.50 abc123"
    echo "Note: If from-hash is not provided, will prompt for starting commit"
    exit 1
fi

# ===========================================
# Main Script
# ===========================================

# If FROM_HASH not provided, find last release commit and prompt user
if [ -z "$FROM_HASH" ]; then
    # Find the most recent commit with "Version" and "released" in the message
    LAST_RELEASE=$(git log --oneline --grep="Version.*released" --grep="released.*Version" -i | head -1 | cut -d' ' -f1)

    if [ -n "$LAST_RELEASE" ]; then
        LAST_RELEASE_MSG=$(git log --oneline -1 $LAST_RELEASE)
        echo "Found last release commit: $LAST_RELEASE_MSG"
        echo ""
        read -p "Enter the starting commit/tag for changelog (default: $LAST_RELEASE): " USER_HASH
        FROM_HASH=${USER_HASH:-$LAST_RELEASE}
    else
        echo "No previous release commit found with 'Version' and 'released' keywords."
        echo "Recent commits:"
        git log --oneline -10
        echo ""
        read -p "Enter the starting commit/tag for changelog: " FROM_HASH
        if [ -z "$FROM_HASH" ]; then
            echo "Error: FROM_HASH is required"
            exit 1
        fi
    fi
    echo "Using FROM_HASH: $FROM_HASH"
fi

echo "Preparing LINE release $VERSION..."

# Update version in all interfaces (before JAR build)
echo "Updating version numbers..."

# Update line-cli.py CLI version
sed -i "s/__version__ = \".*\"/__version__ = \"$VERSION\"/" "$REPO_ROOT/line-cli.py"
echo "  Updated line-cli.py"

# Update GlobalConstants.java (JAR/CLI/REST version)
sed -i "s/Version = \".*\"/Version = \"$VERSION\"/" "$REPO_ROOT/jar/src/main/kotlin/jline/GlobalConstants.java"
echo "  Updated GlobalConstants.java"

# Update Python package version
sed -i "s/^version = \".*\"/version = \"${VERSION}.0\"/" "$REPO_ROOT/python/pyproject.toml"
echo "  Updated python/pyproject.toml to ${VERSION}.0"

# Update website version
echo "Updating website version references..."
sed -i "s/LINE [0-9]\+\.[0-9]\+\.[0-9]\+<\/a> is out/LINE ${VERSION}<\/a> is out/g" "$REPO_ROOT"/doc/web/*.html
echo "  Updated doc/web/*.html"

# Build JAR package
echo "Building JAR package..."
cd "$REPO_ROOT/jar/"
mvn clean package -Pb
cd "$REPO_ROOT"

# Generate Javadoc
echo "Generating Javadoc..."
cd "$REPO_ROOT/jar/"
#./generate-javadoc.sh
cd "$REPO_ROOT"

# Deploy to Maven
echo "Deploying to Maven..."
cd "$REPO_ROOT/jar/"
mvn deploy -DskipTests
cd "$REPO_ROOT"

# Build LDES JAR + native binary (requires jline in Maven cache)
LDES_DEV_DIR="$REPO_ROOT/../ldes-dev.git"
if [ -d "$LDES_DEV_DIR" ]; then
    echo "Building LDES JAR..."
    (cd "$LDES_DEV_DIR/jar" && /usr/bin/mvn clean verify -P b -q)
    echo "  LDES JAR built"

    # Build native binary if GRAALVM_HOME is set
    if [ -z "$GRAALVM_HOME" ] && [ -d "$HOME/.sdkman/candidates/java/21.0.2-graalce" ]; then
        export GRAALVM_HOME="$HOME/.sdkman/candidates/java/21.0.2-graalce"
    fi
    if [ -n "$GRAALVM_HOME" ]; then
        echo "Building LDES native binary..."
        (cd "$LDES_DEV_DIR/jar" && GRAALVM_HOME="$GRAALVM_HOME" /usr/bin/mvn clean verify -P native -q)
        echo "  LDES native binary built"
    else
        echo "  Skipping native binary (GRAALVM_HOME not set)"
    fi
else
    echo "Warning: ldes-dev.git not found at $LDES_DEV_DIR, skipping LDES build"
fi

# ===========================================
# Archive Creation
# ===========================================

REPO_PARENT=$(dirname "$REPO_ROOT")
REPO_NAME=$(basename "$REPO_ROOT")

# Stage a clean copy of the full repository into a temp directory.
# This staged tree is reused by all archive variants to avoid redundant copies.
stage_full_tree() {
    cd "$REPO_PARENT"
    rm -rf temp
    mkdir -p temp/line-solver

    # Copy all distributable directories (excluding hidden files)
    for dir in common doc jar python matlab; do
        echo "  Staging $dir/..."
        rsync -a --exclude=".*" "$REPO_NAME/$dir/" "temp/line-solver/$dir/"
    done

    # Copy important root files
    for pattern in LICENSE* README* NOTICE*AUTHORS* CONTRIBUTING* pom.xml; do
        for file in "$REPO_NAME"/${pattern}; do
            [ -f "$file" ] && cp "$file" temp/line-solver/
        done
    done

    # Clean up all workspace/ folders across codebases
    for ws in $(find temp/line-solver -type d -name "workspace"); do
        rm -rf "$ws"
        mkdir -p "$ws"
    done

    # Remove unnecessary files and ALL hidden directories/files
    cd temp
    find . -type f -name "JMT.jar" -exec rm -r {} +
    find . -type f -name "line-viewer.jar" -exec rm -r {} +
    find . -type f -name "*.lock" -exec rm -r {} +
    find . -type f -name "*.git" -exec rm -r {} +
    find . -type d -name "__pycache__" -exec rm -r {} +
    find . -type d -name "target" -exec rm -r {} +
    find . -type f -name "CLAUDE.md" -exec rm -r {} +
    find . -type f -name "*.class" -exec rm -r {} +
    # Remove build artifacts and test directories
    rm -rf line-solver/python/dist
    rm -rf line-solver/jar/src/test
    # Remove specific folders and files from the release
    rm -rf line-solver/jar/jar
    rm -rf line-solver/doc/latex
    rm -rf line-solver/release
    rm -rf line-solver/matlab/dev
    # Keep only .sh and .pdf files in doc/ directory
    find line-solver/doc -type f ! -name "*.sh" ! -name "*.pdf" -exec rm {} +
    # Ensure cheatsheet is included under docs
    mkdir -p line-solver/doc/cheatsheet
    cp "$REPO_ROOT/doc/cheatsheet/LINE-cheatsheet.pdf" line-solver/doc/cheatsheet/ 2>/dev/null || true
    # Remove ALL hidden files and directories (use -mindepth 1 to avoid matching "." itself)
    find . -mindepth 1 -name ".*" -exec rm -rf {} + 2>/dev/null || true
    # Ensure shell scripts have execute permission
    find line-solver -name "*.sh" -exec chmod u+x {} +

    cd "$REPO_PARENT"
}

# Create a zip archive from the staged tree.
#   $1 = suffix (empty string for uber, "-jar", "-python", "-matlab")
#   $2... = directories to INCLUDE (from the staged line-solver/)
#   Special handling:
#     - "doc" includes the full doc/ tree (uber only)
#     - Per-codebase archives get only their relevant PDF manual(s) + cheatsheet
create_archive() {
    local SUFFIX="$1"
    shift
    local DIRS=("$@")
    local ARCHIVE_NAME="line-${VERSION}${SUFFIX}"
    local DEST="/tmp/${ARCHIVE_NAME}"

    echo ""
    echo "Creating archive: ${ARCHIVE_NAME}.zip ..."

    rm -rf "$DEST"
    mkdir -p "$DEST/line-solver"

    # Copy requested directories from the staged tree
    for dir in "${DIRS[@]}"; do
        if [ -d "temp/line-solver/$dir" ]; then
            cp -a "temp/line-solver/$dir" "$DEST/line-solver/$dir"
        fi
    done

    # Always copy root files (LICENSE, README, etc.)
    for file in temp/line-solver/*; do
        [ -f "$file" ] && cp "$file" "$DEST/line-solver/"
    done

    # For per-codebase archives (non-uber): add only relevant docs
    if [ -n "$SUFFIX" ]; then
        # Remove pom.xml from non-jar archives (only useful for JAR builds)
        if [ "$SUFFIX" != "-jar" ]; then
            rm -f "$DEST/line-solver/pom.xml"
        fi

        # Add relevant PDF manuals + cheatsheet
        mkdir -p "$DEST/line-solver/doc/cheatsheet"
        cp "temp/line-solver/doc/cheatsheet/LINE-cheatsheet.pdf" "$DEST/line-solver/doc/cheatsheet/" 2>/dev/null || true

        case "$SUFFIX" in
            -jar)
                for pdf in LINE-java.pdf LINE-kotlin.pdf; do
                    cp "temp/line-solver/doc/$pdf" "$DEST/line-solver/doc/" 2>/dev/null || true
                done
                ;;
            -python)
                cp "temp/line-solver/doc/LINE-python.pdf" "$DEST/line-solver/doc/" 2>/dev/null || true
                ;;
            -matlab)
                cp "temp/line-solver/doc/LINE-matlab.pdf" "$DEST/line-solver/doc/" 2>/dev/null || true
                ;;
        esac
    fi

    # Create the zip
    local TMP_ZIP="/tmp/${ARCHIVE_NAME}.zip"
    rm -f "$TMP_ZIP"
    (cd "$DEST" && zip -r "$TMP_ZIP" line-solver)

    # Move to release directory
    rm -f "$REPO_ROOT/release/${ARCHIVE_NAME}.zip"
    mv "$TMP_ZIP" "$REPO_ROOT/release/${ARCHIVE_NAME}.zip"

    # Clean up temp archive directory
    rm -rf "$DEST"

    local SIZE=$(du -h "$REPO_ROOT/release/${ARCHIVE_NAME}.zip" | cut -f1)
    echo "  Created: release/${ARCHIVE_NAME}.zip ($SIZE)"
}

echo "Creating release archives..."

# Stage the full cleaned tree once
stage_full_tree

# Create all four archives from the staged tree
create_archive ""         common doc jar python matlab   # uber (full distribution)
create_archive "-jar"     common jar                     # Java/Kotlin users
create_archive "-python"  python                         # Native Python users
create_archive "-matlab"  common matlab                  # MATLAB users

# Clean up staged tree
cd "$REPO_PARENT"
rm -rf temp
sync

# Generate changelog (go back to the git repository directory first)
echo "Generating changelog..."
cd "$REPO_ROOT"
git log --oneline ${FROM_HASH}..HEAD | sed 's/^[^ ]*/-/' | grep -E "fix:|feat:|refactor:|perf:|docs:|test:" | sort | uniq > "./release/CHANGELOG.textile"

# Generate README highlights from changelog using Claude
echo "Generating release highlights..."
claude -p "Read the file release/CHANGELOG.textile. Summarize it into exactly 10 key highlights for LINE ${VERSION}. Each highlight should be a single concise bullet point. Output only the 10 bullet points, no headers or extra text." > "./release/README.textile"

# Navigate to release directory using absolute path to refresh shell's directory view
cd "$REPO_ROOT/release"
cd ..
cd "$REPO_ROOT/release"
chmod u+x *.sh

echo ""
echo "============================================"
echo "Release preparation complete!"
echo "============================================"
echo "Archives created:"
echo "  release/line-${VERSION}.zip         (full distribution)"
echo "  release/line-${VERSION}-jar.zip     (Java/Kotlin only)"
echo "  release/line-${VERSION}-python.zip  (Python only)"
echo "  release/line-${VERSION}-matlab.zip  (MATLAB only)"
echo ""
echo "Changelog saved to CHANGELOG.textile"
echo "Release highlights saved to README.textile"
echo ""
echo "Next steps:"
echo "  1. Run release.sh to upload to SourceForge"
echo "  2. Run post-release.sh to publish to PyPI"
echo ""
echo "Note: If files don't appear, refresh your shell with: cd .. && cd release"
echo ""
