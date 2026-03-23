#!/usr/bin/env python3
"""
Generate detailed API documentation entries for api_algorithms.dox
Extracts function information from MATLAB files and formats them for Doxygen
"""

import re
import sys
import os
from pathlib import Path

def extract_function_info(mfile_path):
    """Extract function signature and documentation from a MATLAB file"""
    with open(mfile_path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    info = {
        'name': Path(mfile_path).stem,
        'signature': None,
        'outputs': [],
        'inputs': [],
        'brief': '',
        'params': {},
        'returns': {}
    }

    # Extract function signature
    func_match = re.search(r'^function\s+(?:\[([^\]]+)\]\s*=\s*)?(\w+)\s*\(([^)]*)\)', content, re.MULTILINE)
    if func_match:
        outputs = func_match.group(1)
        func_name = func_match.group(2)
        inputs = func_match.group(3)

        info['signature'] = func_match.group(0)

        if outputs:
            info['outputs'] = [o.strip() for o in outputs.split(',')]

        if inputs:
            info['inputs'] = [i.strip() for i in inputs.split(',') if i.strip()]

    # Extract brief description from @brief or comments
    brief_match = re.search(r'@brief\s+(.+)', content)
    if brief_match:
        info['brief'] = brief_match.group(1).strip()
    else:
        # Try to find from % comments near function
        lines = content.split('\n')
        for i, line in enumerate(lines):
            if line.startswith('function'):
                # Look at previous and next few lines for description
                for j in range(max(0, i-5), min(len(lines), i+5)):
                    if lines[j].startswith('%') and len(lines[j]) > 3:
                        desc = lines[j].lstrip('%').strip()
                        if len(desc) > 10 and not desc.startswith('@'):
                            info['brief'] = desc
                            break
                break

    # Extract parameters
    for param_match in re.finditer(r'@param\s+(\w+)\s+(.+)', content):
        param_name = param_match.group(1)
        param_desc = param_match.group(2).strip()
        info['params'][param_name] = param_desc

    # Extract returns
    for return_match in re.finditer(r'@return\s+(\w+)\s+(.+)', content):
        return_name = return_match.group(1)
        return_desc = return_match.group(2).strip()
        info['returns'][return_name] = return_desc

    return info

def generate_dox_entry(info, brief_override=None):
    """Generate a .dox file entry for a function"""
    func_name = info['name']
    brief = brief_override if brief_override else info['brief']

    # Start with function reference
    entry = f'- @ref {func_name}.m "{func_name}" - {brief}\n\n'

    # Add syntax
    entry += '  <b>Syntax:</b>\n'
    entry += '  @code\n'

    if info['outputs'] and info['inputs']:
        if len(info['outputs']) == 1:
            entry += f'  {info["outputs"][0]} = {func_name}({", ".join(info["inputs"])})\n'
        else:
            entry += f'  [{", ".join(info["outputs"])}] = {func_name}({", ".join(info["inputs"])})\n'
    elif info['signature']:
        entry += f'  {info["signature"]}\n'
    else:
        entry += f'  {func_name}(...)\n'

    entry += '  @endcode\n\n'

    # Add parameters table
    if info['inputs'] or info['params']:
        entry += '  <b>Parameters:</b>\n'
        entry += '  <table>\n'
        entry += '  <tr><th>Name</th><th>Description</th></tr>\n'

        for param in info['inputs']:
            param_clean = param.replace('varargin', 'options').replace('varargout', 'outputs')
            desc = info['params'].get(param, 'Parameter description')
            entry += f'  <tr><td>{param_clean}</td><td>{desc}</td></tr>\n'

        entry += '  </table>\n\n'

    # Add returns table
    if info['outputs'] or info['returns']:
        entry += '  <b>Returns:</b>\n'
        entry += '  <table>\n'
        entry += '  <tr><th>Name</th><th>Description</th></tr>\n'

        for output in info['outputs']:
            desc = info['returns'].get(output, 'Return value')
            entry += f'  <tr><td>{output}</td><td>{desc}</td></tr>\n'

        entry += '  </table>\n\n'

    # Add description
    if brief:
        entry += f'  <b>Description:</b> {brief}\n'

    return entry

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python3 generate-api-docs.py <matlab_file.m> [brief_description]")
        sys.exit(1)

    mfile = sys.argv[1]
    brief_override = sys.argv[2] if len(sys.argv) > 2 else None

    if not os.path.exists(mfile):
        print(f"Error: File not found: {mfile}")
        sys.exit(1)

    info = extract_function_info(mfile)
    entry = generate_dox_entry(info, brief_override)

    print(entry)
