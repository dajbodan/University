# Pre‑processing, Decision Tree & k‑NN Mini‑Project  
*(folder `preprocessing_decision_tree_knn/`)*

This notebook shows **end‑to‑end tabular‑data classification** with two
classic algorithms:

1. **Decision Tree** (CART) with depth / impurity hyper‑tuning  
2. **k‑Nearest Neighbours** with distance weighting

Both models share a **scikit‑learn `Pipeline`** that

* imputes missing values (median / most‑frequent),  
* standardises numeric columns,  
* one‑hot encodes categorical columns.

| Pipeline |
|---------------------:|
| **Decision Tree** (max_depth = 7) | **0.875** |
| **k‑NN** (k = 9, distance = Euclidean) | 0.862 |


---

## 1 Repository Map

```
preprocessing_decision_tree_knn/
├── preprocessing_decision_tree_knn.ipynb   ← all code, plots & discussion
├── train.csv                               ← tabular dataset (label + features)
└── README.md                               ← you are here
```

`evaluate.csv` for the hidden test set is not version‑controlled; place it
next to the notebook before exporting predictions (see § 5).

---

## 2 Notebook Road‑Map

| Section | What happens |
|---------|--------------|
| **Data inspection** | Shows `head()`, `info()`, class distribution and a heat‑map of missing values. |
| **Pre‑processing pipeline** | Builds a `ColumnTransformer` with *numeric* & *categorical* branches (imputation + scaling / one‑hot). |
| **Baseline Decision Tree** | Trains default `DecisionTreeClassifier`, prints accuracy & confusion matrix. |
| **GridSearch – Tree** | Tunes `max_depth`, `min_samples_leaf`, `criterion`; keeps the best CV model. |
| **GridSearch – k‑NN** | Tunes `n_neighbors`, uniform vs. distance weighting, optional `p` for Minkowski metric. |
| **Model comparison** | Bar plot of cross‑validated accuracy + detailed classification report. |
| **Feature importance (Tree)** | Plots Gini importances for top 15 features. |
| **Prediction export** | Applies the best model to `evaluate.csv`, writes `result.csv` in `ID,label` format. |

---

## 3 Re‑using the Code on Your Data

1. Replace **train.csv** with your own dataset (must have a `label`
   column).  
2. List column names in the `num_cols` / `cat_cols` variables at the top
   of the notebook.  
3. Optionally extend the pipeline with Target Encoding, Polynomial
   features, log‑scaling, etc.  
4. **Run All** – training, tuning and export adapt automatically.

---

## 4 Exporting Predictions

With `evaluate.csv` in place, run the last cell:

```python
best_model.predict(X_eval)
```

A **`result.csv`** is produced alongside the notebook; upload it to the
grading server.

---
## 5 License

Released under the **MIT License** – feel free to re‑use, modify and
share.  Please credit the repo if you use this code in academic work.
