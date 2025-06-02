# Terminal‑Based Spreadsheet Editor  
*(folder `terminal_spreadsheet/`)*

> **Assignment summary (see `Zadani.txt`)**  
> Implement a **CLI spreadsheet** that runs entirely in a Unix terminal
> (no GUI) and supports interactive editing, basic formulas, cell
> navigation and CSV import/export.

The project is written in **Python 3** using the `curses` library for
full‑screen text UI.  It aims to reproduce a small subset of the
functionality of classic tools like *VisiData* or the *Vim* plugin
*vim‑spreadsheet*, but with a clean codebase and zero external
dependencies.

---

## 1 Features

| Category       | Details |
|----------------|---------|
| **Navigation** | Arrow keys or `h j k l` (Vim‑style) to move the cursor.  `Home`, `End`, `PgUp`, `PgDn` to jump. |
| **Editing**    | Press **`e`** to edit a cell; accepts plain text or a formula starting with `=`.  Press **`u`** to undo the last change. |
| **Formulas**   | Support for `+ − * /`, parentheses and cell references (`A1`, `B2`, …).  Re‑evaluation is automatic on dependencies change. |
| **File I/O**   | **`o`** – open a CSV file. **`s`** – save current sheet to CSV (overwrites or asks for path). |
| **Sheets**     | Multiple tabs: **`gt`** / **`gT`** cycle forward/backward. |
| **Status bar** | Shows coordinates, current value, mode (INSERT/NORMAL), file name and dirty flag. |
| **Help**       | **`?`** or **`F1`** pops a cheat‑sheet of all key bindings. |

*(Everything above is required by the assignment; see § 2 of Zadani.txt)*

---

## 2 Repository Layout

```
terminal_spreadsheet/
├── spreadsheet.py           ← main curses application
├── formulas.py              ← recursive‑descent expression parser & evaluator
├── io_csv.py                ← load/save helpers
├── state.py                 ← undo stack & dirty‑flag tracking
├── demo.csv                 ← tiny sample sheet
└── README.md                ← you are here
```

---

## 3 Installation

```bash
git clone https://github.com/your‑user/terminal_spreadsheet.git
cd terminal_spreadsheet

python -m venv .venv
source .venv/bin/activate

pip install -r requirements.txt   # just tabulate for CSV pretty‑print
```

> **Note:** On macOS you may need  
> `export TERM=xterm‑256color`  
> before running, otherwise `curses` uses limited colours.

---

## 4 Usage

```bash
python spreadsheet.py [path/to/file.csv]
```

Key bindings (default Vim‑like *NORMAL* mode):

| Key | Action |
|-----|--------|
| `h j k l` / arrows | Move left / down / up / right |
| `e` | Edit cell (enters *INSERT* mode) |
| `Esc` | Leave *INSERT* mode |
| `s` | Save (`write.csv` if no file yet) |
| `o` | Open CSV |
| `u` | Undo |
| `Ctrl‑r` | Redo |
| `?` | Toggle help panel |
| `q` | Quit (asks to save if sheet is dirty) |

Formulas begin with `=` and can reference cells:

```
=C1 + D1 * 2
=SUM(A1:A10)
```

The grammar is roughly:

```
expr  → term ((+ | −) term)*
term  → factor ((* | /) factor)*
factor→ number | cell | (expr)
```

---

## 5 Testing

A minimal unit test suite lives in **`tests/`** (run via `pytest`).  
It checks:

* correct tokenisation & parsing of formulas  
* cycle‑detection in dependency graphs  
* undo/redo integrity

Run:

```bash
pip install pytest
pytest
```

All tests should pass.

---

## 6 License

Released under the **MIT License**.  
© 2024 *<your‑name>* – feel free to use, modify and share.  Please link back
if you fork.
