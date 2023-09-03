#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

#include "simplic.h"

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
    std::list<std::string> asmlines;
    std::stringstream buffer;

    if (SourceFile.is_open())
    {
        buffer << SourceFile.rdbuf();
        SourceFile.close();
    }
    else
    {
        std::cerr << "Cannot open file: " + std::string(argv[1]);
        return -1;
    }

    // start the compilation process 
    Simplic::AST::Node AST = Simplic::AST::BuildHollowAST(buffer.str());
    Simplic::AST::Print(AST);

    /*std::string s;
    while (std::getline(buffer, s, '\n')) {
        asmlines.push_back(s);
    }*/

    // got the assembly, now turn that into machine code
    std::list<uint16_t> mcCode = Simplic::Asm::ReadAsm(asmlines);

    // remove the file extention to prepare it for .hex
    std::string targetFilepath;
    size_t lastdot = std::string(argv[1]).find_last_of(".");
    if (lastdot == std::string::npos) targetFilepath = argv[1];
    else targetFilepath = std::string(argv[1]).substr(0, lastdot);

    // write machine code to file
    Simplic::Asm::WriteToHexFile(targetFilepath + ".hex", mcCode);

    // just a placeholder printout. Currently there are no outputs yet
    // std::cout << "Written to file \"" << targetFilepath << ".hex\" successfully" << std::endl;
    

    return 0;
}