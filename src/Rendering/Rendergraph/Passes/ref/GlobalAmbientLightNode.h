#pragma once
#ifndef IBLNODE_H
#define IBLNODE_H

#include "../RenderNode.h"

namespace Fracture
{
	struct TextureRegistry;
	struct GlobalAmbientLightNode : public RenderNode
	{
	public:
		GlobalAmbientLightNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		~GlobalAmbientLightNode();

		void Setup();

		void Run(Scene& scene, CameraComponent* camera);
		void ReloadShader();
		void AddHDRTexture(TextureRegistry& reg);
		void ChangeHDRTexture(const UUID& texture_ID);

		void renderCube();

		std::unique_ptr<Shader> EqToCubemapShader;
		std::unique_ptr<Shader> Skybox;
		std::unique_ptr<Shader> IrradianceShader;
		std::unique_ptr<Shader> prefilterShader;
		std::unique_ptr<Shader> BrdfShader;
		std::unique_ptr<Texture> CubeMapTexture;
		std::unique_ptr<Texture> BrdfTexture;
		std::unique_ptr<RenderTarget> CaptureFBO;
		bool IsDirty;

		float Blur = 1.2;

		unsigned int cubeVAO = 0;
		unsigned int cubeVBO = 0;
		UUID hdrtexture;
	};


}

#endif