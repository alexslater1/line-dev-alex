#!/bin/bash

# LINE Manual Compilation Script
# Compiles all three LINE manuals with indexes and cleans up intermediate files

echo "=== LINE Manual Compilation Script ==="

# Store the original directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Remove existing PDFs from doc/ directory
echo "=== Removing existing PDFs from doc/ ==="
rm -f "$SCRIPT_DIR/LINE-java.pdf"
rm -f "$SCRIPT_DIR/LINE-kotlin.pdf"
rm -f "$SCRIPT_DIR/LINE-matlab.pdf"
rm -f "$SCRIPT_DIR/LINE-python.pdf"
# Re-enable when 3.0.4 is out:
# rm -f "$SCRIPT_DIR/LINE-mcp.pdf"
echo "✓ Removed existing PDFs from doc/"

# Change to latex directory
cd "$SCRIPT_DIR/latex" || exit 1

echo "Compiling LINE-java.pdf, LINE-kotlin.pdf, LINE-matlab.pdf, LINE-python.pdf..."

# Function to compile a single manual with index and bibliography
compile_manual() {
    local tex_file=$1
    local base_name=$(basename "$tex_file" .tex)
    
    echo "Compiling $tex_file..."
    
    # First compilation
    pdflatex -interaction=nonstopmode "$tex_file" > /dev/null 2>&1
    if [ ! -f "${base_name}.pdf" ]; then
        echo "ERROR: First pdflatex compilation failed for $tex_file"
        return 1
    fi
    
    # Generate bibliography if aux file exists
    if [ -f "${base_name}.aux" ]; then
        echo "Generating bibliography for $base_name..."
        bibtex "${base_name}" > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            echo "WARNING: Bibliography generation failed for $base_name"
        fi
    fi
    
    # Generate index if idx file exists
    if [ -f "${base_name}.idx" ]; then
        echo "Generating index for $base_name..."
        makeindex "${base_name}.idx" > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            echo "WARNING: Index generation failed for $base_name"
        fi
    fi
    
    # Second compilation to integrate bibliography
    pdflatex -interaction=nonstopmode "$tex_file" > /dev/null 2>&1
    if [ ! -f "${base_name}.pdf" ]; then
        echo "ERROR: Second pdflatex compilation failed for $tex_file"
        return 1
    fi
    
    # Final compilation with complete references and index
    pdflatex -interaction=nonstopmode "$tex_file" > /dev/null 2>&1
    if [ ! -f "${base_name}.pdf" ]; then
        echo "ERROR: Final pdflatex compilation failed for $tex_file"
        return 1
    fi
    
    echo "✓ Successfully compiled $base_name.pdf"
    return 0
}

# Compile all manuals (skip LINE.tex - combined version exceeds TeX capacity)
# compile_manual "LINE.tex"
compile_manual "LINE-java.tex"
compile_manual "LINE-kotlin.tex"
compile_manual "LINE-matlab.tex"
compile_manual "LINE-python.tex"
# Re-enable when 3.0.4 is out:
# compile_manual "LINE-mcp.tex"

echo ""
echo "=== Cleaning up intermediate files ==="

# Clean up intermediate LaTeX files
rm -f *.aux *.toc *.out *.idx *.ilg *.ind *.log *.fls *.fdb_latexmk *.synctex.gz *.bbl *.blg

echo "✓ Cleaned auxiliary files: .aux, .toc, .out, .idx, .ilg, .ind, .log, .fls, .fdb_latexmk, .synctex.gz, .bbl, .blg"

echo ""
echo "=== Moving PDFs to doc/ directory ==="

# Move PDFs to doc/ directory
mv -f LINE-java.pdf "$SCRIPT_DIR/" 2>/dev/null && echo "✓ Moved LINE-java.pdf to doc/"
mv -f LINE-kotlin.pdf "$SCRIPT_DIR/" 2>/dev/null && echo "✓ Moved LINE-kotlin.pdf to doc/"
mv -f LINE-matlab.pdf "$SCRIPT_DIR/" 2>/dev/null && echo "✓ Moved LINE-matlab.pdf to doc/"
mv -f LINE-python.pdf "$SCRIPT_DIR/" 2>/dev/null && echo "✓ Moved LINE-python.pdf to doc/"
# Re-enable when 3.0.4 is out:
# mv -f LINE-mcp.pdf "$SCRIPT_DIR/" 2>/dev/null && echo "✓ Moved LINE-mcp.pdf to doc/"

# Change back to original directory
cd "$SCRIPT_DIR"

echo ""
echo "=== Compilation Summary ==="
if [ -f "LINE-java.pdf" ]; then
    echo "✓ LINE-java.pdf - $(ls -lh LINE-java.pdf | awk '{print $5}') ($(pdfinfo LINE-java.pdf 2>/dev/null | grep "Pages:" | awk '{print $2}' || echo "? pages"))"
fi

if [ -f "LINE-kotlin.pdf" ]; then
    echo "✓ LINE-kotlin.pdf - $(ls -lh LINE-kotlin.pdf | awk '{print $5}') ($(pdfinfo LINE-kotlin.pdf 2>/dev/null | grep "Pages:" | awk '{print $2}' || echo "? pages"))"
fi

if [ -f "LINE-matlab.pdf" ]; then
    echo "✓ LINE-matlab.pdf - $(ls -lh LINE-matlab.pdf | awk '{print $5}') ($(pdfinfo LINE-matlab.pdf 2>/dev/null | grep "Pages:" | awk '{print $2}' || echo "? pages"))"
fi

if [ -f "LINE-python.pdf" ]; then
    echo "✓ LINE-python.pdf - $(ls -lh LINE-python.pdf | awk '{print $5}') ($(pdfinfo LINE-python.pdf 2>/dev/null | grep "Pages:" | awk '{print $2}' || echo "? pages"))"
fi

# Re-enable when 3.0.4 is out:
# if [ -f "LINE-mcp.pdf" ]; then
#     echo "✓ LINE-mcp.pdf - $(ls -lh LINE-mcp.pdf | awk '{print $5}') ($(pdfinfo LINE-mcp.pdf 2>/dev/null | grep "Pages:" | awk '{print $2}' || echo "? pages"))"
# fi

echo ""
echo "=== Compilation Complete ==="
