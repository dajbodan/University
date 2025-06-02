# Grid TSP via Mixed‑Integer Linear Programming  
*(folder `grid_tsp_lp/`)*

This mini–project formulates the **“vacuum‑cleaner world”** from classical AI
(textbook *Russell & Norvig, ch. 2*) as a **Travelling Salesman Problem (TSP)**
on a rectangular grid and then solves it *optimally* with a
**Mixed‑Integer Linear Program (MILP)** written in SageMath.

> **Goal**  
> Given an `m × n` grid and a set `C` of **dirty squares**, find the
> shortest Manhattan tour that starts on any dirty square, visits
> **every** dirty square exactly once, returns to the start and therefore
> gives the vacuum‑cleaner an action plan that uses the *fewest moves*.

The whole workflow lives in a single Jupyter notebook:

```
grid_tsp_lp/
├── grid_tsp_lp.ipynb      ← MILP model + interactive visualisation
└── README.md              ← you are here
```

---

## 1 Dependencies

| Package      | Why it’s needed                      |
|--------------|--------------------------------------|
| **SageMath ≥ 9.6** | Provides `graphs.Grid2dGraph` and `MixedIntegerLinearProgram`. |
| `jupyter`    | Notebook interface. |



## 2 Quick Start

1. Launch Jupyter with the **Sage** kernel.  
2. Open **`grid_tsp_lp.ipynb`**.  
3. Scroll to the final cell and tweak the two lines at the bottom:

```python
grid_size = (7, 6)           # rows, cols
C = {(0, 4), (2, 2), (4, 0), (6, 0), (6, 5)}   # dirty squares
solve_grid_tsp(grid_size, C)
```

4. **Run All**.  
   The notebook prints the optimal edges and pops up a graphic showing the
   cleaned tour in **red**.

---

## 3 Model Formulation

* **Graph** – vertices are grid coordinates `(row, col)` produced by
  `graphs.Grid2dGraph(m, n)`.  
* **Distance matrix** – Manhattan metric  
  `d(i, j) = |rᵢ−rⱼ| + |cᵢ−cⱼ|`, but **only** for pairs in `C` to keep the
  model compact.
* **Variables**  
  * `x[i,j] ∈ {0,1}` – 1 iff the tour uses edge `i → j`.  
  * `u[i]` – MTZ auxiliary variables for subtour elimination.
* **Objective** – minimise `∑ d(i,j)·x[i,j]`.
* **Constraints**  
  1. **Degree**: every dirty square has exactly 1 incoming & 1 outgoing edge.  
  2. **No self‑loops**: `x[i,i] = 0`.  
  3. **Subtour elimination**: standard Miller‑Tucker‑Zemlin (MTZ) linear
     constraints on `u[i]`.

The model is about *O(|C|²)* binary vars and solves instantly for
`|C| ≤ 15` on a laptop.

---

## 4 Visualisation

The helper `visualize(G, optimal_edges)`:

1. Expands each TSP edge into its **Manhattan path**,
2. Draws the grid (grey), dirty squares (green) and the optimal tour (red)
   using Sage’s built‑in plotting.

---

## 5 Adapting / Extending

* **Bigger grids** – just bump `grid_size` and `C`; Manhattan distances stay valid.  
* **Rectangular obstacles** – remove blocked vertices/edges from
  `Grid2dGraph` before building the LP.  
* **Different norms** – switch Manhattan to Euclidean and the objective still
  works; optimality is preserved but you lose the nice rectilinear drawing.  
* **Heuristics** – comment out the MTZ section to test how often the relaxed
  degree constraints already give a single tour (*surprisingly often* on
  random C).

---

## 6 License & Credits

Released under the **MIT License**.  

