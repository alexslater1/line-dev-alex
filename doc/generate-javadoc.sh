#!/bin/bash

# Generate documentation for the LINE jar API using Dokka
# This tool handles both Kotlin and Java code properly
# Run this script from the doc/ directory

echo "Generating API documentation for LINE jar..."

# Clean previous documentation
rm -rf javadoc

# Change to jar directory for Maven operations
cd ../jar

# Generate documentation using Dokka with Javadoc format
mvn dokka:javadoc

if [ $? -eq 0 ]; then
    echo "Documentation generated successfully!"
    echo "View the documentation at: doc/javadoc/index.html"
else
    echo "Documentation generation failed. Please check the error messages above."
    exit 1
fi
