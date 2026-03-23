#!/bin/bash
# Post-process doxygen HTML to hide source code on API function pages

echo "Hiding source code in API function documentation pages..."

# Find all *_source.html files in API directories
find doxygen -name "*_source.html" | while read file; do
    # Check if it's an API file (contains cache, pfqn, mam, sn, ctmc, dtmc pattern)
    if echo "$file" | grep -qE "(cache__|pfqn__|qbd__|map__|aph__|mmap__|sn__|ctmc__|dtmc__|lossn__|lsn__|weakly)"; then
        # Check if CSS is already present to avoid duplicates
        if ! grep -q "\.fragment { display: none" "$file"; then
            # Add CSS to hide the source code fragment section
            sed -i 's|</head>|<style>.fragment { display: none !important; }</style>\n</head>|' "$file"
            echo "Processed: $file"
        fi
    fi
done

echo "Done! Source code hidden in API function pages."
