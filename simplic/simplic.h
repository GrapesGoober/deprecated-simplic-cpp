#pragma once

#include <iostream>
#include <list>
#include <stack>

namespace Simplic 
{
    // assembly stage, i.e. the final stage before the hex file
    namespace Asm
    {
        // Reads assembly and converts into machine code
        std::list<uint16_t> ReadAsm(std::list<std::string>);

        // Recieves machine code and writes to hex file
        void WriteToHexFile(std::string, std::list<uint16_t>);
    }

    struct AsmException : public std::exception 
    {
        std::string message;
        AsmException(std::string message) : message(message) {}
        const char* what() const throw ();
    };
}
