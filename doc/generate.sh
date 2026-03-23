#!/bin/bash

# Master script to generate all documentation for LINE solver
# This script calls all individual generate-*.sh scripts

echo "=========================================="
echo "LINE Documentation - Complete Build"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Track successful and failed builds
SUCCESSFUL=()
FAILED=()
SKIPPED=()

# Function to run a generation script
run_generator() {
    local script=$1
    local description=$2
    
    if [ ! -f "$script" ]; then
        echo -e "${YELLOW}⚠ Skipping $description - script not found${NC}"
        SKIPPED+=("$description")
        return
    fi
    
    echo ""
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${BLUE}Starting: $description${NC}"
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo ""
    
    if bash "$script"; then
        echo -e "${GREEN}✓ $description completed successfully${NC}"
        SUCCESSFUL+=("$description")
    else
        echo -e "${RED}✗ $description failed${NC}"
        FAILED+=("$description")
    fi
}

# Check if we're in the doc/ directory
if [ ! -d "config.doxygen" ] && [ ! -d "config.sphinx" ]; then
    echo -e "${RED}Error: This script must be run from the doc/ directory${NC}"
    echo "Usage: cd doc && ./generate.sh"
    exit 1
fi

# Start time
START_TIME=$(date +%s)

# 1. Generate Doxygen documentation (MATLAB)
run_generator "./generate-doxygen.sh" "Doxygen Documentation (MATLAB)"

# 2. Generate Javadoc documentation (Java/Kotlin)
run_generator "./generate-javadoc.sh" "Javadoc Documentation (Java/Kotlin)"

# 3. Generate Sphinx documentation (Python)
run_generator "./generate-sphinx.sh" "Sphinx Documentation (Python)"

# 4. Generate PDF manuals (LaTeX)
run_generator "./generate-manuals.sh" "PDF Manuals (LaTeX)"

# 5. Generate cheatsheet (LaTeX)
run_generator "./generate-cheatsheet.sh" "Cheatsheet (LaTeX)"

# End time
END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))

# Summary
echo ""
echo ""
echo "=========================================="
echo "Documentation Build Summary"
echo "=========================================="
echo ""

if [ ${#SUCCESSFUL[@]} -gt 0 ]; then
    echo -e "${GREEN}Successfully built:${NC}"
    for item in "${SUCCESSFUL[@]}"; do
        echo -e "  ${GREEN}✓${NC} $item"
    done
fi

if [ ${#FAILED[@]} -gt 0 ]; then
    echo ""
    echo -e "${RED}Failed to build:${NC}"
    for item in "${FAILED[@]}"; do
        echo -e "  ${RED}✗${NC} $item"
    done
fi

if [ ${#SKIPPED[@]} -gt 0 ]; then
    echo ""
    echo -e "${YELLOW}Skipped (script not found):${NC}"
    for item in "${SKIPPED[@]}"; do
        echo -e "  ${YELLOW}⚠${NC} $item"
    done
fi

echo ""
echo "Build time: ${DURATION} seconds"
echo ""

# Check if all builds were successful
if [ ${#FAILED[@]} -eq 0 ] && [ ${#SUCCESSFUL[@]} -gt 0 ]; then
    echo -e "${GREEN}=========================================="
    echo -e "All documentation built successfully!"
    echo -e "==========================================${NC}"
    echo ""
    echo "Generated documentation locations:"
    
    if [[ " ${SUCCESSFUL[@]} " =~ "Doxygen" ]]; then
        echo "  • Doxygen: doc/doxygen/"
    fi
    if [[ " ${SUCCESSFUL[@]} " =~ "Javadoc" ]]; then
        echo "  • Javadoc: doc/javadoc/"
    fi
    if [[ " ${SUCCESSFUL[@]} " =~ "Sphinx" ]]; then
        echo "  • Sphinx:  doc/sphinx/"
    fi
    if [[ " ${SUCCESSFUL[@]} " =~ "PDF" ]]; then
        echo "  • PDFs:    doc/LINE-*.pdf"
    fi
    if [[ " ${SUCCESSFUL[@]} " =~ "Cheatsheet" ]]; then
        echo "  • Cheatsheet: doc/cheatsheet/LINE-cheatsheet.pdf"
    fi
    
    echo ""
    echo "To upload all documentation to SourceForge:"
    echo "  ./upload.sh"
    echo ""
    exit 0
else
    if [ ${#FAILED[@]} -gt 0 ]; then
        echo -e "${RED}=========================================="
        echo -e "Some documentation builds failed"
        echo -e "==========================================${NC}"
        echo ""
        echo "Please check the error messages above and try:"
        echo "  1. Installing missing dependencies"
        echo "  2. Running failed scripts individually for detailed errors"
        echo "  3. Checking that source directories exist"
        echo ""
        exit 1
    else
        echo -e "${YELLOW}=========================================="
        echo -e "No documentation was built"
        echo -e "==========================================${NC}"
        echo ""
        echo "No generate-*.sh scripts were found or all were skipped."
        echo ""
        exit 1
    fi
fi
