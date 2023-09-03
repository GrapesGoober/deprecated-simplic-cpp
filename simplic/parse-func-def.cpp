#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "tokenizer-functions.h"
#include "hollow-parser.h"

namespace Simplic::FrontEnd
{
    void ParseFuncDef(Cursor& cursor, FrontEnd::Node& node)
    {
        node.type = "FUNC DEF";
        node.lexeme = "";
        node.cursorIndex = 0;
        node.prop = std::list<FrontEnd::Node>{};

        // 1) expect an ident group for return type
        FrontEnd::Node returnType;
        if (!IsIdentGroup(cursor, returnType))
        {
            // if not, throw exception
            FrontEnd::CompileError(cursor, "A function definition must begin with a return type.");
        }
        node.prop.push_back(returnType);

        // 2) expect the name of the function
        Tokenize::Clean(cursor);
        if (FrontEnd::Node nameNode; Tokenize::Ident(cursor, nameNode))
        {
            // assign the name to the root of func-def tree
            node.lexeme = nameNode.lexeme;
        }
        // if not, throw exception
        else FrontEnd::CompileError(cursor, "A function definition must have function name.");

        // 3) determine whether there are generics for this func
        FrontEnd::Node genericsList;
        ParseFuncGenerics(cursor, genericsList);
        if (genericsList.prop.size() != 0)
        {
            // turn this func-def into a generic-func-def
            node.type = "GENERIC FUNC DEF";
            node.prop.push_back(genericsList);
        }

        // 4) expect an argument list
        FrontEnd::Node argsList;
        ParseArgumentList(cursor, argsList);
        node.prop.push_back(argsList);

        // WORK IN PROGRESS
        // lastly, skim the function body to get the proper function body
    }

    void ParseFuncGenerics(Cursor& cursor, FrontEnd::Node& node)
    {
        node.type = "GENERICS LIST";
        FrontEnd::Node identNode;
        Tokenize::Clean(cursor);
        if (Tokenize::IsSymbol(cursor, LangDef::openAngBracket))
        {
            while (true)
            {
                Tokenize::Clean(cursor);
                if (Tokenize::Ident(cursor, identNode))
                {
                    node.prop.push_back(identNode);
                    Tokenize::Clean(cursor);
                    if (Tokenize::IsSymbol(cursor, LangDef::closeAngBracket)) break;
                    else if (Tokenize::IsSymbol(cursor, LangDef::comma)) continue;
                    else if (IsEOF(cursor)) FrontEnd::CompileError(cursor, "Unexpected End-Of-File.");
                    else FrontEnd::CompileError(cursor, "Unexpected token");
                }
                else
                {
                    FrontEnd::CompileError(cursor, "Expect an identifier for generics.");
                }
            }
        }
    }

    void ParseArgumentList(Cursor& cursor, FrontEnd::Node& node)
    {
        node.type = "ARGUMENTS LIST";
        node.prop = std::list<FrontEnd::Node>{};

        // first, check for circle bracket
        Tokenize::Clean(cursor);
        if (!Tokenize::IsSymbol(cursor, LangDef::openRoundBracket))
        {
            FrontEnd::CompileError(cursor, "A function argument list must begin with round bracket.");
        }

        // second, check for closing bracket (for no args)
        Tokenize::Clean(cursor);
        if (Tokenize::IsSymbol(cursor, LangDef::closeRoundBracket)) return;

        // then, go into a loop to parse argslist
        while (!IsEOF(cursor))
        {
            FrontEnd::Node argnode;
            // must be an argument, else this line will throw exception
            ParseArgument(cursor, argnode);
            node.prop.push_back(argnode);

            Tokenize::Clean(cursor);
            // could either end with comma
            if (Tokenize::IsSymbol(cursor, LangDef::comma)) continue;
            // or end with a close bracket
            else if (Tokenize::IsSymbol(cursor, LangDef::closeRoundBracket)) return;
            else FrontEnd::CompileError(cursor, "Unexpected token; expected either a comma or closing bracket.");
        }
    }

    void ParseArgument(Cursor& cursor, FrontEnd::Node& node)
    {
        node.type = "ARGUMENT";

        // first, expect value type
        FrontEnd::Node identChain;
        if (!IsIdentGroup(cursor, identChain))
        {
            FrontEnd::CompileError(cursor, "Unexpected token; expected identifiers for argument type.");
        }
        node.prop.push_back(identChain);

        // then, look for pointer operators and then argument name
        FrontEnd::Node pointerList;
        pointerList.type = "POINTERS LIST";
        while (!IsEOF(cursor))
        {
            Tokenize::Clean(cursor);
            // expect optional pointer declarations
            if (Tokenize::IsSymbol(cursor, LangDef::pointerDeclOp))
            {
                FrontEnd::Node pointerNode;
                pointerNode.type = "POINTER";
                pointerNode.lexeme = "*";
                pointerNode.cursorIndex = cursor.index;
                pointerList.prop.push_back(pointerNode);
            }
            // else, expect argument name and end the argument declaration
            else if (FrontEnd::Node name; Tokenize::Ident(cursor, name))
            {
                node.lexeme = name.lexeme;
                node.cursorIndex = name.cursorIndex;
                node.prop.push_back(pointerList);
                break;
            }
            else FrontEnd::CompileError(cursor, "Unexpected token; expected asterisk * for pointer declaration");
        }
    }

    void SkipFuncBody(Cursor& cursor, FrontEnd::Node& node)
    {

    }
}