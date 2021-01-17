#pragma once
#ifndef RENDERGRAPHUTILS_H
#define RENDERGRAPHUTILS_H

#include <string>
#include <array>
#include <iterator>
#include <vector>

namespace Fracture
{

	template<class Iter>
	inline void SplitStringIter(const std::string& s, const std::string& delim, Iter out)
	{
		if (delim.empty())
		{
			*out++ = s;
		}
		else
		{
			size_t a = 0, b = s.find(delim);
			for (; b != std::string::npos;
				a = b + delim.length(), b = s.find(delim, a))
			{
				*out++ = std::move(s.substr(a, b - a));
			}
			*out++ = std::move(s.substr(a, s.length() - a));
		}
	}

	inline std::vector<std::string> SplitString(const std::string& s, const std::string& delim)
	{
		std::vector<std::string> strings;
		SplitStringIter(s, delim, std::back_inserter(strings));
		return strings;
	}
}

#endif