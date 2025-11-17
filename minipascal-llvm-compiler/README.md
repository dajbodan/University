# MiniPascal LLVM Compiler

A small compiler for a Pascal-like language that generates LLVM IR.
The project includes a hand-written lexer, recursive-descent parser,
AST representation, and LLVM-based code generation, plus a tiny C
runtime (`writeln`, `write`, `readln`).

## Features

- Pascal-like syntax:
  - `program ...;`, `const`, `var`
  - `integer` variables and `array [lo .. hi] of integer`
- Control flow:
  - `if ... then ... else`
  - `while` and `for` loops
- Expressions:
  - arithmetic `+ - * /`
  - relational `< <= > >= = <>`
- Arrays:
  - array declarations and indexed access using `getelementptr`
- Built-ins:
  - `writeln`, `write`, `readln` implemented in a C runtime
