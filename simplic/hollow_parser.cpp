#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "lexer.h"

namespace Simplic::AST
{
    typedef std::list<std::list<std::string>> namespScope;

    /// <summary> 
    /// <para> WORK IN PROGRESS </para>
    /// Constructs a partially built syntax tree, called "hollow AST", based solely
    /// on function headers, structs, and consts. This BuildHollowAST is used by the function 
    /// main() as the first step of parsing.
    /// </summary>
    /// 
    /// <param name='src'> source code, in string </param>
    /// <returns> a partially built tree, called "hollow AST" </returns>
    /// <seealso cref="AST::Node"/>
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

    // parse a header of a function; header only contain the return type, name, and arguments
    void ParseFuncHeader(Cursor& cursor, AST::Node& node);

    // a noexcept ident-dot chain parser with or without generics (into a list of idents, append to node's prop)
    // ex: ident1.ident2.someOtherIdent < withOrWithout.Generics >
    // this is used by ParseNamespDef to read the namespace def
    // noexcept parser; it will only return false if the string does not match the required format
    bool IsIdentGroup(Cursor& cursor, AST::Node& node) noexcept;

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
        // list of nodes, this will be our hollow AST containing the signatures
        std::list<AST::Node> signatures;
        // list of strings, to keep track of namespace scope
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
                ParseFuncHeader(cursor, node);
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
        if (IsIdentGroup(cursor, namespNode))
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

    // WORK IN PROGRESS
    void ParseFuncHeader(Cursor& cursor, AST::Node &node)
    {
        node.type = "FUNC DECL";
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
        AST::Node nameNode; // this node is just only used for handling info, not to construct AST
        if (!IsIdentGroup(cursor, nameNode))
        {
            // if not, throw exception
            throw CmplException(cursor, "A function definition must have function name.");
        }

        // check that we only have one identifier for our name
        bool hasGenerics = nameNode.prop.back().type == "GENERICS";
        if (nameNode.prop.size() - hasGenerics > 1)
        {
            throw CmplException(cursor, "A function definition does not allow nested namespace before function name");
        }

        // assign the name to FUNC DEF lexeme
        node.lexeme = nameNode.prop.front().lexeme;

        // would always have a generics subtree node, even if it's empty
        AST::Node genericsNode;
        genericsNode.type = "GENERICS";
        if (hasGenerics)
        {
            genericsNode = nameNode.prop.back();
        }
        node.prop.push_back(genericsNode);

        // third, expect an argument list
        AST::Node argsList;
        ParseArgumentList(cursor, argsList);
        node.prop.push_back(argsList);

        // WORK IN PROGRESS
        // lastly, skim the function body to get the proper function body
    }

    // FINISHED
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
            if (IsGenericList(cursor, identNode))
            {
                node.prop.push_back(identNode);
                return true;
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
            else return true; //not a dotOp -> must have finished ident group
        }
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
        if (!IsIdentGroup(cursor, identChain))
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