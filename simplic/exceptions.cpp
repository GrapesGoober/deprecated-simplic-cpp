#include <iostream>
#include "simplic.h"

namespace Simplic 
{
    const char* CmplException::what() const throw () 
    {
        return message.c_str();
    }
    const char* AsmException::what() const throw () 
    {
        return "ASM Exception";
    }
}