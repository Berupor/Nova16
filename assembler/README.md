
# Nova16 Assembler

This is the assembler for the `nova16`. It takes `.nova` source files and compiles them into flat binary programs that the VM can execute.

## Pipeline

The assembler is broken down into 5 stages, each doing one job:

### 1. Lexer

Turns raw source lines into flat tokens. These are just `{type, value, line}` structs. No context yet, just basic splitting of opcodes, registers, numbers, strings, etc.

### 2. Parser

Turns the token stream into a structured AST. Recognizes instructions, labels, and directives like `ENTRY` and `INCLUDE`.

### 3. Linker

Handles `INCLUDE` directives. Recursively loads and merges external `.nova` files (like `lib/add.nova`) into the main AST. Result: one flat program.

### 4. Resolver

This is where labels get resolved into real addresses.

- Builds the symbol table (`label → address`)
- Computes byte offsets for each instruction
- Rewrites label-based args like `CALL add_3` into absolute numbers

Also checks that the entry point label (`ENTRY main`) exists.

### 5. Emitter

Final step: encodes everything into raw bytecode, with a proper binary header.