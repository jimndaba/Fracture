#pragma once
#ifndef SSAOPASS_H
#define SSAOPASS_H


#include "IPass.h"

namespace Fracture
{

	struct SSAOPass : public IPass
	{
		SSAOPass(const std::string& name, RenderContext* context);

		void Setup();
		void Execute();


		std::vector<glm::vec3> ssaoNoise;
		std::uniform_real_distribution<float> randomFloats; // random floats between [0.0, 1.0]
		std::default_random_engine generator;
		std::vector<glm::vec4> ssaoKernel;
		

		uint32_t Vao;
		std::shared_ptr<Buffer>  mKernelBuffer;
		std::shared_ptr<Shader>  mSSAO_Shader;
		std::shared_ptr<Shader>  mBlur_Shader;
	private:

		float lerp(float a, float b, float f);

	};



}
#endif