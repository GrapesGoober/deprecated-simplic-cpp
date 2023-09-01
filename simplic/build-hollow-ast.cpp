#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "lexer.h"
#include "hollow-parser.h"

namespace Simplic::AST
{
    AST::Node BuildHollowAST(std::string src)
    {
        // creating a new tokenizer cursor to read the source string
        Cursor cursor{ src };
        // root node for hollow AST, containing the signatures
        AST::Node hollowAST;
        hollowAST.type = "ROOT";
        // node to be used for injection by the parsers
        AST::Node node;

        // list of strings, to keep track of namespace scope
        std::list<std::string> nspScope;
        // a counter stack to know how many namespace elements to pop backwards
        std::stack<int> nspblockSize;

        // clean out all comments and whitespaces
        Tokenize::DeepClean(cursor);

        // loop until EOF or sees end curly bracket
        while (!IsEOF(cursor))
        {

            // checking for brackets, to make sure everything's accounted for
            if (Tokenize::IsSymbol(cursor, LangDef::closeCurlyBracket))
            {
                if (!nspScope.empty())
                {
                    for (int i = 0; i < nspblockSize.top(); i++) nspScope.pop_back();
                    nspblockSize.pop();
                }
                else throw CmplException(cursor, "Unexpected token; all curly brackets '}' are accounted for");
            }

            // parse namespace def, if exists
            else if (Tokenize::IsKeyword(cursor, LangDef::KWnamesp))
            {
                ParseNamespDef(cursor, node);

                // first, keep track the size of this namespace block
                nspblockSize.push((int)node.prop.size());

                // then loop through each item to push to namespace stack
                for (AST::Node n : node.prop)
                {
                    nspScope.push_back(n.lexeme);
                }
            }
            // parse struct def, if exists
            else if (Tokenize::IsKeyword(cursor, LangDef::KWstruct))
            {
                ParseStructDef(cursor, node);
                AssignSignatureToAST(hollowAST, node, nspScope);
            }
            // parse const def, if exists
            else if (Tokenize::IsKeyword(cursor, LangDef::KWconst))
            {
                ParseConstDef(cursor, node);
                AssignSignatureToAST(hollowAST, node, nspScope);
            }
            // else, assume it is a function def (since there's nothing else we can parse)
            else
            {
                ParseFuncDef(cursor, node);
                AssignSignatureToAST(hollowAST, node, nspScope);
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

            for (std::string nsp : nspScope)
                leftoverNsp += nsp + ".";
            // just to prettify, remove the last dot
            leftoverNsp.pop_back();

            // then, throw this out
            throw CmplException(cursor, "The namespace '" + leftoverNsp + "' has not been closed; check your '}' brackets.");
        }

        return hollowAST;
    }

    void ParseNamespDef(Cursor& cursor, AST::Node& node)
    {
        if (IsIdentGroup(cursor, node))
        {
            // check that no generics are found
            if (node.prop.back().lexeme == "GENERICS LIST")
            {
                throw CmplException(cursor, "Unexpected token; no generics allowed for namespace declarations");
            }
            // get {, but need to deep clean first (i.e. clean multi-line) 
            Tokenize::DeepClean(cursor);
            if (!Tokenize::IsSymbol(cursor, LangDef::openCurlyBracket))
            {
                throw CmplException(cursor, "Unexpected token; expected a '{' for all namespace declarations");
            }
        }
        else throw CmplException(cursor, "Unexpected token; expected only one or more identifiers");

    }

    void AssignSignatureToAST(AST::Node& hollowAST, AST::Node node, std::list<std::string> nspScope)
    {
        // initial point to start search = root of tree
        AST::Node* currentNode = &hollowAST;
        // looping through each namespace string
        for (std::string namesp : nspScope)
        {
            bool nodeFound = false;
            // loop through each of node's prop to find the right subtree
            for (AST::Node& n : (*currentNode).prop)
            {
                if (n.type == "NAMESP" && n.lexeme == namesp)
                {
                    // subtree found; set the current node to this node, and end inner loop
                    currentNode = &n;
                    nodeFound = true;
                    break;
                }
            }
            // if it reaches this point, it means that the target namespace node is not found
            if (nodeFound == false)
            {
                // counstruct a new namespace node and append it to current node
                AST::Node newNode;
                newNode.type = "NAMESP";
                newNode.lexeme = namesp;
                (*currentNode).prop.push_back(newNode);
                currentNode = &(*currentNode).prop.back();
            }

        }
        // when it reaches this point, it means that we have the proper subtree
        (*currentNode).prop.push_back(node);
    }
}