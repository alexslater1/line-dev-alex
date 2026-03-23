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
* **Coxian(mu, phi)**: Coxian phase-type distribution
* **Cox2(mu1, mu2, phi1)**: 2-phase Cox distribution
* **APH(alpha, T)**: Acyclic Phase-type
* **MAP(D0, D1)**: Markovian Arrival Process
* **Replayer(filename)**: Replay empirical trace data from file
* **Uniform(a, b)**: Uniform distribution

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


