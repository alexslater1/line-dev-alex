#!/usr/bin/env python3
"""Render LQN model views for the Contribution chapter, via the LINE
python-wrapper (JPype -> common/jline.jar). No MATLAB required.

Each model is a faithful translation of the Java test fixtures
(EvaluationSuite / DagFeatureFixtures). Colours follow the hashname prefix
(P:/R:/T:/E:/A:), which is robust (the struct's lqn.type field is misaligned
and the wrapper's own plotGraph mis-maps it -- see report notes).

Run:  .venv/bin/python report/source/figures/scripts/lqn_views.py
Out:  report/source/figures/python-views/<name>.{pdf,png}
"""
import os, sys
os.environ["MPLBACKEND"] = "Agg"
import matplotlib; matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Patch
import networkx as nx
import numpy as np
nx.nx_agraph.graphviz_layout = nx.nx_pydot.graphviz_layout  # use installed `dot`

REPO = "/home/alex/Documents/FYP/line-dev"
sys.path.insert(0, os.path.join(REPO, "python-wrapper"))
from line_solver import *
GlobalConstants.set_verbose(VerboseLevel.SILENT)

OUT = os.path.join(REPO, "report/source/figures/python-views")
os.makedirs(OUT, exist_ok=True)

STYLE = {
    'P:': ('black',   'h', 1200, 'Host/Processor'),
    'R:': ('#EDB120', '^', 1100, 'Reference Task'),
    'T:': ('magenta', 'v', 1000, 'Task'),
    'E:': ('red',     's',  900, 'Entry'),
    'A:': ('blue',    'o',  800, 'Activity'),
    'C:': ('green',   'o',  700, 'Call'),
}
def classify(name):
    for pre, sty in STYLE.items():
        if name.startswith(pre):
            return sty
    return ('lightgray', 'o', 600, 'Other')

def render(model, fname, title, bw=False, figsize=(11, 8)):
    lqn = model.getStruct()
    adj = np.asarray(lqn.graph)
    n = adj.shape[0]
    names = [str(x) for x in lqn.hashnames]
    G = nx.DiGraph()
    labels, colors, shapes, sizes = {}, [], [], []
    for i in range(n):
        nm = names[i] if i < len(names) and names[i] else f'({i})'
        G.add_node(i)
        labels[i] = nm.replace('_', r'\_')
        c, m, s, _ = classify(nm)
        colors.append('white' if bw else c)
        shapes.append('o' if bw else m)
        sizes.append(700 if bw else s)
    for i in range(n):
        for j in range(adj.shape[1]):
            if adj[i, j] > 0:
                G.add_edge(i, j)
    pos = nx.nx_agraph.graphviz_layout(G, prog='dot')
    plt.figure(figsize=figsize)
    for shp in set(shapes):
        idx = [k for k in range(n) if shapes[k] == shp]
        nx.draw_networkx_nodes(G, pos, nodelist=idx,
                               node_color=[colors[k] for k in idx],
                               node_size=[sizes[k] for k in idx],
                               node_shape=shp, edgecolors='black',
                               linewidths=1.0, alpha=(1.0 if bw else 0.85))
    nx.draw_networkx_edges(G, pos, edge_color='black' if bw else 'gray',
                           arrows=True, arrowsize=18, width=1.0,
                           alpha=(0.8 if bw else 0.6))
    nx.draw_networkx_labels(G, pos, labels=labels, font_size=8, font_weight='bold')
    if not bw:
        seen, leg = set(), []
        for i in range(n):
            c, m, s, lab = classify(names[i] if i < len(names) and names[i] else '')
            if lab not in seen:
                seen.add(lab); leg.append(Patch(facecolor=c, label=lab))
        plt.legend(handles=leg, loc='upper right', fontsize=9)
    plt.title(title, fontsize=13)
    plt.axis('off'); plt.tight_layout()
    plt.savefig(f"{OUT}/{fname}.pdf", bbox_inches='tight')
    plt.savefig(f"{OUT}/{fname}.png", dpi=130, bbox_inches='tight')
    plt.close()
    print(f"wrote {fname}.{{pdf,png}}")


# ===================== step 1: lqn_basic (fan-out) =====================
def build_lqn_basic():
    m = LayeredNetwork('lqn_basic')
    P1 = Processor(m, 'P1', 2, SchedStrategy.PS); P2 = Processor(m, 'P2', 3, SchedStrategy.PS)
    T1 = Task(m, 'T1', 50, SchedStrategy.REF).on(P1).set_think_time(Exp(1/2))
    T2 = Task(m, 'T2', 50, SchedStrategy.FCFS).on(P1).set_think_time(Exp(1/3))
    T3 = Task(m, 'T3', 25, SchedStrategy.FCFS).on(P2).set_think_time(Exp(1/4))
    E1 = Entry(m, 'E1').on(T1); E2 = Entry(m, 'E2').on(T2); E3 = Entry(m, 'E3').on(T3)
    Activity(m, 'AS1', Exp(10)).on(T1).bound_to(E1).synch_call(E2, 1)
    Activity(m, 'AS2', Exp(20)).on(T2).bound_to(E2).synch_call(E3, 5).replies_to(E2)
    Activity(m, 'AS3', Exp(50)).on(T3).bound_to(E3).replies_to(E3)
    return m

# ===================== step 2/4: depth-3 chain (immediate intermediates) =====
def build_chain(tiers=3, n=20):
    m = LayeredNetwork(f'chain_d{tiers}_N{n}')
    P, T, E = [], [], []
    for i in range(tiers):
        P.append(Processor(m, f'P{i+1}', 4, SchedStrategy.PS))
        if i == 0:
            T.append(Task(m, 'T1', n, SchedStrategy.REF).on(P[i]).set_think_time(Exp(1/2)))
        else:
            T.append(Task(m, f'T{i+1}', float('inf'), SchedStrategy.INF).on(P[i]))
        E.append(Entry(m, f'E{i+1}').on(T[i]))
    Activity(m, 'AS1', Immediate()).on(T[0]).bound_to(E[0]).synch_call(E[1], 1)
    for i in range(1, tiers-1):
        Activity(m, f'AS{i+1}', Immediate()).on(T[i]).bound_to(E[i]).synch_call(E[i+1], 1).replies_to(E[i-1])
    Activity(m, f'AS{tiers}', Exp.fit_mean(0.8)).on(T[tiers-1]).bound_to(E[tiers-1]).replies_to(E[tiers-2])
    return m

# ===================== step 3: multi-class shared callee (Cat C, asymmetric) =
def build_shared_callee_asym():
    m = LayeredNetwork('multiclass_C_asym')
    PR = Processor(m, 'PR', float('inf'), SchedStrategy.INF)
    PS = Processor(m, 'PS', 1, SchedStrategy.PS)
    R1 = Task(m, 'R1', 20, SchedStrategy.REF).on(PR).set_think_time(Exp.fit_mean(2.0))
    R2 = Task(m, 'R2', 20, SchedStrategy.REF).on(PR).set_think_time(Exp.fit_mean(2.0))
    TS = Task(m, 'TS', float('inf'), SchedStrategy.INF).on(PS)
    ER1 = Entry(m, 'ER1').on(R1); ER2 = Entry(m, 'ER2').on(R2)
    ES1 = Entry(m, 'ES1').on(TS); ES2 = Entry(m, 'ES2').on(TS)
    Activity(m, 'A1', Immediate()).on(R1).bound_to(ER1).synch_call(ES1, 1)
    Activity(m, 'A2', Immediate()).on(R2).bound_to(ER2).synch_call(ES2, 1)
    Activity(m, 'AS1', Exp.fit_mean(0.5)).on(TS).bound_to(ES1).replies_to(ES1)
    Activity(m, 'AS2', Exp.fit_mean(1.5)).on(TS).bound_to(ES2).replies_to(ES2)
    return m

# ===================== step 5: fan-out caller (width 3) =====================
def build_fanout_caller(width=3, callerC=1):
    m = LayeredNetwork(f'fanOutCaller_w{width}')
    PR = Processor(m, 'PR', callerC, SchedStrategy.PS)
    R = Task(m, 'R', 20, SchedStrategy.REF).on(PR).set_think_time(Exp.fit_mean(2.0))
    ER = Entry(m, 'ER').on(R)
    callee_entries = []
    for i in range(width):
        Pi = Processor(m, f'PS{i+1}', 4, SchedStrategy.PS)
        Ti = Task(m, f'TS{i+1}', float('inf'), SchedStrategy.INF).on(Pi)
        ce = Entry(m, f'ES{i+1}').on(Ti)
        callee_entries.append(ce)
        Activity(m, f'AS{i+1}', Exp.fit_mean(0.3 + 0.05*i)).on(Ti).bound_to(ce).replies_to(ce)
    caller = Activity(m, 'A0', Immediate()).on(R).bound_to(ER)
    for ce in callee_entries:
        caller = caller.synch_call(ce, 1)
    return m

# ===================== step 6: combined DAG ================================
def build_combined_dag():
    m = LayeredNetwork('D_combinedDag')
    PR = Processor(m, 'PR', float('inf'), SchedStrategy.INF)
    PS = Processor(m, 'PS', float('inf'), SchedStrategy.INF)
    T1 = Task(m, 'T1', 10, SchedStrategy.REF).on(PR).set_think_time(Exp.fit_mean(2.0))
    T2 = Task(m, 'T2', float('inf'), SchedStrategy.INF).on(PS)
    E1 = Entry(m, 'E1').on(T1); E2 = Entry(m, 'E2').on(T2)
    Activity(m, 'A1', Immediate()).on(T1).bound_to(E1).synch_call(E2, 1)
    A_init      = Activity(m, 'A_init',      Exp.fit_mean(0.1)).on(T2).bound_to(E2)
    A_loop_head = Activity(m, 'A_loop_head', Immediate()).on(T2)
    A_loop_body = Activity(m, 'A_loop_body', Exp.fit_mean(0.2)).on(T2)
    A_loop_end  = Activity(m, 'A_loop_end',  Immediate()).on(T2)
    A_or_x      = Activity(m, 'A_or_x',      Exp.fit_mean(0.3)).on(T2)
    A_or_y      = Activity(m, 'A_or_y',      Exp.fit_mean(0.1)).on(T2)
    A_after_or  = Activity(m, 'A_after_or',  Immediate()).on(T2)
    A_and_p1    = Activity(m, 'A_and_p1',    Exp.fit_mean(0.4)).on(T2)
    A_and_p2    = Activity(m, 'A_and_p2',    Exp.fit_mean(0.4)).on(T2)
    A_join      = Activity(m, 'A_join',      Exp.fit_mean(0.1)).on(T2).replies_to(E2)
    A_phase2    = Activity(m, 'A_phase2',    Exp.fit_mean(0.3)).on(T2).set_phase(2)
    T2.add_precedence(ActivityPrecedence.Serial(A_init, A_loop_head))
    T2.add_precedence(ActivityPrecedence.Loop(A_loop_head, [A_loop_body, A_loop_end], 2))
    T2.add_precedence(ActivityPrecedence.OrFork(A_loop_end, [A_or_x, A_or_y], [0.7, 0.3]))
    T2.add_precedence(ActivityPrecedence.OrJoin([A_or_x, A_or_y], A_after_or))
    T2.add_precedence(ActivityPrecedence.AndFork(A_after_or, [A_and_p1, A_and_p2]))
    T2.add_precedence(ActivityPrecedence.AndJoin([A_and_p1, A_and_p2], A_join))
    T2.add_precedence(ActivityPrecedence.Serial(A_join, A_phase2))
    return m


# ===================== ensemble submodels (step 1 Option E) ================
# Renders the PFQN sub-Networks that SolverLN.buildLayers emits -- the actual
# "view of the ensemble" that step 1 Option E previously only described in code.
NETSTYLE = {  # NodeType.toString() -> (color, marker, size)
    'Delay':       ('#7fc97f', 'o', 1300),   # IS (Clients)
    'Queue':       ('#386cb0', 's', 1300),   # server station
    'Source':      ('#fdc086', '>', 900),
    'Sink':        ('#fdc086', '<', 900),
    'ClassSwitch': ('#bbbbbb', 'd', 500),
}
def render_ensemble(model, fname, title):
    from line_solver.lang import jlineMatrixToArray
    ens = model.getEnsemble()
    nsub = len(ens)
    cols = min(nsub, 2); rows = (nsub + cols - 1) // cols
    fig, axes = plt.subplots(rows, cols, figsize=(6.0 * cols, 4.2 * rows))
    axes = np.atleast_1d(axes).ravel()
    for li in range(nsub):
        net = ens[li]; st = net.getStruct(); ax = axes[li]
        names = [str(x) for x in st.nodenames]
        types = [str(x) for x in st.nodetype]
        conn = np.asarray(jlineMatrixToArray(st.connmatrix))
        G = nx.DiGraph()
        for i in range(len(names)):
            G.add_node(i)
        for i in range(conn.shape[0]):
            for j in range(conn.shape[1]):
                if conn[i, j] > 0:
                    G.add_edge(i, j)
        pos = nx.nx_agraph.graphviz_layout(G, prog='dot')
        for t, (c, mk, sz) in NETSTYLE.items():
            idx = [i for i in range(len(types)) if types[i] == t]
            if idx:
                nx.draw_networkx_nodes(G, pos, nodelist=idx, node_color=c,
                                       node_shape=mk, node_size=sz,
                                       edgecolors='black', linewidths=0.8, ax=ax)
        nx.draw_networkx_edges(G, pos, ax=ax, edge_color='gray', arrows=True,
                               arrowsize=14, width=0.9, alpha=0.6)
        lbl = {i: names[i].replace('_', r'\_') for i in range(len(names))}
        nx.draw_networkx_labels(G, pos, labels=lbl, font_size=7, ax=ax)
        ax.set_title(f'submodel {li}: {str(net.getName())}', fontsize=10)
        ax.axis('off')
    for k in range(nsub, len(axes)):
        axes[k].axis('off')
    fig.suptitle(title, fontsize=13)
    fig.tight_layout()
    fig.savefig(f"{OUT}/{fname}.pdf", bbox_inches='tight')
    fig.savefig(f"{OUT}/{fname}.png", dpi=130, bbox_inches='tight')
    plt.close(fig)
    print(f"wrote {fname}.{{pdf,png}} ({nsub} submodels)")


JOBS = [
    (build_lqn_basic(),         'step1-lqn-basic',      'lqn\\_basic (step 1): minimal LQN with fan-out'),
    (build_chain(3, 20),        'step2-chain-d3',       'chain\\_d3 (step 2): depth-3 chain, Immediate intermediate'),
    (build_shared_callee_asym(),'step3-multiclass-c',   'multi-class Cat C (step 3): two REF callers, shared asymmetric callee'),
    (build_fanout_caller(3, 1), 'step5-fanout-caller',  'fan-out caller (step 5): one caller, three sibling callees'),
    (build_combined_dag(),      'step6-combined-dag',   'D\\_combinedDag (step 6): seq + OR + AND + loop + reply'),
]
for model, fname, title in JOBS:
    render(model, fname + '-colour', title, bw=False)
    render(model, fname + '-bw',     title.split(':')[0].strip(), bw=True)

render_ensemble(build_lqn_basic(), 'step1-ensemble',
                'lqn\\_basic ensemble: PFQN submodels from SolverLN.buildLayers '
                '(Delay = Clients, Queue = T:/P: server, diamonds = class-switch)')
print("ALL DONE")
