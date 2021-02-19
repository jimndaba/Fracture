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
		int ID ;
		std::string Name = std::string{};
		glm::mat4 BoneOffset = glm::mat4(1.0f);
		glm::mat4 LocalTransformation = glm::mat4(1.0f);
		glm::mat4 GlobalTransformation = glm::mat4(1.0f);
		glm::mat4 FinalTransformation = glm::mat4(1.0f);
	};

	struct Bone
	{		
		int ID = 0;
		std::string Name = std::string{};
		glm::mat4 BoneOffset = glm::mat4(1.0f);
		glm::mat4 LocalTransformation = glm::mat4(1.0f);
		glm::mat4 WorldTransformation = glm::mat4(1.0f);
		glm::mat4 FinalTransformation = glm::mat4(1.0f);

		std::vector<std::shared_ptr<Bone>> m_Children;

		void AddChild(std::shared_ptr<Bone> bone)
		{
			m_Children.push_back(bone);
		}
	};


	class Skeleton
	{
	public:
		// Number of bones in the skeleton
		Skeleton()
		{
			m_Root = std::make_shared<Bone>();
			m_Root->Name = "Root";
		}
		int NumBones = 0; 
		int BoneIndex = 0;
		//Bone Name, Bone ID
		std::map<std::string, uint32_t> m_BoneMapping; 

		std::shared_ptr<Bone> m_Root;

		// Bone offset Matricies
		std::vector<BoneInfo> m_BoneInfo;
		glm::mat4 m_InverseTransform = glm::mat4(1.0f);

		// Bone parent index, or -1 if root
		std::vector<int> BoneParents;

		void AddBone(std::shared_ptr<Bone> bone)
		{
			m_Root->AddChild(bone);
		}
		
	};

}

#endif