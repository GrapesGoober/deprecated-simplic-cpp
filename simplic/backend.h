// Simplic Compiler 2022, Author: Nachat K
// backend.h is the primary header file for the backend systems for the compiler


#pragma once
#include <iostream>
#include <list>

// assembly stage, i.e. the final stage before the hex file
namespace Simplic::BackEnd
{
    // Reads assembly and converts into machine code
    std::list<uint16_t> ReadAsm(std::list<std::string>);

    // Recieves machine code and writes to hex file
    void WriteToHexFile(std::string, std::list<uint16_t>);
}