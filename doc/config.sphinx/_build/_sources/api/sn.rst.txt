Stochastic Network Utilities
============================

Network analysis and transformation utilities.

The ``sn`` module contains utility functions for stochastic networks, including
routing probability calculations, visit ratios, and network transformations.

**Key function categories:**

- **Network properties:** :func:`~line_solver.api.sn.sn_has_product_form`, :func:`~line_solver.api.sn.sn_has_class_switching`, :func:`~line_solver.api.sn.sn_has_fork_join`
- **Scheduling queries:** :func:`~line_solver.api.sn.sn_has_fcfs`, :func:`~line_solver.api.sn.sn_has_ps`, :func:`~line_solver.api.sn.sn_has_lcfs`, :func:`~line_solver.api.sn.sn_has_priorities`
- **Model type queries:** :func:`~line_solver.api.sn.sn_is_open_model`, :func:`~line_solver.api.sn.sn_is_closed_model`, :func:`~line_solver.api.sn.sn_is_mixed_model`
- **Performance metrics:** :func:`~line_solver.api.sn.sn_get_demands_chain`, :func:`~line_solver.api.sn.sn_get_arvr_from_tput`, :func:`~line_solver.api.sn.sn_get_residt_from_respt`
- **State management:** :func:`~line_solver.api.sn.sn_get_state_aggr`, :func:`~line_solver.api.sn.sn_is_state_valid`, :func:`~line_solver.api.sn.sn_deaggregate_chain_results`
- **Routing utilities:** :func:`~line_solver.api.sn.sn_refresh_visits`, :func:`~line_solver.api.sn.sn_rtnodes_to_rtorig`, :func:`~line_solver.api.sn.sn_print_routing_matrix`

.. automodule:: line_solver.api.sn
   :members:
   :undoc-members:
   :show-inheritance:
