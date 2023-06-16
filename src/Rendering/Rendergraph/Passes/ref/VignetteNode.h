#pragma once
#ifndef VIGNETTE_H
#define VIGNETTE_H

#include "../RenderNode.h"

namespace Fracture
{

	struct VignetteParams
	{
		float Inner = 0.45f;
		float Outer = 0.38f;
		float Roundness = 0.7f;
		float Smoothness = 0.5f;
	};

	struct VignetteNode : public RenderNode
	{
		VignetteNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		void Setup();
		void Run(Scene& scene, CameraComponent* camera);

		VignetteParams Properties;

	private:
		std::unique_ptr<Shader> shader;
	};

}


#endif

