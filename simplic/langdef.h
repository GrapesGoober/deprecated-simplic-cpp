#pragma once
#include <iostream>
#include <vector>
#include <map>

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

        // for expression
        openBrackets = "([{",
        closeBrackets = ")]}",
        exprEnd = "\n,)]}";

    const std::vector<std::string> unaryOp = { "-", "!", "&", "*" };
    const std::vector<std::string> binaryOp =
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