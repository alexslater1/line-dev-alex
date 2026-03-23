"""
Workflow classes for Python wrapper using JAR via JPype.

This module provides Python wrappers for the Java Workflow, WorkflowActivity,
and ActivityPrecedence classes, enabling workflow modeling and phase-type
distribution conversion.

Copyright (c) 2012-2026, Imperial College London
All rights reserved.
"""

import jpype
import numpy as np
from typing import List, Optional, Tuple, Union

from .. import jlineMatrixFromArray, jlineMatrixToArray


class WorkflowActivity:
    """
    Represents a single activity in a computational workflow.

    An activity has a name and an associated service time distribution
    (the host demand). Activities are connected via precedence relationships
    to form a workflow.

    Args:
        obj: Java WorkflowActivity object (used internally)
    """

    def __init__(self, obj):
        """Initialize from a Java WorkflowActivity object."""
        self._obj = obj

    @property
    def obj(self):
        """Get the underlying Java object."""
        return self._obj

    def getName(self) -> str:
        """Get the name of this activity."""
        return str(self._obj.getName())

    def getIndex(self) -> int:
        """Get the index of this activity in the workflow."""
        return int(self._obj.getIndex())

    def getHostDemandMean(self) -> float:
        """Get the mean host demand (service time)."""
        return float(self._obj.getHostDemandMean())

    def getHostDemandSCV(self) -> float:
        """Get the squared coefficient of variation of the host demand."""
        return float(self._obj.getHostDemandSCV())

    def getNumberOfPhases(self) -> int:
        """Get the number of phases in the PH representation."""
        return int(self._obj.getNumberOfPhases())

    def getPHRepresentation(self) -> Tuple[np.ndarray, np.ndarray]:
        """
        Get the phase-type representation of this activity.

        Returns:
            Tuple of (alpha, T) where alpha is the initial probability
            vector and T is the sub-generator matrix.
        """
        pair = self._obj.getPHRepresentation()
        alpha = jlineMatrixToArray(pair.getLeft())
        T = jlineMatrixToArray(pair.getRight())
        return alpha, T

    # Snake_case aliases
    get_name = getName
    get_index = getIndex
    get_host_demand_mean = getHostDemandMean
    get_host_demand_scv = getHostDemandSCV
    get_number_of_phases = getNumberOfPhases
    get_ph_representation = getPHRepresentation


class Workflow:
    """
    A computational workflow that can be converted to a phase-type distribution.

    A Workflow consists of activities connected by precedence relationships.
    The workflow can be converted to an equivalent phase-type (APH) distribution
    using the toPH() method, which enables queueing analysis.

    Supported workflow patterns:
    - Serial: Sequential execution of activities
    - AND-Fork/Join: Parallel execution with synchronization
    - OR-Fork/Join: Probabilistic branching
    - Loop: Repeated execution of activities

    Examples:
        >>> wf = Workflow('MyWorkflow')
        >>> A = wf.addActivity('A', Exp.fitMean(1.0))
        >>> B = wf.addActivity('B', Exp.fitMean(2.0))
        >>> wf.addPrecedence(Workflow.Serial(A, B))
        >>> alpha, T = wf.toPH()
    """

    def __init__(self, name: str):
        """
        Create a new workflow.

        Args:
            name: Name of the workflow
        """
        self._obj = jpype.JPackage('jline').lang.workflow.Workflow(name)
        self._cached_ph = None

    @property
    def obj(self):
        """Get the underlying Java object."""
        return self._obj

    def addActivity(self, name: str, host_demand) -> WorkflowActivity:
        """
        Add an activity to the workflow.

        Args:
            name: Name of the activity
            host_demand: Service time distribution (e.g., Exp, Erlang, APH)
                        or a float representing mean service time

        Returns:
            WorkflowActivity object representing the added activity
        """
        self._cached_ph = None
        if isinstance(host_demand, (int, float)):
            java_act = self._obj.addActivity(name, float(host_demand))
        else:
            # Assume it's a Distribution object with .obj attribute
            java_act = self._obj.addActivity(name, host_demand.obj)
        return WorkflowActivity(java_act)

    def addPrecedence(self, prec):
        """
        Add precedence relationship(s) to the workflow.

        Args:
            prec: ActivityPrecedence object or array of ActivityPrecedence objects
        """
        self._cached_ph = None
        # Check if it's an array/list of precedences
        if hasattr(prec, '__iter__') and not isinstance(prec, str):
            # It's an iterable (array of precedences from Serial, etc.)
            for p in prec:
                self._obj.addPrecedence(p)
        else:
            # Single precedence
            self._obj.addPrecedence(prec)

    def getActivity(self, name: str) -> Optional[WorkflowActivity]:
        """
        Get an activity by name.

        Args:
            name: Name of the activity

        Returns:
            WorkflowActivity or None if not found
        """
        java_act = self._obj.getActivity(name)
        if java_act is None:
            return None
        return WorkflowActivity(java_act)

    def getActivities(self) -> List[WorkflowActivity]:
        """
        Get all activities in the workflow.

        Returns:
            List of WorkflowActivity objects
        """
        java_list = self._obj.getActivities()
        return [WorkflowActivity(act) for act in java_list]

    def getPrecedences(self):
        """
        Get all precedence relationships in the workflow.

        Returns:
            List of ActivityPrecedence objects (Java)
        """
        return list(self._obj.getPrecedences())

    def validate(self) -> Tuple[bool, str]:
        """
        Validate the workflow structure.

        Returns:
            Tuple of (is_valid, error_message)
        """
        pair = self._obj.validate()
        return bool(pair.getLeft()), str(pair.getRight())

    def toPH(self) -> Tuple[np.ndarray, np.ndarray]:
        """
        Convert the workflow to a phase-type distribution.

        Returns:
            Tuple of (alpha, T) where alpha is the initial probability
            vector and T is the sub-generator matrix.

        Raises:
            RuntimeError: If the workflow is invalid
        """
        if self._cached_ph is not None:
            return self._cached_ph

        aph = self._obj.toPH()
        alpha = jlineMatrixToArray(aph.getInitProb())
        T = jlineMatrixToArray(aph.getSubgenerator())
        self._cached_ph = (alpha, T)
        return self._cached_ph

    def getMean(self) -> float:
        """
        Get the mean of the workflow completion time distribution.

        Returns:
            Mean completion time
        """
        aph = self._obj.toPH()
        return float(aph.getMean())

    def getSCV(self) -> float:
        """
        Get the squared coefficient of variation of workflow completion time.

        Returns:
            SCV of completion time
        """
        aph = self._obj.toPH()
        return float(aph.getSCV())

    def getVar(self) -> float:
        """
        Get the variance of the workflow completion time distribution.

        Returns:
            Variance of completion time
        """
        aph = self._obj.toPH()
        return float(aph.getVar())

    def sample(self, n: int = 1, seed: int = None) -> np.ndarray:
        """
        Generate random samples from the workflow completion time distribution.

        Args:
            n: Number of samples to generate
            seed: Optional random seed for reproducibility

        Returns:
            Array of n random samples
        """
        aph = self._obj.toPH()
        # Create a Java Random object (required by map_sample)
        if seed is not None:
            random = jpype.java.util.Random(seed)
        else:
            random = jpype.java.util.Random()
        samples = aph.sample(n, random)
        # sample() returns a double[] array, convert to numpy
        return np.array(list(samples))

    # Static factory methods for precedences
    @staticmethod
    def Serial(*activities) -> list:
        """
        Create serial precedence relationships.

        Args:
            *activities: Two or more WorkflowActivity objects

        Returns:
            Array of ActivityPrecedence objects

        Examples:
            >>> wf.addPrecedence(Workflow.Serial(A, B, C))  # A -> B -> C
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        # Convert Python WorkflowActivity wrappers to Java objects
        java_activities = []
        for act in activities:
            if hasattr(act, 'obj'):
                java_activities.append(act.obj)
            else:
                java_activities.append(act)

        # Call the Java static method with varargs
        return list(java_workflow.Serial(*java_activities))

    @staticmethod
    def AndFork(pre_act, post_acts: list):
        """
        Create an AND-fork precedence (parallel split).

        Args:
            pre_act: Activity before the fork
            post_acts: List of activities after the fork (executed in parallel)

        Returns:
            ActivityPrecedence object
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        pre = pre_act.obj if hasattr(pre_act, 'obj') else pre_act

        # Convert list to Java List
        java_list = jpype.java.util.ArrayList()
        for act in post_acts:
            if hasattr(act, 'obj'):
                java_list.add(act.obj)
            else:
                java_list.add(act)

        return java_workflow.AndFork(pre, java_list)

    @staticmethod
    def AndJoin(pre_acts: list, post_act):
        """
        Create an AND-join precedence (synchronization).

        Args:
            pre_acts: List of activities before the join
            post_act: Activity after the join

        Returns:
            ActivityPrecedence object
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        post = post_act.obj if hasattr(post_act, 'obj') else post_act

        # Convert list to Java List
        java_list = jpype.java.util.ArrayList()
        for act in pre_acts:
            if hasattr(act, 'obj'):
                java_list.add(act.obj)
            else:
                java_list.add(act)

        return java_workflow.AndJoin(java_list, post)

    @staticmethod
    def OrFork(pre_act, post_acts: list, probs: list):
        """
        Create an OR-fork precedence (probabilistic branching).

        Args:
            pre_act: Activity before the fork
            post_acts: List of activities after the fork
            probs: List of probabilities (must sum to 1)

        Returns:
            ActivityPrecedence object
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        pre = pre_act.obj if hasattr(pre_act, 'obj') else pre_act

        # Convert list to Java List
        java_list = jpype.java.util.ArrayList()
        for act in post_acts:
            if hasattr(act, 'obj'):
                java_list.add(act.obj)
            else:
                java_list.add(act)

        # Convert probs to double array
        probs_array = jpype.JArray(jpype.JDouble)(probs)

        return java_workflow.OrFork(pre, java_list, probs_array)

    @staticmethod
    def OrJoin(pre_acts: list, post_act):
        """
        Create an OR-join precedence (merge).

        Args:
            pre_acts: List of activities before the join
            post_act: Activity after the join

        Returns:
            ActivityPrecedence object
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        post = post_act.obj if hasattr(post_act, 'obj') else post_act

        # Convert list to Java List
        java_list = jpype.java.util.ArrayList()
        for act in pre_acts:
            if hasattr(act, 'obj'):
                java_list.add(act.obj)
            else:
                java_list.add(act)

        return java_workflow.OrJoin(java_list, post)

    @staticmethod
    def Loop(pre_act, post_acts: list, count: float):
        """
        Create a loop precedence (repeated execution).

        Args:
            pre_act: Activity before the loop
            post_acts: List of activities in the loop body and end activity
                      (last activity is executed once after the loop)
            count: Number of loop iterations

        Returns:
            ActivityPrecedence object
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        pre = pre_act.obj if hasattr(pre_act, 'obj') else pre_act

        # Convert list to Java List
        java_list = jpype.java.util.ArrayList()
        for act in post_acts:
            if hasattr(act, 'obj'):
                java_list.add(act.obj)
            else:
                java_list.add(act)

        return java_workflow.Loop(pre, java_list, float(count))

    @staticmethod
    def fromWfCommons(json_file: str):
        """
        Load a workflow from a WfCommons JSON file.

        WfCommons (https://github.com/wfcommons/workflow-schema) is a standard
        format for representing scientific workflow traces.

        Args:
            json_file: Path to the WfCommons JSON file

        Returns:
            Workflow object
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        java_wf = java_workflow.fromWfCommons(json_file)

        # Create a new Python Workflow and replace its Java object
        wf = object.__new__(Workflow)
        wf._obj = java_wf
        wf._cached_ph = None
        return wf

    # Static composition methods (for advanced users)
    @staticmethod
    def composeSerial(alpha1: np.ndarray, T1: np.ndarray,
                      alpha2: np.ndarray, T2: np.ndarray) -> Tuple[np.ndarray, np.ndarray]:
        """
        Compose two PH distributions in serial.

        Args:
            alpha1, T1: First PH distribution
            alpha2, T2: Second PH distribution

        Returns:
            Composed PH distribution (alpha, T)
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        pair = java_workflow.composeSerial(
            jlineMatrixFromArray(alpha1),
            jlineMatrixFromArray(T1),
            jlineMatrixFromArray(alpha2),
            jlineMatrixFromArray(T2)
        )
        return jlineMatrixToArray(pair.getLeft()), jlineMatrixToArray(pair.getRight())

    @staticmethod
    def composeParallel(alpha1: np.ndarray, T1: np.ndarray,
                        alpha2: np.ndarray, T2: np.ndarray) -> Tuple[np.ndarray, np.ndarray]:
        """
        Compose two PH distributions in parallel (fork-join).

        Args:
            alpha1, T1: First PH distribution
            alpha2, T2: Second PH distribution

        Returns:
            Composed PH distribution (alpha, T)
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        pair = java_workflow.composeParallel(
            jlineMatrixFromArray(alpha1),
            jlineMatrixFromArray(T1),
            jlineMatrixFromArray(alpha2),
            jlineMatrixFromArray(T2)
        )
        return jlineMatrixToArray(pair.getLeft()), jlineMatrixToArray(pair.getRight())

    @staticmethod
    def composeRepeat(alpha: np.ndarray, T: np.ndarray,
                      count: int) -> Tuple[np.ndarray, np.ndarray]:
        """
        Compose a PH distribution repeated count times in serial.

        Args:
            alpha, T: PH distribution to repeat
            count: Number of repetitions

        Returns:
            Composed PH distribution (alpha, T)
        """
        java_workflow = jpype.JPackage('jline').lang.workflow.Workflow
        pair = java_workflow.composeRepeat(
            jlineMatrixFromArray(alpha),
            jlineMatrixFromArray(T),
            int(count)
        )
        return jlineMatrixToArray(pair.getLeft()), jlineMatrixToArray(pair.getRight())

    # Snake_case aliases
    add_activity = addActivity
    add_precedence = addPrecedence
    get_activity = getActivity
    get_activities = getActivities
    get_precedences = getPrecedences
    to_ph = toPH
    get_mean = getMean
    get_scv = getSCV
    get_var = getVar
    from_wf_commons = staticmethod(lambda f: Workflow.fromWfCommons(f))
    compose_serial = staticmethod(lambda a1, t1, a2, t2: Workflow.composeSerial(a1, t1, a2, t2))
    compose_parallel = staticmethod(lambda a1, t1, a2, t2: Workflow.composeParallel(a1, t1, a2, t2))
    compose_repeat = staticmethod(lambda a, t, c: Workflow.composeRepeat(a, t, c))


# Convenience aliases for the precedence factory methods
Serial = Workflow.Serial
AndFork = Workflow.AndFork
AndJoin = Workflow.AndJoin
OrFork = Workflow.OrFork
OrJoin = Workflow.OrJoin
Loop = Workflow.Loop
