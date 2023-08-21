#pragma once
#ifndef ANIMATIONPOSE_H
#define ANIMATIONPOSE_H


#include "Rendering/Mesh.h"

namespace Fracture
{

	struct Skeleton
	{
		uint32_t NumBones;
		std::vector<Bone> Bones;
	};
}

#endif