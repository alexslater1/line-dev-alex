"""
LDES solver options and result dataclasses.

This module provides configuration options and result containers for the
LDES (LINE Discrete Event Simulator) solver, which runs via subprocess
calling ldes.jar.
"""

from dataclasses import dataclass, field
from typing import Optional, Dict, Any, List
import numpy as np


@dataclass
class LDESOptions:
    """
    Configuration options for LDES solver.

    Options map to CLI flags of `java -jar ldes.jar solve`.

    Key characteristics:
    - Discrete event simulation via ldes.jar subprocess
    - Handles multiclass Jackson queueing networks
    - Supports steady-state analysis
    - Provides statistical estimates with confidence intervals
    - Uses event-count based stopping (samples = max service completions)
    - Default: 10,000 service completion events
    """

    samples: int = 10_000
    """Maximum number of service completion events (-s). Default: 10,000"""

    seed: int = 23000
    """Random seed for reproducibility (--seed). Use -1 for random seed."""

    method: str = "default"
    """Simulation method (--method). Default: 'default'"""

    cnvgon: bool = False
    """Enable convergence-based stopping (--cnvgon). Default: False"""

    cnvgtol: float = 0.05
    """Convergence tolerance (--cnvgtol). Default: 0.05 (5%)"""

    tranfilter: str = "mser5"
    """Transient filter method (--tranfilter): 'mser5', 'fixed', 'none'. Default: 'mser5'"""

    warmupfrac: float = 0.2
    """Warmup fraction for fixed filter (--warmupfrac). Default: 0.2 (20%)"""

    cimethod: str = "obm"
    """CI computation method (--cimethod): 'obm', 'bm', 'spectral', 'none'. Default: 'obm'.
    'spectral' uses Heidelberger-Welch spectral analysis (log-periodogram regression) to
    account for autocorrelation between batches, giving wider but more honest CIs."""

    spectral_low_freq_frac: float = 0.25
    """Fraction of lowest frequencies for spectral log-periodogram regression
    (--spectralLowFreqFrac). Only used when cimethod='spectral'. Default: 0.25"""

    replications: Optional[int] = None
    """Number of replications (--replications). Default: None (use ldes.jar default)"""

    numthreads: Optional[int] = None
    """Number of threads (--numthreads). Default: None (use ldes.jar default)"""

    verbose: str = "silent"
    """Verbosity level: 'silent', 'std', 'debug'. Default: 'silent'"""

    timespan: Optional[List[float]] = None
    """Time horizon [T0, T1] for transient analysis. None = steady-state."""

    def copy(self) -> 'LDESOptions':
        """Create a deep copy of this options object."""
        return LDESOptions(
            samples=self.samples,
            seed=self.seed,
            method=self.method,
            cnvgon=self.cnvgon,
            cnvgtol=self.cnvgtol,
            tranfilter=self.tranfilter,
            warmupfrac=self.warmupfrac,
            cimethod=self.cimethod,
            spectral_low_freq_frac=self.spectral_low_freq_frac,
            replications=self.replications,
            numthreads=self.numthreads,
            verbose=self.verbose,
            timespan=list(self.timespan) if self.timespan else None,
        )


@dataclass
class LDESResult:
    """
    Result container for LDES solver computations.

    All metrics are stored as numpy arrays with dimensions [stations x classes].
    Populated from the JSON output of ldes.jar.
    """

    # Mean performance metrics [stations x classes]
    QN: Optional[np.ndarray] = None
    """Average queue lengths [stations x classes]"""

    UN: Optional[np.ndarray] = None
    """Server utilizations [stations x classes]"""

    RN: Optional[np.ndarray] = None
    """Response times [stations x classes]"""

    TN: Optional[np.ndarray] = None
    """Throughputs [stations x classes]"""

    AN: Optional[np.ndarray] = None
    """Arrival rates [stations x classes]"""

    WN: Optional[np.ndarray] = None
    """Residence times [stations x classes]"""

    CN: Optional[np.ndarray] = None
    """Visit counts [stations x classes]"""

    XN: Optional[np.ndarray] = None
    """System throughput per class [1 x classes]"""

    # Confidence interval half-widths [stations x classes]
    QNCI: Optional[np.ndarray] = None
    """CI half-widths for queue lengths"""

    UNCI: Optional[np.ndarray] = None
    """CI half-widths for utilizations"""

    RNCI: Optional[np.ndarray] = None
    """CI half-widths for response times"""

    TNCI: Optional[np.ndarray] = None
    """CI half-widths for throughputs"""

    ANCI: Optional[np.ndarray] = None
    """CI half-widths for arrival rates"""

    WNCI: Optional[np.ndarray] = None
    """CI half-widths for residence times"""

    # Relative precision (CI half-width / mean) [stations x classes]
    QNRelPrec: Optional[np.ndarray] = None
    """Relative precision for queue lengths"""

    UNRelPrec: Optional[np.ndarray] = None
    """Relative precision for utilizations"""

    RNRelPrec: Optional[np.ndarray] = None
    """Relative precision for response times"""

    TNRelPrec: Optional[np.ndarray] = None
    """Relative precision for throughputs"""

    # Transient time-series data [stations][classes] -> numpy array (numTimePoints x 2)
    QNt: Optional[List[List[Optional[np.ndarray]]]] = None
    """Queue length time series [stations][classes], each (numTimePoints x 2) with [value, time]"""

    UNt: Optional[List[List[Optional[np.ndarray]]]] = None
    """Utilization time series [stations][classes]"""

    TNt: Optional[List[List[Optional[np.ndarray]]]] = None
    """Throughput time series [stations][classes]"""

    t: Optional[np.ndarray] = None
    """Time vector for transient analysis (numTimePoints x 1)"""

    respTimeSamples: Optional[List[List[Optional[List[float]]]]] = None
    """Response time samples [stations][classes] -> list of individual observations"""

    # Metadata
    method: str = "default"
    """Simulation method used"""

    runtime: float = 0.0
    """Execution time in seconds"""

    converged: bool = False
    """Whether simulation converged before reaching max events/time"""

    stopping_reason: str = ""
    """Stopping reason: 'convergence', 'max_events', or 'max_time'"""

    convergence_batches: int = 0
    """Number of batches used for final CI estimation"""
