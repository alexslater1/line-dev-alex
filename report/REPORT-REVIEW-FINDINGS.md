# Report Review — Findings

Full pass over `report/source/`. Sections: title, abstract, acknowledgements, declarations,
introduction, background, contribution, evaluation, conclusion, bib. All `\ref`→`\label` resolve
(0 dangling). Bibliography style = `ieeetr` (numbered → Vancouver-numeric, acceptable).

Legend: ✏ = I will edit (safe/unambiguous). ⚠ = flagged for you (numbers/results/judgement — I won't change).

---

## 1. LEFT-BEHIND / DRAFT ARTEFACTS  (highest priority — some render in the PDF)

- ⚠ **`UNK` placeholders render as visible text** in the optimisation section:
  - `contribution.tex:515` "…with a runtime decrease of UNK. %TODO"
  - `contribution.tex:640` "…in UNK execution time against UNK."
  These need the optimised-vs-unoptimised solve-phase runtime numbers. I can't invent them; fill from your
  optimisation run data (`report/data/`). Same lines also carry `%TODO`.
- ✏ **`\addedtop` / `\addedbot` review macros** defined `contribution.tex:4-8` ("[ADDED FOR REVIEW]" red rules).
  Never actually called anywhere → safe to delete the whole 4-8 block.
- ⚠ **`DRAFT` in label names** (invisible to readers, code-hygiene only): `sec:DRAFT-perf-optimisation`,
  `tab:DRAFT-sweep`, `fig:DRAFT-jfr-profile`. Referenced from conclusion + contribution. Optional rename
  (would need to update all `\ref` sites). Low priority since labels never print.
- ✏ **Commented-out `% TODO: check` worked example** `contribution.tex:40-55`, and bare `% TODO`
  `contribution.tex:221`, and `%TODO: make this table bigger` `contribution.tex:578`. Dead comments. I'll
  leave the commented worked-example (you may re-enable) but note it; will not touch unless you want them gone.

## 2. INTERNAL CONSISTENCY

- ⚠ **371 vs 539 outer-iteration headline.** Optimisation section reports the optimised solver at **371**
  (`contribution.tex:515,640`, bounce-sweep) then migrates to **elevator = 539** (`tab:DRAFT-sweep`, L634).
  Everywhere else (abstract, intro, conclusion, all of evaluation) the final solver is **539**. A reader hits
  "371…34.7% reduction" in §contrib then "539" in eval. The "What optimising provides" summary (L638-646)
  should state that 371 is the bounce-sweep figure and the **elevator-migrated** solver used in the
  evaluation runs 539, so the two numbers reconcile. (Numbers themselves untouched — your call to reword.)
- ⚠ **`568` unoptimised baseline** (`contribution.tex:515,640`) — verify against your final run; memory notes a
  later "38.7% post-elevator→final" figure, so 568→371/34.7% may be stale vs the elevator solver.
- ✏ **Naming: `\texttt{SolverLQNS}` vs `LQNS` in prose.** Report overwhelmingly says "LQNS". Prose instances of
  `\texttt{SolverLQNS}` at `contribution.tex:16,513,571` and `evaluation.tex:45` (the run list) read
  inconsistently. Keep `\texttt{SolverLQNS}` only where it's literally the class/counter
  (`SolverLQNS.result.iter`, eval L45 second half; eval L294); normalise the rest to LQNS.
- ✏ **`queuing` vs `queueing`.** Report standard is British "queueing". Fix the 4 stray "queuing":
  `background.tex:4` ("solve queuing network models"), `background.tex:578` (heading "Layered Queuing Network
  Solver" — LQNS = QueuEing), `contribution.tex:20` ("a queuing node"), `evaluation.tex:291` ("flat queuing
  network"). (introduction.tex:20 is a comment — ignore.)
- ⚠ **N at which IS instability starts:** `contribution.tex:273` says throughput collapsed "at N = 100";
  `contribution.tex:277` says "instability starts at N = 20". Reconcile (e.g. "starts at 20, fully collapses
  by 100") or correct.
- ✏ **`MvaLD` vs `MvaLd`** casing: `contribution.tex:358` caption "MvaLD" vs class `MvaLd` elsewhere.
- ⚠ **E[max] identity naming:** called "inclusion-exclusion identity" (`contribution.tex:477`),
  "Heidelberger-Trivedi identity" (`contribution.tex:505`, no cite) and "Heidelberger--Trivedi
  \cite[eq.2.6]{franks99}" (`evaluation.tex:513`). Pick one name; if you keep "Heidelberger-Trivedi", either
  cite it consistently (currently via franks99) or add a dedicated reference (see §5).

## 3. SPELLING / GRAMMAR / TYPOS  (✏ all to fix)

abstract.tex
- L8 "built the minimal solution under the paradigm" → "built as the minimal solution …".
- L9 "After the correctness of the model was sufficient" → "Once the solver was sufficiently correct".

introduction.tex — clean (no typos found).

background.tex
- L393 "If these affects were ignored" → "effects".
- L463 "based around entries and phases the mean total execution time" → missing verb: "…entries and
  phases, are the mean total execution time…".
- L463 "the squared variance $c^2_{ep}$" → wrong term; $c^2$ = **squared coefficient of variation**.
- L453 "LQN solver can decompose" → "An LQN solver can decompose".
- L629 "Bart-Schweitzer" → "Bard-Schweitzer".
- L621 "selection criteria routes" → "selection criteria route" (or "criterion routes").
- L629 double space "across the grid,  whilst".
- L576 stray space "\cite{franks99,franks2009} ." 
- Citation-after-period style (cite should precede the full stop) at L28, L45, L393, L401 — rest of report
  puts \cite before the period.

contribution.tex
- L26 "would reuslt in" → "result"; "would then collapses every demand" → "collapse".
- L73 "as this lead to too many assumptions" → "led".
- L127 "the system's behavior" → "behaviour".
- L140 "the numner of calls" → "number".
- L168 "create groups models" → "create groups of models".
- L217 "which is wht the solver uses" → "what".
- L234 "shares JobClass instances across layers meaning, any solver" → "…across layers, meaning any solver".
- L264 "mutate the ensembles structure" → "ensemble's structure".
- L305 "so that the MVA recursion never sees." → "never sees them."
- L498 "One case is left as out of scope … is an AND-fork" → "One case left out of scope … is an AND-fork".
- L513 "optimise the solver's performance on." → drop trailing "on".
- L525 "invariant during a solves, yes we still force the scans recompute" → "invariant during a solve, yet
  we still force the scans to recompute".
- L549 "the state they are establish" → "the state they establish".
- L555 double space "$\Delta_k=\Delta_{k-1}$,  but also".
- L557 "picking most recent result" → "picking the most recent result".
- L563 "in intialisation" → "initialisation".
- L567 "not directly measureable" → "measurable".
- L573 "meaniung the fluctuation" → "meaning".
- L573 "was targetted to callers" → "targeted".
- L575 "drives them towards the itration limit" → "iteration".
- L605 "inflating terations by 24--65\%" → "iterations".
- L609 "Foward is simply" → "Forward".
- L609 "Timings runtimes results measure" → "Runtime results measure" (garbled).
- L609 "(of which SolverLN and LQNS use)" → "(which SolverLN and LQNS use)".
- L632 "enforce per-iteration consistency correct fixed-point lock-in" → missing word, e.g. "…consistency,
  ensuring correct fixed-point lock-in".
- L644 "instead targetting its generalisability" → "targeting".

evaluation.tex
- L16 double space "within the  tolerance bounds".
- L22 double space "categorised to  match-ln".
- L28 "providing no change no in fixture iteration count" → "no change in".
- L32 "Disabling coonvergence control" → "convergence".
- L45 "marked as passing our solver matches either" → "marked as passing if our solver matches either".
- L45 double space "outputs differ.  We also".
- L59 "activity-graph capabilitiues" → "capabilities".
- L61 "It consists linear chains" → "It consists of linear chains".
- L63 "excercise the bare-demand heuristic" → "exercise".
- L97 "Partiton A" → "Partition"; "the postive-evidence paritions" → "positive-evidence partitions".
- L99 "structural complexity(A2" → space before "(".
- L227 "the $\sim$20 ms LQNS floor visible the eleven" → "visible in the eleven".
- L269 "1,260 ms of it is accounted by the three AND-fork fixtures despite all converging in three
  iterations, despite the other A5 fixtures convergin in 2--3 ms" → "…accounted for by the three AND-fork
  fixtures, despite all converging in three iterations, and the other A5 fixtures converging in 2--3 ms".
- L291 "so constructin time stays low" → "construction"; "penalised for usimg" → "using".
- L319 "not affecting iteration count and or output" → "iteration count or output".
- L324 "This causes the consumer short-circuits before" → "causes the consumer to short-circuit".
- L471 "This is an intersting case" → "interesting".
- L517 "woudld fix it" → "would".
- L536 "still a good win for a Java-vs-) (Section…" → **incomplete phrase** ("Java-vs-" trails off);
  finish it, e.g. "a good win for a Java/Kotlin prototype against a tuned C++ tool".
- L538 "On models that do trigger it ,we see" → space before comma; "difference  (Section" double space.
- L540 "where most the fixtures pinned" → "most of the fixtures".
- L564 "the redidual convergence-smoothing machinery" → "residual".

conclusion.tex
- L26 "However, his corpus was built top-down" → "this corpus".
- L33 "actually costs iterations than LQNS's" → "costs more iterations than".
- L47 "The 26.4\% residual we indentify" → "identify".
- L32 double space "A second phase then  optimises".

## 4. REFERENCES — existence, format, and per-reference usage summary

`main.tex:259` `\nocite{*}` forces ALL 30 bib entries into the list even if uncited. **28 of 30 are cited.**
Two are NEVER cited (appear only because of `\nocite{*}`): **`saad2003`** and **`woodside2002`**.
→ Recommend: remove `\nocite{*}`, and either cite these two (see §5) or drop them, so the list = cited works.
(The `\nocite{*}` comment claims related-work cites by author-year prose without `\cite` — that is now stale;
every related-work source IS `\cite`d.)

Format: `ieeetr` is numbered/Vancouver-numeric, ordered by first citation — acceptable as "Vancouver".
Entries are complete (author/title/venue/year). DOIs present on some, absent on others (ieeetr ignores DOI;
harmless). No malformed entries.

### Per-reference: where used and why
| Key | Source | Where cited | Why it's used |
|-----|--------|-------------|---------------|
| lazowska84 | Lazowska et al. 1984, *Quantitative System Performance* | intro; bg §2 (all laws, customers/centres, outputs, multiclass), §3 (MVA algorithms) — ~40 cites | Standard textbook for elementary QN theory, operational laws, and MVA — the spine of the Background. |
| franks99 | Franks PhD 1999 | intro; bg §4–§6 (PF assumptions, surrogates, SRVN, MOL, LQN, pipeline, layering, interlock, E[max]); contrib L477,498; eval L513 | Foundational LQN-solver thesis; almost every structural pipeline claim and the E[max]/CCD machinery trace here. |
| reiser80 | Reiser & Lavenberg 1980, JACM | bg §3 (exact MVA), §5 (Exact MVA variant) | Origin of the MVA recursion. |
| bard79 | Bard 1979 | bg §3 (approx MVA), §5 (AMVA) | Source of Bard–Schweitzer arrival-instant approximation. |
| schweitzer79 | Schweitzer 1979 | bg §3, §5 (with bard79) | Co-source of the Schweitzer AMVA approximation. |
| chandy82 | Chandy & Neuse 1982, Linearizer | bg §5 (MVA variants), §6 (LQNS inner kernel) | LQNS's default inner kernel (Linearizer). |
| seidmann87 | Seidmann et al. 1987 | bg §5 (MVA variants) | The c-server→single-server demand transform for multiserver MVA. |
| rolia92 | Rolia PhD 1992 | bg §5 (MVA variants) | Extension of Seidmann's transform to LQN solvers. |
| rolia95 | Rolia & Sevcik 1995, Method of Layers | bg §4 (MOL), §6 (strict layering, surrogate-delay coupling) | The MOL solution strategy LQN inherits. |
| franks2009 | Franks et al. 2009, TSE (LQNS) | intro; bg §4,§6; contrib L10,656; concl §future | Canonical LQNS V5 description; the secondary comparator. |
| casale2024 | Casale, Gao, Niu, Zhu 2024, TOMACS (LN) | bg §4,§6,§7; contrib L10,656; concl | Formal SolverLN description (primary comparator); source of the outer-loop machinery ablated. |
| franksli2012 | Franks & Li 2012, ICPE (One-Step MVA) | bg §7 (related work); concl §reflection | Primary iteration-economy paper; project positions outer-loop work against its inner-loop work. |
| zahorjan88 | Zahorjan, Eager, Sweillam 1988 | bg §5 (Linearizer accuracy) | Supports "Linearizer improves accuracy at moderate populations". |
| reiser81 | Reiser 1981 (queue-dependent servers) | bg §5 (LD-MVA) | Source of load-dependent MVA. |
| baskett75 | BCMP 1975, JACM | bg §2 (disciplines), §4 (BCMP assumptions); concl §scope | The product-form (BCMP) assumptions the method rests on; class-switching out-of-reach. |
| spirn79 | Spirn 1979 | bg §2 (scheduling disciplines) | Supports SIRO/random-selection discipline (single use; weakest link). |
| woodside89 | Woodside 1989, SRVN throughput | bg §4 (SRVN), §6 (loose layering / SRVN solver) | The SRVN model LQN's vocabulary derives from. |
| woodside95 | Woodside et al. 1995, TC | bg §4 (SRVN) | Fuller SRVN journal description; task/entry/phase vocabulary. |
| israr2007 | Israr, Woodside, Franks 2007 | bg §7 (Franks-Li eval corpus) | Identifies the 13-layer Bookstore model used in franksli2012's evaluation. |
| lewis2014microservices | Lewis & Fowler | intro L5 | Motivates microservice architectures. |
| heinrich2017 | Heinrich et al. 2017, ICPE | intro L5 | Microservice performance-analysis challenges. |
| urgaonkar2005 | Urgaonkar et al. 2005 | intro L5 | Blocking/contention/non-linearity in multi-tier services. |
| casale2020wsc | Casale 2020, WSC (LINE) | bg §6 (SolverLN) | LINE framework motivation (two-paradigm tooling gap). |
| islam2022 | Islam, Petriu, Woodside 2022 | concl §reflection | Work that recognises outer-loop costs but does no per-machinery decomposition. |
| bobbio2005 | Bobbio, Horváth, Telek 2005 | concl §scope (out-of-reach) | 3-moment APH propagation the simple solver cannot do. |
| gao2024 | Gao, Casale, Singhal 2024 | concl §scope + §future | Cache-modelling node types (out-of-reach construct). |
| shampine1997 | Shampine & Reichelt 1997, MATLAB ODE Suite | concl §reflection | Analogy: accreted-heuristic ODE solvers as a transfer candidate. |
| franks2022manual | LQNS User Manual 2022 | bg §6; contrib L10,656; (concl) | The LQNS manual, supporting the LQNS V5 description. |
| **saad2003** | Saad 2003, *Iterative Methods for Sparse Linear Systems* | **NEVER CITED** | Intended for convergence/iterative-methods context. Best home: contrib §opt "Classical sequence acceleration" (Anderson/Broyden/Aitken). |
| **woodside2002** | Woodside 2002, *Tutorial Intro to Layered Modeling* | **NEVER CITED** | Best home: intro or bg §4 as the tutorial-level LQN reference. |

## 5. PLACES THAT SHOULD CITE BUT DON'T (and a suggested source)

1. **contribution.tex:603-605** "Classical sequence acceleration" — names **Aitken Δ², Anderson acceleration,
   Nesterov momentum, Broyden quasi-Newton** with NO citation. Add `\cite{saad2003}` (covers
   Anderson/Broyden/iterative methods) — this also gives the uncited `saad2003` a proper home.
2. **introduction / background §4** — LQN introduced with no tutorial-level citation. Add `\cite{woodside2002}`
   (Woodside's "Tutorial Introduction to Layered Modeling") where LQNs are first defined — gives uncited
   `woodside2002` a home.
3. **Heidelberger–Trivedi E[max] identity** (`contribution.tex:505`, `evaluation.tex:513`) — currently
   attributed via `franks99`. If you want the primary source, add Heidelberger & Trivedi, *Analytic Queueing
   Models for Programs with Internal Concurrency*, IEEE TC 1983. Otherwise keep franks99 and use ONE name.

## 6. BACKGROUND SHORTENING CANDIDATES (only "if necessary"; page budget is clearly tight)

Background = 633 lines, the longest chapter; main.tex shows heavy page-count pressure (singlespacing,
tightened titlespacing). If trimming is wanted, lowest-risk cuts:
- §2.1 "Outputs" (Utilisation/Residence/Throughput/QueueLength, L127-146) heavily restates the Laws and
  multiclass material — could compress to a short paragraph + the formulas.
- The **Approximate multi-class MVA algorithm box** (L340-366) duplicates the single-class box structure;
  could be described in prose referencing the single-class algorithm rather than a full second listing.
- "Little's Law application" 4-level description (L86-102) is illustrative; could shrink.
Estimated saving ~0.7–1 page. I have NOT cut anything — tell me if you want these applied.

---
## STATUS

APPLIED (✏) across abstract / introduction / background / contribution / evaluation / conclusion:
- All ~70 spelling/grammar/typos in §3 (reuslt, numner, wht, indentify, intersting, woudld, coonvergence,
  capabilitiues, excercise, Partiton, postive/paritions, measureable, meaniung, itration, terations, Foward,
  constructin, usimg, redidual, behavior→behaviour, targetted/targetting, etc.) + the garbled sentences.
- `queuing`→`queueing` (4 spots); `\texttt{SolverLQNS}`→`LQNS` in prose (kept where it's the literal class).
- Removed the `\addedtop`/`\addedbot` "[ADDED FOR REVIEW]" macros.
- Citation-after-period style fixed in background (4 spots); stray space before `\cite` period.
- Added `\cite{woodside2002}` (intro, LQN intro) and `\cite{saad2003}` (contrib, acceleration methods) —
  fills two real citation gaps AND gives the two previously-uncited bib entries a home.

NOT TOUCHED — need your input / data (see §1, §2, §4, §6):
- ⚠ `UNK` runtime placeholders (contrib §opt, 2 spots) — fill from your optimisation-run data.
- ⚠ 371 (bounce) vs 539 (elevator) headline reconciliation in contrib "What optimising provides".
- ⚠ 568 baseline + 34.7% — verify vs final elevator run.
- ⚠ IS instability N=20 vs N=100 (contrib §step4) — reconcile.
- ⚠ Heidelberger–Trivedi vs inclusion-exclusion naming — pick one.
- `\nocite{*}` kept (now harmless: all 30 entries are cited). Remove it if you want list = cited-only.
- Background shortening candidates (§6) — not cut; say the word.
- `DRAFT` in 3 label names — cosmetic, not done.
</content>
