#pragma once
#ifndef CLRRENDERTARGET_H
#define CLRRENDERTARGET_H
#include "../RenderNode.h"

namespace Fracture
{
	struct ClearRenderTargetsNode : public RenderNode
	{
		ClearRenderTargetsNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		void Setup();
		void Run(Scene& scene, CameraComponent* camera);
	};
}

#endif