#!/bin/bash
# Extract API function documentation from MATLAB files
# This script helps generate documentation entries for api_algorithms.dox

if [ $# -lt 1 ]; then
    echo "Usage: $0 <matlab_file.m>"
    exit 1
fi

MFILE="$1"
FUNCNAME=$(basename "$MFILE" .m)

echo "Processing: $FUNCNAME"
echo ""

# Extract function signature
echo "Function signature:"
grep -m 1 "^function" "$MFILE" 2>/dev/null || echo "Not found"
echo ""

# Extract brief description from comments
echo "Brief description (from comments):"
grep -E "^%.*$FUNCNAME" "$MFILE" | head -5
echo ""

# Try to extract @brief if it exists
echo "@brief (if exists):"
grep -A 1 "@brief" "$MFILE" | grep -v "^--" | head -3
echo ""

# Try to extract @param entries
echo "Parameters (if documented):"
grep "@param" "$MFILE" | head -10
echo ""

# Try to extract @return entries
echo "Returns (if documented):"
grep "@return" "$MFILE" | head -10
echo ""

echo "---"
echo "Suggested .dox entry format:"
echo ""
echo "- @ref ${FUNCNAME}.m \"${FUNCNAME}\" - [Brief description here]"
echo ""
echo "  <b>Syntax:</b>"
echo "  @code"
echo "  [outputs] = ${FUNCNAME}(inputs)"
echo "  @endcode"
echo ""
echo "  <b>Parameters:</b>"
echo "  <table>"
echo "  <tr><th>Name</th><th>Description</th></tr>"
echo "  <tr><td>param1</td><td>Description</td></tr>"
echo "  </table>"
echo ""
echo "  <b>Returns:</b>"
echo "  <table>"
echo "  <tr><th>Name</th><th>Description</th></tr>"
echo "  <tr><td>output1</td><td>Description</td></tr>"
echo "  </table>"
echo ""
echo "  <b>Description:</b> [Detailed description here]"
