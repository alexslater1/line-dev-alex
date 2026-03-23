#!/bin/bash

# Alternative wrapper script using sshpass for automated password entry
# Usage: ./upload.sh [-u] [-w]
#   -u: Upload only (skip documentation generation)
#   -w: Website only (upload index.html and solver-*.html pages)

echo "LINE Documentation Upload (Automated)"
echo "====================================="
chmod u+x doc/*.sh
# Parse command line arguments
SKIP_GENERATE=false
WEBSITE_ONLY=false
while getopts "uw" opt; do
    case $opt in
        u)
            WEBSITE_ONLY=true
            SKIP_GENERATE=true
            ;;
        w)
            WEBSITE_ONLY=true
            SKIP_GENERATE=true
            ;;
        \?)
            echo "Usage: $0 [-u] [-w]"
            echo "  -u: Upload only (skip documentation generation)"
            echo "  -w: Website only (upload index.html and solver-*.html pages)"
            exit 1
            ;;
    esac
done

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check for required tools
check_tool() {
    if command -v $1 &> /dev/null; then
        echo -e "${GREEN}✓${NC} $1 is installed"
        return 0
    else
        echo -e "${RED}✗${NC} $1 is not installed"
        return 1
    fi
}

# Function to install sshpass
install_sshpass() {
    echo -e "${YELLOW}Attempting to install sshpass...${NC}"
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if command -v apt-get &> /dev/null; then
            sudo apt-get update && sudo apt-get install -y sshpass
        elif command -v yum &> /dev/null; then
            sudo yum install -y sshpass
        else
            echo -e "${RED}Cannot auto-install sshpass. Please install manually.${NC}"
            return 1
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        if command -v brew &> /dev/null; then
            brew install hudochenkov/sshpass/sshpass
        else
            echo -e "${RED}Please install Homebrew first, then run: brew install hudochenkov/sshpass/sshpass${NC}"
            return 1
        fi
    else
        echo -e "${RED}Unsupported OS. Please install sshpass manually.${NC}"
        return 1
    fi
}

# Check for sshpass
if ! check_tool sshpass; then
    echo
    read -p "Would you like to install sshpass? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        if install_sshpass; then
            echo -e "${GREEN}sshpass installed successfully${NC}"
        else
            echo -e "${RED}Failed to install sshpass. Exiting.${NC}"
            exit 1
        fi
    else
        echo -e "${YELLOW}Alternative: Using expect-based script instead...${NC}"
        doc/upload-with-password.sh
        exit $?
    fi
fi

# Prompt for password once
echo
echo -n "Enter SourceForge password for casaleggg: "
read -s PASSWORD
echo
echo

# SourceForge configuration
SF_USER="casaleggg"
SF_HOST="web.sourceforge.net"
SF_PROJECT="/home/project-web/line-solver"

# Export for use in rsync commands
export SSHPASS="$PASSWORD"

if [ "$SKIP_GENERATE" = false ]; then
    echo
    echo "Generating documentation..."
    echo "============================"
    (cd doc && ./generate.sh)
else
    echo
    echo "Skipping documentation generation (-u flag)"
    echo "============================================"
fi

echo
echo "Starting upload process..."
echo "=========================="

# Function to run rsync with sshpass
# Uses --chmod to set permissions during transfer (SourceForge has restricted shell)
run_rsync() {
    local src=$1
    local dest=$2
    local desc=$3

    echo -e "${YELLOW}Uploading $desc...${NC}"

    if sshpass -e rsync -avz --delete --size-only --chmod=F644,D755 \
        -e "ssh -o StrictHostKeyChecking=no -o ConnectTimeout=30" \
        "$src" "${SF_USER}@${SF_HOST}:${SF_PROJECT}/$dest"; then
        echo -e "${GREEN}✓ $desc uploaded successfully${NC}"
        return 0
    else
        echo -e "${RED}✗ Failed to upload $desc${NC}"
        return 1
    fi
}

# Check if files exist
if [ ! -f "doc/web/index.html" ]; then
    echo -e "${RED}Error: doc/web/index.html not found${NC}"
    exit 1
fi

if [ "$WEBSITE_ONLY" = false ] && [ ! -d "doc/doxygen" ]; then
    echo -e "${RED}Error: doc/doxygen directory not found${NC}"
    echo "Please run doc/generate-doxygen.sh first"
    exit 1
fi


# Upload in sequence
echo

# 1. Upload index.html
if ! run_rsync "doc/web/index.html" "htdocs/" "index.html"; then
    echo -e "${RED}Failed to upload index.html. Aborting.${NC}"
    unset SSHPASS
    exit 1
fi

# Skip remaining uploads if -w flag is set
if [ "$WEBSITE_ONLY" = true ]; then
    echo -e "${YELLOW}Website-only mode: uploading index.html and solver pages${NC}"
    echo

    # Upload solver HTML pages
    for solver_html in doc/web/solver-*.html; do
        if [ -f "$solver_html" ]; then
            if ! run_rsync "$solver_html" "htdocs/" "$(basename $solver_html)"; then
                echo -e "${YELLOW}Warning: Failed to upload $(basename $solver_html)${NC}"
            fi
        fi
    done

    # Upload imperial logo
    if [ -f "doc/web/imperial_logo.png" ]; then
        if ! run_rsync "doc/web/imperial_logo.png" "htdocs/" "imperial_logo.png"; then
            echo -e "${YELLOW}Warning: Failed to upload imperial_logo.png${NC}"
        fi
    fi

    # Upload bibliography.html
    if [ -f "doc/web/bibliography.html" ]; then
        if ! run_rsync "doc/web/bibliography.html" "htdocs/" "bibliography.html"; then
            echo -e "${YELLOW}Warning: Failed to upload bibliography.html${NC}"
        fi
    fi

    # Upload tutorials.html
    if [ -f "doc/web/tutorials.html" ]; then
        if ! run_rsync "doc/web/tutorials.html" "htdocs/" "tutorials.html"; then
            echo -e "${YELLOW}Warning: Failed to upload tutorials.html${NC}"
        fi
    fi

    # Upload tutorial HTML pages
    for tutorial_html in doc/web/tutorial-*.html; do
        if [ -f "$tutorial_html" ]; then
            if ! run_rsync "$tutorial_html" "htdocs/" "$(basename $tutorial_html)"; then
                echo -e "${YELLOW}Warning: Failed to upload $(basename $tutorial_html)${NC}"
            fi
        fi
    done

    # Upload interop.html
    if [ -f "doc/web/interop.html" ]; then
        if ! run_rsync "doc/web/interop.html" "htdocs/" "interop.html"; then
            echo -e "${YELLOW}Warning: Failed to upload interop.html${NC}"
        fi
    fi

    # Upload license.html
    if [ -f "doc/web/license.html" ]; then
        if ! run_rsync "doc/web/license.html" "htdocs/" "license.html"; then
            echo -e "${YELLOW}Warning: Failed to upload license.html${NC}"
        fi
    fi

    # Upload line-index.html
    if [ -f "doc/web/line-index.html" ]; then
        if ! run_rsync "doc/web/line-index.html" "htdocs/" "line-index.html"; then
            echo -e "${YELLOW}Warning: Failed to upload line-index.html${NC}"
        fi
    fi

    # Upload sidebar.html
    if [ -f "doc/web/sidebar.html" ]; then
        if ! run_rsync "doc/web/sidebar.html" "htdocs/" "sidebar.html"; then
            echo -e "${YELLOW}Warning: Failed to upload sidebar.html${NC}"
        fi
    fi

    # Upload CSS directory if it exists
    if [ -d "doc/web/css" ]; then
        if ! run_rsync "doc/web/css/" "htdocs/css/" "CSS files"; then
            echo -e "${YELLOW}Warning: Failed to upload CSS directory${NC}"
        fi
    fi

    # Upload fonts directory if it exists
    if [ -d "doc/web/fonts" ]; then
        if ! run_rsync "doc/web/fonts/" "htdocs/fonts/" "Font files"; then
            echo -e "${YELLOW}Warning: Failed to upload fonts directory${NC}"
        fi
    fi

    # Upload js directory if it exists
    if [ -d "doc/web/js" ]; then
        if ! run_rsync "doc/web/js/" "htdocs/js/" "JavaScript files"; then
            echo -e "${YELLOW}Warning: Failed to upload JavaScript directory${NC}"
        fi
    fi

    # Upload images directory if it exists
    if [ -d "doc/web/images" ]; then
        if ! run_rsync "doc/web/images/" "htdocs/images/" "Image files"; then
            echo -e "${YELLOW}Warning: Failed to upload images directory${NC}"
        fi
    fi

    # Upload img directory if it exists (tutorial images)
    if [ -d "doc/web/img" ]; then
        if ! run_rsync "doc/web/img/" "htdocs/img/" "Tutorial image files"; then
            echo -e "${YELLOW}Warning: Failed to upload img directory${NC}"
        fi
    fi

    echo
    echo -e "${GREEN}✓ Permissions set via rsync --chmod=F644,D755${NC}"
else
    # 1a. Upload index-v2.html (new redesigned version)
    if [ -f "doc/web/index-v2.html" ]; then
        if ! run_rsync "doc/web/index-v2.html" "htdocs/" "index-v2.html"; then
            echo -e "${YELLOW}Warning: Failed to upload index-v2.html${NC}"
        fi
    fi

    # 1b. Upload bibliography.html
    if [ -f "doc/web/bibliography.html" ]; then
        if ! run_rsync "doc/web/bibliography.html" "htdocs/" "bibliography.html"; then
            echo -e "${YELLOW}Warning: Failed to upload bibliography.html${NC}"
        fi
    fi

    echo

    # 1c. Upload solver HTML pages
    for solver_html in doc/web/solver-*.html; do
        if [ -f "$solver_html" ]; then
            if ! run_rsync "$solver_html" "htdocs/" "$(basename $solver_html)"; then
                echo -e "${YELLOW}Warning: Failed to upload $(basename $solver_html)${NC}"
            fi
        fi
    done

    # 1d. Upload imperial logo
    if [ -f "doc/web/imperial_logo.png" ]; then
        if ! run_rsync "doc/web/imperial_logo.png" "htdocs/" "imperial_logo.png"; then
            echo -e "${YELLOW}Warning: Failed to upload imperial_logo.png${NC}"
        fi
    fi

    # 1e. Upload tutorials.html
    if [ -f "doc/web/tutorials.html" ]; then
        if ! run_rsync "doc/web/tutorials.html" "htdocs/" "tutorials.html"; then
            echo -e "${YELLOW}Warning: Failed to upload tutorials.html${NC}"
        fi
    fi

    # 1e2. Upload tutorial HTML pages
    for tutorial_html in doc/web/tutorial-*.html; do
        if [ -f "$tutorial_html" ]; then
            if ! run_rsync "$tutorial_html" "htdocs/" "$(basename $tutorial_html)"; then
                echo -e "${YELLOW}Warning: Failed to upload $(basename $tutorial_html)${NC}"
            fi
        fi
    done

    # 1f. Upload interop.html
    if [ -f "doc/web/interop.html" ]; then
        if ! run_rsync "doc/web/interop.html" "htdocs/" "interop.html"; then
            echo -e "${YELLOW}Warning: Failed to upload interop.html${NC}"
        fi
    fi

    # 1g. Upload license.html
    if [ -f "doc/web/license.html" ]; then
        if ! run_rsync "doc/web/license.html" "htdocs/" "license.html"; then
            echo -e "${YELLOW}Warning: Failed to upload license.html${NC}"
        fi
    fi

    # 1h. Upload line-index.html
    if [ -f "doc/web/line-index.html" ]; then
        if ! run_rsync "doc/web/line-index.html" "htdocs/" "line-index.html"; then
            echo -e "${YELLOW}Warning: Failed to upload line-index.html${NC}"
        fi
    fi

    # 1i. Upload sidebar.html
    if [ -f "doc/web/sidebar.html" ]; then
        if ! run_rsync "doc/web/sidebar.html" "htdocs/" "sidebar.html"; then
            echo -e "${YELLOW}Warning: Failed to upload sidebar.html${NC}"
        fi
    fi

    # 1j. Upload CSS directory if it exists
    if [ -d "doc/web/css" ]; then
        if ! run_rsync "doc/web/css/" "htdocs/css/" "CSS files"; then
            echo -e "${YELLOW}Warning: Failed to upload CSS directory${NC}"
        fi
    fi

    # 1k. Upload fonts directory if it exists
    if [ -d "doc/web/fonts" ]; then
        if ! run_rsync "doc/web/fonts/" "htdocs/fonts/" "Font files"; then
            echo -e "${YELLOW}Warning: Failed to upload fonts directory${NC}"
        fi
    fi

    # 1l. Upload js directory if it exists
    if [ -d "doc/web/js" ]; then
        if ! run_rsync "doc/web/js/" "htdocs/js/" "JavaScript files"; then
            echo -e "${YELLOW}Warning: Failed to upload JavaScript directory${NC}"
        fi
    fi

    # 1m. Upload images directory if it exists
    if [ -d "doc/web/images" ]; then
        if ! run_rsync "doc/web/images/" "htdocs/images/" "Image files"; then
            echo -e "${YELLOW}Warning: Failed to upload images directory${NC}"
        fi
    fi

    # 1n. Upload img directory if it exists (tutorial images)
    if [ -d "doc/web/img" ]; then
        if ! run_rsync "doc/web/img/" "htdocs/img/" "Tutorial image files"; then
            echo -e "${YELLOW}Warning: Failed to upload img directory${NC}"
        fi
    fi

    echo

    # 2. Upload PDF manuals to doc/ directory
    for pdf in doc/LINE-*.pdf; do
        if [ -f "$pdf" ]; then
            if ! run_rsync "$pdf" "htdocs/doc/" "$(basename $pdf)"; then
                echo -e "${YELLOW}Warning: Failed to upload $(basename $pdf)${NC}"
            fi
        fi
    done

    # 2b. Upload cheatsheet PDF
    if [ -f "doc/cheatsheet/LINE-cheatsheet.pdf" ]; then
        if ! run_rsync "doc/cheatsheet/LINE-cheatsheet.pdf" "htdocs/doc/" "LINE-cheatsheet.pdf"; then
            echo -e "${YELLOW}Warning: Failed to upload LINE-cheatsheet.pdf${NC}"
        fi
    else
        echo -e "${YELLOW}Note: Cheatsheet not found (run pdflatex on doc/cheatsheet/LINE-cheatsheet.tex to generate)${NC}"
    fi

    echo

    # 3. Upload Doxygen HTML documentation
    if [ -d "doc/doxygen" ]; then
        echo -e "${YELLOW}Found Doxygen documentation${NC}"
        if ! run_rsync "doc/doxygen/" "htdocs/doxygen/" "Doxygen documentation"; then
            echo -e "${RED}Failed to upload Doxygen documentation${NC}"
            unset SSHPASS
            exit 1
        fi
    else
        echo -e "${YELLOW}Note: Doxygen documentation not found (run doc/generate-doxygen.sh to generate)${NC}"
    fi

    echo

    # 4. Upload Dokka Javadoc documentation if it exists
    if [ -d "doc/javadoc" ]; then
        echo -e "${YELLOW}Found Dokka Javadoc documentation${NC}"
        if ! run_rsync "doc/javadoc/" "htdocs/javadoc/" "Dokka Javadoc"; then
            echo -e "${YELLOW}Warning: Failed to upload Dokka Javadoc${NC}"
        fi
        echo
    else
        echo -e "${YELLOW}Note: Dokka Javadoc not found (run doc/generate-javadoc.sh to generate)${NC}"
        echo
    fi

    # 5. Upload Sphinx Python documentation if it exists
    if [ -d "doc/sphinx" ]; then
        echo -e "${YELLOW}Found Sphinx Python documentation${NC}"
        if ! run_rsync "doc/sphinx/" "htdocs/sphinx/" "Sphinx Python documentation"; then
            echo -e "${YELLOW}Warning: Failed to upload Sphinx documentation${NC}"
        fi
        echo
    else
        echo -e "${YELLOW}Note: Sphinx documentation not found (run doc/generate-sphinx.sh to generate)${NC}"
        echo
    fi

    # 6. Build and upload LDES JAR + native binary via upload-ldes.sh
    SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
    if [ -x "$SCRIPT_DIR/upload-ldes.sh" ]; then
        echo -e "${YELLOW}Running upload-ldes.sh...${NC}"
        "$SCRIPT_DIR/upload-ldes.sh"
        echo
    else
        echo -e "${YELLOW}Note: upload-ldes.sh not found${NC}"
        echo
    fi

    # 7. Build and upload JMT JAR via upload-jmt.sh
    if [ -x "$SCRIPT_DIR/upload-jmt.sh" ]; then
        echo -e "${YELLOW}Running upload-jmt.sh...${NC}"
        "$SCRIPT_DIR/upload-jmt.sh"
        echo
    else
        echo -e "${YELLOW}Note: upload-jmt.sh not found${NC}"
        echo
    fi

    # 8. Permissions already set via rsync --chmod=F644,D755
    echo -e "${GREEN}✓ Permissions set via rsync --chmod=F644,D755${NC}"
fi

# Clean up
unset SSHPASS

echo
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}Upload completed successfully!${NC}"
echo -e "${GREEN}========================================${NC}"
echo
echo "Documentation is now available at:"
echo "  Website: https://line-solver.sourceforge.io/"
echo "  Doxygen Docs: https://line-solver.sourceforge.io/doxygen/"
echo "  Javadoc API: https://line-solver.sourceforge.io/javadoc/"
echo "  Sphinx Python Docs: https://line-solver.sourceforge.io/sphinx/"
echo "  Manuals: https://line-solver.sourceforge.io/doc/LINE-*.pdf"

# Prompt user for broken link check (default: no)
RUN_BROKEN_LINK_CHECK="false"
if [ "$SKIP_BROKEN_LINK_CHECK" != "true" ]; then
    echo
    read -p "Run broken link check? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        RUN_BROKEN_LINK_CHECK="true"
        echo "========================================="
        echo "Checking for Broken Links"
        echo "========================================="
        echo "Waiting 10 seconds for website to update..."
        sleep 10
    fi
fi

check_broken_links() {
    local base_url="https://line-solver.sourceforge.net"
    local timeout_sec=10

    echo "Starting broken link check on $base_url"
    echo "Checking key documentation URLs (${timeout_sec}s timeout per URL)..."
    echo ""

    # Check if wget is available
    if ! command -v wget &> /dev/null; then
        echo -e "${YELLOW}⚠ wget is not installed. Cannot check for broken links.${NC}"
        echo "Install wget to enable broken link checking:"
        echo "  Ubuntu/Debian: sudo apt-get install wget"
        echo "  MacOS: brew install wget"
        return 1
    fi

    # Define all URLs to check
    local urls=(
        "$base_url/"
        "$base_url/doxygen/"
        "$base_url/javadoc/"
        "$base_url/sphinx/"
        "$base_url/doc/"
        "$base_url/doc/LINE-matlab.pdf"
        "$base_url/doc/LINE-java.pdf"
        "$base_url/doc/LINE-python.pdf"
        "$base_url/doc/LINE-kotlin.pdf"
        "$base_url/doc/LINE-cheatsheet.pdf"
        "$base_url/solver-auto.html"
        "$base_url/solver-ctmc.html"
        "$base_url/solver-des.html"
        "$base_url/solver-env.html"
        "$base_url/solver-fld.html"
        "$base_url/solver-jmt.html"
        "$base_url/solver-ln.html"
        "$base_url/solver-lqns.html"
        "$base_url/solver-mam.html"
        "$base_url/solver-mva.html"
        "$base_url/solver-nc.html"
        "$base_url/solver-qns.html"
        "$base_url/solver-ssa.html"
        "$base_url/index-v2.html"
        "$base_url/tutorials.html"
        "$base_url/tutorial-01.html"
        "$base_url/tutorial-02.html"
        "$base_url/tutorial-03.html"
        "$base_url/tutorial-04.html"
        "$base_url/tutorial-05.html"
        "$base_url/tutorial-06.html"
        "$base_url/tutorial-07.html"
        "$base_url/tutorial-08.html"
        "$base_url/tutorial-09.html"
        "$base_url/tutorial-10.html"
        "$base_url/tutorial-11.html"
        "$base_url/interop.html"
        "$base_url/license.html"
        "$base_url/line-index.html"
        "$base_url/bibliography.html"
    )

    local total=${#urls[@]}
    local current=0
    local ok_count=0
    local fail_count=0
    local failed_urls=()

    # Check each URL with timeout and progressive output
    for url in "${urls[@]}"; do
        ((current++))
        printf "[%2d/%2d] Checking: %s ... " "$current" "$total" "$url"

        if timeout "$timeout_sec" wget --spider -q --timeout=5 --tries=2 "$url" 2>/dev/null; then
            echo -e "${GREEN}OK${NC}"
            ((ok_count++))
        else
            echo -e "${RED}FAILED${NC}"
            ((fail_count++))
            failed_urls+=("$url")
        fi
    done

    # Summary
    echo ""
    echo "========================================="
    echo "Link Check Summary"
    echo "========================================="
    echo -e "Total checked: $total"
    echo -e "  ${GREEN}✓ OK:${NC} $ok_count"
    echo -e "  ${RED}✗ Failed:${NC} $fail_count"

    if [ "$fail_count" -gt 0 ]; then
        echo ""
        echo -e "${RED}Failed URLs:${NC}"
        for url in "${failed_urls[@]}"; do
            echo "  - $url"
        done
    else
        echo ""
        echo -e "${GREEN}✓ All links are accessible!${NC}"
    fi
}

# Run the broken link check only if user requested it
if [ "$RUN_BROKEN_LINK_CHECK" = "true" ]; then
    check_broken_links
    export BROKEN_LINK_CHECK_DONE="true"
fi

cp doc/latex/manual.tex ~/dropbox/Apps/Overleaf/LINE/

echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}Upload and Verification Complete${NC}"
echo -e "${GREEN}========================================${NC}"
