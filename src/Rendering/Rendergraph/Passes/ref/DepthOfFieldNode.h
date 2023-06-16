#pragma once
#ifndef DOFNODE_H
#define DOFNODE_H
#include "../RenderNode.h"

namespace Fracture
{
	struct DoFParams
	{
		float BokehRadius = 4.0f;
	};

	struct DepthOfFieldNode : public RenderNode
	{
		DepthOfFieldNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		void Setup();
		void Run(Scene& scene, CameraComponent* camera);

		DoFParams Properties;
		
	private:
		std::unique_ptr<Shader> CoCshader;
		std::unique_ptr<Shader> CoCDownSample;
		std::unique_ptr<Shader> BokehShader;
		std::unique_ptr<Shader> BokehBlurShader;
		std::unique_ptr<Shader> DoFCombine;
		std::unique_ptr<Texture> mCoCTexture;
		std::unique_ptr<Texture> mCoCDownSampledTexture;
		std::unique_ptr<Texture> mBokehTexture;
		std::unique_ptr<Texture> mBokehBlurTexture;
	};


}

#endif