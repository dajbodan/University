# Random‑Forest Regression – Life‑Expectancy Prediction  
*(folder `random_forest_regression/`, notebook `homework_02.ipynb`)*

This assignment tackles **Homework 2 – regression** for the CTU FIT course  
**BI‑ML1 – Foundations of Machine Learning**.  
The goal is to **predict life expectancy** (in years) for each
`Country × Year` pair using a tabular dataset with mixed feature types and
missing values.

| Model (best run) | Validation RMSE ↓ | Validation MAE ↓ |
|------------------|------------------:|-----------------:|
| **Custom Random Forest**<br>(35 trees, depth = 10, max_samples = 128) | **≈ 1.75** | **≈ 1.12** |
| Ridge Regression | 3.02 | 2.37 |

---

## 1 Repository layout

```
random_forest_regression/
├── data.csv               ← training data (22 columns, 2 938 rows)
├── evaluation.csv         ← hidden test set for grading
├── results.csv            ← predictions produced by the notebook
├── homework_02.ipynb      ← complete solution (EDA → preprocessing → models)
└── README.md              ← you are here
```

---

## 2 Dataset in a nutshell

* **Target:** `Life expectancy` (float, years)  
* **Numeric features (19)** – mortality rates, GDP, schooling, etc.  
* **Categorical features (3)** – `Country`, `Status`, `Year` (treated as
  numeric but could be ordinal).  
* Contains **NaNs** in almost every column.

The dataset is adapted from the public *WHO Life Expectancy* collection on
Kaggle.

---

## 3 Pre‑processing pipeline

```text
[Numeric] median‑impute ─┐
                         ├─► concatenate ─► RandomForestRegressor
[Categorical] mode‑impute ─► OneHotEncode ┘
```

*Imputation* handles missing values; **one‑hot encoding** turns the two
string columns into ≈ 190 dummy variables.  
We deliberately **skip scaling** because tree ensembles are invariant to
monotone feature transformations.

---

## 4 Custom Random‑Forest implementation

To demonstrate understanding, the notebook implements its **own** random‑forest
class (`CustomRandomForest`) that wraps
`sklearn.tree.DecisionTreeRegressor`:

* **Bootstrap sampling** – each tree gets a random subset of rows
  (`max_samples`).  
* **Feature bagging** – each split considers a random √F subset of features.  
* **Parallel fit** – hyper‑parameter grid searched with `joblib.Parallel`.

Grid searched hyper‑parameters:

| Name | Tested values |
|------|---------------|
| `n_estimators` | 35, 36, 37 |
| `max_depth` | 4 → 12 (step 2) |
| `max_samples` | 10 → |train| (step 2) |

The best triple is saved in `min_value_all_info`.

---

## 5 Re‑running everything

```bash
# ① set up a clean environment
python -m venv .venv && source .venv/bin/activate

# ② install dependencies (CPU)
pip install jupyter pandas numpy scikit-learn joblib matplotlib seaborn

# ③ launch the notebook
jupyter lab homework_02.ipynb
```

Then **Run‑All**.  Execution takes ~2 minutes on a laptop (≈ 1 000 model fits).

---


## 6 License & credits

* Coursework by *<your‑name>* (2024).  
* Data © World Health Organization (public domain).

Released under the **MIT License** – free to use, modify and share.
