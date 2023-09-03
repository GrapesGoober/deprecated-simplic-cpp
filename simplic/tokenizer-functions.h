#pragma once
#include <iostream>
#include <list>
#include <map>
#include "simplic.h"

namespace Simplic
{
    bool IsEOF(Cursor& cursor);

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
        bool IsLineComment(Cursor& cursor);
        bool IsBlockComment(Cursor& cursor);
        bool IsSpace(Cursor& cursor);
        bool IsKeyword(Cursor& cursor, std::string keyword);
        bool IsSymbol(Cursor& cursor, std::string symbol);
        bool UnaryOp(Cursor& cursor, AST::Node& node);
        bool BinaryOp(Cursor& cursor, AST::Node& node);
        bool Literal(Cursor& cursor, AST::Node& node);
        bool Ident(Cursor& cursor, AST::Node& node);
    }

    namespace LangDef
    {
        const std::string
            KWnamesp = "namesp",
            KWstruct = "struct",
            KWconst = "const",
            KWusing = "using",

            KWreturn = "return",
            KWstatic = "static",
            KWinline = "inline",

            KWif = "if",
            KWelse = "else",
            KWwhile = "while",
            KWfor = "for",

            // for hollow-AST CF parsers
            dotOp = ".",
            comma = ",",
            openAngBracket = "<",
            closeAngBracket = ">",
            openCurlyBracket = "{",
            closeCurlyBracket = "}",
            openRoundBracket = "(",
            closeRoundBracket = ")",
            pointerDeclOp = "*",

            // for expression
            openBrackets = "([{",
            closeBrackets = ")]}",
            exprEnd = "\n,)]}";

        const std::list<std::string> keywords =
        {
            KWnamesp, KWstruct, KWconst, KWusing, KWreturn, KWstatic, KWinline, 
            KWif, KWelse, KWwhile, KWfor
        };

        const std::list<std::string> unaryOp = { "-", "!", "&", "*" };
        const std::list<std::string> binaryOp =
        {
            ".",
            "*", "/", "%",
            "+", "-",
            "<<", ">>",
            ">=", "<=", ">", "<",
            "&&", "||",
            "&", "|", "^",
        };

        // here are the order of operations according to priority
        // note: using a u prefix to denote unary operators
        const std::map<std::string, int> precedence =
        {
            {"u&", 9}, {"u*", 9},
            {".", 8},
            {"u-", 7}, {"u!", 7},
            {"*", 6}, {"/", 6}, {"%", 6},
            {"+", 5}, {"-", 5},
            {"<<", 4}, {">>", 4},
            {">=", 3}, {"<=", 3}, {">", 3}, {"<", 3},
            {"==", 2}, {"!=", 2},
            {"&&", 1}, {"||", 1},
            {"&", 0}, {"|", 0}, {"^", 0},
        };
    }
}