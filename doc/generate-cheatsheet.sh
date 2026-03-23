#!/bin/bash

# LINE Cheatsheet Compilation Script
# Compiles the LINE cheatsheet and cleans up intermediate files

echo "=== LINE Cheatsheet Compilation Script ==="

# Store the original directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CHEATSHEET_DIR="$SCRIPT_DIR/cheatsheet"

# Check if cheatsheet directory exists
if [ ! -d "$CHEATSHEET_DIR" ]; then
    echo "ERROR: Cheatsheet directory not found: $CHEATSHEET_DIR"
    exit 1
fi

# Check if source file exists
if [ ! -f "$CHEATSHEET_DIR/LINE-cheatsheet.tex" ]; then
    echo "ERROR: LINE-cheatsheet.tex not found in $CHEATSHEET_DIR"
    exit 1
fi

# Change to cheatsheet directory
cd "$CHEATSHEET_DIR" || exit 1

echo "Compiling LINE-cheatsheet.pdf..."

# First compilation
pdflatex -interaction=nonstopmode LINE-cheatsheet.tex > /dev/null 2>&1
if [ ! -f "LINE-cheatsheet.pdf" ]; then
    echo "ERROR: First pdflatex compilation failed for LINE-cheatsheet.tex"
    exit 1
fi

# Second compilation for cross-references
pdflatex -interaction=nonstopmode LINE-cheatsheet.tex > /dev/null 2>&1
if [ ! -f "LINE-cheatsheet.pdf" ]; then
    echo "ERROR: Second pdflatex compilation failed for LINE-cheatsheet.tex"
    exit 1
fi

echo "✓ Successfully compiled LINE-cheatsheet.pdf"

echo ""
echo "=== Cleaning up intermediate files ==="

# Clean up intermediate LaTeX files
rm -f *.aux *.toc *.out *.idx *.ilg *.ind *.log *.fls *.fdb_latexmk *.synctex.gz *.bbl *.blg

echo "✓ Cleaned auxiliary files"

# Change back to original directory
cd "$SCRIPT_DIR"

echo ""
echo "=== Compilation Summary ==="
if [ -f "$CHEATSHEET_DIR/LINE-cheatsheet.pdf" ]; then
    echo "✓ LINE-cheatsheet.pdf - $(ls -lh "$CHEATSHEET_DIR/LINE-cheatsheet.pdf" | awk '{print $5}') ($(pdfinfo "$CHEATSHEET_DIR/LINE-cheatsheet.pdf" 2>/dev/null | grep "Pages:" | awk '{print $2}' || echo "?") pages)"
else
    echo "ERROR: LINE-cheatsheet.pdf was not created"
    exit 1
fi

echo ""
echo "=== Cheatsheet Compilation Complete ==="
