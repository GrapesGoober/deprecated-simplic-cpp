# Simplic Compiler 

### Author: Graps, Nachat K.
This branch currently contains the latest stable version of the assembler and some one result example. The assembler reads a simplic assembly file (in plain text) and generate a machine code in .hex file, which could then be read by Neeman's Digital ROM module. The assembler is written in C++20 standard, and there are no external dependencies.

## To run this...
Examples `FibbonaciTest.txt` assembly program and `FibbonaciTest.hex` machine code file are provided. However, if you want to run using your own assembly program, you can build and run the assembler. The program has one argument: the file name of the simplic asm file. Note that the assembly file is written in plain text. Execution should look like

`simplic.exe "your-asm-file.txt"`
For example: `simplic.exe "FibbonaciTest.txt"`

This will generate a .hex file with the same name. It contains the SimplicCore instructions. To upload this to Neeman's Digital, right click on the ROM (the instruction memory), go to advanced, tick the "Reload at model start" checkbox then put the .hex file location there. This should load the instructions into processor and it should be able to run your program.
