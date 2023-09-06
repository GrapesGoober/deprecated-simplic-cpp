#include <iostream>
#include <fstream>
#include <list>

import simplic;
export module simplic.tofs;

namespace Simplic::TOFs
{
	// cursor's buffer size limitation
	// this value can be anything, 1000 just seems convenient
    const size_t bufsize = 1000;

	export bool GetChar(Cursor& cursor, char& ch)
	{
        if (cursor.file.get(ch))
		{
            cursor.buffer.push_back(ch);
            if (cursor.buffer.size() > bufsize) cursor.buffer.pop_front();
			return true;
		}
		else
		{
			return false;
		}

	}
}