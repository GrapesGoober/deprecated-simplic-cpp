// Simplic Compiler 2022, Author: Nachat K
// frontend.cppm is the primary module file for the frontend systems for the compiler
// this includes the struct and functions used by TOFs and other parsing functions

export module frontend;
#include <iostream>
#include <list>
#include <map>

export namespace Simplic::FrontEnd
{
    export struct Node
    {
        std::string type;
        std::string lexeme;
        int location;
    };

    export struct Cursor
    {
        std::ifstream& file;
        int index;
    };
}