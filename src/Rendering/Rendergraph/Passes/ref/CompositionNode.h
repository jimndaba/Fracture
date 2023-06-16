#pragma once
#ifndef COMPOSITIONNODE_H
#define COMPOSITIONNODE_H

#include "../RenderNode.h"

namespace Fracture
{
	struct CompositionParams
	{
		float Opacity = 1.0f;
		int MixMode = 0;
		glm::vec2 _pad = glm::vec2(0);
	};

	enum class MixMode
	{
		Add,
		Multiply,
		Subtract,
		Divide,
		Darken,
		Lighten,
		Dodge,
		Burn,

	};

	struct CompositionNode : public RenderNode
	{
		CompositionNode(DeviceContext& context,const  std::string& name,
			const std::vector<std::string >& inputs,
			const std::vector<std::string>& outputs, MixMode mode = MixMode::Multiply);

		void Setup();
		void Run(Scene& scene, CameraComponent* camera);

		CompositionParams Properties;
		MixMode Mode;
	private:
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Buffer> mParams;


	};


}


#endif