#include <iostream>
#include <list>

#include "simplic.h"
#include "lexer.h"

namespace Simplic::AST
{
    void Print(std::list<AST::Node> tree, int indentation)
    {
        for (AST::Node node : tree)
        {
            for (int i = 0; i < indentation; i++) std::cout << "  ";
            // if you wanna print cursorindex too:  << " (" << node.cursorIndex << ")" 
            if (node.cursorIndex != 0)
                std::cout << node.type << " " << node.lexeme << " @" << node.cursorIndex << std::endl;
            else
                std::cout << node.type << " " << node.lexeme << std::endl;

            Print(node.prop, indentation + 1);
        }
    }
}