#pragma once
#ifndef VERTEX_H
#define VERTEX_H

namespace Fracture
{

	struct Vertex
	{
		Vertex();
		glm::vec3 Position = glm::vec3(0);
		glm::vec3 Normal = glm::vec3(0);
		glm::vec2 Uvs = glm::vec2(0);
	};

	struct SkinnedVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 Uvs;
	

		int IDs[4] = { - 1,-1,-1,-1};
		float Weights[4] = { 0.0f,0.0f, 0.0f, 0.0f };

		void AddBoneData(int BoneID, float Weight)
		{
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (Weights[i] == 0)
				{
					Weights[i] = Weight;
					IDs[i] = BoneID;
					return;
				}
			}
		}
	};
}


#endif