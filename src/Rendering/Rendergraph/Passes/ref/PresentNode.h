#pragma once
#ifndef PRESENTNODE_H
#define PRESENTNODE_H

#include "../RenderNode.h"


namespace Fracture
{

	struct PresentNode : public RenderNode
	{
		PresentNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		void Setup();
		void Run(Scene& scene, CameraComponent* camera);

	private:
		std::unique_ptr<Shader> shader;
	};

}

#endif