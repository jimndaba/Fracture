#pragma once
#ifndef UILINK_H
#define UILINK_H

#include <string>

namespace Fracture
{

	class UILink
	{
	public:
		UILink() {};
		std::string From;
		std::string To;
		std::string ToSource;
		std::string FromResource;
	};


}

#endif