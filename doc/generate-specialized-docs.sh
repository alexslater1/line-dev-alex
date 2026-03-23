#!/bin/bash
# Generate complete specialized modules API documentation

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
API_BASE="../matlab/src/api"
OUTPUT_FILE="${SCRIPT_DIR}/api_specialized_complete.dox"

echo "Generating complete specialized modules API documentation..."
echo "Output file: $OUTPUT_FILE"

# Start with header
cat > "$OUTPUT_FILE" << 'EOF'
/**
@page api_specialized Specialized Modules
@brief Polling, loss networks, LQNs, non-product-form networks

These modules provide domain-specific functionality for specialized queueing applications.

@section spec_polling Polling Systems (matlab/src/api/polling/)

The polling module implements algorithms for polling systems with various
polling strategies (gated, exhaustive, k-limited).

@subsection spec_polling_functions Key Functions

EOF

# Process polling functions
echo "Processing polling functions..."
if [ -d "${API_BASE}/polling" ]; then
    for mfile in ${API_BASE}/polling/*.m; do
        if [ -f "$mfile" ]; then
            basename=$(basename "$mfile" .m)
            echo "  Processing: $basename"
            python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $basename"
        fi
    done
else
    echo "  Warning: polling directory not found"
fi

# Add NPFQN section
cat >> "$OUTPUT_FILE" << 'EOF'

@section spec_npfqn Non-Product-Form Networks (matlab/src/api/npfqn/)

The npfqn module provides approximation methods for non-product-form queueing
networks that do not satisfy BCMP conditions.

@subsection spec_npfqn_functions Key Functions

EOF

# Process NPFQN functions
echo "Processing NPFQN functions..."
if [ -d "${API_BASE}/npfqn" ]; then
    for mfile in ${API_BASE}/npfqn/*.m; do
        if [ -f "$mfile" ]; then
            basename=$(basename "$mfile" .m)
            echo "  Processing: $basename"
            python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $basename"
        fi
    done
else
    echo "  Warning: npfqn directory not found"
fi

# Add loss networks section
cat >> "$OUTPUT_FILE" << 'EOF'

@section spec_lossn Loss Networks (matlab/src/api/lossn/)

The lossn module implements algorithms for loss networks where arrivals can
be blocked when resources are unavailable.

@subsection spec_lossn_functions Key Functions

EOF

# Process loss network functions
echo "Processing loss network functions..."
if [ -d "${API_BASE}/lossn" ]; then
    for mfile in ${API_BASE}/lossn/*.m; do
        if [ -f "$mfile" ]; then
            basename=$(basename "$mfile" .m)
            echo "  Processing: $basename"
            python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $basename"
        fi
    done
else
    echo "  Warning: lossn directory not found"
fi

# Add LSN section
cat >> "$OUTPUT_FILE" << 'EOF'

@section spec_lsn Layered Stochastic Networks (matlab/src/api/lsn/)

The lsn module provides algorithms for layered stochastic networks and
layered queueing networks (LQNs).

@subsection spec_lsn_functions Key Functions

EOF

# Process LSN functions
echo "Processing LSN functions..."
if [ -d "${API_BASE}/lsn" ]; then
    for mfile in ${API_BASE}/lsn/*.m; do
        if [ -f "$mfile" ]; then
            basename=$(basename "$mfile" .m)
            echo "  Processing: $basename"
            python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $basename"
        fi
    done
else
    echo "  Warning: lsn directory not found"
fi

# Add RL section
cat >> "$OUTPUT_FILE" << 'EOF'

@section spec_rl Reinforcement Learning (matlab/src/api/rl/)

The rl module provides reinforcement learning utilities for queueing control
and optimization problems.

@subsection spec_rl_functions Key Functions

EOF

# Process RL functions
echo "Processing RL functions..."
if [ -d "${API_BASE}/rl" ]; then
    for mfile in ${API_BASE}/rl/*.m; do
        if [ -f "$mfile" ]; then
            basename=$(basename "$mfile" .m)
            echo "  Processing: $basename"
            python3 "${SCRIPT_DIR}/generate-api-docs.py" "$mfile" "" >> "$OUTPUT_FILE" 2>/dev/null || echo "  Error processing $basename"
        fi
    done
else
    echo "  Warning: rl directory not found"
fi

# Close the page
cat >> "$OUTPUT_FILE" << 'EOF'

*/
EOF

echo ""
echo "Specialized modules documentation generation complete!"
echo "Output saved to: $OUTPUT_FILE"
echo ""
echo "To use this documentation:"
echo "  1. Review descriptions in $OUTPUT_FILE"
echo "  2. Replace config.doxygen/api_specialized.dox"
echo "  3. Run ./generate-doxygen.sh to build HTML"
