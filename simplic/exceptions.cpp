#include <iostream>
#include "simplic.h"

namespace Simplic 
{
    const char* AsmException::what() const throw () 
    {
        return "ASM Exception";
    }
}