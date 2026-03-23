#!/bin/bash

# Build JMT in jmt-thinned.git and upload JMT.jar to SourceForge latest/
# Usage: ./upload-jmt.sh
# Called from upload.sh with SSHPASS already set, or standalone (prompts for password)

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
JMT_DIR="$SCRIPT_DIR/../jmt-thinned.git"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

if [ ! -d "$JMT_DIR" ]; then
    echo -e "${RED}Error: jmt-thinned.git not found at $JMT_DIR${NC}"
    exit 1
fi

# 1. Build JMT JAR
echo -e "${YELLOW}Building JMT JAR in jmt-thinned.git...${NC}"
(cd "$JMT_DIR" && /usr/bin/mvn clean package -q)
if [ $? -ne 0 ]; then
    echo -e "${RED}Error: JMT build failed${NC}"
    exit 1
fi

JMT_JAR="$JMT_DIR/target/JMT.jar"
if [ ! -f "$JMT_JAR" ]; then
    echo -e "${RED}Error: JMT.jar not found at $JMT_JAR${NC}"
    exit 1
fi
echo -e "${GREEN}✓ JMT build complete${NC} ($(du -h "$JMT_JAR" | cut -f1))"
echo

# 2. Get password: from SSHPASS env, from argument, or prompt
if [ -z "$SSHPASS" ]; then
    if [ -n "$1" ]; then
        export SSHPASS="$1"
    else
        if ! command -v sshpass &> /dev/null; then
            echo -e "${RED}Error: sshpass is not installed${NC}"
            echo "Install with: sudo apt-get install sshpass"
            exit 1
        fi
        echo -n "Enter SourceForge password for casaleggg: "
        read -s PASSWORD
        echo
        echo
        export SSHPASS="$PASSWORD"
    fi
fi

# SourceForge configuration
SF_USER="casaleggg"
SF_HOST="web.sourceforge.net"
SF_PROJECT="/home/project-web/line-solver"

# 3. Upload JMT.jar
echo -e "${YELLOW}Uploading JMT.jar to latest/...${NC}"

if sshpass -e rsync -avz --size-only --chmod=F644,D755 \
    -e "ssh -o StrictHostKeyChecking=no -o ConnectTimeout=30" \
    "$JMT_JAR" "${SF_USER}@${SF_HOST}:${SF_PROJECT}/htdocs/latest/JMT.jar"; then
    echo -e "${GREEN}✓ JMT.jar uploaded successfully${NC}"
else
    echo -e "${RED}✗ Failed to upload JMT.jar${NC}"
    exit 1
fi

echo
echo -e "${GREEN}Upload complete!${NC}"
echo "Available at: https://line-solver.sourceforge.io/latest/JMT.jar"
