# Simplic Compiler 
Author: Graps, Nachat K.
## About The Compiler
Simplic is a series of personal coding projects with the goal of becoming a simplified representation of modern computers, all the way from a series of logic gates, to a soft-core processor, to a functional programming language, and to a running software, all from scratch. Every part of this project is meant to be a simplified analogy of their respective counterparts.

SimplicCompiler, which started in December 2022, is a project under Simplic. It is a simplified compiler written in C++, which parses a unique C-like language simply called Simplic, into its own unique machine code (in hex file) to be used for SimplicCore. Its main goal is to be reduced, simplified, and readable, while maintaining the functionality to be analogous to other compilers.

## About The Language
Simplic language is also intended to be reduced and simplified, while also maintaining a similar syntax to most languages (to be readable by most people and be analogous to other programming languages). The syntax is inspired by Golang, with its simplified nature of C, and so Simplic also has a C-like syntax. 

The details of Simplic language has not yet been finalized besides the syntax. Nevertheless, Simplic is intended to be as simple and bare-bone as possible without compromising functionality. Simplic would be a functional language, much like C, but with structs, generics, and simple polymorphism. It is not intended to be an OOP language.

## How It Works
SimplicCompiler is separated into 3 parts, as follows
### Front End
Takes care of parsing the initial source code, as strings, into a High-Level Intermediate Representation (HIR). This HIR will take the form of a Parse Tree (PT), which comes in two flavours: Hollow PT (or HPT), and Complete PT (or CPT). Here is the detailed explanation:
1) The Tokenizer reads the source code, as a one long string, into tokens, implemented as AST::Node.
2) The HollowParser functions generates the HPT using the Tokenizer. This parsing process only considers the namespaces, function signatures, constants, and structs. It does not consider function bodies. This stage checks most of the syntatic errors.
3) The Analyzer functions does typechecks and further syntatic and structural checks. This would prepare the HPT to be populated.
4) The CompleteParser functions parses the function bodies and populate the HPT. Once completed, this will become the CPT. This stage will also do further typechecking as well.

### Medium End
unimplemented

### Back End
unimplemented

For futher understanding of the theory, I recommend "Advanced Compiler Design and Implementation by Steven Muchnick. 
