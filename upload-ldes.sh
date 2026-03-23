#!/bin/bash

# Build and upload LDES JAR + native binary via ldes-dev.git
# Usage: ./upload-ldes.sh [--no-native]
#   --no-native: Skip GraalVM native binary build (JAR only)

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
LDES_DIR="$SCRIPT_DIR/../ldes-dev.git"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

if [ ! -d "$LDES_DIR" ]; then
    echo -e "${RED}Error: ldes-dev.git not found at $LDES_DIR${NC}"
    exit 1
fi

# Set GRAALVM_HOME if not already set (needed for native-image)
if [ -z "$GRAALVM_HOME" ]; then
    if [ -d "$HOME/.sdkman/candidates/java/21.0.2-graalce" ]; then
        export GRAALVM_HOME="$HOME/.sdkman/candidates/java/21.0.2-graalce"
    fi
fi

# 1. Deploy jline to local Maven cache
echo -e "${YELLOW}Deploying jline to local Maven cache...${NC}"
(cd "$SCRIPT_DIR/jar" && /usr/bin/mvn clean deploy -P jar-mvn -q)
if [ $? -ne 0 ]; then
    echo -e "${RED}Error: jline Maven deploy failed${NC}"
    exit 1
fi
echo -e "${GREEN}✓ jline deployed${NC}"
echo

# 2. Build LDES JAR (and optionally native binary) in ldes-dev.git
if [ "$1" = "--no-native" ]; then
    echo -e "${YELLOW}Building LDES JAR...${NC}"
    (cd "$LDES_DIR/jar" && /usr/bin/mvn clean verify -P b)
else
    echo -e "${YELLOW}Building LDES JAR + native binary...${NC}"
    (cd "$LDES_DIR/jar" && GRAALVM_HOME="$GRAALVM_HOME" /usr/bin/mvn clean verify -P native)
fi
if [ $? -ne 0 ]; then
    echo -e "${RED}Error: LDES build failed${NC}"
    exit 1
fi
echo -e "${GREEN}✓ LDES build complete${NC}"
echo

# 3. Upload via ldes-dev.git/upload.sh (pass password if available)
if [ -n "$SSHPASS" ]; then
    (cd "$LDES_DIR" && ./upload.sh "$SSHPASS")
else
    (cd "$LDES_DIR" && ./upload.sh)
fi
