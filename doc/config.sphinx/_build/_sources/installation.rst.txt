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

You can also visit the PyPI package page: https://pypi.org/project/line-solver/

Dependencies
------------

LINE Solver Python requires the following packages:

* enum_tools
* jpype1 (for Java integration)
* numpy
* pandas
* scipy
* twisted
* nbformat
* nbconvert
* matplotlib
* networkx

These will be automatically installed when you install LINE Solver.

To install these dependencies manually, you can use::

    pip install enum_tools jpype1 numpy pandas scipy twisted nbformat nbconvert matplotlib networkx

Verifying Installation
----------------------

To verify your installation, run::

    python -c "import line_solver; print(line_solver.__version__)"
