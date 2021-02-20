#pragma once
#ifndef SKELETONMESH_H
#define SKELETONMESH_H

#include "Mesh.h"

namespace Fracture
{

	class SkeletonMesh : public Mesh
	{
	public:
		SkeletonMesh();
		~SkeletonMesh() = default;

		uint32_t RenderID();

	private:
		uint32_t m_RenderID = 0;

	};


}

#endif