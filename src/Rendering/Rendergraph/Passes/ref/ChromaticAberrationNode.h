#pragma once
#ifndef ChromaticAberrationNode_H
#define ChromaticAberrationNode_H

#include "../RenderNode.h"

namespace Fracture
{
	struct ChromaticParams
	{
		bool ByPass = true;
		float redOffset = 0.0006f;
		float greenOffset = 0.0007f;
		float blueOffset = -0.0005f;
	};

	struct ChromaticAberrationNode : public RenderNode
	{
		ChromaticAberrationNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		void Setup();
		void Run(Scene& scene, CameraComponent* camera);
		
		ChromaticParams Properties;

	private:
		std::unique_ptr<Shader> shader;
	};


}

#endif