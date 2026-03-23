Queueing Systems
================

Single-station queueing system analysis.

The ``qsys`` module provides exact and approximate formulas for single queueing
systems such as M/M/c, M/G/1, G/G/1, and their variants.

**Key function categories:**

- **Exact solutions:** :func:`~line_solver.api.qsys.qsys_mm1`, :func:`~line_solver.api.qsys.qsys_mmk`, :func:`~line_solver.api.qsys.qsys_mg1`, :func:`~line_solver.api.qsys.qsys_gm1`
- **G/G/1 approximations:** :func:`~line_solver.api.qsys.qsys_gig1_approx_kingman`, :func:`~line_solver.api.qsys.qsys_gig1_approx_kimura`, :func:`~line_solver.api.qsys.qsys_gig1_approx_marchal`
- **Multi-server approximations:** :func:`~line_solver.api.qsys.qsys_gigk_approx`, :func:`~line_solver.api.qsys.qsys_gigk_approx_kingman`
- **Loss systems:** :func:`~line_solver.api.qsys.qsys_mm1k_loss`, :func:`~line_solver.api.qsys.qsys_mg1k_loss`, :func:`~line_solver.api.qsys.qsys_mg1k_loss_mgs`
- **Performance bounds:** :func:`~line_solver.api.qsys.qsys_gig1_ubnd_kingman`

.. automodule:: line_solver.api.qsys
   :members:
   :undoc-members:
   :show-inheritance:
