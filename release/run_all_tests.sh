#!/bin/bash

# LINE Solver Unified Test Runner
# Runs all tests across JAR, Python, and MATLAB codebases in sequence
# Reports all failures and errors at the end
#
# Usage: ./run_all_tests.sh [options]
#   -j, --jar       Run only JAR tests
#   -p, --python    Run only Python tests (native + wrapper)
#   -m, --matlab    Run only MATLAB tests
#   -h, --help      Show this help message

set -o pipefail

# Get repository root directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Test execution flags
RUN_JAR=true
RUN_PYTHON=true
RUN_MATLAB=true

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -j|--jar)
            RUN_JAR=true
            RUN_PYTHON=false
            RUN_MATLAB=false
            shift
            ;;
        -p|--python)
            RUN_JAR=false
            RUN_PYTHON=true
            RUN_MATLAB=false
            shift
            ;;
        -m|--matlab)
            RUN_JAR=false
            RUN_PYTHON=false
            RUN_MATLAB=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo ""
            echo "Run all LINE solver tests across codebases."
            echo ""
            echo "Options:"
            echo "  -j, --jar       Run only JAR tests"
            echo "  -p, --python    Run only Python tests (native + wrapper)"
            echo "  -m, --matlab    Run only MATLAB tests"
            echo "  -h, --help      Show this help message"
            echo ""
            echo "Without options, runs all tests in sequence:"
            echo "  1. JAR (Java/Kotlin)"
            echo "  2. Python (native + wrapper)"
            echo "  3. MATLAB"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use -h for help"
            exit 1
            ;;
    esac
done

# Create logs directory
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOG_DIR="$REPO_ROOT/release/logs"
mkdir -p "$LOG_DIR"

# Log files for each codebase
JAR_LOG="$LOG_DIR/jar_tests_${TIMESTAMP}.log"
PYTHON_NATIVE_LOG="$LOG_DIR/python_native_tests_${TIMESTAMP}.log"
PYTHON_WRAPPER_LOG="$LOG_DIR/python_wrapper_tests_${TIMESTAMP}.log"
MATLAB_LOG="$LOG_DIR/matlab_tests_${TIMESTAMP}.log"
SUMMARY_LOG="$LOG_DIR/test_summary_${TIMESTAMP}.log"

# Track results
declare -A TEST_STATUS
declare -A TEST_LOGS
TOTAL_FAILURES=0

echo ""
echo -e "${BOLD}=========================================="
echo "LINE Solver Unified Test Runner"
echo "==========================================${NC}"
echo "Repository: $REPO_ROOT"
echo "Timestamp:  $(date '+%Y-%m-%d %H:%M:%S')"
echo "Summary:    $SUMMARY_LOG"
echo ""

# Initialize summary log
cat > "$SUMMARY_LOG" << EOF
# LINE Solver Test Summary

- **Date:** $(date '+%Y-%m-%d %H:%M:%S')
- **Git Hash:** $(git -C "$REPO_ROOT" rev-parse --short HEAD 2>/dev/null || echo "N/A")
- **Branch:** $(git -C "$REPO_ROOT" branch --show-current 2>/dev/null || echo "N/A")

---

EOF

# ===========================================
# JAR Tests (Java/Kotlin)
# ===========================================
if [ "$RUN_JAR" = true ]; then
    echo -e "${BLUE}[1/4] Running JAR (Java/Kotlin) tests...${NC}"
    echo "Log: $JAR_LOG"
    echo ""

    echo "## JAR Tests (Java/Kotlin)" >> "$SUMMARY_LOG"
    echo "" >> "$SUMMARY_LOG"

    if [ -x "$REPO_ROOT/jar/run_tests.sh" ]; then
        cd "$REPO_ROOT/jar"
        if ./run_tests.sh 2>&1 | tee "$JAR_LOG"; then
            TEST_STATUS["jar"]="PASSED"
            echo -e "${GREEN}JAR tests: PASSED${NC}"
        else
            TEST_STATUS["jar"]="FAILED"
            TOTAL_FAILURES=$((TOTAL_FAILURES + 1))
            echo -e "${RED}JAR tests: FAILED${NC}"
        fi
        TEST_LOGS["jar"]="$JAR_LOG"
    else
        TEST_STATUS["jar"]="SKIPPED (script not found)"
        echo -e "${YELLOW}JAR tests: SKIPPED (run_tests.sh not found)${NC}"
    fi

    echo "- **Status:** ${TEST_STATUS["jar"]}" >> "$SUMMARY_LOG"
    echo "- **Log:** $JAR_LOG" >> "$SUMMARY_LOG"
    echo "" >> "$SUMMARY_LOG"
    echo ""
fi

# ===========================================
# Python Native Tests
# ===========================================
if [ "$RUN_PYTHON" = true ]; then
    echo -e "${BLUE}[2/4] Running Python native tests...${NC}"
    echo "Log: $PYTHON_NATIVE_LOG"
    echo ""

    echo "## Python Native Tests" >> "$SUMMARY_LOG"
    echo "" >> "$SUMMARY_LOG"

    if [ -x "$REPO_ROOT/python/run_tests_native.sh" ]; then
        cd "$REPO_ROOT/python"
        if ./run_tests_native.sh 2>&1 | tee "$PYTHON_NATIVE_LOG"; then
            TEST_STATUS["python_native"]="PASSED"
            echo -e "${GREEN}Python native tests: PASSED${NC}"
        else
            TEST_STATUS["python_native"]="FAILED"
            TOTAL_FAILURES=$((TOTAL_FAILURES + 1))
            echo -e "${RED}Python native tests: FAILED${NC}"
        fi
        TEST_LOGS["python_native"]="$PYTHON_NATIVE_LOG"
    else
        TEST_STATUS["python_native"]="SKIPPED (script not found)"
        echo -e "${YELLOW}Python native tests: SKIPPED (run_tests_native.sh not found)${NC}"
    fi

    echo "- **Status:** ${TEST_STATUS["python_native"]}" >> "$SUMMARY_LOG"
    echo "- **Log:** $PYTHON_NATIVE_LOG" >> "$SUMMARY_LOG"
    echo "" >> "$SUMMARY_LOG"
    echo ""
fi

# ===========================================
# Python Wrapper Tests
# ===========================================
if [ "$RUN_PYTHON" = true ]; then
    echo -e "${BLUE}[3/4] Running Python wrapper tests...${NC}"
    echo "Log: $PYTHON_WRAPPER_LOG"
    echo ""

    echo "## Python Wrapper Tests" >> "$SUMMARY_LOG"
    echo "" >> "$SUMMARY_LOG"

    if [ -x "$REPO_ROOT/python/run_tests_wrapper.sh" ]; then
        cd "$REPO_ROOT/python"
        if ./run_tests_wrapper.sh 2>&1 | tee "$PYTHON_WRAPPER_LOG"; then
            TEST_STATUS["python_wrapper"]="PASSED"
            echo -e "${GREEN}Python wrapper tests: PASSED${NC}"
        else
            TEST_STATUS["python_wrapper"]="FAILED"
            TOTAL_FAILURES=$((TOTAL_FAILURES + 1))
            echo -e "${RED}Python wrapper tests: FAILED${NC}"
        fi
        TEST_LOGS["python_wrapper"]="$PYTHON_WRAPPER_LOG"
    else
        TEST_STATUS["python_wrapper"]="SKIPPED (script not found)"
        echo -e "${YELLOW}Python wrapper tests: SKIPPED (run_tests_wrapper.sh not found)${NC}"
    fi

    echo "- **Status:** ${TEST_STATUS["python_wrapper"]}" >> "$SUMMARY_LOG"
    echo "- **Log:** $PYTHON_WRAPPER_LOG" >> "$SUMMARY_LOG"
    echo "" >> "$SUMMARY_LOG"
    echo ""
fi

# ===========================================
# MATLAB Tests
# ===========================================
if [ "$RUN_MATLAB" = true ]; then
    echo -e "${BLUE}[4/4] Running MATLAB tests...${NC}"
    echo "Log: $MATLAB_LOG"
    echo ""

    echo "## MATLAB Tests" >> "$SUMMARY_LOG"
    echo "" >> "$SUMMARY_LOG"

    if [ -x "$REPO_ROOT/matlab/run_tests.sh" ]; then
        cd "$REPO_ROOT/matlab"
        if ./run_tests.sh 2>&1 | tee "$MATLAB_LOG"; then
            TEST_STATUS["matlab"]="PASSED"
            echo -e "${GREEN}MATLAB tests: PASSED${NC}"
        else
            TEST_STATUS["matlab"]="FAILED"
            TOTAL_FAILURES=$((TOTAL_FAILURES + 1))
            echo -e "${RED}MATLAB tests: FAILED${NC}"
        fi
        TEST_LOGS["matlab"]="$MATLAB_LOG"
    else
        TEST_STATUS["matlab"]="SKIPPED (script not found)"
        echo -e "${YELLOW}MATLAB tests: SKIPPED (run_tests.sh not found)${NC}"
    fi

    echo "- **Status:** ${TEST_STATUS["matlab"]}" >> "$SUMMARY_LOG"
    echo "- **Log:** $MATLAB_LOG" >> "$SUMMARY_LOG"
    echo "" >> "$SUMMARY_LOG"
    echo ""
fi

# ===========================================
# Summary Report
# ===========================================
echo ""
echo -e "${BOLD}=========================================="
echo "TEST SUMMARY"
echo "==========================================${NC}"
echo ""

echo "---" >> "$SUMMARY_LOG"
echo "" >> "$SUMMARY_LOG"
echo "## Overall Summary" >> "$SUMMARY_LOG"
echo "" >> "$SUMMARY_LOG"
echo "| Component | Status |" >> "$SUMMARY_LOG"
echo "|-----------|--------|" >> "$SUMMARY_LOG"

printf "%-25s %s\n" "Component" "Status"
printf "%-25s %s\n" "-------------------------" "--------"

for component in "jar" "python_native" "python_wrapper" "matlab"; do
    status="${TEST_STATUS[$component]:-SKIPPED}"
    if [ "$status" = "PASSED" ]; then
        printf "%-25s ${GREEN}%s${NC}\n" "$component" "$status"
    elif [ "$status" = "FAILED" ]; then
        printf "%-25s ${RED}%s${NC}\n" "$component" "$status"
    else
        printf "%-25s ${YELLOW}%s${NC}\n" "$component" "$status"
    fi
    echo "| $component | $status |" >> "$SUMMARY_LOG"
done

echo "" >> "$SUMMARY_LOG"

echo ""
echo "==========================================="

# ===========================================
# Extract and Display Failures
# ===========================================
if [ $TOTAL_FAILURES -gt 0 ]; then
    echo ""
    echo -e "${RED}${BOLD}FAILURES DETECTED${NC}"
    echo ""

    echo "## Failure Details" >> "$SUMMARY_LOG"
    echo "" >> "$SUMMARY_LOG"

    # Extract failures from each log
    for component in "jar" "python_native" "python_wrapper" "matlab"; do
        status="${TEST_STATUS[$component]:-SKIPPED}"
        log_file="${TEST_LOGS[$component]}"

        if [ "$status" = "FAILED" ] && [ -f "$log_file" ]; then
            echo -e "${RED}=== $component failures ===${NC}"
            echo ""

            echo "### $component" >> "$SUMMARY_LOG"
            echo '```' >> "$SUMMARY_LOG"

            case "$component" in
                jar)
                    # Extract Maven test failures
                    grep -A 5 "FAILURE\|ERROR\|Failed tests:" "$log_file" 2>/dev/null | head -50 || echo "See log for details"
                    grep -A 5 "FAILURE\|ERROR\|Failed tests:" "$log_file" 2>/dev/null | head -50 >> "$SUMMARY_LOG" || echo "See log for details" >> "$SUMMARY_LOG"
                    ;;
                python_native|python_wrapper)
                    # Extract pytest failures
                    grep -E "^(FAILED|ERROR|=+ FAILURES =+)" "$log_file" 2>/dev/null | head -50 || echo "See log for details"
                    grep -A 10 "^FAILED\|^=+ FAILURES =+" "$log_file" 2>/dev/null | head -50 >> "$SUMMARY_LOG" || echo "See log for details" >> "$SUMMARY_LOG"
                    ;;
                matlab)
                    # Extract MATLAB failures
                    grep -E "(FAILED|Error|Exception)" "$log_file" 2>/dev/null | head -50 || echo "See log for details"
                    grep -E "(FAILED|Error|Exception)" "$log_file" 2>/dev/null | head -50 >> "$SUMMARY_LOG" || echo "See log for details" >> "$SUMMARY_LOG"
                    ;;
            esac

            echo '```' >> "$SUMMARY_LOG"
            echo "" >> "$SUMMARY_LOG"
            echo ""
        fi
    done

    echo "==========================================="
    echo -e "${RED}${BOLD}OVERALL RESULT: $TOTAL_FAILURES COMPONENT(S) FAILED${NC}"
    echo "==========================================="
    echo "" >> "$SUMMARY_LOG"
    echo "**OVERALL RESULT: $TOTAL_FAILURES COMPONENT(S) FAILED**" >> "$SUMMARY_LOG"
else
    echo ""
    echo -e "${GREEN}${BOLD}OVERALL RESULT: ALL TESTS PASSED${NC}"
    echo "==========================================="
    echo "" >> "$SUMMARY_LOG"
    echo "**OVERALL RESULT: ALL TESTS PASSED**" >> "$SUMMARY_LOG"
fi

echo ""
echo "Log files saved in: $LOG_DIR"
echo "Summary report: $SUMMARY_LOG"
echo ""

# Return non-zero exit code if any tests failed
exit $TOTAL_FAILURES
