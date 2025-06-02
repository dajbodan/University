# Neural‑Networks Image‑Classification Project  
*(folder `neural_networks/`, notebook `neural_networks_images.ipynb`)*

This homework notebook (Úkol 2, CTU FIT course **BI‑ML2 – Machine Learning**)
builds, trains and tunes several **PyTorch** neural‑network models to
classify **Fashion‑MNIST‑like** 32 × 32 grayscale images into 10 clothing
categories.

| Pipeline
|-------------------------------
| 3‑layer **MLP** (ReLU, Dropout)        
| 4‑layer **CNN** (3×3 conv, max‑pool)    
| VGG

*(numbers reproduced with seed = 42; ±0.5 % variance per run)*

---

## 1 Repository Map

```
neural_networks/
├── neural_networks_images.ipynb   ← complete end‑to‑end solution
└── README.md                      ← you are here
```

The **CSV datasets** (`train.csv`, `evaluate.csv`) are *not* tracked in git
due to size limits – place them next to the notebook before running.

---

## 2 Quick Start

```bash
# 1) create & activate a fresh env  (PyTorch needs ≥ Py3.9)
python -m venv .venv && source .venv/bin/activate

# 2) install python deps (CPU build)
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu
pip install pandas scikit-learn matplotlib tqdm

# 3) launch Jupyter
jupyter lab neural_networks_images.ipynb
```

> **GPU:** on Colab or a CUDA host no changes are required – the notebook
> auto‑detects `cuda` / Apple MPS and moves the tensors accordingly.

---

## 3 Notebook Walk‑Through

| Section | Purpose |
|---------|---------|
| **Čtení vstupu** | Loads `train.csv`, normalises pixel values to ⟨0, 1⟩ and splits the 32 × 32 grid into a `(1, 32, 32)` tensor. |
| **Průzkum dat** | Shows 4 × 4 image grid and class distribution for sanity check. |
| **Baseline MLP** | Defines `Net` (3 dense layers), trains for 20 epochs, plots loss & accuracy curves. |
| **Hyper‑param GridSearch** | Loops over hidden sizes, dropout, optimiser (AdamW / SGD) & LR; keeps the best checkpoint on the **validation set**. |
| **CNN block factory** | Utility `make_convolutional_block()` to stack Conv → BN → ReLU → Pool. |
| **CNN training** | Trains 4‑conv architecture, optional data augmentation (`RandomHorizontalFlip`, `ColorJitter`). |
| **Early‑Stopping & LR Schedule** | Implements patience‑based early stop and `CosineAnnealingLR`. |
| **Predikce & Export** | Runs the best model on `evaluate.csv` and saves `result.csv` in the required `ID,label` format. |

---

## 4 Re‑using the Code

* **Different image size** – adjust `input_size` in the MLP or the first
  Conv layer’s `in_channels`.  
* **Another dataset** – replace the loading cell; shape `(N, pixels)` or
  `(N, H, W)` is expected.  
* **More classes** – change `output_size` (final layer) and the `CLASSES`
  list used for pretty plots.

---
## 5 License

Released under the **MIT License** – you are free to use, modify and
redistribute.  If you publish results or derived work, please cite this
repository.
