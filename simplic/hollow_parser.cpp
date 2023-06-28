#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "lexer.h"

namespace Simplic::AST
{
    typedef std::list<std::list<std::string>> namespScope;

    // Constructs a hollow AST based solely on function signatures, struct definitions, and consts
    // This function is used by the main func as the first step of parsing
    std::list<AST::Node> BuildHollowAST(std::string src);

    // Parse a namespace def and push identifiers onto the namespace scope vector
    // This is used by BuildHollowAST to read each namespace defs
    // This function assumes it's parsing namespace def and will throw compiler
    // exception if not written in the required format
    void ParseNamespDef(Cursor& cursor, namespScope& namespScope);
    
    // parse a definition of a struct, including all its properties
    void ParseStructDef(Cursor& cursor, AST::Node& node);

    // parse a definition of a const block, including all its member constants
    void ParseConstDef(Cursor& cursor, AST::Node& node);

    // parse a signature of a function; signature only contain the identifiers, generics, and arguments
    void ParseFuncSignature(Cursor& cursor, AST::Node& node);

    // a noexcept ident-dot chain parser with or without generics (into a list of idents, append to node's prop)
    // ex: ident1.ident2.someOtherIdent < withOrWithout.Generics >
    // this is used by ParseNamespDef to read the namespace def
    // noexcept parser; it will only return false if the string does not match the required format
    bool IsIdentChain(Cursor& cursor, AST::Node& node) noexcept;

    // a noexcept generics list parser (into a list of ident groups, append to node's prop)
    // ex: <string, int, Math.StringNumber>
    // this parser will assume that all angle brackets are for generics, and not comparison operators
    // noexcept parser; it will only return false if the string does not match the required format
    bool IsGenericList(Cursor& cursor, AST::Node& node) noexcept;
    
    // a noexcept argument list parser (into a list of argument nodes)
    // ex: (array<string> args, tuple<int,string> tag)
    void ParseArgumentList(Cursor& cursor, AST::Node& node);

    // a noexcept argument parser (into an argument node)
    // ex: some.type<genric>& ident
    void ParseArgument(Cursor& cursor, AST::Node& node);





    // WORK IN PROGRESS
    std::list<AST::Node> BuildHollowAST(std::string src)
    {
        // creating a new tokenizer cursor to read the source string
        Cursor cursor{ src };
        std::list<AST::Node> signatures;
        namespScope nspScope;

        // clean out all comments and whitespaces
        Tokenize::DeepClean(cursor);

        // loop until EOF or sees end curly bracket
        while (!IsEOF(cursor))
        {
            // node to be used for injection by the parsers
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

            // parse namespace def, if exists
            else if (Tokenize::IsKeyword(cursor, LangDef::KWnamesp))
            {
                ParseNamespDef(cursor, nspScope);
            }
            // parse struct def, if exists
            else if (Tokenize::IsKeyword(cursor, LangDef::KWstruct))
            {
                ParseStructDef(cursor, node);
                signatures.push_back(node);
            }
            // parse const def, if exists
            else if (Tokenize::IsKeyword(cursor, LangDef::KWconst))
            {
                ParseConstDef(cursor, node);
                signatures.push_back(node);
            }
            // else, assume it is a function def (since there's nothing else we can parse)
            else
            {
                ParseFuncSignature(cursor, node);
                signatures.push_back(node);
            }

            // deep clean again before next iteration, so the isEOF check can work properly
            Tokenize::DeepClean(cursor);
        }

        // check that the namesp has been closed (i.e. proper curly brackets)
        if (!nspScope.empty())
        {
            // not proper brackets, throw exception
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

    // FINISHED
    void ParseNamespDef(Cursor& cursor, namespScope& nspScope)
    {
        // expect ident-dot chain   ex:   MyNamespace.SomeNamespace.AnotherNamespace
        AST::Node namespNode;
        if (IsIdentChain(cursor, namespNode))
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
            // get {, but need to deep clean first (i.e. clean multi-line) 
            Tokenize::DeepClean(cursor);
            if (!Tokenize::IsSymbol(cursor, LangDef::openCurlyBracket))
            {
                throw CmplException(cursor, "Unexpected token; expected a '{' for all namespace declarations");
            }
        }
        else throw CmplException(cursor, "Unexpected token; expected only one or more identifiers");

    }

    // UNIMPLEMENTED
    void ParseStructDef(Cursor& cursor, AST::Node& node)
    {
        throw CmplException(cursor, "struct unimplemented");
    }
    
    // UNIMPLEMENTED
    void ParseConstDef(Cursor& cursor, AST::Node& node)
    {
        throw CmplException(cursor, "const unimplemented");
    }

    // AWAITING - FOR ParseArgumentList & ParseArgument
    void ParseFuncSignature(Cursor& cursor, AST::Node &node)
    {
        node.type = "FUNC";
        node.lexeme = "";
        node.cursorIndex = 0;
        node.prop = std::list<AST::Node>{};

        // First, expect an ident group for return type
        AST::Node returnType;
        returnType.type = "RETURN TYPE";
        if (!IsIdentChain(cursor, returnType))
        {
            // if not, throw exception
            throw CmplException(cursor, "A function definition must begin with a return type.");
        }
        node.prop.push_back(returnType);

        // then, expect another ident group for function identifiers (and maybe generics and namespace)
        AST::Node name;
        if (!IsIdentChain(cursor, name))
        {
            // if not, throw exception
            throw CmplException(cursor, "A function definition must have function name.");
        }
        node.prop.push_back(name);

        // then, expect an argument list
        AST::Node argsList;
        ParseArgumentList(cursor, argsList);
        node.prop.push_back(argsList);
    }

    // FINISHED
    bool IsIdentChain(Cursor& cursor, AST::Node& node) noexcept
    {
        node.type = "IDENT GROUP";
        node.lexeme = "";
        node.cursorIndex = 0;
        node.prop = std::list<AST::Node>{};
        int initialIndex = cursor.index;
        AST::Node nsNode;

        Tokenize::Clean(cursor);

        // must begin with an ident
        if (!Tokenize::Ident(cursor, nsNode)) return false;
        node.prop.push_back(nsNode);

        // keep looping for more idents
        while (!IsEOF(cursor))
        {
            Tokenize::Clean(cursor);
            if (IsGenericList(cursor, nsNode))
            {
                node.prop.push_back(nsNode);
                break;
            }

            // as long as there are dot operators, expect more idents
            if (Tokenize::IsSymbol(cursor, LangDef::dotOp))
            {
                // but if this does not get an ident, then return false
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

    // FINISHED
    bool IsGenericList(Cursor& cursor, AST::Node& node) noexcept
    {
        node.type = "GENERICS";
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
        if (!IsIdentChain(cursor, genericNode))
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
                if (IsIdentChain(cursor, genericNode))
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

    // WORK IN PROGRESS - NEED TESTING
    void ParseArgumentList(Cursor& cursor, AST::Node& node)
    {
        node.type = "ARGSLIST";
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

    // WORK IN PROGRESS - REQUIRE TESTING
    void ParseArgument(Cursor& cursor, AST::Node& node)
    {
        node.type = "ARGUMENT";

        // first, expect value type
        AST::Node identChain;
        if (!IsIdentChain(cursor, identChain))
        {
            throw CmplException(cursor, "Unexpected token; expected identifiers for argument type.");
        }
        node.prop.push_back(identChain);

        // then, look for pointer operators and then argument name
        AST::Node pointerList;
        pointerList.type = "POINTER LIST";
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
                node.prop.push_back(pointerList);
            }
            // else, expect argument name and end the argument declaration
            else if (AST::Node name; Tokenize::Ident(cursor, name))
            {
                node.lexeme = name.lexeme;
                node.cursorIndex = name.cursorIndex;
                break;
            }
            else throw CmplException(cursor, "Unexpected token; expected asterisk * for pointer declaration");
        }
    }

}