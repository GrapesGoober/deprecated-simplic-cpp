// Simplic Compiler 2022, Author: Nachat K
// frontend.h is the primary header file for the frontend systems for the compiler
// this includes the struct and functions used by TOFs and other parsing functions

#pragma once
#include <iostream>
#include <list>
#include <map>

namespace Simplic::FrontEnd
{
    struct Node
    {
        std::string type;
        std::string lexeme;
        int location;
    };

    struct Cursor
    {
        std::ifstream& file;
        int index;
    };
}