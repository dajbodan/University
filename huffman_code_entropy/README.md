# Huffman Coding & Entropy Analysis  
*(folder `huffman_code_entropy/`)*


In short, we

1. estimate **symbol probabilities** (letters + space) for each text,  
2. **visualise** the distributions,  
3. compute the **entropy** H of each distribution,  
4. build an **optimal instantaneous binary code** C for the *first* text
   using the **Huffman algorithm**,  
5. measure the **average code‑word length** 𝐿(C) for *both* texts and
   discuss optimality.

The whole solution lives in a single Jupyter notebook:

```
huffman_code_entropy/
├── 001.txt                    ← first Czech prose excerpt
├── 011.txt                    ← second Czech prose excerpt
├── huffman_code_entropy.ipynb ← complete analysis & plots
└── README.md                  ← you are here
```

---

## 1 Quick Start

```bash
# (1) create & activate a fresh env (recommended)
python -m venv .venv && source .venv/bin/activate

# (2) install required packages
pip install jupyter pandas matplotlib

# (3) open the notebook
jupyter lab huffman_code_entropy.ipynb
```

Run **all cells** – execution takes \< 1 s.

---

## 2 What the Notebook Does

| Section | Code highlights | Outputs |
|---------|-----------------|---------|
| **Load & preprocess** | `Counter`, lowercase mapping, keep **letters a–z + space**. | Text length & filtered length stats. |
| **Probability estimation** | Normalise symbol counts. | Two bar plots (probability vs. symbol). |
| **Entropy** | `H = −Σ pᵢ log₂ pᵢ` for each text. | Prints `H₁`, `H₂` in bits / symbol. |
| **Huffman coding** | Classic priority‑queue implementation (`Leaf`, `Node`). | Table `char → code`, prefix‑free. |
| **Average length** | `𝐿 = Σ pᵢ · |codeᵢ|` for *C* on text 1 and **also** on text 2. | 𝐿(C, text 1) and 𝐿(C, text 2). |
| **Optimality check** | Shows `𝐿 − H` gaps & explains why Huffman is minimal. | Commentary cell with justification. |

---

## 3 Why the Code C is Optimal

The Huffman algorithm constructs a **prefix‑free (instantaneous) code**
whose average length 𝐿 satisfies  

> **H ≤ 𝐿 < H + 1**

and is **provably minimal** among all binary prefix codes for the given
probability vector.  
Because our implementation uses the canonical algorithm (repeatedly
merging the two least‑probable symbols), the resulting code *must* be
optimal for the **first text**.

For the **second text** we reuse the same code and observe  
`Δ = 𝐿(C, text 2) − H₂ > 0`.  
If Δ is close to 1 bit we know we can build a shorter code tailored to
the second distribution; hence C is **not** optimal there.

---

## 4 License & Credits

Released under the **MIT License** – feel free to use, modify and
redistribute.  If you publish derived work, please cite this repo.
