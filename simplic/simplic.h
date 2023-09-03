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

    namespace FrontEnd
    {
        // each node in the AST with any m-ary children (called property)
        struct Node
        {
            std::string type; // STRING, CHAR, IDENT, OPERATOR etc...
            std::string lexeme; // ident or literal
            std::list<Node> prop; // can be treated as subtrees or a list of values
            int cursorIndex = 0; // location of this token in the source string
        };

        /// <summary> 
        /// <para> WORK IN PROGRESS </para>
        /// Constructs a partially built syntax tree, called "hollow AST", based solely
        /// on function headers, structs, and consts. This BuildHollowAST is used by the function 
        /// main() as the first step of parsing.
        /// </summary>
        /// 
        /// <param name='src'> source code, in string </param>
        /// <returns> a partially built tree, called "hollow AST" </returns>
        /// <seealso cref="FrontEnd::Node"/>
        FrontEnd::Node BuildHollowAST(std::string src);

        // Utility function for debugging
        void Print(FrontEnd::Node tree, int indentation = 0);

        // If there are issues with the code
        void CompileError(Cursor cursor, std::string message);
    }

    // assembly stage, i.e. the final stage before the hex file
    namespace Asm
    {
        // Reads assembly and converts into machine code
        std::list<uint16_t> ReadAsm(std::list<std::string>);

        // Recieves machine code and writes to hex file
        void WriteToHexFile(std::string, std::list<uint16_t>);
    }
}
