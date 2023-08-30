# Simplic Compiler's Hollow Parser Functions
### Author: Nachat K (GrapesGoober)
This branch contains the development of the Hollow Parser Functions (HPFs). HPFs are responsible for generating the initial Hollow Parse Tree. This stage is called hollow to distinguish it from the Complete Parse Tree. HPFs only concerns itself with context-free codes, which are namespaces and definitions. Function bodies and expresstions are not contex-free, so they are excluded. This stage builds the HPT, which can then be used as a symbol table to later populate itself to become CPT. Here is the **detailed** explaination of the HPFs.

## Tokenizer Functions (TNFs)
