#!/usr/bin/env python3
"""
Generate detailed class method documentation for api_core.dox
Extracts methods from MATLAB class files and formats them for Doxygen
"""

import re
import sys
import os
from pathlib import Path

def extract_class_methods(class_file_path):
    """Extract all public methods from a MATLAB class file"""
    with open(class_file_path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    class_name = Path(class_file_path).stem

    methods = {
        'class_name': class_name,
        'brief': '',
        'methods': []
    }

    # Extract class brief description
    brief_match = re.search(r'%\s*@brief\s+(.+)', content)
    if brief_match:
        methods['brief'] = brief_match.group(1).strip()

    # Find all function definitions (public methods)
    # Look for: function [outputs] = methodName(inputs) or function methodName(inputs)
    method_pattern = r'function\s+(?:\[([^\]]+)\]\s*=\s*)?(\w+)\s*\(([^)]*)\)'

    for match in re.finditer(method_pattern, content):
        outputs = match.group(1)
        method_name = match.group(2)
        inputs = match.group(3)

        # Skip constructor (same name as class) and private methods starting with _
        if method_name == class_name or method_name.startswith('_'):
            continue

        method_info = {
            'name': method_name,
            'inputs': [],
            'outputs': [],
            'brief': ''
        }

        if outputs:
            method_info['outputs'] = [o.strip() for o in outputs.split(',')]

        if inputs:
            method_info['inputs'] = [i.strip() for i in inputs.split(',') if i.strip() and i.strip() != 'self']

        # Try to find brief description for this method
        # Look backwards from function definition for comment
        lines = content[:match.start()].split('\n')
        for line in reversed(lines[-10:]):  # Check last 10 lines before method
            if '%' in line and not line.strip().startswith('%%'):
                desc = line.split('%', 1)[1].strip()
                if len(desc) > 5 and not desc.startswith('@'):
                    method_info['brief'] = desc
                    break

        methods['methods'].append(method_info)

    return methods

def generate_class_dox_entry(class_info):
    """Generate .dox entry for a class with its methods"""
    class_name = class_info['class_name']
    brief = class_info['brief'] if class_info['brief'] else f"{class_name} class"

    entry = f'\n<b>{class_name}</b> - {brief}\n\n'

    if class_info['methods']:
        entry += '<table>\n'
        entry += '<tr><th>Method</th><th>Description</th></tr>\n'

        for method in class_info['methods']:
            method_name = method['name']
            method_brief = method['brief'] if method['brief'] else 'Method description'

            # Format signature
            if method['outputs'] and method['inputs']:
                if len(method['outputs']) == 1:
                    sig = f"{method['outputs'][0]} = {method_name}({', '.join(method['inputs'])})"
                else:
                    sig = f"[{', '.join(method['outputs'])}] = {method_name}({', '.join(method['inputs'])})"
            elif method['inputs']:
                sig = f"{method_name}({', '.join(method['inputs'])})"
            else:
                sig = method_name

            entry += f'<tr><td><code>{sig}</code></td><td>{method_brief}</td></tr>\n'

        entry += '</table>\n'

    return entry

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python3 generate-class-docs.py <class_file.m>")
        sys.exit(1)

    class_file = sys.argv[1]

    if not os.path.exists(class_file):
        print(f"Error: File not found: {class_file}")
        sys.exit(1)

    class_info = extract_class_methods(class_file)
    entry = generate_class_dox_entry(class_info)

    print(entry)
