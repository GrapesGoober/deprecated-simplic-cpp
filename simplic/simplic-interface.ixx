#include <string>
#include <deque>

// Simplic Compiler, 2022, Author: Nachat Kaewmeesang
// simplic-interface.ixx contains definitions and declarations used
// and shared amongst other simplic ixx module files
// this is a good starting point to read how this compiler works

export module simplic;

namespace Simplic
{
	export struct Cursor
	{
		std::ifstream& file;
		std::deque<char> buffer;
		Cursor(std::ifstream& ifstream) : file(ifstream) {}
	};

	export struct Node
	{
		std::string type;
		std::string lexeme;
	};
}