#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "lexer.h"
#include "hollow-parser.h"

namespace Simplic::AST
{
    void ParseStructDef(Cursor& cursor, AST::Node& node)
    {
        AST::CompileError(cursor, "struct unimplemented");
    }

    void ParseConstDef(Cursor& cursor, AST::Node& node)
    {
        AST::CompileError(cursor, "const unimplemented");
    }
}