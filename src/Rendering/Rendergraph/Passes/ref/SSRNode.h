#pragma once
#ifndef SSRNODE_H
#define SSRNODE_H

#include "../RenderNode.h"


namespace Fracture
{
	struct SSRParams
	{
		float step = 0.1f;
		float minRayStep = 0.1f;
		float maxSteps = 30.0f;
		int numBinarySearchSteps = 5;
		float reflectionSpecularFalloffExponent = 3.0f;
		float strength = 0.1f;
		glm::vec2 _pad;
	};


	struct SSRNode : public RenderNode
	{
	public:
		SSRNode(DeviceContext& context, const  std::string& name,
			const std::vector<std::string>& inputs,
			const std::vector<std::string>& outputs);
		~SSRNode();

		void Setup();
		void Run(Scene& scene, CameraComponent* camera);
		void ReloadShader();

		glm::vec2 Resolution;
		SSRParams Properties;

	private:
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> combineshader;
		std::unique_ptr<Buffer> mParamsBuffer;
		std::unique_ptr<Texture> mSSRTexture;
	};
}

#endif