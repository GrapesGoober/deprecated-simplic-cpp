# Simplic Compiler 

### Author: Graps, Nachat K.
This branch currently contains the latest stable version of the assembler and some one result example. The assembler reads a simplic assembly file (in plain text) and generate a machine code in .hex file, which could then be read by Neeman's Digital ROM module. The assembler is written in C++20 standard, and there are no external dependencies.

## To run this...
Examples `FibbonaciTest.txt` assembly program and `FibbonaciTest.hex` machine code file are provided. However, if you want to run using your own assembly program, you can build and run the assembler. The program has one argument: the file name of the simplic asm file. Note that the assembly file is written in plain text. Execution should look like

`simplic.exe "your-asm-file.txt"`
For example: `simplic.exe "FibbonaciTest.txt"`

This will generate a .hex file with the same name. It contains the SimplicCore instructions. To upload this to Neeman's Digital, right click on the ROM (the instruction memory), go to advanced, tick the "Reload at model start" checkbox then put the .hex file location there. This should load the instructions into processor and it should be able to run your program.

## About The Compiler
Simplic is a series of personal coding projects with the goal of becoming a simplified representation of modern computers, all the way from a series of logic gates, to a soft-core processor, to a functional programming language, and to a running software, all from scratch. Every part of this project is meant to be a simplified analogy of their respective counterparts.

SimplicCompiler, which started in December 2022, is a project under Simplic. It is a simplified compiler written in C++, which parses a unique C-like language simply called Simplic, into its own unique machine code (in hex file) to be used for SimplicCore. Its main goal is to be reduced, simplified, and readable, while maintaining the functionality to be analogous to other compilers.

## About The Language
Simplic language is also intended to be reduced and simplified, while also maintaining a similar syntax to most languages (to be readable by most people and be analogous to other programming languages). The syntax is inspired by Golang, with its simplified nature of C, and so Simplic also has a C-like syntax. 

The details of Simplic language has not yet been finalized besides the syntax. Nevertheless, Simplic is intended to be as simple and bare-bone as possible without compromising functionality. Simplic would be a functional language, much like C, but with structs, generics, and simple polymorphism. It is not intended to be an OOP language.

## How It Works
This section will only give an overview of the inner workings. A full complete documentation does not yet exist. 

SimplicCompiler is separated into 3 parts, as follows
### Front End
Takes care of parsing the initial source code, as strings, into a High-Level Intermediate Representation (HIR). This HIR will take the form of a Parse Tree (PT), which comes in two flavours: Hollow PT (or HPT), and Complete PT (or CPT). Here is the detailed explanation:
1) The Tokenizer reads the source code, as a one long string, into tokens, implemented as AST::Node. Note that it does not generate a list nor array of AST::Node. Instead, it parses the strings into nodes along as it build the HPT.
2) The HollowParser functions generates the HPT using the Tokenizer. HPT is built by iteratively populating the root node as it tokenizes. This parsing process only considers the namespaces, function signatures, constants, and structs. It does not consider function bodies. This stage checks the most of the definition's syntatic errors.
3) The Analyzer functions does typechecks and further syntatic and structural checks. This would prepare the HPT to be populated.
4) The CompleteParser functions parses the function bodies and populate the HPT. Once completed, this will become the CPT. 
5) The Analyzer will then further typecheck the CPT. Since the CPT also acts as a functionally complete symbol table, it is used to typecheck itself.

### Medium End
unimplemented

### Back End
unimplemented

For futher understanding of the theory, I recommend "Advanced Compiler Design and Implementation" by Steven Muchnick. 
