#pragma once
#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include <string>
#include <map>
#include <memory>

#include <glm/glm.hpp>

namespace Fracture
{

	struct BoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
	};

	class Skeleton
	{
	public:
		// Number of bones in the skeleton
		int NumBones; 

		//Bone Name, Bone ID
		std::map <std::string, uint32_t> m_BoneMapping; 

		// Bone offset Matricies
		std::vector<BoneInfo> m_BoneInfo;

		// Bone parent index, or -1 if root
		std::vector<int> BoneParents;
		
	};

}

#endif