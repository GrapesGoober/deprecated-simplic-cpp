#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

#include "frontend.h"
#include "backend.h"

int main(int argc, char* argv[])
{
    // cli should look like      smplc.exe 'file-to-compile'
    if (argc != 2)
    {
        std::cout << "Requires 1 argument: filepath" << std::endl;
        return 1;
    }

    // start reading the file
    std::ifstream SourceFile(argv[1]);

    if (!SourceFile.is_open())
    {
        std::cerr << "Cannot open file: " + std::string(argv[1]);
        return -1;
    }

    // placeholder asm lines
    std::list<std::string> asmlines;

    // got the assembly, now turn that into machine code
    std::list<uint16_t> mcCode = Simplic::BackEnd::ReadAsm(asmlines);

    // remove the file extention to prepare it for .hex
    std::string targetFilepath;
    size_t lastdot = std::string(argv[1]).find_last_of(".");
    if (lastdot == std::string::npos) targetFilepath = argv[1];
    else targetFilepath = std::string(argv[1]).substr(0, lastdot);

    // write machine code to file
    Simplic::BackEnd::WriteToHexFile(targetFilepath + ".hex", mcCode);

    // doesn't need this for the moment
    // std::cout << "Written to file \"" << targetFilepath << ".hex\" successfully" << std::endl;

    return 0;
}