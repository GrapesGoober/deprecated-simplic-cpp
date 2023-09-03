
///     This is Hollow Parser header file. This contain function declarations only useful for
///     hollow parser scripts. This is also the main location for hollow parsers documentation

#pragma once

#include <iostream>
#include <list>
#include <stack>

#include "simplic.h"
#include "tokenizer-functions.h"

namespace Simplic::FrontEnd
{
    /// <summary>
    /// Parse a namespace def and push identifiers onto the namespace scope vector.
    /// This is used by BuildHollowAST to read each namespace defs.
    /// This function assumes it's parsing namespace def and will throw compiler
    /// exception if not written in the required format.
    /// </summary>
    /// <param name="cursor"> cursor with the source code </param>
    /// <param name="node"> a node to be injected with data </param>
    void ParseNamespDef(Cursor& cursor, FrontEnd::Node& node);

    // parse a definition of a struct, including all its properties
    void ParseStructDef(Cursor& cursor, FrontEnd::Node& node);

    // parse a definition of a const block, including all its member constants
    void ParseConstDef(Cursor& cursor, FrontEnd::Node& node);

    // parse a header of a function; header only contain the return type, name, and arguments
    void ParseFuncDef(Cursor& cursor, FrontEnd::Node& node);

    // a function generics parser
    void ParseFuncGenerics(Cursor& cursor, FrontEnd::Node& node);

    // an argument list parser (into a list of argument nodes)
    // ex: (array<string> args, tuple<int,string> tag)
    void ParseArgumentList(Cursor& cursor, FrontEnd::Node& node);

    // an argument parser (into an argument node)
    // ex: some.type<genric>& ident
    void ParseArgument(Cursor& cursor, FrontEnd::Node& node);

    void SkipFuncBody(Cursor& cursor, FrontEnd::Node& node);

    // a noexcept ident-dot chain parser with or without generics (into a list of idents, append to node's prop)
    // ex: ident1.ident2.someOtherIdent < withOrWithout.Generics >
    // this is used by ParseNamespDef to read the namespace def
    // noexcept parser; it will only return false if the string does not match the required format
    bool IsIdentGroup(Cursor& cursor, FrontEnd::Node& node) noexcept;

    // a noexcept generics list parser (into a list of ident groups, append to node's prop)
    // ex: <string, int, Math.StringNumber>
    // this parser will assume that all angle brackets are for generics, and not comparison operators
    // noexcept parser; it will only return false if the string does not match the required format
    bool IsGenericsList(Cursor& cursor, FrontEnd::Node& node) noexcept;

    // assign a node to the proper location in signature tree
    void AssignSignatureToAST(FrontEnd::Node& signatures, FrontEnd::Node node, std::list<std::string> nspScope);
}