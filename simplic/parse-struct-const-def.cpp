#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "tokenizer-functions.h"
#include "hollow-parser.h"

namespace Simplic::FrontEnd
{
    void ParseStructDef(Cursor& cursor, FrontEnd::Node& node)
    {
        FrontEnd::CompileError(cursor, "struct unimplemented");
    }

    void ParseConstDef(Cursor& cursor, FrontEnd::Node& node)
    {
        FrontEnd::CompileError(cursor, "const unimplemented");
    }
}