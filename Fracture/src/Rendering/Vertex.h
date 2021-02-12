#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

namespace Fracture
{

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 Uvs;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct AnimatedVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 Uvs;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;

		uint32_t IDs[4] = {0,0,0,0};
		float Weights[4] = {0.0f,0.0f,0.0f,0.0f};

		void AddBoneData(uint32_t BoneID, float Weight)
		{
			for (size_t i = 0; i < 4; i++)
			{
				if (Weights[i] == 0.0)
				{
					IDs[i] = BoneID;
					Weights[i] = Weight;
					return;
				}
			}
		}
	};

}

#endif
