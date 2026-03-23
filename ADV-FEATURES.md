# Advanced Features Tested in testsAdvFeatures

Features **unique to testsAdvFeatures** (not tested elsewhere in line-test.git):

| # | Feature | Tests |
|---|---------|-------|
| 1 | **Finite Capacity Regions (FCR)** — drop/waitq policies | 8 in `fcr/` + 2 in `des/` |
| 2 | **Polling systems** (gated/exhaustive) | `test_des_polling` |
| 3 | **Setup/Delayoff server times** | `test_des_setupdelayoff` |
| 4 | **Reneging/impatient customers** | `test_mam_reneging` |
| 5 | **Retrial queues** (BMAP/PH/N, FCR) | 2 in `distributions/` |
| 6 | **BMAP/MAP distributions** & conversions | 3 in `distributions/` |
| 7 | **Non-Markov Bernstein approximation** | `test_nonmarkov_bernstein` |
| 8 | **IndexedTable API** | 4 in `misc/` |
| 9 | **Reward API** (custom CTMC rewards) | `test_reward_api` |
| 10 | **QRF bounds** (Quick Response Function) | `test_qrf_bounds` |
| 11 | **MMDP** (Markov Modulated Decision Process) | `test_mmdp` |
| 12 | **Prior/Posterior Bayesian analysis** | `test_prior`, `test_posterior_plot` |
| 13 | **Sampling convergence** (SSA warm-up) | `test_sampling_convergence` |
| 14 | **Random environments** (FES solver) | 3 in `env/` |
| 15 | **LQN forwarding** (call chains) | `test_forwarding`, `test_forwarding_2` |
| 16 | **LQN-to-QN conversion** | `test_LQN2QN` |
| 17 | **CTMC Router stochcomp** (Java vs MATLAB parity) | `test_ctmc_router_java_vs_matlab` |
| 18 | **LJCD** (Limited Joint Capacity Dependence) | `test_des_ljcd` |
| 19 | **SRPT/LPS/LRPT size-based scheduling** | 4 in `scheduling/` |
| 20 | **Fluid immediate nodes** (Router/Service/Simple) | 3 in `fluid/` |
| 21 | **Fluid MFQ** (Mean-Field QBD with MAP/MMPP) | 3 in `fluid/` |
| 22 | **Fluid p-norm optimization** | `test_fluid_pnorm` |
| 23 | **NetworkStruct set APIs** (runtime model modification) | `test_sn_set_apis` |
| 24 | **DES transient analysis** (getTranAvg) | `test_des_transient` |
| 25 | **DES confidence intervals** (MSER-5) | `test_des_ci` |
| 26 | **LN state transfer** between solver instances | `test_ln_state_transfer` |
| 27 | **LN 3rd moment** accuracy | `test_ln_moment3` |
| 28 | **MAM LDQBD** (Level-Dependent QBD) | 3 in `mam/` |
| 29 | **MAM marginal/aggregate probabilities** | 2 in `mam/` |
| 30 | **NC loss networks** (Erlang-B) | `test_nc_lossn` |
| 31 | **NC probability methods** with load-dependent | 3 in `nc/` |
| 32 | **MVA Schmidt/AB** approximate algorithms | `test_mva_schmidt` |
| 33 | **Reliability metrics** | `test_reliability_metrics` |
| 34 | **Workflow models** (WfCommons, APH conversion) | 5 in `workflow/` |
| 35 | **FCFSPRPRIO** (preemptive resume priority) | `test_fcfsprprio` |
| 36 | **Tardiness metrics** | `test_tardiness`, `test_jmt_tardiness` |

Features already well-covered elsewhere (testsAdvFeatures adds depth but not unique coverage): cache, basic fork-join, basic DES/JMT/LDES, HOL priority, BAS, LN solver convergence, async calls.
