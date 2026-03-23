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
release = '3.0.5'

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
# List of camelCase methods to exclude when snake_case equivalents exist
camelcase_exclude_methods = [
    'getIndex', 'getNumberOfJobs', 'getName', 'getPriority', 'numberOfJobs',
    'setRouting', 'setProbRouting', 'setState', 'setRoutingMatrix', 'addRoute',
    'setName', 'getVersion', 'fromJline', 'addLink', 'initRoutingMatrix',
    'getNumberOfNodes', 'getNumberOfStations', 'getNumberOfClasses', 'getClasses',
    'getNodeIndex', 'getStationIndex', 'getStatefulIndex', 'getJobClassIndex',
    'getChainIndex', 'getNodes', 'getTranHandles', 'jsimgView', 'jsimwView',
    'addLinks', 'getStruct', 'printStruct', 'getState', 'refreshStruct',
    'printRoutingMatrix', 'getProductFormParameters', 'getGraph', 'tandemPsInf',
    'tandemFcfsInf', 'tandemPs', 'tandemFcfs', 'cyclicPsInf', 'cyclicFcfsInf',
    'cyclicPs', 'cyclicFcfs', 'initDefault', 'initFromMarginal', 'initFromMarginalAndStarted',
    'nodeIndex', 'stationIndex', 'statefulIndex', 'jobClassIndex', 'chainIndex',
    'setRead', 'setHitClass', 'setMissClass', 'getHitRatio', 'getMissRatio',
    'getModel', 'getEnsemble', 'addStage', 'addTransition', 'getStageTable',
    'setArrival', 'getArrivalProcess', 'setStartTime', 'setJobID', 'setJobClass',
    'setTimestamp', 'setTimeSameClass', 'setTimeAnyClass', 'initClassSwitchMatrix',
    'setClassSwitchingMatrix', 'setTasksPerLink', 'setService', 'setNumberOfServers',
    'setLoadDependence', 'getServiceProcess', 'setPollingType', 'setPollingK',
    'setSwitchover', 'getSwitchover', 'getSchedStrategy', 'setClassCapacity',
    'setSchedStrategy', 'setCapacity', 'setDropRule', 'getIndex', 'getTransition',
    'printSummary', 'addMode', 'getModes', 'getModeNames', 'getNumberOfModes',
    'setDistribution', 'setTimingStrategy', 'getFiringDistribution', 'setEnablingConditions',
    'setInhibitingConditions', 'setFiringOutcome', 'setFiringPriorities', 'setFiringWeights',
    'getNumberOfModeServers', 'getPopulation', 'toMarginal', 'fromMarginalAndStarted',
    'toMarginalAggr', 'fromMarginalAndRunning', 'isValid', 'fromMarginal',
    'afterEvent', 'afterEventHashed', 'spaceGenerator', 'spaceClosedMultiCS'
]

# List of get_* methods to exclude when short versions exist
get_underscore_exclude_methods = [
    'get_index', 'get_number_of_jobs', 'get_name', 'get_priority', 'get_version',
    'get_number_of_nodes', 'get_number_of_stations', 'get_number_of_classes', 
    'get_classes', 'get_node_index', 'get_station_index', 'get_stateful_index',
    'get_job_class_index', 'get_chain_index', 'get_nodes', 'get_tran_handles',
    'get_struct', 'get_state', 'get_product_form_parameters', 'get_graph',
    'get_hit_ratio', 'get_miss_ratio', 'get_model', 'get_ensemble', 'get_stage_table',
    'get_arrival_process', 'get_service_process', 'get_switchover', 'get_sched_strategy',
    'get_transition', 'get_modes', 'get_mode_names', 'get_number_of_modes',
    'get_firing_distribution', 'get_number_of_mode_servers', 'get_population',
    'get_cardinality', 'get_popularity', 'get_item_level_cap', 'get_replacestrategy'
]

# List of enum members to exclude to prevent duplication
enum_members_exclude = [
    'GATED', 'EXHAUSTIVE', 'KLIMITED',  # PollingType
    'PRE_SEQ', 'PRE_AND', 'PRE_OR', 'POST_SEQ', 'POST_AND', 'POST_OR', 'POST_LOOP', 'POST_CACHE',  # ActivityPrecedenceType
    'SYNC', 'ASYNC', 'FWD',  # CallType
    'WaitingQueue', 'Drop', 'BlockingAfterService',  # DropStrategy
    'INIT', 'LOCAL', 'ARV', 'DEP', 'PHASE', 'read', 'STAGE',  # EventType
    'OPEN', 'CLOSED', 'DISABLED',  # JobClassType
    'STD', 'PARTIAL', 'Quorum', 'Guard',  # JoinStrategy
    'ResidT', 'RespT', 'DropRate', 'QLen', 'QueueT', 'FCRWeight', 'FCRMemOcc', 'FJQLen', 'FJRespT',
    'RespTSink', 'SysQLen', 'SysRespT', 'SysTput', 'Tput', 'ArvR',
    'TputSink', 'Util', 'TranQLen', 'TranUtil', 'TranTput', 'TranRespT',  # MetricType
    'Transition', 'Place', 'Fork', 'Router', 'Cache', 'Logger', 'ClassSwitch', 'Delay',
    'Source', 'Sink', 'Join', 'Queue',  # NodeType
    'EXP', 'ERLANG', 'DISABLED', 'IMMEDIATE', 'HYPEREXP', 'APH', 'COXIAN', 'PH', 'MAP',
    'UNIFORM', 'DET', 'GAMMA', 'PARETO', 'WEIBULL', 'LOGNORMAL', 'MMPP2', 'REPLAYER',
    'TRACE', 'COX2', 'BINOMIAL', 'POISSON',  # ProcessType
    'RR', 'FIFO', 'SFIFO', 'LRU',  # ReplacementStrategy
    'RAND', 'PROB', 'RROBIN', 'WRROBIN', 'JSQ', 'KCHOICES', 'FIRING',  # RoutingStrategy
    'INF', 'FCFS', 'LCFS', 'LCFSPR', 'SIRO', 'SJF', 'LJF', 'PS', 'DPS', 'GPS',
    'SEPT', 'LEPT', 'FCFSPRIO', 'FORK', 'EXT', 'REF', 'POLLING', 'PSPRIO', 'DPSPRIO', 'GPSPRIO',  # SchedStrategy
    'PR', 'PNR', 'NP', 'NPPrio',  # SchedStrategyType
    'LI', 'LD', 'CD', 'SD',  # ServiceStrategy
    'AUTO', 'CTMC', 'ENV', 'FLUID', 'JMT', 'LN', 'LQNS', 'MAM', 'MVA', 'NC', 'QNS', 'SSA',  # SolverType
    'TIMED',  # TimingStrategy (IMMEDIATE already listed)
    'SILENT', 'DEBUG',  # VerboseLevel (STD conflicts with JoinStrategy.STD)
    # GlobalConstants attributes
    'Zero', 'CoarseTol', 'FineTol', 'MaxInt', 'Version', 'DummyMode'
]

def exclude_unwanted_members(app, what, name, obj, skip, options):
    """Custom filter to exclude specific members"""
    # Always exclude these special methods and method lists
    if (name in ('__init__', '__index__', '__weakref__') or
        name in camelcase_exclude_methods or
        name in get_underscore_exclude_methods or
        name in enum_members_exclude):
        return True

    return skip

def suppress_empty_sections(app, doctree, docname):
    """Remove empty sections from the documentation"""
    from docutils import nodes

    # Find and remove empty sections
    for section in doctree.traverse(nodes.section):
        # Check if section has only title and no other content
        children = [child for child in section.children
                   if not isinstance(child, nodes.title)]

        # If section only has title or is effectively empty
        if not children or all(not child.astext().strip() for child in children):
            section.parent.remove(section)

all_exclude_methods = camelcase_exclude_methods + get_underscore_exclude_methods + enum_members_exclude

autodoc_default_options = {
    'members': True,
    'member-order': 'bysource',
    'special-members': False,
    'undoc-members': False,
    'exclude-members': '__weakref__,' + ','.join(all_exclude_methods),
    'show-inheritance': True,
    'inherited-members': False,
    'private-members': False,
}

# Additional autodoc configuration
autodoc_member_order = 'bysource'
autodoc_typehints = 'description'
autodoc_typehints_description_target = 'documented'
add_module_names = False

# Suppress empty sections and modules
autodoc_mock_imports_prefixes = ['java', 'jline']
autodoc_class_signature = 'mixed'
autoclass_content = 'both'
autodoc_preserve_defaults = True

# Mock imports for modules that might not be installed
autodoc_mock_imports = [
    'jpype',
    'jpype1',
    'jpype.imports',
    'java',
    'jline',
    'pandas',
    'numpy',
    'matplotlib',
    'scipy',
    'urllib',
    'urllib.request'
]

# Additional mock setup for missing functions
import sys
from unittest.mock import MagicMock

class MockModule(MagicMock):
    """Mock module that can handle any attribute access"""
    @classmethod
    def __getattr__(cls, name):
        return MagicMock()

# Mock the missing utility functions
sys.modules['line_solver.utils'] = MockModule()
sys.modules['line_solver.utils'].jlineMatrixFromArray = MagicMock()
sys.modules['line_solver.utils'].jlineMatrixToArray = MagicMock()

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
    'prev_next_buttons_location': 'both',
}

# Add any paths that contain custom static files
html_static_path = ['_static']

# The logo to display in the sidebar
html_logo = '_static/imperial_logo.png'

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

def setup(app):
    """Setup function for Sphinx extension"""
    app.connect('autodoc-skip-member', exclude_unwanted_members)
    app.connect('doctree-resolved', suppress_empty_sections)
    return {
        'version': '0.1',
        'parallel_read_safe': True,
        'parallel_write_safe': True,
    }

