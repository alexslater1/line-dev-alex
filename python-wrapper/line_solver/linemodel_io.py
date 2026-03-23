"""
LINE Model JSON Save/Load (Python Wrapper)

Delegates to JAR's LineModelIO via JPype for serializing LINE
Network and LayeredNetwork models to/from JSON format.

Copyright (c) 2012-2026, Imperial College London
All rights reserved.
"""

import jpype


def save_model(model, filename):
    """
    Save a LINE model (Network or LayeredNetwork) to a JSON file.

    The output conforms to the line-model.schema.json specification.

    Args:
        model: A Network or LayeredNetwork wrapper instance (with .obj attribute).
        filename: Output file path (should end in .json).

    Example:
        >>> model = Network('M/M/1')
        >>> # ... define model ...
        >>> save_model(model, 'mymodel.json')
    """
    LineModelIO = jpype.JPackage('jline').io.LineModelIO
    java_obj = model.obj
    LineModelIO.save(java_obj, str(filename))


def load_model(filename):
    """
    Load a LINE model from a JSON file.

    Returns a Network or LayeredNetwork wrapper depending on the model type
    in the JSON file.

    Args:
        filename: Path to a .json file conforming to line-model.schema.json.

    Returns:
        Network or LayeredNetwork wrapper instance.

    Example:
        >>> model = load_model('mymodel.json')
        >>> solver = SolverMVA(model)
        >>> print(solver.avg_table())
    """
    from ._lang import Network
    from .layered import LayeredNetwork as LQN
    from .lang.workflow import Workflow
    from ._lang import Environment

    LineModelIO = jpype.JPackage('jline').io.LineModelIO
    java_obj = LineModelIO.load(str(filename))

    jline_pkg = jpype.JPackage('jline')
    if isinstance(java_obj, jline_pkg.lang.Network):
        return Network(java_obj)
    elif isinstance(java_obj, jline_pkg.lang.layered.LayeredNetwork):
        lqn = LQN.__new__(LQN)
        lqn.obj = java_obj
        lqn._wrapper_nodes = {}
        lqn._nodes_by_name = {}
        return lqn
    elif isinstance(java_obj, jline_pkg.lang.workflow.Workflow):
        wf = Workflow.__new__(Workflow)
        wf._obj = java_obj
        wf._cached_ph = None
        return wf
    elif isinstance(java_obj, jline_pkg.lang.Environment):
        env = Environment.__new__(Environment)
        env.obj = java_obj
        return env
    else:
        raise ValueError("Unsupported model type returned from JAR: {}".format(type(java_obj)))
