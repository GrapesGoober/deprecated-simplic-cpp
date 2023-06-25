#include "simplic.h"
#include "lexer.h"

namespace Simplic
{

    bool IsEOF(Cursor& cursor)
    {
        return cursor.index >= cursor.source.size();
    }

    namespace Match
    {
        char Char(Cursor& cursor, char c)
        {
            if (cursor.source[cursor.index] == c)
            {
                cursor.index++;
                return c;
            }
            else return 0;
        }
        char CharSet(Cursor& cursor, std::string cset)
        {
            for (char c : cset)
            {
                if (char isChar = Char(cursor, c)) return isChar;
            }
            return 0;
        }
        bool String(Cursor& cursor, std::string str)
        {
            // return true only when every char in source is the same as cs
            for (int i = 0; i < str.size(); i++)
            {
                if (cursor.source[(size_t)cursor.index + i] != str.c_str()[i]) return false;
            }
            // hasSpace is used for keyword matching, where there needs whitespace after string
            cursor.index += (int)str.size();
            return true;
        }
        std::string StringSet(Cursor& cursor, std::list<std::string> strset)
        {
            for (std::string str : strset)
            {
                if (String(cursor, str)) return str;
            }
            return "";
        }
    }

    namespace Tokenize
    {
        void Clean(Cursor& cursor)
        {
            // keep cleaning until stumbles across something else
            while (true)
            {
                bool isLine = IsLineComment(cursor);
                bool isBlock = IsBlockComment(cursor);
                bool isSpace = IsSpace(cursor);
                if (!isLine && !isBlock && !isSpace) break;
            }
        }
        void DeepClean(Cursor& cursor)
        {
            while (!IsEOF(cursor))
            {
                Tokenize::Clean(cursor);
                // needs to redo (i.e. continue) the loop 
                // to clear out any potential comments and other spaces
                if (Match::Char(cursor, '\n')) continue;
                else break;
            }
        }
        bool IsLineComment(Cursor& cursor)
        {
            int initialIndex = cursor.index;

            // must begin with double slashes
            if (!Match::String(cursor, "//")) return false;

            // loop until found a newline (does not consume newline)
            while (!IsEOF(cursor))
            {
                if (Match::Char(cursor, '\n')) return true;
                else cursor.index++;
            }

            // will always find the newline char
            return false;
        }
        bool IsBlockComment(Cursor& cursor)
        {
            int initialIndex = cursor.index;

            // must begin with double slashes
            if (!Match::String(cursor, "/*")) return false;

            // loop until found a end star slash
            while (!IsEOF(cursor))
            {
                if (Match::String(cursor, "*/")) return true;
                else cursor.index++;
            }

            // the return inside the loop should fire, 
            // otherwise there are some issues with closing star-slash
            cursor.index = initialIndex;
            throw CmplException(cursor, "No closing star-slash for block comment");
        }
        bool IsSpace(Cursor& cursor)
        {
            int initialIndex = cursor.index;
            // using C's isspace for this
            while (isspace(cursor.source.c_str()[cursor.index]))
            {
                // newlines are the exception
                if (cursor.source.c_str()[cursor.index] == '\n') break;
                cursor.index++;
            }
            return cursor.index != initialIndex;
        }

        bool IsKeyword(Cursor& cursor, std::string keyword)
        {
            int initialIndex = cursor.index;
            if (Match::String(cursor, keyword))
            {
                // check if there is whitespace in the end of keyword
                if (IsSpace(cursor)) return true;
            }
            cursor.index = initialIndex;
            return false;
        }

        bool IsSymbol(Cursor& cursor, std::string symbol)
        {
            int initialIndex = cursor.index;

            if (Match::CharSet(cursor, symbol))
            {
                return true;
            }
            else
            {
                cursor.index = initialIndex;
                return false;
            }
        }
        bool UnaryOp(Cursor& cursor, AST::Node& node)
        {
            node.cursorIndex = cursor.index;

            std::string match = Match::StringSet(cursor, LangDef::unaryOp);
            if (match != "")
            {
                node.type = "OPERATOR";
                node.lexeme = "u" + match; // using a u prefix to denote unary operators
                return true;
            }
            return false;
        }
        bool BinaryOp(Cursor& cursor, AST::Node& node)
        {
            node.cursorIndex = cursor.index;

            std::string match = Match::StringSet(cursor, LangDef::binaryOp);
            if (match != "")
            {
                node.type = "OPERATOR";
                node.lexeme = match;
                return true;
            }
            return false;
        }

        // TODO: for charstring, implement "Char length can only be 1" exception, and also escaped chars
        bool CharString(Cursor& cursor, AST::Node& node)
        {
            node.cursorIndex = cursor.index;

            // raw charstring literals have r prefixed
            bool raw = Match::Char(cursor, 'r');

            // must begin with appropriate quotations
            char quotation = Match::CharSet(cursor, "'\"");
            if (quotation != '"' && quotation != '\'') return false;

            // loop until found a closing quotation
            while (!IsEOF(cursor))
            {
                // for escaped characters, just ignore and skip
                if (Match::Char(cursor, '\\'))
                {
                    cursor.index ++;
                }
                // found the right closing quotation, return sub string
                else if (Match::Char(cursor, quotation))
                {
                    if (quotation == '"') node.type = "STRING";
                    if (quotation == '\'') node.type = "CHAR";

                    int beginIndex = node.cursorIndex + raw + 1;
                    int length = cursor.index - node.cursorIndex - raw - 2;
                    node.lexeme = cursor.source.substr(beginIndex, length);

                    return true;
                }
                else cursor.index++;
            }

            // the return inside the loop should fire, 
            // otherwise there are some issues with closing quotation
            cursor.index = node.cursorIndex;
            throw CmplException(cursor, "No closing quotation for charstring literal");
        }
        // TODO: implement
        bool BinHex(Cursor& cursor, AST::Node& node)
        {
            return false;
        }
        // TODO: implement
        bool Decimals(Cursor& cursor, AST::Node& node)
        {
            return false;
        }
        bool Literal(Cursor& cursor, AST::Node& node)
        {
            if (CharString(cursor, node)) return true;
            else if (BinHex(cursor, node)) return true;
            else if (Decimals(cursor, node)) return true;
            else return false;
        }

        bool Ident(Cursor& cursor, AST::Node& node)
        {
            node.cursorIndex = cursor.index;

            // check that this is not a reserved keyword
            for (std::string s : LangDef::keywords)
            {
                // if found a keyword, return false
                if (IsKeyword(cursor, s))
                {
                    // reset the cursor index
                    cursor.index = node.cursorIndex;
                    return false;
                }
            }

            // first char allows for either alphabet or underscore
            char ch = cursor.source[cursor.index];
            if (!isalpha(ch) && ch != '_') return false;
            cursor.index++;

            // loop until found a termination condition
            while (!IsEOF(cursor))
            {
                // other than first char allows for alphanumerics and underscores
                ch = cursor.source[cursor.index];
                if (!isalnum(ch) && ch != '_')
                {
                    node.type = "IDENT";

                    int beginIndex = node.cursorIndex;
                    int length = cursor.index - node.cursorIndex;
                    node.lexeme = cursor.source.substr(beginIndex, length);

                    return true;
                }
                else cursor.index++;
            }

            // logic flow dictates it will never reach this; it's only here to suppress warning
            return false;
        }
    }
}