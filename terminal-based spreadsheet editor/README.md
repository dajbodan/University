# Terminal‑Based Spreadsheet Editor  
*(folder `terminal-based spreadsheet editor/` in the repo)*

A full‑screen **command‑line spreadsheet** written in modern **C++17**
that fulfils the specification in **`zadani.txt`**.

The program lets you edit a table interactively, write formulas,
evaluate numeric / string expressions, apply simple functions
(`sin`, `cos`, `abs`, *etc.*), **save / load** sheets and detect cycles
between cell references — all directly in a Unix terminal, no GUI
required.

---

## 1 Key Features (required by _zadani.txt_)

| Category       | Implemented Details |
|----------------|---------------------|
| **Data types** | Numbers (signed / floating), character strings |
| **Arithmetic** | `+ − * /` with parentheses, left‑to‑right evaluation |
| **String ops** | Concatenation with `.` & repetition with `*` |
| **Built‑ins**  | `sin(x)`, `cos(x)`, `abs(x)` *(add more easily via* **`src/…Function.hpp`** *templates)* |
| **Persistence**| `save  path/to/file.txt` & `load  path/to/file.txt` |
| **Printing**   | `print ( expression )` ‑‑ prints value only<br>`PrintTable()` ‑‑ dumps all cells as `A1 = …` & their formulas |
| **Cycle guard**| The AST checks for cyclic dependencies and cancels the statement with an error |
| **Undo safety**| If a statement fails (lex/parser error or cycle) the sheet is left unchanged |

All functionality is available through **single‑line commands** typed
into the running program.

---

## 2 Repository Layout

```
terminal-based spreadsheet editor/
├── Makefile                    ← one‑command build
├── zadani.txt                  ← Czech assignment text
├── examples/                   ← ready‑made demo sheets & scripts
├── src/                        ← ~35 C++17 implementation files
│   ├── main.cpp                ← entry point (creates Controller)
│   ├── Controller.*            ← REPL loop & I/O loaders
│   ├── (lexer / parser / AST / Function classes …)
└── README.md                   ← you are here
```

---

## 3 Build & Run

You only need **`g++ 11+`** and `make`.

```bash
cd "terminal-based spreadsheet editor"
make            # builds build/*.o and final binary `dajbodan`
./dajbodan      # start the interactive REPL
```

The program prints:

```
Begin
>                # ← cursor waits for a command
```

Type commands, one per line; hit **`Ctrl‑D`** (EOF) to quit.

Clean, rebuild & docs:

```bash
make clean      # object files + binary
make doc        # generates HTML via Doxygen
```

---

## 4 Quick Demo

```text
# numbers
A1 = 5
B1 = 10
print ( A1 + B1 )          # → 15

# strings
C1 = "cvut"
D1 = C1 . ".cz"            # string concat => "cvut.cz"
E1 = "-" * 5               # repetition    => "-----"

# functions
F1 = sin ( 1.57 )
G1 = abs ( -42 )

# formula referencing another cell
H1 = ( A1 + B1 ) * 3

PrintTable ( )             # pretty dump of every cell
save ./examples/mySheet.txt
load ./examples/resLoadAndSaveTest.txt
```

If you attempt something like

```
A1 = B1 + 1
B1 = A1 + 1
```

you’ll get **“Cycle detected. Statement is cancelled.”**

---

## 5 Extending the Spreadsheet

* **New functions** – derive from `UnaryFunction`/`BinaryFunction`, register
  the token in `CLexan.cpp`.
* **Operator precedence** – currently evaluated left‑to‑right as stated in
  zadani; adapt `CParser` if you want full precedence grammar.
* **CSV import/export** – hook into `TableSaver` / `TableLoader` interfaces.
* **Terminal colours** – tweak `src/Controller.cpp` prints (uses
  `std::cout` only, no `ncurses` dependency).

---

## 6 License 

Source code released under the **MIT License**.  
Feel free to use, modify and share; please credit the original repo.
