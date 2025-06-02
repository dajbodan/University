# 8‑Puzzle SAT Solver (Z3 Notebook)

Solve the 3 × 3 sliding‑tile **8‑puzzle** by reducing it to a propositional satisfiability problem and letting the
[Z3 SMT solver](https://github.com/Z3Prover/z3) search for a plan.
The implementation lives entirely in a single Jupyter notebook – no extra scripts needed.

> **Decision problem** – *Given an upper bound **k**, does there exist a sequence of ≤ k blank‑tile moves that transforms the start board into the goal board?*

---

## 1  Problem Statement

* **Start configuration** `S₀` (pictured below ☞ the notebook pre‑loads the one from the assignment).
* **Goal configuration** `G` <sub>(1 2 \_), (3 4 5), (6 7 8)</sub>
* **Depth bound** `k` – maximum moves allowed (default **26**).

Encode all board states for time steps `t = 0 … k` as Boolean variables, constrain legal transitions, assert `S₀` at `t = 0` and `G` at `t = k`, then ask Z3 for satisfiability. A model ⇢ a concrete move sequence; UNSAT ⇢ impossible within *k* moves.

---

## 2  Repository Layout

```text
8-puzzle-sat-solver/
└── 8-puzzle-sat-solver.ipynb   # everything happens here
```
---

## 3  Inside the Notebook – Encoding Sketch

| Variable     | Meaning                                               |
| ------------ | ----------------------------------------------------- |
| `x[i][j][t]` | *Tile i is at board position j at time t* (i = 1 … 8) |
| `y[j][t]`    | *Blank is at position j at time t* (i = 0)            |

Constraints (expressed in Z3):

1. **Exactly‑one per cell** – every position holds either one numbered tile or the blank.
2. **Exactly‑one per tile** – a tile occupies exactly one position at each time step.
3. **Legal move** – between `t` and `t+1` *only* the blank swaps with a horizontally/vertically adjacent tile.
4. **Initial / Goal** – fix `x` & `y` at `t = 0` and `t = k` to `S₀` and `G` respectively.

The code relies on Z3’s high‑level constraints; no DIMACS is generated.

---

## 4  Changing the Puzzle or Depth

Open the first code cell and edit:

```python
k = 30                                       # new depth
initial_state = [7, 2, 4, 5, None, 6, 8, 3, 1]  # flat list, None = blank
goal_state    = [None,1,2, 3,4,5, 6,7,8]
```

Re‑run the notebook – everything else adapts automatically.

---

## 5  What to Expect

* For the default instance the solver finds a **24‑move** solution in under a second on a laptop.

---

## 6  License

MIT license for this notebook.  Z3 is used under its official MIT license as well.
