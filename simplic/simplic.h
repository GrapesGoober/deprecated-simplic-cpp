#pragma once

#include <iostream>
#include <list>
#include <stack>

namespace Simplic 
{
    struct Cursor
    {
        int index = 0;
        std::string source;
        Cursor(std::string src) : source(src + '\n') { }
    };

    namespace AST
    {
        // each node in the AST with any m-ary children (called property)
        struct Node
        {
            std::string type; // STRING, CHAR, IDENT, OPERATOR etc...
            std::string lexeme; // ident or literal
            std::list<Node> prop; // can be treated as subtrees or a list of values
            int cursorIndex = 0; // location of this token in the source string
        };

        AST::Node BuildHollowAST(std::string src);
        void ParseExpr(Cursor& cursor, AST::Node& node);

        // Utility function for debugging
        void Print(AST::Node tree, int indentation = 0);
    }

    // assembly stage, i.e. the final stage before the hex file
    namespace Asm
    {
        // Reads assembly and converts into machine code
        std::list<uint16_t> ReadAsm(std::list<std::string>);

        // Recieves machine code and writes to hex file
        void WriteToHexFile(std::string, std::list<uint16_t>);
    }

    struct CmplException : public std::exception 
    {
        Cursor& cursor;
        std::string message;
        CmplException(Cursor& cursor, std::string message) : cursor(cursor), message(message) {}
        const char* what() const throw ();
    };

    struct AsmException : public std::exception 
    {
        std::string message;
        AsmException(std::string message) : message(message) {}
        const char* what() const throw ();
    };
}
