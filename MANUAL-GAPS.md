# Documentation Gaps for Advanced Features

Analysis of ADV-FEATURES.md coverage across LINE documentation (manuals, API references, web pages, cheatsheet).

## Summary

- **Fully documented:** 20 / 35
- **Not documented:** 5 / 35
- **Partially documented:** 10 / 35

## Not Documented

| # | Feature | Notes |
|---|---------|-------|
| 8 | IndexedTable API | Completely absent from all documentation |
| 10 | QRF bounds (Quick Response Function) | Completely absent from all documentation |
| 15 | LQN forwarding (call chains) | Commented-out line in feature table; no actual content |
| 20 | Fluid immediate nodes (Router/Service/Simple) | No coverage in any documentation |
| 27 | LN 3rd moment accuracy | No coverage in any documentation |

## Partially Documented (Need Expansion)

| # | Feature | What exists | What is missing |
|---|---------|-------------|-----------------|
| 3 | Setup/Delayoff server times | Feature table mention, API function | Dedicated usage section with examples |
| 7 | Non-Markov Bernstein approx | `aph_bernstein()` API autodoc | Manual section explaining the method |
| 11 | MMDP | Documented as distribution type | Coverage as a decision process framework |
| 13 | Sampling convergence | LDES MSER-5 options | SSA warm-up documentation |
| 16 | LQN-to-QN conversion | One-line mention in interop table | Usage documentation and examples |
| 17 | CTMC Router stochcomp | API autodocs for stochcomp functions | Manual section on Router elimination use case |
| 23 | NetworkStruct set APIs | `sn_set_service` example | Comprehensive listing of all set functions |
| 28 | MAM LDQBD | API autodoc mention | Dedicated manual section |
| 31 | NC prob methods + load-dep | Separate docs for each | Documentation linking NC prob methods to load-dependent models |
| 33 | Reliability metrics | Passing mentions | Dedicated section on reliability metric computation |
