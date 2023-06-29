#include <iostream>
#include <list>

#include "simplic.h"
#include "lexer.h"

namespace Simplic::AST
{
    void Print(AST::Node tree, int indentation)
    {
        for (AST::Node node : tree.prop)
        {
            for (int i = 0; i < indentation; i++) std::cout << "  ";
            std::cout << node.type << " " << node.lexeme;

            if (node.cursorIndex != 0)
                std::cout << " @" << node.cursorIndex;
            if (node.prop.size() == 0 && (
                node.type == "GENERICS LIST" ||
                node.type == "ARGUMENTS LIST" ||
                node.type == "POINTERS LIST"))
                std::cout << " (empty)";
            
            std::cout << std::endl;

            Print(node, indentation + 1);
        }
    }
}