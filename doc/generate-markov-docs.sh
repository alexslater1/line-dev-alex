#!/bin/bash
# Generate complete Markov chain API documentation

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MC_API_DIR="../matlab/src/api/mc"
MC_KPCTOOLBOX_DIR="../matlab/lib/kpctoolbox/mc"
OUTPUT_FILE="${SCRIPT_DIR}/api_markov_complete.dox"

echo "Generating complete Markov chain API documentation..."
echo "Output file: $OUTPUT_FILE"

# Start with header
cat > "$OUTPUT_FILE" << 'EOF'
/**
@page api_markov Markov Chain Modules
@brief CTMC and DTMC analysis

These modules provide tools for analyzing Markov chains and related stochastic processes.

@section markov_mc Markov Chain Analysis

The mc module provides general tools for analyzing both continuous-time and discrete-time
Markov chains, including steady-state and transient analysis.

@subsection markov_ctmc_steady CTMC Steady-State Analysis

EOF

# Process CTMC steady-state functions
echo "Processing CTMC steady-state functions..."
for func in ctmc_solve ctmc_relsolve ctmc_timereverse ctmc_solve_reducible ctmc_stochcomp ctmc_takahashi ctmc_courtois ctmc_kms; do
    # Try main API first
    mfile="${MC_API_DIR}/${func}.m"
    if [ ! -f "$mfile" ]; then
        # Try kpctoolbox
        mfile="${MC_KPCTOOLBOX_DIR}/${func}.m"
    fi

    if [ -f "$mfile" ]; then
        echo "  Processing: $func"
        python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $func"
    else
        echo "  Warning: $func not found"
    fi
done

# Add transient section
cat >> "$OUTPUT_FILE" << 'EOF'

@subsection markov_ctmc_transient CTMC Transient Analysis

EOF

# Process CTMC transient functions
echo "Processing CTMC transient functions..."
for func in ctmc_transient ctmc_uniformization ctmc_randomization ctmc_simulate; do
    mfile="${MC_API_DIR}/${func}.m"
    if [ ! -f "$mfile" ]; then
        mfile="${MC_KPCTOOLBOX_DIR}/${func}.m"
    fi

    if [ -f "$mfile" ]; then
        echo "  Processing: $func"
        python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $func"
    else
        echo "  Warning: $func not found"
    fi
done

# Add construction section
cat >> "$OUTPUT_FILE" << 'EOF'

@subsection markov_ctmc_construction CTMC State Space Construction

EOF

# Process CTMC construction functions
echo "Processing CTMC construction functions..."
for func in ctmc_makeinfgen ctmc_rand ctmc_ssg ctmc_ssg_reachability ctmc_multi; do
    mfile="${MC_API_DIR}/${func}.m"
    if [ ! -f "$mfile" ]; then
        mfile="${MC_KPCTOOLBOX_DIR}/${func}.m"
    fi

    if [ -f "$mfile" ]; then
        echo "  Processing: $func"
        python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $func"
    else
        echo "  Warning: $func not found"
    fi
done

# Add DTMC section
cat >> "$OUTPUT_FILE" << 'EOF'

@subsection markov_dtmc Discrete-Time Markov Chain (DTMC) Functions

<b>Steady-State Analysis:</b>

EOF

# Process DTMC functions
echo "Processing DTMC functions..."
for func in dtmc_solve dtmc_timereverse dtmc_stochcomp dtmc_solve_reducible dtmc_simulate dtmc_rand dtmc_makestochastic dtmc_isfeasible dtmc_uniformization; do
    mfile="${MC_API_DIR}/${func}.m"
    if [ ! -f "$mfile" ]; then
        mfile="${MC_KPCTOOLBOX_DIR}/${func}.m"
    fi

    if [ -f "$mfile" ]; then
        echo "  Processing: $func"
        python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $func"
    else
        echo "  Warning: $func not found"
    fi
done

# Add utilities section
cat >> "$OUTPUT_FILE" << 'EOF'

@subsection markov_utilities Utilities

EOF

# Process utility functions
echo "Processing utility functions..."
for func in weaklyconncomp options; do
    mfile="${MC_API_DIR}/${func}.m"
    if [ ! -f "$mfile" ]; then
        mfile="${MC_KPCTOOLBOX_DIR}/${func}.m"
    fi

    if [ -f "$mfile" ]; then
        echo "  Processing: $func"
        python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $func"
    else
        echo "  Warning: $func not found"
    fi
done

# Close the page
cat >> "$OUTPUT_FILE" << 'EOF'

*/
EOF

echo ""
echo "Markov chain documentation generation complete!"
echo "Output saved to: $OUTPUT_FILE"
echo ""
echo "To use this documentation:"
echo "  1. Review descriptions in $OUTPUT_FILE"
echo "  2. Replace config.doxygen/api_markov.dox"
echo "  3. Run ./generate-doxygen.sh to build HTML"
