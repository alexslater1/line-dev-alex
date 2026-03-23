#!/bin/bash

# Simple upload script for LINE solver documentation to SourceForge
# Works with SourceForge's restricted shell environment

# Configuration
SF_USER="${SF_USER:-casaleggg}"
SF_PROJECT="line-solver"
SF_HOST="web.sourceforge.net"
SF_WEB_PATH="/home/project-web/${SF_PROJECT}/htdocs"

echo "============================================================"
echo "LINE Documentation Upload Script (Simple Version)"
echo "============================================================"
echo "Project: ${SF_PROJECT}"
echo "User: ${SF_USER}"
echo "Target: ${SF_USER}@${SF_HOST}:${SF_WEB_PATH}"
echo ""

# Check if user is set
if [ -z "$SF_USER" ]; then
    echo "Error: SourceForge username not specified."
    echo "Set SF_USER environment variable or run as: SF_USER=username $0"
    exit 1
fi

# SSH Key setup (optional but recommended)
SSH_KEY_PATH="${SSH_KEY_PATH:-$HOME/.ssh/id_rsa}"
SSH_OPTS=""

if [ -f "$SSH_KEY_PATH" ]; then
    echo "✓ Using SSH key: $SSH_KEY_PATH"
    SSH_OPTS="-i $SSH_KEY_PATH"
else
    echo "⚠ SSH key not found at: $SSH_KEY_PATH"
    echo "Will use password authentication (you'll be prompted for each transfer)"
    echo "To avoid multiple password prompts, set up SSH keys:"
    echo "  1. Generate key: ssh-keygen -t rsa"
    echo "  2. Upload to SF: https://sourceforge.net/auth/shell_services"
fi
echo ""

# Test connectivity
echo "Testing connectivity to ${SF_HOST}..."
if ! ping -c 1 ${SF_HOST} >/dev/null 2>&1; then
    echo "✗ Cannot reach ${SF_HOST} - check your internet connection"
    exit 1
fi
echo "✓ Host ${SF_HOST} is reachable"
echo ""

echo "============================================================"
echo "Uploading Documentation"
echo "============================================================"
echo "Note: SourceForge uses a restricted shell that only allows file transfers."
echo "You may see a 'restricted Shell Account' message - this is normal."
echo ""

# Check what files are available
MANUAL_FILES=(
    "LINE-java.pdf"
    "LINE-kotlin.pdf" 
    "LINE-matlab.pdf"
    "LINE-python.pdf"
)

echo "Checking for files to upload..."
HTML_EXISTS=false
if [ -d "html" ]; then
    echo "✓ Found HTML documentation directory"
    HTML_EXISTS=true
else
    echo "⚠ HTML documentation directory not found"
fi

PDF_COUNT=0
for pdf in "${MANUAL_FILES[@]}"; do
    if [ -f "$pdf" ]; then
        echo "✓ Found $pdf"
        PDF_COUNT=$((PDF_COUNT + 1))
    fi
done

if [ -f "index.html" ]; then
    echo "✓ Found index.html"
fi

if [ "$HTML_EXISTS" = "false" ] && [ $PDF_COUNT -eq 0 ] && [ ! -f "index.html" ]; then
    echo ""
    echo "✗ No documentation files found to upload!"
    echo "Please generate documentation first:"
    echo "  ./generate-manuals.sh"
    echo "  ./generate-doxygen.sh"
    exit 1
fi

echo ""
echo "Starting uploads..."
echo ""

# Upload index.html first
if [ -f "index.html" ]; then
    echo "1. Uploading index.html..."
    
    if [ -n "$SSH_OPTS" ]; then
        rsync -avz -e "ssh $SSH_OPTS" "index.html" ${SF_USER}@${SF_HOST}:${SF_WEB_PATH}/
    else
        rsync -avz "index.html" ${SF_USER}@${SF_HOST}:${SF_WEB_PATH}/
    fi
    
    if [ $? -eq 0 ]; then
        echo "✓ index.html uploaded"
    else
        echo "✗ Failed to upload index.html"
    fi
    echo ""
fi

# Upload PDF manuals second
if [ $PDF_COUNT -gt 0 ]; then
    echo "2. Uploading PDF manuals..."
    for pdf in "${MANUAL_FILES[@]}"; do
        if [ -f "$pdf" ]; then
            echo "  Uploading $pdf..."
            
            if [ -n "$SSH_OPTS" ]; then
                rsync -avz -e "ssh $SSH_OPTS" "$pdf" ${SF_USER}@${SF_HOST}:${SF_WEB_PATH}/doc/
            else
                rsync -avz "$pdf" ${SF_USER}@${SF_HOST}:${SF_WEB_PATH}/doc/
            fi
            
            if [ $? -eq 0 ]; then
                echo "  ✓ $pdf uploaded"
            else
                echo "  ✗ Failed to upload $pdf"
            fi
        fi
    done
    echo ""
fi

# Upload Doxygen HTML documentation last
if [ "$HTML_EXISTS" = "true" ]; then
    echo "3. Uploading Doxygen HTML documentation (this may take a while)..."
    echo "Command: rsync -avz --delete html/ to ${SF_WEB_PATH}/doc/"
    
    if [ -n "$SSH_OPTS" ]; then
        rsync -avz --delete -e "ssh $SSH_OPTS" html/ ${SF_USER}@${SF_HOST}:${SF_WEB_PATH}/doc/
    else
        rsync -avz --delete html/ ${SF_USER}@${SF_HOST}:${SF_WEB_PATH}/doc/
    fi
    
    if [ $? -eq 0 ]; then
        echo "✓ Doxygen HTML documentation uploaded successfully"
    else
        echo "✗ Failed to upload Doxygen HTML documentation"
        echo "Try uploading manually with:"
        echo "  rsync -avz --delete html/ ${SF_USER}@${SF_HOST}:${SF_WEB_PATH}/doc/"
    fi
    echo ""
fi

echo "============================================================"
echo "Upload Process Completed"
echo "============================================================"
echo ""
echo "Documentation URLs:"
echo "  🌐 Website: http://line-solver.sf.net/"
echo "  📚 API docs: http://line-solver.sf.net/doc/"
echo ""

if [ $PDF_COUNT -gt 0 ]; then
    echo "Available manuals:"
    for pdf in "${MANUAL_FILES[@]}"; do
        if [ -f "$pdf" ]; then
            echo "  📄 http://line-solver.sf.net/doc/$pdf"
        fi
    done
    echo ""
fi

echo "📝 Note: Changes may take a few minutes to appear on the website."
echo ""

# Check for broken links (skip if already done by another script)
if [ "$SKIP_BROKEN_LINK_CHECK" != "true" ]; then
    echo "============================================================"
    echo "Checking for Broken Links"
    echo "============================================================"
    echo "Waiting 10 seconds for website to update..."
    sleep 10
fi

check_broken_links() {
    local base_url="https://line-solver.sourceforge.net"
    local log_file="broken_links_check.log"
    
    echo "Starting broken link check on $base_url"
    echo "This may take a few minutes..."
    echo ""
    
    # Check if wget is available
    if ! command -v wget &> /dev/null; then
        echo "⚠ wget is not installed. Cannot check for broken links."
        echo "Install wget to enable broken link checking:"
        echo "  Ubuntu/Debian: sudo apt-get install wget"
        echo "  MacOS: brew install wget"
        return 1
    fi
    
    # Use wget to spider the website and check for broken links
    # --spider: Check links without downloading
    # --recursive: Follow links recursively
    # --level=3: Limit recursion depth
    # --no-verbose: Less verbose output
    # --output-file: Log output to file
    # --no-directories: Don't create local directory structure
    # --execute robots=off: Ignore robots.txt
    # --wait=0.5: Wait between requests to be polite
    # --reject-regex: Skip certain file types that might cause issues
    
    wget --spider \
         --recursive \
         --level=3 \
         --no-verbose \
         --output-file="$log_file" \
         --no-directories \
         --execute robots=off \
         --wait=0.5 \
         --reject-regex='(\.tar\.gz|\.zip|\.tar\.bz2|\.exe|\.dmg)' \
         "$base_url" 2>&1
    
    # Analyze the log for broken links
    echo "Analyzing results..."
    echo ""
    
    # Count different types of issues
    local broken_count=$(grep -c "404 Not Found" "$log_file" 2>/dev/null || echo "0")
    local error_count=$(grep -c "ERROR" "$log_file" 2>/dev/null || echo "0")
    
    if [ "$broken_count" -gt 0 ] || [ "$error_count" -gt 0 ]; then
        echo "⚠ Found potential issues:"
        echo "  - 404 errors: $broken_count"
        echo "  - Other errors: $error_count"
        echo ""
        echo "Broken links found (404 errors):"
        grep -B2 "404 Not Found" "$log_file" | grep "^--" | sed 's/--[0-9]*-- /  /' | sort -u || true
        echo ""
        echo "Other errors:"
        grep "ERROR" "$log_file" | head -10 || true
        echo ""
        echo "Full log saved to: $log_file"
    else
        echo "✓ No broken links detected!"
        echo "All links appear to be working correctly."
        # Clean up log file if no errors
        rm -f "$log_file"
    fi
    
    echo ""
    
    # Also do a quick check on specific important URLs
    echo "Verifying key documentation URLs..."
    local urls=(
        "$base_url/"
        "$base_url/doc/"
        "$base_url/doc/LINE-matlab.pdf"
        "$base_url/doc/LINE-java.pdf"
        "$base_url/doc/LINE-python.pdf"
        "$base_url/doc/LINE-kotlin.pdf"
    )
    
    for url in "${urls[@]}"; do
        if wget --spider -q "$url" 2>/dev/null; then
            echo "✓ $url"
        else
            echo "✗ $url (not accessible)"
        fi
    done
}

# Run the broken link check (unless skipped)
if [ "$SKIP_BROKEN_LINK_CHECK" != "true" ]; then
    check_broken_links
fi

echo ""
echo "============================================================"
echo "Upload and Verification Complete"
echo "============================================================"
echo ""

# Provide helpful information about SSH keys if not configured
if [ ! -f "$SSH_KEY_PATH" ]; then
    echo "============================================================"
    echo "TIP: Set up SSH key authentication to avoid password prompts"
    echo "============================================================"
    echo "1. Generate an SSH key (if you don't have one):"
    echo "   ssh-keygen -t rsa -b 4096"
    echo ""
    echo "2. Upload your public key to SourceForge:"
    echo "   Visit: https://sourceforge.net/auth/shell_services"
    echo "   Paste contents of: ~/.ssh/id_rsa.pub"
    echo ""
    echo "3. Next time, the script will use key authentication automatically"
    echo "============================================================"
fi