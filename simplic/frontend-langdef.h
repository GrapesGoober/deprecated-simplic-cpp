// Simplic Compiler 2022, Author: Nachat K
// frontend-langdef.h contains the constants for TOFs and parsing functions

#pragma once
#include <iostream>
#include <list>
#include <map>

namespace Simplic::LangDef
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

    const std::list<std::string> unaryOp = { "-", "!" };
    const std::list<std::string> binaryOp =
    {
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