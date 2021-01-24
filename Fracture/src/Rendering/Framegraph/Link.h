#pragma once
#ifndef LINK_H
#define LINK_H

#include <string>

namespace Fracture
{

	class Link
	{

	public:
		Link();

		std::string FrameNode_From;
		std::string FrameNode_To;
		std::string FrameNode_Source;
		std::string FrameNode_Resource;
	};


}
#endif