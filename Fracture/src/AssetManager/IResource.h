#pragma once
#ifndef IRESOURCE_H
#define IRESOURCE_H

namespace Fracture {

	enum class ResourceType
	{
		MESH,
		MODEL,
		MATERIAL,
		TEXTURE,
		SHADER,
	};

	class IResource
	{

	public:
		IResource(ResourceType mtype):Resource_Type(mtype)
		{};
		ResourceType Resource_Type;
	};


}


#endif