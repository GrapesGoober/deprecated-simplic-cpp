#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "lexer.h"

namespace Simplic::AST
{
    typedef std::list<std::list<std::string>> namespScope;
    std::list<AST::Node> BuildHollowAST(std::string src);

    void ParseNamespDef(Cursor& cursor, namespScope& namespScope);
    /*
    void ParseStructSig(Cursor& cursor, AST::Node node);
    void ParseConstSig(Cursor& cursor, AST::Node node);
    bool ParseSignature(Cursor& cursor, AST::Node& node) noexcept;*/

    bool ParseNamespAccess(Cursor& cursor, AST::Node& node) noexcept;
    bool ParseGenerics(Cursor& cursor, AST::Node& node) noexcept;

    // constructs a hollow AST based solely on function signatures, struct definitions, and consts
    std::list<AST::Node> BuildHollowAST(std::string src)
    {
        // creating a new tokenizer cursor to read the source string
        Cursor cursor{ src };
        std::list<AST::Node> signatures;
        namespScope nspScope;

        // loop until EOF or sees end curly bracket
        while (!IsEOF(cursor))
        {
            // clean out all comments and whitespaces
            Tokenize::DeepClean(cursor);

            AST::Node node;

            // checking for brackets, to make sure everything's accounted for
            if (Tokenize::IsSymbol(cursor, LangDef::closeCurlyBracket))
            {
                if (!nspScope.empty())
                {
                    nspScope.pop_back();
                }
                else throw CmplException(cursor, "Unexpected token; all curly brackets '}' are accounted for");
            }

            // parse namespace definition, if exists
            else if (Tokenize::IsKeyword(cursor, LangDef::KWnamesp))
            {
                ParseNamespDef(cursor, nspScope);
                continue;
            }
            /*
            else if (Tokenize::IsKeyword(cursor, LangDef::KWstruct))
            {
                ParseStructSig(cursor, node);
                continue;
            }
            else if (Tokenize::IsKeyword(cursor, LangDef::KWconst))
            {
                ParseConstSig(cursor, node);
                continue;
            }*/


            // Expression parser to check if it still works
            // ParseExpr(cursor, node);
        }

        // check that the namesp has been closed (i.e. proper curly brackets)
        if (!nspScope.empty())
        {
            // to properly printout the error, list out the contents of the namesp
            std::string leftoverNsp;

            for (std::list<std::string> level : nspScope)
                for (std::string nsp : level)
                    leftoverNsp += nsp + ".";
            // just to prettify, remove the last dot
            leftoverNsp.pop_back();

            // then, throw this out
            throw CmplException(cursor, "The namespace '" + leftoverNsp + "' has not been closed; check your '}' brackets.");
        }

        return signatures;
    }

    // parse a namespace def and push identifiers onto the namespace scope vector
    // this is by BuildHollowAST to read each namespace defs
    void ParseNamespDef(Cursor& cursor, namespScope& nspScope)
    {
        // expect ident-dot chain   ex:   MyNamespace.SomeNamespace.AnotherNamespace
        AST::Node namespNode;
        if (ParseNamespAccess(cursor, namespNode))
        {
            // check that no generics are found
            if (namespNode.prop.back().lexeme == "GENERICS")
            {
                throw CmplException(cursor, "Unexpected token; no generics allowed for namespace declarations");
            }
            // then loop through each item to push to namespace stack
            std::list<std::string> namesps;
            for (AST::Node n : namespNode.prop)
            {
                namesps.push_back(n.lexeme);
            }
            nspScope.push_back(namesps);
            // get {
            if (!Tokenize::IsSymbol(cursor, LangDef::openCurlyBracket))
            {
                throw CmplException(cursor, "Unexpected token; expected a '{' for all namespace declarations");
            }
        }
        else throw CmplException(cursor, "Unexpected token; expected only one or more identifiers");

    }

    // ident-dot chain parser with or without generics (into a list of idents, append to node's prop)
    // an ident-dot chain could be generics, functions, structs, anything
    // this is used by ParseNamespDef to read the namespace def
    bool ParseNamespAccess(Cursor& cursor, AST::Node& node) noexcept
    {
        node.type = "NS ACCESS";
        node.prop = std::list<AST::Node>{};
        int initialIndex = cursor.index;
        AST::Node nsNode;

        Tokenize::Clean(cursor);

        // must begin with an ident
        if (!Tokenize::Ident(cursor, nsNode)) return false;
        node.prop.push_back(nsNode);

        while (!IsEOF(cursor))
        {
            Tokenize::Clean(cursor);

            if (ParseGenerics(cursor, nsNode))
            {
                node.prop.push_back(nsNode);
                break;
            }

            // as long as there are dot operators, expect more idents
            if (Tokenize::IsSymbol(cursor, LangDef::dotOp))
            {
                if (!Tokenize::Ident(cursor, nsNode))
                {
                    cursor.index = initialIndex;
                    return false;
                }
                node.prop.push_back(nsNode);
            }
            else break; //not a dotOp -> must have finished namespace
        }

        return true;
    }
    
    // generics chain parser (into a list of namespace accesses, append to node's prop)
    bool ParseGenerics(Cursor& cursor, AST::Node& node) noexcept
    {
        node.type = "GENERICS";
        int initialIndex = cursor.index;

        Tokenize::Clean(cursor);

        // check for angle bracket
        if (!Tokenize::IsSymbol(cursor, LangDef::openAngBracket)) return false;

        Tokenize::Clean(cursor);

        // first node must be ns-access
        AST::Node genericNode;
        if (!ParseNamespAccess(cursor, genericNode))
        {
            cursor.index = initialIndex;
            return false;
        }
        node.prop.push_back(genericNode);

        while (!IsEOF(cursor))
        {
            Tokenize::Clean(cursor);

            // as long as there are commas, expect more ns-access
            if (Tokenize::IsSymbol(cursor, LangDef::comma))
            {
                if (ParseNamespAccess(cursor, genericNode))
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