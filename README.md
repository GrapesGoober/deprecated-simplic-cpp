# Simplic Compiler's Hollow Parser Functions
### Author: Nachat K (GrapesGoober)
This branch contains the development of the Hollow Parser Functions (HPFs). HPFs are responsible for generating the initial Hollow Parse Tree. This stage is called hollow to distinguish it from the Complete Parse Tree. HPFs only concerns itself with context-free codes, which are namespaces and definitions. Function bodies and expresstions are not contex-free, so they are excluded. This stage builds the HPT, which can then be used as a symbol table to later populate itself to become CPT. Here is the **detailed** explaination of the HPFs.

## Tokenizer Functions (TNFs)
The Tokenizer Functions (TNFs) reads the input file stream `std::ifstream` of the source code character by character, then returns a boolean (whether successful or not) and if true, will reject the results into `Simplic::Node`, representing a new token. This token contains the informations such as 
- token type (identifier, operator, etc)
- the lexeme ("int", "myVariable" etc)
- the cursor-index (to keep track of the location this token, which is to be used later on for throwing compiler exceptions) 
This node will then be used to construct both Hollow Parse Tree (HPT) and Complete Parse Tree (CPT). The way the TNFs parses the source code is dependent on the specific function at hand. TNFs are used by the Hollow Parser Functions (HPFs) and Complete Parser Functions (CPFs).