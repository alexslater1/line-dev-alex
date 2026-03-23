#!/bin/bash

# LINE Solver SourceForge Release Script
# Usage: ./release.sh <version>
# Example: ./release.sh 3.0.50
#
# This script uploads release materials to SourceForge

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check for required tools
check_tool() {
    if command -v $1 &> /dev/null; then
        echo -e "${GREEN}✓${NC} $1 is installed"
        return 0
    else
        echo -e "${RED}✗${NC} $1 is not installed"
        return 1
    fi
}

# Function to install sshpass
install_sshpass() {
    echo -e "${YELLOW}Attempting to install sshpass...${NC}"
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if command -v apt-get &> /dev/null; then
            sudo apt-get update && sudo apt-get install -y sshpass
        elif command -v yum &> /dev/null; then
            sudo yum install -y sshpass
        else
            echo -e "${RED}Cannot auto-install sshpass. Please install manually.${NC}"
            return 1
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        if command -v brew &> /dev/null; then
            brew install hudochenkov/sshpass/sshpass
        else
            echo -e "${RED}Please install Homebrew first, then run: brew install hudochenkov/sshpass/sshpass${NC}"
            return 1
        fi
    else
        echo -e "${RED}Unsupported OS. Please install sshpass manually.${NC}"
        return 1
    fi
}

VERSION=$1

if [ -z "$VERSION" ]; then
    echo "Usage: $0 <version>"
    echo "Example: $0 3.0.50"
    exit 1
fi

echo "================================================"
echo "LINE Solver SourceForge Release - Version $VERSION"
echo "================================================"
echo ""

# Check if at least the uber release archive exists
ARCHIVE="line-${VERSION}.zip"
if [ ! -f "$ARCHIVE" ]; then
    echo "Error: Release archive not found: $ARCHIVE"
    echo "Please run ./pre-release.sh $VERSION first"
    exit 1
fi

# Collect all archives to upload (uber + per-codebase)
ARCHIVES=("$ARCHIVE")
for suffix in -jar -python -matlab; do
    local_archive="line-${VERSION}${suffix}.zip"
    if [ -f "$local_archive" ]; then
        ARCHIVES+=("$local_archive")
    else
        echo "Warning: Per-codebase archive not found: $local_archive (skipping)"
    fi
done

# Check for sshpass
if ! check_tool sshpass; then
    echo
    read -p "Would you like to install sshpass? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        if install_sshpass; then
            echo -e "${GREEN}sshpass installed successfully${NC}"
        else
            echo -e "${RED}Failed to install sshpass. Exiting.${NC}"
            exit 1
        fi
    else
        echo -e "${RED}sshpass is required for this script. Exiting.${NC}"
        exit 1
    fi
fi

# SourceForge configuration
SF_PROJECT="line-solver"
SF_USER="casaleggg"
SF_PATH="/home/frs/project/${SF_PROJECT}/${VERSION}/"

echo "SourceForge Configuration:"
echo "  Project: $SF_PROJECT"
echo "  User: $SF_USER"
echo "  Remote path: $SF_PATH"
echo "  Archives: ${ARCHIVES[*]}"
echo ""

# Prompt for confirmation
read -p "Upload to SourceForge as user '$SF_USER'? (y/N): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Upload cancelled."
    exit 0
fi

# Prompt for password once
echo
echo -n "Enter SourceForge password for $SF_USER: "
read -s PASSWORD
echo
echo

# Export for use in sshpass commands
export SSHPASS="$PASSWORD"

# Create remote directory and upload files
echo ""
echo "Step 1: Creating remote directory on SourceForge..."
sshpass -e ssh -o StrictHostKeyChecking=no ${SF_USER}@frs.sourceforge.net "mkdir -p ${SF_PATH}" || {
    echo "Warning: Directory may already exist or creation failed"
}

echo ""
echo "Step 2: Uploading release archives..."
for archive in "${ARCHIVES[@]}"; do
    echo "Uploading: $archive"
    sshpass -e rsync -avP -e "ssh -o StrictHostKeyChecking=no" "$archive" ${SF_USER}@frs.sourceforge.net:${SF_PATH}
done

# Check if documentation files exist and upload them
echo ""
echo "Step 3: Checking for additional release materials..."

# Upload PDFs if they exist
DOC_DIR="doc"
if [ -d "$DOC_DIR" ]; then
    for pdf in LINE-matlab.pdf LINE-java.pdf LINE-kotlin.pdf LINE-python.pdf; do
        if [ -f "$DOC_DIR/$pdf" ]; then
            echo "Uploading: $DOC_DIR/$pdf"
            sshpass -e rsync -avP -e "ssh -o StrictHostKeyChecking=no" "$DOC_DIR/$pdf" ${SF_USER}@frs.sourceforge.net:${SF_PATH}
        fi
    done
fi

# Upload cheatsheet if it exists
CHEATSHEET="doc/cheatsheet/LINE-cheatsheet.pdf"
if [ -f "$CHEATSHEET" ]; then
    echo "Uploading: $CHEATSHEET"
    sshpass -e rsync -avP -e "ssh -o StrictHostKeyChecking=no" "$CHEATSHEET" ${SF_USER}@frs.sourceforge.net:${SF_PATH}
fi

# Upload README.textile (changelog) if it exists
if [ -f "README.textile" ]; then
    echo "Uploading: README.textile"
    sshpass -e rsync -avP -e "ssh -o StrictHostKeyChecking=no" "README.textile" ${SF_USER}@frs.sourceforge.net:${SF_PATH}
fi

echo ""
echo "================================================"
echo "SourceForge Upload Complete!"
echo "================================================"
echo ""
echo "Release materials uploaded to:"
echo "  https://sourceforge.net/projects/${SF_PROJECT}/files/${VERSION}/"
echo ""

# Clean up
unset SSHPASS

echo ""
echo "Step 4: Syncing to GitHub (line-solver.git)..."
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
bash "$SCRIPT_DIR/github-rsynch.sh" "$VERSION"

echo ""
echo "================================================"
echo "Release Complete!"
echo "================================================"
echo ""
echo "Next steps:"
echo "  1. Verify files on SourceForge web interface"
echo "  2. Set default download (if needed)"
echo "  3. Push line-solver.git to GitHub"
echo "  4. Run ./post-release.sh $VERSION <pypi-token> to publish to PyPI"
echo ""
