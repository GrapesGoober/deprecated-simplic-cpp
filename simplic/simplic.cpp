#include <iostream>
#include <sstream>
#include <fstream>
#include <list>

import assembler;

int main(int argc, char* argv[])
{
    
    // cli should look like      smplc.exe 'file-to-compile'
    if (argc != 2)
    {
        std::cout << "Requires 1 argument: filepath" << std::endl;
        return 1;
    }

    // start reading the file
    std::ifstream asmfile(argv[1]);

    if (!asmfile.is_open())
    {
        std::cerr << "Cannot open file: " + std::string(argv[1]);
        return -1;
    }

    // remove the file extention to prepare it for .hex
    std::string targetFilepath;
    size_t lastdot = std::string(argv[1]).find_last_of(".");
    if (lastdot == std::string::npos) targetFilepath = argv[1];
    else targetFilepath = std::string(argv[1]).substr(0, lastdot);

    // write machine code to file
    std::ofstream hexfile(targetFilepath + ".hex");
    Simplic::Asm::FileToFile(asmfile, hexfile);
    std::cout << "Written to file \"" << targetFilepath << ".hex\" successfully" << std::endl;

    asmfile.close();
    hexfile.close();

    return 0;
    
}