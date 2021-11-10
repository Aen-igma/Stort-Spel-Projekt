#include "PCH.h"
#include "Helper.h"

namespace Aen
{
	std::wstring Helper::StringToWide(std::string s)
	{
		return std::wstring(s.begin(), s.end());
	}
}

