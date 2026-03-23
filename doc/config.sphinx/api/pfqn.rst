Product-Form Queueing Networks
==============================

MVA, convolution, and normalizing constant methods.

The ``pfqn`` module contains algorithms for product-form queueing networks, including
Mean Value Analysis (MVA), convolution, and normalizing constant methods.

**Key function categories:**

- **Mean Value Analysis:** :func:`~line_solver.api.pfqn.pfqn_mva`, :func:`~line_solver.api.pfqn.pfqn_mvald`, :func:`~line_solver.api.pfqn.pfqn_mvams`, :func:`~line_solver.api.pfqn.pfqn_mvamx`
- **Normalizing constants:** :func:`~line_solver.api.pfqn.pfqn_nc`, :func:`~line_solver.api.pfqn.pfqn_ncld`, :func:`~line_solver.api.pfqn.pfqn_fnc`
- **Convolution algorithms:** :func:`~line_solver.api.pfqn.pfqn_ca`, :func:`~line_solver.api.pfqn.pfqn_bs`, :func:`~line_solver.api.pfqn.pfqn_comom`
- **Linearizer methods:** :func:`~line_solver.api.pfqn.pfqn_linearizer`, :func:`~line_solver.api.pfqn.pfqn_linearizerms`, :func:`~line_solver.api.pfqn.pfqn_gflinearizer`
- **Bounds and approximations:** :func:`~line_solver.api.pfqn.pfqn_qzgblow`, :func:`~line_solver.api.pfqn.pfqn_qzgbup`, :func:`~line_solver.api.pfqn.pfqn_xzabalow`, :func:`~line_solver.api.pfqn.pfqn_xzabaup`
- **LCFS queueing networks:** :func:`~line_solver.api.pfqn.pfqn_lcfsqn_mva`, :func:`~line_solver.api.pfqn.pfqn_lcfsqn_nc`, :func:`~line_solver.api.pfqn.pfqn_lcfsqn_ca`
- **Maximum entropy:** :func:`~line_solver.api.nc.me_oqn` - Maximum Entropy for open queueing networks

.. automodule:: line_solver.api.pfqn
   :members:
   :undoc-members:
   :show-inheritance:
