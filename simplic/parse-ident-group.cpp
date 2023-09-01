#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "tokenizer-functions.h"
#include "hollow-parser.h"

namespace Simplic::AST
{
    bool IsIdentGroup(Cursor& cursor, AST::Node& node) noexcept
    {
        node.type = "IDENT GROUP";
        node.lexeme = "";
        node.cursorIndex = 0;
        node.prop = std::list<AST::Node>{};
        int initialIndex = cursor.index;
        AST::Node identNode;

        Tokenize::Clean(cursor);

        // must begin with an ident
        if (!Tokenize::Ident(cursor, identNode)) return false;
        node.prop.push_back(identNode);

        // keep looping for more idents
        while (!IsEOF(cursor))
        {
            // if found generics, break the loop and assume ident group is finished parsing
            if (IsGenericsList(cursor, identNode))
            {
                node.prop.push_back(identNode);
                break;
            }

            // as long as there are dot operators, expect more idents
            Tokenize::Clean(cursor);
            if (Tokenize::IsSymbol(cursor, LangDef::dotOp))
            {
                // but if this does not get an ident, then return false
                if (!Tokenize::Ident(cursor, identNode))
                {
                    cursor.index = initialIndex;
                    return false;
                }
                node.prop.push_back(identNode);
            }
            else break; //not a dotOp -> must have finished ident group
        }
        return true;
    }

    bool IsGenericsList(Cursor& cursor, AST::Node& node) noexcept
    {
        node.type = "GENERICS LIST";
        node.lexeme = "";
        node.cursorIndex = 0;
        node.prop = std::list<AST::Node>{};
        int initialIndex = cursor.index;

        // check for angle bracket
        Tokenize::Clean(cursor);
        if (!Tokenize::IsSymbol(cursor, LangDef::openAngBracket))
        {
            cursor.index = initialIndex;
            return false;
        }

        // first node must be ident group
        AST::Node genericNode;
        if (!IsIdentGroup(cursor, genericNode))
        {
            cursor.index = initialIndex;
            return false;
        }
        node.prop.push_back(genericNode);

        while (!IsEOF(cursor))
        {
            // as long as there are commas, expect more ident group
            Tokenize::Clean(cursor);
            if (Tokenize::IsSymbol(cursor, LangDef::comma))
            {
                if (IsIdentGroup(cursor, genericNode))
                {
                    node.prop.push_back(genericNode);
                }
                else
                {
                    cursor.index = initialIndex;
                    return false;
                }
            }
            // check for closing angle bracket
            else if (Tokenize::IsSymbol(cursor, LangDef::closeAngBracket)) break;
        }

        return true;
    }

}