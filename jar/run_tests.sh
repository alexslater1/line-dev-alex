#!/bin/bash
# Run all Java/Kotlin tests for LINE solver
# Usage: ./run_tests.sh [optional: specific test class]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Create logs directory if it doesn't exist
LOG_DIR="$SCRIPT_DIR/logs"
mkdir -p "$LOG_DIR"

# Generate timestamped log filename
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
LOG_FILE="$LOG_DIR/test_run_${TIMESTAMP}.txt"

echo "=========================================="
echo "LINE Solver Java/Kotlin Test Runner"
echo "=========================================="
echo "Log file: $LOG_FILE"
echo ""

# Check if Maven is available
if ! command -v mvn &> /dev/null; then
    echo -e "${RED}Error: mvn not found${NC}"
    exit 1
fi

# Check if running specific test class
if [ $# -gt 0 ]; then
    echo -e "${YELLOW}Running specific test: $1${NC}"
    /usr/bin/mvn test -DskipTests=false -Dtest="$1" 2>&1 | tee "$LOG_FILE"
    TEST_EXIT_CODE=${PIPESTATUS[0]}
else
    # Run all tests
    echo -e "${YELLOW}Running all tests...${NC}"
    echo ""

    # Run tests using Maven (output goes to /tmp/target/ by default)
    # Output is displayed in real-time and saved to log file
    /usr/bin/mvn test -DskipTests=false 2>&1 | tee "$LOG_FILE"
    TEST_EXIT_CODE=${PIPESTATUS[0]}
fi

echo ""
echo "=========================================="
if [ $TEST_EXIT_CODE -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
else
    echo -e "${RED}Some tests failed (exit code: $TEST_EXIT_CODE)${NC}"
fi
echo "Log saved to: $LOG_FILE"
echo "=========================================="

exit $TEST_EXIT_CODE
