// Simplic Compiler, 2022, Author: Nachat Kaewmeesang
// simplic.cpp contains entry point of the compiler (the main function)
// this is a good starting point to read how this compiler works

#include <iostream>
#include <sstream>
#include <fstream>
#include <list>

import simplic;
import simplic.tofs;
import simplic.assembler;

int main(int argc, char* argv[])
{
    // read cli arguments to get the filepath
    // cli should look like      smplc.exe 'file-to-compile'
    if (argc != 2)
    {
        std::cout << "Requires 1 argument: filepath" << std::endl;
        return 1;
    }

    // start reading the file
    std::ifstream sourcefile(argv[1]);
    if (!sourcefile.is_open())
    {
        std::cerr << "Cannot open file: " + std::string(argv[1]);
        return -1;
    }

    // reads the file and tokenize, to test the TOFs
    char ch;
    Simplic::Cursor cursor(sourcefile);

    sourcefile.close();

    // remove the file extention to prepare it for .hex
    std::string hexfilePath = Simplic::Asm::ReplaceExtentionToHex(argv[1]);
    // currently the development does not write to hex file
    // Simplic::Asm::BinaryToHexfile(hexfilePath, binary);
    // std::cout << "Written to file \"" << hexfilePath << ".hex\" successfully" << std::endl;
    return 0;
    
}