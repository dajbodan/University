# Dimensionality-Reduction & Binary-Classification Mini-Project

A compact study of **how various dimensionality‑reduction (DR) techniques affect the performance of classic binary classifiers** on a small image dataset (1 500 samples, 28 × 28 grayscale pixels).

| Technique | Best Validation F1 | Dimensionality |
|-----------|-------------------:|---------------:|
| **Raw 784** + SVM (RBF) | **0.994** | 784 |
| **PCA** + SVM | 0.992 | 174 |
| **Truncated SVD** + SVM | 0.990 | 174 |
| **LLE** + SVM | 0.985 | 174 |
| **Raw 784** + Gaussian NB | 0.931 | 784 |

> *Numbers come from the notebook’s last execution (seed = 42, 5‑fold CV). Re‑running may vary by ± 0.5 %.*

---

## 1 Project Layout

```
dimensionality_reduction_binary_classification/
├── dimensionality_reduction_binary_classification.ipynb   ← all code & charts
├── train.csv                                              ← 1 500 × 785 CSV (label + pixels)
└── README.md                                              ← you are here
```

Everything lives inside a **single Jupyter notebook**—no separate modules: data load → train/val split → baseline models → DR → re‑train → compare.

---

## 2 Quick Start


The notebook automatically:

* reads **train.csv**
* scales pixel intensities to ⟨0, 1⟩
* stratifies into 80 % train / 20 % validation
* grid‑searches hyper‑parameters for each model

---

## 3 Notebook Walk‑Through

| Section | Purpose |
|---------|---------|
| **Data preview** | Show 25 random digits to sanity‑check labels *(balanced 0 vs 1)*. |
| **Baseline (no DR)** | Train SVM (RBF), Gaussian NB & LDA on raw 784‑D space. |
| **PCA** | Try 54, 94, 134, 174 PCs (≈ 70 %–95 % variance) and re‑evaluate SVM. |
| **Truncated SVD** | Same dims as PCA but via randomized SVD (faster, sparse‑friendly). |
| **Locally Linear Embedding** | Non‑linear manifold learning (*n_neighbors = 5*; dims 54‑174). |


---

## 4 Using the Code on Your Data

1. Replace **train.csv** with your **own** CSV containing a `label` column followed by numeric features.  
2. Edit the `load_data()` helper at the top of the notebook (≈ 2 lines).  
3. Adjust the `dims_to_try` list to suit your dataset size.  
4. **Run All**—the rest is parameterised.

---

## 6 License

Released under the **MIT License**—feel free to use, modify, and distribute.  
If you publish results based on this notebook, please credit the repo.
