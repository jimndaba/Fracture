#pragma once
#ifndef ZPREPASS_H
#define ZPREPASS_H

#include "../RenderNode.h"

namespace Fracture
{
	struct ZPrePassNode : public RenderNode
	{
		ZPrePassNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		void Setup();
		void Run(Scene& scene, CameraComponent* camera);

	private:
		std::unique_ptr<Shader> shader;
	};

}

#endif