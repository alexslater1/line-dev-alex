"""
Two-layer LQN example (Python)

This example maps two Java MVA submodels into one Layered Network (LQN).

Mapping / assumptions:
- Java submodels used nodes: t1 (Delay), t3 (Queue/Delay), p3 (Queue).
- We model these as LQN tasks: t1 (REF, population=2), t3 (server + client roles), p3 (server).
- t3 acts as both a server (top layer) and a client calling p3 (bottom layer). To represent both roles
  we create two entries on task `t3`: one for its server role and one for its client role.
- Client-role service demand for t3 in the bottom layer is set to 0 (as in the Java model).
- Server tasks are modelled with infinite population (SchedStrategy.INF) to represent server behaviour
  and replies_to on their entries. Closed population is modelled by the REF task `t1` with population 2.

This is a best-effort mapping; adjust multiplicities and scheduling to match your exact semantics.
"""
import os
import sys

# Ensure the package root (python/) is on sys.path so this example can be
# executed from different working directories. The file lives in
# python/examples/basic/layeredModel, so go up three levels to reach python/.
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..')))

from line_solver.layered import LayeredNetwork, Processor, Task, Entry, Activity
from line_solver.constants import SchedStrategy
from line_solver.distributions import Exp, Immediate
from line_solver.solvers import LN


def build_two_layer_ln():
    """
    Two-layer LQN based on SolverLNSimple Java model.
    
    Layer 0: t1 (Delay, REF task) -> t3 (Queue, server task)
    Layer 1: t3 (Delay, client role) -> p3 (Queue, server task)
    
    Population: 2 jobs in closed class
    Service times:
    - Layer 0: t1 = 0.0001, t3 = 0.0001
    - Layer 1: t3 = 0 (client), p3 = 0.9
    """
    model = LayeredNetwork('TwoLayerLN')
    
    # Processors
    P_t1 = Processor(model, 'P_t1', 1, SchedStrategy.INF)
    P_t3 = Processor(model, 'P_t3', 1, SchedStrategy.INF)
    P_p3 = Processor(model, 'P_p3', 1, SchedStrategy.INF)
    
    # Tasks
    T1 = Task(model, 't1', 2, SchedStrategy.REF).on(P_t1).set_think_time(Immediate())
    T3 = Task(model, 't3', 2, SchedStrategy.FCFS).on(P_t3)
    P3 = Task(model, 'p3', 2, SchedStrategy.FCFS).on(P_p3)
    
    # Entries
    E_t1 = Entry(model, 'E_t1').on(T1)
    E_t3 = Entry(model, 'E_t3').on(T3)
    E_p3 = Entry(model, 'E_p3').on(P3)
    
    # Activities
    # Layer 0: t1 calls t3
    A_t1 = Activity(model, 'A_t1', Immediate()).on(T1).bound_to(E_t1).synch_call(E_t3, 1)
    
    # Layer 1: t3 acts as client calling p3, and p3 acts as server
    # t3's service time as client is Immediate (0) because it's modeled as infinite server
    A_t3 = Activity(model, 'A_t3', Immediate()).on(T3).bound_to(E_t3).synch_call(E_p3, 1).replies_to(E_t3)
    A_p3 = Activity(model, 'A_p3', Exp.fit_mean(0.9)).on(P3).bound_to(E_p3).replies_to(E_p3)
    
    return model


def build_three_layer_ln():
    model = LayeredNetwork('ThreeLayerLN')
 
    P1 = Processor(model, 'P1', 1, SchedStrategy.INF)
    P2 = Processor(model, 'P2', 1, SchedStrategy.INF)
    P3 = Processor(model, 'P3', 1, SchedStrategy.INF)
    
    
    T1 = Task(model, 'T1', 2, SchedStrategy.REF).on(P1).set_think_time(Immediate())
    T2 = Task(model, 'T2', 2, SchedStrategy.FCFS).on(P2)
    T3 = Task(model, 'T3', 2, SchedStrategy.FCFS).on(P3)
    
    
    E1 = Entry(model, 'E1').on(T1)
    E2 = Entry(model, 'E2').on(T2)
    E3 = Entry(model, 'E3').on(T3)
    
    
    A1 = Activity(model, 'AS1', Immediate()).on(T1).bound_to(E1).synch_call(E2, 1)
    A2 = Activity(model, 'AS2', Immediate()).on(T2).bound_to(E2).synch_call(E3, 1).replies_to(E2)
    A3 = Activity(model, 'AS3', Exp.fit_mean(0.9)).on(T3).bound_to(E3).replies_to(E3)

    return model


if __name__ == '__main__':
    # Run two-layer model (default)
    model = build_two_layer_ln()

    # Solve the layered network using the LN solver
    solver = LN(model)
    avg_table = solver.avg_table()
    print(avg_table)
    
    # Optional: also run three-layer model
    # model_3 = build_three_layer_ln()
    # solver_3 = LN(model_3)
    # avg_table_3 = solver_3.avg_table()
    # print(avg_table_3)