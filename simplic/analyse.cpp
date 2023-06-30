#include <iostream>
#include <list>

#include "simplic.h"
#include "lexer.h"

namespace Simplic::AST
{
    void Print(AST::Node tree, int indentation)
    {
        for (int i = 0; i < indentation; i++) std::cout << "  ";
        std::cout << tree.type << " " << tree.lexeme;

        if (tree.cursorIndex != 0)
            std::cout << " @" << tree.cursorIndex;
        if (tree.prop.size() == 0 && (
            tree.type == "GENERICS LIST" ||
            tree.type == "ARGUMENTS LIST" ||
            tree.type == "POINTERS LIST"))
            std::cout << " (empty)";

        std::cout << std::endl;

        for (AST::Node node : tree.prop)
        {
            Print(node, indentation + 1);
        }
    }
}