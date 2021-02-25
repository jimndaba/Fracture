#pragma once
#ifndef LINK_H
#define LINK_H

#include <string>
#include "Serialisation/FrameGraphSerialisation/FrameNodeSerialiser.h"

namespace Fracture
{
	class FrameNodeSerialiser;
	class Link
	{

	public:
		Link();
		std::string FrameNode_From;
		std::string FrameNode_To;
		std::string FrameNode_Source;
		std::string FrameNode_Resource;

		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);
	};


}
#endif