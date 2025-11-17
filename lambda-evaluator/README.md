# Lambda Calculus Evaluator

This project implements an evaluator for untyped lambda calculus in Racket.  
It supports normal-order and applicative-order evaluation, macro expansion,  
η-reduction, and multi-argument lambda abstractions.

---

## ✨ Features

- **Normal-order evaluation**  
- **Applicative-order evaluation**  
- **Macro expansion**  
- **η-reduction / simplification**  
- **Lambda abstractions with multiple parameters**  
- **Verbose mode** for step-by-step reductions

All logic is implemented in `semestralka.rkt`.  
Several example expressions are provided in `examples.txt`.

---

## 🚀 Usage

### Normal-order evaluation
```racket
(normalEvaluation expr verbose?)
```
- `expr` – lambda expression (after macro expansion if needed)  
- `verbose?` – `0` for silent, `1` to print reduction steps  

### Applicative-order evaluation
```racket
(applicativeEvaluation expr)
```

### Macro expansion
```racket
(expansion expr)
```

### η-reduction
```racket
(optimilization expr)
```

---

## 📘 Examples (from `examples.txt`)

### Arithmetic
```racket
(normalEvaluation (expansion '(* ((λ (x y) (+ x y)) 5 6) 1)) 0)
(applicativeEvaluation (expansion '(* ((λ (x y) (+ x y)) 5 6) 1)))
```

### Nested lambdas
```racket
(normalEvaluation (expansion '((λ (x) (+ x ((λ (x) (+ x 1)) 3))) 2)) 0)
(applicativeEvaluation (expansion '((λ (x) (+ x ((λ (x) (+ x 1)) 3))) 2)))
```

### Boolean logic
```racket
(normalEvaluation (expansion '(NOT (OR F F))) 0)
(applicativeEvaluation (expansion '(NOT (OR F F))))
```

### Y-combinator (factorial)
```racket
(normalEvaluation
 (expansion
  '(Y (λ (f n) (zero n 1 (* n (f (- n 1))))) 4))
 0)
```

### η-reduction
```racket
(optimilization (expansion '(λ (x) ((λ (x y) (x y)) x))))
```

---

## 📄 Project Structure

```
semestralka.rkt     ; evaluator implementation
examples.txt        ; sample expressions
README.md           ; documentation
```

---

## ✔ Notes

- Macro expansion should be applied before evaluation when macros are used.  
- Verbose mode prints every reduction step and is useful for debugging.  
- Expressions follow standard Racket syntax.
