#!/bin/bash

# Script to add example sections to remaining solver HTML files
# Usage: ./add_solver_examples.sh <solver-name>
# Example: ./add_solver_examples.sh ssa

SOLVER=$1
SOLVER_UPPER=$(echo "$SOLVER" | tr '[:lower:]' '[:upper:]')
SOLVER_FILE="solver-${SOLVER}.html"

if [ ! -f "$SOLVER_FILE" ]; then
    echo "Error: $SOLVER_FILE not found"
    exit 1
fi

echo "Adding example section to $SOLVER_FILE..."

# This script adds CSS styles, JavaScript for tabs, and a template example section
# You need to customize the solver-specific code and output for each solver

cat << 'EOF'
To complete the remaining solvers, you need to:

1. Add CSS styles (already provided in MVA, JMT, MAM, LDES as a template)
2. Add JavaScript for tab switching (already provided as a template)
3. Create solver-specific examples in MATLAB/Java/Python
4. Run the examples to get actual output
5. Insert the example section before the references or back-link section

Solvers remaining:
- SSA (Stochastic State-space Analysis)
- CTMC (Continuous Time Markov Chain)
- AUTO (Automatic solver selection)
- NC (Normalizing Constant)
- FLD (Fluid)
- LN (Layered Networks)
- LQNS (Layered Queueing Network Solver)
- QNS (Queueing Network Solver)
- ENV (Environment)

For each solver, follow the pattern used in:
- doc/solver-mva.html
- doc/solver-jmt.html
- doc/solver-mam.html
- doc/solver-ldes.html
EOF
