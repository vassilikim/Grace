# Grace Compiler

This is a compiler for the language "Grace" developed as a project for ECE NTUA (Compilers). The compiler is designed to translate Grace source code into intermediate language code and assembly code. The compiler was implemented using C++ and the tools flex, bison and LLVM-14.0.0.

## Features

- Lexical analysis (Tokenizer)
- Syntax analysis (Parser)
- Semantic analysis
- Intermediate code generation
- Code optimization
- Code generation
- Symbol table management
- Error handling

## Getting Started

1. Clone the repository:

   ```bash
   git clone https://github.com/vassilikim/Grace.git
   cd Grace_demo
   ```

2. Build the compiler:

   ```bash
   make
   ```

## Usage

- Give the input _file.grc_ as an argument. Running this command will create _file.imm_ and _file.asm_ in the same directory as the input file with the intermediate language code and assembly code respectively.

  ```bash
  ./grace /path/file.grc [-O]
  ```

- After running this command, give the Grace source code in the standard input and the assembly code generated will be printed at the standard output.

  ```bash
  ./grace -f [-O]
  ```

- After running this command, give the Grace source code in the standard input and the intermediate language code generated will be printed at the standard output.

  ```bash
  ./grace -i [-O]
  ```

If the optimization flag -O is used, the intermediate code will get optimized.

## Program Execution

By running the _make_ command, the file _lib.a_ will get generated from _lib.c_, containing the implementations of the library functions. By linking this file with _file.asm_, the executable _a.out_ will be created:

```bash
clang -no-pie temp.asm lib.a
```

You can run the executable with the command:

```bash
./a.out
```
