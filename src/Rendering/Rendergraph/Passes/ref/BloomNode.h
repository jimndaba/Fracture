#pragma once
#ifndef BLOOMNODE_H
#define BLOOMNODE_H

#include "../RenderNode.h"


namespace Fracture
{
	struct BloomMipMap
	{
		glm::vec2 size;
		glm::ivec2 isize;
		std::unique_ptr<Texture>  texture;
	};

	struct BloomParams
	{
		bool UseBrightPass = true;
		float FilterRadius = 0.005f;
		float bloomStrength = 0.04f;
		float DirtStrength = 20.0f;
		float Threshold = 0.8f;
		float EmissionStrength = 0.0f;
	};

	struct BloomNode : public RenderNode
	{

		BloomNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		void Setup();
		void Run(Scene& scene, CameraComponent* camera);
		void ReloadShader();
		const std::vector<std::shared_ptr<Fracture::BloomMipMap>>& MipChain() const;

		BloomParams Properties;

	private:
	
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> bloomfinal;
		std::unique_ptr<Shader> upsampleShader;
		std::unique_ptr<Shader> downsampleShader;
		std::unique_ptr<Shader> brightpassShader;
		std::unique_ptr<Texture> mDirtMask;
		std::unique_ptr<RenderTarget> mBloomFBO;
		std::vector<std::shared_ptr<BloomMipMap>>  BloomMipMaps;

	};

}

#endif