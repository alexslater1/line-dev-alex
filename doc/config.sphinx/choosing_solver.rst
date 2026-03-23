==========================
Choosing the Right Solver
==========================

Selection Guidelines
====================

Use **AUTO** when:
   - You want LINE to automatically select the best solver for your model
   - You are not sure which solver is most appropriate
   - The model may change and you want adaptive solver selection

Use **CTMC** when:
   - You need exact solutions for a Markovian model
   - The model is small to medium-sized (<6000 states)
   - You need detailed state probabilities or transient analysis
   - Precision is more important than speed

Use **FLD** when:
   - You have a large-scale model where CTMC would have too many states
   - The model has many jobs or uses PS/INF scheduling
   - Approximate solutions with fast computation are acceptable
   - You need transient analysis for large models

Use **JMT** when:
   - Your model uses non-Markovian distributions (Pareto, deterministic, traces)
   - You need comprehensive feature support (JMT supports virtually everything)
   - You want simulation-based validation or sample paths
   - CTMC state space is too large and FLD/MVA do not support your features

Use **MAM** when:
   - You have open Markovian systems with MAP/PH arrivals
   - You need exact solutions for infinite state-space models
   - The model exhibits QBD structure

Use **MVA** when:
   - You have a closed or mixed product-form queueing network
   - You need fast approximate solutions for medium to large models
   - Single-server FCFS stations dominate your model
   - Mean performance metrics are sufficient (no distributions needed)

Use **NC** when:
   - You need state probabilities for closed networks
   - You want normalizing constant computations
   - The model is product-form and you prefer exact or sampling-based methods

Use **SSA** when:
   - You need sample paths with detailed phase information
   - You want flexible parallelization of stochastic simulation
   - You need to track service phase evolution
   - CTMC is too large but you prefer simulation over FLD approximation

Use **LDES** when:
   - You need support for stochastic Petri nets (Place/Transition nodes)
   - Your model uses fork-join parallelism with synchronization
   - You need advanced scheduling disciplines (LCFS preemptive, PS variants, priority)
   - You want trace-driven simulation (Replayer distributions)
   - You need class switching or router nodes with specialized routing strategies
   - You prefer an SSJ-based simulation engine over JMT

Performance Considerations
===========================

**Speed (fastest to slowest):**
   1. MVA (approximate methods)
   2. NC (sampling methods), FLD
   3. MAM (for QBD models)
   4. CTMC (small models)
   5. JMT, SSA, LDES (simulation)

**Accuracy (most accurate to approximate):**
   1. CTMC (exact for Markovian models)
   2. MAM (exact for QBD models)
   3. NC with exact methods
   4. MVA with exact methods
   5. NC with sampling, MVA approximations
   6. FLD approximations
   7. JMT/SSA/LDES (simulation - statistical accuracy)

**Scalability (supports largest models):**
   1. JMT, SSA, LDES (simulation)
   2. FLD, MVA
   3. NC (with sampling)
   4. MAM (for QBD structure)
   5. CTMC (limited by state space)

**Feature Coverage (most comprehensive):**
   1. LDES (Petri nets, fork-join, advanced scheduling, routing strategies)
   2. JMT (non-Markovian distributions, general networks)
   3. SSA (Markovian with phase tracking)
   4. CTMC, FLD, MVA, NC, MAM (specialized features)

See Also
========

- :doc:`solver_reference` - Complete solver method reference
- :doc:`quickstart` - Getting started with LINE solvers
- :doc:`examples` - Example models using different solvers
- :doc:`api/index` - Complete API reference
