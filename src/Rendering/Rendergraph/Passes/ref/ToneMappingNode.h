#pragma once
#ifndef TONEMAPPINGNODE_H
#define TONEMAPPINGNODE_H

#include "../RenderNode.h"


namespace Fracture
{
	struct ToneMappingParams
	{
		float Exposure = 1.0f;
		float Gamma = 2.2f;
		float Contrast = 1.0f;
		float Brightness = 0.0f;
		int ToneMapMode = 0;
	};

	enum class ToneMappingMode
	{
		HDR,
		Uncharted2v1,
		Uncharted2v2,
		Filmic,
		ACES,
		Reinhard
	};


	struct ToneMappingNode :public RenderNode
	{
		ToneMappingNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		void Setup();
		void Run(Scene& scene, CameraComponent* camera);

		ToneMappingParams Properties;
		ToneMappingMode mode = ToneMappingMode::Reinhard;

	private:
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Buffer> mParams;
	};



}

#endif