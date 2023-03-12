#pragma once
#ifndef IGRAPHICSRESOURCE_H
#define IGRAPHICSRESOURCE_H

namespace Fracture
{

	struct IGraphicsResource
	{
		virtual ~IGraphicsResource() = default;

	private:
		// pure virtual implementation
		virtual void bind() = 0;

	};


}

#endif