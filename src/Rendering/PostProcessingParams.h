#pragma once
#ifndef POSTPROCESSINGPARAMS_H
#define POSTPROCESSINGPARAMS_H


namespace Fracture
{

	struct GlobalPostProcessParams
	{
		//SSAO Parmas
		int SSAO_Enabled = 1;
		float SSAO_Base = 0.2f;
		float SSAO_Bias = 0.025f;
		float SSAO_Radius = 0.5f;
		float SSAO_Falloff = 0.00001f;
		float SSAO_Area = 0.0001f;
		int SSAO_BlurSize = 10;
		int SSAO_NoiseSize = 16;
		int SSAO_NoSamples = 4;
		float SSAO_Strength = 1.0f;
		glm::vec2 SSAO_Resolution = { 1920,1080};
		int ShadowCascadeCont;
		float ShadowBias = 0.01f;
		glm::vec2 pad = glm::vec2(0);
	};





}

#endif