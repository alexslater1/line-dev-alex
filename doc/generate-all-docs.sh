#!/bin/bash
# Generate complete API documentation for all functions
# This script processes all MATLAB API files and generates .dox entries

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MATLAB_API_DIR="../matlab/src/api"
OUTPUT_FILE="${SCRIPT_DIR}/api_functions_complete.dox"

echo "Generating complete API documentation..."
echo "Output file: $OUTPUT_FILE"

# Start with header
cat > "$OUTPUT_FILE" << 'EOF'
/**
@page api_algorithms Algorithm Modules
@brief PFQN, cache, MAM, queueing systems

These modules provide specialized algorithms for queueing network analysis.

@section alg_pfqn Product-Form Queueing Networks (matlab/src/api/pfqn/)

The pfqn module contains algorithms for product-form queueing networks, including
Mean Value Analysis (MVA), convolution, and normalizing constant methods.

@subsection alg_pfqn_functions Key Functions

<b>Primary Solvers:</b>
EOF

# Process PFQN functions
echo "Processing PFQN functions..."
for mfile in ${MATLAB_API_DIR}/pfqn/pfqn_*.m; do
    if [ -f "$mfile" ]; then
        basename=$(basename "$mfile" .m)
        echo "  Processing: $basename"
        python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $basename"
    fi
done

# Add cache section
cat >> "$OUTPUT_FILE" << 'EOF'

@section alg_cache Cache Algorithms (matlab/src/api/cache/)

The cache module provides algorithms for analyzing cache systems with various
replacement policies (LRU, FIFO, RR, etc.) and item popularity distributions.

@subsection alg_cache_functions Key Functions

<b>Primary Methods:</b>
EOF

# Process Cache functions
echo "Processing Cache functions..."
for mfile in ${MATLAB_API_DIR}/cache/cache_*.m; do
    if [ -f "$mfile" ]; then
        basename=$(basename "$mfile" .m)
        echo "  Processing: $basename"
        python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $basename"
    fi
done

# Add MAM section
cat >> "$OUTPUT_FILE" << 'EOF'

@section alg_mam Matrix-Analytic Methods (matlab/src/api/mam/)

The mam module implements matrix-analytic methods for analyzing queues with
structured Markov chains, including QBD, MAP, RAP, and G/M/1 and M/G/1 type processes.

@subsection alg_mam_functions Key Functions

<b>QBD Methods:</b>
EOF

# Process MAM functions
echo "Processing MAM functions..."
for mfile in ${MATLAB_API_DIR}/mam/*.m; do
    if [ -f "$mfile" ]; then
        basename=$(basename "$mfile" .m)
        echo "  Processing: $basename"
        python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $basename"
    fi
done

# Add SN section
cat >> "$OUTPUT_FILE" << 'EOF'

@section alg_sn Stochastic Network Utilities (matlab/src/api/sn/)

Network analysis utilities for checking properties and computing derived metrics.

@subsection alg_sn_functions Key Functions
EOF

# Process SN functions
echo "Processing SN functions..."
for mfile in ${MATLAB_API_DIR}/sn/sn_*.m; do
    if [ -f "$mfile" ]; then
        basename=$(basename "$mfile" .m)
        echo "  Processing: $basename"
        python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $basename"
    fi
done

# Close the page
cat >> "$OUTPUT_FILE" << 'EOF'

*/
EOF

echo ""
echo "Documentation generation complete!"
echo "Output saved to: $OUTPUT_FILE"
echo ""
echo "To use this documentation:"
echo "  1. Review and edit descriptions in $OUTPUT_FILE"
echo "  2. Replace or merge with config.doxygen/api_algorithms.dox"
echo "  3. Run ./generate-doxygen.sh to build HTML"
