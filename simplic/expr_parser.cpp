#include <iostream>
#include <stack>

#include "simplic.h"
#include "lexer.h"

namespace Simplic::AST
{
    // a struct to compact-ify passing arguments among parsers
    struct ExprStacks
    {
        std::stack<AST::Node> operators, operands;
    };

    void ParseExpr(Cursor& cursor, AST::Node& node);
    bool IsPrecedenceLesser(ExprStacks& stacks, AST::Node& currentOp);
    void ParseOperator(Cursor& cursor, ExprStacks& stacks);
    void ParseOperand(Cursor& cursor, ExprStacks& stacks);
    void BuildSubtree(ExprStacks& stacks);
    bool IsEndCondition(Cursor& cursor);

    // Our protagonist expression parser
    void ParseExpr(Cursor& cursor, AST::Node& node)
    {
        ExprStacks stacks;

        while (true)
        {
            // clean only spaces, comments should throw "unexpected token"
            Tokenize::IsSpace(cursor);

            // optionally expect a unary operator
            if (Tokenize::UnaryOp(cursor, node))
            {
                stacks.operators.push(node);
            }

            // operands include idents, literals, inits, and expr subtree
            ParseOperand(cursor, stacks);

            // check for END condition, if true, clear the stack
            if (IsEndCondition(cursor))
            {
                while (stacks.operands.size() != 1)
                {
                    BuildSubtree(stacks);
                }
                node = stacks.operands.top();
                return;
            }

            // operators include symbols and bracket operators
            ParseOperator(cursor, stacks);
        }
    }

    // Check for operator precedence in an expression
    bool IsPrecedenceLesser(ExprStacks& stacks, AST::Node& currentOp)
    {
        int thisPrec = LangDef::precedence.at(currentOp.lexeme);
        if (stacks.operators.empty()) return false;
        int prevPrec = LangDef::precedence.at(stacks.operators.top().lexeme);
        return thisPrec <= prevPrec;
    }

    // TODO: implement "smart" end conditions, with newlines n shit
    bool IsEndCondition(Cursor& cursor)
    {
        Tokenize::IsSpace(cursor);

        // implement other end conditions
        /*
            - newlines but also check the other line to see if expr continues
            - any comment (block or otherwise) terminates expression
        */
        return Tokenize::IsSymbol(cursor, LangDef::exprEnd);
    }

    // TODO: implement check: dot operator's right hand MUST be ident
    // TODO: dot operators are non-CF; namespace access and property access
    void BuildSubtree(ExprStacks& stacks)
    {
        AST::Node op = stacks.operators.top(); stacks.operators.pop();

        if (op.lexeme[0] != 'u') // binary operation
        {
            AST::Node right = stacks.operands.top(); stacks.operands.pop();
            AST::Node left = stacks.operands.top(); stacks.operands.pop();
            op.prop.push_back(left);
            op.prop.push_back(right);
        }
        else
        {
            AST::Node operand = stacks.operands.top(); stacks.operands.pop();
            op.prop.push_back(operand);
        }

        stacks.operands.push(op);
    }

    // TODO: implement brackets operators
    void ParseOperator(Cursor& cursor, ExprStacks& stacks)
    {
        Tokenize::IsSpace(cursor);

        AST::Node opNode;
        if (Tokenize::BinaryOp(cursor, opNode))
        {
            // check for operator doesn't exist
            if (LangDef::precedence.count(opNode.lexeme) == 0)
            {
                throw CmplException(cursor, "Unrecognized operator");
            }
            // finish evaluating higher operators first
            while (IsPrecedenceLesser(stacks, opNode))
            {
                BuildSubtree(stacks);
            }

            stacks.operators.push(opNode);

        }
        else if (Tokenize::IsSymbol(cursor, LangDef::openBrackets))
        {
            // bracket-operators, such as calls, inits, indexing or generics (called ingen)
            // call bracket parser? ParseBracketOperator
            throw CmplException(cursor, "Unimplemented");
        }
        else throw CmplException(cursor, "Unexpected token; expected either a binary or bracket operator");
    }

    // TODO: implement brackets i.e. nested expr, argslist & ingen
    void ParseOperand(Cursor& cursor, ExprStacks& stacks)
    {
        Tokenize::IsSpace(cursor);

        AST::Node operand;

        // literals and idents can be used as a node without hassle
        if (Tokenize::Literal(cursor, operand))
        {
            stacks.operands.push(operand);
        }
        else if (Tokenize::Ident(cursor, operand))
        {
            stacks.operands.push(operand);
        }
        // open brackets can either mean expression subtree or struct init.
        else if (Tokenize::IsSymbol(cursor, LangDef::openBrackets))
        {
            // i.e. check for bracket type, parse accordingly, and check for closing bracket
            throw CmplException(cursor, "Unimplemented");
        }
        else throw CmplException(cursor, "Unexpected Token; expected literal, identifier, or open bracket");
    }
}