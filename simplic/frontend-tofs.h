#pragma once
#include "frontend.h"

namespace Simplic::FrontEnd::TOFs
{
    namespace Match
    {
        char Char(Cursor& cursor, char c);
        char CharSet(Cursor& cursor, std::string cset);
        bool String(Cursor& cursor, std::string str);
        std::string StringSet(Cursor& cursor, std::list<std::string> strset);
    }

    namespace Tokenize
    {
        void Clean(Cursor& cursor);
        void DeepClean(Cursor& cursor);
        bool CleanLineComment(Cursor& cursor);
        bool CleanBlockComment(Cursor& cursor);
        bool CleanSpace(Cursor& cursor);
        bool IsKeyword(Cursor& cursor, std::string keyword);
        bool IsSymbol(Cursor& cursor, std::string symbol);
        bool UnaryOp(Cursor& cursor, FrontEnd::Node& node);
        bool BinaryOp(Cursor& cursor, FrontEnd::Node& node);
        bool Literal(Cursor& cursor, FrontEnd::Node& node);
        bool Ident(Cursor& cursor, FrontEnd::Node& node);
    }
}