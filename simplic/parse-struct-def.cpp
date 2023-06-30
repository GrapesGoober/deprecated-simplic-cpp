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
        throw CmplException(cursor, "struct unimplemented");
    }
}