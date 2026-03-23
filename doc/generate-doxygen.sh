#!/bin/bash

# Generate documentation for LINE MATLAB implementation using Doxygen
# This script generates documentation specifically for the MATLAB codebase
# Must be run from the doc/ directory

echo "=========================================="
echo "LINE MATLAB - Doxygen Documentation Build"
echo "=========================================="

# Check if we're in the doc/ directory
if [[ ! -d "config.doxygen" ]]; then
    echo "Error: config.doxygen directory not found in current directory"
    echo "Usage: cd doc && ./generate-doxygen.sh"
    exit 1
fi

# Check if Doxygen is installed
if ! command -v doxygen &> /dev/null; then
    echo "Error: Doxygen is not installed. Please install it first:"
    echo "  Ubuntu/Debian: sudo apt-get install doxygen graphviz"
    echo "  macOS: brew install doxygen graphviz"
    echo "  Other: Visit https://www.doxygen.nl/download.html"
    exit 1
fi

# Check if graphviz is installed (for diagrams)
if ! command -v dot &> /dev/null; then
    echo "Warning: Graphviz is not installed. Diagrams will not be generated."
    echo "  Ubuntu/Debian: sudo apt-get install graphviz"
    echo "  macOS: brew install graphviz"
fi

# Clean previous documentation
echo ""
echo "Cleaning previous documentation..."
rm -rf doxygen/
rm -rf config.doxygen/html/

# Generate documentation using configuration from config.doxygen
echo "Running Doxygen for MATLAB implementation (this may take a few minutes)..."
echo ""

# Run doxygen with the Doxyfile from config.doxygen
cd config.doxygen
doxygen Doxyfile 2>&1 | while IFS= read -r line; do
    # Filter out common warnings to reduce noise
    if [[ ! "$line" =~ "Caller graph for" ]] && \
       [[ ! "$line" =~ "too many nodes" ]] && \
       [[ ! "$line" =~ "Consider increasing DOT_GRAPH_MAX_NODES" ]]; then
        echo "$line"
    fi
done

# No need to move files - Doxyfile is configured to output directly to ../doxygen

# Return to doc directory
cd ..

# Hide source code in API function pages (BuTools-style documentation)
echo ""
echo "Applying BuTools-style formatting (hiding source code)..."
./hide-source-code.sh

# Check if documentation was generated successfully
if [ -d "doxygen" ] && [ -f "doxygen/index.html" ]; then
    # Count generated files
    FILE_COUNT=$(find doxygen -name "*.html" -type f | wc -l)
    
    echo ""
    echo "=========================================="
    echo "MATLAB Documentation generated successfully!"
    echo "=========================================="
    echo ""
    echo "Statistics:"
    echo "  - HTML files generated: $FILE_COUNT"
    echo "  - Output directory: doc/doxygen/"
    echo ""
    echo "View the documentation:"
    echo "  Local: file://$(pwd)/doxygen/index.html"
    echo "  After upload: https://line-solver.sourceforge.io/doxygen/"
    echo ""
    echo "Features enabled:"
    echo "  ✓ Tree view navigation sidebar"
    echo "  ✓ Source code browsing with syntax highlighting"
    echo "  ✓ UML-style class diagrams"
    echo "  ✓ Cross-references (caller/callee graphs)"
    echo "  ✓ Module/group organization"
    echo "  ✓ Dynamic HTML sections (collapsible)"
    echo ""
    echo "Documentation includes:"
    echo "  - MATLAB implementation (matlab/src/)"
    echo "  - Solver implementations (MVA, JMT, SSA, CTMC, Fluid, MAM, NC)"
    echo "  - Core language classes (Network, Queue, Source, Sink, etc.)"
    echo "  - Stochastic processes and distributions"
    echo "  - Examples and tutorials"
    echo ""
    echo "To upload documentation to SourceForge:"
    echo "  ./upload.sh"
    echo ""
else
    echo ""
    echo "=========================================="
    echo "Error: Documentation generation failed!"
    echo "=========================================="
    echo ""
    echo "Please check:"
    echo "  1. Doxygen is properly installed"
    echo "  2. You are in the doc/ directory"
    echo "  3. The config.doxygen/Doxyfile exists"
    echo "  4. MATLAB source directory exists (matlab/src)"
    echo ""
    echo "For detailed error messages, run:"
    echo "  cd config.doxygen && doxygen Doxyfile"
    exit 1
fi