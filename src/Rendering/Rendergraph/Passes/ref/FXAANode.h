#pragma once
#ifndef FXAANODE_H
#define FXAANODE_H
#include "../RenderNode.h"

namespace Fracture
{
	struct FxaaParams
	{
		float MaxSpans = 16.0f;
	};

	struct FXAANode : public RenderNode
	{
		FXAANode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		void Setup();
		void Run(Scene& scene, CameraComponent* camera);

		FxaaParams Properties;

	private:
		std::unique_ptr<Shader> shader;
	};


}

#endif