#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "lexer.h"
#include "hollow-parser.h"

namespace Simplic::AST
{
    void ParseFuncDef(Cursor& cursor, AST::Node& node)
    {
        node.type = "FUNC DEF";
        node.lexeme = "";
        node.cursorIndex = 0;
        node.prop = std::list<AST::Node>{};

        // First, expect an ident group for return type
        AST::Node returnType;
        if (!IsIdentGroup(cursor, returnType))
        {
            // if not, throw exception
            throw CmplException(cursor, "A function definition must begin with a return type.");
        }
        node.prop.push_back(returnType);

        // second, expect another ident group for function name, and maybe generics
        ParseFuncName(cursor, node);

        // third, expect an argument list
        AST::Node argsList;
        ParseArgumentList(cursor, argsList);
        node.prop.push_back(argsList);

        // WORK IN PROGRESS
        // lastly, skim the function body to get the proper function body
    }

    void ParseFuncName(Cursor& cursor, AST::Node& node)
    {
        AST::Node nameNode; // this node is just only used for handling info, not to construct AST
        if (!IsIdentGroup(cursor, nameNode))
        {
            // if not, throw exception
            throw CmplException(cursor, "A function definition must have function name.");
        }

        // check that we only have one identifier for our name
        bool hasGenerics = nameNode.prop.back().type == "GENERICS LIST";
        if (nameNode.prop.size() - hasGenerics > 1)
        {
            throw CmplException(cursor, "A function definition does not allow nested namespace before function name");
        }

        // assign the name to FUNC DEF lexeme
        node.lexeme = nameNode.prop.front().lexeme;

        // would always have a generics subtree node, even if it's empty
        AST::Node genericsNode;
        genericsNode.type = "GENERICS LIST";
        if (hasGenerics)
        {
            genericsNode = nameNode.prop.back();
        }
        node.prop.push_back(genericsNode);
    }

    void ParseArgumentList(Cursor& cursor, AST::Node& node)
    {
        node.type = "ARGUMENTS LIST";
        node.prop = std::list<AST::Node>{};

        // first, check for circle bracket
        Tokenize::Clean(cursor);
        if (!Tokenize::IsSymbol(cursor, LangDef::openRoundBracket))
        {
            throw CmplException(cursor, "A function argument list must begin with round bracket.");
        }

        // second, check for closing bracket (for no args)
        Tokenize::Clean(cursor);
        if (Tokenize::IsSymbol(cursor, LangDef::closeRoundBracket)) return;

        // then, go into a loop to parse argslist
        while (!IsEOF(cursor))
        {
            AST::Node argnode;
            // must be an argument, else this line will throw exception
            ParseArgument(cursor, argnode);
            node.prop.push_back(argnode);

            Tokenize::Clean(cursor);
            // could either end with comma
            if (Tokenize::IsSymbol(cursor, LangDef::comma)) continue;
            // or end with a close bracket
            else if (Tokenize::IsSymbol(cursor, LangDef::closeRoundBracket)) return;
            else throw CmplException(cursor, "Unexpected token; expected either a comma or closing bracket.");
        }
    }

    void ParseArgument(Cursor& cursor, AST::Node& node)
    {
        node.type = "ARGUMENT";

        // first, expect value type
        AST::Node identChain;
        if (!IsIdentGroup(cursor, identChain))
        {
            throw CmplException(cursor, "Unexpected token; expected identifiers for argument type.");
        }
        node.prop.push_back(identChain);

        // then, look for pointer operators and then argument name
        AST::Node pointerList;
        pointerList.type = "POINTERS LIST";
        while (!IsEOF(cursor))
        {
            Tokenize::Clean(cursor);
            // expect optional pointer declarations
            if (Tokenize::IsSymbol(cursor, LangDef::pointerDeclOp))
            {
                AST::Node pointerNode;
                pointerNode.type = "POINTER";
                pointerNode.lexeme = "*";
                pointerNode.cursorIndex = cursor.index;
                pointerList.prop.push_back(pointerNode);
            }
            // else, expect argument name and end the argument declaration
            else if (AST::Node name; Tokenize::Ident(cursor, name))
            {
                node.lexeme = name.lexeme;
                node.cursorIndex = name.cursorIndex;
                node.prop.push_back(pointerList);
                break;
            }
            else throw CmplException(cursor, "Unexpected token; expected asterisk * for pointer declaration");
        }
    }

    void SkipFuncBody(Cursor& cursor, AST::Node& node)
    {

    }
}