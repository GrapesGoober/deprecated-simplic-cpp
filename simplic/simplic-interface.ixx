// Simplic Compiler, 2022, Author: Nachat Kaewmeesang
// simplic-interface.ixx contains definitions and declarations used
// and shared amongst other simplic ixx module files
// this is a good starting point to read how this compiler works

#include <string>
#include <deque>
#include <list>

export module simplic;

namespace Simplic
{
	/// <summary>
	/// Cursor encapsulates the information needed by the TOFs to read the file. 
	/// This contains the ifstream, a buffer, and indices used by TOFs to read from buffer
	/// </summary>
	export struct Cursor
	{
		std::ifstream& file;
		std::deque<char> buffer;
		std::size_t bufferIndex;
		std::size_t fileIndex;

		Cursor(std::ifstream& ifstream) : file(ifstream) {}
	};

	/// <summary>
	/// Node encapsulates the information output from some TOFs, 
	/// and is used as building block of the AST
	/// </summary>
	export struct Node
	{
		std::string type;
		std::string lexeme;
		std::list<Node> children;
	};
}