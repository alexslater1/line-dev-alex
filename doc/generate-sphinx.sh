#!/bin/bash

# Script to generate Sphinx documentation for LINE Python implementation
# Usage: ./generate-sphinx.sh

echo "LINE Python Documentation Generation (Sphinx)"
echo "============================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PYTHON_DIR="$SCRIPT_DIR/../python"
SPHINX_DIR="$SCRIPT_DIR/config.sphinx"
SPHINX_BUILD_DIR="$SCRIPT_DIR/sphinx"

# Function to check if a command exists
check_command() {
    if command -v $1 &> /dev/null; then
        echo -e "${GREEN}✓${NC} $1 is installed"
        return 0
    else
        echo -e "${RED}✗${NC} $1 is not installed"
        return 1
    fi
}

# Function to install Python packages
install_sphinx() {
    echo -e "${YELLOW}Installing Sphinx and dependencies...${NC}"
    pip install sphinx sphinx-rtd-theme sphinx-autodoc-typehints sphinxcontrib-napoleon autodoc 2>/dev/null || \
    pip3 install sphinx sphinx-rtd-theme sphinx-autodoc-typehints sphinxcontrib-napoleon autodoc
}

# Check for Python
echo "Checking dependencies..."
if ! check_command python3; then
    echo -e "${RED}Python 3 is required but not installed.${NC}"
    exit 1
fi

# Check for pip
if ! check_command pip3 && ! check_command pip; then
    echo -e "${RED}pip is required but not installed.${NC}"
    exit 1
fi

# Check for Sphinx - try different methods
if command -v sphinx-build &> /dev/null; then
    SPHINX_CMD="sphinx-build"
elif python3 -m sphinx --version &> /dev/null; then
    SPHINX_CMD="python3 -m sphinx"
else
    echo -e "${RED}Sphinx is not installed.${NC}"
    echo
    echo "Please install Sphinx using one of these methods:"
    echo
    echo "  1. Using apt (recommended):"
    echo "     sudo apt update"
    echo "     sudo apt install python3-sphinx python3-sphinx-rtd-theme"
    echo
    echo "  2. Using pip with --break-system-packages:"
    echo "     pip3 install --break-system-packages sphinx sphinx-rtd-theme sphinx-autodoc-typehints sphinxcontrib-napoleon"
    echo
    echo "  3. Using pip with --user flag:"
    echo "     pip3 install --user sphinx sphinx-rtd-theme sphinx-autodoc-typehints sphinxcontrib-napoleon"
    echo
    exit 1
fi

# Create Sphinx directory if it doesn't exist
if [ ! -d "$SPHINX_DIR" ]; then
    echo -e "${YELLOW}Creating Sphinx configuration...${NC}"
    mkdir -p "$SPHINX_DIR"
    
    # Create conf.py
    cat > "$SPHINX_DIR/conf.py" << 'EOF'
# Configuration file for the Sphinx documentation builder.

import os
import sys

# Add Python source directory to path
sys.path.insert(0, os.path.abspath('../../python'))
sys.path.insert(0, os.path.abspath('../../python/line_solver'))

# Project information
project = 'LINE Solver (Python)'
copyright = '2025, Imperial College London'
author = 'Giuliano Casale and contributors'
release = '3.0.0'

# General configuration
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.napoleon',
    'sphinx.ext.viewcode',
    'sphinx.ext.intersphinx',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx_rtd_theme',
]

# Napoleon settings for Google and NumPy docstring styles
napoleon_google_docstring = True
napoleon_numpy_docstring = True
napoleon_include_init_with_doc = True
napoleon_include_private_with_doc = False
napoleon_include_special_with_doc = True
napoleon_use_admonition_for_examples = False
napoleon_use_admonition_for_notes = False
napoleon_use_admonition_for_references = False
napoleon_use_ivar = False
napoleon_use_param = True
napoleon_use_rtype = True
napoleon_preprocess_types = True

# Autodoc settings
autodoc_default_options = {
    'members': True,
    'member-order': 'bysource',
    'special-members': '__init__',
    'undoc-members': True,
    'exclude-members': '__weakref__'
}

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
exclude_patterns = []

# The theme to use for HTML and HTML Help pages
html_theme = 'sphinx_rtd_theme'

# Theme options
html_theme_options = {
    'collapse_navigation': False,
    'sticky_navigation': True,
    'navigation_depth': 4,
    'includehidden': True,
    'titles_only': False,
    'display_version': True,
    'prev_next_buttons_location': 'both',
}

# Add any paths that contain custom static files
html_static_path = ['_static']

# The suffix(es) of source filenames
source_suffix = '.rst'

# The master toctree document
master_doc = 'index'

# Intersphinx mapping
intersphinx_mapping = {
    'python': ('https://docs.python.org/3', None),
    'numpy': ('https://numpy.org/doc/stable/', None),
}

# Output file base name for HTML help builder
htmlhelp_basename = 'LINESolverPythondoc'

# LaTeX output
latex_elements = {
    'papersize': 'letterpaper',
    'pointsize': '10pt',
    'preamble': '',
    'fncychap': '\\usepackage[Bjornstrup]{fncychap}',
}

latex_documents = [
    (master_doc, 'LINESolverPython.tex', 'LINE Solver Python Documentation',
     'Giuliano Casale', 'manual'),
]

# Man page output
man_pages = [
    (master_doc, 'linesolverpython', 'LINE Solver Python Documentation',
     [author], 1)
]

# Texinfo output
texinfo_documents = [
    (master_doc, 'LINESolverPython', 'LINE Solver Python Documentation',
     author, 'LINESolverPython', 'Queueing network solver for Python.',
     'Miscellaneous'),
]

# Epub output
epub_title = project
epub_exclude_files = ['search.html']

# Todo extension
todo_include_todos = True
EOF

    # Create index.rst
    cat > "$SPHINX_DIR/index.rst" << 'EOF'
LINE Solver Python Documentation
=================================

Welcome to the LINE Solver Python documentation. LINE is an open-source solver for queueing models 
and queueing networks, supporting both analytical methods and simulation.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   installation
   quickstart
   api
   examples
   reference

Installation
============

Install LINE Solver using pip::

    pip install line-solver

Quick Start
===========

Here's a simple M/M/1 queue example::

    from line_solver import *
    
    model = Network('M/M/1 Queue')
    
    # Create nodes
    source = Source(model, 'Source')
    queue = Queue(model, 'Queue', SchedStrategy.FCFS)
    sink = Sink(model, 'Sink')
    
    # Create job class
    jobclass = OpenClass(model, 'Class1')
    
    # Set service process
    queue.setService(jobclass, Exp(1.0))
    
    # Set arrival process
    source.setArrival(jobclass, Exp(0.5))
    
    # Link nodes
    model.link(Network.serialRouting([source, queue, sink]))
    
    # Solve
    solver = MVA(model)
    result = solver.getAvgTable()
    print(result)

API Reference
=============

Core Classes
------------

.. automodule:: line_solver.lang
   :members:
   :undoc-members:
   :show-inheritance:

Solvers
-------

.. automodule:: line_solver.solvers
   :members:
   :undoc-members:
   :show-inheritance:

Distributions
-------------

.. automodule:: line_solver.distributions
   :members:
   :undoc-members:
   :show-inheritance:

Layered Networks
----------------

.. automodule:: line_solver.layered
   :members:
   :undoc-members:
   :show-inheritance:

API Modules
-----------

.. automodule:: line_solver.api
   :members:
   :undoc-members:
   :show-inheritance:

Utilities
---------

.. automodule:: line_solver.utils
   :members:
   :undoc-members:
   :show-inheritance:

Constants
---------

.. automodule:: line_solver.constants
   :members:
   :undoc-members:
   :show-inheritance:

Examples Gallery
----------------

.. automodule:: line_solver.gallery
   :members:
   :undoc-members:
   :show-inheritance:

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
EOF

    # Create installation.rst
    cat > "$SPHINX_DIR/installation.rst" << 'EOF'
Installation
============

Requirements
------------

* Python 3.7 or higher
* Java 8 or higher (for the JAR backend)

Installing from PyPI
--------------------

The easiest way to install LINE Solver is via pip::

    pip install line-solver

Dependencies
------------

LINE Solver Python requires the following packages:

* numpy
* scipy
* pandas
* jpype1 (for Java integration)

These will be automatically installed when you install LINE Solver.

Verifying Installation
----------------------

To verify your installation, run::

    python -c "import line_solver; print(line_solver.__version__)"

Or try a simple example::

    from line_solver import *
    model = Network('Test')
    print(model)
EOF

    # Create quickstart.rst
    cat > "$SPHINX_DIR/quickstart.rst" << 'EOF'
Quick Start Guide
=================

This guide will help you get started with LINE Solver for Python.

Basic Concepts
--------------

LINE Solver models queueing networks using these core components:

* **Network**: The main container for your model
* **Nodes**: Source, Queue, Sink, Router, etc.
* **Job Classes**: OpenClass or ClosedClass
* **Service Processes**: Exponential, Deterministic, etc.
* **Solvers**: MVA, CTMC, Fluid, SSA, etc.

Creating a Simple Model
-----------------------

Here's how to create and solve a simple M/M/1 queue::

    from line_solver import *
    
    # Create network
    model = Network('Simple Queue')
    
    # Add nodes
    source = Source(model, 'Source')
    queue = Queue(model, 'Queue', SchedStrategy.FCFS)
    sink = Sink(model, 'Sink')
    
    # Define job class
    jobclass = OpenClass(model, 'Jobs')
    
    # Set arrival and service rates
    source.setArrival(jobclass, Exp(0.5))  # λ = 0.5
    queue.setService(jobclass, Exp(1.0))   # μ = 1.0
    
    # Connect nodes
    model.link(Network.serialRouting([source, queue, sink]))
    
    # Solve
    solver = MVA(model)
    avg_table = solver.getAvgTable()
    print(avg_table)

Multi-Class Networks
--------------------

You can model systems with multiple job classes::

    model = Network('Multi-Class')
    
    source = Source(model, 'Source')
    queue = Queue(model, 'Queue', SchedStrategy.PS)
    sink = Sink(model, 'Sink')
    
    # Define multiple classes
    class1 = OpenClass(model, 'Gold')
    class2 = OpenClass(model, 'Silver')
    
    # Different arrival rates
    source.setArrival(class1, Exp(0.2))
    source.setArrival(class2, Exp(0.3))
    
    # Different service rates
    queue.setService(class1, Exp(2.0))
    queue.setService(class2, Exp(1.0))

Closed Networks
---------------

For closed queueing networks::

    model = Network('Closed Network')
    
    delay = Delay(model, 'Think')
    queue1 = Queue(model, 'CPU', SchedStrategy.PS)
    queue2 = Queue(model, 'Disk', SchedStrategy.FCFS)
    
    # Closed class with population
    jobclass = ClosedClass(model, 'Users', 10)  # 10 users
    
    # Set service times
    delay.setService(jobclass, Exp(5.0))
    queue1.setService(jobclass, Exp(0.1))
    queue2.setService(jobclass, Exp(0.2))
    
    # Routing probabilities
    P = model.initRoutingMatrix()
    P[delay, queue1] = 1.0
    P[queue1, queue2] = 0.3
    P[queue1, delay] = 0.7
    P[queue2, queue1] = 1.0
    model.link(P)

Available Solvers
-----------------

LINE provides multiple solvers:

* **MVA**: Mean Value Analysis (exact for product-form)
* **CTMC**: Continuous-Time Markov Chain
* **FLD**: Fluid/Mean-Field ODE Solver
* **SSA**: Stochastic Simulation Algorithm
* **JMT**: Simulation via Java Modelling Tools
* **NC**: Normalizing Constant Analyzer
EOF

    # Create api.rst
    cat > "$SPHINX_DIR/api.rst" << 'EOF'
API Documentation
=================

This section provides detailed API documentation for all LINE Solver Python modules.

Core Module (line_solver.lang)
-------------------------------

.. automodule:: line_solver.lang
   :members:
   :special-members: __init__
   :show-inheritance:

Solver Module (line_solver.solvers)
------------------------------------

.. automodule:: line_solver.solvers
   :members:
   :special-members: __init__
   :show-inheritance:

Distribution Module (line_solver.distributions)
------------------------------------------------

.. automodule:: line_solver.distributions
   :members:
   :special-members: __init__
   :show-inheritance:

Layered Networks Module (line_solver.layered)
----------------------------------------------

.. automodule:: line_solver.layered
   :members:
   :special-members: __init__
   :show-inheritance:

API Submodules
--------------

.. automodule:: line_solver.api.CACHE
   :members:
   :show-inheritance:

.. automodule:: line_solver.api.PFQN
   :members:
   :show-inheritance:

.. automodule:: line_solver.api.LN
   :members:
   :show-inheritance:

Utilities Module (line_solver.utils)
-------------------------------------

.. automodule:: line_solver.utils
   :members:
   :show-inheritance:

Constants Module (line_solver.constants)
-----------------------------------------

.. automodule:: line_solver.constants
   :members:
   :show-inheritance:

Gallery Module (line_solver.gallery)
-------------------------------------

.. automodule:: line_solver.gallery
   :members:
   :show-inheritance:
EOF

    # Create examples.rst
    cat > "$SPHINX_DIR/examples.rst" << 'EOF'
Examples
========

This section contains various examples demonstrating LINE Solver capabilities.

Basic Examples
--------------

M/M/1 Queue
~~~~~~~~~~~

A simple single-server queue with exponential arrivals and service::

    from line_solver import *
    
    model = Network('M/M/1')
    source = Source(model, 'Source')
    queue = Queue(model, 'Queue', SchedStrategy.FCFS)
    sink = Sink(model, 'Sink')
    
    jobclass = OpenClass(model, 'Jobs')
    source.setArrival(jobclass, Exp(0.5))
    queue.setService(jobclass, Exp(1.0))
    
    model.link(Network.serialRouting([source, queue, sink]))
    
    solver = MVA(model)
    print(solver.getAvgTable())

M/M/c Queue
~~~~~~~~~~~

Multi-server queue::

    model = Network('M/M/c')
    source = Source(model, 'Source')
    queue = Queue(model, 'MultiServer', SchedStrategy.FCFS)
    queue.setNumberOfServers(3)  # 3 servers
    sink = Sink(model, 'Sink')
    
    jobclass = OpenClass(model, 'Jobs')
    source.setArrival(jobclass, Exp(2.0))
    queue.setService(jobclass, Exp(1.0))
    
    model.link(Network.serialRouting([source, queue, sink]))

Advanced Examples
-----------------

Fork-Join Network
~~~~~~~~~~~~~~~~~

Parallel processing with synchronization::

    model = Network('Fork-Join')
    source = Source(model, 'Source')
    fork = Fork(model, 'Fork')
    queue1 = Queue(model, 'Worker1', SchedStrategy.FCFS)
    queue2 = Queue(model, 'Worker2', SchedStrategy.FCFS)
    join = Join(model, 'Join', fork)
    sink = Sink(model, 'Sink')
    
    jobclass = OpenClass(model, 'Jobs')
    source.setArrival(jobclass, Exp(0.5))
    queue1.setService(jobclass, Exp(1.0))
    queue2.setService(jobclass, Exp(1.5))
    
    # Routing
    model.link(Network.serialRouting([source, fork]))
    model.link(Network.serialRouting([fork, queue1, join]))
    model.link(Network.serialRouting([fork, queue2, join]))
    model.link(Network.serialRouting([join, sink]))

Priority Classes
~~~~~~~~~~~~~~~~

Multiple priority classes::

    model = Network('Priority')
    source = Source(model, 'Source')
    queue = Queue(model, 'Server', SchedStrategy.FCFSPRIO)  # First-Come First-Served with Priority
    sink = Sink(model, 'Sink')
    
    # High and low priority classes
    high_priority = OpenClass(model, 'High', priority=1)
    low_priority = OpenClass(model, 'Low', priority=2)
    
    source.setArrival(high_priority, Exp(0.2))
    source.setArrival(low_priority, Exp(0.3))
    queue.setService(high_priority, Exp(2.0))
    queue.setService(low_priority, Exp(1.0))

Cache Network
~~~~~~~~~~~~~

Modeling cache behavior::

    from line_solver.api.CACHE import *
    
    model = Network('Cache')
    source = Source(model, 'Requests')
    cache = Cache(model, 'L1Cache', size=100, ReplacementStrategy.LRU)
    server = Queue(model, 'MainMemory', SchedStrategy.FCFS)
    sink = Sink(model, 'Complete')
    
    items = ItemSet(n=1000)  # 1000 unique items
    access_prob = [1.0/1000] * 1000  # Uniform access
    
    jobclass = OpenClass(model, 'Requests')
    source.setArrival(jobclass, Exp(10.0))
    cache.setHitService(jobclass, Exp(100.0))  # Fast cache hit
    cache.setMissService(jobclass, Exp(10.0))  # Slower cache miss
    server.setService(jobclass, Exp(1.0))  # Main memory access

Performance Metrics
-------------------

Getting various performance metrics::

    # After solving a model
    solver = MVA(model)
    
    # Average metrics
    avg_table = solver.getAvgTable()
    print("Average Queue Lengths and Response Times:")
    print(avg_table)
    
    # Throughput
    tput = solver.getTput()
    print(f"Throughput: {tput}")
    
    # Utilization
    util = solver.getUtil()
    print(f"Utilization: {util}")
    
    # Response time distribution (if supported)
    if hasattr(solver, 'getRespT'):
        resp_time = solver.getRespT()
        print(f"Response Time Distribution: {resp_time}")
EOF

    # Create reference.rst
    cat > "$SPHINX_DIR/reference.rst" << 'EOF'
Reference
=========

Scheduling Strategies
---------------------

Available scheduling strategies:

* **FCFS**: First-Come First-Served
* **LCFS**: Last-Come First-Served
* **PS**: Processor Sharing
* **DPS**: Discriminatory Processor Sharing
* **GPS**: Generalized Processor Sharing
* **SIRO**: Service In Random Order
* **FCFSPRIO**: First-Come First-Served with Priority
* **POLLING**: Polling service
* **INF**: Infinite Server

Distribution Types
------------------

Supported distributions:

* **Exp(rate)**: Exponential distribution
* **Det(value)**: Deterministic (constant)
* **Erlang(k, rate)**: Erlang distribution
* **HyperExp(p, rates)**: Hyper-exponential
* **Cox(phases)**: Cox distribution
* **APH(alpha, T)**: Acyclic Phase-type
* **MAP(D0, D1)**: Markovian Arrival Process
* **Trace(data)**: Empirical trace data
* **Uniform(a, b)**: Uniform distribution
* **Normal(mu, sigma)**: Normal distribution

Node Types
----------

Available node types:

* **Source**: Generates jobs
* **Queue**: Service station with waiting room
* **Delay**: Infinite server (think time)
* **Sink**: Job departure point
* **Router**: Probabilistic routing
* **Fork**: Split jobs into tasks
* **Join**: Synchronize forked tasks
* **Cache**: Cache with hit/miss behavior
* **Logger**: Log job passages
* **ClassSwitch**: Change job class

Solver Types
------------

Available solvers and their characteristics:

**Exact Solvers:**

* **MVA**: Mean Value Analysis
  - Product-form networks only
  - Fast for moderate network sizes
  - Exact results

* **NC**: Normalizing Constant Analyzer
  - Product-form networks
  - Efficient for closed networks

* **CTMC**: Continuous Time Markov Chain
  - Small state spaces only
  - Exact transient/steady-state

**Approximate Solvers:**

* **FLD**: Fluid/Mean-Field ODE Solver
  - Large populations
  - Fast computation
  - Good for high loads

* **SSA**: Stochastic State-space
  - Non-product-form features
  - State-dependent rates

**Simulation:**

* **JMT**: Java Modelling Tools
  - Any network topology
  - Statistical results
  - Confidence intervals

Error Messages
--------------

Common error messages and solutions:

* **"Network is not connected"**: Check routing matrix
* **"Solver does not support feature X"**: Try different solver
* **"Java backend not available"**: Check Java installation
* **"Invalid distribution parameters"**: Check parameter values
EOF

    echo -e "${GREEN}✓ Sphinx configuration created${NC}"
fi

# Clean previous build
echo
echo -e "${YELLOW}Cleaning previous build...${NC}"
rm -rf "$SPHINX_BUILD_DIR"

# Generate API documentation
echo
echo -e "${BLUE}Generating Sphinx documentation...${NC}"
cd "$SPHINX_DIR"

# Run sphinx-build (-Q suppresses warnings, only shows errors)
if $SPHINX_CMD -Q -b html . "$SPHINX_BUILD_DIR" -E -a; then
    echo -e "${GREEN}✓ Documentation generated successfully${NC}"
else
    echo -e "${RED}✗ Failed to generate documentation${NC}"
    exit 1
fi

# PDF generation disabled
# Uncomment the following block if you want to enable PDF generation
# if command -v pdflatex &> /dev/null; then
#     echo
#     echo -e "${YELLOW}Generating PDF documentation...${NC}"
#     if $SPHINX_CMD -b latex . "$SPHINX_BUILD_DIR/latex" -E -a; then
#         cd "$SPHINX_BUILD_DIR/latex"
#         if make; then
#             echo -e "${GREEN}✓ PDF documentation generated${NC}"
#             cp LINESolverPython.pdf "$SCRIPT_DIR/LINE-python-sphinx.pdf"
#         else
#             echo -e "${YELLOW}Warning: PDF generation failed${NC}"
#         fi
#     fi
#     cd "$SPHINX_DIR"
# fi

# The index.html is already generated by Sphinx, no need to create a redirect

# Summary
echo
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}Sphinx documentation generated successfully!${NC}"
echo -e "${GREEN}========================================${NC}"
echo
echo "Documentation location:"
echo "  HTML: $SPHINX_BUILD_DIR/index.html"
echo
echo "To view locally:"
echo "  cd $SPHINX_BUILD_DIR"
echo "  python3 -m http.server 8000"
echo "  # Then open http://localhost:8000 in your browser"
echo
echo "To upload to SourceForge, run:"
echo "  ./upload.sh"