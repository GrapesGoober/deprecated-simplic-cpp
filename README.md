# Simplic Assembler
### Author: Nachat K (GrapesGoober)
This branch contains the latest stable version of the assembler. This reads an assembly file and generate a machine code in .hex file, which could then be read by Neeman's Digital ROM module. This is the instructions for the SimplicCore. This is a VS2022 C++ project. There are no external dependencies. Note: the project is built in x64, but if your computer is of a different species, you can always build it yourself :)

### To run this...
Examples `FibbonaciTest.txt` assembly program and `FibbonaciTest.hex` machine code files are provided.
Execute the exe in your preferred cli. This has one argument: the file name of the asm file. (Note: asm file can be written in any format, just make sure to specify it)
`simplic.exe "your-asm-file.txt"`
For example: `simplic.exe "FibbonaciTest.txt"`
This will generate a .hex file with the same name. It contains the SimplicCore instructions. To upload this to Neeman's Digital, right click on the ROM (the instruction memory), go to advanced, tick the "Reload at model start" checkbox then put the .hex file location there. This should load the instructions into processor and it should be able to run your program.
