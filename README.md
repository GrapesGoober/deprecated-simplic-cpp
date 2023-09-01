# Simplic Compiler's Unified Parser Functions
### Author: Nachat K (GrapesGoober)

This branch aims to explore the possibility of developing the Unified Parser Functions as opposed to HPFs and CPFs. Note that, if this is completed, it would not be called  UPFs. The term UPFs are only a placeholder name used to describe this concept and to distinguish it from the original HPFs and CPFs. Here are my ideas of how UPFs should look like.

- Tokenizer Functions (TOFs): These functions act as an interface with the source code file. It reads from `ifstream` and generates tokens.
- Expression Parser Functions (EPFs): These functions recursively parses expressions. It uses TOFs to read the source code file.
- Definition Parser Functions (DPFs): These functions iteratively parses types, constants, and function definitions. Since there are expressions interweaved within definitions, DPFs would utilize both EPFs and TOFs to parse.

All of these would be used to build the Initial Parse Tree (IPT), which will then be type-checked to later become Checked Parse Tree (CPT).
